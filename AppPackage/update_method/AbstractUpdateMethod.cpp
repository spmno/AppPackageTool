#include "AbstractUpdateMethod.h"
#include "../File/FileInfo.h"
#include "../md5/md5.h"
#include "json/json.h"
#include "../SymbolDefine.h"
#include "UpdateUnit.h"

AbstractUpdateMethod::AbstractUpdateMethod(void)
{

}

AbstractUpdateMethod::~AbstractUpdateMethod(void)
{

}

DECOMPRESS_ERROR AbstractUpdateMethod::checkMD5()
{
	
	TCHAR* targetDir = FileInfo::getInstance().getTargetFileName();
	TCHAR jsonFileName[MAX_PATH] = {0};
	wcscpy(jsonFileName, targetDir);
	wcscat(jsonFileName, L"F33APP\\AppMd5.json");
	HANDLE jsonFile = CreateFile(jsonFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (jsonFile == INVALID_HANDLE_VALUE) {
		DWORD hr = GetLastError();
		return MD5_CHECK_ERROR;
	}

	DWORD fileSize = GetFileSize(jsonFile, NULL);
	if (fileSize == -1) {
		CloseHandle(jsonFile);
		return MD5_CHECK_ERROR;
	}
	char* jsonDocument = new char[fileSize];

	if (jsonDocument == NULL) {
		CloseHandle(jsonFile);
		return MD5_CHECK_ERROR;
	}
	DWORD readSize = 0;
	
	if (!ReadFile(jsonFile, jsonDocument, fileSize, &readSize, NULL)) {
		delete[] jsonDocument;
		CloseHandle(jsonFile);
		return MD5_CHECK_ERROR;
	}

	Json::Reader reader;
	Json::Value jsonObject;
	Json::Value md5Value;

	if(reader.parse(jsonDocument, jsonObject) == false)	{
		delete[] jsonDocument;
		CloseHandle(jsonFile);
		//decompressDelegate_->decompressError(JSON_FILE_PARSE_ERROR);
		return MD5_CHECK_ERROR;
	}

	md5Value = jsonObject[MD5_CONTENT];
	Json::Value::Members member;
	member = md5Value.getMemberNames();

	for (Json::Value::Members::iterator iter = member.begin(); iter != member.end(); iter++) {
		TCHAR relativePath[MAX_PATH] = {0};
		MultiByteToWideChar(GetACP(), 0, (*iter).c_str(), (*iter).size()*2, relativePath, MAX_PATH);
		FILE * fp = NULL;
		map<wstring, wstring >::iterator it; 
		it=fileContainer_.find(relativePath);
		if(it==fileContainer_.end()) {
			//没有MD5值先忽略
			continue;
		}

		wstring absolutePath((*it).second);
		if (absolutePath.find(L"AppMd5.json") != absolutePath.npos) {
			//MD5文件跳过
			continue;
		}

		_wfopen_s(&fp, (*it).second.c_str(), L"rb");

		if (fp == NULL) {
			continue ;
		}
		MD5VAL val=md5File(fp);
		char md5_result[MD5_KEY_MAX_LENGTH] = {0};
		sprintf(md5_result, "%08x%08x%08x%08x",UpdateUnit::conv_(val.a),UpdateUnit::conv_(val.b),UpdateUnit::conv_(val.c),UpdateUnit::conv_(val.d));
		fclose(fp);
		string origString = md5Value[(*iter)].asString();
		if (origString != md5_result) {
			delete[] jsonDocument;
			CloseHandle(jsonFile);
			//decompressDelegate_->decompressError(MD5_CHECK_ERROR);
			return MD5_CHECK_ERROR;
		}
		
	}
	delete[] jsonDocument;
	CloseHandle(jsonFile);
	return UPDATE_PACKAGE_SUCCESS;
}