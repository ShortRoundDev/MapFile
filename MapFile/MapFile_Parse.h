/** \file   MapFile_parse.h
 *  \brief  Contains all the functions and error codes for parsing text and lexemes into MapFile structs
 */

#pragma once

#include "framework.h"

#include "MapFile_System.h"
#include "MapFile_Types.h"
#include "MapFile_List.h"

// Have to #define in C because you can't | a const with another const. Has to be known at runtime (constexpr or #define)
// This is probably a bad solution and I welcome contribution
#ifdef __cplusplus
CONSTEXPR uint32_t MF_PARSE_ERR = 0x02000000;
#else
#define MF_PARSE_ERR (0x02000000)
#endif

CONSTEXPR uint32_t MF_PARSE_NO_ENTS_CODE = MF_PARSE_ERR | 0x01;
CONSTEXPR char MF_PARSE_NO_ENTS_MSG[] = "No entities in map file counted";

CONSTEXPR uint32_t MF_PARSE_ENT_NO_ATT_CODE = MF_PARSE_ERR | 0x02;
CONSTEXPR char MF_PARSE_ENT_NO_ATT_MSG[] = "No attributes found in an entity. Entities requires at least a classname";

CONSTEXPR uint32_t MF_PARSE_MISALIGNED_ATT_CODE = MF_PARSE_ERR | 0x03;
CONSTEXPR char MF_PARSE_MISALIGNED_ATT_MSG[] = "Expected \" while parsing attribute list";

CONSTEXPR uint32_t MF_PARSE_MISALIGNED_BRUSH_PAREN_CODE = MF_PARSE_ERR | 0x04;
CONSTEXPR char MF_PARSE_MISALIGNED_BRUSH_PAREN_MSG[] = "Expected ( while parsing brush";

CONSTEXPR uint32_t MF_PARSE_REACHED_END_CODE = MF_PARSE_ERR | 0x05;
CONSTEXPR char MF_PARSE_REACHED_END_MSG[] = "Reached end of document while parsing";

CONSTEXPR uint32_t MF_PARSE_MISALIGNED_ENT_CODE = MF_PARSE_ERR | 0x06;
CONSTEXPR char MF_PARSE_MISALIGNED_ENT_MSG[] = "Expected { while parsing ent";

/** \brief            Parses the text of a mapfile into its relevant MF_Map structs. This runs allocations every step of the way.
 *                    If there is an error in parsing, this returns false and an error code is set. The developer can get error
 *                    information with MF_GetErrCode() and MF_GetErrMessage()
 *  \param[In]  text  The text of the mapfile
 *  \param[Out] map   A pointer to where the map data will be stored after parsing
 *  \return           True on success, False on failure
 */
_Success_(return)
DLL BOOL MF_Parse(_In_ char* text, _Out_ MF_Map_t* map);

/** \brief               Parses an entity into the provided MF_Entity*
 *  \param[In]  lexemes  The lexeme node of the opening bracket of the Entity
 *  \param[Out] entity   A pointer to where the entity data will be stored after parsing
 *  \return              If successful, returns a pointer to the next lexeme node after the entity definition. Else, returns NULL
 */
_Success_(return != NULL)
PRIVATE MF_Lexeme_t* MF_ParseEntity(_In_ MF_Lexeme_t* lexemes, _Out_ MF_Entity_t* entity);

/** \brief                    Parses a key-value-pair from the entity attribute list into the provided MF_KeyValuePair*
 *  \param[In]  lexemes       The lexeme node of the key
 *  \param[Out] keyValuePair  A pointer to where the Key Value Pair data will be stored after parsing
 *  \return                   If successful, returns a pointer to the next lexeme node after the key value pair definition. Else, returns NULL
 */
_Success_(return != NULL)
PRIVATE MF_Lexeme_t* MF_ParseKeyValuePair(_In_ MF_Lexeme_t* lexemes, _Out_ MF_KeyValuePair_t* keyValuePair);

