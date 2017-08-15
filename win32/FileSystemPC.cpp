#include "pch.h"
#include "FileSystemPC.h"


FileSystem* FileSystem::GetInstance()
{
	return FileSystemPC::GetInstance();
}


FileSystemPC::FileSystemPC()
{
}


FileSystemPC::~FileSystemPC()
{
}


char* FileSystemPC::LoadFile(const char* name, size_t* size)
{
	HANDLE hf = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hf == INVALID_HANDLE_VALUE)
		return false;

	DWORD sz = GetFileSize(hf, NULL);
	char* data = new char[sz + 1];
	ReadFile(hf, data, sz, &sz, NULL);
	CloseHandle(hf);

	if (size)
		*size = sz;

	return data;
}


bool FileSystemPC::SaveFile(const char* name, const void* data, size_t size)
{
	HANDLE hf = CreateFile(name, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if (hf == INVALID_HANDLE_VALUE)
		return false;

	DWORD written;
	WriteFile(hf, data, size, &written, 0);
	CloseHandle(hf);

	if (written != size)
		return false;

	return true;
}