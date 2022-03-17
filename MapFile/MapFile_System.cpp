#include "MapFile_System.h"
#include <stdlib.h>

MF_AllocatorFunc MF_Alloc = NULL;
MF_FreeFunc MF_Free = NULL;

enum MF_FACE_ORIENTATION_DIRECTION {
	MF_CW,
	MF_CCW
};

THREAD_LOCAL uint32_t MF_Err = 0;
THREAD_LOCAL char MF_ErrMessage[1024] = { 0 };
THREAD_LOCAL MF_FACE_ORIENTATION_DIRECTION MF_FaceOrientation = MF_CCW;

void* _MF_HeapAlloc(size_t size);

void _MF_FreeHeap(void* mem);

void MF_Init()
{
	MF_Alloc = malloc;
	MF_Free = free;
}

void* _MF_HeapAlloc(size_t size) {
	return ::HeapAlloc(
		GetProcessHeap(),
		HEAP_ZERO_MEMORY,
		size
	);
}

void _MF_FreeHeap(void* mem) {
	::HeapFree(GetProcessHeap(), NULL, mem);
}

void MF_SetAllocator(MF_AllocatorFunc allocFunc)
{
	MF_Alloc = allocFunc;
}

void MF_SetFaceOrientation(MF_FACE_ORIENTATION_DIRECTION direction) {
	MF_FaceOrientation = direction;
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