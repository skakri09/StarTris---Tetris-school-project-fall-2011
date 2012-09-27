
/********************************************************
*	File: GameManager.h
*	Orginal Author:	Kristian Skarseth / NITH / 2011
*	Changed by: -
*
*	Description: Uses elements from inputManager, looptimers,
*			     Soundmanager, playArea, piece and outputmanager
*				 to make a tetris game. The code itself is window -
*				 size dependant, but all you have to do to change 
*				 size of the game, is changing the constants in spacetris.h.
*				 However there is not programmed suport for resizing the graphics,
*				 so the background images will not follow resizing of the window.
*
*	Special notes: 
*				- Requires several SDL libraries to run. All input and output
*				  managment is done done through SDL
*				- All sound and music belongs to Blizzard Etertainment
*				  and is not to be used for profit.
/
********************************************************/


#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

/* Includes */
#include <iostream>
//#include <time.h>
//#include <fstream>
//#include <exception>

 /* Headers */
#include "inputManagerSDL.h"
#include "startris.h"
#include "PlayArea.h"
#include "outputManager.h"
#include "Piece.h"
#include "LoopTimerBase.h"
#include "GeneralTimer.h"
#include "PieceTimer.h"
#include "Score.h"
#include "soundManager.h"
#include "KeyHeldDownTimer.h"

// Namespace
using namespace std;

  
/*|----------------------------------------|*/
/*|  C L A S S    D E C L A R A T I O N S  |*/
/*|										   |*/
class CGameManager
{
public:
	CGameManager();								//CGameManager constructor
	~CGameManager();							//CGameManager destructor
	
	//Public initialization function. Initializes necessary parts of the game
	void Initialize();								

	enum eGameStatus {	MENU,						
						PLAY,
						PAUSE,
						GAMEOVER,
						QUIT		
					 };				

private:
	eGameStatus m_eCurrentGameState;			// creates m_eGameStatus object of enum eGameStatus
	
	/* Creates pointers objects of classes that CGameManager uses */
	CScore*				 m_pScoreManager;		// Handles current score and highscore. Saves highscore to txt
	CPiece*				 m_pPieceManager;		// Handles the piece. Rotation, collision managment etc
	CPlayArea*			 m_pPlayAreaManager;	// Handles the play area. Background, landed pieces, deleting of rows, adding of pieces to landed pieces. Also has gammOver check
	CPieceTimer*		 m_pPieceUpdateTimer;	// Timer for piece. Starting speed 20 updates per 15 sec. Increses with 1 update every time row(s) are removed
	CGeneralTimer*		 m_pGeneralUpdateTimer; // Timer for general updates (game loop speed). Default @ 60 updates per 
	COutputManager*		 m_pOutputManager;		// Handels graphic output
	CInputManagerSDL*	 m_pInputManager;		// Handles all input
	CLoopTimerBase*		 m_pComboTimerManager;	// Timer for combo-points. Interval and upi is default loopTimerBase (interval = 10sec, upi = 1)
	CKeyHeldDownTimer*   m_pKeyHeldDownTimer;	// Timer to check if a piece should automatically be moved in a direction when a key is pressed. Using pieceTimer class as it has the needed functionality
	CSoundManager*		 m_pSoundManager;		// Handles all sound managment
	
	ETileColor *m_pNextPiecePointer;			// Pointer that will point to a spot in the nextPiece array
	ETileColor *m_pCurrentPiecePointer;			// Pointer that will point to a spot in the currentPiece array
	ETileColor *m_pBackgroundPointer;			// Pointer that will point to a spot in the background array

	int m_iKeyInput;							// Stores key input from user
	
	unsigned short m_usNumberOfCellsHardDropped;// The number of cells hard-dropped with "space" key	
	unsigned short m_usPieceSpeed;				// CurrentPiece falling speed. 1 = 1 tile per second.
	unsigned short m_usTotalTimesRowsRemoved;	// Holds the total amount of times one or more rows has been removed
	unsigned short m_usNextLevelRequirement;	// Holds the amount of rows required to remove for next lvl
	unsigned short m_usCurrentLevel;			// Holds current level
	
	bool m_bGameLoop();							// The game loop
	bool m_bIsGameRunning;						// Returns true as long as its not game over. False if it is Game Over		
	bool m_bPieceHasBeenHardDropped;			// Returns true if a piece has been hard-dropped. Used to work arround piece update timer.
	
	void m_checkWhatToUpdateAndUpdate();		// Runs one of the below functions based on GameState
	void m_doMenuUpdates();						// Calls functions that are used during menu state
	void m_doGameUpdates();						// Calls functions that are used during while playing
	void m_doPauseUpdates();					// Calls functions that are used during pause state
	void m_doGameOverUpdates();					// Calls functions that are used during gameOver state

	void m_checkIfKeyPressAltersMenues();		// Checks if a key press alters the menu system in any way
	void m_checkIfKeyPressAltersGame();			// Checks if a key press alters the game in any way
	void m_checkForSingleKeyPress();			// Sub function of game keypress check. Checks for single key presses
	void m_checkForHoldDownKeyPress();		    // Sub function of game keypress check. Checks if user holds a key down
	void m_checkIfKeyPressAltersPause();		// Checks if a key press alters the pause in any way
	
	void m_drawNextPiece();						// Draws next piece
	void m_drawCurrentPiece();					// Draws current piece
	void m_drawBackgroundTiles();				// Draws the background
	void m_drawComboPoints() const;				// Draws comboPoints
	void m_drawHighscore() const;				// Draws the highscore
	void m_drawCurrentScore() const;			// Draws the current score
	void m_drawCurrentLevel() const;			// Draws the current level

	void m_dropCurrentPieceDown();				// Hard-drops the current piece down
	void m_updateCurrentScoreIfRowsRemoved();	// Updates the score if rows have been removed
	void m_addCurrentMakeNewIfPieceHasLanded(); // Adds old currentPiece to landed pieces if piece has landed, and gets a new currentpiece and nextPiece
	void m_checkIfIsGameOver();					// Checks if its gameOver
	void m_checkIfShouldPlaySpecialSound();		// Checks if a specialSound should be played (at this point it plays sound when gaining set amount of combo points
	void m_checkIfIsNextLevelAndUpdate();		// Checks if it's next level, 

};//end CGameManager
#endif