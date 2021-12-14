#pragma once

struct MF_TextureParameters
{
    float offsetX;
    float offsetY;
    float angle;
    float scaleX;
    float scaleY;
};

struct MF_Vertex
{
    float vertices[4];
};

struct MF_Face
{
    MF_Vertex facePoints[3];
    char* texture;
    MF_TextureParameters textureParameters;
};

struct MF_Brush
{
    size_t totalFaces;
    MF_Face* faces;
};

struct MF_KeyValuePair
{
    char* key;
    char* value;
};

struct MF_Entity
{
    char* classname;
    char* textures;
    char* origin;

    size_t totalAttributes;
    MF_KeyValuePair* attributes;

    size_t totalBrushes;
    MF_Brush* brushes;

};

struct MF_Map
{
    size_t totalItems;
    MF_Entity* items;
};