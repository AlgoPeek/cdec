#include "stdafx.h"
#include "testenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

static char TEXT_PARSE[] = "1 2 3\nA\r\nB  C ";

class TestFastLogReader: public UnitTestSuite
{
	UNITTEST_SUITE(TestFastLogReader)
		UNITTEST_METHOD(testReadTextColumnsA)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testReadTextColumnsA()
	{
		std::vector<std::wstring16> ss;

		ref<MemoryStream> ms = gc_new<MemoryStream>(TEXT_PARSE, sizeof(TEXT_PARSE) - 1);
		ref<FastLogReader> fr = gc_new<FastLogReader>(ms);

		UNITTEST_ASSERT(fr->ReadTextColumns(REF ss, ' '));
		UNITTEST_ASSERT(ss.size() == 3 && ss[0] == __X("1") && ss[1] == __X("2") && ss[2] == __X("3"));
		
		UNITTEST_ASSERT(fr->ReadTextColumns(REF ss, ' '));
		UNITTEST_ASSERT(ss.size() == 1 && ss[0] == __X("A"));

		UNITTEST_ASSERT(fr->ReadTextColumns(REF ss, ' '));
		UNITTEST_ASSERT(ss.size() == 3 && ss[0] == __X("B") && ss[1] == __X("") && ss[2] == __X("C"));

		fr->Close();
		ms->Close();
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestFastLogReader);

// -------------------------------------------------------------------------- //
#endif
