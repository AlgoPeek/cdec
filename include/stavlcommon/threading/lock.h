// -------------------------------------------------------------------------- //
// Atom Operation & Lock
// -------------------------------------------------------------------------- //

#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// SpinLock
// -------------------------------------------------------------------------- //

class CSpinLock
{
protected:
#ifdef X_OS_WINDOWS
	CRITICAL_SECTION	_x;
#else
	pthread_spinlock_t	_x;
#endif

public:
	CSpinLock();
	CSpinLock(UINT count);
	~CSpinLock();

	bool	TryEnter();
	void	Enter();
	void	Leave();
};

#ifdef X_OS_WINDOWS

inline CSpinLock::CSpinLock()
{
	InitializeCriticalSection(&_x);
}

inline CSpinLock::CSpinLock(UINT count)
{
	InitializeCriticalSectionAndSpinCount(&_x, count);
}

inline CSpinLock::~CSpinLock()
{
	DeleteCriticalSection(&_x);
}

inline bool CSpinLock::TryEnter()
{
	return TryEnterCriticalSection(&_x) != 0;
}

inline void CSpinLock::Enter()
{
	EnterCriticalSection(&_x);
}

inline void CSpinLock::Leave()
{
	LeaveCriticalSection(&_x);
}

#else

inline CSpinLock::CSpinLock()
{
	int r = pthread_spin_init(&_x, PTHREAD_PROCESS_PRIVATE);
	if (r != 0)
		cdec_throw_stdc_err(Exception, r);
		
}

inline CSpinLock::CSpinLock(UINT)
{
	ASSERT(false);	// parameter "spin count" ignored
	int r = pthread_spin_init(&_x, PTHREAD_PROCESS_PRIVATE);
	if (r != 0)
		cdec_throw_stdc_err(Exception, r);
}

inline CSpinLock::~CSpinLock()
{
	int r = pthread_spin_destroy(&_x);
	if (r != 0)
		cdec_throw_stdc_err(Exception, r);
}

inline bool CSpinLock::TryEnter()
{
	int r = pthread_spin_trylock(&_x);
	if (r == 0)
		return true;
	else if (r == EBUSY)
		return false;
	else
		cdec_throw_stdc_err(Exception, r);
}

inline void CSpinLock::Enter()
{
	int r = pthread_spin_lock(&_x);
	if (r != 0)
		cdec_throw_stdc_err(Exception, r);
}

inline void CSpinLock::Leave()
{
	int r = pthread_spin_unlock(&_x);
	if (r != 0)
		cdec_throw_stdc_err(Exception, r);
}

#endif

// -------------------------------------------------------------------------- //

class SpinLock : public Object, CSpinLock
{
	DECLARE_REF_CLASS(SpinLock)

public:
	SpinLock(): CSpinLock() {}
	SpinLock(UINT count): CSpinLock(count) {}

	using CSpinLock::TryEnter;
	using CSpinLock::Enter;
	using CSpinLock::Leave;
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
