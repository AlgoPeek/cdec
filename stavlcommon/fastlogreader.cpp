#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Fast LOG Reader
// -------------------------------------------------------------------------- //

BYTE FastLogReader::ReadByteExtend()
{
	ASSERT(m_offset == m_size1);
	if (m_size2 == 0)
		return 0;

	// Swap buffer
	BYTE* t = m_buffer1;
	m_buffer1 = m_buffer2;
	m_buffer2 = t;

	m_size1 = m_size2;
	m_offset = 0;

	m_size2 = m_istream->Read(m_buffer2, FLR_BUFFER_SIZE);

	return m_buffer1[m_offset++];
}

// -------------------------------------------------------------------------- //

FastLogReader::FastLogReader(ref<Stream> istream): m_istream(istream)
{
	m_istream->Seek(0, Stream::SeekBegin);
	m_buffer1 = (BYTE*)CoreAllocLC(FLR_BUFFER_SIZE);
	m_buffer2 = (BYTE*)CoreAllocLC(FLR_BUFFER_SIZE);

	m_size1 = m_istream->Read(m_buffer1, FLR_BUFFER_SIZE);
	m_size2 = m_istream->Read(m_buffer2, FLR_BUFFER_SIZE);
	m_offset = 0;
}

bool FastLogReader::ReadTextColumns(REF std::wstring16& buffer, REF std::vector<FastLogReader::Seg>& segs, WCHAR separator)
{
	if (IsEof())
		return false;

	buffer.clear();
	segs.clear();

	int pb = 0, pc = 0;
	while (!IsEof())
	{
		WCHAR ch = ReadChar();
		buffer += ch;
		if (ch == separator)
		{
			segs.push_back(Seg(pb, pc - pb));
			pb = pc + 1;
		}
		else if (ch == 10)	// \n
		{
			segs.push_back(Seg(pb, pc - pb));
			return true;
		}
		else if (ch == 13)	// \r, \r\n
		{
			if (PeekByte() == 10)
				ReadByte();
			segs.push_back(Seg(pb, pc - pb));
			return true;
		}
		++pc;
	}
	return true;
}

bool FastLogReader::ReadTextColumns(REF std::vector<std::wstring16>& ss, WCHAR separator)
{
	std::wstring16 buffer;
	std::vector<Seg> segs;
	ss.clear();

	bool f = ReadTextColumns(REF buffer, REF segs, separator);
	if (f)
	{
		for (std::vector<Seg>::iterator it = segs.begin(); it != segs.end(); ++it)
		{
			std::wstring16::iterator sit = buffer.begin() + it->Pos;
			ss.push_back(std::wstring16(sit, sit + it->Len));
		}
	}
	return f;
}

ref<StringArrayList> FastLogReader::ReadTextColumns(WCHAR separator)
{
	if (IsEof())
		return NULL;

	std::wstring16 seg;
	ref<StringArrayList> r = gc_new<StringArrayList>();
	while (!IsEof())
	{
		WCHAR ch = ReadChar();
		if (ch == separator)
		{
			r->Add(stringx(seg));
			seg.clear();
		}
		else if (ch == 10)	// \n
		{
			r->Add(stringx(seg));
			return r;
		}
		else if (ch == 13)	// \r, \r\n
		{
			if (PeekByte() == 10)
				ReadByte();
			r->Add(stringx(seg));
			return r;
		}
		else
			seg += ch;
	}
	r->Add(stringx(seg));
	return r;
}

void FastLogReader::Close()
{
	CoreFreeLC(m_buffer1, FLR_BUFFER_SIZE);
	CoreFreeLC(m_buffer2, FLR_BUFFER_SIZE);
	m_istream = NULL;
	m_buffer1 = m_buffer2 = NULL;
}

// -------------------------------------------------------------------------- //
CDEC_NS_END
