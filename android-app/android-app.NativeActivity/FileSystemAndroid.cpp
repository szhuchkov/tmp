#include "pch.h"
#include "FileSystemAndroid.h"


FileSystem* FileSystem::GetInstance()
{
	return FileSystemAndroid::GetInstance();
}


FileSystemAndroid::FileSystemAndroid()
{
}


FileSystemAndroid::~FileSystemAndroid()
{
}


bool FileSystemAndroid::Init()
{
	return true;
}


void FileSystemAndroid::Shutdown()
{
}


char* FileSystemAndroid::LoadFile(const char* name, size_t* size)
{
	return nullptr;
}


bool FileSystemAndroid::SaveFile(const char* name, const void* data, size_t size)
{
	return false;
}


bool FileSystemAndroid::RemoveFile(const char* name)
{
	return false;
}


const char* FileSystemAndroid::GetRootDir()
{
	return nullptr;
}


const char* FileSystemAndroid::GetCurDir()
{
	return nullptr;
}



bool FileSystemAndroid::CreateDir(const char* name)
{
	return false;
}


bool FileSystemAndroid::ChangeDir(const char* name)
{
	return false;
}


bool FileSystemAndroid::RemoveDir(const char* name)
{
	return false;
}
