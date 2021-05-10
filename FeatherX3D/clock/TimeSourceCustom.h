#ifndef TIMESOURCECUSTOM_H_
#define TIMESOURCECUSTOM_H_

#include "TimeSource.h"
#include <vector>

class TimeSourceCustom : public TimeSource
{
public:
    TimeSourceCustom();
    virtual double GetTime() const;

    void QueueTime (double time);
    void QueueStep (double time);

private:
    mutable std::vector<double> m_timeQueue;
};

#endif
