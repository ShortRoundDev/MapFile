#include "MapFile.h"
#include <stdlib.h>
#include <stdio.h>

/* runs MF_Parse and prints the first attribute of the first entity,
 * or the error code, if there is an error */
void parse_and_print(char* text);

int main(int argc, char** argv)
{
	MF_Init();

	FILE* fp;
	if (fopen_s(&fp, "Data\\Weird.map", "r")) {
		return;
	}
	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	
	fseek(fp, 0, SEEK_SET);
	char* buffer = (char*)calloc(size, sizeof(char));
	if (buffer == NULL) {
		return;
	}
	fread_s(buffer, size, sizeof(char), size, fp);

	fclose(fp);

	MF_Map_t map;
	BOOL success = MF_Parse(buffer, &map);
	success = MF_GenerateMesh(&map);

	return 0;
}

void parse_and_print(char* text)
{
	MF_Map_t map;
	BOOL success = MF_Parse(text, &map);
	if (success) {
		printf("Success!\n");
		printf("Total Entities: %zu\n", map.totalItems);

		MF_KeyValuePair_t attr1 = map.items[0].attributes[0];

		printf("Entity #1 attribute: %s = %s\n\n", attr1.key, attr1.value);
	}
	else {
		printf("Failure!\n");
		int error_code = MF_GetErrCode();
		char buffer[1024] = { 0 };
		MF_GetErrMessage(buffer);
		printf("Error [0x%x]: %s\n\n", error_code, buffer);
	}
	MF_ResetErr();
}