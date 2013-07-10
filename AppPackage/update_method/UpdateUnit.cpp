#include "StdAfx.h"
#include "UpdateUnit.h"
char* UpdateUnit::jsonBuffer;
UpdateUnit::UpdateUnit(void)
{
}

UpdateUnit::~UpdateUnit(void)
{
}

void UpdateUnit::replace(TCHAR *source)
{
	if (source == NULL) {
		return ;
	}

	int sourceLength = wcslen(source);

	for (int i = 0; i < sourceLength; ++i) {
		if (source[i] == L'/') {
			source[i] = L'\\';
		}
	}
}

void UpdateUnit::replaceBack(TCHAR *source)
{
	if (source == NULL)	{
		return ;
	}

	int sourceLength = wcslen(source);

	for (int i = 0; i < sourceLength; ++i) {
		if (source[i] == L'\\') {
			source[i] = L'/';
		}
	}
}

bool UpdateUnit::setJsonBuffer(int size, char *buffer)
{
	jsonBuffer = (char*)malloc(size);
	if (jsonBuffer == NULL) {
		return false;	
	}
	memcpy(jsonBuffer, buffer, size);
	return true;
}

void UpdateUnit::releaseJsonBuffer()
{
	if (jsonBuffer != NULL) {
		free(jsonBuffer);
		jsonBuffer = NULL;
	}
}

unsigned int UpdateUnit::conv_(unsigned int a)
{
	unsigned int b=0; 
	b|=(a<<24)&0xff000000;
	b|=(a<<8)&0x00ff0000;
	b|=(a>>8)&0x0000ff00;
	b|=(a>>24)&0x000000ff;
	return b;
}