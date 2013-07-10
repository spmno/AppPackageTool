#pragma once
#include "AbstractUpdateMethod.h"

class SettingUpdateMethod : public AbstractUpdateMethod
{
public:
	SettingUpdateMethod(void);
	~SettingUpdateMethod(void);

	virtual DECOMPRESS_ERROR prepare();
	virtual DECOMPRESS_ERROR copyUpdateFile();
};
