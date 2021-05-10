#include "stdafx.h"

#include "TimeSourceHRWin32.h"
#include <windows.h>
#include <assert.h>

TimeSourceHRWin32::TimeSourceHRWin32( const int nCPU/*=0*/ )
{
    LARGE_INTEGER freq;
    if( !::QueryPerformanceFrequency(&freq) )
	{
		m_bInit = false;
		DWORD dwErr = GetLastError();
		assert(0 && "Not support QueryPerformanceFrequency" );
	}
	else
	{
		m_bInit = true;
		m_secsPerTick = 1.0/freq.QuadPart;
	}

	if( nCPU > 0 ) {
		DWORD_PTR oldmask = ::SetThreadAffinityMask( GetCurrentThread(), nCPU );
	}
}


double TimeSourceHRWin32::GetTime() const
{
    LARGE_INTEGER time;
    ::QueryPerformanceCounter(&time);
    return time.QuadPart * m_secsPerTick;
}
