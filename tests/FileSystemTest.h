#pragma once


#include "UnitTest.h"


class FileSystemTest : public UnitTest
{
private:
	FileSystemTest()
	{
	}

	~FileSystemTest()
	{
	}

public:
	static FileSystemTest* GetInstance()
	{
		static FileSystemTest instance;
		return &instance;
	}

	virtual const char* GetName()
	{
		return "File System Test";
	}

	virtual bool Run()
	{
		static const char* testFileName = "FileSystemTest.txt";
		static const char* testData = "Test message";

		if (!FS_SaveFile(testFileName, testData, sizeof(testData)))
			return false;

		char* read = if (!FS_

		return true;
	}
};
