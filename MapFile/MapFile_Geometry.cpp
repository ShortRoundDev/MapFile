#include "MapFile_System.h"
#include "MapFile_Geometry.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_set>

// Private declarations. Don't want this in the .C file
typedef struct MF_VertexHasher {
public:
	size_t operator()(const MF_Vector3_t& vert) const {
		return    (UINT64)(vert.x * 73856093)
			^ (UINT64)(vert.y * 19349663)
			^ (UINT64)(vert.z * 83492791);
	}
} MF_VertexHasher_t;


typedef struct MF_FaceDTO {
	MF_Vector3_t normal;
	std::vector<MF_Vector3_t> points;
	MF_Face* rawFace;
} MF_FaceDTO_t;

_Success_(return)
BOOL _MF_GenerateFacesUnordered(
	_In_ MF_Map_t * map,
	_In_ std::map<std::string, std::vector<MF_FaceDTO>>*points,
	_In_ MF_Vector3_t * centroid
);

void MF_FixCrossedQuad(_In_ MF_FaceDTO * face);
void MF_FixOrientation(_In_ MF_FaceDTO * face);
void MF_TriangulateFace(
	_In_ MF_FaceDTO * face,
	_In_ std::unordered_set<MF_Vector3_t, MF_VertexHasher>*uniqueBrushVertices,
	_In_ std::vector<MF_Vector3_t>*orderedBrushVertices,
	_In_ std::vector<MF_IndexedTriangle_t>*brushIndices
);
void MF_UVMapFace(_In_ MF_FaceDTO * face);

BOOL MF_PolygonEdgeIsExterior(
	std::vector<MF_Vector3_t> face,
	MF_Vector3_t a,
	MF_Vector3_t b,
	MF_Vector3_t planeNormal
);
// End private declarations

void _MF_Dedupe_Vertices(const std::vector<MF_Vector3_t>& vertices, std::vector<MF_Vector3_t>& unique)
{
	//Have to de-dupe here because I'm not sure how to correctly order a
	//unique std::vector in c++, and std::unique requires a sorted list to
	//really work properly
	//so this is a slow version
	for (auto vertex : vertices) {
		bool exists = false;
		for (auto existing : unique) {
			if (existing == vertex) {
				exists = true;
				break;
			}
		}
		if (!exists) {
			unique.push_back(vertex);
		}
	}
}

_Success_(return)
BOOL MF_GenerateMesh(_In_ MF_Map_t* map, _Out_ MF_BrushDictionary* meshDictionary)
{
	if (meshDictionary == NULL) {
		return false;
	}

	//Each key is the name of the brush
	std::map<std::string, std::vector<MF_FaceDTO>> unorderedBrushFaces;
	MF_Vector3_t centroid = { 0, 0, 0 };	

	// Convert all MF_Brushes into sets of polygon edges grouped by face
	if (!_MF_GenerateFacesUnordered(map, &unorderedBrushFaces, &centroid)) {
		return FALSE;
	}

	//meshes = (MF_Mesh*)MF_Alloc(sizeof(MF_Mesh) * unorderedBrushFaces.size());

	int currentBrush = 0;
	//debug
	for (auto & brush : unorderedBrushFaces) {

		std::unordered_set<MF_Vector3_t, MF_VertexHasher> uniqueBrushVertices;
		std::vector<MF_Vector3_t> orderedBrushVertices;
		std::vector<MF_IndexedTriangle_t> brushIndices;

		//CopyMemory((*meshes)[currentBrush].name, brush.first.c_str(), brush.first.length() + 1); // + 1 for null terminator
		int triangle = 0;

		for (auto & face : brush.second) {
			if (face.points.size() > 3) {

				// Simplify polygons
				MF_FixCrossedQuad(&face);
				// Re-orient to ccw
				MF_FixOrientation(&face);
				//MF_UVMapFace
				// Triangulate
				MF_TriangulateFace(&face, &uniqueBrushVertices, &orderedBrushVertices, &brushIndices);
				
			}
		}
		/*(*meshes)[currentBrush].vertices = (MF_Vector3_t*)MF_Alloc(sizeof(MF_Vector3_t) * orderedBrushVertices.size());
		(*meshes)[currentBrush].uv = (MF_Vector3_t*)MF_Alloc(sizeof(MF_Vector2_t) * orderedBrushVertices.size());
		(*meshes)[currentBrush].indices = (MF_IndexedTriangle_t*)MF_Alloc(sizeof(MF_IndexedTriangle_t) * brushIndices.size());*/

//		std::copy(orderedBrushVertices.begin(), orderedBrushVertices.end(), (*meshes)[currentBrush].vertices);
	//	std::copy(brushIndices.begin(), brushIndices.end(), (*meshes)[currentBrush].indices);

		currentBrush++;
	}

	return TRUE;
}

