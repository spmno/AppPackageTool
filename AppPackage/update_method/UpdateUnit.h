#pragma once

class UpdateUnit
{
public:
	UpdateUnit(void);
	~UpdateUnit(void);
	static void replace(TCHAR* source);
	static void replaceBack(TCHAR* source);
	static bool setJsonBuffer(int size, char* buffer);
	static void releaseJsonBuffer();
	static char* getJsonBuffer()
	{
		return jsonBuffer;
	}
	static unsigned conv_(unsigned int a);
private:
	static char* jsonBuffer;
};
