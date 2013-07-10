#include "AllUpdateMethod.h"
#include "../File/FileInfo.h"
#include "../SymbolDefine.h"
#include "../md5/md5.h"
#include "json/json.h"
#include "UpdateUnit.h"

AllUpdateMethod::AllUpdateMethod(void)
{
}

AllUpdateMethod::~AllUpdateMethod(void)
{
}


DECOMPRESS_ERROR AllUpdateMethod::prepare()
{
	TCHAR appDir[MAX_PATH];
	TCHAR* targetDir = FileInfo::getInstance().getTargetFileName();
	wcscpy(appDir, targetDir);
	wcscat(appDir, L"F33APP\\");
	deleteDirectory_(appDir);

	ZIPENTRY ze;
	if (GetZipItem(zipHandle_,-1,&ze) != ZR_OK) {
		return GET_ZIP_HEAD_ERROR;
	}

	DWORD numitems=ze.index;
	updateFileNumber_ = numitems;
	long unzipSize = ze.unc_size;
	
	for (int zi=0; zi<numitems; zi++)
	{
		
		ZIPENTRY ze;
		// fetch individual details
		if (GetZipItem(zipHandle_,zi,&ze) != ZR_OK) {
			return GET_ZIP_ITEM_ERROR;
		}
		unzipSize += ze.unc_size;
	}

	ULARGE_INTEGER freeBytesAvailableToCaller;
    ULARGE_INTEGER totalNumberOfBytes;
    ULARGE_INTEGER totalNumberOfFreeBytes;

#define RESERVED_SPACE	1024*1024
	if (GetDiskFreeSpaceEx(targetDir, &freeBytesAvailableToCaller, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
		if (freeBytesAvailableToCaller.QuadPart < (unzipSize + RESERVED_SPACE))	{
			
			return NO_ENOUGH_SPACE_ERROR;
		}
	} else {
		return NO_ENOUGH_SPACE_ERROR;
	}
	return UPDATE_PACKAGE_SUCCESS;
}

DECOMPRESS_ERROR AllUpdateMethod::copyUpdateFile()
{
	TCHAR* targetDir = FileInfo::getInstance().getTargetFileName();
	for (int zi=0; zi<updateFileNumber_; zi++)
	{
		
		ZIPENTRY ze;
		// fetch individual details
		if (GetZipItem(zipHandle_,zi,&ze) != ZR_OK) {
			return GET_ZIP_ITEM_ERROR;
		}
		
		TCHAR tempPath[MAX_PATH] = {0};
		wcscpy(tempPath, targetDir);
		wcscat(tempPath, ze.name);
		UpdateUnit::replace(tempPath);
		
		DWORD unzipResult = UnzipItem(zipHandle_, zi, tempPath);
	    if (unzipResult != ZR_OK) {
			return UNZIP_PROC_ERROR;
		}
		delegate_->updateStep(ze.name);
		int lastPostion = wcslen(ze.name)-1;
		if ((ze.name[lastPostion] != L'\\') && (ze.name[lastPostion] != L'/'))
		{
			//处理一下,使MAP的KEY和JSON的KEY一致
			TCHAR keyName[MAX_PATH] = {0};
			wcscpy(keyName, ze.name);
			UpdateUnit::replace(keyName);
			fileContainer_[keyName] = tempPath;
		}
	}
	CloseZip(zipHandle_);
	zipHandle_ = NULL;
	return UPDATE_PACKAGE_SUCCESS;
}

void AllUpdateMethod::deleteDirectory_(TCHAR* dirName)  
{  
	TCHAR targetDir[MAX_PATH] = {0};
	wcscpy(targetDir, dirName);

	if(targetDir[wcslen(targetDir) - 1] == TEXT('\\')) {  
		wcscat(targetDir,  TEXT("*.*"));
    } else {  
		wcscat(targetDir,  TEXT("\\*.*"));
	}
  
    WIN32_FIND_DATA fd;  
    HANDLE hdFind;  
    hdFind = FindFirstFile(targetDir,&fd);  
    if(hdFind != INVALID_HANDLE_VALUE) {  
        do{  
            //At winXP system the "." means current directory, the ".."means parent directory.  
            if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (fd.cFileName[0] != TEXT('.'))) {         
                //It is directory     
				if (wcscmp(fd.cFileName, L"Config") != 0) {
					TCHAR nextDir[MAX_PATH] = {0};
					wcscpy(nextDir, dirName);  
					if(nextDir[wcslen(nextDir) -1] != TEXT('\\')) {
						wcscat(nextDir, TEXT("\\"));
					}
					wcscat(nextDir, fd.cFileName);
	                
					deleteDirectory_(nextDir);  
					RemoveDirectory(nextDir);  
				}

            } else if(fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) {     
                //It is file   
				TCHAR filePath[MAX_PATH] = {0};
				wcscpy(filePath, dirName);
                if(filePath[wcslen(filePath) - 1] !=TEXT( '\\')) {
					wcscat(filePath, TEXT("\\"));
				}
				wcscat(filePath,fd.cFileName);
                DeleteFile(filePath);  
            }  
        }while(FindNextFile(hdFind,&fd));  
    }  
    FindClose(hdFind);  
 
}  


	

