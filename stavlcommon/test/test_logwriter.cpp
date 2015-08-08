#include "stdafx.h"
#include "testenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if ENABLE_TEST
// -------------------------------------------------------------------------- //
class TestLogWriter : public UnitTestSuite
{
	UNITTEST_SUITE(TestLogWriter)
		UNITTEST_METHOD(testWriteInfo)
		UNITTEST_METHOD(testWriteClaim)
		UNITTEST_METHOD(testWriteWarn)
		UNITTEST_METHOD(testWriteError)
		UNITTEST_METHOD(testWriteC_ZN)
	UNITTEST_SUITE_END()

	stringx m_name;

	void testWriteHlp(LoggerLevel level, stringx message, int column)
	{
		const static stringx LogLevel[] = 
		{
			__X("INFO"), __X("CLAIM"), __X("WARN"), __X("ERROR")
		};
		std::vector<std::wstring16> ss;

		UNITTEST_ASSERT(File::Exists(m_name));
		ref<FileStream> fs = gc_new<FileStream>(m_name, FileStream::AccessRead, FileStream::ShareReadWrite, false);
		ref<FastLogReader> fr = gc_new<FastLogReader>(fs);

		UNITTEST_ASSERT(fr->ReadTextColumns(REF ss, ' '));
		UNITTEST_ASSERT(ss.size() == column);
		UNITTEST_ASSERT(stringx(ss[0]) == LogLevel[level]);
		UNITTEST_ASSERT(stringx(ss[ss.size() - 1]) == message);

		fr->Close();
		fs->Close();
	}

public:
	void setUp()
	{
		m_name = __X("TestLogWriter.log");
		LogOutputFile::Init(m_name);	
	}

	void testWriteInfo()
	{
		stringx message = __X("testWriteInfo");
		YLOG << message;
		testWriteHlp(LOG_INFORMATION, message, 4);
	}

	void testWriteClaim()
	{
		stringx message = __X("testWriteClaim");
		YCLAIM << message;
		testWriteHlp(LOG_CLAIM, message, 4);
	}

	void testWriteWarn()
	{
		stringx message = __X("testWriteWarn");
		YWARN << message;
		testWriteHlp(LOG_WARNING, message, 4);
	}

	void testWriteError()
	{
		stringx message = __X("testWriteError");
		YERR << message;
		testWriteHlp(LOG_ERROR, message, 4);
	}

	void testWriteC_ZN()
	{
#ifdef X_OS_WINDOWS
		WCHAR message[] = {L'\u4e2d', L'\u6587'};
#else
		WCHAR message[] = {u'\u4e2d', u'\u6587'};
#endif 
		YLOG << message;
		testWriteHlp(LOG_INFORMATION, message, 4);
	}

	void tearDown()
	{
		LogOutputFile::Term();
		File::Delete(m_name);
		UNITTEST_ASSERT(!File::Exists(m_name));
	}
};

UNITTEST_SUITE_REGISTRATION(TestLogWriter);

#endif 