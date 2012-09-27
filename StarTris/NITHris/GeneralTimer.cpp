////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: GeneralTimer.cpp													  //
//																			  //
//	Original author: Tomas Sandnes / NITH / 2007-2011						  //
//	Changed by:      Kristian Skarseth										  //
//			  - Removed several functions and put them in classes inheriting  //
//			  - from this instead. This way each object of the sub-classes	  //
//			  - wont include functions it has no use for.					  //
//	Examples: - The general update timer dont need to change upi/intervalTime //	
//			    or call isTimeForUpdate()									  //
//			  - The Piece timer dont need sleepUntilUpdate()				  //  
//																			  //
//	Descr.: A helper class for gameLoop timing.								  //
//																			  //
//	Special notes: Requires the SDL library.								  //
//																			  //
////////////////////////////////////////////////////////////////////////////////



#include "GeneralTimer.h"


CGeneralTimer::CGeneralTimer(unsigned short usWantedUpdatesPerInterval,
							 unsigned short usIntervalTimeInMilliSeconds)
	: CLoopTimerBase(usWantedUpdatesPerInterval, usIntervalTimeInMilliSeconds)
{
}
CGeneralTimer::~CGeneralTimer()
{
}


// This function makes the program (or current thread, if multi-threaded) halt execution until its 
// time for another update. In other words, it frees up the CPU for other tasks until its time for 
// a new update. 
// NOTE: As with any kind of "sleep" functionality, we're not guaranteed to get control back at the 
//       exact moment we want it, that's up to the OS (Windows) to handle. (99.9% of the time it 
//       works like a charm though. :-)
void CGeneralTimer::SleepUntilUpdate()
{
	// Sleep until it's time for coming update.
	long lTicksRemaining = m_ui32NextUpdateTick - SDL_GetTicks();
	if (0 < lTicksRemaining)
	{
		SDL_Delay(lTicksRemaining);
	}

	// Sleep done - update variables so they're ready for the next update again.
	++m_usUpdatesCurrInt;
	m_ui32NextUpdateTick = m_ui32CurrIntStartTick + ((m_usUpdatesCurrInt + 1) * m_usIntDur / m_usWantedUPI);
	while (m_ui32NextUpdateTick > m_ui32NextIntStartTick)
	{
		m_ui32CurrIntStartTick = m_ui32NextIntStartTick;
		m_ui32NextIntStartTick += m_usIntDur;
		m_usUpdatesLastInt = m_usUpdatesCurrInt;
		m_usUpdatesCurrInt = 0;
	}
}
