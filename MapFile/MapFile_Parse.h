#pragma once

#include "MapFile_Types.h"
#include "MapFile_List.h"

_Success_(return)
DLL bool MF_Parse(_In_ char* text, _Out_ MF_Map* map);

_Success_(return != NULL)
MF_Lexeme* MF_ParseEntity(_In_ MF_Lexeme* lexemes, _Out_ MF_Entity* entity);

_Success_(return != NULL)
MF_Lexeme* MF_ParseKeyValuePair(_In_ MF_Lexeme* lexemes, _Out_ MF_KeyValuePair* keyValuePair);

_Success_(return != NULL)
MF_Lexeme* MF_ParseBrush(_In_ MF_Lexeme* lexemes, _Out_ MF_Brush* brush);

_Success_(return != NULL)
MF_Lexeme* MF_ParseFace(_In_ MF_Lexeme* lexemes, _Out_ MF_Face* face);

_Success_(return != NULL)
MF_Lexeme* MF_ParseVertex(_In_ MF_Lexeme* lexemes, _Out_ MF_Vertex* vertex);

_Success_(return != NULL)
MF_Lexeme* MF_ParseTextureParameters(_In_ MF_Lexeme* lexemes, _Out_ MF_TextureParameters* textureParameters);

_Success_(return);
int MF_CountEntities(_In_ MF_Lexeme* lexemes);

_Success_(return);
int MF_CountAttributes(_In_ MF_Lexeme* lexemes);

_Success_(return);
int MF_CountBrushes(_In_ MF_Lexeme* lexemes);

_Success_(return);
int MF_CountFaces(_In_ MF_Lexeme* lexemes);