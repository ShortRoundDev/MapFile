#include "MapFile_System.h"
#include <stdlib.h>

MF_AllocatorFunc MF_Alloc = NULL;
MF_FreeFunc MF_Free = NULL;

THREAD_LOCAL uint32_t MF_Err = 0;
THREAD_LOCAL char MF_ErrMessage[1024] = { 0 };

void MF_Init()
{
	MF_Alloc = malloc;
	MF_Free = free;
}

void MF_SetAllocator(MF_AllocatorFunc allocFunc)
{
	MF_Alloc = allocFunc;
}

void MF_SetFree(MF_FreeFunc freeFunc)
{
	MF_Free = freeFunc;
}

void MF_Raise(uint32_t err, const char* message)
{
	MF_Err = err;
	strncpy_s(MF_ErrMessage, 1024, message, strlen(message));
}

void MF_ResetErr()
{
	memset(MF_ErrMessage, 0, 1024);
	MF_Err = 0;
}

uint32_t MF_GetErrCode()
{
	return MF_Err;
}

void MF_GetErrMessage(_Out_writes_(1024) char* buffer)
{
	strncpy_s(buffer, 1024, MF_ErrMessage, 1024);
}