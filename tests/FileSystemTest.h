#pragma once


#include "UnitTest.h"


class FileSystemTest : public UnitTest
{
public:
	FileSystemTest()
	{
	}

	~FileSystemTest()
	{
	}

	virtual const char* GetName()
	{
		return "File System Test";
	}

	virtual bool Run()
	{
		static const char* testDirName = "FileSystemTest";
		static const char* testFileName = "FileSystemTest.txt";
		static const char* write = "Test message";

		if (!FS_CreateDir(testDirName))
			return false;

		if (!FS_ChangeDir(testDirName))
			return false;

		if (!FS_SaveFile(testFileName, write, strlen(write)))
			return false;

		char* read = FS_LoadFile(testFileName, nullptr);
		if (!read)
			return false;

		bool res = 0 == strcmp(read, write);

		delete[] read;

		if (!FS_DeleteFile(testFileName))
			return false;

		if (!FS_ChangeDir(nullptr))
			return false;

		if (!FS_DeleteDir(testDirName))
			return false;

		return res;
	}
};
