#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

#ifdef X_OS_WINDOWS

bool UAssertDialog(bool expr, const WCHAR* msg, const WCHAR* file, int line)
{
	if (expr)
		return true;

	WCHAR ln[12];
	_itow(line, ln, 10);
	std::wstring str = L"Debug assertion failed.\n\nExpress: ";
	str.append(msg).append(L"\n\n");
	str.append(file).append(L" (").append(ln).append(L")\n\nBreak now?");
	return MessageBoxW(NULL, str.c_str(), L"Assertion failed", MB_YESNO | MB_ICONSTOP) != IDYES;
}

#else

bool UAssertDialog(bool expr, const char* msg, const char* file, int line)
{
	if (expr)
		return true;

	KERRT << "Assertion failed: " << msg << ", at " << file << " (" << line << ')';
	exit(1);
	return false;
}

#endif

// -------------------------------------------------------------------------- //

CDEC_NS_END
