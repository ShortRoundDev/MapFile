#pragma once

#include "framework.h"
#include <cstdint>

extern thread_local uint32_t MF_Err;
extern thread_local char MF_ErrMessage[1024];

DLL void MF_Init();

/* Memory stuff */
#pragma region Memory
/// <summary>
/// The function pointer type for an allocator. Takes in a single size_t argument,
/// but the implementation may be decided by the developer (calloc, malloc, HeapAlloc, etc.)
/// </summary>
typedef void* (*MF_AllocatorFunc)(size_t size);

/// <summary>
/// The function pointer type for a deallocator. Takes in a single size_t argument,
/// but the implementation may be decided by the developer (zero before free, etc.)
/// </summary>
typedef void (*MF_FreeFunc)(void* memory);

extern MF_AllocatorFunc MF_Alloc;
extern MF_FreeFunc MF_Free;

/// <summary>
/// Sets a global allocator function. MF_Init defaults this to malloc
/// </summary>
/// <param name="allocFunc">The function to use as the global allocator</param>
DLL void MF_SetAllocator(MF_AllocatorFunc allocFunc);

/// <summary>
/// Set a global deallocator function. MF_Init defaults this to free
/// </summary>
/// <param name="freeFunc">The function to use as the global deallocator</param>
DLL void MF_SetFree(MF_FreeFunc freeFunc);
#pragma endregion

#pragma region Error
/// <summary>
/// Sets MF_Err to err and MF_ErrMessage to message
/// </summary>
/// <param name="err">The error code</param>
/// <param name="message">The error message</param>
void MF_Raise(uint32_t err, const char* message);

/// <summary>
/// Resets MapFile Error code to 0 and the MapFile Error message to an empty string.
/// This is called at the top of MF_Parse. This should be called after an error is
/// handled in MF_Parse if execution should continue
/// </summary>
DLL void MF_ResetErr();

/// <summary>
/// Returns the current error code for MapFile
/// </summary>
/// <returns>The current error code for MapFile</returns>
DLL uint32_t MF_GetErrCode();

/// <summary>
/// Copies the current MapFile error message to the provided buffer. The buffer must be 1024 bytes.
/// </summary>
/// <param name="buffer">The buffer to be written to. Must be 1024 bytes</param>
DLL void MF_GetErrMessage(_Out_writes_(1024) char* buffer);

#pragma endregion