#pragma once


#include "FileSystem.h"


class FileSystemPC : public FileSystem
{
public:
	inline static FileSystemPC* GetInstance()
	{
		static FileSystemPC instance;
		return &instance;
	}

	char* LoadFile(const char* name, size_t* size) override;
	bool SaveFile(const char* name, const void* data, size_t size) override;

private:
	FileSystemPC();
	~FileSystemPC();
};
