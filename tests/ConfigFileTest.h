#pragma once


#include "UnitTest.h"
#include "ConfigFile.h"
#include "ConfigFileTest.h"


class ConfigFileTest : public UnitTest
{
public:
	const char* GetName() override
	{
		return "ConfigFileTest";
	}

	bool Run() override
	{
		static const char* TEST_CONFIG_NAME = "__test.cfg";

		static const char* vars[] =
		{
			"test1",
			"test2",
			"test3",
		};
		
		static const char* values[] =
		{
			"test string 1",
			"2",
			"3",
		};

		static const int NUM_VARS = ARRAY_SIZE(vars);

		ConfigFile writeConf;

		for (int i = 0; i < NUM_VARS; i++)
			writeConf.SetVar(vars[i], values[i]);

		if (!writeConf.Save(TEST_CONFIG_NAME))
			return false;

		ConfigFile readConf;
		if (!readConf.Load(TEST_CONFIG_NAME))
			return false;

		if (readConf.GetNumVars() != NUM_VARS)
			return false;

		for(int i = 0; i < NUM_VARS; i++)
		{
			if (strcmp(vars[i], readConf.GetVarName(i)))
				return false;

			if (strcmp(values[i], readConf.GetVarValue(i)))
				return false;
		}

		FS_DeleteFile(TEST_CONFIG_NAME);

		return true;
	}
};