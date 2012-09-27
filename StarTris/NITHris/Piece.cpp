
/********************************************************
*	File: Piece.cpp
*	Orginal Author:	Kristian Skarrseth / NITH / 2011
*	Changed by: -
*
*	Description: Handles the basic needs of any piece in a tetris
*				 game. Trough public functions another class
*				 spawn new current and new pieces, move current piece
*				 left/right/down/(up to revert bad movement)
*				 and rotate piece. Also includes some helpfull 
*				 functions to give other classes references to current/nextpiece
*
*	Special notes: Can not directly draw a piece. Drawing must be done
*				   trough another class as is now.
*
/********************************************************/

#include "Piece.h"

/*|---------------------------------------|*/
/*|  P U B L I C    F U N C T I O N S     |*/
/*|									      |*/

CPiece::CPiece()
{
	//Seeds srand with timme, casting to int to avoid unnecessary warning
	srand (static_cast<int>(time(0)));
	
	// Makes nextPiecePointer pointer point to a new random piece
	m_makeNewRandNextPiece();				
	
	// Sets currentPiecePointer to point to m_aCurrentPieceOne
	m_pCurrentPiecePointer = &m_aCurrentPieceOne[0][0]; 
	
	// Says It's "true" that we are currently pointing to currentPieceOne
	m_bCurrPieceIsFirstArray = true;				
}

CPiece::~CPiece()
{	
}

// Returns a reference to the nextPiece array
ETileColor* CPiece::getNextPieceReference() const
{
	return m_pNextPiecePointer;
}//end getNextPieceReference()

// Returns a reference to the currentPiece array
ETileColor* CPiece::getCurrentPieceReference() const
{	
	return m_pCurrentPiecePointer;
}//end getCurrentPieceReference()

// Gives the CPiece class a reference to the background array to use for collision checks etc
void CPiece::setLocalBackgroundArrayReference(ETileColor* backgroundArrayReference) 
{
	m_pLocalBackgroundArrayReferencePointer = backgroundArrayReference;
}

// Increses and decreses X/Y position for currentpiece. Will move
// currentPiece one tile in the specified direction
void CPiece::moveCurrentPieceOneTileLeft()
{
	m_sCurrPieceXPos--;
	if(m_bIsWallCollision() || bIsCollisionBetweenCurrPieceAndLandedPieces(m_pLocalBackgroundArrayReferencePointer))
	{
		m_sCurrPieceXPos++;
	}
}
void CPiece::moveCurrentPieceOneTileRight()
{
	m_sCurrPieceXPos++;
	if(m_bIsWallCollision() || bIsCollisionBetweenCurrPieceAndLandedPieces(m_pLocalBackgroundArrayReferencePointer))
	{
		m_sCurrPieceXPos--;
	}
}
void CPiece::moveCurrentPieceOneTileDown()
{
	m_sCurrPieceYPos++;
	if(m_bIsWallCollision())
	{
		m_sCurrPieceYPos--;
	}
}
void CPiece::moveCurrentPieceOneTileUp()
{
	m_sCurrPieceYPos--;
}

// Returns current X and Y position for the active currentPiece 
short CPiece::getCurrentPieceXPos() const
{
	return m_sCurrPieceXPos;
}
short CPiece::getCurrentPieceYPos() const
{
	return m_sCurrPieceYPos;
}

// Copies the values from the array that nextPiecePointer is pointing to,
// to the currently active currentActive, then calls makeNewRandNextPiece()
// function to get a new random nextPiece
void CPiece::makeNewNextPiece()
{
	m_setCurrentPieceStartPosition();
	m_eCurrentPieceType = m_eNextPieceType;				// Sets currentPieceType to what nextPieceType was. Is used to decide if rotation is needed (not needed on BoxPiece)
	memcpy(m_pCurrentPiecePointer, m_pNextPiecePointer,	// Copies what nextPiecePointer points to, to what currentPiecePointer points to
		   sizeof(int)*((g_usPIECE_ARRAY_SIZE)*(g_usPIECE_ARRAY_SIZE))); 
	
	m_makeNewRandNextPiece();							// Makes a new random nextPiece
}

