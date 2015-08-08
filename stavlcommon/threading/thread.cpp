#include "stdafx.h"

#ifdef X_OS_WINDOWS
#include <process.h>
#else
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

#ifdef X_OS_WINDOWS
UINT __stdcall _InvokeThreadStart(void* pArg)
#else
void* _InvokeThreadStart(void* pArg)
#endif
{
	Thread::TSE* tse = (Thread::TSE*)pArg;
	int r = tse->Run(tse->m_val);
	delete tse;
#ifdef X_OS_WINDOWS
	return r;
#else
	return (void*)r;
#endif
}

#ifdef X_OS_WINDOWS

void Thread::Start(void* arg)
{
	UINT tid;
	TSE* tse = new TSE(m_ts, arg);
	m_handle = (void*)_beginthreadex(NULL, 0, _InvokeThreadStart, tse, 0, &tid);
}

void Thread::Wait()
{
	WaitForSingleObject((HANDLE)m_handle, INFINITE);
}

void Thread::Dispose()
{
	if (m_handle != NULL)
	{
		CloseHandle(m_handle);
		m_handle = NULL;
	}
}

void Thread::Sleep(UINT milliseconds)
{
	::Sleep(milliseconds);
}

#else

void Thread::Start(void* arg)
{
	TSE* tse = new TSE(m_ts, arg);
	int r = pthread_create(&m_tid, NULL, _InvokeThreadStart, tse);
	if (r != 0)
		cdec_throw_stdc_err(Exception, r);
}

void Thread::Wait()
{
	void* pval;
	int r = pthread_join(m_tid, &pval);
	if (r != 0)
		cdec_throw_stdc_err(Exception, r);
}

void Thread::Dispose()
{
	m_tid = 0;
}

void Thread::Sleep(UINT milliseconds)
{
	::usleep(milliseconds * 1000);
}

#endif

// -------------------------------------------------------------------------- //

CDEC_NS_END
