////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: LoopTimerBase.cpp													  //
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



// I N C L U D E S
// ===============

#include "LoopTimerBase.h"



// P U B L I C   F U N C T I O N S
// ===============================

CLoopTimerBase::CLoopTimerBase(unsigned short usWantedUpdatesPerInterval,    // Defaults to 60.
							   unsigned short usIntervalTimeInMilliSeconds)  // Defaults to 1000.
{
	// Calling own functions instead of setting variables directly to verify valid input.
	//SetUpdatesPerInterval(usWantedUpdatesPerInterval);
	//SetIntervalDuration(usIntervalTimeInMilliSeconds);
	m_usWantedUPI = usWantedUpdatesPerInterval;
	m_usIntDur = usIntervalTimeInMilliSeconds;

	// Setting the rest of the variables to default values.
	m_usUpdatesLastInt = 0;
	m_usUpdatesCurrInt = 0;
	m_ui32CurrIntStartTick = SDL_GetTicks();
	m_ui32NextIntStartTick = m_ui32CurrIntStartTick + m_usIntDur;
	m_ui32NextUpdateTick = m_ui32CurrIntStartTick + ((m_usUpdatesCurrInt + 1) * m_usIntDur / m_usWantedUPI);

}  // END CLoopTimer

CLoopTimerBase::~CLoopTimerBase()
{
}  // END ~CLoopTimer

// Call Start() when you start using the timers (when you enter the gameloop, or resume play after a pause).
// Also call it whenever you have updated the "Updates per Interval" or "Interval Duration" variables, 
// to activate the change immediately.
void CLoopTimerBase::Start()
{
	m_usUpdatesLastInt = 0;
	m_usUpdatesCurrInt = 0;
	m_ui32CurrIntStartTick = SDL_GetTicks();
	m_ui32NextIntStartTick = m_ui32CurrIntStartTick + m_usIntDur;
	m_ui32NextUpdateTick =   m_ui32CurrIntStartTick + ((m_usUpdatesCurrInt + 1) * m_usIntDur / m_usWantedUPI);

}  // END Start


// Returns true if it's time for a new update (unlike "SleepUntilUpdate()", above, this one 
// doesn't call any sleep functionality. Its merely a check to see if enough time has passed 
// since last update.)
bool CLoopTimerBase::IsTimeForUpdate()
{
	bool r_bDoUpdate = false;

	if (SDL_GetTicks() >= m_ui32NextUpdateTick)
	{
		r_bDoUpdate = true;

		// Update variables so they're ready for the next update again.
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

	return r_bDoUpdate;

}  // END IsTimeForUpdate
