#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Windows Base Defines
// -------------------------------------------------------------------------- //

#define FILE_BEGIN			0
#define FILE_CURRENT		1
#define FILE_END			2

// -------------------------------------------------------------------------- //
// Stream
// -------------------------------------------------------------------------- //

class CDECCOREEXPORT Stream : public Object
{
	DECLARE_REF_CLASS(Stream)

public:
	enum SeekPosition
	{
		SeekBegin	= FILE_BEGIN,
		SeekCurrent	= FILE_CURRENT,
		SeekEnd		= FILE_END,
	};

protected:
	Stream() {}
	virtual ~Stream() {}

public:
	virtual int		Read(void* buffer, int count) = 0;
	virtual int		Read2(ref<ByteArray> buffer, int offset, int count) = 0;
	virtual int		Write(const void* buffer, int count) = 0;
	virtual int		Write2(ref<ByteArray> buffer, int offset, int count) = 0;
	virtual INT64	Seek(INT64 pos, SeekPosition cp = Stream::SeekBegin) = 0;
	virtual void	SetLength(INT64 length) = 0;
	virtual INT64	Position() = 0;
	virtual INT64	Length() = 0;
	virtual void	Flush() {}
	virtual void	Close() = 0;

	virtual int		AtomRead(INT64 pos, void* buffer, int count) = 0;
	virtual int		AtomWrite(INT64 pos, const void* buffer, int count) = 0;
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
