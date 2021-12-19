#include "pch.h"
#include "CppUnitTest.h"

#include "MapFile.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestMapFile
{
	TEST_CLASS(TestMapFile_List)
	{
	public:
		TEST_METHOD(OK)
		{
			MF_Init();

			MF_Lexeme* lexList = MF_Lex(
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
				"}\n"
			);
			MF_Lexeme* node = lexList;
			Assert::AreEqual( "// Game: Generic", node->lexeme); // 0
			node = node->next;
			Assert::AreEqual("\n", node->lexeme); // 1

			// Pick a random entry
			for (int i = 0; i < 13; i++) {
				node = node->next;
			}
			Assert::AreEqual("// brush 0", node->lexeme);
		}
	};
}