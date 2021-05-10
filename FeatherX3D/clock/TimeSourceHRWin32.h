
#ifndef TIMESOURCEHRWIN32_H_
#define TIMESOURCEHRWIN32_H_

#include <Windows.h>
#include <MMSystem.h>
#include "TimeSource.h"

#ifndef WIN64

//////////////////////////////////////////////////////////////////////////
inline unsigned __int64 getMisteryCount() 
{ 
	__asm 
	{ 
		mov edx,dword ptr ds:[7FFE000Ch]
		mov eax,dword ptr ds:[7FFE0008h] 
	} 
} 

inline unsigned int getElapsedMilliSecond() 
{ 
	return static_cast< unsigned int >( getMisteryCount()/10000 ); 
}

#else

inline unsigned int getElapsedMilliSecond()
{
	return timeGetTime();
};

#endif

//////////////////////////////////////////////////////////////////////////
class TimeSourceHRWin32 : public TimeSource
{
public:
    TimeSourceHRWin32( const int nCPU=0 );
    virtual double GetTime() const;


private:
    double GetSecondsPerTick() const;

    double m_secsPerTick;

	bool m_bInit;
};



#endif