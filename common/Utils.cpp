#include "pch.h"
#include "Utils.h"
#include "FileSystem.h"
#include "ConsoleVar.h"


const char* GetLine(const char* context, char* buffer)
{
	while (*context)
	{
		if (*context == '\n' || *context == '\r')
		{
			context++;
			break;
		}

		*buffer++ = *context++;
	}
	return *context ? context : nullptr;
}


const char* GetLine(const char* context, std::string& buffer)
{
	buffer.clear();
	while (*context)
	{
		if (*context == '\n' || *context == '\r')
		{
			context++;
			break;
		}

		buffer.push_back(*context++);
	}
	return *context ? context : nullptr;
}


char* FS_LoadFile(const char* name, size_t* size)
{
	char* res = FileSystem::GetInstance()->LoadFile(name, size);
	return res;
}


bool FS_SaveFile(const char* name, const void* data, size_t size)
{
	bool res = FileSystem::GetInstance()->SaveFile(name, data, size);
	return res;
}


bool FS_DeleteFile(const char* name)
{
	bool res = FileSystem::GetInstance()->RemoveFile(name);
	return res;
}


const char* FS_GetRootDir()
{
	const char* res = FileSystem::GetInstance()->GetRootDir();
	return res;
}


const char* FS_GetCurDir()
{
	const char* res = FileSystem::GetInstance()->GetCurDir();
	return res;
}


bool FS_CreateDir(const char* name)
{
	bool res = FileSystem::GetInstance()->CreateDir(name);
	return res;
}


bool FS_DeleteDir(const char* name)
{
	bool res = FileSystem::GetInstance()->RemoveDir(name);
	return res;
}


bool FS_ChangeDir(const char* name)
{
	if (!name)
		name = ".";

	bool res = FileSystem::GetInstance()->ChangeDir(name);
	return res;
}


void CVar_Clear()
{
	// enum dynamic vars
	size_t num = ConsoleVar::GetNumVars();
	std::vector<ConsoleVar*> vars;
	vars.reserve(num);
	for(size_t i = 0; i < num; i++)
	{
		auto var = ConsoleVar::GetVar(i);
		if (!var->IsStatic())
			vars.push_back(var);
	}

	// remove dynamic vars
	num = vars.size();
	for(size_t i = 0; i < num; i++)
	{
		auto var = vars[i];
		delete var;
	}
}


template<>
int CVar_Get(const char* name)
{
	auto* var = ConsoleVar::GetVarByName(name);
	if (var != nullptr)
		return var->GetInt();

	return 0;
}


template<>
float CVar_Get(const char* name)
{
	auto* var = ConsoleVar::GetVarByName(name);
	if (var != nullptr)
		return var->GetFloat();

	return 0.0f;
}


template<>
const char* CVar_Get(const char* name)
{
	auto* var = ConsoleVar::GetVarByName(name);
	if (var != nullptr)
		return var->GetString();

	return nullptr;
}


template<>
std::string CVar_Get(const char* name)
{
	auto* var = ConsoleVar::GetVarByName(name);
	if (var != nullptr)
		return std::string(var->GetString());

	return std::string("");
}


void CVar_Set(const char* name, int value)
{
	auto* var = ConsoleVar::GetVarByName(name);
	if (var)
	{
		var->SetValue(value);
	}
	else
	{
		new ConsoleVar(name, value);
	}
}



void CVar_Set(const char* name, float value)
{
	auto* var = ConsoleVar::GetVarByName(name);
	if (var)
	{
		var->SetValue(value);
	}
	else
	{
		new ConsoleVar(name, value);
	}
}



void CVar_Set(const char* name, const char* value)
{
	auto* var = ConsoleVar::GetVarByName(name);
	if (var)
	{
		var->SetValue(value);
	}
	else
	{
		new ConsoleVar(name, value);
	}
}
