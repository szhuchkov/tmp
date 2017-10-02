#pragma once


#include "UnitTest.h"


class CVarTest : public UnitTest
{
public:
	virtual const char* GetName() override
	{
		return "CVar test";
	}

	virtual bool Run() override
	{
		const char* var1 = "__testVar1";
		const char* var2 = "__testVar2";
		const char* var3 = "__testVar3";

		CVar_Set(var1, 13);
		CVar_Set(var2, 13.666f);
		CVar_Set(var3, "TEST");

		if (CVar_Get<int>(var1) != 13)
			return false;

		if (CVar_Get<float>(var2) != 13.666f)
			return false;

		if (CVar_Get<std::string>(var3) != "TEST")
			return false;

		if (CVar_Get<float>(var1) != 13.0f)
			return false;

		if (CVar_Get<int>(var2) != 13)
			return false;

		return true;
	}
};
