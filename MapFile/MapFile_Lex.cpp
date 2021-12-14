#include "MapFile_Lex.h"

#include <regex>

MF_Lexeme* MF_Lex(_In_    const   char*   string)
{
    MF_Lexeme* list = NULL;
    MF_Lexeme* cursor = NULL;
    char* c = (char*)string;
    while (*c != NULL) {
        char* token = NULL;
        size_t size = 0;

        if (MF_LexIsWhitespace(*c)) {
            c++;
            continue;
        }

        if (MF_LexIsMapFileSyntax(*c)) {
            token = c;
            size = 1;
            goto append;
        }
        
        size = MF_LexString(c);
        if (size != 0) {
            token = c;
            goto append;
        }

        size = MF_LexNumber(c);
        if (size != 0) {
            token = c;
            goto append;
        }

        size = MF_LexComment(c);
        if (size != 0) {
            token = c;
            goto append;
        }

        size = MF_LexWhitespacePaddedString(c);
        if (size != 0) {
            token = c;
            goto append;
        }

        if (size == 0) {
            printf("error");
            return NULL;
        }

    append:
        if (list == NULL) {
            list = MF_NewLexeme(token, size);
            cursor = list;
        }
        else
        {
            cursor = MF_AttachLexeme(cursor, token, size);
        }
        c += size;
    }
    return list;
}

bool MF_LexIsMapFileSyntax(char c)
{
    return
        c == '{' ||
        c == '}' ||
        c == '(' ||
        c == ')' ||
        c == '\n';
}

bool MF_LexIsWhitespace(char c)
{
    return
        c == ' ' ||
        c == '\t';
}


_Success_(return)
size_t MF_LexString(_In_ char* string)
{
    if (*string != '"') {
        return 0;
    }
    
    string++; // iterate past "

    size_t size = 1;
    while (*string != '"') {
        switch (*string) {
        case '\0': {
            break;
        }
        case '\\': {
            size += 2;
            string += 2;
            break;
        }
        default: {
            size++;
            string++;
        }
        }
    }

    return size + 1; // + 1 for the end "
}

size_t MF_LexNumber(_In_ char* string)
{
    size_t size = 0;
    while (MF_LexIsNumber(*string)) {
        string++;
        size++;
    }
    return size;
}

bool MF_LexIsNumber(char c)
{
    return (c >= '0' && c <= '9') || c == '.' || c == '-';
}

size_t MF_LexComment(_In_ char* string)
{
    if (string[0] != '/' || string[1] != '/') {
        return 0;
    }
    size_t t = 2;
    string += 2;
    while (*string != '\n') {
        t++;
        string++;
    }
    return t;
}

_Success_(return)
size_t MF_LexWhitespacePaddedString(_In_ char* string)
{
    size_t t = 1;
    while (!MF_LexIsWhitespace(*string)) {
        t++;
        string++;
    }
    return t;
}