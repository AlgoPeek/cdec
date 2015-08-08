#include "stdafx.h"
#include "testenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

static BYTE TA[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

class TestArrayExt: public UnitTestSuite
{
	UNITTEST_SUITE(TestArrayExt)
		UNITTEST_METHOD(testArrayIterator)
		UNITTEST_METHOD(testArrayBound)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testArrayIterator()
	{		
		ArrayIterator<2> it;
		UNITTEST_ASSERT(*it == 0);
		it = ArrayIterator<2>(TA);
		UNITTEST_ASSERT(*it == TA);
		UNITTEST_ASSERT(*(++it) == TA + 2);
		UNITTEST_ASSERT(*(it++) == TA + 2);
		UNITTEST_ASSERT(*it == TA + 4);
		UNITTEST_ASSERT(*(--it) == TA + 2);
		UNITTEST_ASSERT(*(it--) == TA + 2);
		UNITTEST_ASSERT(*it == TA);
		UNITTEST_ASSERT(*(it += 2) == TA + 4);
		UNITTEST_ASSERT(*(it + 1) == TA + 6 && *it == TA + 4);
		UNITTEST_ASSERT(*(it -= 2) == TA);
		UNITTEST_ASSERT(*(it - 1) == TA - 2 && *it == TA);

		ArrayIterator<2> it2 = it + 1;
		UNITTEST_ASSERT(it2 - it == 1 && it - it2 == -1);
		UNITTEST_ASSERT(it[0] == TA && it[1] == TA + 2);

		it2 = it + 1;
		UNITTEST_ASSERT(!(it == it2) && (it != it2) && (it < it2) && (it <= it2) && !(it > it2) && !(it >= it2));
		--it2;
		UNITTEST_ASSERT((it == it2) && !(it != it2) && !(it < it2) && (it <= it2) && !(it > it2) && (it >= it2));
		++it;
		UNITTEST_ASSERT(!(it == it2) && (it != it2) && !(it < it2) && !(it <= it2) && (it > it2) && (it >= it2));
	}

	void testArrayBound()
	{
		ArrayIterator<2> itb = ArrayIterator<2>(TA);
		ArrayIterator<2> ite = itb + 4;
		Less_Base16<2> less;

		BYTE tag[] = { 0, 0 };
		UNITTEST_ASSERT(std::lower_bound(itb, ite, tag, less) == itb);

		tag[1] = 1;	// { 0, 1 }
		UNITTEST_ASSERT(std::lower_bound(itb, ite, tag, less) == itb);

		tag[1] = 2;	// { 0, 2 }
		UNITTEST_ASSERT(std::lower_bound(itb, ite, tag, less) == itb + 1);

		tag[0] = 2;	// { 2, 2 }
		UNITTEST_ASSERT(std::lower_bound(itb, ite, tag, less) == itb + 1);

		tag[0] = 3;	// { 2, 3 }
		UNITTEST_ASSERT(std::lower_bound(itb, ite, tag, less) == itb + 2);

		tag[0] = 3;	// { 2, 4 }
		UNITTEST_ASSERT(std::lower_bound(itb, ite, tag, less) == itb + 2);

		tag[0] = 6; tag[1] = 7;	// { 6, 7 }
		UNITTEST_ASSERT(std::lower_bound(itb, ite, tag, less) == ite - 1);

		tag[1] = 8;	// { 6, 8 }
		UNITTEST_ASSERT(std::lower_bound(itb, ite, tag, less) == ite);
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestArrayExt);

// -------------------------------------------------------------------------- //
#endif
