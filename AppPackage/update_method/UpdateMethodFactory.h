#pragma once
#include <string>
#include "AbstractUpdateMethod.h"

class UpdateMethodFactory
{
public:
	UpdateMethodFactory(void);
	~UpdateMethodFactory(void);
	static AbstractUpdateMethod* createUpdateMethod(char* updateName);
};
