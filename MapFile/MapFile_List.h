#pragma once

#include "framework.h"

/// <summary>
/// A single text node in a linked list of text nodes. next = null at the end of the list
/// </summary>
typedef struct MF_Lexeme
{
	char* lexeme;
	MF_Lexeme* next;
} MF_Lexeme;

/// <summary>
/// Allocates a new lexeme struct and a text buffer for it, then copies the provided text
/// into that buffer
/// </summary>
/// <param name="text">Text for the lexeme</param>
/// <param name="stringLength">Length of the lexeme text</param>
/// <returns>A pointer to the newly allocated lexeme</returns>
MF_Lexeme* MF_NewLexeme(const char* text, size_t stringLength);

/// <summary>
/// Calls MF_NewLexeme, then appends it to the end of the list provided. If the Lexeme list
/// node provided is not the last node in the list, MF_AttachLexeme will traverse the list and
/// add the new lexeme node to the end
/// </summary>
/// <param name="current">The list node to append to</param>
/// <param name="text">The text to add to the lexeme</param>
/// <param name="stringLength">The length of the text</param>
/// <returns>A pointer to the newly allocated lexeme</returns>
MF_Lexeme* MF_AttachLexeme(MF_Lexeme* current, const char* text, size_t stringLength);

/// <summary>
/// Destroys every node (and their text buffers) in the lexeme list and frees them
/// </summary>
/// <param name="head">The head of the lexeme list</param>
DLL void MF_DestroyLexemeList(MF_Lexeme* head);