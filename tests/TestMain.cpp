#include "pch.h"
#include "UnitTest.h"
#include "FileSystemTest.h"


static const UnitTest* g_UnitTests[] =
{
	FileSystemTest::GetInstance()
};


static const int NUM_TESTS = sizeof(g_UnitTests) / sizeof(g_UnitTests[0]);


int test_main()
{
	int passed = 0;
	int errors = 0;

	for(int i = 0; i < NUM_TESTS; i++)
	{
		UnitTest* test = g_UnitTests[i];

		const char* name = test->GetName();

		LogPrintf("Running test '%s'...", name);

		bool res = test->Run();
		if (!res)
		{
			LogPrintf("TEST FAILED!!!");
			errors++;
		}
		else
		{
			LogPrintf("Success!");
			passed++;
		}
	}

	return (errors ? -1 : 0);
}