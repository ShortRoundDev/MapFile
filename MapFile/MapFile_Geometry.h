#pragma once

#include "MapFile_Types.h"
#include "framework.h"

#define MF_EPSILON (0.0001f)

typedef struct MF_Plane
{
	MF_Vector4_t position;
	MF_Vector4_t normal;
	float d;
} MF_Plane_t;

typedef struct MF_MeshVertex
{
	MF_Vector4_t vertex;
	MF_UV_t uv;
} MF_MeshVertex_t;

typedef union MF_Line
{
	// line segment
	struct
	{
		MF_Vector4_t a;
		MF_Vector4_t b;
	};

	// ray
	struct
	{
		MF_Vector4_t origin;
		MF_Vector4_t direction;
	};

	//todo: one for an infinite line??
} MF_Line_t, MF_Ray_t, MF_LineSegment_t;

_Success_(return)
DLL BOOL MF_GenerateMesh(_In_ MF_Map_t* map);

PRIVATE MF_Plane_t MF_MakePlaneFromFace(_In_ MF_Face_t face);

_Success_(return)
PRIVATE BOOL MF_IntersectPlanePlane(_In_ MF_Plane_t a, _In_ MF_Plane_t b, _Out_ MF_Line_t* line);

_Success_(return)
PRIVATE BOOL MF_Intersect3Planes(_In_ MF_Plane_t a, _In_ MF_Plane_t b, _In_ MF_Plane_t c, _Out_ MF_Vector4_t* point);

_Success_(return)
PRIVATE BOOL MF_IntersectPlaneLine(_In_ MF_Plane_t a, _In_ MF_Line_t b, _Out_ MF_Vector4_t* point);

PRIVATE MF_Vector4_t MF_CrossProductVector3(_In_ MF_Vector4_t a, _In_ MF_Vector4_t b);
PRIVATE float MF_DotProductVector3(_In_ MF_Vector4_t a, _In_ MF_Vector4_t b);
PRIVATE MF_Vector4_t MF_ScalarMultiplyVector4(_In_ MF_Vector4_t a, _In_ float  b);
PRIVATE MF_Vector4_t MF_AddVector4(_In_ MF_Vector4_t a, _In_ MF_Vector4_t b);
PRIVATE MF_Vector4_t MF_NegativeVector4(_In_ MF_Vector4_t a);
PRIVATE MF_Vector4_t MF_NormalizeVector3(_In_ MF_Vector4_t a);
PRIVATE float MF_Length(_In_ MF_Vector4_t a);

#ifdef __cplusplus
MF_Vector4_t operator + (MF_Vector4_t a, MF_Vector4_t b);

MF_Vector4_t operator -(MF_Vector4_t a);

MF_Vector4_t operator - (MF_Vector4_t a, MF_Vector4_t b);

MF_Vector4_t operator * (MF_Vector4_t a, float b);
#endif