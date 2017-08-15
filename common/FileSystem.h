#pragma once


class FileSystem
{
public:
	static FileSystem* GetInstance();

	virtual char* LoadFile(const char* name, size_t* size) = 0;
	virtual bool SaveFile(const char* name, const void* data, size_t size) = 0;

protected:
	FileSystem() {}
	virtual ~FileSystem() {}
};
