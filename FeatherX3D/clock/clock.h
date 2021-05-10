
#ifndef CLOCK_H_
#define CLOCK_H_


#include <cstddef>
#include <vector>

#include "FBSingleton.h"
#include "Interface.h"
class TimeSource;

class Clock : public ISubject, public FBSingleton < Clock >
{
public:
    Clock(const TimeSource * pSource = NULL);
    ~Clock();

    void		SetTimeSource(const TimeSource * pSource, double dMinStep = 0.5 );

    void		FrameStep();

    double		GetTime() const { return m_currentTime; }
    int			GetFrameNumber() const { return m_frameNumber; }
    float		GetFrameRate() const;

    void		SetFiltering (int frameWindow, double frameDefault = 0.030);

public:
	// ¿Á¡§¿«ISubject
    double		GetFrameDuration() const { return m_frameTime; }
	void		AddObserver (IObserver * pObserver);
    void		RemoveObserver (IObserver * pObserver);

private:
    double		GetExactLastFrameDuration ();
    void		AddToFrameHistory (double exactFrameDuration);
    double		GetPredictedFrameDuration () const;


    const TimeSource* m_pTimeSource;
    double		m_currentTime;
    double		m_frameTime;
    int			m_frameNumber;

    double		m_sourceStartValue;
    double		m_sourceLastValue;

    int			m_frameFilteringWindow;
    double		m_frameDefaultTime;
    std::vector<double> m_frameDurationHistory;

    typedef std::vector<IObserver *> ObserverList;
    ObserverList m_observers;
	double		m_dMinStep;
};

#endif