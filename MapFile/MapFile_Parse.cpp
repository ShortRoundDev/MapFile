#include "MapFile_Parse.h"

#include "MapFile_System.h"
#include "MapFile_Lex.h"

_Success_(return)
bool MF_Parse(_In_ char* text, _Out_ MF_Map* map)
{
	MF_Lexeme* lexemes = MF_Lex(text);
	if (lexemes == NULL) {
		//error
		return false;
	}

	map->totalItems = MF_CountEntities(lexemes);
	if (map->totalItems == 0) {
		return false; // technically not false...
	}
	map->items = (MF_Entity*) MF_Alloc(sizeof(MF_Entity) * map->totalItems);

	MF_Lexeme* cursor = lexemes;
	MF_Entity* entityCursor = map->items;
	
	while (cursor != NULL)
	{
		//newline or comment
		if (cursor->lexeme[0] == '\n' || (cursor->lexeme[0] == '/' && cursor->lexeme[1] == '/')) {
			goto next;
		}
		//new entity
		if (cursor->lexeme[0] == '{') {
			cursor = MF_ParseEntity(cursor, entityCursor);
			entityCursor++;
		}
	next:
		cursor = cursor->next;
	}
}

_Success_(return != NULL)
MF_Lexeme* MF_ParseEntity(_In_ MF_Lexeme * lexemes, _Out_ MF_Entity * entity)
{

}

_Success_(return != NULL)
MF_Lexeme* MF_ParseKeyValuePair(_In_ MF_Lexeme * lexemes, _Out_ MF_KeyValuePair * keyValuePair)
{

}

_Success_(return != NULL)
MF_Lexeme* MF_ParseBrush(_In_ MF_Lexeme * lexemes, _Out_ MF_Brush * brush)
{

}

_Success_(return != NULL)
MF_Lexeme* MF_ParseFace(_In_ MF_Lexeme * lexemes, _Out_ MF_Face * face)
{

}

_Success_(return != NULL)
MF_Lexeme* MF_ParseVertex(_In_ MF_Lexeme * lexemes, _Out_ MF_Vertex * vertex)
{

}

_Success_(return != NULL)
MF_Lexeme* MF_ParseTextureParameters(_In_ MF_Lexeme * lexemes, _Out_ MF_TextureParameters * textureParameters)
{

}

_Success_(return)
int MF_CountEntities(_In_ MF_Lexeme * lexemes)
{
	int depth = 0;
	int total = 0;
	MF_Lexeme* cursor = lexemes;
	while (cursor != NULL)
	{
		if (cursor->lexeme[0] == '{')
		{
			depth++;
		}
		else if (cursor->lexeme[0] == '}')
		{
			depth--;
			if (depth == 0) {
				total++;
			}
		}
		cursor = cursor->next;
	}
	return total;
}