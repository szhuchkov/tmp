#pragma once


//-----------------------------------------------------------------------------
//
//	utility definitions
//
//-----------------------------------------------------------------------------

#define ARRAY_SIZE(x)		(sizeof(x) / sizeof((x)[0]))


//-----------------------------------------------------------------------------
//
//	strings
//
//-----------------------------------------------------------------------------
const char* GetLine(const char* context, char* buffer);
const char* GetLine(const char* context, std::string& buffer);


//-----------------------------------------------------------------------------
//
//	logger
//
//-----------------------------------------------------------------------------

void _LogPrintf(const char* file, int line, const char* format, ...);
#define LogPrintf(...)		_LogPrintf(__FILE__, __LINE__, __VA_ARGS__)


//-----------------------------------------------------------------------------
//
//	file utils
//
//-----------------------------------------------------------------------------

char* FS_LoadFile(const char* name, size_t* size);
bool FS_SaveFile(const char* name, const void* data, size_t size);
bool FS_DeleteFile(const char* name);


//-----------------------------------------------------------------------------
//
//	directory utils
//
//-----------------------------------------------------------------------------

bool FS_CreateDir(const char* name);
bool FS_DeleteDir(const char* name);
bool FS_ChangeDir(const char* name);
const char* FS_GetCurDir();
const char* FS_GetRootDir();


//-----------------------------------------------------------------------------
//
//	console vars
//
//-----------------------------------------------------------------------------

void CVar_Set(const char* name, int value);
void CVar_Set(const char* name, float value);
void CVar_Set(const char* name, const char* value);
void CVar_Clear();


template<typename _T>
_T CVar_Get(const char* name);
