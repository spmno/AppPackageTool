#pragma once
#include "../stdafx.h"
#include "../xml/MarkUp.h"
class FileInfo
{
public:
	static FileInfo & getInstance()
	{
		static FileInfo info;
		return info;
	}
	~FileInfo(void);
#ifdef USE_MFC
	CString& getSourceFileName()
	{
		return sourceFileName;
	}

	CString& getTargetFileName()
	{
		return targetDirName;
	}
#else
	TCHAR* getSourceFileName()
	{
		return sourceFileName;
	}

	TCHAR* getTargetFileName()
	{
		return targetDirName;
	}
#endif

private:
#ifdef USE_MFC
	CMarkup xmlParser;
	CString configFileName;
	CString sourceFileName;
	CString targetDirName;
	void makeConfigName();
	bool loadConfigFile();
	void loadSourceFileName();
	void loadTargetDirName();	
#else
	TCHAR sourceFileName[MAX_PATH];
	TCHAR targetDirName[MAX_PATH];
#endif
	FileInfo(void);
};
