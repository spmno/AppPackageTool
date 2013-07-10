#pragma once
#include <windows.h>
#include "../DecompressDelegate.h"
class UpdateManager
{
public:
	UpdateManager(void);
	~UpdateManager(void);
	DECOMPRESS_ERROR update();
private:

};
