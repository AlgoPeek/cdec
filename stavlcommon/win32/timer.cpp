#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDEC_NS_BEGIN

#ifdef X_OS_WINDOWS

// -------------------------------------------------------------------------- //
// Timer
// -------------------------------------------------------------------------- //

// Even using a call-back function, a message loop is also REQUIRED.
//
// void CALLBACK MyTimerFunc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
// {
//	Console::WriteLine(__X("OnTimer"));
// }
//
// void TestTimer()
// {
//	UINT_PTR id = SetTimer(NULL, 0, 1000, MyTimerFunc);
//	ASSERT(id != 0);
//
//	MSG msg;
//	while(GetMessage(&msg,NULL,NULL,NULL))
//	{
//		if (msg.message == WM_TIMER) 
//		{ 
//			TranslateMessage(&msg); 
//			DispatchMessage(&msg); 
//		} 
//	}
//
//	KillTimer(NULL, id);
// }

static std::map<UINT_PTR, WinTimer*> g_tmm;	// Timer Map

void CALLBACK StavlTimerProc(HWND hwnd, UINT uMsg, UINT_PTR id, DWORD dwTime)
{
	std::map<UINT_PTR, WinTimer*>::iterator it = g_tmm.find(id);
	if (it != g_tmm.end())
		(it->second)->OnTimer();
}

void WinTimer::Start(UINT interval)
{
	if (m_id != 0)
		cdec_throw(IOException(EC_IO_AlreadyOpened));

	m_id = SetTimer(NULL, 0, interval, StavlTimerProc);
	if (m_id == 0)
		cdec_throw_win32_lasterr(Exception);

	g_tmm.insert(std::map<UINT_PTR, WinTimer*>::value_type(m_id, this));
}

void WinTimer::Close()
{
	if (m_id != 0)
	{
		g_tmm.erase(m_id);

		KillTimer(NULL, m_id);
		m_id = 0;
	}
}

// -------------------------------------------------------------------------- //

#endif

CDEC_NS_END
