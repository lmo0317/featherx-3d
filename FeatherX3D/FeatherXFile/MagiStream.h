


#pragma once

#include <stdio.h>
#include <string>
#include <tchar.h>

typedef unsigned __int64 uint64;
class MBCriticalSection;

//////////////////////////////////////////////////////////////////////////
class MagiStream
{
public:
	MagiStream() : m_Length(0), m_nRef(0) {};
	virtual ~MagiStream() {};

	enum SEEK_MODE
	{
		seek_cur = 1,
		seek_end = 2,
		seek_set = 0,
	};

	static void ViewError( int err );

	virtual size_t Read( void *pBuf, size_t size ) = 0;
	virtual size_t Write( const void *pBuf, size_t size ) = 0;
	virtual int Seek( uint64 offset, SEEK_MODE origin ) = 0;
	virtual uint64 Tell() = 0;
	virtual uint64 GetLength() = 0;
	virtual bool IsEOF() = 0;

	virtual bool IsValid() const = 0;

	inline void	IncRef() { ++m_nRef; }
	inline void	DecRef() { --m_nRef; }
	inline int Ref() const { return m_nRef; }

	static void ThreadInit();
	static void ThreadFinish();

protected:
	uint64  m_Length;
	int		m_nRef;

private:
};

//#define _USE_MEMORYMAPPED_IO

//////////////////////////////////////////////////////////////////////////
//fopen 사용
class MagiFileStream : public MagiStream
{
public:
	MagiFileStream();
	MagiFileStream( const TCHAR *filename, const TCHAR *mode );
	virtual ~MagiFileStream();

	virtual size_t Read( void *pBuf, size_t size );
	virtual size_t Write( const void *pBuf, size_t size );
	virtual int Seek( uint64 offset, SEEK_MODE origin );
	virtual uint64 Tell();
	virtual uint64 GetLength();
	virtual bool IsEOF();
	virtual bool IsValid() const;

	void FilePrint( const TCHAR *format, ... );
	bool Open( const TCHAR *filename, const TCHAR *mode );
	void Close();

private:

	FILE* m_pStream;
};

//////////////////////////////////////////////////////////////////////////
//fopen 사용
class MagiCompressFileStream : public MagiFileStream
{
public:
	MagiCompressFileStream();
	MagiCompressFileStream( const TCHAR *filename, const TCHAR *mode );
	virtual ~MagiCompressFileStream();

	void UnCompress();

	unsigned long  GetUnCompressBufLen();
	unsigned char* GetUnCompressBuf();

protected:
	unsigned long  m_dwDestLen;
	unsigned char* m_pDstBuf;
};

//////////////////////////////////////////////////////////////////////////
//CreateFile 사용
class MagiFileMapViewStream : public MagiStream
{
public:
	MagiFileMapViewStream();
#ifdef UNICODE
	MagiFileMapViewStream( const wchar_t *filename, unsigned long dwDesiredAccess, unsigned long dwShareMode, unsigned long dwCreationDisposition );
#else
	MagiFileMapViewStream( const char* filename, unsigned long dwDesiredAccess, unsigned long dwShareMode, unsigned long dwCreationDisposition );
#endif
	virtual ~MagiFileMapViewStream();

	virtual size_t Read( void *pBuf, size_t size );
	virtual size_t Write( const void *pBuf, size_t size );
	virtual int Seek( uint64 offset, SEEK_MODE origin );
	virtual uint64 Tell();
	virtual uint64 GetLength();
	virtual bool IsEOF();
	virtual bool IsValid() const;

	/* ex) Open( package_path, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING ); */
#ifdef UNICODE
	bool Open( const wchar_t *filename, unsigned long dwDesiredAccess, unsigned long dwShareMode, unsigned long dwCreationDisposition );
#else
	bool Open( const char *filename, unsigned long dwDesiredAccess, unsigned long dwShareMode, unsigned long dwCreationDisposition );
#endif

	void Close();

	enum MAP_MODE
	{
		MAP_READ,
		MAP_WRITE,
	};

	void *GetMap( int options );
	void FreeMap( void *pMap );

private:
	void*  m_handle;
	void*  m_hMap;
	void*  m_pMap;

};

//////////////////////////////////////////////////////////////////////////
//Packed File
class MagiPackedFileStream : public MagiStream
{
public:
	MagiPackedFileStream( FILE *fp, const TCHAR *szFileName, uint64 offset, uint64 size );
	virtual ~MagiPackedFileStream();

	virtual size_t Read( void *pBuf, size_t size );
	virtual size_t Write( const void *pBuf, size_t size );
	virtual int Seek( uint64 offset, SEEK_MODE origin );
	virtual uint64 Tell();
	virtual uint64 GetLength();
	virtual bool IsEOF();
	virtual bool IsValid() const;

private:
#ifdef UNICODE
	std::wstring m_strFileName;
#else
	std::string m_strFileName;
#endif

