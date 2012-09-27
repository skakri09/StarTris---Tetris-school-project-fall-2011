////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: KeyHeldDownTimer.cpp													  //
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

#include "KeyHeldDownTimer.h"

/*|---------------------------------------|*/
/*|  P U B L I C    F U N C T I O N S     |*/
/*|									      |*/

CKeyHeldDownTimer::CKeyHeldDownTimer(unsigned short usWantedUpdatesPerInterval,
						   unsigned short usIntervalTimeInMilliSeconds)
			: CLoopTimerBase(usWantedUpdatesPerInterval, usIntervalTimeInMilliSeconds)
{
	// tiny delay used for the start function 
	m_usKeyHeldDownDelay = 200;
}
	
CKeyHeldDownTimer::~CKeyHeldDownTimer()
{
}
// Rewritten start function. Includes a small delay on first update 
void CKeyHeldDownTimer::Start()
{
	m_usUpdatesLastInt = 0;
	m_usUpdatesCurrInt = 0;
	m_ui32CurrIntStartTick = SDL_GetTicks();
	m_ui32NextIntStartTick = m_usKeyHeldDownDelay + m_ui32CurrIntStartTick + m_usIntDur;
	m_ui32NextUpdateTick =  m_usKeyHeldDownDelay + m_ui32CurrIntStartTick + ((m_usUpdatesCurrInt + 1) * m_usIntDur / m_usWantedUPI);
}