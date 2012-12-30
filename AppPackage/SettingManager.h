#pragma once

#include "./xml/MarkUp.h"

class SettingManager
{
public:
	SettingManager(void);
	~SettingManager(void);
private:
	CMarkup settingConfig;
};
