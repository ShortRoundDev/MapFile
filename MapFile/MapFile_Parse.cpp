#include "MapFile_Parse.h"

#include "MapFile_System.h"
#include "MapFile_Lex.h"

#define WHITE_OR_COMMENT(s) ((s)[0] == '\n' || ((s)[0] == '/' && (s)[1] == '/'))

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
	
	while(cursor != NULL)
	{
		//newline or comment
		if (WHITE_OR_COMMENT(cursor->lexeme)) {
			goto next;
		}
		//new entity
		if (cursor->lexeme[0] == '{') {
			cursor = MF_ParseEntity(cursor->next, entityCursor);
			entityCursor++;
		}
	next:
		if (cursor == NULL) {
			break;
		}
		cursor = cursor->next;
	}
	return true;
}

_Success_(return != NULL)
MF_Lexeme* MF_ParseEntity(_In_ MF_Lexeme * lexemes, _Out_ MF_Entity * entity)
{
	entity->totalAttributes = MF_CountAttributes(lexemes);
	if (entity->totalAttributes == 0) {
		return NULL;
	}
	entity->attributes = (MF_KeyValuePair*)MF_Alloc(entity->totalAttributes * sizeof(MF_KeyValuePair));
	MF_Lexeme* cursor = lexemes;
	MF_KeyValuePair* kvpCursor = entity->attributes;
	int i = 0;
	while(cursor != NULL && i < entity->totalAttributes)
	{
		if (WHITE_OR_COMMENT(cursor->lexeme))
		{
			cursor = cursor->next;
			continue;
		}
		cursor = MF_ParseKeyValuePair(cursor, kvpCursor);
		kvpCursor++;
		i++;
	}

	if (cursor == NULL) {
		return NULL;
	}

	entity->totalBrushes = MF_CountEntities(cursor);
	if (entity->totalBrushes == 0) {
		return NULL;
	}
	entity->brushes = (MF_Brush*)MF_Alloc(entity->totalBrushes * sizeof(MF_Brush));
	MF_Brush* brushCursor = entity->brushes;
	i = 0;
	while (cursor != NULL && i < entity->totalBrushes) {
		if (WHITE_OR_COMMENT(cursor->lexeme)) {
			goto next;
		}
		if (cursor->lexeme[0] == '{') {
			cursor = MF_ParseBrush(cursor, brushCursor);
			brushCursor++;
			i++;
		}
	next:
		if (cursor == NULL) {
			break;
		}
		cursor = cursor->next;
	}

	return cursor;
}

_Success_(return != NULL)
MF_Lexeme* MF_ParseKeyValuePair(_In_ MF_Lexeme* lexemes, _Out_ MF_KeyValuePair* keyValuePair)
{
	if (lexemes->lexeme[0] != '"')
	{
		return NULL;
	}

	int keyLength = strlen(lexemes->lexeme);
	char* key = (char*)MF_Alloc(keyLength - 1); // -2 for quote marks; +1 for null terminator
	ZeroMemory(key, keyLength - 1);
	memcpy(key, lexemes->lexeme + 1, keyLength - 2);
	keyValuePair->key = key;

	lexemes = lexemes->next;

	if (lexemes->lexeme[0] != '"')
	{
		return NULL;
	}

	int valueLength = strlen(lexemes->lexeme);
	char* value = (char*)MF_Alloc(valueLength - 1);
	ZeroMemory(value, valueLength - 1);
	memcpy(value, lexemes->lexeme + 1, valueLength - 2);
	keyValuePair->value = value;
	
	return lexemes->next; // newline probably
}

_Success_(return != NULL)
MF_Lexeme* MF_ParseBrush(_In_ MF_Lexeme * lexemes, _Out_ MF_Brush * brush)
{
	
}

_Success_(return != NULL)
MF_Lexeme* MF_ParseFace(_In_ MF_Lexeme * lexemes, _Out_ MF_Face * face)
{
	return NULL;
}

_Success_(return != NULL)
MF_Lexeme* MF_ParseVertex(_In_ MF_Lexeme * lexemes, _Out_ MF_Vertex * vertex)
{
	return NULL;
}

_Success_(return != NULL)
MF_Lexeme* MF_ParseTextureParameters(_In_ MF_Lexeme * lexemes, _Out_ MF_TextureParameters * textureParameters)
{
	return NULL;
}

_Success_(return)
int MF_CountEntities(_In_ MF_Lexeme* lexemes)
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

_Success_(return);
int MF_CountAttributes(_In_ MF_Lexeme* lexemes)
{
	int x = 0;
	int total = 0;

	MF_Lexeme* cursor = lexemes;
	while (cursor != NULL && cursor->lexeme[0] != '{')
	{
		if (WHITE_OR_COMMENT(cursor->lexeme))
		{
			goto next;
		}
		if (cursor->lexeme[0] == '"') {
			x++;
			if (x % 2 == 0) {
				total++;
			}
		}
	next:
		cursor = cursor->next;
	}

	return total;
}