#define _CRTDBG_MAPALLOC
#define _CRTDBG_MAP_ALLOC

#include <stdlib.h>
#include <crtdbg.h>

#include "MapFile.h"
#include <stdio.h>

/* runs MF_Parse and prints the first attribute of the first entity,
 * or the error code, if there is an error */
void parse_and_print(char* text);

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	MF_Init();

	HANDLE file = CreateFile(
		L"Data\\Weird.map",
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (file == INVALID_HANDLE_VALUE) {
		return 1;
	}

	DWORD size = GetFileSize(file, NULL);
	char* mapText = (char*)malloc(size + 1);
	ZeroMemory(mapText, size + 1);
	if (mapText == NULL) {
		return;
	}
	if (!ReadFile(file, mapText, size, NULL, NULL)) {
		return;
	}

	MF_Map_t map;
	if (!MF_Parse(mapText, &map))
	{
		char msg[1024];
		MF_GetErrMessage(msg);
		//free(mapText);		

		return;
	}

	free(mapText);
	MF_DestroyMap(&map);

	//MF_Mesh_t* mesh;
	//MF_GenerateMesh(&map, &mesh);
	return 0;
}
