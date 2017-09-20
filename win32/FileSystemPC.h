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
	FileSystemPC();
	~FileSystemPC();

	void GetFullName(char* dst, const char* name);
	void GetCurrName(char* dst, const char* name);
	void UpdatePath(char* path);

private:
	char m_rootDir[1024] = {'.', 0};
	char m_curDir[1024] = {'.', 0};
};
