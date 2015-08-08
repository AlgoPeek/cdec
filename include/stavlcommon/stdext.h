// -------------------------------------------------------------------------- //
// Standard Library Extensive
// -------------------------------------------------------------------------- //

#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Common
// -------------------------------------------------------------------------- //

class auto_buffer	// Not a Cdec class
{
public:
	typedef BYTE		value_type;
	typedef size_t		size_type;
	typedef off_t		difference_type;

	typedef BYTE*		pointer;
	typedef BYTE&		reference;
	typedef const BYTE*	const_pointer;
	typedef const BYTE&	const_reference;

	typedef BYTE*		iterator;
	typedef const BYTE*	const_iterator;

protected:
	BYTE*	m_ptr;
	size_t	m_size;

protected:
	auto_buffer(const auto_buffer& rhs)
	{
		cdec_throw(Exception(EC_NotImplemented));
	}

public:
	auto_buffer(size_type size): m_size(size)
	{
		m_ptr = (BYTE*)malloc(size);
	}

	~auto_buffer() { close(); }

	void close()
	{
		free(m_ptr);
		m_ptr = NULL;
		m_size = 0;
	}

	size_type size() const { return m_size; }

	pointer ptr() { return m_ptr; }
	const_pointer ptr() const { return m_ptr; }

	iterator begin() { return m_ptr; }
	iterator end() { return m_ptr + m_size; }
	const_iterator begin() const { return m_ptr; }
	const_iterator end() const { return m_ptr + m_size; }

	void take_owner(auto_buffer& rhs)
	{
		close();
		m_ptr = rhs.m_ptr;
		m_size = rhs.m_size;
		rhs.m_ptr = NULL;
		rhs.m_size = 0;
	}
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
