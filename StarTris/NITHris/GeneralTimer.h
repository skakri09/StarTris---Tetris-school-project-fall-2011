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


#ifndef GENERAL_TIMER_H
#define GENERAL_TIMER_H

#include "LoopTimerBase.h"
#include "startris.h"
#include <SDL.h>

class CGeneralTimer : public CLoopTimerBase
{
public:
	CGeneralTimer(unsigned short usWantedUpdatesPerInterval = g_usGENERAL_TIMER_UPI,
				  unsigned short usIntervalTimeInMilliSeconds = 1000);
	
	virtual ~CGeneralTimer();
	
	// This function makes the program (or current thread, if multi-threaded) halt execution until its 
	// time for another update. In other words, it frees up the CPU for other tasks until its time for 
	// a new update. 
	// NOTE: As with any kind of "sleep" functionality, we're not guaranteed to get control back at the 
	//       exact moment we want it, that's up to the OS (Windows) to handle. (99.9% of the time it 
	//       works like a charm though. :-)
	void SleepUntilUpdate();

private:
};

#endif