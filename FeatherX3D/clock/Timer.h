#ifndef TIMER_H_
#define TIMER_H_

#include "Interface.h"
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

namespace Magi
{
	class Timer : private IObserver
	{
	public:
		Timer(ISubject* pClock);
		~Timer();

		double		GetTime() const { return m_currentTime; }
		double		GetFrameDuration() const { return m_frameTime; }

		void		Pause (bool bOn);
		void		SetScale (float fScale);   

		bool		IsPaused () const;
		float		GetScale () const;
		void		NotifyFrame();

	private:
		virtual void Notify();

		ISubject*			m_pClock;

		double		m_currentTime;
		double		m_frameTime;

		bool		m_bPaused;
		float		m_fScale;
	};
	typedef boost::shared_ptr< Timer > TimerPtr;
	typedef boost::shared_array< Timer > TimerArray;
}

#endif