// Checks for collision between current piece and landed pieces. 
// Returns true if there is collision
bool CPiece::bIsCollisionBetweenCurrPieceAndLandedPieces(ETileColor* backgroundReference)
{
	for(unsigned int x = 0; x < g_usPIECE_ARRAY_SIZE; ++x)
	{
		for(unsigned int y = 0; y < g_usPIECE_ARRAY_SIZE; ++y)
		{
			m_pCurrentPiecePointerForCollisionCheck = (m_pCurrentPiecePointer + (x * g_usPIECE_ARRAY_SIZE) + y );//piece
			m_pBackgroundPointerForCollisionCheck = (m_pLocalBackgroundArrayReferencePointer + ( (y+m_sCurrPieceXPos)*g_usGAME_TILEHEIGHT) + (x+m_sCurrPieceYPos));//background
			if(*(m_pBackgroundPointerForCollisionCheck) != TC_BACKGROUND && m_sCurrPieceYPos != g_sPIECE_STARTPOS_Y && *(m_pCurrentPiecePointerForCollisionCheck) != TC_NO_DRAW)
			{
				return true;
			}
			else if(*(m_pCurrentPiecePointerForCollisionCheck) != TC_NO_DRAW && (m_sCurrPieceYPos + x) >= g_usGAME_TILEHEIGHT  && m_sCurrPieceYPos != g_sPIECE_STARTPOS_Y)
			{
 				return true;
			}
		}
	}
	return false;
}

// Copies values of the active currentArray to the inactive currentArray
// with a simple algorithm that turns the piece 90 degrees clockwise. Then 
// sets the inactive array as the active one. If not-allowed collision should
// happen after a turn, the first currentArray is still the active one.
void CPiece::rotateCurrentPiece()
{
	// Not allowing rotating a piece untill its base position is inside the screen, in case starting position is outside.
	// Also not rotating the piece if its a BoxPiece since it has no meaning
	if(m_sCurrPieceYPos >= -1 && m_eCurrentPieceType != BoxPiece) 
	{
		if(m_bCurrPieceIsFirstArray)
		{
			for(unsigned int x = 0; x < g_usPIECE_ARRAY_SIZE; ++x)
			{
				for(unsigned int y = 0; y < g_usPIECE_ARRAY_SIZE; ++y)
				{
					m_aCurrentPieceTwo[x][3-y] = m_aCurrentPieceOne[y][x];
				}
			}
			m_pCurrentPiecePointer = &m_aCurrentPieceTwo[0][0];
			m_bCurrPieceIsFirstArray = false;
			//If there is collision after the rotation, the pointer is not changed, and currentPieceOne is still the active one
			if(m_bIsWallCollision() || bIsCollisionBetweenCurrPieceAndLandedPieces(m_pLocalBackgroundArrayReferencePointer))
			{
				m_pCurrentPiecePointer = &m_aCurrentPieceOne[0][0];
				m_bCurrPieceIsFirstArray = true;
			}
		}
		else
		{
			for(unsigned int x = 0; x < 4; ++x)
			{
				for(unsigned int y = 0; y < 4; ++y)
				{
					m_aCurrentPieceOne[x][3-y] = m_aCurrentPieceTwo[y][x];
				} 
			}
			m_pCurrentPiecePointer = &m_aCurrentPieceOne[0][0];
			m_bCurrPieceIsFirstArray = true;
			//If there is collision after the rotation, the pointer is not changed, and currentPieceTwo is still the active one
			if(m_bIsWallCollision() || bIsCollisionBetweenCurrPieceAndLandedPieces(m_pLocalBackgroundArrayReferencePointer))
			{
				m_pCurrentPiecePointer = &m_aCurrentPieceTwo[0][0];
				m_bCurrPieceIsFirstArray = false;
			}
		}
	}
}//end rotateCurrentPiece()

/*|------------------------------------------|*/
/*|   P R I V A T E    F U N C T I O N S     |*/
/*|										     |*/


