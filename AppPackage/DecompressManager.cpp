#include "StdAfx.h"
#include "DecompressManager.h"
#include "file/FileInfo.h"

#include "json/json.h"

#include "update_method/UpdateMethodFactory.h"
#include "update_method/UpdateUnit.h"
#include "SymbolDefine.h"


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

	if (sourceFileName == NULL) {
		self->decompressDelegate_->decompressError(SOURCE_FILE_PATH_ERROR);
		return 0;
	}

	if (targetDir == NULL) {
		self->decompressDelegate_->decompressError(TARGET_DIR_PATH_ERROR);
		return 0;
	}

	if (!self->openZipFile_()) {
		self->decompressDelegate_->decompressError(OPEN_ZIP_FILE_ERROR); 
		return 0;
	}

	if (!self->selectMethod_()) {
		self->decompressDelegate_->decompressError(JSON_FILE_PARSE_ERROR); 
		return 0;
	}

	AbstractUpdateMethod* updater = UpdateMethodFactory::createUpdateMethod(self->updateMethodName_);
	updater->setZipHandle(self->zipHandle_);
	updater->setDelegate(self->decompressDelegate_);
	DECOMPRESS_ERROR prepareResult = updater->prepare();

	if (prepareResult != UPDATE_PACKAGE_SUCCESS) {
		self->decompressDelegate_->decompressError(prepareResult); 
		CloseZip(self->zipHandle_);
		return 0;
	}

	int numitems = updater->getUpdateFileNumber();
	self->decompressDelegate_->updateZipInfo(numitems);
	
	DECOMPRESS_ERROR updateResult = updater->copyUpdateFile();

	if (updateResult != UPDATE_PACKAGE_SUCCESS) {
		self->decompressDelegate_->decompressError(updateResult); 
		CloseZip(self->zipHandle_);
		return 0;
	}

	if (self->checkFlag_) {
		if (updater->checkMD5() != UPDATE_PACKAGE_SUCCESS) {
			self->decompressDelegate_->decompressError(MD5_CHECK_ERROR);
		}
	}

	if (strcmp(self->updateMethodName_, "COMPARE") == 0) {
		UpdateUnit::releaseJsonBuffer();
	}
	delete updater;
	self->decompressDelegate_->decompressFinish();

	return 0;
}






bool DecompressManager::openZipFile_()
{
	TCHAR* zipFileName = FileInfo::getInstance().getSourceFileName();
	zipHandle_ = OpenZip(zipFileName,0);
	return (zipHandle_ == NULL) ? false : true;
}

bool DecompressManager::selectMethod_()
{
	ZIPENTRY ze;
	int index = 0;
	ZRESULT result = FindZipItem(zipHandle_,_T("F33APP/AppMd5.json"),true,&index,&ze); 
	if (result == ZR_NOTFOUND) {  
		CloseZip(zipHandle_);  
		return false;
	}
	char* jsonBuffer = new char[ze.unc_size+1];  
	memset(jsonBuffer,0,ze.unc_size+1);  
	UnzipItem(zipHandle_,index, jsonBuffer, ze.unc_size);  

	Json::Reader reader;
	Json::Value jsonObject;


	if(reader.parse(jsonBuffer, jsonObject) == false) {
		delete[] jsonBuffer;
		decompressDelegate_->decompressError(JSON_FILE_PARSE_ERROR);
		return false;
	}

	Json::Value updateMethod =  jsonObject[UPDATE_METHOD];
	if (updateMethod.isNull()) {
		//compatibility with older versions
		strcpy(updateMethodName_, "ALL");
	} else {
		strcpy(updateMethodName_, jsonObject[UPDATE_METHOD].asCString());
	}
	
	if (strlen(updateMethodName_) == 0) {
		return false;
	}

	if (strcmp(updateMethodName_, "COMPARE") == 0) {
		UpdateUnit::setJsonBuffer(ze.unc_size+1, jsonBuffer);
	}
	delete[] jsonBuffer;
//	AbstractUpdateMethod* updateMethod = UpdateMethodFactory::createUpdateMethod(updateMethodName_);
	
	return true;
}