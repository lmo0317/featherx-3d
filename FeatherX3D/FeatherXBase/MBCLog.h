#ifndef _MBCLOG_H_
#define _MBCLOG_H_
/*
Create: 2004, 7 ... dobal
Last  : 2004, 7 ... dobal

Desc: Log Ãâ·Â..
*/

#include <wtypes.h>
#include "FBSingleton.h"
#include <stdio.h>
#include <string>
#include <cfloat>        

//#ifdef _DEBUG
	//#define DONOT_LOG
//#endif


#ifndef NDEBUG
#define _MBASSERT(x) \
	if (x); else assert_failure(__FILE__, __LINE__, #x)
void assert_failure(char const *file, int line, char const *expr);
#else
#define _MBASSERT(x) (void)0
#endif

void FPClass(double x);        

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
struct MBCheckMemory
{
	MBCheckMemory();
	~MBCheckMemory();
};

struct _TIME_CHECK_SIMPLE_
{
	_TIME_CHECK_SIMPLE_( const CHAR* pLog );

	~_TIME_CHECK_SIMPLE_();

	const CHAR* m_pLog;
	DWORD m_dwStart, m_dwCheckTime;
};

namespace LOG_UTIL
{
	void _out_consoleA( const char *str, ... );
	void _out_console( const TCHAR *str, ... );

	struct  MB_ERROR
	{
		MB_ERROR( char const *file, int line ) {
			pFile = file;
			nLine = line;
		}
		~MB_ERROR();

		char const * pFile;
		int nLine;
	};
};

enum LogAttribute
{ 
	log_console = 1<<1, 
	log_win     = 1<<2, 
	log_file    = 1<<3, 

	log_max = log_console | log_win | log_file 
};

class MBCLog : public FBSingleton< MBCLog >
{
public:

	MBCLog();
	~MBCLog();

	void	Init( const char* pName = NULL );
	void	Init_Real( const char* pFolder, const char* pName );
	void	Init_Protocol( const TCHAR* pName, const TCHAR* pStaticsName=NULL );

	void SetAttribute( LogAttribute attribute) { m_attribute = attribute; }
	void Finish();

	void DebugOutputA( const char* pFormat, ... );
	void DebugOutput( const WCHAR* pFormat, ... );
	void DebugOutputLmo( const WCHAR* pFormat, ... );

	void OutputWindow( const WCHAR* pFormat, ... );
	void OutputFile( const WCHAR* pFormat, ... );
	void OutputFileReal( const WCHAR* pFormat, ... );

	void OutputStringFile( const WCHAR* pFormat, ... );


	HRESULT DxDebugOutput( HRESULT hResult, const WCHAR* pFormat, ... );

	//
	void OutputFileForProtocol( const WCHAR* pFormat, ... );
	void OutputFileForStatics( const WCHAR* pFormat, ... );
	void OutputFileForDungeonItem( const WCHAR* pFormat, ... );
	void OutputFileForDungeonMob( const WCHAR* pFormat, ... );
	BOOL GetCmdInfo( OUT HANDLE& hConsole, OUT CONSOLE_SCREEN_BUFFER_INFO& cmdScreenBuff );

#ifdef UNICODE
	void SetOutputFileForDungeonItem( const WCHAR* pName ) { m_strDungeonItem = pName; }
	void SetOutputFileForDungeonMob( const WCHAR* pName )  { m_strDungeonMob  = pName; }
#else
	void SetOutputFileForDungeonItem( const CHAR* pName )  { m_strDungeonItem = pName; }
	void SetOutputFileForDungeonMob( const CHAR* pName )   { m_strDungeonMob  = pName; }
#endif

private:

	LogAttribute m_attribute;

#ifdef UNICODE
	std::wstring	m_strProtocol;
	std::wstring	m_strStatics;

	std::wstring	m_strDungeonItem;
	std::wstring	m_strDungeonMob;

#else
	std::string		m_strProtocol;
	std::string		m_strStatics;

	std::string		m_strDungeonItem;
	std::string		m_strDungeonMob;
#endif

	FILE* m_pFile;
	FILE* m_pRealFile;
	enum
	{
		LOG_PROTOCOL,
		LOG_STATICS,

		LOG_DUNGEON_ITEM,
		LOG_DUNGEON_MOB,

		LOG_FILE_MAX,
	};

	FILE* m_pLOG_File[LOG_FILE_MAX];
};

extern MBCLog g_Log;

#define LOGOUTA	  MBCLog::GetSingleton()->DebugOutputA

#define LOGOUT			MBCLog::GetSingleton()->DebugOutput
#define LOGOUT_LMO		MBCLog::GetSingleton()->DebugOutputLmo
#define LOGOUTFILE_REAL	MBCLog::GetSingleton()->OutputFileReal

#define DX_LOGOUT MBCLog::GetSingleton()->DxDebugOutput

#define LOGOUTWIN MBCLog::GetSingleton()->OutputWindow
#define LOGOUTFILE MBCLog::GetSingleton()->OutputFile

#define PROTOCOL_FILE MBCLog::GetSingleton()->OutputFileForProtocol
#define STATICS_FILE  MBCLog::GetSingleton()->OutputFileForStatics

#define DUNGEON_ITEM_FILE  MBCLog::GetSingleton()->OutputFileForDungeonItem
#define DUNGEON_MOB_FILE   MBCLog::GetSingleton()->OutputFileForDungeonMob

#endif