_Success_(return)
BOOL _MF_GenerateFacesUnordered(
	_In_ MF_Map_t* map,
	_In_ std::map<std::string, std::vector<MF_FaceDTO>>* brushes,
	_In_ MF_Vector3_t* centroid
)
{
	int vertices = 0;
	for (int i = 0; i < map->totalItems; i++) {
		for (int j = 0; j < map->items[i].totalBrushes; j++) {

			auto brush = map->items[i].brushes[j];

			std::vector<MF_FaceDTO> faces(brush.totalFaces);

			for (int k = 0; k < brush.totalFaces; k++) {
				auto face1 = MF_MakePlaneFromFace(brush.faces[k]);
				faces[k].normal = face1.normal;
				faces[k].rawFace = brush.faces + k;

				for (int l = 0; l < brush.totalFaces; l++) {
					for (int m = 0; m < brush.totalFaces; m++) {
						if (k == l || k == m || l == m) {
							continue;
						}

						bool illegal = false;


						auto face2 = MF_MakePlaneFromFace(brush.faces[l]);
						auto face3 = MF_MakePlaneFromFace(brush.faces[m]);

						MF_Vector3_t point;

						if (MF_Intersect3Planes(face1, face2, face3, &point)) {
							for (int n = 0; n < brush.totalFaces; n++) {
								auto plane = MF_MakePlaneFromFace(brush.faces[n]);
								auto dp = MF_DotProductVector3(plane.normal, point - plane.position); // translate point into plane's normal-space
								if (dp > MF_EPSILON) {
									illegal = true;
								}
							}
							if (!illegal) {
								faces[k].points.push_back(point);
							}
						}
					}
				}
			}

			for (int i = 0; i < faces.size(); i++) {
				std::vector<MF_Vector3_t> unique;
				_MF_Dedupe_Vertices(faces[i].points, unique);
				faces[i].points = unique;

				for (auto vertex : faces[i].points) {
					*centroid = *centroid + vertex;
					vertices++;
				}
			}

			brushes->insert({ std::string(map->items[i].classname), faces });
		}
	}

	*centroid = *centroid * (1.0f/((float)vertices));

	return TRUE;
}

PRIVATE MF_Plane_t MF_MakePlaneFromFace(_In_ MF_Face_t face)
{
	MF_Vector3_t u = face.facePoints[2] - face.facePoints[0];
	MF_Vector3_t v = face.facePoints[1] - face.facePoints[0];

	MF_Vector3_t normal = MF_NormalizeVector3(MF_CrossProductVector3(u, v));

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
	MF_Vector3_t direction = MF_CrossProductVector3(a.normal, b.normal);

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

	MF_Vector3_t origin = a.normal * k1 + b.normal * k2;

	line->direction = direction;
	line->origin = origin;

	return TRUE;
}

_Success_(return)
BOOL MF_Intersect3Planes(_In_ MF_Plane_t a, _In_ MF_Plane_t b, _In_ MF_Plane_t c, _Out_ MF_Vector3_t * point)
{
	MF_Vector3	m1 = { a.normal.x, b.normal.x, c.normal.x },
		m2 = { a.normal.y, b.normal.y, c.normal.y },
		m3 = { a.normal.z, b.normal.z, c.normal.z };

	MF_Vector3 u = MF_CrossProductVector3(m2, m3);

	float denom = MF_DotProductVector3(m1, u);

	if (std::fabs(denom) < MF_EPSILON) {
		return FALSE;
	}

	MF_Vector3 d = { a.d, b.d, c.d };

	MF_Vector3 v = MF_CrossProductVector3(m1, d);
	
	float ood = 1.0f / denom;

	*point = {
		MF_DotProductVector3(d, u) * ood,
		MF_DotProductVector3(m3, v) * ood,
		-MF_DotProductVector3(m2, v) * ood
	};

	return TRUE;
}

