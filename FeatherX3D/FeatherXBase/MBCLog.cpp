#include "FeatherXBasePrecompile.h"
#include "mbdef.h"
#include "MBCLog.h"

MBCLog::MBCLog()
{
	m_pFile = NULL;
	m_attribute = log_max;

	for( int i(0); LOG_FILE_MAX>i; ++i )
		m_pLOG_File[i] = NULL;
}

MBCLog::~MBCLog()
{
	Finish();
}

void MBCLog::Init( const char* pName )
{
	errno_t err;
	if( pName == NULL ) {
		if( err = fopen_s( &m_pFile, "MBLOG.TXT", "w" ) ) {
			m_pFile = NULL;
			DebugOutput( L"MBLOG open error" );
		}
	}
	else {
		if( err = fopen_s( &m_pFile, pName, "w" ) ) {
			m_pFile = NULL;
			DebugOutput( L"MBLOG open error" );
		}
	}
	//fopen(
	LOGOUTA( "MBLOG %s writting\n", pName );
}

void MBCLog::Init_Protocol( const TCHAR* pName, const TCHAR* pStaticsName/*=NULL*/ )
{
	m_strProtocol = pName;

	if( pStaticsName ) {
		m_strStatics = pStaticsName;
	}
}

void MBCLog::Finish()
{
	if( m_pFile ) {
		fclose( m_pFile );
	}

	for( int i(0); LOG_FILE_MAX>i; ++i ) {
		if( m_pLOG_File[i] ) {
			fclose( m_pLOG_File[i] );
		}
	}
}

void MBCLog::DebugOutputA( const char* pFormat, ... )
{
	va_list argptr;
	char szStr[2048];

	va_start( argptr, pFormat );
	int nLength = _vscprintf( pFormat, argptr )+1;
	vsprintf_s( szStr, nLength, pFormat, argptr );
	va_end( argptr );
//	strcat_s( szStr, 512, "\n" );

	if( m_attribute & log_console )
		printf( szStr );

	if( m_attribute & log_win )
		OutputDebugStringA( szStr );

	if( m_pFile && m_attribute & log_file ) {
		fprintf( m_pFile, "%s", szStr );
	}
}

void MBCLog::DebugOutput( const WCHAR* pFormat, ... )
{
	va_list argptr;
	WCHAR szStr[512];

	va_start( argptr, pFormat );
	int nLength = _vscwprintf( pFormat, argptr )+1;
	vswprintf_s( szStr, nLength, pFormat, argptr );
	va_end( argptr );
	wcscat_s( szStr, 512, L"\n" );

	if( m_attribute & log_win )
		OutputDebugStringW( szStr );

//#ifdef _DEBUG
	if( m_attribute & log_console )
	{
		wprintf( szStr );
	}

	if( m_pFile && m_attribute & log_file ) {
		fwprintf( m_pFile, L"%s", szStr );
	}
//#endif
}


void MBCLog::DebugOutputLmo( const WCHAR* pFormat, ... )
{
	va_list argptr;
	WCHAR szStr[512];

	va_start( argptr, pFormat );
	int nLength = _vscwprintf( pFormat, argptr )+1;
	vswprintf_s( szStr, nLength, pFormat, argptr );
	va_end( argptr );

	std::wstring strOutput;
	strOutput += L"---------- ";
	strOutput += szStr;
	strOutput += L" ----------\n";

	if( m_attribute & log_win )
	{
		OutputDebugStringW( strOutput.c_str());
	}

	if( m_attribute & log_console )
	{
		HANDLE hOutputHandle = INVALID_HANDLE_VALUE;
		CONSOLE_SCREEN_BUFFER_INFO oldConsoleBuf;
		if( GetCmdInfo( hOutputHandle, oldConsoleBuf ) )
		{
			::SetConsoleTextAttribute(hOutputHandle, ( ( oldConsoleBuf.wAttributes & 0xf0 ) ) | ( 14 & 0xf ) );
			wprintf(strOutput.c_str());
			::SetConsoleTextAttribute(hOutputHandle, oldConsoleBuf.wAttributes);
		}
	}

	if( m_pFile && m_attribute & log_file ) 
	{
		fwprintf( m_pFile, L"%s", strOutput.c_str() );
	}	
}

BOOL MBCLog::GetCmdInfo( OUT HANDLE& hConsole, OUT CONSOLE_SCREEN_BUFFER_INFO& cmdScreenBuff )
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if( INVALID_HANDLE_VALUE == hConsole )
		return FALSE;

	cmdScreenBuff = CONSOLE_SCREEN_BUFFER_INFO();

	return GetConsoleScreenBufferInfo( hConsole, &cmdScreenBuff );
}

