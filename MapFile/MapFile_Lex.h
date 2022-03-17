/** \file    MapFile_Lex.h
 *  \brief   Contains all the functions for generating a lexeme list from the source text
 */

#pragma once

#include "MapFile_System.h"
#include "framework.h"
#include "MapFile_List.h"

#ifdef __cplusplus
CONSTEXPR uint32_t MF_LEX_ERR = 0x01000000;
#else
#define MF_LEX_ERR (0x01000000)
#endif

CONSTEXPR uint32_t MF_LEX_UNREC_STR_CODE = MF_LEX_ERR | 0x01;
CONSTEXPR char MF_LEX_UNREC_STR_MSG[] = "Unrecognized token in string!";

/** \brief      Creates a linked list of uncategorized lexemes from the text input
 *  \param[In]  The mapfile text input to lex
 *  \return     A pointer to the linked list of lexemes
 */
_Success_(return != NULL)
DLL MF_Lexeme_t* MF_Lex(_In_ const char* string);

/** \brief      If the input string points to the start of a string of the format
 *              "string"
 *              then MF_LexString returns the length of the lexeme. Otherwise, it returns 0
 *  \param[In]  The cursor within the map text to analyze</param>
 *  \return     The size of the lexeme
 */
_Success_(return != 0)
PRIVATE size_t MF_LexString(_In_ char* string);

/** \brief              If the input string points to the start of a number string
 *                      (which allows - and . symbols), then MF_LexNumber returns the
 *                      length of the lexeme. Otherwise, it returns 0
 *  \param[In] string   The cursor within the map text to analyze
 *  \return             The size of the lexeme
 */
_Success_(return != 0)
PRIVATE size_t MF_LexNumber(_In_ char* string);

/** \brief             If the input string points to the start of a comment string of the
 *                     format //comment (ending with either a newline or null terminator), then
 *                     MF_LexComment returns the length of the lexeme. Otherwise, it returns 0
 *  \param[In] string  The cursor within the map text to analyze</param>
 *  \return            The size of the lexeme
 */
_Success_(return != 0)
PRIVATE size_t MF_LexComment(_In_ char* string);

/** \brief             Returns the length of the lexeme ending with whitespace or a null terminator.
 *                     All characters are matched up until whitespace, a newline, or the null terminator, so
 *                     this function should never return 0 (unless the input itself starts with
 *                     whitespace, which should never happen in the core MF_Lex loop).
 *  \param[In] string  The cursor within the map text to analyze
 *  \return            The size of the lexeme
 */
_Success_(return != 0)
PRIVATE size_t MF_LexWhitespacePaddedString(_In_ char* string);

/** \brief             Returns true if the character is either { } ( ) or a newline (\n). Else, returns false
 *  \param[In] c       The character to check
 *  \return            Whether the character is a map file syntax character
 */
_Success_(return)
PRIVATE BOOL MF_LexIsMapFileSyntax(char c);

/** \brief             Returns true if the character is either a space or a tab
 *  \param[In] c       The character to check
 *  \return            Whether the character is whitespace
 */
_Success_(return)
PRIVATE BOOL MF_LexIsWhitespace(char c);

/** \brief             Returns true if the character is a digit, a period (.) or a minus (-)
 *  \param[In] c       The character to check
 *  \return            Whether the character is a valid part of a number string
 */
_Success_(return)
PRIVATE BOOL MF_LexIsNumber(char c);

DLL BOOL MF_DestroyLexChain(_In_ MF_Lexeme_t* lexemeChain);

PRIVATE BOOL MF_DestroyLexeme(_In_ MF_Lexeme_t* lexeme);