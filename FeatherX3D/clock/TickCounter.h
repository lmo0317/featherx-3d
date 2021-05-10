#pragma once

#include <cstddef>
#include <vector>

#include "Interface.h"
#include "FBSingleton.h"

class TimeSourceHRWin32;

class TickCounter : public ISubject, public FBSingleton < TickCounter >
{
public:
	TickCounter(const TimeSourceHRWin32 * pSource = NULL);
	~TickCounter(void){}
	// ISubject ¿Á¡§¿«
	double		GetFrameDuration() const { return m_frameTime; }
	void		AddObserver (IObserver * pObserver);
    void		RemoveObserver (IObserver * pObserver);
public:
	void SetTimeSource(const TimeSourceHRWin32 * pSource, double dMinStep = 0);

	void		FrameStep();
	double		GetExactLastFrameDuration();

protected:

private:
    typedef std::vector<IObserver *> ObserverList;
    ObserverList m_observers;
    const TimeSourceHRWin32* m_pTimeSource;

	double		m_currentTime;
    double		m_frameTime;

	double		m_sourceStartValue;
    double		m_sourceLastValue;

};
