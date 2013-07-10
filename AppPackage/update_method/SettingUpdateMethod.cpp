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
	TCHAR appDir[MAX_PATH];
	TCHAR* targetDir = FileInfo::getInstance().getTargetFileName();
	wcscpy(appDir, targetDir);
	wcscat(appDir, L"F33APP\\");

	ZIPENTRY ze;
	if (GetZipItem(zipHandle_,-1,&ze) != ZR_OK) {
		return GET_ZIP_HEAD_ERROR;
	}

	DWORD numitems=ze.index;
	updateFileNumber_ = numitems;
	long unzipSize = ze.unc_size;
	
	for (int zi=0; zi<numitems; zi++) {
		
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