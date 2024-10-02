#pragma once

#ifdef PROJECT_WIN
#ifdef PROJECT_DLL_BUILD
#define PROJECT_API __declspec(dllexport)
#else
#define PROJECT_API __declspec(dllimport)
#endif
#else
#error The engine doesn't support this platform
#endif /