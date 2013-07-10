#pragma once
#include "DecompressDelegate.h"
#include <map>
#include <string>

#include "zip_util\unzip.h"

using namespace std;

class DecompressManager
{
public:
	DecompressManager(void);
	~DecompressManager(void);
	//解压函数,输入delegate,有信息会回调delegate
	bool decompressWithDelegate(DecompressDelegate* dele, bool checkFlag);
private:
	static DWORD deCompressProc_(LPVOID lpParameter);
	
	bool openZipFile_();
	bool selectMethod_();
	


	DecompressDelegate* decompressDelegate_;
	
	bool checkFlag_;
	HZIP zipHandle_;

	char updateMethodName_[64];
};
