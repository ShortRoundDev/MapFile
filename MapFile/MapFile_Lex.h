#pragma once

#include "MapFile_System.h"
#include "framework.h"
#include "MapFile_List.h"

constexpr uint32_t MF_LEX_ERR = 0x01000000;
constexpr uint32_t MF_LEX_UNREC_STR_CODE = MF_LEX_ERR | 0x01;
constexpr char MF_LEX_UNREC_STR_MSG[] = "Unrecognized token in string!";

/// <summary>
/// Creates a linked list of uncategorized lexemes from the text input
/// </summary>
/// <param name="string">The mapfile text input to lex</param>
/// <returns>A pointer to the linked list of lexemes</returns>
_Success_(return)
DLL MF_Lexeme* MF_Lex(_In_ const char* string);

/// <summary>
/// If the input string points to the start of a string of the format
/// 
/// "string"
/// 
/// then MF_LexString returns the length of the lexeme. Otherwise, it
/// returns 0
/// </summary>
/// <param name="string">The cursor within the map text to analyze</param>
/// <returns>The size of the lexeme</returns>
_Success_(return)
size_t MF_LexString(_In_ char* string);


/// <summary>
/// If the input string points to the start of a number string
/// (which allows - and . symbols), then MF_LexNumber returns the
/// length of the lexeme. Otherwise, it returns 0
/// </summary>
/// <param name="string">The cursor within the map text to analyze</param>
/// <returns>The size of the lexeme</returns>
_Success_(return)
size_t MF_LexNumber(_In_ char* string);

/// <summary>
/// If the input string points to the start of a comment string of the format
/// 
/// //comment
/// 
/// (ending with either a newline or null terminator), then MF_LexComment
/// returns the length of the lexeme. Otherwise, it returns 0
/// </summary>
/// <param name="string">The cursor within the map text to analyze</param>
/// <returns>The size of the lexeme</returns>
_Success_(return)
size_t MF_LexComment(_In_ char* string);

/// <summary>
/// Returns the length of the lexeme ending with whitespace or a null terminator.
/// All characters are matched up until whitespace, a newline, or the null terminator, so
/// this function should never return 0 (unless the input itself starts with
/// whitespace, which should never happen in the core MF_Lex loop).
/// </summary>
/// <param name="string">The cursor within the map text to analyze</param>
/// <returns>The size of the lexeme</returns>
_Success_(return)
size_t MF_LexWhitespacePaddedString(_In_ char* string);

/// <summary>
/// Returns true if the character is either { } ( ) or a newline (\n). Else, returns false
/// </summary>
/// <param name="c">The character to check</param>
/// <returns>Whether the character is a map file syntax character</returns>
_Success_(return)
bool MF_LexIsMapFileSyntax(char c);

/// <summary>
/// Returns true if the character is either a space or a tab
/// </summary>
/// <param name="c">The character to check</param>
/// <returns>Whether the character is whitespace</returns>
_Success_(return)
bool MF_LexIsWhitespace(char c);

/// <summary>
/// Returns true if the character is a digit, a period (.) or a minus (-)
/// </summary>
/// <param name="c">The character to check</param>
/// <returns>Whether the character is a valid part of a number string</returns>
_Success_(return)
bool MF_LexIsNumber(char c);