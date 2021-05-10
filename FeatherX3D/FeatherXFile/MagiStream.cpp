
#include "MagiFileDef.h"
#include "MagiZlib.h"

#include "MagiStream.h"
#include <assert.h>
#include <errno.h>
#include <Windows.h>
#include "MBCLog.h"

using namespace LOG_UTIL;
const char* pErrorMsg[] = 
{
	"Operation not permitted	1",
	"No such file or directory	2",
	"No such process 3",                                                         
	"Interrupted function 4",
	"I/O error	5",
	"No such device or address	6",
	"Argument list too long 7",
	"Exec format error	8",
	"Bad file number 9",
	"No spawned processes 10",
	"No more processes or not enough memory or maximum nesting level reached 11",
	"Not enough memory	12",
	"Permission denied	13",
	"Bad address 14",
	"Device or resource busy 16",
	"File exists 17",
	"Cross-device link 18",
	"No such device 19",
	"Not a directory 20",
	"Is a directory 21",
	"Invalid argument 22",
	"Too many files open in system	23",
	"Too many open files 24",
	"Inappropriate I/O control operation 25",
	"File too large 27",
	"No space left on device 28",
	"Invalid seek 29",
	"Read-only file system	30",
	"Too many links 31",
	"Broken pipe 32",
	"Math argument 33",
	"Result too large 34",
	"Resource deadlock would occur	36",
	"Filename too long	38",
	"No locks available 39",
	"Function not supported 40",
	"Directory not empty 41",
	"Illegal byte sequence 42",
	"String was truncated 80",
};

//////////////////////////////////////////////////////////////////////////
/*
namespace
{
	class THREAD_BLOCK
	{
	public:
		THREAD_BLOCK(MBCriticalSection* pSection)
		{
//			_out_consoleA( "PackRead:%d\n", GetCurrentThreadId() );
			m_pSection = pSection;
			m_pSection->Enter();
		}
		~THREAD_BLOCK()
		{
			m_pSection->Leave();
//			_out_consoleA( "~PackRead:%d\n", GetCurrentThreadId() );
		}

		MBCriticalSection* m_pSection;
	};
}
MBCriticalSection* MagiStream::s_Sync = NULL;
*/


void MagiStream::ThreadInit()
{  
}

void MagiStream::ThreadFinish()
{  
}

