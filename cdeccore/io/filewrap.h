#pragma once

CDEC_NS_BEGIN
// -------------------------------------------------------------------------- //
// File operation API wrapper
//
// Caution: Never use this class in the application code, it is only made
// for C-- library classes (such as FileStream)
// -------------------------------------------------------------------------- //

class FileWrapper
{
public:
	typedef FileStream::AccessMode AccessMode;
	typedef FileStream::ShareMode ShareMode;
	typedef FileStream::CreationMode CreationMode;
	typedef FileStream::SeekPosition SeekPosition;

private:

#if defined(X_OS_WINDOWS)
	HANDLE	m_hFile;
#else
	int		m_fd;
#endif

public:
	FileWrapper()
	{
	#if defined(X_OS_WINDOWS)
		m_hFile = INVALID_HANDLE_VALUE;
	#else
		m_fd = -1;
	#endif
	}

	~FileWrapper() { Close(); }

	bool	IsOpen();
	void	Open(PCWSTR path, AccessMode access, ShareMode share, bool fCreate);
	void	Open(PCWSTR path, AccessMode access, ShareMode share, CreationMode creation);
	int		Read(void* buffer, int cbToRead);
	int		Write(const void* buffer, int cbToWrite);
	int		AtomRead(INT64 pos, void* buffer, int cbToRead);
	int		AtomWrite(INT64 pos, const void* buffer, int cbToWrite);
	INT64	Seek(INT64 offset, SeekPosition origin);
	INT64	Pos() { return Seek(0, Stream::SeekCurrent); }
	INT64	Size();
	void	SetLength(INT64);
	void	Flush();
	void	Close();

	// Inter-operation functions
	// Do not use unless neccessary
#if defined(X_OS_WINDOWS)
	void FromHandle(HANDLE h);
	HANDLE GetHandle() { return m_hFile; }
#else
	void FromFD(int fd);
	int GetFD() { return m_fd; }
#endif

};

// -------------------------------------------------------------------------- //

#if defined(X_OS_WINDOWS)

inline bool FileWrapper::IsOpen() { return m_hFile != INVALID_HANDLE_VALUE; }

#else

inline bool FileWrapper::IsOpen() { return m_fd != -1; }

#endif

// -------------------------------------------------------------------------- //
CDEC_NS_END
