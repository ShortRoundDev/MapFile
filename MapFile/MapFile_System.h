/** \file   MapFile_System.h
 *  \brief  Contains all the functions for allocating memory, as well as raising errors
 */

#pragma once

#include "framework.h"

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

extern THREAD_LOCAL uint32_t MF_Err;
extern THREAD_LOCAL char MF_ErrMessage[1024];

/** \brief   Initializes MapFile.dll. Sets the global allocator to malloc and the global deallocator to free */
DLL void MF_Init();

#pragma region Memory
/**  \brief  The function pointer type for an allocator. Takes in a single size_t argument,
 *           but the implementation may be decided by the developer (calloc, malloc, HeapAlloc, etc.)
 */
typedef void* (*MF_AllocatorFunc)(size_t);

/** \brief   The function pointer type for a deallocator. Takes in a single size_t argument,
 *           but the implementation may be decided by the developer (zero before free, etc.)
 */
typedef void (*MF_FreeFunc)(void*);

/** \brief   The global allocator */
extern void* (*MF_Alloc)(size_t);

/** \brief   The global deallocator */
extern void (*MF_Free)(void*);

/** \brief                 Sets a global allocator function. MF_Init defaults this to malloc
 *  \param[In] allocFunc   The function to use as the global allocator
 */
DLL void MF_SetAllocator(void* (*allocFunc)(size_t));

/** \brief                 Set a global deallocator function. MF_Init defaults this to free
 *  \param[In] freeFunc    The function to use as the global deallocator
 */
DLL void MF_SetFree(void (*freeFunc)(void*));
#pragma endregion

#pragma region Error
/** \brief              Sets MF_Err to err and MF_ErrMessage to message
 *  \param[In] err      The error code
 *  \param[In] message  The error message
 */
PRIVATE void MF_Raise(uint32_t err, const char* message);

/** \brief   Resets MapFile Error code to 0 and the MapFile Error message to an empty string.
 *           This is called at the top of MF_Parse. This should be called after an error is
 *           handled in MF_Parse if execution should continue
 */
DLL void MF_ResetErr();

/** \brief    Returns the current error code for MapFile
 *  \return   The current error code for MapFile
 */
DLL uint32_t MF_GetErrCode();

/** \brief              Copies the current MapFile error message to the provided buffer.
                        The buffer must be 1024 bytes.
 *  \param[In] buffer   The buffer to be written to. Must be 1024 bytes
 */
DLL void MF_GetErrMessage(_Out_writes_(1024) char* buffer);

#pragma endregion