void MagiStream::ViewError( int err )
{  
	switch( err ) 
	{
		case EPERM        : LOGOUTA("%s\n",pErrorMsg[0]); assert( 0 ); break;
		case ENOENT       : LOGOUTA("%s\n",pErrorMsg[1]); assert( 0 ); break;
		case ESRCH        : LOGOUTA("%s\n",pErrorMsg[2]); assert( 0 ); break;
		case EINTR        : LOGOUTA("%s\n",pErrorMsg[3]); assert( 0 ); break;
		case EIO          : LOGOUTA("%s\n",pErrorMsg[4]); assert( 0 ); break;
		case ENXIO        : LOGOUTA("%s\n",pErrorMsg[5]); assert( 0 ); break;
		case E2BIG        : LOGOUTA("%s\n",pErrorMsg[6]); assert( 0 ); break;
		case ENOEXEC      : LOGOUTA("%s\n",pErrorMsg[7]); assert( 0 ); break;
		case EBADF        : LOGOUTA("%s\n",pErrorMsg[8]); assert( 0 ); break;
		case ECHILD       : LOGOUTA("%s\n",pErrorMsg[9]); assert( 0 ); break;
		case EAGAIN       : LOGOUTA("%s\n",pErrorMsg[10]); assert( 0 ); break;
		case ENOMEM       : LOGOUTA("%s\n",pErrorMsg[11]); assert( 0 ); break;
		case EACCES       : LOGOUTA("%s\n",pErrorMsg[12]); assert( 0 ); break;
		case EFAULT       : LOGOUTA("%s\n",pErrorMsg[13]); assert( 0 ); break;
		case EBUSY        : LOGOUTA("%s\n",pErrorMsg[14]); assert( 0 ); break;
		case EEXIST       : LOGOUTA("%s\n",pErrorMsg[15]); assert( 0 ); break;
		case EXDEV        : LOGOUTA("%s\n",pErrorMsg[16]); assert( 0 ); break;
		case ENODEV       : LOGOUTA("%s\n",pErrorMsg[17]); assert( 0 ); break;
		case ENOTDIR      : LOGOUTA("%s\n",pErrorMsg[18]); assert( 0 ); break;
		case EISDIR       : LOGOUTA("%s\n",pErrorMsg[19]); assert( 0 ); break;
		case EINVAL       : LOGOUTA("%s\n",pErrorMsg[20]); assert( 0 ); break;
		case ENFILE       : LOGOUTA("%s\n",pErrorMsg[21]); assert( 0 ); break;
		case EMFILE       : LOGOUTA("%s\n",pErrorMsg[22]); assert( 0 ); break;
		case ENOTTY       : LOGOUTA("%s\n",pErrorMsg[23]); assert( 0 ); break;
		case EFBIG        : LOGOUTA("%s\n",pErrorMsg[24]); assert( 0 ); break;
		case ENOSPC       : LOGOUTA("%s\n",pErrorMsg[25]); assert( 0 ); break;
		case ESPIPE       : LOGOUTA("%s\n",pErrorMsg[26]); assert( 0 ); break;
		case EROFS        : LOGOUTA("%s\n",pErrorMsg[27]); assert( 0 ); break;
		case EMLINK       : LOGOUTA("%s\n",pErrorMsg[28]); assert( 0 ); break;
		case EPIPE        : LOGOUTA("%s\n",pErrorMsg[29]); assert( 0 ); break;
		case EDOM         : LOGOUTA("%s\n",pErrorMsg[30]); assert( 0 ); break;
		case ERANGE       : LOGOUTA("%s\n",pErrorMsg[31]); assert( 0 ); break;
		case EDEADLK      : LOGOUTA("%s\n",pErrorMsg[32]); assert( 0 ); break;
		case ENAMETOOLONG : LOGOUTA("%s\n",pErrorMsg[33]); assert( 0 ); break;
		case ENOLCK       : LOGOUTA("%s\n",pErrorMsg[34]); assert( 0 ); break;
		case ENOSYS       : LOGOUTA("%s\n",pErrorMsg[35]); assert( 0 ); break;
		case ENOTEMPTY    : LOGOUTA("%s\n",pErrorMsg[36]); assert( 0 ); break;
		case EILSEQ       : LOGOUTA("%s\n",pErrorMsg[37]); assert( 0 ); break;
		case STRUNCATE    : LOGOUTA("%s\n",pErrorMsg[38]); assert( 0 ); break;
	}

//	__asm { int 3 };
}