	FILE*  m_pStream;

	char*  m_pBuf;	  //Write 사용
	uint64 m_BufSize;

	uint64 m_Offset;
	uint64 m_nCurPos;

};

//////////////////////////////////////////////////////////////////////////
//Memory
class MagiMemoryStream : public MagiStream
{
public:
	MagiMemoryStream();
	MagiMemoryStream( size_t size );
	MagiMemoryStream( size_t size, char* pBuff );
	virtual ~MagiMemoryStream();

	virtual size_t Read( void *pBuf, size_t size );
	virtual size_t Write( const void *pBuf, size_t size );
	virtual int Seek( uint64 offset, SEEK_MODE origin );
	virtual uint64 Tell();
	virtual uint64 GetLength();
	virtual bool IsEOF();
	virtual bool IsValid() const;

	char*	GetBuf() { return m_pBuf; }
	void    SetSize( size_t size );
protected:
	char*	m_pBuf;
	uint64	m_nCurPos;
	uint64	m_nSize;
};

#ifdef _USE_MEMORYMAPPED_IO
#if 1
class MagiMemoryMappedStream : public MagiMemoryStream
{
public:
	~MagiMemoryMappedStream()
	{
		if( m_pAddress != NULL )
			UnLoad();
		m_pBuf = NULL;
	}
	size_t Read( void *pBuf, size_t size )
	{
		if( m_pAddress == NULL )
			Load();
		size_t readSize = MagiMemoryStream::Read(pBuf, size);
		if (IsEOF())
			UnLoad();
		return readSize;
	}

	bool Load()
	{
		unsigned __int64 alignedoff = m_Offset & (~0xFFFFLL); 
		m_pAddress = MapViewOfFile( m_hMapFile, FILE_MAP_READ, (unsigned long)(alignedoff>>32), (unsigned long)alignedoff, (unsigned int)(m_Offset + GetLength() - alignedoff));
		m_pBuf = (char*) m_pAddress + (m_Offset - alignedoff);
		_ASSERT(GetLastError() == 0);
		return m_pAddress != NULL ? true : false;
	}
	void UnLoad()
	{
		if( m_pAddress != NULL )
			UnmapViewOfFile( m_pAddress );
		m_pAddress = NULL;
	}

	static MagiStream * Create(HANDLE hMapFile, unsigned __int64 offset, size_t size)
	{
		MagiMemoryMappedStream * f = new MagiMemoryMappedStream(hMapFile, offset, size);

		if( f->Load() == false )
		{
			delete f;
			return NULL;
		}
		return f;
	}
protected :
	MagiMemoryMappedStream( HANDLE hMapFile, unsigned __int64 offset, size_t size ) : MagiMemoryStream(size, NULL)
	{
		m_hMapFile = hMapFile;
		m_pAddress = NULL;
		m_Offset = offset;
	}

	void * m_pAddress;
	unsigned long m_Offset;
	HANDLE m_hMapFile;
};
#else
class MagiMemoryMappedStream : public MagiMemoryStream
{
public:
	~MagiMemoryMappedStream()
	{
		UnmapViewOfFile( m_pAddress );
		m_pBuf = NULL;
	}
	static MagiStream * Create(HANDLE hMapFile, unsigned __int64 offset, size_t size)
	{
		unsigned __int64 alignedoff = offset & (~0xFFFFLL); 
		void * pAddress = MapViewOfFile( hMapFile, FILE_MAP_READ, (unsigned long)(alignedoff>>32), (unsigned long)alignedoff, (unsigned int)(offset + size - alignedoff));
		if (pAddress == NULL)
			return NULL;
		_ASSERT(GetLastError() == 0);
		return new MagiMemoryMappedStream( pAddress, (char*) pAddress + (offset - alignedoff), size );
	}
protected :
	MagiMemoryMappedStream( void * address, char* buff, int size ) : MagiMemoryStream(size, buff)
	{
		m_pAddress = address;
	}

private :
	void * m_pAddress;
};
#endif
#endif


//////////////////////////////////////////////////////////////////////////
//Memory
class MagiBufferStream : public MagiMemoryStream
{
public:
	MagiBufferStream( size_t capacity = 256, size_t step = 128 );
	virtual ~MagiBufferStream();

	virtual size_t Write( const void* ptr, unsigned long size );

protected:
	void resize( size_t newsize );
	size_t m_capacity;
	size_t m_step;
};


//////////////////////////////////////////////////////////////////////////
//Shared Memory
class MagiSharedMemory
{
public:
	MagiSharedMemory() : m_nSize(0), m_pBuf(NULL) {}

	void Init( size_t size, char* pBuff )
	{
		m_nSize = size;
		m_pBuf  = pBuff;
	}

	char*	GetBuf()	{ return m_pBuf; }
	uint64	GetSize()	{ return m_nSize; }

protected:
	char*	m_pBuf;
	uint64	m_nSize;
};