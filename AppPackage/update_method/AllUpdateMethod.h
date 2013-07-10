#pragma once
#include "AbstractUpdateMethod.h"

class AllUpdateMethod : public AbstractUpdateMethod
{
public:
	AllUpdateMethod(void);
	~AllUpdateMethod(void);
	
	virtual DECOMPRESS_ERROR prepare();
	virtual DECOMPRESS_ERROR copyUpdateFile();
	DECOMPRESS_ERROR checkMd5();
private:
	void deleteDirectory_(TCHAR* dirName); 

	

};