//////////////////////////////////////////////////////////////////////////
//fopen 사용
MagiFileStream::MagiFileStream()
{
	m_pStream = NULL;
	m_Length = 0;
}
#ifdef UNICODE
MagiFileStream::MagiFileStream( const wchar_t *filename, const wchar_t *mode )
#else
MagiFileStream::MagiFileStream( const char *filename, const char *mode )
#endif
{ 
	 
#ifdef _DEBUG
#ifdef UNICODE
	if( wcslen(filename) == wcslen(_T("c:\\gameclient\\")) && !_wcsicmp(filename, _T("c:\\gameclient\\") ) ) {
		_MBASSERT(0&&"존재 하지 않는 파일을 읽지 않도록 수정하시오!!!");
#else
	if( strlen(filename) == strlen(_T("c:\\gameclient\\")) && !_stricmp(filename, _T("c:\\gameclient\\") ) ) {
		_MBASSERT(0&&"존재 하지 않는 파일을 읽지 않도록 수정하시오!!!");
#endif
	}

#endif

//	LOGOUT( L"MagiFileStream [%s]", filename );

	m_pStream = NULL;

	Open( filename, mode );
}

MagiFileStream::~MagiFileStream()
{
	Close();
}

size_t MagiFileStream::Read( void *pBuf, size_t size )
{
	return fread( pBuf, size, 1, m_pStream );
}

size_t MagiFileStream::Write( const void *pBuf, size_t size )
{
	return fwrite( pBuf, size, 1, m_pStream );
}

int MagiFileStream::Seek( uint64 offset, SEEK_MODE origin )
{
	return _fseeki64( m_pStream, offset, origin );
}

bool MagiFileStream::IsValid() const
{
	return m_pStream != NULL ? true : false;
}

uint64	MagiFileStream::Tell()
{
	return _ftelli64( m_pStream );
}

bool MagiFileStream::IsEOF()
{
	return !!feof( m_pStream );
}

uint64 MagiFileStream::GetLength()
{
	if( m_Length == 0 ) //Write Mode
	{
		_fseeki64( m_pStream, 0, seek_end );
		m_Length = _ftelli64( m_pStream );
		_fseeki64( m_pStream, 0, seek_set );
	}

	return m_Length;
}

bool MagiFileStream::Open( const TCHAR *filename, const TCHAR *mode )
{

	errno_t err = _tfopen_s( &m_pStream, filename, mode );
	if( err != 0 )
	{
//		ViewError( err );
	}

	if( m_pStream )
	{
		_fseeki64( m_pStream, 0, seek_end );
		m_Length = _ftelli64( m_pStream );
		_fseeki64( m_pStream, 0, seek_set );
	}

	return IsValid();
}

void MagiFileStream::Close()
{
	if( IsValid() )
	{
		fclose( m_pStream );
		m_pStream = NULL;
	}
}

void MagiFileStream::FilePrint( const TCHAR *format, ... )
{
	if( IsValid() )
	{
		va_list va;
		va_start( va, format );
		_vftprintf( m_pStream, format, va );
		va_end( va );
	}
}

//////////////////////////////////////////////////////////////////////////
//
namespace
{
	const int	c_nUnitMegaSize = 1024*1024;
};

MagiCompressFileStream::MagiCompressFileStream() 
: m_pDstBuf(NULL), m_dwDestLen(0)
{

}
MagiCompressFileStream::MagiCompressFileStream( const TCHAR *filename, const TCHAR *mode ) 
: MagiFileStream( filename, mode ), m_pDstBuf(NULL), m_dwDestLen(0)
{

}

MagiCompressFileStream::~MagiCompressFileStream()
{
	if( m_pDstBuf ) {
		delete [] m_pDstBuf;
		m_pDstBuf = NULL;
	}
}

unsigned long  MagiCompressFileStream::GetUnCompressBufLen()
{
	return m_dwDestLen;
}	

unsigned char* MagiCompressFileStream::GetUnCompressBuf()
{
	return m_pDstBuf;
}

void MagiCompressFileStream::UnCompress()
{
	if( !IsValid() ) return;

	if( m_pDstBuf ) return;

	unsigned char* pSrcBuf = NULL;
	unsigned char* pDstBuf = NULL;

	unsigned long dwDestLen = 0;
	int nRetUnCompress      = 0;

	try
	{
		// 데이터 파일 크기 세팅
		unsigned long fsize = 0;
		fsize = (unsigned long)GetLength();

		if( fsize < c_nUnitMegaSize )
			dwDestLen = fsize*15;  //15배
		else
			dwDestLen = fsize*5;   //5배

		pSrcBuf = new unsigned char[fsize];
		_AUTO_ARRAY_<unsigned char> autoSrc( pSrcBuf );

		memset( pSrcBuf, 0, sizeof(unsigned char)*fsize );

		//원본
		Read( pSrcBuf, fsize );

		//XOR
		for( unsigned int i(0); fsize>i; i++ ) pSrcBuf[i] ^= SimpleXORCodec::GetEncodeKeyChar( i );

		while( true )
		{
 			pDstBuf = new unsigned char[dwDestLen];
			
			memset( pDstBuf, 0, sizeof(unsigned char)*dwDestLen );
			//압축
			nRetUnCompress = Magi_uncompress( pDstBuf, &dwDestLen, pSrcBuf, sizeof(unsigned char)*fsize );

			if( nRetUnCompress == 0/*Z_OK*/ )
			{
				m_dwDestLen = dwDestLen;
				m_pDstBuf   = pDstBuf;
				break;
			}
			else if( nRetUnCompress != -5/*Z_BUF_ERROR*/ )
			{
				//assert( 0 && "압축 실패" );
				break;
			}

			delete [] pDstBuf;
			pDstBuf = NULL;

			dwDestLen += fsize;  
		}
	}
	catch(std::bad_alloc &ba)
	{
		LOG_UTIL::_out_consoleA( "%s\n", ba.what() );
	}
}

//////////////////////////////////////////////////////////////////////////
//CreateFile 사용
MagiFileMapViewStream::MagiFileMapViewStream()
{  
	m_handle = INVALID_HANDLE_VALUE;
	m_hMap   = INVALID_HANDLE_VALUE;
	m_pMap   = NULL;
}
#ifdef UNICODE
MagiFileMapViewStream::MagiFileMapViewStream( const wchar_t *filename, unsigned long dwDesiredAccess, unsigned long dwShareMode, unsigned long dwCreationDisposition )
#else
MagiFileMapViewStream::MagiFileMapViewStream( const char *filename, unsigned long dwDesiredAccess, unsigned long dwShareMode, unsigned long dwCreationDisposition )
#endif
{  
	Open( filename, dwDesiredAccess, dwShareMode, dwCreationDisposition );
	m_hMap   = INVALID_HANDLE_VALUE;
	m_pMap   = NULL;
}

MagiFileMapViewStream::~MagiFileMapViewStream()
{  
	Close();
}

size_t MagiFileMapViewStream::Read( void *pBuf, size_t size )
{  
	size_t readsize;
	if( ::ReadFile( m_handle, pBuf, static_cast<unsigned long>(size), (unsigned long*)&readsize, NULL ) == TRUE ) {
		//성공
	}
	return readsize;
}

size_t MagiFileMapViewStream::Write( const void *pBuf, size_t size )
{  
	size_t writtensize;
	::WriteFile( m_handle, pBuf, static_cast<unsigned long>(size), (unsigned long*)&writtensize, NULL );
	return writtensize;
}

int MagiFileMapViewStream::Seek( uint64 offset, SEEK_MODE origin )
{  
	LARGE_INTEGER li_offset;
	li_offset.QuadPart = offset;
	return ::SetFilePointerEx( m_handle, li_offset, NULL, origin );
}

uint64 MagiFileMapViewStream::Tell()
{  
	LARGE_INTEGER li_offset;
	li_offset.QuadPart = 0;

	return ::SetFilePointerEx( m_handle, li_offset, NULL, FILE_CURRENT );
}

bool MagiFileMapViewStream::IsEOF()
{  
	return GetLength() == Tell();
}

bool MagiFileMapViewStream::IsValid() const
{
	return m_handle != INVALID_HANDLE_VALUE;
}

uint64 MagiFileMapViewStream::GetLength()
{  
	LARGE_INTEGER FileSize;

	uint64 size = 0;
	if( !GetFileSizeEx( m_handle, &FileSize ) )
	{
		_MBASSERT(0);
	}

	size = FileSize.QuadPart;

	return size;
}

#ifdef UNICODE
bool MagiFileMapViewStream::Open( const wchar_t *filename, unsigned long dwDesiredAccess, unsigned long dwShareMode, unsigned long dwCreationDisposition )
#else
bool MagiFileMapViewStream::Open( const char *filename, unsigned long dwDesiredAccess, unsigned long dwShareMode, unsigned long dwCreationDisposition )
#endif
{ 
	 
	m_handle = ::CreateFile( filename, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, 0 );
	
	if( m_handle == INVALID_HANDLE_VALUE )
	{
		assert( 0 && "CreateFile Failed" );
		return false;
	}

	return true;
}

void MagiFileMapViewStream::Close()
{  
	if ( m_pMap )                           ::UnmapViewOfFile( m_pMap );
	if ( m_hMap   != INVALID_HANDLE_VALUE ) ::CloseHandle( m_hMap );
	if ( m_handle != INVALID_HANDLE_VALUE ) ::CloseHandle( m_handle );

	m_pMap   = NULL;
	m_hMap   = INVALID_HANDLE_VALUE;
	m_handle = INVALID_HANDLE_VALUE;
}

void *MagiFileMapViewStream::GetMap( int options )
{  
	DWORD mode;
	DWORD mapmode;

	if ( m_hMap != INVALID_HANDLE_VALUE )
		return m_pMap;

	if( options & MAP_WRITE )
	{
		mode	= PAGE_READWRITE;
		mapmode = FILE_MAP_ALL_ACCESS;
	}
	else 
	{
		mode	= PAGE_READONLY;
		mapmode = FILE_MAP_READ;
	}

	m_hMap = CreateFileMapping( m_handle, NULL, mode, 0, static_cast<DWORD>(GetLength()), NULL ); 
	if ( m_hMap != INVALID_HANDLE_VALUE )
	{
		m_pMap = MapViewOfFile( m_hMap, mapmode, 0, 0, 0 );
		return m_pMap;
	}
	return NULL;
}

void MagiFileMapViewStream::FreeMap( void *pMap )
{  
	if ( m_pMap != NULL )
		UnmapViewOfFile( m_pMap );

	if ( m_hMap != INVALID_HANDLE_VALUE )
		CloseHandle( m_hMap );

	m_pMap = NULL;
	m_hMap = INVALID_HANDLE_VALUE;
}

//////////////////////////////////////////////////////////////////////////
//Packed File
MagiPackedFileStream::MagiPackedFileStream( FILE *fp, const TCHAR *szFileName, uint64 offset, uint64 size )
: m_pStream( fp ), m_strFileName( szFileName ), m_Offset( offset ), m_pBuf( NULL ), m_BufSize( 0 ), m_nCurPos( 0 )
{  
	m_Length = size;
}

MagiPackedFileStream::~MagiPackedFileStream()
{  
	if ( m_pBuf )
	{
		delete [] m_pBuf;
		m_pBuf = NULL;
	}
}

size_t MagiPackedFileStream::Read( void *pBuf, size_t read_size )
{  
	if( read_size == 0 ) return 0;

	if ( m_pStream == NULL )	return 0;

	if ( read_size+m_nCurPos > m_Length ) {
		_MBASSERT( 0 );
		MB_ERROR( __FILE__, __LINE__ );

		read_size = (size_t)(m_Length - m_nCurPos);

		if( read_size == 0 ) return 0;
	}

	_fseeki64( m_pStream, static_cast<long>(m_Offset+m_nCurPos), SEEK_SET );

	if ( fread( pBuf, read_size, 1, m_pStream ) == 1 )
	{
		m_nCurPos += read_size;
		return read_size;
	}

	errno_t err;
	_get_errno( &err );
	ViewError( err );

	_MBASSERT( 0 );
	MB_ERROR( __FILE__, __LINE__ );
	return 0;
}

size_t MagiPackedFileStream::Write( const void *pBuf, size_t write_size )
{  
	if ( m_pStream == NULL ) return 0;

	if ( m_nCurPos + write_size > m_BufSize )
	{
		char *pNewBuf = new char[ (size_t)m_BufSize + write_size ];
		m_BufSize = m_BufSize + write_size;

		if ( m_pBuf )
		{
			memcpy( pNewBuf, m_pBuf, (size_t)m_Length );
			delete [] m_pBuf;			
		}

		m_pBuf = pNewBuf;
	}

	memcpy( &m_pBuf[ m_nCurPos ], pBuf, write_size );
	m_nCurPos += write_size;

	if ( m_nCurPos > m_Length ) m_Length = m_nCurPos;

	return write_size;
}

int MagiPackedFileStream::Seek( uint64 offset, SEEK_MODE origin )
{  
	if ( m_pStream == NULL )	return -1;

	if( offset > m_Length ) {
		_MBASSERT( 0 && "Error MagiPackedFileStream::Seek" );
		MB_ERROR( __FILE__, __LINE__ );
	}

	switch(origin)
	{
	case seek_set:
		m_nCurPos = offset;
		break;
	case seek_cur:
		m_nCurPos += offset;
		break;
	case seek_end:
		m_nCurPos = m_Length;
		break;
	}

	return (int)m_nCurPos;
}


uint64 MagiPackedFileStream::Tell()
{  
	if ( m_pStream == NULL )	return 0;

	return m_nCurPos;
}

uint64 MagiPackedFileStream::GetLength()
{  
	return m_Length;
}

bool MagiPackedFileStream::IsEOF()
{  
	return GetLength() == Tell();
}

bool MagiPackedFileStream::IsValid() const
{
	return m_pStream != NULL ? true : false;
}

//////////////////////////////////////////////////////////////////////////
//Memory File

MagiMemoryStream::MagiMemoryStream() : m_pBuf(NULL), m_nSize(0), m_nCurPos(0)
{

}

MagiMemoryStream::MagiMemoryStream( size_t size )
{
	m_pBuf = new char[size];
	m_nSize = size;
	m_nCurPos = 0;
}

MagiMemoryStream::MagiMemoryStream( size_t size, char* pBuff )
{
	m_pBuf = pBuff;
	m_nSize = size;
	m_nCurPos = 0;
}

MagiMemoryStream::~MagiMemoryStream()
{
	if( m_pBuf ) delete [] m_pBuf;
	m_pBuf = NULL;
}

void MagiMemoryStream::SetSize( size_t size )
{
	m_pBuf = new char[size];
	m_nSize = size;
	m_nCurPos = 0;
}

size_t MagiMemoryStream::Read( void *pBuf, size_t size )
{
	if ( size+m_nCurPos > m_nSize )
		size = (size_t)(m_nSize - m_nCurPos);

	if ( size > 0 )
	{
		memcpy( pBuf, m_pBuf+m_nCurPos, size );
		m_nCurPos += size;
		return size;
	}
	return 0;
}

size_t MagiMemoryStream::Write( const void *pBuf, size_t size )
{
	if ( size+m_nCurPos > m_nSize )
		size = (size_t)(m_nSize - m_nCurPos);

	if ( size > 0 )
	{
		memcpy( m_pBuf+m_nCurPos, pBuf, size );
		m_nCurPos += size;
		return size;
	}
	return 0;
}

int MagiMemoryStream::Seek( uint64 offset, SEEK_MODE origin )
{
	if( !IsValid() ) return -1;

	switch(origin)
	{
	case seek_set:
		m_nCurPos = offset;
		break;
	case seek_cur:
		m_nCurPos += offset;
		break;
	case seek_end:
		m_nCurPos = m_nSize;
		break;
	}

	if ( m_nCurPos < 0       ) { _MBASSERT(0); m_nCurPos = 0;       }
	if ( m_nCurPos > m_nSize ) { _MBASSERT(0); m_nCurPos = m_nSize; }

	return (int)m_nCurPos;
}

uint64 MagiMemoryStream::Tell()
{
	return m_nCurPos;	
}

uint64 MagiMemoryStream::GetLength()
{
	return m_nSize;
}

bool MagiMemoryStream::IsEOF()
{
	return m_nCurPos == m_nSize;
}

bool MagiMemoryStream::IsValid() const
{
	return m_pBuf != NULL;
}

//////////////////// buffer stream ///////////////////
MagiBufferStream::MagiBufferStream( size_t capacity, size_t step )
: MagiMemoryStream( capacity ) 
{
	m_capacity = capacity;
	m_step = step;
}

MagiBufferStream::~MagiBufferStream()
{  
}

void MagiBufferStream::resize(size_t newsize)
{  
	size_t oldCapacity = m_capacity;
	m_capacity = ((newsize-1) / m_step + 1) * m_step;
	char* pNewBuf = new char[ m_capacity ];
	memcpy( pNewBuf, m_pBuf, oldCapacity );

	delete[] m_pBuf;
	m_pBuf = pNewBuf;
}

size_t MagiBufferStream::Write( const void *ptr, unsigned long size )
{  
	if ( m_nCurPos + size > m_nSize ) {
		m_nSize = m_nCurPos+size;
		if ( m_nSize > m_capacity ) 
			resize( (size_t)m_nSize );
	}
	memcpy( m_pBuf + m_nCurPos, ptr, size );
	m_nCurPos += size;
	return size;
}
