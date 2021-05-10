#include "stdafx.h"

#include "Timer.h"

using namespace Magi;

Timer::Timer(ISubject* pClock) :
	m_pClock (pClock),
	m_currentTime (0),
	m_frameTime (0),
	m_bPaused (false),
	m_fScale (1.0f)
{
	m_pClock->AddObserver(this);
}

Timer::~Timer()
{
	m_pClock->RemoveObserver(this);
}

void Timer::Notify ()
{
	if (!m_bPaused)
	{
		m_frameTime = m_pClock->GetFrameDuration() * m_fScale;
		m_currentTime += m_frameTime;
	}
}

void Timer::Pause (bool bOn)
{
    m_bPaused = bOn;
}

void Timer::SetScale(float fScale)
{
    m_fScale = fScale;
}

float Timer::GetScale () const
{
    return m_fScale;
}

bool Timer::IsPaused () const
{
    return m_bPaused;
}

void Timer::NotifyFrame()
{
	Notify();
}
