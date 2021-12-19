#include "MapFile_Parse.h"

#include "MapFile_Lex.h"

#include <cctype>
#include <math.h>

#define WHITE_OR_COMMENT(s) ((s)[0] == '\n' || ((s)[0] == '/' && (s)[1] == '/'))

_Success_(return)
BOOL MF_Parse(_In_ char* text, _Out_ MF_Map* map)
{
	MF_ResetErr();
	MF_Lexeme* lexemes = MF_Lex(text);
	if (lexemes == NULL) {
		// error
		// raise nothing, Lex has already raised an error
		return false;
	}

	MF_Lexeme* cursor = lexemes;

	map->totalItems = MF_CountEntities(lexemes);
	if (map->totalItems == 0) {
		MF_Raise(MF_PARSE_NO_ENTS_CODE, MF_PARSE_NO_ENTS_MSG);
		MF_DestroyLexemeList(lexemes);
		return false;
	}
	map->items = (MF_Entity*) MF_Alloc(sizeof(MF_Entity) * map->totalItems);
	MF_Entity* entityCursor = map->items;
	
	while(cursor != NULL)
	{
		//newline or comment
		if (WHITE_OR_COMMENT(cursor->lexeme) || cursor->lexeme[0] == '}') {
			goto next;
		}
		//new entity
		if (cursor->lexeme[0] == '{') {
			cursor = MF_ParseEntity(cursor->next, entityCursor);
			entityCursor++;
		}
		else {
			MF_Raise(MF_PARSE_MISALIGNED_ENT_CODE, MF_PARSE_MISALIGNED_ENT_MSG);
			break;
		}
	next:
		if (cursor == NULL) {
			break;
		}
		cursor = cursor->next;
	}
	MF_DestroyLexemeList(lexemes);
	if (MF_Err) {
		return false;
	}
	return true;
}