// Returns false as long as there is not collision between the walls
// and currentPiece
bool CPiece::m_bIsWallCollision()
{
	for(unsigned int x = 0; x < g_usPIECE_ARRAY_SIZE; ++x)
	{
		for(unsigned int y = 0; y < g_usPIECE_ARRAY_SIZE; ++y)
		{
			m_pCurrentPieceCollisionCheckPointer = ((getCurrentPieceReference()) + (x * g_usPIECE_ARRAY_SIZE) + y );
			if(*(m_pCurrentPieceCollisionCheckPointer) != TC_NO_DRAW && m_sCurrPieceXPos+y >= g_usPLAYAREA_TILEWIDTH)
			{
				return true;
			}
			else if(*(m_pCurrentPieceCollisionCheckPointer) != TC_NO_DRAW && m_sCurrPieceXPos+y < 0)
			{
				return true;
			}
		}
	}
	return false;
}

// Sets currentPiece to the correct starting possition
void CPiece::m_setCurrentPieceStartPosition()
{
	m_sCurrPieceXPos = g_usPIECE_STARTPOS_X;
	m_sCurrPieceYPos = g_sPIECE_STARTPOS_Y;
}

// Makes a new random piece and sets eNextPieceType
void CPiece::m_makeNewRandNextPiece()
{
	randNumber = (rand() % 7+1);
 	switch(randNumber)
	{
	case 1:
		m_pNextPiecePointer = &m_aBoxPiece[0][0];
		m_eNextPieceType = BoxPiece;
		break;
	case 2:
		m_pNextPiecePointer = &m_aTowerPiece[0][0];
		m_eNextPieceType = TowerPiece;
		break;
	case 3:
		m_pNextPiecePointer = &m_aPyramidPiece[0][0];
		m_eNextPieceType = PyramidPiece;
		break;
	case 4:
		m_pNextPiecePointer = &m_aLeftHookPiece[0][0];
		m_eNextPieceType = LeftHookPiece;
		break;
	case 5:
		m_pNextPiecePointer = &m_aRightHookPiece[0][0];
		m_eNextPieceType = RightHookPiece;
		break;
	case 6:
		m_pNextPiecePointer = &m_aLeftStairPiece[0][0];
		m_eNextPieceType = LeftStairPiece;
		break;
	case 7:
		m_pNextPiecePointer = &m_aRightStairPiece[0][0];
		m_eNextPieceType = RightStairPiece;
		break;
// memcpy(&m_nextPiece[0][0], &m_aRightStairPiece[0][0], sizeof(int)*((g_usPIECE_ARRAY_SIZE)*(g_usPIECE_ARRAY_SIZE)));
	} 
}

/* ----------- Arrays with the base layout for all the 7 pieces ----------- */
ETileColor CPiece::m_aTowerPiece[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE] =
{	{	TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW,	},
	{	TC_RED,		TC_RED,		TC_RED,		TC_RED,		},
	{	TC_NO_DRAW, TC_NO_DRAW,	TC_NO_DRAW,	TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW,	}
};
ETileColor CPiece::m_aRightStairPiece[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE] =
{	{	TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_GREEN,	TC_GREEN,	TC_NO_DRAW,	},
	{	TC_GREEN,	TC_GREEN,	TC_NO_DRAW,	TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW,	}
};
ETileColor CPiece::m_aRightHookPiece[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE] = 
{	{	TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_MAGENTA,	TC_MAGENTA,	TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_MAGENTA,	TC_NO_DRAW,	TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_MAGENTA, TC_NO_DRAW, TC_NO_DRAW,	}
};
ETileColor CPiece::m_aPyramidPiece[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE] = 
{	{	TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_YELLOW,	TC_NO_DRAW,	TC_NO_DRAW,	},
	{	TC_YELLOW,	TC_YELLOW,	TC_YELLOW,	TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW,	}
};
ETileColor CPiece::m_aLeftStairPiece[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE] = 
{	{	TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW,	},
	{	TC_CYAN,	TC_CYAN,	TC_NO_DRAW,	TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_CYAN,	TC_CYAN,	TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW,	}
};
ETileColor CPiece::m_aLeftHookPiece[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE] = 
{	{	TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_ORANGE,	TC_ORANGE,	TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_NO_DRAW,	TC_ORANGE,	TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_NO_DRAW, TC_ORANGE,  TC_NO_DRAW,	}
};
ETileColor CPiece::m_aBoxPiece[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE] = 
{	{	TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_BLUE,	TC_BLUE,	TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_BLUE,	TC_BLUE,	TC_NO_DRAW,	},
	{	TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW, TC_NO_DRAW,	}
};
