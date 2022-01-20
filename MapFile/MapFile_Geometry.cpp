#include "MapFile_Geometry.h"

#include <cmath>
#include <iostream>
#include <vector>

_Success_(return)
BOOL MF_GenerateMesh(_In_ MF_Map* map)
{
	MF_Vector4_t points[1000];
	int pointCursor = 0;

	for (int i = 0; i < map->totalItems; i++) {
		for (int j = 0; j < map->items[i].totalBrushes; j++) {
			auto brush = map->items[i].brushes[j];

			for (int k = 0; k < brush.totalFaces; k++) {
				for (int l = 0; l < brush.totalFaces; l++) {
					for (int m = 0; m < brush.totalFaces; m++) {
						if (k == l || k == m || l == m){
							continue;
						}
						bool illegal = false;

						auto face1 = MF_MakePlaneFromFace(brush.faces[k]);
						auto face2 = MF_MakePlaneFromFace(brush.faces[l]);
						auto face3 = MF_MakePlaneFromFace(brush.faces[m]);

						MF_Vector4_t point;

						if (MF_Intersect3Planes(face1, face2, face3, &point)) {
							for (int n = 0; n < brush.totalFaces; n++) {
								auto plane = MF_MakePlaneFromFace(brush.faces[n]);
								auto dp = MF_DotProductVector3(plane.normal, point - plane.position); // translate point into plane's normal-space
								if (dp > MF_EPSILON) {
									illegal = true;
								}
							}
							if (!illegal) {
								points[pointCursor++] = point;
							}
						}
					}
				}
			}
		}
		for (int i = 0; i < pointCursor; i++) {
			auto p = points[i];
			std::cout << p.x << ", " << p.y << ", " << p.z << std::endl;
		}
	}
	
	return TRUE;
}


PRIVATE MF_Plane_t MF_MakePlaneFromFace(_In_ MF_Face_t face)
{
	MF_Vector4_t u = face.facePoints[2] - face.facePoints[0];
	MF_Vector4_t v = face.facePoints[1] - face.facePoints[0];

	MF_Vector4_t normal = MF_NormalizeVector3(MF_CrossProductVector3(u, v));

	MF_Plane_t plane = {
		face.facePoints[0],
		normal,
		MF_DotProductVector3(normal, face.facePoints[0])
	};

	return plane;
}

_Success_(return)
PRIVATE BOOL MF_IntersectPlanePlane(_In_ MF_Plane_t a, _In_ MF_Plane_t b, _Out_ MF_Line_t* line)
{
	MF_Vector4_t direction = MF_CrossProductVector3(a.normal, b.normal);

	if (MF_DotProductVector3(direction, direction) < MF_EPSILON)
		return FALSE;

	float d11 = MF_DotProductVector3(a.normal, a.normal);
	float d12 = MF_DotProductVector3(a.normal, b.normal);
	float d22 = MF_DotProductVector3(b.normal, b.normal);

	float denom = d11 * d22 - d12 * d12;
	float a_dot = MF_DotProductVector3(a.normal, a.position);
	float b_dot = MF_DotProductVector3(b.normal, b.position);

	float k1 = (a_dot * d22 - b_dot * d12) / denom;
	float k2 = (b_dot * d11 - a_dot * d12) / denom;

	MF_Vector4_t origin = a.normal * k1 + b.normal * k2;

	line->direction = direction;
	line->origin = origin;

	return TRUE;
}

_Success_(return)
BOOL MF_Intersect3Planes(_In_ MF_Plane_t a, _In_ MF_Plane_t b, _In_ MF_Plane_t c, _Out_ MF_Vector4_t * point)
{
	/*MF_Vector4_t	n1 = a.normal,
					n2 = b.normal,
					n3 = c.normal;*/

	MF_Vector4	m1 = { a.normal.x, b.normal.x, c.normal.x },
		m2 = { a.normal.y, b.normal.y, c.normal.y },
		m3 = { a.normal.z, b.normal.z, c.normal.z };

	MF_Vector4 u = MF_CrossProductVector3(m2, m3);

	float denom = MF_DotProductVector3(m1, u);

	if (std::fabs(denom) < MF_EPSILON) {
		return FALSE;
	}

	MF_Vector4 d = { a.d, b.d, c.d };

	MF_Vector4 v = MF_CrossProductVector3(m1, d);
	
	float ood = 1.0f / denom;

	*point = {
		MF_DotProductVector3(d, u) * ood,
		MF_DotProductVector3(m3, v) * ood,
		-MF_DotProductVector3(m2, v) * ood,
		0.0f
	};

	return TRUE;
}

_Success_(return)
BOOL MF_IntersectPlaneLine(_In_ MF_Plane_t p, _In_ MF_Line_t line, _Out_ MF_Vector4_t* point)
{
	MF_Vector4_t	a = line.origin,
					b = line.origin + line.direction;

	MF_Vector4_t ab = b - a;

	float a_dot = MF_DotProductVector3(p.normal, p.position);

	float t = (a_dot - MF_DotProductVector3(p.normal, a)) / (MF_DotProductVector3(p.normal, ab));
	if (t > 100000.0f || t < -100000.0f || !std::isfinite<float>(t)) {
		return FALSE;
	}

	*point = a + (ab * t);

	return TRUE;
}


MF_Vector4_t MF_CrossProductVector3(_In_ MF_Vector4_t a, _In_ MF_Vector4_t b)
{
	MF_Vector4_t c = { 0, 0, 0, 0 };
	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;
	c.w = 0.0f;
	return c;
}

float MF_DotProductVector3(_In_ MF_Vector4_t a, _In_ MF_Vector4_t b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

MF_Vector4_t MF_AddVector4(_In_ MF_Vector4_t a, _In_ MF_Vector4_t b)
{
	return {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
		a.w + b.w
	};
}

MF_Vector4_t MF_NegativeVector4(_In_ MF_Vector4_t a)
{
	return {
		-a.x,
		-a.y,
		-a.z,
		-a.w
	};
}

float MF_Length(_In_ MF_Vector4_t a)
{
	return std::sqrtf((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

MF_Vector4_t MF_ScalarMultiplyVector4(_In_ MF_Vector4_t a, _In_ float b)
{
	return {
		a.x * b,
		a.y * b,
		a.z * b,
		a.w * b
	};
}

MF_Vector4_t MF_NormalizeVector3(_In_ MF_Vector4_t a)
{
	float len = MF_Length(a);
	if (len == 0.0f) {
		return { 0, 0, 0, 0 };
	}
	return MF_ScalarMultiplyVector4(a, 1.0f / MF_Length(a));
}

MF_Vector4_t operator + (MF_Vector4_t a, MF_Vector4_t b)
{
	return MF_AddVector4(a, b);
}

MF_Vector4_t operator -(MF_Vector4_t a)
{
	return MF_NegativeVector4(a);
}

MF_Vector4_t operator - (MF_Vector4_t a, MF_Vector4_t b)
{
	return MF_AddVector4(a, -b);
}

MF_Vector4_t operator * (MF_Vector4_t a, float b)
{
	return MF_ScalarMultiplyVector4(a, b);
}
