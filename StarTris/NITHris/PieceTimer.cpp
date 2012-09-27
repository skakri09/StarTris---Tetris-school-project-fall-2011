////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: PieceTimer.cpp													  //
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

#include "PieceTimer.h"

/*|---------------------------------------|*/
/*|  P U B L I C    F U N C T I O N S     |*/
/*|									      |*/

CPieceTimer::CPieceTimer(unsigned short usWantedUpdatesPerInterval,
						 unsigned short usIntervalTimeInMilliSeconds)
	: CLoopTimerBase(usWantedUpdatesPerInterval, usIntervalTimeInMilliSeconds)
{
}

CPieceTimer::~CPieceTimer()
{
}

// Set number of wanted updates during the time interval.
void CPieceTimer::SetUpdatesPerInterval(unsigned short usWantedUPI = 20000) 
{ 
	if ((1 > usWantedUPI))
	{
		m_usWantedUPI = 1;
	}
	else
	{
		m_usWantedUPI = usWantedUPI;
	}

}  // END SetUpdatesPerInterval

// Set duration (in millisecs) of the time interval.
void CPieceTimer::SetIntervalDuration(unsigned short usIntDurInMilliSecs = 15000) 
{ 
	if ((1 > usIntDurInMilliSecs))
	{
		m_usIntDur = 1;
	}
	else
	{
		m_usIntDur = usIntDurInMilliSecs;
	}	

}  // END SetIntervalDuration

// Snapshots variables pieceTimer uses to be able to continue from same spot in the update-loop
void CPieceTimer::pausePieceTimer()
{
	m_usUpdatesLastIntSnapshot = m_usUpdatesLastInt;
	m_usUpdatesCurrIntSnapshot = m_usUpdatesCurrInt;
	m_ui32PausedTicks = SDL_GetTicks() - m_ui32CurrIntStartTick;
}

// Uses the "snapshotted" variables to set when next update is
void CPieceTimer::continuePieceTimer()
{
	m_usUpdatesLastInt = m_usUpdatesLastIntSnapshot;
	m_usUpdatesCurrInt = m_usUpdatesCurrIntSnapshot;
	m_ui32CurrIntStartTick = SDL_GetTicks() - m_ui32PausedTicks; 
	m_ui32NextIntStartTick = m_ui32CurrIntStartTick + m_usIntDur;
	m_ui32NextUpdateTick = m_ui32CurrIntStartTick + ((m_usUpdatesCurrInt + 1) * m_usIntDur / m_usWantedUPI);
}
