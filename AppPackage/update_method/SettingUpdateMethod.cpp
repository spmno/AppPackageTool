#include "SettingUpdateMethod.h"
#include "../File/FileInfo.h"
#include "json/json.h"
#include "UpdateUnit.h"

SettingUpdateMethod::SettingUpdateMethod(void)
{
}

SettingUpdateMethod::~SettingUpdateMethod(void)
{

}

DECOMPRESS_ERROR SettingUpdateMethod::prepare()
{
	return checkFreeSpace(AbstractUpdateMethod::DIFF);
}

DECOMPRESS_ERROR SettingUpdateMethod::copyUpdateFile()
{
	TCHAR* targetDir = FileInfo::getInstance().getTargetFileName();
	for (int zi=0; zi<updateFileNumber_; zi++)  {
		
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
		if ((ze.name[lastPostion] != L'\\') && (ze.name[lastPostion] != L'/')) {
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