#include "StdAfx.h"
#include "DecompressManager.h"
#include "file/FileInfo.h"
#include "zip_util\unzip.h"
#include "json/json.h"
#include "md5/md5.h"

#define MD5_KEY_MAX_LENGTH							64
DecompressManager::DecompressManager(void) : checkFlag_(false)
{

}

DecompressManager::~DecompressManager(void)
{
	
}

bool DecompressManager::decompressWithDelegate(DecompressDelegate* dele, bool checkFlag)
{
	if (dele == NULL)
	{
		return false;
	}

	decompressDelegate_ = dele;
	checkFlag_ = checkFlag;
	if (CreateThread(NULL, 0, deCompressProc_, this, NULL, NULL) == NULL)
	{
		decompressDelegate_->decompressError(CREATE_THREAD_ERROR);
		return false;
	}
	return true;
}

DWORD DecompressManager::deCompressProc_(LPVOID lpParameter)
{

	DecompressManager* self = (DecompressManager*)lpParameter;

	TCHAR* sourceFileName = FileInfo::getInstance().getSourceFileName();
	TCHAR* targetDir = FileInfo::getInstance().getTargetFileName();

	if (sourceFileName == NULL)
	{
		self->decompressDelegate_->decompressError(SOURCE_FILE_PATH_ERROR);
		return 0;
	}

	if (targetDir == NULL)
	{
		self->decompressDelegate_->decompressError(TARGET_DIR_PATH_ERROR);
		return 0;
	}



	HZIP hz = OpenZip(sourceFileName,0);
	if (hz == NULL)
	{
		self->decompressDelegate_->decompressError(OPEN_ZIP_FILE_ERROR);
		return 0;
	}

	TCHAR appDir[MAX_PATH];
	wcscpy(appDir, targetDir);
	wcscat(appDir, L"F33APP\\");
	self->deleteDirectory_(appDir);

	ZIPENTRY ze;
	if (GetZipItem(hz,-1,&ze) != ZR_OK)
	{
		self->decompressDelegate_->decompressError(GET_ZIP_HEAD_ERROR);
		return 0;
	}

	DWORD numitems=ze.index;
	long unzipSize = ze.unc_size;

	for (int zi=0; zi<numitems; zi++)
	{
		
		ZIPENTRY ze;
		if (GetZipItem(hz,zi,&ze) != ZR_OK) // fetch individual details
		{
			self->decompressDelegate_->decompressError(GET_ZIP_ITEM_ERROR);
			return 0;
		}
		unzipSize += ze.unc_size;
	}

	ULARGE_INTEGER freeBytesAvailableToCaller;
    ULARGE_INTEGER totalNumberOfBytes;
    ULARGE_INTEGER totalNumberOfFreeBytes;

#define RESERVED_SPACE	1024*1024
	if (GetDiskFreeSpaceEx(targetDir, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes))
	{
		if (freeBytesAvailableToCaller.QuadPart < (unzipSize + RESERVED_SPACE))
		{
			self->decompressDelegate_->decompressError(NO_ENOUGH_SPACE_ERROR);
			CloseZip(hz);
			return 0;
		}
	}
	else
	{
		self->decompressDelegate_->decompressError(NO_ENOUGH_SPACE_ERROR);
		CloseZip(hz);
		return 0;
	}

	self->decompressDelegate_->updateZipInfo(numitems);

	for (int zi=0; zi<numitems; zi++)
	{
		
		ZIPENTRY ze;
		if (GetZipItem(hz,zi,&ze) != ZR_OK) // fetch individual details
		{
			self->decompressDelegate_->decompressError(GET_ZIP_ITEM_ERROR);
			return 0;
		}
		
		TCHAR tempPath[MAX_PATH] = {0};
		wcscpy(tempPath, targetDir);
		wcscat(tempPath, ze.name);
		self->replace_(tempPath);
		
		DWORD unzipResult = UnzipItem(hz, zi, tempPath);
	    if (unzipResult != ZR_OK)         // e.g. the item's name.
		{
			self->decompressDelegate_->decompressError(UNZIP_PROC_ERROR);
			return 0;
		}
		self->decompressDelegate_->updateStep(ze.name);
		int lastPostion = wcslen(ze.name)-1;
		if ((ze.name[lastPostion] != L'\\') && (ze.name[lastPostion] != L'/'))
		{
			//处理一下,使MAP的KEY和JSON的KEY一致
			TCHAR keyName[MAX_PATH] = {0};
			wcscpy(keyName, ze.name);
			self->replace_(keyName);
			self->fileContainer_[keyName] = tempPath;
		}
	}
	CloseZip(hz);
	if (self->checkFlag_)
	{
		self->checkMd5();
	}
	
	self->decompressDelegate_->decompressFinish();
	return 0;
}

void DecompressManager::replace_(TCHAR* source)
{
	if (source == NULL)
	{
		return ;
	}

	int sourceLength = wcslen(source);

	for (int i = 0; i < sourceLength; ++i)
	{
		if (source[i] == L'/')
		{
			source[i] = L'\\';
		}
	}
}

