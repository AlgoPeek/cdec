#include "stdafx.h"
#include "../../test/testenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //

class Task
{
protected:
	int m_base;

public:
	Task(int base): m_base(base)
	{
	}

	int Run(void* pArg)
	{
		Thread::Sleep(200);
		int n = *(int*)pArg;
		int s = 0;
		for (int i = 1; i <= n; ++i)
			s += i;
		*(volatile int*)pArg = m_base + s;

		return 0;
	}
};

class TestThreadingThread: public UnitTestSuite
{
	UNITTEST_SUITE(TestThreadingThread)
		UNITTEST_METHOD(testStart)
	UNITTEST_SUITE_END()

public:
	void setUp()
	{
	}

	void testStart()
	{
		Task task(-5050);

		volatile int n = 100;

		ThreadStart ts(&Task::Run, &task);
		ref<Thread> th = gc_new<Thread>(ts);
		th->Start((void*)&n);
		th->Wait();

		UNITTEST_ASSERT(n == 0);
	}

	void tearDown()
	{
	}
};

UNITTEST_SUITE_REGISTRATION(TestThreadingThread);

// -------------------------------------------------------------------------- //
#endif
