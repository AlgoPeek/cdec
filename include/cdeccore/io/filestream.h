#pragma once

CDEC_NS_BEGIN

// -------------------------------------------------------------------------- //
// Windows Base Defines
// -------------------------------------------------------------------------- //

#define GENERIC_READ		(0x80000000L)
#define GENERIC_WRITE		(0x40000000L)

#define FILE_SHARE_READ		0x00000001  
#define FILE_SHARE_WRITE	0x00000002  
#define FILE_SHARE_DELETE	0x00000004  

#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5

// -------------------------------------------------------------------------- //
// File Stream
// -------------------------------------------------------------------------- //

class FileWrapper;

class CDECCOREEXPORT FileStream: public Stream
{
public:
	enum CreationMode
	{
		CreateNew = CREATE_NEW,
		CreateAlways = CREATE_ALWAYS,
		OpenExisting = OPEN_EXISTING,
		OpenAlways = OPEN_ALWAYS,
		TruncateExisting = TRUNCATE_EXISTING
	};

	enum AccessMode
	{
		AccessRead		= GENERIC_READ,
		AccessWrite		= GENERIC_WRITE,
		AccessReadWrite	= AccessRead | AccessWrite,
	};

	enum ShareMode
	{
		ShareRead		= FILE_SHARE_READ,
		ShareReadWrite	= FILE_SHARE_READ | FILE_SHARE_WRITE,
	};

protected:
	FileWrapper*	m_pFile;

	FileStream(FileWrapper* pFile): m_pFile(pFile) {}

public:
	FileStream(stringx filename, AccessMode accs, bool fCreate): m_pFile(NULL)
	{
		Open(filename, accs, fCreate);
	}

	FileStream(stringx filename, AccessMode accs, ShareMode share, bool fCreate): m_pFile(NULL)
	{
		Open(filename, accs, share, fCreate);
	}

#ifdef X_OS_WINDOWS
	FileStream(stringx filename, AccessMode accs, ShareMode share, CreationMode creation): m_pFile(NULL)
	{
		Open(filename, accs, share, creation);
	}
#endif

	int		Read(void* pv, int count);
	int		Read2(ref<ByteArray> buffer, int offset, int count);
	int		Write(const void* pv, int count);
	int		Write2(ref<ByteArray> buffer, int offset, int count);
	INT64	Seek(INT64 pos, SeekPosition cp = Stream::SeekBegin);
	void	SetLength(INT64 length);
	INT64	Position();
	INT64	Length();

	void	Flush();
	void	Close();

	int		AtomRead(INT64 pos, void* buffer, int count);
	int		AtomWrite(INT64 pos, const void* buffer, int count);
	
#ifdef X_OS_WINDOWS
	static ref<FileStream>	FromHandle(HANDLE hFile);
#endif

	FileWrapper* __Inner() { return m_pFile; }

protected:
	~FileStream() { Close(); }

	void Open(stringx filename, AccessMode accs, bool fCreate)
	{
		int share = ShareRead;
		if (accs == AccessRead)
			share |= ShareReadWrite;
		Open(filename, accs, (ShareMode)share, fCreate);
	}
	void Open(stringx filename, AccessMode accs, ShareMode share, bool fCreate);

#ifdef X_OS_WINDOWS
	void Open(stringx filename, AccessMode accs, ShareMode share, CreationMode creation);
#endif
};

// -------------------------------------------------------------------------- //

CDEC_NS_END
