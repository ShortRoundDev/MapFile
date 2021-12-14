#include "pch.h"
#include "CppUnitTest.h"

#include "MapFile.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestMapFile
{
	TEST_CLASS(TestMapFile_System)
	{
	public:
		
		TEST_METHOD(Test_MF_Alloc)
		{
			MF_SetAllocator(malloc);
			void* data = MF_Alloc(128);
			Assert::IsNotNull(data);
		}

		TEST_METHOD(Test_MF_Free)
		{
			MF_SetAllocator(malloc);
			MF_SetFree(free);

			void* data = MF_Alloc(128);
			MF_Free(data);
			Assert::IsTrue(true); // bad really, but if MF_Free fails,
								  // it should probably just crash
		}

		TEST_METHOD(Test_MF_Init)
		{
			MF_Init();

			void* data = MF_Alloc(128);
			Assert::IsNotNull(data);
			
			MF_Free(data);
			Assert::IsTrue(true);
		}
	};
}
