#pragma once


void _LogPrintf(const char* file, int line, const char* format, ...);
#define LogPrintf(...)		_LogPrintf(__FILE__, __LINE__, __VA_ARGS__)


char* FS_LoadFile(const char* name, size_t* size);
bool FS_SaveFile(const char* name, const void* data, size_t size);


#define ARRAY_SIZE(x)		(sizeof(x) / sizeof((x)[0]))