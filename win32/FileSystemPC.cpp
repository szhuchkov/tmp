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


bool FileSystemPC::Init()
{
	if (!GetCurrentDirectory(sizeof(m_rootDir), m_rootDir))
	{
		LogPrintf("GetCurrentDirectory() failed");
		return false;
	}

	strcpy(m_curDir, m_rootDir);

	return true;
}


void FileSystemPC::Shutdown()
{
}


char* FileSystemPC::LoadFile(const char* name, size_t* size)
{
	char currName[1024];
	GetCurrName(currName, name);

	HANDLE hf = CreateFile(currName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hf == INVALID_HANDLE_VALUE)
		return false;

	DWORD sz = GetFileSize(hf, NULL);
	char* data = new char[sz + 1];
	ReadFile(hf, data, sz, &sz, NULL);
	data[sz] = 0;

	CloseHandle(hf);

	if (size)
		*size = sz;

	return data;
}


bool FileSystemPC::SaveFile(const char* name, const void* data, size_t size)
{
	char currName[1024];
	GetCurrName(currName, name);

	HANDLE hf = CreateFile(currName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if (hf == INVALID_HANDLE_VALUE)
		return false;

	DWORD written;
	WriteFile(hf, data, size, &written, 0);
	CloseHandle(hf);

	if (written != size)
		return false;

	return true;
}


bool FileSystemPC::RemoveFile(const char* name)
{
	char currName[1024];
	GetCurrName(currName, name);

	if (!DeleteFile(currName))
	{
		LogPrintf("DeleteFile() failed for '%s'", name);
		return false;
	}
	return true;
}


bool FileSystemPC::CreateDir(const char* name)
{
	char fullName[1024];

	std::vector<std::string> treePath;
	const char* last = name;
	for(const char* p = name; ; p++)
	{
		if (*p == '/' || *p == '\\' || *p == 0)
		{
			treePath.resize(treePath.size() + 1);
			std::string& back = treePath.back();
			back.assign(last, p);

			if (!*p)
				break;
		}
	}

	size_t treeSize = treePath.size();
	for(size_t i = 0; i < treeSize; i++)
	{
		GetFullName(fullName, treePath[i].c_str());

		if (!CreateDirectory(fullName, NULL))
		{
			DWORD err = GetLastError();
			if (err != ERROR_ALREADY_EXISTS)
			{
				LogPrintf("CreateDirectory() failed for '%s'", name);
				return false;
			}
		}
	}

	return true;
}


bool FileSystemPC::ChangeDir(const char* name)
{
	char fullName[1024];
	GetFullName(fullName, name);

	if (!SetCurrentDirectory(fullName))
	{
		LogPrintf("SetCurrentDirectory() failed for '%s'", name);
		return false;
	}

	// update current directory
	GetCurrentDirectory(sizeof(m_curDir), m_curDir);

	return true;
}


bool FileSystemPC::RemoveDir(const char* name)
{
	char fullName[1024];
	GetFullName(fullName, name);

	if (!RemoveDirectory(fullName))
	{
		DWORD err = GetLastError();
		if (err == ERROR_PATH_NOT_FOUND)
			return true;

		LogPrintf("RemoveDirectory() failed for '%s'", name);
		return false;
	}

	return true;
}


const char* FileSystemPC::GetRootDir()
{
	return m_rootDir;
}


const char* FileSystemPC::GetCurDir()
{
	return m_curDir;
}


void FileSystemPC::GetFullName(char* dst, const char* name)
{
	sprintf(dst, "%s\\%s", m_rootDir, name);
	UpdatePath(dst);
}


void FileSystemPC::GetCurrName(char* dst, const char* name)
{
	sprintf(dst, "%s\\%s", m_curDir, name);
	UpdatePath(dst);
}


void FileSystemPC::UpdatePath(char* path)
{
	while(*path)
	{
		if (*path == '/')
			*path = '\\';
		path++;
	}
}
