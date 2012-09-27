
/********************************************************
*	File: Piece.h
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


#ifndef PIECE_H
#define PIECE_H

#include "startris.h"
#include <time.h>
#include <fstream>
class CPiece
{
public:
	CPiece();	//CPiece constructor
	~CPiece();	//CPiece destructor
	
	// Enum with the different piece Types
	enum EPieceType {	BoxPiece,
						TowerPiece,
						PyramidPiece,
						LeftHookPiece,
						RightHookPiece,
						LeftStairPiece,
						RightStairPiece
					};

	// Returns a reference to the nextPiece array	
	ETileColor* getNextPieceReference() const;
	
	// Returns a reference to the currentPiece array
	ETileColor* getCurrentPieceReference() const;
	
	// Copies the values from the array that nextPiecePointer is pointing to,
	// to the currently active currentActive, then calls makeNewRandNextPiece()
	// function to get a new random nextPiece
	void makeNewNextPiece();		
	
	// Returns true if there is collision between current piece and a landed piece.
	bool bIsCollisionBetweenCurrPieceAndLandedPieces(ETileColor* backgroundReference);
	
	// Copies values of the active currentArray to the inactive currentArray
	// with a simple algorithm that turns the piece 90 degrees clockwise. Then 
	// sets the inactive array as the active one.
	void rotateCurrentPiece();				
	
	// Sets a local pointer to point to the backgroundArray.
	void setLocalBackgroundArrayReference(ETileColor* backgroundArrayReference);

	// Increses and decreses X/Y position for currentpiece. Will move
	// currentPiece one tile in the specified direction
	void moveCurrentPieceOneTileLeft();
	void moveCurrentPieceOneTileRight();
	void moveCurrentPieceOneTileDown();
	void moveCurrentPieceOneTileUp();	// Used to reverd a move if a piece "fell" into another piece

	// Returns current X and Y position for the active currentPiece 
	short getCurrentPieceXPos() const;				
	short getCurrentPieceYPos() const;					

private:
	unsigned short randNumber;								// Used as random number to choose new nextPiece
	short m_sCurrPieceXPos;									// X pos for piece
	short m_sCurrPieceYPos;									// Y pos for piece
	bool m_bCurrPieceIsFirstArray;							// Decideds which array is used for currentPiece

	void m_makeNewRandNextPiece();					// Makes a new random piece and sets eNextPieceType
	void m_setCurrentPieceStartPosition();			// Sets currentPiece to the correct starting possition
	bool m_bIsWallCollision();						// Bool returning true if there is collision between current piece and a wall
	
	EPieceType m_eNextPieceType;							// Holding nextPiece piece type
	EPieceType m_eCurrentPieceType;							// Holding currentPiece piece type

	ETileColor* m_pNextPiecePointer;						// Pointer to nextPiece
	ETileColor* m_pCurrentPiecePointer;						// Pointer to the current currentPiece
	ETileColor* m_pCurrentPieceCollisionCheckPointer;		// Pointer used for collision check
	ETileColor* m_pLocalBackgroundArrayReferencePointer;	// Pointer to background array for some local use
	ETileColor* m_pCurrentPiecePointerForCollisionCheck;	// Pointer to currentPiece for collision check
	ETileColor* m_pBackgroundPointerForCollisionCheck;		// Pointer to background array for collision check
	
	// Two currentPiece arrays, storing the current piece. When rotating the currentPiece
	// The arrays are alternating which one is active.
	ETileColor m_aCurrentPieceTwo[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE];	
	ETileColor m_aCurrentPieceOne[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE];	
	
	/* ----------- Arrays with the base layout for all the 7 pieces ----------- */
	static ETileColor			  m_aBoxPiece[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE];
	static ETileColor			m_aTowerPiece[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE];
	static ETileColor		  m_aPyramidPiece[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE];
	static ETileColor		 m_aLeftHookPiece[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE];
	static ETileColor		m_aRightHookPiece[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE];
	static ETileColor		m_aLeftStairPiece[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE];
	static ETileColor	   m_aRightStairPiece[g_usPIECE_ARRAY_SIZE][g_usPIECE_ARRAY_SIZE];

}; //end CPiece
#endif
