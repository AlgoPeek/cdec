// -------------------------------------------------------------------------- //
// Application Assert
// -------------------------------------------------------------------------- //

#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

#ifdef X_OS_WINDOWS
bool STAVLCOMMONEXPORT UAssertDialog(bool expr, const WCHAR* msg, const WCHAR* file, int line);
#define U_ASSERT_EXPR(expr, msg) (void)(UAssertDialog((expr), msg, _CRT_WIDE(__FILE__), __LINE__) || (__debugbreak(), 0))
#define U_ASSERT(expr) U_ASSERT_EXPR((expr), _CRT_WIDE(#expr))
#else
bool STAVLCOMMONEXPORT UAssertDialog(bool expr, const char* msg, const char* file, int line);
#define U_ASSERT_EXPR(expr, msg) (void)(UAssertDialog((expr), msg, __FILE__, __LINE__))
#define U_ASSERT(expr) U_ASSERT_EXPR((expr), #expr)
#endif

// -------------------------------------------------------------------------- //

CDEC_NS_END
