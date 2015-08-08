#include "stdafx.h"

#include <sstream>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

LogOutputConsole g_stdlogout;
LogOutputFile g_filelogout;

ILogOutput *g_logout = &g_stdlogout;
ILogOutput *g_flogout = &g_filelogout;

#ifdef X_OS_WINDOWS
ref<Encoding> g_defcp = Encoding::get_Default();
#else
ref<Encoding> g_defcp = Encoding::get_UTF8();
#endif

const char* LMSG[] = 
{
	"INFO ",	// LOG_INFORMATION,
	"CLAIM ",	// LOG_CLAIM,
	"WARN ",	// LOG_WARNING,
	"ERROR ",	// LOG_ERROR,
};

// -------------------------------------------------------------------------- //
// LOG Writer
// -------------------------------------------------------------------------- //

Logger::Logger(ILogOutput* out, LoggerLevel level, const char* function, const char* file, int line)
{
	m_out = out;
	m_level = level;
	m_message = LMSG[level];
	if (file != NULL)
		m_message.append(file).append("(").append(ConverterA::ToString(line)).append(") ");

	std::stringstream sstrm;
	sstrm << DateTime::NowTimestamp() << " ";
	m_message.append(sstrm.str());
}

Logger& Logger::operator << (stringx value)
{
	m_message += g_defcp->FromUnicode(value);
	return *this;
}

Logger& Logger::operator << (const std::wstring16& value)
{
	m_message += g_defcp->FromUnicode(value);
	return *this;
}

// -------------------------------------------------------------------------- //
// LogOutputConsole
// -------------------------------------------------------------------------- //

void DebugSetOutputColor(LoggerLevel level)
{
#ifdef X_OS_WINDOWS
	static WORD attrs[] = { 7, 10, 14, 12 };
	static HANDLE _hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	static LoggerLevel lastLevel = LOG_INFORMATION;
	if (level != lastLevel)
	{
		WORD attr = attrs[level];
		SetConsoleTextAttribute(_hStdout, attr);
		lastLevel = level;
	}
#endif
}

void LogOutputConsole::WriteMessage(const std::string& message, LoggerLevel level)
{
	DebugSetOutputColor(level);
	puts(message.c_str());
	DebugSetOutputColor(LOG_INFORMATION);
}

// -------------------------------------------------------------------------- //

ref<FileStream> LogOutputFile::m_fstream = NULL;

LogOutputFile::~LogOutputFile()
{
	Term();
}

void LogOutputFile::Init(stringx logFileName)
{
	if (m_fstream == NULL) 
	{
		bool bCreate = !File::Exists(logFileName);
		m_fstream = gc_new<FileStream>(logFileName, FileStream::AccessReadWrite, FileStream::ShareReadWrite, bCreate);
		INT64 length = m_fstream->Length();
		m_fstream->Seek(length);
	}
}

void LogOutputFile::Term()
{
	if (m_fstream != NULL)
	{
		m_fstream->Flush();
		m_fstream->Close();
		m_fstream = NULL;
	}
}

void LogOutputFile::WriteMessage(const std::string& message, LoggerLevel level)
{
	if (m_fstream != NULL)
	{
		std::string _message = message;

#ifdef X_OS_WINDOWS
		_message.append("\r\n");
#else
		_message.append("\n");
#endif

		m_fstream->Write(_message.c_str(), _message.size());
	}
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
