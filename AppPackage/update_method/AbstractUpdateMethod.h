#pragma once
#include <windows.h>
#include <map>
#include <string>
#include "../zip_util/unzip.h"
#include "../DecompressDelegate.h"

using namespace std;

class AbstractUpdateMethod
{
public:
	enum CheckSpaceMethod {
		ALL,
		DIFF,
	};

	AbstractUpdateMethod(void);
	~AbstractUpdateMethod(void);
	void setZipHandle(HZIP zipHandle)
	{
		zipHandle_ = zipHandle;
	}
	void setDelegate(DecompressDelegate* dele)
	{
		delegate_ = dele;
	}
	int getUpdateFileNumber()
	{
		return updateFileNumber_;
	}
	virtual DECOMPRESS_ERROR prepare() = 0;
	virtual DECOMPRESS_ERROR copyUpdateFile() = 0;
	DECOMPRESS_ERROR checkMD5();
	DECOMPRESS_ERROR checkFreeSpace(CheckSpaceMethod method);
protected:
	HZIP zipHandle_;
	int updateFileNumber_;
	map<wstring, wstring > fileContainer_;
	DecompressDelegate* delegate_;
};
