#pragma once

#include "MapFile_System.h"
#include "MapFile_Types.h"
#include "MapFile_List.h"

constexpr uint32_t MF_PARSE_ERR = 0x02000000;

constexpr uint32_t MF_PARSE_NO_ENTS_CODE = MF_PARSE_ERR | 0x01;
constexpr char MF_PARSE_NO_ENTS_MSG[] = "No entities in map file counted";

constexpr uint32_t MF_PARSE_ENT_NO_ATT_CODE = MF_PARSE_ERR | 0x02;
constexpr char MF_PARSE_ENT_NO_ATT_MSG[] = "No attributes found in an entity. Entities requires at least a classname";

constexpr uint32_t MF_PARSE_MISALIGNED_ATT_CODE = MF_PARSE_ERR | 0x03;
constexpr char MF_PARSE_MISALIGNED_ATT_MSG[] = "Expected \" while parsing attribute list";

constexpr uint32_t MF_PARSE_MISALIGNED_BRUSH_PAREN_CODE = MF_PARSE_ERR | 0x04;
constexpr char MF_PARSE_MISALIGNED_BRUSH_PAREN_MSG[] = "Expected ( while parsing brush";

constexpr uint32_t MF_PARSE_REACHED_END_CODE = MF_PARSE_ERR | 0x05;
constexpr char MF_PARSE_REACHED_END_MSG[] = "Reached end of document while parsing";

constexpr uint32_t MF_PARSE_MISALIGNED_ENT_CODE = MF_PARSE_ERR | 0x06;
constexpr char MF_PARSE_MISALIGNED_ENT_MSG[] = "Expected { while parsing ent";

/// <summary>
/// Parses the text of a mapfile into its relevant MF_Map structs. This runs allocations every step of the way.
/// If there is an error in parsing, this returns false and an error code is set. The developer can get error
/// information with MF_GetErrCode() and MF_GetErrMessage()
/// </summary>
/// <param name="text">The text of the mapfile</param>
/// <param name="map">A pointer to where the map data will be stored after parsing</param>
/// <returns>True on success, False on failure</returns>
_Success_(return)
DLL bool MF_Parse(_In_ char* text, _Out_ MF_Map* map);

/// <summary>
/// Parses an entity into the provided MF_Entity*
/// </summary>
/// <param name="lexemes">The lexeme node of the opening bracket of the Entity</param>
/// <param name="entity">A pointer to where the entity data will be stored after parsing</param>
/// <returns>If successful, returns a pointer to the next lexeme node after the entity definition. Else, returns NULL</returns>
_Success_(return != NULL)
MF_Lexeme* MF_ParseEntity(_In_ MF_Lexeme* lexemes, _Out_ MF_Entity* entity);


/// <summary>
/// Parses a key-value-pair from the entity attribute list into the provided MF_KeyValuePair*
/// </summary>
/// <param name="lexemes">The lexeme node of the key</param>
/// <param name="keyValuePair">A pointer to where the Key Value Pair data will be stored after parsing</param>
/// <returns>If successful, returns a pointer to the next lexeme node after the key value pair definition. Else, returns NULL</returns>
_Success_(return != NULL)
MF_Lexeme* MF_ParseKeyValuePair(_In_ MF_Lexeme* lexemes, _Out_ MF_KeyValuePair* keyValuePair);

/// <summary>
/// Parses a brush into the provided MF_Brush*
/// </summary>
/// <param name="lexemes">The lexeme node of the opening parenthetical of the first face of the brush</param>
/// <param name="brush">A pointer to where the Brush data will be stored after parsing</param>
/// <returns>If successful, returns a pointer to the end bracket of the brush definition. Else, returns NULL</returns>
_Success_(return != NULL)
MF_Lexeme* MF_ParseBrush(_In_ MF_Lexeme* lexemes, _Out_ MF_Brush* brush);

/// <summary>
/// Parses a brush face into the provided MF_Face*
/// </summary>
/// <param name="lexemes">The lexeme node of the opening parenthetical of the first vertex of the face</param>
/// <param name="face">A pointer to where the Face data will be stored after parsing</param>
/// <returns>If successful, returns a pointer to the newline lexeme separating the current face from the next line</returns>
_Success_(return != NULL)
MF_Lexeme* MF_ParseFace(_In_ MF_Lexeme* lexemes, _Out_ MF_Face* face);

/// <summary>
/// Parses a face vertex into the provided MF_Vertex*
/// </summary>
/// <param name="lexemes">A pointer to the opening parenthetical of ther vertex</param>
/// <param name="vertex">A pointer to where the Vertex data will be stored after parsing</param>
/// <returns>If successful, returns a pointer to the next lexeme after the closing parenthetical of the Vertex</returns>
_Success_(return != NULL)
MF_Lexeme* MF_ParseVertex(_In_ MF_Lexeme* lexemes, _Out_ MF_Vertex* vertex);

/// <summary>
/// Parses a set of texture parameters into the provided MF_TextureParameters*
/// </summary>
/// <param name="lexemes">The lexeme node of the first texture parameter (offsetX)</param>
/// <param name="textureParameters">A pointer to where the Texture Parameter data will be stored after parsing</param>
/// <returns>
/// A pointer to 5 lexemes down the list from the input lexeme. This function simply iterates through the list without type
/// checking and has no failure state.
/// </returns>
_Success_(return != NULL)
MF_Lexeme* MF_ParseTextureParameters(_In_ MF_Lexeme* lexemes, _Out_ MF_TextureParameters* textureParameters);

/// <summary>
/// Counts the number of top level open-close brackets starting at the input lexeme. Does not count nested brackets.
/// This function is also good for counting the number of brushes inside a given entity
/// </summary>
/// <param name="lexemes">The lexeme to begin counting at</param>
/// <returns>The number of open-close bracket pairs</returns>
_Success_(return);
int MF_CountEntities(_In_ MF_Lexeme* lexemes);

/// <summary>
/// Counts the number of Key Value Pairs at the given hierarchy of the lexeme list.
/// Halts when it encounters to first opening bracket {, or end of the list
/// </summary>
/// <param name="lexemes">The lexeme to begin counting at</param>
/// <returns>The number of attributes in the entity. 0 is bad (all entities need at least a classname).</returns>
_Success_(return);
int MF_CountAttributes(_In_ MF_Lexeme* lexemes);

/// <summary>
/// Counts the number of faces in a brush. Gets the number of opening parentheses ( that occur immediately after a
/// newline character, stopping when an opening bracket { is reached, or end of list
/// </summary>
/// <param name="lexemes">The lexeme to begin counting at</param>
/// <returns>The number of faces in the brush</returns>
_Success_(return);
int MF_CountFaces(_In_ MF_Lexeme* lexemes);