_Success_(return)
BOOL MF_IntersectPlaneLine(_In_ MF_Plane_t p, _In_ MF_Line_t line, _Out_ MF_Vector3_t* point)
{
	MF_Vector3_t	a = line.origin,
					b = line.origin + line.direction;

	MF_Vector3_t ab = b - a;

	float a_dot = MF_DotProductVector3(p.normal, p.position);

	float t = (a_dot - MF_DotProductVector3(p.normal, a)) / (MF_DotProductVector3(p.normal, ab));
	if (t > 100000.0f || t < -100000.0f || !std::isfinite<float>(t)) {
		return FALSE;
	}

	*point = a + (ab * t);

	return TRUE;
}
void MF_UVMapFace(_In_ MF_FaceDTO* face) {

}

void MF_TriangulateFace(
	_In_ MF_FaceDTO* face,
	_In_ std::unordered_set<MF_Vector3_t, MF_VertexHasher>* uniqueBrushVertices,
	_In_ std::vector<MF_Vector3_t>* orderedBrushVertices,
	_In_ std::vector<MF_IndexedTriangle_t>* brushIndices
) {
	for (auto point : face->points) {
		if (uniqueBrushVertices->insert(point).second) {
			orderedBrushVertices->push_back(point);
		}
	}

	//get index in vertex set
	UINT origin = (UINT)std::distance(
		orderedBrushVertices->begin(),
		std::find(
			orderedBrushVertices->begin(),
			orderedBrushVertices->end(),
			face->points[0]
		)
	);
	for (int i = 0; i < face->points.size() - 2; i++) {

		auto b = (UINT)std::distance(
			orderedBrushVertices->begin(),
			std::find(
				orderedBrushVertices->begin(),
				orderedBrushVertices->end(),
				face->points[i + 1]
			)
		);

		auto c = (UINT)std::distance(
			orderedBrushVertices->begin(),
			std::find(
				orderedBrushVertices->begin(),
				orderedBrushVertices->end(),
				face->points[i + 2]
			)
		);

		MF_IndexedTriangle_t triangle;
		triangle.a = origin;
		triangle.b = b;
		triangle.c = c;

		brushIndices->push_back(triangle);
	}
}

void MF_FixOrientation(_In_ MF_FaceDTO* face) {
	//used for correcting face orientation
	auto newNormal = MF_MakePlaneFromFace({
		face->points[0],
		face->points[1],
		face->points[2]
	});

	//correct face orientation
	if (newNormal.normal != face->normal) {
		std::reverse(
			face->points.begin(),
			face->points.end()
		);
	}
}

void MF_FixCrossedQuad(_In_ MF_FaceDTO* faceDto) {
	auto plane = MF_MakePlaneFromFace({
		faceDto->points[0],
		faceDto->points[1],
		faceDto->points[2]
	});

	std::vector<MF_Vector3_t>* face = &(faceDto->points);

	BOOL broken = false;
	do {
		broken = false;
		for (int i = 0; i < face->size(); i++) {
			MF_Vector3 a = (*face)[i];
			MF_Vector3 b = (*face)[(i + 1) % face->size()];

			auto outwardVec = MF_CrossProductVector3(a - b, plane.normal);
			for (int j = 0; j < face->size(); j++) {
				auto point = (* face)[j];
				if (point == a || point == b) {
					continue; // dotproduct = 0
				}

				// convert pont to line-space
				if (MF_DotProductVector3(point - a, outwardVec) > 0) { // point is "outside" of polygon
					//swap b and c
					int match = 0;

					for (match = j; !MF_PolygonEdgeIsExterior(*face, a, (*face)[match], plane.normal); match = (match + 1) % face->size());

					auto tmp = b;
					(*face)[(i + 1) % face->size()] = (*face)[match];
					(*face)[match] = tmp;

					broken = true;
					break;
				}
			}
		}
	} while (broken);
}

BOOL MF_PolygonEdgeIsExterior(
	std::vector<MF_Vector3_t> face,
	MF_Vector3_t a,
	MF_Vector3_t b,
	MF_Vector3_t planeNormal
) {

	auto outwardVec = MF_CrossProductVector3(a - b, planeNormal);

	for (int i = 0; i < face.size(); i++) {
		auto point = face[i];
		if (point == a || point == b) {
			continue;
		}

		// convert pont to line-space
		if (MF_DotProductVector3(point - a, outwardVec) > 0) { // point is "outside" of polygon
			return false;
		}
	}
	return true;
}

MF_Vector3_t MF_CrossProductVector3(_In_ MF_Vector3_t a, _In_ MF_Vector3_t b)
{
	MF_Vector3_t c = { 0, 0, 0 };
	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;
	return c;
}

