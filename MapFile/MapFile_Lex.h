#pragma once

#include "framework.h"

#include "MapFile_List.h"

DLL MF_Lexeme* MF_Lex(_In_ const char* string);

_Success_(return)
size_t MF_LexString(_In_ char* string);

_Success_(return)
size_t MF_LexNumber(_In_ char* string);

_Success_(return)
size_t MF_LexComment(_In_ char* string);

_Success_(return)
size_t MF_LexWhitespacePaddedString(_In_ char* string);

bool MF_LexIsMapFileSyntax(char c);
bool MF_LexIsWhitespace(char c);
bool MF_LexIsNumber(char c);