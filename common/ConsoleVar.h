#pragma once


class ConsoleVar
{
public:
	ConsoleVar(const char* name, int value = 0, bool dynamic = false);
	ConsoleVar(const char* name, float value = 0.0f, bool dynamic = false);
	ConsoleVar(const char* name, const char* value = "", bool dynamic = false);
	~ConsoleVar();

	const char* GetName() const;

	void SetValue(int value);
	void SetValue(float value);
	void SetValue(const char* value);

	int GetInt() const;
	float GetFloat() const;
	const char* GetString() const;

	static ConsoleVar* GetVarByName(const char* name);
	static size_t GetNumVars();
	static ConsoleVar* GetVar(size_t index);

protected:
	static std::unordered_map<std::string, ConsoleVar*> g_ConsoleVarsByName;
	static std::vector<ConsoleVar*> g_ConsoleVars;

	static void AddVar(ConsoleVar* var);
	static void RemoveVar(ConsoleVar* var);

private:
	bool			m_dynamic;
	std::string		m_name;
	std::string		m_s;
	int				m_i;
	float			m_f;
};
