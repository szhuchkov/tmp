#include "pch.h"
#include "ConsoleVar.h"


std::unordered_map<std::string, ConsoleVar*> ConsoleVar::g_ConsoleVarsByName;
std::vector<ConsoleVar*> ConsoleVar::g_ConsoleVars;


ConsoleVar::ConsoleVar(const char* name, int value, bool dynamic)
{
	m_name = name;
	m_dynamic = dynamic;
	SetValue(value);
	AddVar(this);
}


ConsoleVar::ConsoleVar(const char* name, float value, bool dynamic)
{
	m_name = name;
	m_dynamic = dynamic;
	SetValue(value);
	AddVar(this);
}


ConsoleVar::ConsoleVar(const char* name, const char* value, bool dynamic)
{
	m_name = name;
	m_dynamic = dynamic;
	SetValue(value);
	AddVar(this);
}


ConsoleVar::~ConsoleVar()
{
	RemoveVar(this);
}


const char* ConsoleVar::GetName() const
{
	return m_name.c_str();
}


bool ConsoleVar::IsStatic() const
{
	return !m_dynamic;
}


void ConsoleVar::SetValue(int value)
{
	char str[256];
	sprintf(str, "%d", value);

	m_i = value;
	m_f = static_cast<float>(value);
	m_s = str;
}


void ConsoleVar::SetValue(float value)
{
	char str[256];
	sprintf(str, "%f", value);

	m_i = static_cast<int>(value);
	m_f = value;
	m_s = str;
}


void ConsoleVar::SetValue(const char* value)
{
	m_i = value ? atoi(value) : 0;
	m_f = value ? static_cast<float>(atof(value)) : 0.0f;
	m_s = value ? value : "";
}


int ConsoleVar::GetInt() const
{
	return m_i;
}


float ConsoleVar::GetFloat() const
{
	return m_f;
}


const char* ConsoleVar::GetString() const
{
	return m_s.c_str();
}


ConsoleVar* ConsoleVar::GetVarByName(const char* name)
{
	auto it = g_ConsoleVarsByName.find(name);
	if (it == g_ConsoleVarsByName.end())
	{
		LogPrintf("Console var '%s' not found", name);
		return nullptr;
	}

	return it->second;
}


size_t ConsoleVar::GetNumVars()
{
	return g_ConsoleVars.size();
}


ConsoleVar * ConsoleVar::GetVar(size_t index)
{
	return g_ConsoleVars[index];
}


void ConsoleVar::AddVar(ConsoleVar* var)
{
	const char* name = var->GetName();
	auto it = g_ConsoleVarsByName.find(name);

	assert(it == g_ConsoleVarsByName.end());

	g_ConsoleVars.push_back(var);
	g_ConsoleVarsByName[name] = var;
}


void ConsoleVar::RemoveVar(ConsoleVar* var)
{
	const char* name = var->GetName();
	auto it = g_ConsoleVarsByName.find(name);
	auto it2 = std::find(g_ConsoleVars.begin(), g_ConsoleVars.end(), var);

	assert(it != g_ConsoleVarsByName.end());
	assert(it2 != g_ConsoleVars.end());

	g_ConsoleVarsByName.erase(it);
	g_ConsoleVars.erase(it2);
}
