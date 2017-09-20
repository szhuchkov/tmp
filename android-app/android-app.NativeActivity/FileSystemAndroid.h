#pragma once


#include "FileSystem.h"


class FileSystemAndroid : public FileSystem
{
public:
	inline static FileSystemAndroid* GetInstance()
	{
		static FileSystemAndroid instance;
		return &instance;
	}

	bool Init() override;
	void Shutdown() override;

	char* LoadFile(const char* name, size_t* size) override;
	bool SaveFile(const char* name, const void* data, size_t size) override;
	bool RemoveFile(const char* name) override;

	const char* GetRootDir() override;
	const char* GetCurDir() override;
	bool CreateDir(const char* name) override;
	bool ChangeDir(const char* name) override;
	bool RemoveDir(const char* name) override;

private:
	FileSystemAndroid();
	~FileSystemAndroid();
};
