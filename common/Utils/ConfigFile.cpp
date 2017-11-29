#include "pch.h"
#include "ConfigFile.h"
#include "ConsoleVar.h"


ConfigFile::ConfigFile()
{
}


ConfigFile::~ConfigFile()
{
}


bool ConfigFile::Load(const char* name)
{
	char* data = FS_LoadFile(name, nullptr);
	if (!data)
		return false;

	std::string line;
	const char* context = data;
	while(context)
	{
		context = GetLine(context, line);
		size_t index = line.find_first_of(' ');
		if (index != std::string::npos)
		{
			line[index] = 0;
			SetVar(&line[0], &line[index + 1]);
		}
	}
	delete[] data;

	return true;
}


bool ConfigFile::Save(const char* name)
{
	std::string buffer;
	buffer.reserve(1024);

	for(const auto var: m_vars)
	{
		buffer += var.name;
		buffer += " ";
		buffer += var.value;
		buffer += "\n";
	}

	if (!buffer.empty())
	{
		if (!FS_SaveFile(name, &buffer[0], buffer.size()))
			return false;
	}

	return true;
}


size_t ConfigFile::GetNumVars() const
{
	return m_vars.size();
}


const char* ConfigFile::GetVarName(size_t index) const
{
	return m_vars[index].name.c_str();
}


const char* ConfigFile::GetVarValue(size_t index) const
{
	return m_vars[index].value.c_str();
}


int ConfigFile::FindVar(const char* name) const
{
	for(size_t i = 0; i < m_vars.size(); i++)
	{
		if (m_vars[i].name == name)
			return (int)i;
	}
	return -1;
}


void ConfigFile::SetVar(const char* name, const char* value)
{
	int index = FindVar(name);
	if (index == -1)
	{
		m_vars.resize(m_vars.size() + 1);
		m_vars.back().name = name;
		m_vars.back().value = value;
	}
	else
	{
		m_vars[index].value = value;
	}
}


void ConfigFile::AddToEnv() const
{
	for (const auto var: m_vars)
	{
		CVar_Set(var.name.c_str(), var.value.c_str());
	}
}