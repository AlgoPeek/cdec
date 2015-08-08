#include "stdafx.h"
#include "../../test/testenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class TestThreadingLock: public UnitTestSuite
{
	UNITTEST_SUITE(TestThreadingLock)
		UNITTEST_METHOD(testAtomOp)
		UNITTEST_METHOD(testAtomOpX)
		UNITTEST_METHOD(testSpinLock)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testAtomOp()
	{
		INT64 i = 0x123456789;
		int& j = (int&)i;

		UNITTEST_ASSERT(Atom::LockInc(&j) == 0x2345678A);
		UNITTEST_ASSERT(Atom::LockDec(&j) == 0x23456789);

		UNITTEST_ASSERT(Atom::LockAdd(&j, 0x10001) == 0x2346678A);
		UNITTEST_ASSERT(Atom::LockSub(&j, 0x10001) == 0x23456789);

		UNITTEST_ASSERT(Atom::LockAnd(&j, 0x11111111) == 0x1010101);
		UNITTEST_ASSERT(Atom::LockOr(&j,  0x22446688) == 0x23456789);

		UNITTEST_ASSERT(Atom::LockXor(&j, 0xFFFFFFFF) == (int)0xDCBA9876);
		UNITTEST_ASSERT(Atom::LockXor(&j, 0xFFFFFFFF) == 0x23456789);

#ifdef X_CPU_X64
		UNITTEST_ASSERT(Atom::LockInc64(&i) == 0x12345678A);
		UNITTEST_ASSERT(Atom::LockDec64(&i) == 0x123456789);

		UNITTEST_ASSERT(Atom::LockAdd64(&i, 0x100010001) == 0x22346678A);
		UNITTEST_ASSERT(Atom::LockSub64(&i, 0x100010001) == 0x123456789);

		UNITTEST_ASSERT(Atom::LockAnd64(&i, 0x111111111) == 0x101010101);
		UNITTEST_ASSERT(Atom::LockOr64(&i,  0x022446688) == 0x123456789);

		UNITTEST_ASSERT(Atom::LockXor64(&i, 0xFFFFFFFFF) == 0xEDCBA9876);
		UNITTEST_ASSERT(Atom::LockXor64(&i, 0xFFFFFFFFF) == 0x123456789);
#endif
	}

	void testAtomOpX()
	{
		INT64 i = 0x123456789;
		int& j = (int&)i;

		UNITTEST_ASSERT(Atom::XLockAdd(&j, 0x10001) == 0x23456789 && j == 0x2346678A);
		UNITTEST_ASSERT(Atom::XLockSub(&j, 0x10001) == 0x2346678A && j == 0x23456789);

		UNITTEST_ASSERT(Atom::XLockAnd(&j, 0x11111111) == 0x23456789 && j == 0x1010101);
		UNITTEST_ASSERT(Atom::XLockOr(&j,  0x22446688) == 0x1010101  && j == 0x23456789);

		UNITTEST_ASSERT(Atom::XLockXor(&j, 0xFFFFFFFF) == 0x23456789 && j == (int)0xDCBA9876);
		UNITTEST_ASSERT(Atom::XLockXor(&j, 0xFFFFFFFF) == (int)0xDCBA9876 && j == 0x23456789);

#ifdef X_CPU_X64
		UNITTEST_ASSERT(Atom::XLockAdd64(&i, 0x100010001) == 0x123456789 && i == 0x22346678A);
		UNITTEST_ASSERT(Atom::XLockSub64(&i, 0x100010001) == 0x22346678A && i == 0x123456789);

		UNITTEST_ASSERT(Atom::XLockAnd64(&i, 0x111111111) == 0x123456789 && i == 0x101010101);
		UNITTEST_ASSERT(Atom::XLockOr64(&i,  0x022446688) == 0x101010101 && i == 0x123456789);

		UNITTEST_ASSERT(Atom::XLockXor64(&i, 0xFFFFFFFFF) == 0x123456789 && i == 0xEDCBA9876);
		UNITTEST_ASSERT(Atom::XLockXor64(&i, 0xFFFFFFFFF) == 0xEDCBA9876 && i == 0x123456789);
#endif
	}

	void testSpinLock()
	{
		CSpinLock l1;
#ifdef X_OS_WINDOWS
		l1.Enter();
		UNITTEST_ASSERT(l1.TryEnter());
		l1.Leave();
		l1.Leave();
#else
		l1.Enter();
		l1.Leave();
		UNITTEST_ASSERT(l1.TryEnter());
		l1.Leave();
#endif

		ref<SpinLock> l2 = gc_new<SpinLock>();
#ifdef X_OS_WINDOWS
		l2->Enter();
		UNITTEST_ASSERT(l2->TryEnter());
		l2->Leave();
		l2->Leave();
#else
		l2->Enter();
		l2->Leave();
		UNITTEST_ASSERT(l2->TryEnter());
		l2->Leave();
		l2->Leave();
#endif
	}

	void tearDown()
	{

	}
};

UNITTEST_SUITE_REGISTRATION(TestThreadingLock);

// -------------------------------------------------------------------------- //
#endif
