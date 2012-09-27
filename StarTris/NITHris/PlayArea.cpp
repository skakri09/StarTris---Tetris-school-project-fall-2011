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

#include "PlayArea.h"

/*|---------------------------------------|*/
/*|  P U B L I C    F U N C T I O N S     |*/
/*|									      |*/

CPlayArea::CPlayArea()
{
	m_pToBackgroundArray = &m_aPlayAreaBackground[0][0];
}

CPlayArea::~CPlayArea()
{
}

// Returns a reference pointer to the background array
ETileColor* CPlayArea::getBackgroundArrayReference() const
{
	return m_pToBackgroundArray;
}

// Checks if currentPiece is crashing with landed pieces on the top row, in which case
// bIsGameOver returns true. Else it returns false
bool CPlayArea::bIsGameOver(ETileColor* currentPieceReference)
{
	for(unsigned int x = 0; x < g_usPIECE_ARRAY_SIZE; ++x)
	{
		for(unsigned int y = 0; y < g_usPIECE_ARRAY_SIZE; ++y)
		{
			m_pCurrentPiecePointerForGameOverCheck = (currentPieceReference + (x * g_usPIECE_ARRAY_SIZE) + y );
			
			if(m_aPlayAreaBackground[y+ g_usPIECE_STARTPOS_X][x] != TC_BACKGROUND && *(m_pCurrentPiecePointerForGameOverCheck) != TC_NO_DRAW)
			{
				return true;
			}
		}
	}
	return false;
}

// Checks if a row has been filled, and should then be deleted. If it's the case, 
// sub functions to delete rows and move above pieces down will be called
void CPlayArea::CheckIfShouldDeleteRowAndDelete()
{
	m_uiNumberOfRowsRemoved = 0;
	m_uiDeleteRowXCoordinate = 0;
	m_uiDeleteRowYCoordinate = g_usGAME_TILEHEIGHT;
	for (m_uiDeleteRowYCoordinate; m_uiDeleteRowYCoordinate > 0; --m_uiDeleteRowYCoordinate)
	{
		m_uiNumberOfFilledTilesInRow = 0;
		m_uiDeleteRowXCoordinate = 0;
		for (m_uiDeleteRowXCoordinate; m_uiDeleteRowXCoordinate < g_usPLAYAREA_TILEWIDTH; ++m_uiDeleteRowXCoordinate)
		{
			if(m_aPlayAreaBackground[m_uiDeleteRowXCoordinate][m_uiDeleteRowYCoordinate] != TC_BACKGROUND)
			{
				++m_uiNumberOfFilledTilesInRow;
				if(m_uiNumberOfFilledTilesInRow == g_usPLAYAREA_TILEWIDTH)
				{
					++m_uiNumberOfRowsRemoved;
					deleteAFilledRow(m_uiDeleteRowYCoordinate);
					m_moveAllRowsDownAfterDelete(m_uiDeleteRowYCoordinate);
				}
			}
		}
	}
}

// Returns the number of rows that has been removed in this round
unsigned short CPlayArea::getNumberOfRowsRemoved()const
{
	return m_uiNumberOfRowsRemoved;
}

// Adds current piece to the background, aka landed pieces. 
// Requires a reference to currentPiece and the x and y possition of the currentPiece
void CPlayArea::addCurrentPieceToLandedPiecesArray(ETileColor* currentPieceReference, 
												   short landedPieceXPos, 
												   short landedPieceYPos)
{
	for (int x = 0; x < g_usPIECE_ARRAY_SIZE; ++x)
	{
		for (int y = 0; y < g_usPIECE_ARRAY_SIZE; ++y)
		{
			m_pCurrentPiecePointer = (currentPieceReference + (x * g_usPIECE_ARRAY_SIZE) + y );
			if(*(m_pCurrentPiecePointer) != TC_NO_DRAW)
			{
				m_aPlayAreaBackground[y + landedPieceXPos][x + landedPieceYPos] = *(m_pCurrentPiecePointer);
			}
			
		}
	}	
}

// Resets the background to "empty". That means wiping all landed pieces from the background array
// and making it fully TC_BACKGROUND
void CPlayArea::resetBackgroundToEmpty()
{
	for (int x = 0; x < g_usPLAYAREA_TILEWIDTH; ++x)
	{
		for (int y = 0; y < g_usGAME_TILEHEIGHT; ++y)
		{
				m_aPlayAreaBackground[x][y] = TC_BACKGROUND;
		}
	}
}

/*|------------------------------------------|*/
/*|   P R I V A T E    F U N C T I O N S     |*/
/*|										     |*/


// Deletes a filled row. Parameter is the y coordinate for the row to delete
void CPlayArea::deleteAFilledRow(unsigned int yCoordinateToDeleteFrom)
{
	for (unsigned short x= 0; x < g_usPLAYAREA_TILEWIDTH; ++x)
	{
		m_aPlayAreaBackground[x][yCoordinateToDeleteFrom] = TC_BACKGROUND;
	}
}

// Moves all rows above the deleted row one tile down. Parameter gives the 
// y coordinate to move down from
void CPlayArea::m_moveAllRowsDownAfterDelete(unsigned int yCoordinateToMoveDownFrom)
{
	for (unsigned short y = yCoordinateToMoveDownFrom; y > 0; --y)
	{
		for (unsigned short x = 0; x < g_usPLAYAREA_TILEWIDTH; ++x)
		{
			m_aPlayAreaBackground[x][y] = m_aPlayAreaBackground[x][y-1];
		}
	}
}