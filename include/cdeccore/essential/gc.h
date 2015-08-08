// This header contains the following:
//   - gc_new

#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //

template<class T>
struct GcValueTrait
{
	// Initialize a single instance with default constructor
	inline static void InitInstance(T* p)
	{
		new(p) T();
	}
	
	// Initialize a single instance with copy constructor
	inline static void InitInstanceCopy(T* p, const T* ps)
	{
		new(p) T(*ps);
	}
	
	// Initialize a fixed array with default constructor
	inline static void InitArray(T* p, size_t n)
	{
		for (size_t i = 0; i < n; ++i)
			new(p + i) T();
	}

	// Initialize a fixed array with copy constructor
	inline static void InitArrayCopy(T* pt, const T* ps, size_t n)
	{
		for (size_t i = 0; i < n; ++i)
			new(pt + i) T(ps[i]);
	}

	// Destruct a single instance
	// A NULL pointer is accepted and simply ignored
	inline static void DisposeInstance(T* p)
	{
		if (p != NULL)
			p->~T();
	}

	// Destruct a fixed array
	// If the pointer is not NULL, an instance array of n items will be destructed
	// If the pointer is NULL, parameter n is ignored
	inline static void DisposeArray(T* p, size_t n)
	{
		if (p != NULL)
		{
			for (size_t i = 0; i < n; ++i)
				p[i].~T();
		}
	}
};

template<class T>
struct GcFundValueTrait
{
	inline static void InitInstance(T* p)
	{
		*p = 0;
	}

	inline static void InitInstanceCopy(T* p, const T* ps)
	{
		*p = *ps;
	}

	inline static void InitArray(T* p, size_t n)
	{
		memset(p, 0, n * sizeof(T));
	}

	inline static void InitArrayCopy(T* pt, const T* ps, size_t n)
	{
		memcpy(pt, ps, n * sizeof(T));
	}

	inline static void DisposeInstance(T*) { }
	inline static void DisposeArray(T*, size_t) { }
};

#define DECLARE_FUND_VALUE_TRAIT(T)		\
	template<> struct GcValueTrait<T> : GcFundValueTrait<T> {}

DECLARE_FUND_VALUE_TRAIT(INT8);
DECLARE_FUND_VALUE_TRAIT(UINT8);
DECLARE_FUND_VALUE_TRAIT(INT16);
DECLARE_FUND_VALUE_TRAIT(UINT16);
DECLARE_FUND_VALUE_TRAIT(int);
DECLARE_FUND_VALUE_TRAIT(UINT);
DECLARE_FUND_VALUE_TRAIT(INT64);
DECLARE_FUND_VALUE_TRAIT(UINT64);

DECLARE_FUND_VALUE_TRAIT(long);
DECLARE_FUND_VALUE_TRAIT(unsigned long);

DECLARE_FUND_VALUE_TRAIT(bool);
DECLARE_FUND_VALUE_TRAIT(float);
DECLARE_FUND_VALUE_TRAIT(double);

DECLARE_FUND_VALUE_TRAIT(wchar_t);

template<class T>
struct GcValueTrait<T*> : GcFundValueTrait<T*>
{
};

template<class T>
struct GcValueTrait<weak_ref<T> > : GcFundValueTrait<weak_ref<T> >
{
};

template<class T>
struct GcValueTrait<ref<T> > : GcFundValueTrait<ref<T> >
{
};

// -------------------------------------------------------------------------- //

template<class T>
class GcCreation
{
public:
	typedef GcValueTrait<T>	ValueTrait;

	// Allocate memory for a single instance
	inline static T* AllocInstanceMemory()
	{
		return (T*)CoreAllocT(sizeof(T));
	}

	// Allocate memory for a fixed array
	inline static T* AllocArrayMemory(size_t count)
	{
		return (T*)CoreAllocT(sizeof(T) * count);
	}

	// Free memory allocated for a single instance
	inline static void FreeInstanceMemory(T* p)
	{
		CoreFreeT(p, sizeof(T));
	}

	// Free memory allocated for a fixed array
	inline static void FreeArrayMemory(T* p, size_t count)
	{
		CoreFreeT(p, sizeof(T) * count);
	}
};

// -------------------------------------------------------------------------- //
// Reference Counter for internal classes
// -------------------------------------------------------------------------- //

#if defined(X_OS_WINDOWS)
#	define REFCOUNT_ADD(x)				_InterlockedIncrement((long*)&x)
#	define REFCOUNT_RELEASE_TEST(x)		(_InterlockedDecrement((long*)&x) == 0)
#elif defined(X_OS_LINUX)
#	define REFCOUNT_ADD(x)				__sync_add_and_fetch(&x,1)
#	define REFCOUNT_RELEASE_TEST(x)		(__sync_sub_and_fetch(&x,1) == 0)
#elif 0
#	define REFCOUNT_ADD(x)				(++x)
#	define REFCOUNT_RELEASE_TEST(x)		(--x == 0)
#endif

