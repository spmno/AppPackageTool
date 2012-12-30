#pragma once
#include "DecompressDelegate.h"
#include <map>
#include <string>

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
	void replace_(TCHAR* source);
	void deleteDirectory_(TCHAR* dirName);  
	void checkMd5();
	unsigned conv_(unsigned int a);
	DecompressDelegate* decompressDelegate_;
	map<wstring, wstring > fileContainer_;
	bool checkFlag_;
};
