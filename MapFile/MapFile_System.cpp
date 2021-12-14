#include "MapFile_System.h"
#include <stdlib.h>

MF_AllocatorFunc MF_Alloc = NULL;
MF_FreeFunc MF_Free = NULL;

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