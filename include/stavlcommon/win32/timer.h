#pragma once

CDEC_NS_BEGIN

#ifdef X_OS_WINDOWS

// -------------------------------------------------------------------------- //
// Timer
// -------------------------------------------------------------------------- //

// Even using a call-back function, a message loop is also REQUIRED.
// See timer.cpp for the sample of message loop.
class STAVLCOMMONEXPORT WinTimer : public Object
{
	DECLARE_REF_CLASS(WinTimer)

protected:
	UINT_PTR m_id;

public:
	inline WinTimer(): m_id(0) { }
	inline ~WinTimer() { Close(); }

	void Start(UINT interval);	// elapse in milliseconds
	void Close();

	virtual void OnTimer() { }
};

// -------------------------------------------------------------------------- //

#endif

CDEC_NS_END
