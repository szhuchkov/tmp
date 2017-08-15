#include "pch.h"
#include "Utils.h"
#include "FileSystem.h"


char* FS_LoadFile(const char* name, size_t* size)
{
	char* res = FileSystem::GetInstance()->LoadFile(name, size);
	return res;
}


bool FS_SaveFile(const char* name, const void* data, size_t size)
{
	bool res = FileSystem::GetInstance()->SaveFile(name, data, size);
	return res;
}