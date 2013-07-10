#include "UpdateManager.h"
#include "UpdateMethodFactory.h"

UpdateManager::UpdateManager(void)
{
}

UpdateManager::~UpdateManager(void)
{
}

DECOMPRESS_ERROR UpdateManager::update()
{
	if (!openZipFile()_) {
		return OPEN_ZIP_FILE_ERROR;
	}

	if (!selectMethod()_) {
		return UpdateMethodError;
	}

	AbstractUpdateMethod* updater = createUpdateMethod(updateMethodName_);

	updater->prepare();

	if (!updater->copyUpdateFile()) {
		return UpdateCopyError;
	}

	return UPDATE_PACKAGE_SUCCESS;
}


bool UpdateManager::openZipFile_()
{
	TCHAR* zipFileName = FileInfo::getInstance().getSourceFileName();
	zipHandle_ = OpenZip(zipFileName,0);
	return (zipHandle_ == NULL) ? false : true;
}

bool UpdateManager::selectMethod_()
{

}