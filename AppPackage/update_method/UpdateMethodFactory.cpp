#include "UpdateMethodFactory.h"
#include "AllUpdateMethod.h"
#include "CompareUpdateMethod.h"
#include "SettingUpdateMethod.h"

const std::string SETTING_FILE_UPDATE_METHOD("SETTING");
const std::string COMPARE_FILE_UPDATE_METHOD("COMPARE");
const std::string ALL_REPLACE_UPDATE_METHOD("ALL");

UpdateMethodFactory::UpdateMethodFactory(void)
{
}

UpdateMethodFactory::~UpdateMethodFactory(void)
{
}

AbstractUpdateMethod* UpdateMethodFactory::createUpdateMethod(char* updateName)
{
	if (SETTING_FILE_UPDATE_METHOD == updateName) {
		return new SettingUpdateMethod;
	} else if (COMPARE_FILE_UPDATE_METHOD == updateName) {
		return new CompareUpdateMethod;
	}
	return new AllUpdateMethod;
}
