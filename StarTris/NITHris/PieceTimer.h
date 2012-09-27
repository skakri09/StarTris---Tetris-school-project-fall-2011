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

#ifndef PIECE_TIMER_H
#define PIECE_TIMER_H

#include "LoopTimerBase.h"
#include <SDL.h>

class CPieceTimer : public CLoopTimerBase
{
public:
	CPieceTimer(unsigned short usWantedUpdatesPerInterval = 20,
				unsigned short usIntervalTimeInMilliSeconds = 15000);
	virtual ~CPieceTimer();

	// Set number of wanted updates during the time interval.
	void SetUpdatesPerInterval( unsigned short usWantedUPI );
	
	// Set duration (in millisecs) of the time interval.
	void SetIntervalDuration( unsigned short usIntDurInMilliSecs );

	// Snapshots variables pieceTimer uses to be able to continue from same spot in the update-loop
	void pausePieceTimer();
	
	// Uses the "snapshotted" variables to set when next update is
	void continuePieceTimer();

private:
	// "Snapshot of the timer" variables to store the snapshotted values
	unsigned short	m_usUpdatesLastIntSnapshot;		
	unsigned short	m_usUpdatesCurrIntSnapshot;		
	Uint32			m_ui32PausedTicks;				

};

#endif