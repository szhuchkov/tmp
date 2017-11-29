#pragma once


class ConfigFile
{
public:
	ConfigFile();
	~ConfigFile();

	bool Load(const char* name);
	bool Save(const char* name);

	size_t GetNumVars() const;
	const char* GetVarName(size_t index) const;
	const char* GetVarValue(size_t index) const;

	int FindVar(const char* name) const;
	void SetVar(const char* name, const char* value);

	void AddToEnv() const;

private:
	struct Var
	{
		std::string name;
		std::string value;
	};
	std::vector<Var> m_vars;
};
