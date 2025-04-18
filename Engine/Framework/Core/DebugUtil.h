#pragma once

#include "TimeUtil.h"

using namespace IExeEngine;
using namespace IExeEngine::Core;

#if defined(_DEBUG)
#define LOG(format, ...);\
    do{\
        char _buffer[256];\
        int _res = snprintfA(_buffer, std::size(_buffer), "{%.3f}: "##format##"\n", TimeUtil::GetTime(), VA_ARGS);\
        OutputDebugString(_buffer);\
    } while(false)

#define ASSERT(condition, format, ...)\
    do{\
        if(!(condition))\
        {\
            LOG("ASSERT! %s(%d)\n"##format##, FILE, LINE, VA_ARGS);\
            DebugBreak();\
        }\
    } while(false)
#else
#define LOG(format, ...)
#define ASSERT(condition, format, ...) do{ (void) sizeof(condition); } while(false)
#endif
