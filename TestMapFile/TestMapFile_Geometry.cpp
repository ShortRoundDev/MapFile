#include "pch.h"
#include "CppUnitTest.h"

#include <Windows.h>

#include "MapFile.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestMapFile
{
	TEST_CLASS(TestMapFile_Geometry)
	{
	public:
		TEST_METHOD(OK_Make_Plane_From_Face)
		{
			MF_Face face;
			face.facePoints[0] = { 0, 0, 0 };
			face.facePoints[1] = { 0, 1, 0 };
			face.facePoints[2] = { 1, 1, 0 };
			auto plane = MF_MakePlaneFromFace(face);
			Assert::AreEqual(0.0f, plane.normal.x);
		}

		TEST_METHOD(OK_INTERSECT_THREE_PLANES)
		{
			MF_Face a, b, c;
			a.facePoints[0] = { 0, 0, 0 };
			a.facePoints[1] = { 0, 1, 0 };
			a.facePoints[2] = { 1, 1, 0 };

			b.facePoints[0] = { 0, 0, 0 };
			b.facePoints[1] = { 0, 0, 1 };
			b.facePoints[2] = { 0, 1, 1 };

			c.facePoints[0] = { 0, 0.5f, 0 };
			c.facePoints[1] = { 1, 0.5f, 0 };
			c.facePoints[2] = { 1, 0.5f, 1 };

			MF_Plane	_a = MF_MakePlaneFromFace(a),
						_b = MF_MakePlaneFromFace(b),
						_c = MF_MakePlaneFromFace(c);

			Assert::AreEqual(0.0f, _a.normal.x);
			Assert::AreEqual(0.0f, _a.normal.y);
			Assert::AreEqual(-1.0f, _a.normal.z); // toward camera

			Assert::AreEqual(-1.0f, _b.normal.x);
			Assert::AreEqual(0.0f, _b.normal.y);
			Assert::AreEqual(0.0f, _b.normal.z); // to left

			Assert::AreEqual(0.0f, _c.normal.x);
			Assert::AreEqual(-1.0f, _c.normal.y);
			Assert::AreEqual(0.0f, _c.normal.z); // down

			MF_Vector3 intersection;
			MF_Intersect3Planes(_a, _b, _c, &intersection);

			Assert::AreEqual(0.0f, intersection.x);
			Assert::AreEqual(0.5f, intersection.y);
			Assert::AreEqual(0.0f, intersection.z);
		}

		TEST_METHOD(OK_Make_Brush)
		{
			_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
			MF_Init();
			MF_SetAllocator(malloc);
			MF_SetFree(free);

			HANDLE file = CreateFile(
				L"Data\\Weird.map",
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);
			if (file == INVALID_HANDLE_VALUE) {
				Assert::IsTrue(false);
			}

			DWORD size = GetFileSize(file, NULL);
			char* mapText = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size + 1);
			if (mapText == NULL) {
				return;
			}
			if (!ReadFile(file, mapText, size, NULL, NULL)) {
				return;
			}

			MF_Map map;
			if (!MF_Parse(mapText, &map))
			{
				char msg[1024];
				MF_GetErrMessage(msg);

				return;
			}

			MF_GenerateMesh(&map);
			Assert::AreEqual(1, 1);
		}
	};
}