struct RefCounter
{
	int		_value;
	RefCounter(): _value(1) {}
	void	AddRef() { REFCOUNT_ADD(_value); }
	bool	ReleaseAndTest() { return REFCOUNT_RELEASE_TEST(_value); }
	operator UINT() const { return _value; }
};

// The following line works in VC but incompliant to gcc
// friend _gcCreation
//
#define _CDEC_SHARED_CLASS(_Ty)								\
	protected:												\
		typedef GcCreation<_Ty > _gcCreation;				\
		typedef GcValueTrait<_Ty > _value_trait;			\
		friend class GcValueTrait<_Ty >;					\
		RefCounter	_ref;									\
	public:													\
		void	AddRef() { _ref.AddRef(); }					\
		void	Release() {									\
			if (_ref.ReleaseAndTest()) {					\
				_value_trait::DisposeInstance(this);		\
				_gcCreation::FreeInstanceMemory(this);		\
			}												\
		}

// -------------------------------------------------------------------------- //
// Object wrapper for general reference class
// -------------------------------------------------------------------------- //

template<class _Ty>
class ObjectWrapper: public _Ty
{
protected:
	RefCounter	_ref;
public:
	ObjectWrapper(): _Ty() {}
	template<class _Arg1>
	ObjectWrapper(_Arg1 arg1): _Ty(arg1) {}
	template<class _Arg1, class _Arg2>
	ObjectWrapper(_Arg1 arg1, _Arg2 arg2): _Ty(arg1, arg2) {}
	template<class _Arg1, class _Arg2, class _Arg3>
	ObjectWrapper(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3): _Ty(arg1, arg2, arg3) {}
	template<class _Arg1, class _Arg2, class _Arg3, class _Arg4>
	ObjectWrapper(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3, _Arg4 arg4): _Ty(arg1, arg2, arg3, arg4) {}
	template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5>
	ObjectWrapper(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3, _Arg4 arg4, _Arg5 arg5): _Ty(arg1, arg2, arg3, arg4, arg5) {}
	template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5, class _Arg6>
	ObjectWrapper(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3, _Arg4 arg4, _Arg5 arg5, _Arg6 arg6): _Ty(arg1, arg2, arg3, arg4, arg5, arg6) {}

	void	AddRef() { return _ref.AddRef(); }
	void	Release()
	{
		if (_ref.ReleaseAndTest())
		{
			GcValueTrait<ObjectWrapper<_Ty> >::DisposeInstance(this);
			GcCreation<ObjectWrapper<_Ty> >::FreeInstanceMemory(this);
		}
	}
};

template<class Ty>
class gc_new: public ref<typename _TypeTraits<Ty>::type>
{
    typedef typename _TypeTraits<Ty>::type _Ty;

    using weak_ref<_Ty>::_ptr;		// gcc compliant

public:
    typedef ObjectWrapper<_Ty> WrapperType;
    typedef GcCreation<WrapperType> GcCreationType;

	template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5, class _Arg6>
	gc_new(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3, _Arg4 arg4, _Arg5 arg5, _Arg6 arg6)
	{
		_ptr = GcCreationType::AllocInstanceMemory();
		new(_ptr) WrapperType(arg1, arg2, arg3, arg4, arg5, arg6);
	}

	template<class _Arg1, class _Arg2, class _Arg3, class _Arg4, class _Arg5>
	gc_new(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3, _Arg4 arg4, _Arg5 arg5)
	{
		_ptr = GcCreationType::AllocInstanceMemory();
		new(_ptr) WrapperType(arg1, arg2, arg3, arg4, arg5);
	}

	template<class _Arg1, class _Arg2, class _Arg3, class _Arg4>
	gc_new(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3, _Arg4 arg4)
	{
		_ptr = GcCreationType::AllocInstanceMemory();
		new(_ptr) WrapperType(arg1, arg2, arg3, arg4);
	}

	template<class _Arg1, class _Arg2, class _Arg3>
	gc_new(_Arg1 arg1, _Arg2 arg2, _Arg3 arg3)
	{
		_ptr = GcCreationType::AllocInstanceMemory();
		new(_ptr) WrapperType(arg1, arg2, arg3);
	}

	template<class _Arg1, class _Arg2>
	gc_new(_Arg1 arg1, _Arg2 arg2)
	{
		_ptr = GcCreationType::AllocInstanceMemory();
		new(_ptr) WrapperType(arg1, arg2);
	}

	template<class _Arg1>
	gc_new(_Arg1 arg1)
	{
		_ptr = GcCreationType::AllocInstanceMemory();
		new(_ptr) WrapperType(arg1);
	}

	gc_new()
	{
		_ptr = GcCreationType::AllocInstanceMemory();
		new(_ptr) WrapperType();
	}
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