_Success_(return != NULL)
MF_Lexeme* MF_ParseEntity(_In_ MF_Lexeme * lexemes, _Out_ MF_Entity * entity)
{
	entity->totalAttributes = MF_CountAttributes(lexemes);
	if (entity->totalAttributes == 0) {
		MF_Raise(MF_PARSE_ENT_NO_ATT_CODE, MF_PARSE_ENT_NO_ATT_MSG);
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
		return cursor;
	}
	entity->brushes = (MF_Brush*)MF_Alloc(entity->totalBrushes * sizeof(MF_Brush));
	MF_Brush* brushCursor = entity->brushes;
	i = 0;
	while (cursor != NULL && i < entity->totalBrushes) {
		if (WHITE_OR_COMMENT(cursor->lexeme)) {
			goto next;
		}
		if (cursor->lexeme[0] == '{') {
			cursor = MF_ParseBrush(cursor->next, brushCursor);
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
		MF_Raise(MF_PARSE_MISALIGNED_ATT_CODE, MF_PARSE_MISALIGNED_ATT_MSG);
		return NULL;
	}

	size_t keyLength = strlen(lexemes->lexeme);
	char* key = (char*)MF_Alloc(keyLength - 1); // -2 for quote marks; +1 for null terminator
	ZeroMemory(key, keyLength - 1);
	memcpy(key, lexemes->lexeme + 1, keyLength - 2);
	keyValuePair->key = key;

	lexemes = lexemes->next;

	if (lexemes->lexeme[0] != '"')
	{
		MF_Raise(MF_PARSE_MISALIGNED_ATT_CODE, MF_PARSE_MISALIGNED_ATT_MSG);
		return NULL;
	}

	size_t valueLength = strlen(lexemes->lexeme);
	char* value = (char*)MF_Alloc(valueLength - 1);
	ZeroMemory(value, valueLength - 1);
	memcpy(value, lexemes->lexeme + 1, valueLength - 2);
	keyValuePair->value = value;
	
	return lexemes->next; // newline probably
}

_Success_(return != NULL)
MF_Lexeme* MF_ParseBrush(_In_ MF_Lexeme * lexemes, _Out_ MF_Brush * brush)
{
	int totalFaces = MF_CountFaces(lexemes);
	brush->totalFaces = totalFaces;
	brush->faces = (MF_Face*)MF_Alloc(sizeof(MF_Face) * totalFaces);
	int i = 0;

	MF_Face* faceCursor = brush->faces;
	MF_Lexeme* cursor = lexemes;
	while (cursor != NULL && cursor->lexeme[0] != '}' && i < totalFaces) { //exit at end of object or faces all parsed
		if (WHITE_OR_COMMENT(cursor->lexeme))
		{
			goto next;
		}
		else if (cursor->lexeme[0] == '(') {
			cursor = MF_ParseFace(cursor->next, faceCursor);
			i++;
			faceCursor++;
		}
		else {
			MF_Raise(MF_PARSE_MISALIGNED_BRUSH_PAREN_CODE, MF_PARSE_MISALIGNED_BRUSH_PAREN_MSG);
			return NULL;
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
MF_Lexeme* MF_ParseFace(_In_ MF_Lexeme * lexemes, _Out_ MF_Face * face)
{

	MF_Lexeme* cursor = lexemes;

	for (int i = 0; i < 3; i++) {
		cursor = MF_ParseVertex(cursor, face->facePoints + i);
		
		if (cursor == NULL) {
			return NULL;
		}
	}

	size_t texNameLength = strlen(cursor->lexeme);
	face->texture = (char*)MF_Alloc(texNameLength) + 1; // + 1 for NULL terminator
	memset(face->texture, 0, texNameLength + 1);
	memcpy(face->texture, cursor->lexeme, texNameLength);

	cursor = cursor->next;
	
	if (cursor == NULL) {
		MF_Raise(MF_PARSE_REACHED_END_CODE, MF_PARSE_REACHED_END_MSG);
		return NULL;
	}
	
	cursor = MF_ParseTextureParameters(cursor, &(face->textureParameters));

	return cursor;
}

_Success_(return != NULL)
MF_Lexeme* MF_ParseVertex(_In_ MF_Lexeme * lexemes, _Out_ MF_Vertex * vertex)
{
	MF_Lexeme* cursor = lexemes;
	int i = 0;
	while (cursor != NULL && cursor->lexeme[0] != ')')
	{
		if (WHITE_OR_COMMENT(cursor->lexeme) || cursor->lexeme[0] == '(')
		{
			goto next;
		}
		vertex->comp[i++] = (float)atof(cursor->lexeme);

	next:
		cursor = cursor->next;
	}
	if (cursor == NULL) {
		MF_Raise(MF_PARSE_REACHED_END_CODE, MF_PARSE_REACHED_END_MSG);
		return NULL;
	}
	vertex->comp[3] = 0.0f; // init 4th to 0. using float4 for alignment
	return cursor->next;
}

_Success_(return != NULL)
MF_Lexeme* MF_ParseTextureParameters(_In_ MF_Lexeme * lexemes, _Out_ MF_TextureParameters * textureParameters)
{
	textureParameters->offsetX = (float)atof(lexemes->lexeme);
	lexemes = lexemes->next;

	textureParameters->offsetY = (float)atof(lexemes->lexeme);
	lexemes = lexemes->next;

	textureParameters->angle = (float)atof(lexemes->lexeme);
	lexemes = lexemes->next;

	textureParameters->scaleX = (float)atof(lexemes->lexeme);
	lexemes = lexemes->next;

	textureParameters->scaleY = (float)atof(lexemes->lexeme);

	return lexemes->next; //newline
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
	while (cursor != NULL && cursor->lexeme[0] != '{' && cursor->lexeme[0] != '}')
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

_Success_(return);
int MF_CountFaces(_In_ MF_Lexeme* lexemes)
{
	bool primed = false;
	int total = 0;
	while (lexemes != NULL && lexemes->lexeme[0] != '}') // until end of brush
	{
		if (lexemes->lexeme[0] == '\n') {
			primed = true;
		}
		else if (primed && lexemes->lexeme[0] == '(') {
			total++;
			primed = false;
		}
		lexemes = lexemes->next;
	}
	return total;
}