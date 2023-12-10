#pragma once

#ifdef ENGINE_WIN
	#ifdef ENGINE_DLL_BUILD
		#define ENGINE_API __declspec(dllexport)
	#else
		#define ENGINE_API __declspec(dllimport)
	#endif
#else
	#error The engine doesn't support this platform
#endif /