/** \brief               Parses a brush into the provided MF_Brush*
 *  \param[In]  lexemes  The lexeme node of the opening parenthetical of the first face of the brush
 *  \param[Out] brush    A pointer to where the Brush data will be stored after parsing
 *  \return              If successful, returns a pointer to the end bracket of the brush definition. Else, returns NULL
 */
_Success_(return != NULL)
PRIVATE MF_Lexeme_t* MF_ParseBrush(_In_ MF_Lexeme_t* lexemes, _Out_ MF_Brush_t* brush);

/** \brief              Parses a brush face into the provided MF_Face*
 *  \param[In]  lexemes  The lexeme node of the opening parenthetical of the first vertex of the face
 *  \param[Out] face    A pointer to where the Face data will be stored after parsing
 *  \return             If successful, returns a pointer to the newline lexeme separating the current face from the next line
 */
_Success_(return != NULL)
PRIVATE MF_Lexeme_t* MF_ParseFace(_In_ MF_Lexeme_t* lexemes, _Out_ MF_Face_t* face);

/** \brief               Parses a face vertex into the provided MF_Vertex*
 *  \param[In]  lexemes  A pointer to the opening parenthetical of ther vertex
 *  \param[Out] vertex   A pointer to where the Vertex data will be stored after parsing
 *  \return              If successful, returns a pointer to the next lexeme after the closing
 *                       parenthetical of the Vertex
 */
_Success_(return != NULL)
PRIVATE MF_Lexeme_t* MF_ParseVertex(_In_ MF_Lexeme_t* lexemes, _In_ union MF_Vector3* vertex);

/** \brief                        Parses a set of texture parameters into the provided MF_TextureParameters*
 *  \param[In]  lexemes           The lexeme node of the first texture parameter (offsetX)
 *  \param[Out] textureParameters A pointer to where the Texture Parameter data will be stored after parsing
 *  \return                       A pointer to 5 lexemes down the list from the input lexeme. This function
                                  simply iterates through the list without type checking and has no failure state.
 */
_Success_(return != NULL)
PRIVATE MF_Lexeme_t* MF_ParseTextureParameters(_In_ MF_Lexeme_t* lexemes, _Out_ MF_TextureParameters_t* textureParameters);

/** \brief              Counts the number of top level open-close brackets starting at the input lexeme.
 *                      Does not count nested brackets. This function is also good for counting the number
 *                      of brushes inside a given entity
 *  \param[In] lexemes  The lexeme to begin counting at
 *  \return             The number of open-close bracket pairs
 */
_Success_(return != 0)
PRIVATE int MF_CountEntities(_In_ MF_Lexeme_t* lexemes);

/** \brief              Counts the number of Key Value Pairs at the given hierarchy of the lexeme list.
 *                      Halts when it encounters to first opening bracket {, or end of the list
 *  \param[In] lexemes  The lexeme to begin counting at
 *  \return             The number of attributes in the entity. 0 is bad (all entities need at least a classname).
 */
_Success_(return != 0)
PRIVATE int MF_CountAttributes(_In_ MF_Lexeme_t* lexemes);

/** \brief              Counts the number of faces in a brush. Gets the number of opening parentheses (
 *                      that occur immediately after a newline character, stopping when an opening bracket
 *                      { is reached, or end of list
 *  \param[In] lexemes  The lexeme to begin counting at
 *  \return             The number of faces in the brush
 */
_Success_(return != 0)
PRIVATE int MF_CountFaces(_In_ MF_Lexeme_t* lexemes);

DLL BOOL MF_DestroyMap(_In_ MF_Map_t* map);

PRIVATE BOOL MF_DestroyEntity(_In_ MF_Entity_t* entity);

PRIVATE BOOL MF_DestroyKeyValuePair(_In_ MF_KeyValuePair_t* kvp);

PRIVATE BOOL MF_DestroyBrush(_In_ MF_Brush_t* brush);

PRIVATE BOOL MF_DestroyFace(_In_ MF_Face_t* face);