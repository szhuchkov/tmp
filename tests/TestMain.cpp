#include "pch.h"
#include "UnitTest.h"
#include "MeshDataTest.h"


int main(int argc, char** argv)
{
    UnitTest* tests[] = {
        new MeshDataTest(),
    };

    int numTests = ARRAY_SIZE(tests);

	int passed = 0;
	int errors = 0;

	for(int i = 0; i < numTests; i++)
	{
        auto test = tests[i];
		auto name = test->GetName();

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

        delete test;
	}

	return (errors ? -1 : 0);
}


void _LogPrintf(const char* file, int line, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char buffer[10000] = { 0 };
    int offset = 0;

    // VS double click navigation support
    offset += sprintf(&buffer[offset], "%s(%d): ", file, line);
    offset += vsprintf(&buffer[offset], format, args);
    buffer[offset++] = '\n';
    buffer[offset++] = 0;

    va_end(args);

    // dump to the console and debug output
    printf("%s", buffer);
}