void DecompressManager::deleteDirectory_(TCHAR* dirName)  
{  
	TCHAR targetDir[MAX_PATH] = {0};
	wcscpy(targetDir, dirName);

	if(targetDir[wcslen(targetDir) - 1] == TEXT('\\'))  
    {  
        wcscat(targetDir,  TEXT("*.*"));
    }  
    else  
    {  
       wcscat(targetDir,  TEXT("\\*.*"));
	}
  
    WIN32_FIND_DATA fd;  
    HANDLE hdFind;  
    hdFind = FindFirstFile(targetDir,&fd);  
    if(hdFind != INVALID_HANDLE_VALUE)  
    {  
        do{  
            //At winXP system the "." means current directory, the ".."means parent directory.  
            if(fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && fd.cFileName[0] != TEXT('.'))  
            {         
                //It is directory     
				if (wcscmp(fd.cFileName, L"Config") != 0)
				{
					TCHAR nextDir[MAX_PATH] = {0};
					wcscpy(nextDir, dirName);  
					if(nextDir[wcslen(nextDir) -1] != TEXT('\\'))  
					{
						wcscat(nextDir, TEXT("\\"));
					}
					wcscat(nextDir, fd.cFileName);
	                
					deleteDirectory_(nextDir);  
					RemoveDirectory(nextDir);  
				}

            }  
            else if(fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)  
            {     
                //It is file   
				TCHAR filePath[MAX_PATH] = {0};
				wcscpy(filePath, dirName);
                if(filePath[wcslen(filePath) - 1] !=TEXT( '\\'))  
				{
					wcscat(filePath, TEXT("\\"));
				}
				wcscat(filePath,fd.cFileName);
                DeleteFile(filePath);  
            }  
        }while(FindNextFile(hdFind,&fd));  
    }  
    FindClose(hdFind);  
 
}  

void DecompressManager::checkMd5()
{
	TCHAR* targetDir = FileInfo::getInstance().getTargetFileName();
	TCHAR jsonFileName[MAX_PATH] = {0};
	wcscpy(jsonFileName, targetDir);
	wcscat(jsonFileName, L"F33APP\\AppMd5.json");
	HANDLE jsonFile = CreateFile(jsonFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (jsonFile == INVALID_HANDLE_VALUE)
	{
		DWORD hr = GetLastError();
		return ;
	}

	DWORD fileSize = GetFileSize(jsonFile, NULL);
	if (fileSize == -1)
	{
		return ;
	}
	char* jsonDocument = new char[fileSize];

	if (jsonDocument == NULL)
	{
		return ;
	}
	DWORD readSize = 0;
	
	if (!ReadFile(jsonFile, jsonDocument, fileSize, &readSize, NULL))
	{
		delete[] jsonDocument;
		return ;
	}

	Json::Reader reader;
	Json::Value jsonObject;
	if(reader.parse(jsonDocument, jsonObject) == false)
	{
		delete[] jsonDocument;
		decompressDelegate_->decompressError(JSON_FILE_PARSE_ERROR);
		return ;
	}

	Json::Value::Members member;
	member = jsonObject.getMemberNames();

	for (Json::Value::Members::iterator iter = member.begin(); iter != member.end(); iter++)
	{
		TCHAR relativePath[MAX_PATH] = {0};
		MultiByteToWideChar(GetACP(), 0, (*iter).c_str(), (*iter).size()*2, relativePath, MAX_PATH);
		FILE * fp = NULL;
		map<wstring, wstring >::iterator it; 
		it=fileContainer_.find(relativePath);
		if(it==fileContainer_.end())
		{
			//没有MD5值先忽略
			continue;
		}

		wstring absolutePath((*it).second);
		if (absolutePath.find(L"AppMd5.json") != absolutePath.npos)
		{
			//MD5文件跳过
			continue;
		}

		_wfopen_s(&fp, (*it).second.c_str(), L"rb");

		if (fp == NULL)
		{
			continue ;
		}
		MD5VAL val=md5File(fp);
		char md5_result[MD5_KEY_MAX_LENGTH] = {0};
		sprintf(md5_result, "%08x%08x%08x%08x",conv_(val.a),conv_(val.b),conv_(val.c),conv_(val.d));
		fclose(fp);
		string origString = jsonObject[(*iter)].asString();
		if (origString != md5_result)
		{
			delete[] jsonDocument;
			decompressDelegate_->decompressError(MD5_CHECK_ERROR);
			return ;
		}
		
	}
	delete[] jsonDocument;
}

unsigned int DecompressManager::conv_(unsigned int a)
{
	unsigned int b=0;
	b|=(a<<24)&0xff000000;
	b|=(a<<8)&0x00ff0000;
	b|=(a>>8)&0x0000ff00;
	b|=(a>>24)&0x000000ff;
	return b;
}
