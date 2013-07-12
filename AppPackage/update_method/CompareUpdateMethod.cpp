#include <string>
#include "CompareUpdateMethod.h"
#include "../File/FileInfo.h"

#include "UpdateUnit.h"
#include "../SymbolDefine.h"

CompareUpdateMethod::CompareUpdateMethod(void)
{
}

CompareUpdateMethod::~CompareUpdateMethod(void)
{
}



DECOMPRESS_ERROR CompareUpdateMethod::prepare()
{
	return checkFreeSpace(AbstractUpdateMethod::DIFF);
}

DECOMPRESS_ERROR CompareUpdateMethod::copyUpdateFile()
{
	DECOMPRESS_ERROR result = makeCopyList_();
	if (result != UPDATE_PACKAGE_SUCCESS) {
		return result;
	}

	return copyAllUpdateFile_();
}

DECOMPRESS_ERROR CompareUpdateMethod::makeCopyList_()
{
	//JSON IN ZIP
	char* jsonBuffer = UpdateUnit::getJsonBuffer();
	Json::Reader reader;
	Json::Value jsonObject;
	Json::Value diskJsonObject;
	Json::Value contentObjects;
	Json::Value diskContentObjects;
	if(reader.parse(jsonBuffer, jsonObject) == false) {
		return JSON_FILE_PARSE_ERROR;
	}

	contentObjects = jsonObject[MD5_CONTENT];
	if (contentObjects.empty()) {
		return JSON_FILE_PARSE_ERROR;
	}

	//json in the disk
	Json::Value::Members member;
	member = contentObjects.getMemberNames();
	
	ZIPENTRY ze;
	int index = 0;
	TCHAR zipFileName[MAX_PATH];
	if (getDiskJsonObject_(diskJsonObject)) {

		diskContentObjects = diskJsonObject[MD5_CONTENT];
		for (Json::Value::Members::iterator iter = member.begin(); iter != member.end(); iter++) {
			string zipMD5Value = contentObjects[*iter].asString();
			string diskMD5Value = diskContentObjects[*iter].asString();
			if (zipMD5Value != diskMD5Value) {
				MultiByteToWideChar(GetACP(), 0, (*iter).c_str(), (*iter).size()*2, zipFileName, MAX_PATH);
				UpdateUnit::replaceBack(zipFileName);
				ZRESULT result = FindZipItem(zipHandle_,zipFileName,true,&index,&ze); 
				updateList_.push_back(index);
			}
			
		}
	} else {
		//copy all file in zip
		for (Json::Value::Members::iterator iter = member.begin(); iter != member.end(); iter++) {
			string zipMD5Value = contentObjects[*iter].asString();
			string diskMD5Value = diskContentObjects[*iter].asString();
			MultiByteToWideChar(GetACP(), 0, (*iter).c_str(), (*iter).size()*2, zipFileName, MAX_PATH);
			UpdateUnit::replaceBack(zipFileName);
			ZRESULT result = FindZipItem(zipHandle_,zipFileName,true,&index,&ze); 
			updateList_.push_back(index);
		}
	}
	
	return UPDATE_PACKAGE_SUCCESS;
}

bool CompareUpdateMethod::getDiskJsonObject_(Json::Value &jsonObject)
{
	TCHAR* targetDir = FileInfo::getInstance().getTargetFileName();
	TCHAR jsonFileName[MAX_PATH] = {0};
	wcscpy(jsonFileName, targetDir);
	wcscat(jsonFileName, L"F33APP\\AppMd5.json");
	HANDLE jsonFile = CreateFile(jsonFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (jsonFile == INVALID_HANDLE_VALUE) {
		DWORD hr = GetLastError();
		return false;
	}

	DWORD fileSize = GetFileSize(jsonFile, NULL);
	if (fileSize == -1) {
		CloseHandle(jsonFile);
		return false;
	}
	char* jsonDocument = new char[fileSize];

	if (jsonDocument == NULL) {
		CloseHandle(jsonFile);
		return false;
	}
	DWORD readSize = 0;
	
	if (!ReadFile(jsonFile, jsonDocument, fileSize, &readSize, NULL)) {
		delete[] jsonDocument;
		CloseHandle(jsonFile);
		return false;
	}
	Json::Reader reader;
	if(reader.parse(jsonDocument, jsonObject) == false)
	{
		delete[] jsonDocument;
		CloseHandle(jsonFile);
		return false;
	}
	delete[] jsonDocument;
	CloseHandle(jsonFile);
	return true;
}

DECOMPRESS_ERROR CompareUpdateMethod::copyAllUpdateFile_()
{
	TCHAR* targetDir = FileInfo::getInstance().getTargetFileName();

	for (vector<int>::iterator iter = updateList_.begin(); iter != updateList_.end(); ++iter) {
		ZIPENTRY ze;
		// fetch individual details
		if (GetZipItem(zipHandle_,*iter,&ze) != ZR_OK) {
			return GET_ZIP_ITEM_ERROR;
		}
		
		TCHAR tempPath[MAX_PATH] = {0};
		wcscpy(tempPath, targetDir);
		wcscat(tempPath, ze.name);
		UpdateUnit::replace(tempPath);
		
		DWORD unzipResult = UnzipItem(zipHandle_, *iter, tempPath);
	    if (unzipResult != ZR_OK) {
			return UNZIP_PROC_ERROR;
		}
		delegate_->updateStep(ze.name);
		int lastPostion = wcslen(ze.name)-1;
		if ((ze.name[lastPostion] != L'\\') && (ze.name[lastPostion] != L'/')) {
			//处理一下,使MAP的KEY和JSON的KEY一致
			TCHAR keyName[MAX_PATH] = {0};
			wcscpy(keyName, ze.name);
			UpdateUnit::replace(keyName);
			fileContainer_[keyName] = tempPath;
		}
	}
	return UPDATE_PACKAGE_SUCCESS;
}