#pragma once


#include "Platform.h"


#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>


#include <vector>
#include <list>
#include <string>
#include <algorithm>


void _LogPrintf(const char* file, int line, const char* format, ...);
#define LogPrintf(...)		_LogPrintf(__FILE__, __LINE__, __VA_ARGS__)
