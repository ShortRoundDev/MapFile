#include "MapFile_List.h"
#include "MapFile_System.h"

#include <string>

MF_Lexeme* MF_NewLexeme(const char* text, size_t stringLength)
{
	MF_Lexeme* node = (MF_Lexeme*)MF_Alloc(sizeof(MF_Lexeme));
	node->next = NULL;
	node->lexeme[stringLength] = 0;

	CopyMemory(node->lexeme, text, stringLength);

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
