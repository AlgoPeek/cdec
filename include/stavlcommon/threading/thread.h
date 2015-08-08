#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Thread Start
// -------------------------------------------------------------------------- //

struct ThreadStart
{
	struct It
	{
	};

	int (It::*m_func)(void*);
	It* m_this;

	ThreadStart()
	{
		m_func = NULL;
		m_this = NULL;
	}

	template<class T>
	ThreadStart(int (T::*func)(void*), T* pobj)
	{
		m_func = (int (It::*)(void*))func;
		m_this = (It*)pobj;
	}

	int Run(void* val)
	{
		return (m_this->*m_func)(val);
	}
};

// -------------------------------------------------------------------------- //
// Thread
// -------------------------------------------------------------------------- //

class STAVLCOMMONEXPORT Thread : public Object
{
	DECLARE_REF_CLASS(Thread)

public:
	struct TSE: ThreadStart
	{
		void* m_val;

		TSE(const ThreadStart& rhs, void* val): ThreadStart(rhs), m_val(val)
		{
		}
	};

protected:
	ThreadStart m_ts;

#ifdef X_OS_WINDOWS
	void*		m_handle;
#else
	pthread_t	m_tid;
#endif

public:
#ifdef X_OS_WINDOWS
	Thread(const ThreadStart& ts): m_ts(ts), m_handle(NULL) {}
#else
	Thread(const ThreadStart& ts): m_ts(ts), m_tid(0) {}
#endif
	~Thread() { Dispose(); }

	void	Start(void* arg);
	void	Wait();

	void	Dispose();

	static void	Sleep(UINT milliseconds);
};

// -------------------------------------------------------------------------- //
// PAUSE instrument
// -------------------------------------------------------------------------- //

#ifdef X_OS_WINDOWS
#	define asm_pause()	__asm pause
#else
#	define asm_pause()	__asm__("pause")
#endif

// -------------------------------------------------------------------------- //

CDEC_NS_END