HRESULT MBCLog::DxDebugOutput( HRESULT hResult, const WCHAR* pFormat, ... )
{
	va_list argptr;
	WCHAR szStr[512],szStr2[512];

	va_start( argptr, pFormat );
	int nLength = _vscwprintf( pFormat, argptr )+1;
	vswprintf_s( szStr, nLength, pFormat, argptr );
	va_end( argptr );
	swprintf_s( szStr,  512, L"%s : %s\n",   szStr, DXGetErrorString9W( hResult ) );
	swprintf_s( szStr2, 512, L"Desc : %s\n", DXGetErrorDescription9W( hResult ) );
	wcscat_s( szStr, 512, szStr2 );

	if( m_attribute & log_win )
		OutputDebugStringW( szStr );

	if( m_pFile && m_attribute & log_file ) {
		fwprintf( m_pFile, L"%s : %s\n", szStr, DXGetErrorString9W( hResult ) );
		fwprintf( m_pFile, L"Desc : %s\n", DXGetErrorDescription9W( hResult ) );
	}
	return hResult;
}

void MBCLog::OutputWindow( const WCHAR* pFormat, ... )
{
	va_list argptr;
	WCHAR szStr[1024];

	va_start( argptr, pFormat );
	int nLength = _vscwprintf( pFormat, argptr )+1;
	vswprintf_s( szStr, nLength, pFormat, argptr );
	va_end( argptr );
	wcscat_s( szStr, 1024, L"\n" );

	if( m_attribute )
		OutputDebugStringW( szStr );
}

void MBCLog::OutputStringFile( const WCHAR* pFormat, ... )
{
	va_list argptr;

	va_start( argptr, pFormat );
	int nLength = _vscwprintf( pFormat, argptr )+1;

	WCHAR *pStr = new WCHAR[nLength+3];
	memset( pStr, 0, sizeof(WCHAR)*(nLength+3) );

	vswprintf_s( pStr, nLength, pFormat, argptr );
	va_end( argptr );

	if( m_pFile && m_attribute ) {
		fwprintf( m_pFile, L"%s", pStr );
	}

	delete [] pStr;
}

void MBCLog::OutputFile( const WCHAR* pFormat, ... )
{
	va_list argptr;
	WCHAR szStr[2048];

	va_start( argptr, pFormat );
	int nLength = _vscwprintf( pFormat, argptr )+1;
	vswprintf_s( szStr, nLength, pFormat, argptr );
	va_end( argptr );
	wcscat_s( szStr, 2048, L"\n" );

	if( m_pFile && m_attribute ) {
		fwprintf( m_pFile, L"%s", szStr );
	}
}

void MBCLog::OutputFileForDungeonItem( const WCHAR* pFormat, ... )
{
	if( m_strDungeonItem.length() < 3 )
		return;

	errno_t err;
	err = _tfopen_s( &m_pLOG_File[LOG_DUNGEON_ITEM], m_strDungeonItem.c_str(), _T("a"));
	if( err )
		return;

	va_list argptr;
	WCHAR szStr[512];

	va_start( argptr, pFormat );
	int nLength = _vscwprintf( pFormat, argptr )+1;
	vswprintf_s( szStr, nLength, pFormat, argptr );
	va_end( argptr );
	wcscat_s( szStr, 512, L"\n" );

	if( m_attribute & log_win )
		OutputDebugStringW( szStr );

	//if( m_attribute & log_console )
	//	wprintf( szStr );

	if( m_pLOG_File[LOG_DUNGEON_ITEM] ) {
		fwprintf( m_pLOG_File[LOG_DUNGEON_ITEM], L"%s", szStr );

		fflush(m_pLOG_File[LOG_DUNGEON_ITEM]);
		fclose(m_pLOG_File[LOG_DUNGEON_ITEM]);
	}
}

void MBCLog::OutputFileForDungeonMob( const WCHAR* pFormat, ... )
{
	if( m_strDungeonMob.length() < 3 )
		return;

	errno_t err;
	err = _tfopen_s( &m_pLOG_File[LOG_DUNGEON_MOB], m_strDungeonMob.c_str(), _T("a"));
	if( err )
		return;

	va_list argptr;
	WCHAR szStr[512];

	va_start( argptr, pFormat );
	int nLength = _vscwprintf( pFormat, argptr )+1;
	vswprintf_s( szStr, nLength, pFormat, argptr );
	va_end( argptr );
	wcscat_s( szStr, 512, L"\n" );

	if( m_attribute & log_win )
		OutputDebugStringW( szStr );

	//if( m_attribute & log_console )
	//	wprintf( szStr );

	if( m_pLOG_File[LOG_DUNGEON_MOB] ) {
		fwprintf( m_pLOG_File[LOG_DUNGEON_MOB], L"%s", szStr );

		fflush(m_pLOG_File[LOG_DUNGEON_MOB]);
		fclose(m_pLOG_File[LOG_DUNGEON_MOB]);
	}
}

