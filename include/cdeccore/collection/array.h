// This header contains the following:
//   - ArrayV
//	 - Array

#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Array for Value Types
// -------------------------------------------------------------------------- //

inline bool CheckOutOfRange(int offset, int length, int capacity)
{
	return offset < 0 || length < 0 || capacity - offset < length;
}

template<class _Ty>
class ArrayV: public Object
{
	DECLARE_REF_CLASS(Array<_Ty>)

public:
	typedef _Ty					value_type;
	typedef ArrayV<_Ty>			this_type;
	typedef GcCreation<_Ty>		gc_type;
	typedef GcValueTrait<_Ty>	vt_type;

protected:
	size_t			m_count;
	_Ty*			m_buffer;

protected:
	ArrayV(size_t count) : m_count(count), m_buffer(NULL)
	{
		if (count != 0)
		{
			m_buffer = gc_type::AllocArrayMemory(count);
			vt_type::InitArray(m_buffer, count);
		}
	}

	ArrayV(const _Ty* source, size_t count): m_count(count), m_buffer(NULL)
	{
		if (count != 0)
		{
			m_buffer = gc_type::AllocArrayMemory(count);
			vt_type::InitArrayCopy(m_buffer, source, count);
		}
	}

	ArrayV(ref<this_type> source, int offset, int count): m_count(count), m_buffer(NULL)
	{
		if (CheckOutOfRange(offset, count, source->Count()))
			cdec_throw(Exception(EC_OutOfRange));

		if (count != 0)
		{
			m_buffer = gc_type::AllocArrayMemory(count);

			pin_ptr<_Ty> pin = source->GetBuffer();
			vt_type::InitArrayCopy(m_buffer, pin.ptr() + offset, count);
		}
	}

	~ArrayV()
	{
		vt_type::DisposeArray(m_buffer, m_count);
		gc_type::FreeArrayMemory(m_buffer, m_count);
	}

public:
	size_t	Count() const { return m_count; }

	_Ty&	at(size_t index) { ASSERT(index < m_count); return m_buffer[index]; }
	_Ty&	operator[] (size_t index) { return at(index); }

	pin_ptr<_Ty> GetBuffer() { return pin_ptr<_Ty>::CreatePinptr(m_buffer); }

	ref<IEnumerator<_Ty> > GetEnumerator()
	{
		return gc_new<_ContinuousEnum<_Ty> >(m_buffer, m_count);
	}

	void	Sort() { std::sort(m_buffer, m_buffer + m_count); }

	void	CopyFrom(int offT, const _Ty* ps, int count)
	{
		if (CheckOutOfRange(offT, count, m_count))
			cdec_throw(Exception(EC_OutOfRange));

		_Ty* pt = m_buffer + offT;
		vt_type::DisposeArray(pt, count);
		vt_type::InitArrayCopy(pt, ps, count);
	}

	void	CopyFrom(int offT, ref<ArrayV<_Ty> > s, int offS, int count)
	{
		if (CheckOutOfRange(offT, count, m_count) || CheckOutOfRange(offS, count, s->Count()))
			cdec_throw(Exception(EC_OutOfRange));

		_Ty* ps = s->m_buffer + offS;
		_Ty* pt = m_buffer + offT;
		vt_type::DisposeArray(pt, count);
		vt_type::InitArrayCopy(pt, ps, count);
	}

	// Deprecated
	void	MemoryCopyFrom(int offT, const _Ty* ps, int count)
	{
		CopyFrom(offT, ps, count);
	}

	// Deprecated
	void	MemoryCopyFrom(int offT, ref<ArrayV<_Ty> > s, int offS, int count)
	{
		CopyFrom(offT, s, offS, count);
	}

	int		MemoryCompare(int offT, const _Ty* ps, int count)
	{
		if (CheckOutOfRange(offT, count, m_count))
			cdec_throw(Exception(EC_OutOfRange));

		_Ty* pt = m_buffer + offT;
		return memcmp(pt, ps, count * sizeof(_Ty));
	}

	int		MemoryCompare(int offT, ref<ArrayV<_Ty> > s, int offS, int count)
	{
		if (CheckOutOfRange(offT, count, m_count) || CheckOutOfRange(offS, count, s->Count()))
			cdec_throw(Exception(EC_OutOfRange));

		_Ty* pt = m_buffer + offT;
		_Ty* ps = s->m_buffer + offS;
		return memcmp(pt, ps, count * sizeof(_Ty));
	}

	void	Init(int offset, int count)
	{
		if (CheckOutOfRange(offset, count, m_count))
			cdec_throw(Exception(EC_OutOfRange));

		_Ty* pt = m_buffer + offset;
		vt_type::DisposeArray(pt, count);
		vt_type::InitArray(pt, count);
	}

	void	Fill(const _Ty& value, int offset, int count)
	{
		if (CheckOutOfRange(offset, count, m_count))
			cdec_throw(Exception(EC_OutOfRange));

		_Ty* ps = m_buffer + offset;
		for (int i = 0; i < count; ++i)
			*ps++ = value;
	}

	void	MemoryFill(_Ty value, int offset, int count)
	{
		if (CheckOutOfRange(offset, count, m_count))
			cdec_throw(Exception(EC_OutOfRange));

		_Ty* ps = m_buffer + offset;
		for (int i = 0; i < count; ++i)		// Not optimized
			*ps++ = value;		
	}

	ref<this_type>	GetRange(int offset, int count)
	{
		if (CheckOutOfRange(offset, count, m_count))
			cdec_throw(Exception(EC_OutOfRange));

		ref<this_type> r = gc_new<this_type>(m_buffer + offset, count);
		return r;
	}
};

// -------------------------------------------------------------------------- //
// Array for Reference Types
// -------------------------------------------------------------------------- //

template<class _Ty>
class Array: public ArrayV<ref<_Ty> >
{
	DECLARE_REF_CLASS(Array<_Ty>)

public:
	typedef ArrayV<ref<_Ty> >	base_type;
	typedef Array<_Ty>			this_type;

protected:
	using base_type::m_buffer;

	Array(size_t count) : base_type(count) {}
	Array(const ref<_Ty>* source, size_t count): base_type(source, count) {}
	Array(ref<this_type> source, int offset, int count): base_type(source, offset, count) {}
};

// -------------------------------------------------------------------------- //
// FAQ Alias
// -------------------------------------------------------------------------- //

typedef ArrayV<BYTE>		ByteArray;
typedef ArrayV<WORD>		WordArray;
typedef ArrayV<DWORD>		DWordArray;

typedef ArrayV<INT8>		Int8Array;
typedef ArrayV<INT16>		Int16Array;
typedef ArrayV<INT32>		Int32Array;
typedef ArrayV<INT64>		Int64Array;

typedef ArrayV<UINT8>		UInt8Array;
typedef ArrayV<UINT16>		UInt16Array;
typedef ArrayV<UINT32>		UInt32Array;
typedef ArrayV<UINT64>		UInt64Array;

typedef ArrayV<int>			IntArray;
typedef ArrayV<UINT>		UIntArray;
typedef ArrayV<WCHAR>		CharArray;
typedef ArrayV<stringx>		StringArray;

// -------------------------------------------------------------------------- //
CDEC_NS_END
