#pragma once


class UnitTest
{
protected:
	UnitTest() {}
	virtual ~UnitTest() {}

public:
	virtual const char* GetName() = 0;
	virtual bool Run() = 0;
};
