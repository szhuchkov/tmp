#pragma once


class UnitTest
{
public:
    UnitTest() {}
    virtual ~UnitTest() {}
	virtual const char* GetName() = 0;
	virtual bool Run() = 0;
};
