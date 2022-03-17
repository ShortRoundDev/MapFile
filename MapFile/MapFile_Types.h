/** \file   MapFile_Types.h
 *  \brief  The core types which define the data in a Map file
 */

#pragma once
/** \brief     A set of floats for deriving the UV Map of the texture on the face. */
typedef struct MF_TextureParameters
{
    /** \brief     The offset (x) of the texture. */
    float offsetX; // TODO: Determine how this is used by trenchbroom, precisely
    /** \brief     The offset (x) of the texture. */
    float offsetY; // TODO: Determine how this is used by trenchbroom, precisely
    /** \brief     The rotation of the texture, in degrees [0, 360] */
    float angle;
    /** \brief     The scale (x) of the texture
     */
    float scaleX;
    /** \brief     The scale (y) of the texture */
    float scaleY;
} MF_TextureParameters_t;


/** \brief     A vector of 4 floats.
 * 
 *  \details   This was designed to be closely cast-able to other vec4
 *             representations. Though no face in the brush actually contains 4 components,
 *             this uses 4 for struct packing reasons. Additionally, many users in graphics
 *             environments tend to send float4s for everything (for both compatibility with
 *             transformation matrices as well as packing reasons). The last component is
 *             initialized to 0
 *  
 *             The floats can be accessed by component (vector.x, vector.y, vector.z, vector.w)
 *             or through the comp array (vector.comp[0], vector.comp[1], etc.)
 */
typedef union MF_Vector3
{
    struct // I think this is UB in C, but not C++
    {
        float x;
        float y;
        float z;
    };
    float comp[3]; // components
} MF_Vector3_t;

/** \brief      A face computed from the intersections of the MF_Faces */
typedef struct MF_GeoFace
{
    MF_Vector3_t* vertices;
    int totalVertices;
} MF_GeoFace_t;

typedef union MF_Vector2
{
    struct {
        float x;
        float y;
    };

    struct {
        float u;
        float v;
    };
    
    float comp[2];
} MF_Vector2_t, MF_UV_t;

/** \brief     A single face of the brush, which includes the points of the brush as well as
 *             texture information for that face
 */
typedef struct MF_Face
{
    /** \brief     The 3 vertices which make up the triangle along the bounding plane */
    MF_Vector3_t facePoints[3];
    /** \brief     The name of the texture */
    char* texture;
    /** \brief     A set of parameters which make up information about the UV Map of the triangle
     *
     *  \details   Unfortunately, because there are no actual mesh triangles in the map brush,
     *             these parameters are angles and pixel offsets rather than strict UV coordinates
     */
    MF_TextureParameters_t textureParameters;
} MF_Face_t;
/** \brief     A brush. A wall, floor, ceiling, slope, etc.
 *
 *  \details   Brushes are convex polyhedra made up of a set of bounding planes, which are in turn
 *             represented by a single triangle which lies along that plane.
 *
 *             Each plane needs to be collided with each other to retrieve a point cloud which make up
 *             the actual mesh, which can be triangulated from there
 */
typedef struct MF_Brush
{
    /** \brief     The number of faces in the brush */
    size_t totalFaces;
    /** \brief     A contiguous set of faces in memory */
    MF_Face_t* faces;
} MF_Brush_t;
/** \brief     An attribute in the entity definition. Both Key and Value are allocated with MF_Alloc,
 *             and so need to be destroyed individually. */
typedef struct MF_KeyValuePair
{
    /** \brief     The key of the attribute */
    char* key;
    /** \brief     The value of the attribute */
    char* value;
} MF_KeyValuePair_t;
/** \brief     An object in the game; either a true entity (some item, an enemy, etc.)
 *             or a brush (a wall, a floor, etc.) */
typedef struct MF_Entity
{
    /** \brief     The name of the entity type.
     * 
     *  \details   This is specifically parsed out of the attributes,
     *             and points to the same area of memory as the value in the Key Value Pair with the
     *             "classname" key. Do not destroy this pointer manually, as it should be destroyed
     *             when the attributes are cleaned up
     */
    char* classname;
    /** \brief     The starting position of the entity, as a string of 3 floats (a vertex).
     *
     *  \details   This is
     *             specifically parsed out of the attributes, and points to the same area of memory
     *             as the value in the Key Value Pair with the "origin" key. Do not destroy this
     *             pointer manually, as it should be destroyed when the attributes are cleaned up
     */
    char* origin;
    /** \brief     The total number of Key Value Pairs in the attributes pointer */
    size_t totalAttributes;
    /** \brief     A contiguous set of key value pairs in memory */
    MF_KeyValuePair_t* attributes;
    /** \brief     The total number of brushes in the brushes pointer */
    size_t totalBrushes;
    /** \brief     A contiguous set of brushes in memory */
    MF_Brush_t* brushes;
} MF_Entity_t;
/** \brief     The main map struct. */
typedef struct MF_Map
{
    /** \brief     The total number of entities in the items pointer */
    size_t totalItems;
    /** \brief     A contiguous set of entities in memory */
    MF_Entity_t* items;
} MF_Map_t;