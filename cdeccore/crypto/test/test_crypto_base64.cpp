#include "stdafx.h"
#include "../../test_support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestBase64: public UnitTestSuite
{
	UNITTEST_SUITE(TestBase64)
		UNITTEST_METHOD(testNormal)
		UNITTEST_METHOD(testByteArray)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{

	}

	bool _compareArray(ref<ByteArray> arr1, ref<ByteArray> arr2)
	{
		if (arr1->Count() == arr2->Count())
			return memcmp(arr1->GetBuffer().ptr(), arr2->GetBuffer().ptr(), arr1->Count()) == 0;
		return false;	
	}

	void testNormal()
	{
		ref<Encoding> e = Encoding::get_UTF8();
		stringx org = __X("greate");
		ref<ByteArray> orga = e->GetBytes(org);
		stringx enc = Base64::Encode(orga);
		UNITTEST_ASSERT(enc == __X("Z3JlYXRl"));
		UNITTEST_ASSERT(_compareArray(orga, Base64::Decode(enc)));
		
		org = __X("baby");
		orga = e->GetBytes(org);
		enc = Base64::Encode(orga);
		UNITTEST_ASSERT(enc == __X("YmFieQ=="));
		UNITTEST_ASSERT(_compareArray(orga, Base64::Decode(enc)));

		org = __X("mouse");
		orga = e->GetBytes(org);
		enc = Base64::Encode(orga);
		UNITTEST_ASSERT(enc == __X("bW91c2U="));
		UNITTEST_ASSERT(_compareArray(orga, Base64::Decode(enc)));
	}

	void testByteArray()
	{
		BYTE org1[] = { 0, 1, 2, 0x7f, 0x80, 0x81, 0xfd, 0xfe, 0xff };
		stringx enc = Base64::Encode(org1, sizeof(org1));
		UNITTEST_ASSERT(enc == __X("AAECf4CB/f7/"));
		UNITTEST_ASSERT(_compareArray(gc_new<ByteArray>(org1, sizeof(org1)), Base64::Decode(enc)));
	}

	void tearDown()
	{

	}
};

UNITTEST_SUITE_REGISTRATION(TestBase64);

// -------------------------------------------------------------------------- //
#endif
