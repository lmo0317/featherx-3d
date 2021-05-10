#include "stdafx.h"

#include "TickCounter.h"

#include "TimeSourceHRWin32.h"

#include <algorithm>

TickCounter::TickCounter(const TimeSourceHRWin32 * pSource) :
	m_pTimeSource (NULL),
	m_currentTime (0),
	m_frameTime (0),
	m_sourceStartValue (0),
	m_sourceLastValue (0)
{
	m_pTimeSource = pSource;
}

void TickCounter::SetTimeSource(const TimeSourceHRWin32 * pSource, double dMinStep)
{
	m_pTimeSource = pSource;

	if (m_pTimeSource != NULL)
    {
        m_sourceStartValue = m_pTimeSource->GetTime();
        m_sourceLastValue = m_sourceStartValue;
    }
}

void TickCounter::AddObserver (IObserver * pObserver)
{
    if (pObserver != NULL)
        m_observers.push_back(pObserver);
}

void TickCounter::RemoveObserver (IObserver * pObserver)
{
    m_observers.erase( std::remove(m_observers.begin(), m_observers.end(), pObserver), m_observers.end());
}

void TickCounter::FrameStep()
{
    m_frameTime = GetExactLastFrameDuration();
    m_currentTime += m_frameTime;

    ObserverList::iterator it;
    for (it = m_observers.begin(); it != m_observers.end(); ++it)
        (*it)->Notify();
}

double TickCounter::GetExactLastFrameDuration()
{
    double sourceTime;
    if (m_pTimeSource == NULL)
        sourceTime = 0;
    else
        sourceTime = m_pTimeSource->GetTime();

    double frameDuration = sourceTime - m_sourceLastValue;
    m_sourceLastValue = sourceTime;
    return frameDuration;
}


