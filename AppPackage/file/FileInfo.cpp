#include "FileInfo.h"

FileInfo::FileInfo(void)
{
#ifdef USE_MFC
	makeConfigName();
	if(loadConfigFile())
	{
		loadSourceFileName();
		loadTargetDirName();
	}
	else
	{
		sourceFileName = L"\\CardC\\F33APP.zip";
		targetDirName = L"\\NandFlash\\";
	}
#else
	wcscpy(sourceFileName, L"\\CardC\\F33APP.zip");
	wcscpy(targetDirName,  L"\\NandFlash\\");
	//wcscpy(sourceFileName, L"\\Storage Card\\F33APP.zip");
	//wcscpy(targetDirName,  L"\\Storage Card\\");
#endif
}

FileInfo::~FileInfo(void)
{
}

#ifdef USE_MFC
void FileInfo::makeConfigName()
{
	TCHAR currentFileName[MAX_PATH];
	GetModuleFileName(NULL, currentFileName, MAX_PATH);
	configFileName = currentFileName;
	int lastSignPos = configFileName.ReverseFind('\\');
	if (lastSignPos == -1)
	{
		MessageBox(NULL, L"path error", NULL, MB_OK|MB_TOPMOST);
		return ;
	}
	configFileName = configFileName.Left(lastSignPos+1);
	configFileName += L"AppConfig.xml";
}

bool FileInfo::loadConfigFile()
{
	if (xmlParser.Load(configFileName.GetBuffer()) == false)
	{
		return false;
	}
	return true;
}

void FileInfo::loadSourceFileName()
{
	bool findMarkFlag = false;
	while(xmlParser.FindChildElem(_T("SourcePackagePath")))
	{
		findMarkFlag = true;
		sourceFileName = xmlParser.GetChildData();
		size_t findPos = 0;
	}

	if (findMarkFlag = false)
	{
		MessageBox(NULL, _T("load source config error"), NULL, MB_OK|MB_TOPMOST);
		ExitProcess(0);
	}
}

void FileInfo::loadTargetDirName()
{
	bool findMarkFlag = false;
	while(xmlParser.FindChildElem(_T("TargetDir")))
	{
		findMarkFlag = true;
		targetDirName = xmlParser.GetChildData();
		size_t findPos = 0;
	}

	if (findMarkFlag = false)
	{
		MessageBox(NULL, _T("load target config error"), NULL, MB_OK|MB_TOPMOST);
		ExitProcess(0);
	}
}
#endif