// -------------------------------------------------------------------------- //
// Atom Operation & Lock
// -------------------------------------------------------------------------- //

#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Atom Operation
// -------------------------------------------------------------------------- //

#ifdef X_OS_WINDOWS

class Atom
{
public:
	static int		LockInc(int* px) { return _InterlockedIncrement((long*)px); }
	static int		LockDec(int* px) { return _InterlockedDecrement((long*)px); }

	static int		XLockAdd(int* px, int y) { return _InterlockedExchangeAdd((long*)px, y); }
	static int		XLockSub(int* px, int y) { return XLockAdd(px, -y); }

	static int		XLockAnd(int* px, int y) { return (int)_InterlockedAnd((long*)px, y); }
	static int		XLockOr(int* px, int y)  { return (int)_InterlockedOr((long*)px, y); }
	static int		XLockXor(int* px, int y) { return (int)_InterlockedXor((long*)px, y); }

#	ifdef X_CPU_X64
	static int		LockAdd(int* px, int y) { return _InterlockedAdd((long*)px, y); }
#	else
	static int		LockAdd(int* px, int y) { return XLockAdd(px, y) + y; }
#	endif
	static int		LockSub(int* px, int y) { return LockAdd(px, -y); }

	static int		LockAnd(int* px, int y) { return XLockAnd(px, y) & y; }
	static int		LockOr(int* px, int y)  { return XLockOr(px, y) | y; }
	static int		LockXor(int* px, int y) { return XLockXor(px, y) ^ y; }

#	ifdef X_CPU_X64
	static INT64	LockInc64(INT64* px) { return _InterlockedIncrement64(px); }
	static INT64	LockDec64(INT64* px) { return _InterlockedDecrement64(px); }

	static INT64	XLockAdd64(INT64* px, INT64 y) { return _InterlockedExchangeAdd64(px, y); }
	static INT64	XLockSub64(INT64* px, INT64 y) { return XLockAdd64(px, -y); }

	static INT64	XLockAnd64(INT64* px, INT64 y) { return _InterlockedAnd64(px, y); }
	static INT64	XLockOr64(INT64* px, INT64 y)  { return _InterlockedOr64(px, y); }
	static INT64	XLockXor64(INT64* px, INT64 y) { return _InterlockedXor64(px, y); }

	static INT64	LockAdd64(INT64* px, INT64 y) { return _InterlockedAdd64(px, y); }
	static INT64	LockSub64(INT64* px, INT64 y) { return LockAdd64(px, -y); }

	static INT64	LockAnd64(INT64* px, INT64 y) { return _InterlockedAnd64(px, y) & y; }
	static INT64	LockOr64(INT64* px, INT64 y)  { return _InterlockedOr64(px, y) | y; }
	static INT64	LockXor64(INT64* px, INT64 y) { return _InterlockedXor64(px, y) ^ y; }
#	endif
};

#else

class Atom
{
public:
	static int		LockInc(int* px) { return __sync_add_and_fetch(px, 1); }
	static int		LockDec(int* px) { return __sync_sub_and_fetch(px, 1); }

	static int		XLockAdd(int* px, int y) { return __sync_fetch_and_add(px, y); }
	static int		XLockSub(int* px, int y) { return __sync_fetch_and_sub(px, y); }

	static int		XLockAnd(int* px, int y) { return __sync_fetch_and_and(px, y); }
	static int		XLockOr(int* px, int y)  { return __sync_fetch_and_or(px, y); }
	static int		XLockXor(int* px, int y) { return __sync_fetch_and_xor(px, y); }

	static int		LockAdd(int* px, int y) { return __sync_add_and_fetch(px, y); }
	static int		LockSub(int* px, int y) { return __sync_sub_and_fetch(px, y); }

	static int		LockAnd(int* px, int y) { return __sync_and_and_fetch(px, y); }
	static int		LockOr(int* px, int y)  { return __sync_or_and_fetch(px, y); }
	static int		LockXor(int* px, int y) { return __sync_xor_and_fetch(px, y); }

#	ifdef X_CPU_X64
	static INT64	LockInc64(INT64* px) { return __sync_add_and_fetch(px, 1); }
	static INT64	LockDec64(INT64* px) { return __sync_sub_and_fetch(px, 1); }

	static INT64	XLockAdd64(INT64* px, INT64 y) { return __sync_fetch_and_add(px, y); }
	static INT64	XLockSub64(INT64* px, INT64 y) { return __sync_fetch_and_sub(px, y); }

	static INT64	XLockAnd64(INT64* px, INT64 y) { return __sync_fetch_and_and(px, y); }
	static INT64	XLockOr64(INT64* px, INT64 y)  { return __sync_fetch_and_or(px, y); }
	static INT64	XLockXor64(INT64* px, INT64 y) { return __sync_fetch_and_xor(px, y); }

	static INT64	LockAdd64(INT64* px, INT64 y) { return __sync_add_and_fetch(px, y); }
	static INT64	LockSub64(INT64* px, INT64 y) { return __sync_sub_and_fetch(px, y); }

	static INT64	LockAnd64(INT64* px, INT64 y) { return __sync_and_and_fetch(px, y); }
	static INT64	LockOr64(INT64* px, INT64 y)  { return __sync_or_and_fetch(px, y); }
	static INT64	LockXor64(INT64* px, INT64 y) { return __sync_xor_and_fetch(px, y); }
#	endif
};

#endif

// -------------------------------------------------------------------------- //

CDEC_NS_END
