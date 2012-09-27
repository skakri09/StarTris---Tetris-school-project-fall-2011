////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: KeyHeldDownTimer.h														  //
//																			  //
//	Original author: Kristian Skarseth / NITH / 2011						  //
//	Changed by:   															  //
//																			  //
//	Descr.: A helper class for simple timers. Interval and duration must      //
//			be set trough the constructor. After that you can call start()	  //
//			and isTimeForUpdate() that SimpleTimer inherits					  //
//																			  //
//	Special notes: Requires the SDL library.								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

#ifndef SIMPLE_TIMER_H
#define SIMPLE_TIMER_H

#include "LoopTimerBase.h"

class CKeyHeldDownTimer : public CLoopTimerBase
{
public:
	CKeyHeldDownTimer(unsigned short usWantedUpdatesPerInterval = 10,
				      unsigned short usIntervalTimeInMilliSeconds = 1000);
	
	virtual ~CKeyHeldDownTimer();
	
	// Rewriting the start function from LoopTimerBase to have an additional
	// delay on nextUpdate when calling start() from this class.
	virtual void Start();  

private:
	unsigned short m_usKeyHeldDownDelay; // tiny delay used for the start function 

};


#endif