
#pragma once

//Win2K ¿ÃªÛ µø¿€µ .
#include <tchar.h>
#include <Windows.h>
#include <MMSystem.h>

//#ifndef WIN64

//inline unsigned __int64 getSystemCount() 
//{ 
//	__asm 
//	{ 
//		mov edx,dword ptr ds:[7FFE000Ch]
//		mov eax,dword ptr ds:[7FFE0008h] 
//	} 
//} 
//
//inline unsigned int getMilliSecTime()
//{
//	return static_cast< unsigned int >(getSystemCount() / 10000);
//};
//
//#else

inline unsigned int getMilliSecTime()
{
	return timeGetTime();
};

//#endif

class ElapsedTime
{
public:
	ElapsedTime( const WCHAR* pLog )
	{
		m_strLog = pLog;
		m_dwStartTime = getMilliSecTime();
	}

	~ElapsedTime()
	{
		LOGOUT( L"%s Check TIME %d ms", m_strLog.c_str(), getMilliSecTime()-m_dwStartTime );
	}

private:
	unsigned int m_dwStartTime;
	std::wstring m_strLog;
};


//Profile Util
class CheckPerformance
{
public:
	CheckPerformance( unsigned int dwCheckTime, const char* pFile, const char* pFunction, const int nLine )
	{
		m_dwCheckTime = dwCheckTime;
		m_dwStartTime = getMilliSecTime();

		m_strFile     = pFile;
		m_strFunction = pFunction;
		m_nLine	      = nLine;
	}

	~CheckPerformance()
	{
		if( (getMilliSecTime()-m_dwStartTime) > m_dwCheckTime )
		{
			//Log
			const int c_nTemp = 2048;
			char szTemp[c_nTemp];
			sprintf_s( szTemp, c_nTemp, "File:%s, Func:%s - Line:%d\n", m_strFile.c_str(), m_strFunction.c_str(), m_nLine );
			OutputDebugStringA( szTemp );
		}
	}

private:
	std::string  m_strFile;
	std::string  m_strFunction;
	int          m_nLine;
	unsigned int m_dwCheckTime;
	unsigned int m_dwStartTime;
};

struct _TIME_FUNCTION_CHECK_
{
	_TIME_FUNCTION_CHECK_( DWORD dwCheckTime, const char* pFile, const char* pFunction, const int nLine ) : m_strFile(pFile), m_strFunction(pFunction), m_nLine(nLine), m_dwCheckTime(dwCheckTime)
	{
		m_dwStart     = getMilliSecTime();
	}
	~_TIME_FUNCTION_CHECK_()
	{
		DWORD loadtime = getMilliSecTime()-m_dwStart;
		if( loadtime >= m_dwCheckTime ) {
			const int c_nTemp = 2048;
			char szTemp[c_nTemp];
			sprintf_s( szTemp, c_nTemp, "Time [ %010d ]ms - File:[ %s ], Function:[ %s ], Line: [ %d ]\n", loadtime, m_strFile.c_str(), m_strFunction.c_str(), m_nLine );
			OutputDebugStringA( szTemp );
		}
	}

	std::string		m_strFile;
	std::string		m_strFunction;
	int				m_nLine;
	DWORD m_dwStart, m_dwCheckTime;
};

struct _TIME_CHECK_
{
	_TIME_CHECK_( DWORD dwCheckTime, const char* pFile, const int nLine ) : m_strFile(pFile), m_nLine(nLine), m_dwCheckTime(dwCheckTime)
	{
		m_dwStart     = getMilliSecTime();
	}
	~_TIME_CHECK_()
	{
		DWORD loadtime = getMilliSecTime()-m_dwStart;
		if( loadtime >= m_dwCheckTime ) {
			const int c_nTemp = 2048;
			char szTemp[c_nTemp];
			sprintf_s( szTemp, c_nTemp, "Time [ %10d ]ms - File:[ %s ], Line:[ %d ]\n", loadtime, m_strFile.c_str(), m_nLine );
			OutputDebugStringA( szTemp );
		}
	}

	std::string		m_strFile;
	int				m_nLine;
	DWORD m_dwStart, m_dwCheckTime;
};


