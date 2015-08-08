// -------------------------------------------------------------------------- //
// Array Extensive
// -------------------------------------------------------------------------- //

#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Fixed length in-memory array operation
// -------------------------------------------------------------------------- //

template<int ItemLen>
struct ArrayIterator
{
	typedef std::random_access_iterator_tag iterator_category;
	typedef BYTE* value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef const value_type* pointer;
	typedef const value_type& reference;

	BYTE* m_p;

	inline ArrayIterator(): m_p(NULL)
	{
	}

	inline explicit ArrayIterator(BYTE* p): m_p(p)
	{
	}

	inline reference operator*() const
	{
		return m_p;
	}

	inline ArrayIterator& operator++()
	{
		m_p += ItemLen;
		return *this;
	}

	inline ArrayIterator operator++(int)
	{
		ArrayIterator tmp = *this;
		++*this;
		return tmp;
	}

	inline ArrayIterator& operator--()
	{
		m_p -= ItemLen;
		return *this;
	}

	inline ArrayIterator operator--(int)
	{
		ArrayIterator tmp = *this;
		--*this;
		return tmp;
	}

	inline ArrayIterator& operator+=(difference_type off)
	{
		m_p += ItemLen * off;
		return *this;
	}

	inline ArrayIterator operator+(difference_type off) const
	{
		ArrayIterator tmp = *this;
		return tmp += off;
	}

	inline ArrayIterator& operator-=(difference_type off)
	{
		return *this += -off;
	}

	inline ArrayIterator operator-(difference_type off) const
	{
		return *this + (-off);
	}

	difference_type operator-(const ArrayIterator& rhs) const
	{
		ASSERT((m_p - rhs.m_p) % ItemLen == 0);
		return (m_p - rhs.m_p) / ItemLen;
	}

	reference operator[](difference_type off) const
	{
		return m_p + off * ItemLen;
	}

	bool operator==(const ArrayIterator& rhs) const
	{
		return m_p == rhs.m_p;
	}

	bool operator!=(const ArrayIterator& rhs) const
	{
		return m_p != rhs.m_p;
	}

	bool operator<(const ArrayIterator& rhs) const
	{
		return m_p < rhs.m_p;
	}

	bool operator>(const ArrayIterator& rhs) const
	{
		return m_p > rhs.m_p;
	}

	bool operator<=(const ArrayIterator& rhs) const
	{
		return m_p <= rhs.m_p;
	}

	bool operator>=(const ArrayIterator& rhs) const
	{
		return m_p >= rhs.m_p;
	}
};

template<int Len>
struct Compare_Base16
{
	int operator()(const BYTE* lhs, const BYTE* rhs) const
	{
		return memcmp(lhs, rhs, Len);
	}
};

template<int Len>
struct Less_Base16
{
	bool operator()(const BYTE* lhs, const BYTE* rhs) const
	{
		return memcmp(lhs, rhs, Len) < 0;
	}
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
