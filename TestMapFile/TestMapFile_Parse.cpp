#include "pch.h"
#include "CppUnitTest.h"

#include "MapFile.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestMapFile
{
	TEST_CLASS(TestMapFile_Parse)
	{
	public:
		TEST_METHOD(OK)
		{
			MF_Init();
			MF_Map map;
			bool success = MF_Parse(
				"// Game: Generic\n"
				"// Format: Standard\n"
				"// entity 0\n"
				"{\n"
				"\"classname\" \"worldspawn\"\n"
				"\"_tb_textures\" \"textures\"\n"
				"// brush 0\n"
				"{\n"
				"(-64 -64 -16) (-64 -63 -16) (-64 -64 -15) __TB_empty 0 0 0 1 1\n"
				"(-64 -64 -16) (-64 -64 -15) (-63 -64 -16) __TB_empty 0 0 0 1 1\n"
				"(-64 -64 -16) (-63 -64 -16) (-64 -63 -16) __TB_empty 0 0 0 1 1\n"
				"(64 64 16) (64 65 16) (65 64 16) __TB_empty 0 0 0 1 1\n"
				"(64 64 16) (65 64 16) (64 64 17) __TB_empty 0 0 0 1 1\n"
				"(64 64 16) (64 64 17) (64 65 16) __TB_empty 0 0 0 1 1\n"
				"}\n"
				"// brush 1\n"
				"{\n"
				"(-16 -48 80) (-16 32 16) (-16 32 80) __TB_empty 0.25 0.5 33.5 2.3 4.6\n"
				"(16 -48 80) (-16 -48 16) (-16 -48 80) __TB_empty 0.25 0.5 33.5 2.3 4.6\n"
				"(16 32 16) (-16 -48 16) (16 -48 16) __TB_empty 0.25 0.5 33.5 2.3 4.6\n"
				"(16 32 80) (-16 -48 80) (-16 32 80) __TB_empty 0.25 0.5 33.5 2.3 4.6\n"
				"(16 32 80) (-16 32 16) (16 32 16) __TB_empty 0.25 0.5 33.5 2.3 4.6\n"
				"(16 32 80) (16 -48 16) (16 -48 80) __TB_empty 0.25 0.5 33.5 2.3 4.6\n"
				"}\n"
				"}\n"
				"// entity 1\n"
				"{\n"
				"\"classname\" \"info_player_start\"\n"
				"\"origin\" \"-48 0 40\"\n"
				"\"customKey\" \"1234565\"\n"
				"}\n",
				&map
			);

			Assert::AreEqual(0u, MF_GetErrCode());

			Assert::AreEqual((size_t)2, map.totalItems);
			//entity 0, worldbrush
			{
				auto entity0 = map.items[0];
				
				//attribute test
				Assert::AreEqual((size_t)2, entity0.totalAttributes);
				{
					Assert::AreEqual("classname", entity0.attributes[0].key);
					Assert::AreEqual("worldspawn", entity0.attributes[0].value);

					Assert::AreEqual("_tb_textures", entity0.attributes[1].key);
					Assert::AreEqual("textures", entity0.attributes[1].value);
				}

				//brush test
				Assert::AreEqual((size_t)2, entity0.totalBrushes);
				{
					Assert::AreEqual((size_t)6, entity0.brushes[0].totalFaces);
					{
						auto brush = entity0.brushes[0];

						//face 1, vertex 1
						Assert::AreEqual(-64.0f, brush.faces[0].facePoints[0].x);
						Assert::AreEqual(-64.0f, brush.faces[0].facePoints[0].y);
						Assert::AreEqual(-16.0f, brush.faces[0].facePoints[0].z);
						Assert::AreEqual(0.0f, brush.faces[0].facePoints[0].w);

						//face 1, texture
						Assert::AreEqual("__TB_empty", brush.faces[0].texture);

						//face 1, texture parameters
						Assert::AreEqual(0.0f, brush.faces[0].textureParameters.offsetX);
						Assert::AreEqual(0.0f, brush.faces[0].textureParameters.offsetY);
						Assert::AreEqual(0.0f, brush.faces[0].textureParameters.angle);
						Assert::AreEqual(1.0f, brush.faces[0].textureParameters.scaleX);
						Assert::AreEqual(1.0f, brush.faces[0].textureParameters.scaleY);

						//face 5, vertex 3 (skipping the rest because I'm too lazy to write the
						//entire unit test out
						Assert::AreEqual(64.0f, brush.faces[4].facePoints[2].x);
						Assert::AreEqual(64.0f, brush.faces[4].facePoints[2].y);
						Assert::AreEqual(17.0f, brush.faces[4].facePoints[2].z);
						Assert::AreEqual(0.0f, brush.faces[4].facePoints[2].w);
					}

					Assert::AreEqual((size_t)6, entity0.brushes[1].totalFaces);
					{
						auto brush = entity0.brushes[1];

						//face 1, vertex 1
						Assert::AreEqual(-16.0f, brush.faces[0].facePoints[0].x);
						Assert::AreEqual(-48.0f, brush.faces[0].facePoints[0].y);
						Assert::AreEqual(80.0f, brush.faces[0].facePoints[0].z);
						Assert::AreEqual(0.0f, brush.faces[0].facePoints[0].w);

						//face 1, texture
						Assert::AreEqual("__TB_empty", brush.faces[0].texture);

						//face 1, texture parameters
						Assert::AreEqual(0.25f, brush.faces[0].textureParameters.offsetX);
						Assert::AreEqual(0.5f, brush.faces[0].textureParameters.offsetY);
						Assert::AreEqual(33.5f, brush.faces[0].textureParameters.angle);
						Assert::AreEqual(2.3f, brush.faces[0].textureParameters.scaleX);
						Assert::AreEqual(4.6f, brush.faces[0].textureParameters.scaleY);

						//face 5, vertex 3 (skipping the rest because I'm too lazy to write the
						//entire unit test out
						Assert::AreEqual(16.0f, brush.faces[4].facePoints[2].x);
						Assert::AreEqual(32.0f, brush.faces[4].facePoints[2].y);
						Assert::AreEqual(16.0f, brush.faces[4].facePoints[2].z);
						Assert::AreEqual(0.0f, brush.faces[4].facePoints[2].w);
					}
				}
				//entity test
			}
			{
				auto entity1 = map.items[1];
				Assert::AreEqual((size_t)0, entity1.totalBrushes);
				Assert::AreEqual((size_t)3, entity1.totalAttributes);
				{
					Assert::AreEqual("classname", entity1.attributes[0].key);
					Assert::AreEqual("info_player_start", entity1.attributes[0].value);

					Assert::AreEqual("origin", entity1.attributes[1].key);
					Assert::AreEqual("-48 0 40", entity1.attributes[1].value);

					Assert::AreEqual("customKey", entity1.attributes[2].key);
					Assert::AreEqual("1234565", entity1.attributes[2].value);
				}
			}
		}

		TEST_METHOD(ERR_Ent_No_Att)
		{
			MF_Init();
			MF_Map map;
			bool success = MF_Parse("{}", &map);
			uint32_t err = MF_GetErrCode();

			Assert::AreEqual(MF_PARSE_ENT_NO_ATT_CODE, err);
		}

		TEST_METHOD(ERR_No_Ent)
		{
			MF_Init();
			MF_Map map;
			bool success = MF_Parse("//No ents", &map);
			uint32_t err = MF_GetErrCode();

			Assert::AreEqual(MF_PARSE_NO_ENTS_CODE, err);
		}

		TEST_METHOD(ERR_Misaligned_Att)
		{
			MF_Init();
			MF_Map map;
			bool success = MF_Parse("{ \"Key\" abc \"Value\" }", &map);
			uint32_t err = MF_GetErrCode();

			Assert::AreEqual(MF_PARSE_MISALIGNED_ATT_CODE, err);
		}

		TEST_METHOD(ERR_Misaligned_Brush_Paren)
		{
			MF_Init();
			MF_Map map;
			bool success = MF_Parse("{\"foo\" \"bar\"\n{\n\"test\" (1 2 3) }\n}", &map);
			uint32_t err = MF_GetErrCode();

			Assert::AreEqual(MF_PARSE_MISALIGNED_BRUSH_PAREN_CODE, err);
		}

		TEST_METHOD(ERR_Misaligned_Ent)
		{
			MF_Init();
			MF_Map map;
			bool success = MF_Parse("{\"foo\" \"bar\"\n}\n\"hello world\"\n{\"valid\" \"ent\"}", &map);
			uint32_t err = MF_GetErrCode();

			Assert::AreEqual(MF_PARSE_MISALIGNED_ENT_CODE, err);
		}
	};
}