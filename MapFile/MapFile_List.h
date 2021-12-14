#pragma once

#include "framework.h"

typedef struct MF_Lexeme
{
	char* lexeme;
	MF_Lexeme* next;
} MF_Lexeme;

DLL MF_Lexeme* MF_NewLexeme(const char* text, size_t stringLength);
DLL MF_Lexeme* MF_AttachLexeme(MF_Lexeme* current, const char* next, size_t stringLength);
DLL void MF_DestroyLexemeList(MF_Lexeme* head);