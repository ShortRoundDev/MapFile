#pragma once

#include "framework.h"

DLL void MF_Init();

/* Memory stuff */
#pragma region Memory
typedef void* (*MF_AllocatorFunc)(size_t size);
typedef void (*MF_FreeFunc)(void* memory);

DLL extern MF_AllocatorFunc MF_Alloc;
DLL extern MF_FreeFunc MF_Free;

DLL void MF_SetAllocator(MF_AllocatorFunc allocFunc);
DLL void MF_SetFree(MF_FreeFunc freeFunc);
#pragma endregion