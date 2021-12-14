#include "MapFile_List.h"
#include "MapFile_System.h"

#include <string>

MF_Lexeme* MF_NewLexeme(const char* text, size_t stringLength)
{
	void* memory = MF_Alloc(sizeof(MF_Lexeme) + stringLength + 1);
	MF_Lexeme* node = (MF_Lexeme*)memory;
	node->next = NULL;

	node->lexeme = (char*)memory + sizeof(MF_Lexeme);
	memset(node->lexeme, 0, stringLength + 1);
	memcpy(node->lexeme, text, stringLength);

	return node;
}

MF_Lexeme* MF_AttachLexeme(MF_Lexeme* current, const char* text, size_t stringLength)
{
	MF_Lexeme* cursor = current;
	while (cursor->next != NULL) {
		cursor = cursor->next;
	}
	cursor->next = MF_NewLexeme(text, stringLength);
	return cursor->next;
}

void MF_DestroyLexemeList(MF_Lexeme* head)
{
	MF_Lexeme* cursor = head;
	while (cursor != NULL) {
		auto tmp = cursor->next;
		memset(cursor, 0, sizeof(MF_Lexeme) + strlen(cursor->lexeme) + 1);
		MF_Free(cursor);
		cursor = tmp;
	}
}