void MBCLog::OutputFileForStatics( const WCHAR* pFormat, ... )
{
	if( m_strStatics.length() < 3 )
		return;

	errno_t err;
	err = _tfopen_s( &m_pLOG_File[LOG_STATICS], m_strStatics.c_str(), _T("a"));
	if( err )
		return;

	va_list argptr;
	WCHAR szStr[512];

	va_start( argptr, pFormat );
	int nLength = _vscwprintf( pFormat, argptr )+1;
	vswprintf_s( szStr, nLength, pFormat, argptr );
	va_end( argptr );
	wcscat_s( szStr, 512, L"\n" );

	if( m_attribute & log_win )
		OutputDebugStringW( szStr );

	if( m_attribute & log_console )
		wprintf( szStr );

	if( m_pLOG_File[LOG_STATICS] ) {
		fwprintf( m_pLOG_File[LOG_STATICS], L"%s", szStr );

		fflush(m_pLOG_File[LOG_STATICS]);
		fclose(m_pLOG_File[LOG_STATICS]);
	}
}

//
void MBCLog::OutputFileForProtocol( const WCHAR* pFormat, ... )
{
	SYSTEMTIME	SystemTime;
	TCHAR		CurrentDate[32]					= {0,};
	TCHAR		CurrentFileName[MAX_PATH]		= {0,};
	GetLocalTime(&SystemTime);
	_sntprintf_s(CurrentDate, 32, _T("%04d-%02d-%02d %02d:%02d:%02d"),
		SystemTime.wYear, 
		SystemTime.wMonth, 
		SystemTime.wDay, 
		SystemTime.wHour,
		SystemTime.wMinute,
		SystemTime.wSecond);

	_sntprintf_s(CurrentFileName, MAX_PATH, _T("%s_%04d-%02d-%02d_%02d.log"), 
		m_strProtocol.c_str(),
		SystemTime.wYear, 
		SystemTime.wMonth, 
		SystemTime.wDay,
		SystemTime.wHour);

	errno_t err;
	if( err = _tfopen_s( &m_pLOG_File[LOG_PROTOCOL], CurrentFileName, _T("a") ) ) {
		return;
	}

	va_list argptr;
	WCHAR szStr[512];

	va_start( argptr, pFormat );
	int nLength = _vscwprintf( pFormat, argptr )+1;
	vswprintf_s( szStr, nLength, pFormat, argptr );
	va_end( argptr );
	wcscat_s( szStr, 512, L"\n" );

	if( m_attribute & log_win )
		OutputDebugStringW( szStr );

	if( m_attribute & log_console )
		wprintf( szStr );

	if( m_pLOG_File[LOG_PROTOCOL] ) {
		fwprintf( m_pLOG_File[LOG_PROTOCOL], L"%s", szStr );

		fflush(m_pLOG_File[LOG_PROTOCOL]);
		fclose(m_pLOG_File[LOG_PROTOCOL]);
	}
}

namespace 
{
	#define MAXLINELEN  60 /* max length for line in message box */
	#define ASSERTBUFSZ (MAXLINELEN * 9) /* 9 lines in message box */

#if defined (_M_IX86)
#define _DbgBreak() __asm { int 3 }
#elif defined (_M_IA64)
	void __break(int);
#pragma intrinsic (__break)
#define _DbgBreak() __break(0x80016)
#else  /* defined (_M_IA64) */
#define _DbgBreak() DebugBreak()
#endif  /* defined (_M_IA64) */
}

namespace LOG_UTIL
{
	void _out_consoleA( const char *str, ... )
	{
		char szBuf[1024];
		va_list va;
		va_start( va, str );
		vsnprintf_s( szBuf, sizeof(szBuf), _TRUNCATE, str, va );
		va_end( va );

		//		std::cout << szBuf;

		OutputDebugStringA( szBuf );
	}

	void _out_console( const TCHAR *str, ... )
	{
		TCHAR szBuf[2048];
		va_list va;
		va_start( va, str );
		_vsntprintf_s( szBuf, sizeof(szBuf), str, va );
		va_end( va );

		_tprintf( szBuf );

		OutputDebugString( szBuf );
	}

	MB_ERROR::~MB_ERROR() {
		_out_consoleA( "!!BOMB : %s %d\n", pFile, nLine );
//		_DbgBreak();
	}
};

/*
The assert support functions in this file were taken from assert.c in the
Microsoft C run time source code and may not be generally distributed.

They enhance the assert functionality in conjunction with an updated
assert define to avoid recursive asserts and to allow you to break
directly into your assert, instead of into assert.c.

If you need to work with these files, start from the last function in
this file.
*/

