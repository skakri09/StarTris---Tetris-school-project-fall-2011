/********************************************************
*	File: PlayArea.h
*	Orginal Author:	Kristian Skarrseth / NITH / 2011
*	Changed by: -
*
*	Description: Handles the basic features of the play area
*				 of a tetris game. Holds stuff like the background
*				 (which includes landed pieces), check to see if 
*				 rows should be deleted, and functions to do so.
*
*	Special notes: Requires a reference pointer to the currentPiece array
*				   as parameter in many of the functions
*				   Drawing of background is not done internally. The class only
*				   holds the info required to draw on corrent spots
/
********************************************************/

#ifndef PLAY_AREA_H
#define PLAY_AREA_H

#include "startris.h"

class CPlayArea
{
public:
	CPlayArea();		//CPlayArea constructor
	~CPlayArea();		//CPlayArea destructor

	// Returns a reference pointer to the background array
	ETileColor* getBackgroundArrayReference() const;
	
	// Checks if currentPiece is crashing with landed pieces on the top row, in which case
	// bIsGameOver returns true. Else it returns false
	bool bIsGameOver(ETileColor* currentPieceReference);

	// Checks if a row has been filled, and should then be deleted. If it's the case, 
	// sub functions to delete rows and move above pieces down will be called
	void CheckIfShouldDeleteRowAndDelete();
	
	// Returns the number of rows that has been removed in this round
	unsigned short getNumberOfRowsRemoved() const;

	// Adds current piece to the background, aka landed pieces: Requires a reference to currentPiece and the x and y possition
	// of the currentPiece
	void addCurrentPieceToLandedPiecesArray(ETileColor* currentPieceReference, short landedPieceXPos, short landedPieceYPos);
	
	// Resets the background to "empty". That means wiping all landed pieces from the background array
	// and making it fully TC_BACKGROUND
	void resetBackgroundToEmpty();

private:
	unsigned short m_uiDeleteRowXCoordinate;
	unsigned short m_uiDeleteRowYCoordinate;
	unsigned short m_uiNumberOfRowsRemoved;
	unsigned short m_uiNumberOfFilledTilesInRow;

	ETileColor m_aPlayAreaBackground[g_usPLAYAREA_TILEWIDTH][g_usGAME_TILEHEIGHT];	// ETileColor array holding the background/landed pieces.

	ETileColor* m_pToBackgroundArray;					 // pointer to backgroundarray used to send to others
	ETileColor* m_pCurrentPiecePointer;					 // Pointer to currentPiece used internally
	ETileColor* m_pCurrentPiecePointerForGameOverCheck;	 // Pointer to currentPiece used by gameOver check

	// Deletes a filled row. Parameter is the y coordinate for the row to delete
	void deleteAFilledRow(unsigned int yCoordinateToDeleteFrom);

	// Moves all rows above the deleted row one tile down. Parameter gives the 
	// y coordinate to move down from
	void m_moveAllRowsDownAfterDelete(unsigned int yCoordinateToMoveDownFrom);
	
};//end CPlayArea

#endif