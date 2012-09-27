////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: LoopTimerBase.h													  //
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


#ifndef LOOP_TIMER_BASE_H  // Inclusion guard.
#define LOOP_TIMER_BASE_H

// I N C L U D E S
// ===============

#include <SDL.h>

// C L A S S   D E C L A R A T I O N
// =================================

class CLoopTimerBase
{
public:
	// Constructor.
	CLoopTimerBase(unsigned short usWantedUpdatesPerInterval = 1, 
				   unsigned short usIntervalTimeInMilliSeconds = 10000);
	// Destructor.
	virtual ~CLoopTimerBase();
	
	// Returns true if it's time for a new update (unlike "SleepUntilUpdate()", above, this one 
	// doesn't call any sleep functionality. Its merely a check to see if enough time has passed 
	// since last update.)
	bool IsTimeForUpdate();
	
	// Call Start() when you start using the timers (when you enter the gameloop, or resume play after a pause).
	// Also call it whenever you have updated the "Updates per Interval" or "Interval Duration" variables, 
	// to activate the change immediately.
	// Made virtual so it can be changed by other classes inheriting from this (KeyHeldDownTimer)
	virtual void Start();  

protected:
	unsigned short	m_usWantedUPI;			 // Number of updates we're striving to get per interval.
	unsigned short	m_usIntDur;				 // Duration of interval in milliseconds.
	unsigned short	m_usUpdatesLastInt;		 // Number of updates last interval total.
	unsigned short	m_usUpdatesCurrInt;		 // Number of updates current interval so far.
	Uint32			m_ui32CurrIntStartTick;	 // When current interval started.
	Uint32			m_ui32NextIntStartTick;	 // When next interval starts.
	Uint32			m_ui32NextUpdateTick;	 // Ideal time for next update.
};



#endif  // LOOP_TIMER_BASE_H
 