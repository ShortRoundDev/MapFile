#include "pch.h"
#include "CppUnitTest.h"

#include "MapFile.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestMapFile
{
	TEST_CLASS(TestMapFile_List)
	{
	public:
		TEST_METHOD(Test_MF_NewLexeme)
		{
			MF_Init();

			const char text[] = "Hello world!";
			MF_Lexeme* node = MF_NewLexeme(text, strlen(text));
			Assert::AreEqual(text, node->lexeme);
			Assert::IsNull(node->next);
			Assert::AreEqual((char)0, (char)(*(node->lexeme + strlen(text)))); // null terminated
		}

		TEST_METHOD(Test_MF_AttachLexeme)
		{
			MF_Init();

			const char text[] = "Hello world!";
			MF_Lexeme* node = MF_NewLexeme(text, strlen(text));

			//New is constructed correctly
			Assert::AreEqual(text, node->lexeme);
			Assert::IsNull(node->next);
			Assert::AreEqual((char)0, (char)(*(node->lexeme + strlen(text)))); // null terminated

			const char foo[] = "John Romero";

			MF_Lexeme* next = MF_AttachLexeme(node, foo, strlen(foo));

			//Next is constructed correctly
			Assert::IsNull(next->next);
			Assert::AreEqual(next->lexeme, foo);
			Assert::AreEqual((char)0, (char)(*(next->lexeme + strlen(foo)))); // null terminated

			//Head is attached correctly
			Assert::IsNotNull(node->next);
			Assert::AreEqual((uint64_t)node->next, (uint64_t)next); // compare addresses as longs
			//Head is uncorrupted
			Assert::AreEqual(node->lexeme, text);
		}

		TEST_METHOD(Test_MF_DestroyLexemeList)
		{
			MF_Init();

			const char text[] = "Hello world";
			MF_Lexeme* head = MF_NewLexeme(text, strlen(text));

			const char node1text[] = "John Carmack";
			MF_Lexeme* node1 = MF_AttachLexeme(head, node1text, strlen(node1text));

			const char node2text[] = "John Romero";
			MF_Lexeme* node2 = MF_AttachLexeme(head, node2text, strlen(node2text));
			
			const char node3text[] = "Tom Hall";
			MF_Lexeme* node3 = MF_AttachLexeme(head, node3text, strlen(node3text));

			MF_DestroyLexemeList(head);

			//todo: write unit test for free?
		}

		TEST_METHOD(Test_MF_Lex)
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
			while (node != NULL) {
				printf("%s\n", node->lexeme);
				node = node->next;
			}
			Assert::IsTrue(true);
		}
	};
}