/***
*__crtMessageBox - call MessageBoxA dynamically.
*
*Purpose:
*	   Avoid static link with user32.dll. Only load it when actually needed.
*
*Entry:
*	   see MessageBoxA docs.
*
*Exit:
*	   see MessageBoxA docs.
*
*Exceptions:
*
*******************************************************************************/
static int __cdecl __crtMessageBoxA(
									LPCSTR lpText,
									LPCSTR lpCaption,
									UINT uType
									)
{
	static int (APIENTRY *pfnMessageBoxA)(HWND, LPCSTR, LPCSTR, UINT) = NULL;
	static HWND (APIENTRY *pfnGetActiveWindow)(void) = NULL;
	static HWND (APIENTRY *pfnGetLastActivePopup)(HWND) = NULL;

	HWND hWndParent = NULL;

	if (NULL == pfnMessageBoxA)
	{
		HINSTANCE hlib = LoadLibraryA("user32.dll");

		if (NULL == hlib || NULL == (pfnMessageBoxA =
			(int (APIENTRY *)(HWND, LPCSTR, LPCSTR, UINT))
			GetProcAddress(hlib, "MessageBoxA")))
			return 0;

		pfnGetActiveWindow = (HWND (APIENTRY *)(void))
			GetProcAddress(hlib, "GetActiveWindow");

		pfnGetLastActivePopup = (HWND (APIENTRY *)(HWND))
			GetProcAddress(hlib, "GetLastActivePopup");
	}

	if (pfnGetActiveWindow)
		hWndParent = (*pfnGetActiveWindow)();

	if (hWndParent != NULL ) {
		if( pfnGetLastActivePopup ) {
			hWndParent = (*pfnGetLastActivePopup)(hWndParent);
		}
	}

	OutputDebugStringA( lpText );
	OutputDebugStringA( "\t" );
	OutputDebugStringA( lpCaption );

	return (*pfnMessageBoxA)(hWndParent, lpText, lpCaption, uType);
}


void assert_failure(char const *file, int line, char const *expr)
{
	int nCode;
	char buf[ASSERTBUFSZ];
	_snprintf_s(buf, ASSERTBUFSZ, "Src : %s\nLine : %d\nAssertion failed\n%s\n", file, line, expr);
	buf[ASSERTBUFSZ-1] = 0;

	OutputDebugStringA( buf );

	nCode = __crtMessageBoxA(buf, "Microsoft Visual C++ Runtime Library", MB_ABORTRETRYIGNORE|MB_ICONHAND|MB_SETFOREGROUND|MB_TASKMODAL);

	/* Abort: abort the program */
	if (nCode == IDABORT)
	{
		/* raise abort signal */
		raise(SIGABRT);

		/* We usually won't get here, but it's possible that
		SIGABRT was ignored.  So exit the program anyway. */

		_exit(3);
	}

	/* Retry: call the debugger */
	if (nCode == IDRETRY)
	{
		_DbgBreak();
		/* return to user code */
		return;
	}

	/* Ignore: continue execution */
	if (nCode == IDIGNORE)
		return;
}


void FPClass(double x)
{            
#ifdef _DEBUG
	std::wstring s;            
	int i = _fpclass(x);            
	switch (i)            
	{            
	case _FPCLASS_SNAN: s = L"Signaling NaN";                _MBASSERT(0); break;            
	case _FPCLASS_QNAN: s = L"Quiet NaN";                    _MBASSERT(0); break;             
	case _FPCLASS_NINF: s = L"Negative infinity (-INF)";     _MBASSERT(0); break;             
	case _FPCLASS_NN:   s = L"Negative normalized non-zero"; break;            
	case _FPCLASS_ND:   s = L"Negative denormalized";        break;             
	case _FPCLASS_NZ:   s = L"Negative zero (-0)";           break;             
	case _FPCLASS_PZ:   s = L"Positive 0 (+0)";              break;             
	case _FPCLASS_PD:   s = L"Positive denormalized";        break;             
	case _FPCLASS_PN:   s = L"Positive normalized non-zero"; break;             
	case _FPCLASS_PINF: s = L"Positive infinity (+INF)";     _MBASSERT(0); break;            
	}        
#endif
}

void MBCLog::Init_Real( const char* pFolder, const char* pName )
{
}

void MBCLog::OutputFileReal( const WCHAR* pFormat, ... )
{
	va_list argptr;
	WCHAR szStr[2048];

	va_start( argptr, pFormat );
	int nLength = _vscwprintf( pFormat, argptr )+1;
	vswprintf_s( szStr, nLength, pFormat, argptr );
	va_end( argptr );
	wcscat_s( szStr, 2048, L"\n" );

	if( m_pRealFile ) {
		fwprintf( m_pRealFile, L"%s", szStr );
		fflush( m_pRealFile );
	}

	wprintf( L"%s", szStr );
}
