/** \file    Mapfile_List.h
 *  \brief   Contains all the functions and types for allocating and destroying new members of a lexeme list
 */

#pragma once

#include "framework.h"

/** \brief      A single text node in a linked list of text nodes. next = null at the end of the list */
typedef struct MF_Lexeme
{
	char lexeme[512];
	struct MF_Lexeme* next;
} MF_Lexeme_t;

/** \brief                    Allocates a new lexeme struct and a text buffer for it, then copies the provided text
 *                            into that buffer
 *  \param[In] text           Text for the lexeme
 *  \param[In] stringLength   Length of the lexeme text
 *  \return                   A pointer to the newly allocated lexeme
 */
PRIVATE struct MF_Lexeme* MF_NewLexeme(const char* text, size_t stringLength);

/** \brief                    Creates a new lexeme, then appends it to the end of the list provided. If the Lexeme list
 *                            node provided is not the last node in the list, MF_AttachLexeme will traverse the list and
 *                            add the new lexeme node to the end
 *  \param[In] current        The list node to append to
 *  \param[In] text           The text to add to the lexeme
 *  \param[In] stringLength   The length of the text
 *  \return                   A pointer to the newly allocated lexeme
 */
PRIVATE struct MF_Lexeme* MF_AttachLexeme(MF_Lexeme_t* current, const char* text, size_t stringLength);

/** \brief            Destroys every node (and their text buffers) in the lexeme list and frees them
 *  \param[In] head   The head of the lexeme list
 */