#pragma once


class FileSystem
{
public:
	static FileSystem* GetInstance();

	virtual bool Init() = 0;
	virtual void Shutdown() = 0;

	virtual char* LoadFile(const char* name, size_t* size) = 0;
	virtual bool SaveFile(const char* name, const void* data, size_t size) = 0;
	virtual bool RemoveFile(const char* name) = 0;

	virtual const char* GetRootDir() = 0;
	virtual const char* GetCurDir() = 0;
	virtual bool CreateDir(const char* name) = 0;
	virtual bool ChangeDir(const char* name) = 0;
	virtual bool RemoveDir(const char* name) = 0;

protected:
	FileSystem() {}
	virtual ~FileSystem() {}
};
