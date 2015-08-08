// -------------------------------------------------------------------------- //
// LOG Writer
// -------------------------------------------------------------------------- //

#pragma once

// Non-zero to enable Simple Logger
#define ENABLE_SIMPLE_LOG	1

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

enum LoggerLevel
{
	LOG_INFORMATION,
	LOG_CLAIM,
	LOG_WARNING,
	LOG_ERROR,
};

// -------------------------------------------------------------------------- //

interface ILogOutput
{
	virtual void WriteMessage(const std::string& message, LoggerLevel level) = 0;
};

// -------------------------------------------------------------------------- //

struct STAVLCOMMONEXPORT Logger
{
	ILogOutput* m_out;
	LoggerLevel	m_level;
	std::string m_message;

	Logger(ILogOutput* out, LoggerLevel level, const char* function, const char* file, int line);

	inline ~Logger()
	{
		m_out->WriteMessage(m_message, m_level);
	}

	Logger& operator << (stringx value);
	Logger& operator << (const std::wstring16& value);

	inline Logger& operator << (const WCHAR* value)
	{
		return (operator<<)(std::wstring16(value));
	}

	inline Logger& operator << (char value)
	{
		m_message.append(1, value);
		return *this;
	}

	inline Logger& operator << (const char* value)
	{
		m_message.append(value);
		return *this;
	}

	inline Logger& operator << (const std::string& value)
	{
		m_message.append(value);
		return *this;
	}

	inline Logger& operator << (int value)
	{
		m_message.append(ConverterA::ToString(value));
		return *this;
	}

	inline Logger& operator << (UINT value)
	{
		m_message.append(ConverterA::ToString(value));
		return *this;
	}

	inline Logger& operator << (INT64 value)
	{
		m_message.append(ConverterA::ToString(value));
		return *this;
	}

	inline Logger& operator << (UINT64 value)
	{
		m_message.append(ConverterA::ToString(value));
		return *this;
	}

	inline Logger& operator << (long value)
	{
		return (operator <<)((int)value);
	}

	inline Logger& operator << (unsigned long value)
	{
		return (operator <<)((unsigned int)value);
	}
};

// -------------------------------------------------------------------------- //

class STAVLCOMMONEXPORT LogOutputConsole : public ILogOutput
{
public:
	void WriteMessage(const std::string& message, LoggerLevel level);
};

// -------------------------------------------------------------------------- //

class STAVLCOMMONEXPORT LogOutputFile : public ILogOutput
{
public:
	~LogOutputFile();

	static void Init(stringx logFileName);
	static void Term();
	void WriteMessage(const std::string& message, LoggerLevel level);
	
private:
	static ref<FileStream> m_fstream;
};

// -------------------------------------------------------------------------- //

class LogOutputNull : public ILogOutput
{
public:
	void WriteMessage(const std::string& message, LoggerLevel level) {}
};


extern STAVLCOMMONEXPORT ILogOutput *g_logout;
extern STAVLCOMMONEXPORT ILogOutput *g_flogout;

// -------------------------------------------------------------------------- //

#if ENABLE_SIMPLE_LOG

#define LOG_(out)		Logger(out, LOG_INFORMATION, __FUNCTION__, __FILE__, __LINE__)
#define CLAIM_(out)		Logger(out, LOG_CLAIM, __FUNCTION__, __FILE__, __LINE__)
#define WARN_(out)		Logger(out, LOG_WARNING, __FUNCTION__, __FILE__, __LINE__)
#define ERR_(out)		Logger(out, LOG_ERROR, __FUNCTION__, __FILE__, __LINE__)

#define LOGT_(out)		Logger(out, LOG_INFORMATION, NULL, NULL, 0)
#define CLAIMT_(out)	Logger(out, LOG_CLAIM, NULL, NULL, 0)
#define WARNT_(out)		Logger(out, LOG_WARNING, NULL, NULL, 0)
#define ERRT_(out)		Logger(out, LOG_ERROR, NULL, NULL, 0)

#else

struct NullLogger
{
	template<typename T>
	NullLogger& operator << (T) { return *this; }
};

#define LOG_(out)		NullLogger()
#define CLAIM_(out)		NullLogger()
#define WARN_(out)		NullLogger()
#define ERR_(out)		NullLogger()

#define LOGT_(out)		NullLogger()
#define CLAIMT_(out)	NullLogger()
#define WARNT_(out)		NullLogger()
#define ERRT_(out)		NullLogger()

#endif

// Log to console
#define KLOG	LOG_(g_logout)
#define KCLAIM	CLAIM_(g_logout)
#define KWARN	WARN_(g_logout)
#define KERR	ERR_(g_logout)

#define KLOGT	LOGT_(g_logout)
#define KCLAIMT	CLAIMT_(g_logout)
#define KWARNT	WARNT_(g_logout)
#define KERRT	ERRT_(g_logout)

// Log to file
#define YLOG	LOG_(g_flogout)
#define YCLAIM	CLAIM_(g_flogout)
#define YWARN	WARN_(g_flogout)
#define YERR	ERR_(g_flogout)

#define YLOGT	LOGT_(g_flogout)
#define YCLAIMT	CLAIMT_(g_flogout)
#define YWARNT	WARNT_(g_flogout)
#define YERRT	ERRT_(g_flogout)

// -------------------------------------------------------------------------- //

CDEC_NS_END
