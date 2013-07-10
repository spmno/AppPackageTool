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
	//��ѹ����,����delegate,����Ϣ��ص�delegate
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
