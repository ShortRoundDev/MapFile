/** \file    framework.h
 *  \brief   Windows specific stuff, related to DLL exporting and importing
 */

#pragma once

#ifdef MAPFILE_EXPORTS
	#define DLL extern "C" __declspec(dllexport)
	#define LIB_GLOBAL
#else
	#ifdef __cplusplus
		#define DLL extern "C" __declspec(dllimport)
	#else
		#define DLL __declspec(dllimport)
	#endif
	#define LIB_GLOBAL extern
#endif

#ifdef __cplusplus
	#define CONSTEXPR constexpr
	#ifdef __TEST
		#define PRIVATE DLL
	#else
		#define PRIVATE
	#endif
#else
	#define CONSTEXPR const
	#define PRIVATE
#endif

#define THREAD_LOCAL __declspec( thread )

#define WIN32_LEAN_AND_MEAN
#include <windows.h>