float MF_DotProductVector3(_In_ MF_Vector3_t a, _In_ MF_Vector3_t b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float MF_DotProductVector2(_In_ MF_Vector2_t a, _In_ MF_Vector2_t b) {
	return a.x * b.x + a.y * b.y;
}

MF_Vector3_t MF_AddVector3(_In_ MF_Vector3_t a, _In_ MF_Vector3_t b)
{
	return {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z
	};
}

MF_Vector2_t MF_AddVector2(_In_ MF_Vector2_t a, _In_ MF_Vector2_t b) {
	return {
		a.x + b.x,
		a.y + b.y
	};
}

MF_Vector3_t MF_NegativeVector3(_In_ MF_Vector3_t a)
{
	return {
		-a.x,
		-a.y,
		-a.z
	};
}

MF_Vector2_t MF_NegativeVector2(_In_ MF_Vector2_t a) {
	return {
		-a.x,
		-a.y
	};
}

float MF_Length3(_In_ MF_Vector3_t a)
{
	return std::sqrtf((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

float MF_Length2(_In_ MF_Vector2_t a) {
	return std::sqrtf((a.x * a.x) + (a.y * a.y));
}

MF_Vector3_t MF_ScalarMultiplyVector3(_In_ MF_Vector3_t a, _In_ float b)
{
	return {
		a.x * b,
		a.y * b,
		a.z * b
	};
}

MF_Vector2_t MF_ScalarMultiplyVector2(_In_ MF_Vector2_t a, _In_ float  b) {
	return {
		a.x * b,
		a.y * b
	};
}

MF_Vector3_t MF_NormalizeVector3(_In_ MF_Vector3_t a)
{
	float len = MF_Length3(a);
	if (len == 0.0f) {
		return { 0, 0, 0 };
	}
	return MF_ScalarMultiplyVector3(a, 1.0f / len);
}

MF_Vector2_t MF_NormalizeVector2(_In_ MF_Vector2_t a) {
	float len = MF_Length2(a);
	if (len == 0.0f) {
		return { 0, 0 };
	}
	return MF_ScalarMultiplyVector2(a, 1.0f / len);

}

MF_Vector3_t operator + (MF_Vector3_t a, MF_Vector3_t b)
{
	return MF_AddVector3(a, b);
}

MF_Vector2_t operator + (MF_Vector2_t a, MF_Vector2_t b)
{
	return MF_AddVector2(a, b);
}

MF_Vector3_t operator - (MF_Vector3_t a)
{
	return MF_NegativeVector3(a);
}

MF_Vector2_t operator - (MF_Vector2_t a)
{
	return MF_NegativeVector2(a);
}

MF_Vector3_t operator - (MF_Vector3_t a, MF_Vector3_t b)
{
	return MF_AddVector3(a, -b);
}

MF_Vector2_t operator - (MF_Vector2_t a, MF_Vector2_t b)
{
	return MF_AddVector2(a, -b);
}

MF_Vector3_t operator * (MF_Vector3_t a, float b)
{
	return MF_ScalarMultiplyVector3(a, b);
}

MF_Vector2_t operator * (MF_Vector2_t a, float b)
{
	return MF_ScalarMultiplyVector2(a, b);
}

bool operator == (const MF_Vector3_t& a, const MF_Vector3_t& b)
{
	auto isEqual =	(int)std::roundf(a.x * 1000) == (int)std::roundf(b.x * 1000) &&
					(int)std::roundf(a.y * 1000) == (int)std::roundf(b.y * 1000) &&
					(int)std::roundf(a.z * 1000) == (int)std::roundf(b.z * 1000);
	return isEqual;
}

bool operator == (const MF_Vector2_t& a, const MF_Vector2_t& b)
{
	auto isEqual =	(int)std::roundf(a.x * 1000) == (int)std::roundf(b.x * 1000) &&
					(int)std::roundf(a.y * 1000) == (int)std::roundf(b.y * 1000);
	return isEqual;
}

bool operator != (MF_Vector3_t a, MF_Vector3_t b) {
	return !(a == b);
}

bool operator != (MF_Vector2_t a, MF_Vector2_t b) {
	return !(a == b);
}

bool operator == (const MF_IndexedTriangle_t& a, const MF_IndexedTriangle_t& b) {
	return	a.a == b.a &&
			a.b == b.b &&
			a.c == b.c;
}

std::ostream& operator << (std::ostream& os, MF_Vector3 const& vec)
{
	return os << vec.x << ", " << vec.y << ", " << vec.z;
}

std::ostream& operator << (std::ostream& os, MF_Vector2 const& vec) {
	return os << vec.x << ", " << vec.y;
}