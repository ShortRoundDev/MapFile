#pragma once

#ifdef MAPFILE_EXPORTS
	#define DLL __declspec(dllexport)
	#define LIB_GLOBAL
#else
	#define DLL __declspec(dllimport)
	#define LIB_GLOBAL extern
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>