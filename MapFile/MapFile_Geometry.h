#pragma once

#include "MapFile_Types.h"
#include "framework.h"

#ifdef __cplusplus
#include <map>
#include <string>
#include <vector>
#include <unordered_set>>
#endif

#define MF_EPSILON (0.0001f)

typedef struct MF_Plane
{
	MF_Vector3_t position;
	MF_Vector3_t normal;
	float d;
} MF_Plane_t;

typedef struct MF_MeshVertex
{
	MF_Vector3_t vertex; //   0 -  96
	MF_Vector3_t normal; //  96 - 192 (3 words)
	MF_UV_t uv;          // 192 - 256 (4 words)
	UINT texture;        // 256 - 288 (4.5 words)
} MF_MeshVertex_t;

typedef union MF_Line
{
	// line segment
	struct
	{
		MF_Vector3_t a;
		MF_Vector3_t b;
	};

	// ray
	struct
	{
		MF_Vector3_t origin;
		MF_Vector3_t direction;
	};

	//todo: one for an infinite line??
} MF_Line_t, MF_Ray_t, MF_LineSegment_t;


typedef union MF_IndexedTriangle {
	struct {
		struct {
			UINT a;
			UINT b;
			UINT c;
		};
	};
} MF_IndexedTriangle_t;

typedef struct MF_Mesh {
	char name[128];

	MF_MeshVertex_t* vertices;
	size_t totalVertices;

	UINT* indices;
	size_t totalIndices;
} MF_Mesh_t;

typedef struct MF_MeshEntity {
	char name[128];

	MF_Entity_t* rawEntity;

	char** textures;
	size_t totalTextures;

	MF_Mesh_t* meshes;
	size_t totalMeshes;
} MF_MeshEntity_t;

typedef struct MF_BrushDictionary {
	MF_MeshEntity_t* brushes;
	size_t totalBrushes;
} MF_BrushDictionary_t;

_Success_(return)
DLL BOOL MF_GenerateMesh(
	_In_ MF_Map_t* map,
	_Out_ MF_BrushDictionary_t* meshDictionary
);

PRIVATE MF_Plane_t MF_MakePlaneFromFace(_In_ MF_Face_t face);

_Success_(return)
PRIVATE BOOL MF_IntersectPlanePlane(_In_ MF_Plane_t a, _In_ MF_Plane_t b, _Out_ MF_Line_t* line);

_Success_(return)
PRIVATE BOOL MF_Intersect3Planes(_In_ MF_Plane_t a, _In_ MF_Plane_t b, _In_ MF_Plane_t c, _Out_ MF_Vector3_t* point);

_Success_(return)
PRIVATE BOOL MF_IntersectPlaneLine(_In_ MF_Plane_t a, _In_ MF_Line_t b, _Out_ MF_Vector3_t* point);


// ========== Vector3 ========== 
PRIVATE MF_Vector3_t MF_CrossProductVector3(_In_ MF_Vector3_t a, _In_ MF_Vector3_t b);
PRIVATE float MF_DotProductVector3(_In_ MF_Vector3_t a, _In_ MF_Vector3_t b);
PRIVATE MF_Vector3_t MF_ScalarMultiplyVector3(_In_ MF_Vector3_t a, _In_ float  b);
PRIVATE MF_Vector3_t MF_AddVector3(_In_ MF_Vector3_t a, _In_ MF_Vector3_t b);
PRIVATE MF_Vector3_t MF_NegativeVector3(_In_ MF_Vector3_t a);
PRIVATE MF_Vector3_t MF_NormalizeVector3(_In_ MF_Vector3_t a);
PRIVATE float MF_Length3(_In_ MF_Vector3_t a);

#ifdef __cplusplus
MF_Vector3_t operator + (MF_Vector3_t a, MF_Vector3_t b);
MF_Vector3_t operator - (MF_Vector3_t a);
MF_Vector3_t operator - (MF_Vector3_t a, MF_Vector3_t b);
MF_Vector3_t operator * (MF_Vector3_t a, float b);

bool operator == (const MF_Vector3_t& a, const MF_Vector3_t& b);
bool operator != (MF_Vector3_t a, MF_Vector3_t b);

bool operator == (const MF_IndexedTriangle_t& a, const MF_IndexedTriangle_t& b);

std::ostream& operator << (std::ostream& os, MF_Vector3 const& vec);
#endif


// ========== Vector2 ========== 
PRIVATE float MF_DotProductVector2(_In_ MF_Vector2_t a, _In_ MF_Vector2_t b);
PRIVATE MF_Vector2_t MF_ScalarMultiplyVector2(_In_ MF_Vector2_t a, _In_ float  b);
PRIVATE MF_Vector2_t MF_AddVector2(_In_ MF_Vector2_t a, _In_ MF_Vector2_t b);
PRIVATE MF_Vector2_t MF_NegativeVector2(_In_ MF_Vector2_t a);
PRIVATE MF_Vector2_t MF_NormalizeVector2(_In_ MF_Vector2_t a);
PRIVATE float MF_Length2(_In_ MF_Vector2_t a);

#ifdef __cplusplus
MF_Vector2_t operator + (MF_Vector2_t a, MF_Vector2_t b);
MF_Vector2_t operator - (MF_Vector2_t a);
MF_Vector2_t operator - (MF_Vector2_t a, MF_Vector2_t b);
MF_Vector2_t operator * (MF_Vector2_t a, float b);

bool operator == (const MF_Vector2_t& a, const MF_Vector2_t& b);
bool operator != (MF_Vector2_t a, MF_Vector2_t b);

std::ostream& operator << (std::ostream& os, MF_Vector2 const& vec);
#endif