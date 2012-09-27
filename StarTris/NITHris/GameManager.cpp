
/********************************************************
*	File: GameManager.cpp
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

#include "GameManager.h"

/*|---------------------------------------|*/
/*|  P U B L I C    F U N C T I O N S     |*/
/*|									      |*/

CGameManager::CGameManager()
{
	try
	{	
		cout << "Allocating memory for ScoreManager" << endl;
		m_pScoreManager		  = new CScore;
		
		cout << "Allocating memory for PlayAreaManager" << endl;
		m_pPlayAreaManager	  = new CPlayArea;
		
		cout << "Allocating memory for PieceUpdateTimer" << endl;
		m_pPieceUpdateTimer	  = new CPieceTimer;
		
		cout << "Allocating memory for SoundManager" << endl;
		m_pSoundManager		  = new CSoundManager;
		
		cout << "Allocating memory for PieceManager" << endl;
		m_pPieceManager		  = new CPiece;
		
		cout << "Allocating memory for GeneralUpdateTimer" << endl;
		m_pGeneralUpdateTimer = new CGeneralTimer;
		
		cout << "Allocating memory for OutputManager" << endl;
		m_pOutputManager	  = new COutputManager;
		
		cout << "Allocating memory for ComboTimerManager" << endl;
		m_pComboTimerManager  = new CLoopTimerBase;
		
		cout << "Allocating memory for InputManager" << endl;
		m_pInputManager		  = new CInputManagerSDL;	

		cout << "Allocating memory for KeyHeldDownTimer" << endl << endl;
		m_pKeyHeldDownTimer	  = new CKeyHeldDownTimer;
	}	
	catch (const bad_alloc& oBadAllocation)
	{
		cout << "Not able to allocate: " << oBadAllocation.what() << endl;
		exit(EXIT_FAILURE);
	}
	
}//end CGameManager()

CGameManager::~CGameManager()
{
	if(m_pScoreManager)
	{
		delete m_pScoreManager;
		m_pScoreManager = 0;
	}

	if(m_pPlayAreaManager)
	{
		delete m_pPlayAreaManager;
		m_pPlayAreaManager = 0;
	}

	if(m_pPieceUpdateTimer)
	{
		delete m_pPieceUpdateTimer;
		m_pPieceUpdateTimer = 0;
	}

	if(m_pSoundManager)
	{
		delete m_pSoundManager;
		m_pSoundManager = 0;
	}

	if(m_pPieceManager)
	{
		delete m_pPieceManager;
		m_pPieceManager = 0;
	}

	if(m_pGeneralUpdateTimer)
	{
		delete m_pGeneralUpdateTimer;
		m_pGeneralUpdateTimer = 0;
	}

	if(m_pOutputManager)
	{
		delete m_pOutputManager;
		m_pOutputManager = 0;
	}

	if(m_pComboTimerManager)
	{
		delete m_pComboTimerManager;
		m_pComboTimerManager = 0;
	}

	if(m_pInputManager)
	{
		delete m_pInputManager;
		m_pInputManager = 0;
	}
	if(m_pKeyHeldDownTimer)
	{
		delete m_pKeyHeldDownTimer;
		m_pKeyHeldDownTimer = 0;
	}

}//end CGameManager()

//Public initialization function. Initializes necessary parts of the game
void CGameManager::Initialize()
{
	try
	{
		cout << "Initializing Game...." << endl << endl;
		cout << "Initializing SDL subsystems and application window..." << endl;
		m_pOutputManager->Init("StarTris");							// Initializes the SDL subsystems and application window stuff
		cout << "SDL subsystems and application window successfully initialized" << endl << endl;
		
		cout << "Loading and initializing sounds..." << endl;
		m_pSoundManager->initSounds();								// Initializing sounds
		cout << "Sounds successfully loaded initialized" << endl << endl;

		cout << "Loading and updating highscore..." << endl;
		m_pScoreManager->loadHighscore();							// Updates the highscore (Checks external highscore.txt with saved highscore)
		cout << "Highscore successfully loaded and updated " << endl << endl;
		
		m_pGeneralUpdateTimer->Start();								// Starts the general update timer (default @ 60 fps)
		cout << "GeneralUpdate timer successfully started" << endl << endl;
		
m_pOutputManager->DrawMenuBackground();						// Draws the game-area menu background
		m_pScoreManager->resetComboPoints();						// "Resets" combopoints to 0
		m_pOutputManager->DrawBackground();							// Draws the side-menu background
		m_pOutputManager->DrawStartupMessage();						// Draws startup/menu message
		m_pOutputManager->DrawScore(g_usSCORE_POS_X,				// Draws score on screen 
									g_usSCORE_POS_Y, 0);
		m_drawCurrentLevel();										// Draws the current level (01)
		m_drawHighscore();											// Draws highscore on screen
		m_drawComboPoints();										// Draws combopoints counter on screen 
		m_pSoundManager->playMenuSong();							// Plays the menu song

		m_usCurrentLevel		   = 1;								// Sets current level to 1
		m_eCurrentGameState		   = MENU;							// Sets current game state to MENU	
		m_bIsGameRunning		   = true;							// Sets bool bIsGameRunning to true
		m_bPieceHasBeenHardDropped = false;							// Sets bool bPieceHasBeenHardDropped to false
		
		
		cout << "Initialization successful!" << endl;				// Outputs that the initialization was successful. 
		
		m_bGameLoop();												// Starts the Game loop
	}
	
	catch(exception& oException)
	{
		cout << "Initialization failed..." << endl << "Exception: " << oException.what() << endl << endl;
		system("pause");
	}

}//end init()

/*|------------------------------------------|*/
/*|   P R I V A T E    F U N C T I O N S     |*/
/*|										     |*/


bool CGameManager::m_bGameLoop()
{	
	while(m_bIsGameRunning)			
	{
		// Getting key input. Decitions based on possible key input is done
		// in keyPress checks
		m_pInputManager->GetNextKey(&m_iKeyInput);

		// Function checks what to update based on gameState
		m_checkWhatToUpdateAndUpdate();	

		// Makes the game "sleep" untill its time for update. Default is 60 updates per sec.
		m_pGeneralUpdateTimer->SleepUntilUpdate();
	}
	return true;
}//end m_bGameLoop()

/* W H A T   T O    U P D A T E    C H E C K */
void CGameManager::m_checkWhatToUpdateAndUpdate()
{
	// Switch deciding what to update based on currentGameState
	switch(m_eCurrentGameState)	
	{
	case MENU:
		m_doMenuUpdates();
		break;
	case PLAY:
		m_doGameUpdates();
		break;
	case PAUSE:	
		m_doPauseUpdates();
		break;
	case GAMEOVER:	
		m_doGameOverUpdates();
		break;
	case QUIT:		// Exits to windows
		m_bIsGameRunning =  false; 
		break;
	}
}//end _checkGameState()
/* --- Sub functions who runs the updates--- */
void CGameManager::m_doMenuUpdates()
{
		m_checkIfKeyPressAltersMenues();
		m_pOutputManager->DrawStartupMessage();
		m_pOutputManager->OutputGraphics();
}//end m_doMenuUpdates()
void CGameManager::m_doGameOverUpdates()
{
	m_checkIfKeyPressAltersMenues();
	m_pOutputManager->DrawGameOverMessage();
	m_pOutputManager->OutputGraphics();
}
void CGameManager::m_doGameUpdates()
{
	if(m_bPieceHasBeenHardDropped)
	{
		m_bPieceHasBeenHardDropped = false;
		m_pPieceUpdateTimer->SetUpdatesPerInterval(m_usPieceSpeed);
	}
	m_checkIfKeyPressAltersGame();
	m_drawBackgroundTiles();				
	m_drawNextPiece();
	m_drawCurrentPiece();
	m_pPlayAreaManager->CheckIfShouldDeleteRowAndDelete();
	if(m_pPieceUpdateTimer->IsTimeForUpdate())
	{	//If its time for new piece update, piece is moved
		m_pPieceManager->moveCurrentPieceOneTileDown();
	}
	
	m_addCurrentMakeNewIfPieceHasLanded();
	m_updateCurrentScoreIfRowsRemoved();
	m_drawCurrentScore();
	m_drawComboPoints();
	m_drawCurrentLevel();
	m_pOutputManager->OutputGraphics();		
	m_checkIfIsGameOver();

}//end m_doGameUpdates()
void CGameManager::m_doPauseUpdates()
{
	 	m_pOutputManager->DrawPauseMessage();	// Drawing the pause screen 
		m_checkIfKeyPressAltersPause();			
		m_pOutputManager->OutputGraphics();

}//end m_doPauseUpdates()

	/* K E Y P R E S S - C H E C K S */
void CGameManager::m_checkIfKeyPressAltersGame()
{
	m_checkForSingleKeyPress();
	m_checkForHoldDownKeyPress();

}//end m_checkIfKeyPressAltersGame()
void CGameManager::m_checkForSingleKeyPress()
{
	if(m_iKeyInput == SDLK_DOWN)
	{	// Moves the piece one tile down
		m_pKeyHeldDownTimer->Start();
		m_pPieceManager->moveCurrentPieceOneTileDown();
		if(m_pPieceManager->bIsCollisionBetweenCurrPieceAndLandedPieces(m_pPlayAreaManager->getBackgroundArrayReference()))
		{   // If the move caused collision, piece is moved up again
			m_pPieceManager->moveCurrentPieceOneTileUp();
		}
		else
		{	// If it was not a collision, piece timer is restarted
			m_pPieceUpdateTimer->Start(); 
		}
	}
	else if(m_iKeyInput == SDLK_LEFT)
	{	// Moves the piece one tile to the left. Collision checking done internally in pieceManager
		m_pKeyHeldDownTimer->Start();
		m_pPieceManager->moveCurrentPieceOneTileLeft();
	}
	else if(m_iKeyInput == SDLK_RIGHT)
	{	
		m_pKeyHeldDownTimer->Start();
		// Moves the piece one tile to the right. Collision checking done internally in pieceManager
		m_pPieceManager->moveCurrentPieceOneTileRight();
	}
	else if(m_iKeyInput == SDLK_UP)
	{	// Rotates the piece 90 degrees clockwise
		m_pPieceManager->rotateCurrentPiece();
	}
	else if(m_iKeyInput == SDLK_SPACE)
	{	// Drops the piece instantly down. Collision checking done in function
		m_dropCurrentPieceDown();
	}
	else if(m_iKeyInput == SDLK_ESCAPE || m_iKeyInput == SDLK_p) // Pauses the game if keyboard input is Esc or P
	{	
		m_eCurrentGameState = PAUSE;			// Setting gameState to pause
		m_pPieceUpdateTimer->pausePieceTimer(); // "snapshots" the pieceUpdate timer to be able to continue from the same exact moment
	}
}
void CGameManager::m_checkForHoldDownKeyPress()
{
	// Does additional checks for the same, but now it checks against if they key is held down or not, in 
	// which case the piece moves automatically on a timer, untill key is let loose
	if(m_pInputManager->IsDownKeyPressed() && m_pKeyHeldDownTimer->IsTimeForUpdate())
	{
		m_pPieceManager->moveCurrentPieceOneTileDown();
		if(m_pPieceManager->bIsCollisionBetweenCurrPieceAndLandedPieces(m_pPlayAreaManager->getBackgroundArrayReference()))
		{   // If the move caused collision, piece is moved up again
			m_pPieceManager->moveCurrentPieceOneTileUp();
		}
		else
		{	// If it was not a collision, piece timer is restarted
			m_pPieceUpdateTimer->Start(); 
		}
	}
	else if(m_pInputManager->IsLeftKeyPressed() && m_pKeyHeldDownTimer->IsTimeForUpdate())
	{
		m_pPieceManager->moveCurrentPieceOneTileLeft();
	}
	else if(m_pInputManager->IsRightKeyPressed() && m_pKeyHeldDownTimer->IsTimeForUpdate())
	{
		m_pPieceManager->moveCurrentPieceOneTileRight();
	}
}
void CGameManager::m_checkIfKeyPressAltersMenues()
{
	if(m_iKeyInput == SDLK_SPACE)
	{
		m_pSoundManager->playNewGameSound();
		m_pSoundManager->playGameSong();
		
		m_bPieceHasBeenHardDropped = false;			
		m_usPieceSpeed = g_usPIECE_STARTING_SPEED;			// Starting game-speed
		m_usNextLevelRequirement = g_usLINES_TO_REMOVE_FOR_NEXT_LEVEL;	// Number of rows to remove for next level
		m_usCurrentLevel = 1;								// Current level
		m_usTotalTimesRowsRemoved = 0;
		m_pPlayAreaManager->resetBackgroundToEmpty();		// Resets the background to empty (removes landed pieces)
		m_pPieceManager->makeNewNextPiece();				// Makes a new nextPiece two times
		m_pPieceManager->makeNewNextPiece();				// to make sure player has not seen the nextPiece before gameOver
		m_pOutputManager->DrawOverOldNextPiece();			// Draws over old nextPiece after making new ones
		m_pPieceManager->setLocalBackgroundArrayReference(m_pPlayAreaManager->getBackgroundArrayReference());//Storing a local referencepointer to backgroundArray in pieceManager.
		m_pScoreManager->resetCurrentScoreAndComboPoints();	// Resetting the currentScore before a new game
		m_eCurrentGameState = PLAY;							// Sets currentGameState to play. Next round of the loop game will run
		m_pPieceUpdateTimer->Start();						// Starts the piece timer
	}
	else if(m_iKeyInput == SDLK_ESCAPE)
	{
		m_eCurrentGameState = QUIT;
	}
}//end m_checkIfKeyPressAltersMenues()
void CGameManager::m_checkIfKeyPressAltersPause()
{
	if(m_iKeyInput == SDLK_y)
	{
		m_pSoundManager->playMenuSong();
		m_eCurrentGameState = MENU;
		m_pOutputManager->DrawMenuBackground();
		m_pOutputManager->DrawOverOldNextPiece();
	}
	else if(m_iKeyInput == SDLK_n)
	{
		m_pSoundManager->playPauseResumeSound();
		m_eCurrentGameState = PLAY;
		m_pPieceUpdateTimer->continuePieceTimer();
	}
}//end _checkIfKeypressAltersPause()

	/*  D R A W    F U N C T I O N S  */
void CGameManager::m_drawCurrentPiece()
{
	for(unsigned int x = 0; x < 4; ++x)
	{
		for(unsigned int y = 0; y < 4; ++y)
		{
			m_pCurrentPiecePointer = ((m_pPieceManager->getCurrentPieceReference()) + (x * g_usPIECE_ARRAY_SIZE) + y );
			m_pOutputManager->DrawTile((m_pPieceManager->getCurrentPieceXPos()+y), (m_pPieceManager->getCurrentPieceYPos()+x), *(m_pCurrentPiecePointer));
		}
	}
}//end m_drawCurrentPiece
void CGameManager::m_drawNextPiece()
{	

	for(unsigned int x = 0; x < 4; ++x)
	{
		for(unsigned int y = 0; y < 4; ++y)
		{
			m_pNextPiecePointer = ((m_pPieceManager->getNextPieceReference()) + (x * g_usPIECE_ARRAY_SIZE) + y );
			m_pOutputManager->DrawTile((g_usPREVIEWPIECE_POS_X+y), (g_usPREVIEWPIECE_POS_Y+x), *(m_pNextPiecePointer));
		}
	}
		
}//end m_drawNextPiece()
void CGameManager::m_drawBackgroundTiles()
{
	for(unsigned int x = 0; x < g_usPLAYAREA_TILEWIDTH; ++x)
	{
		for(unsigned int y = 0; y < g_usGAME_TILEHEIGHT; ++y)
		{
			m_pBackgroundPointer = (m_pPlayAreaManager->getBackgroundArrayReference() + ( x * g_usGAME_TILEHEIGHT) + y);
			m_pOutputManager->DrawTile(x, y, *(m_pBackgroundPointer));
		}
	}
}
void CGameManager::m_dropCurrentPieceDown()
{
	m_bPieceHasBeenHardDropped = true;

	// "hacking" the piece update timer to not allow the player to move the piece
	// after a hard-drop.
	m_pPieceUpdateTimer->SetUpdatesPerInterval(10000); 
	m_pPieceUpdateTimer->Start();
	m_usNumberOfCellsHardDropped = 0;	// Counter to decide on extra score gain.
	while(!m_pPieceManager->bIsCollisionBetweenCurrPieceAndLandedPieces(m_pPlayAreaManager->getBackgroundArrayReference()))
	{
		m_pPieceManager->moveCurrentPieceOneTileDown();
		++m_usNumberOfCellsHardDropped;
	}
	m_pPieceManager->moveCurrentPieceOneTileUp();
	m_pScoreManager->updateScoreAfterHardDrop(m_usNumberOfCellsHardDropped);
}
void CGameManager::m_drawComboPoints() const
{
	m_pOutputManager->DrawComboPoints(g_usCOMBO_POINTS_POS_X, g_usCOMBO_POINTS_POS_Y, m_pScoreManager->getAmountOfComboPoints());
}
void CGameManager::m_drawHighscore() const
{
	m_pOutputManager->DrawScore(g_usSCORE_POS_X, g_usHIGH_SCORE_POS_Y, m_pScoreManager->getHighscore());
}
void CGameManager::m_drawCurrentLevel() const
{
	// Using drawComboPoints function as it uses same amount of digits as we need for lvl
	m_pOutputManager->DrawComboPoints(g_usLEVEL_COUNTER_POS_X, g_usLEVEL_COUNTER_POS_Y, m_usCurrentLevel);
}
void CGameManager::m_drawCurrentScore() const
{
		m_pOutputManager->DrawScore(g_usSCORE_POS_X, g_usSCORE_POS_Y, m_pScoreManager->getCurrentScore());		
}

		/* Other functions */
void CGameManager::m_addCurrentMakeNewIfPieceHasLanded()
{
	if(m_pPieceManager->bIsCollisionBetweenCurrPieceAndLandedPieces(m_pPlayAreaManager->getBackgroundArrayReference()))
	{
		m_pPieceManager->moveCurrentPieceOneTileUp();
		m_pPlayAreaManager->addCurrentPieceToLandedPiecesArray(m_pPieceManager->getCurrentPieceReference(), 
															   m_pPieceManager->getCurrentPieceXPos(), 
															   m_pPieceManager->getCurrentPieceYPos());
		m_pOutputManager->DrawOverOldNextPiece();
		m_pPieceManager->makeNewNextPiece();
		m_pPieceUpdateTimer->Start();
	}
}
void CGameManager::m_updateCurrentScoreIfRowsRemoved() 
{
	// Resets combo points if time has run out without any new lines cleared
	if(m_pComboTimerManager->IsTimeForUpdate())
	{
		m_pScoreManager->resetComboPoints();
	}
	if(m_pPlayAreaManager->getNumberOfRowsRemoved() != 0)
	{
		m_pSoundManager->playLineRemovedSound();
		m_pScoreManager->increseAmountOfComboPoints();
		++m_usTotalTimesRowsRemoved;
		m_checkIfShouldPlaySpecialSound();
		m_checkIfIsNextLevelAndUpdate(); 
		m_pScoreManager->updateScoreAfterPieceLanded(m_pPlayAreaManager->getNumberOfRowsRemoved(), m_usCurrentLevel);
		m_pComboTimerManager->Start();
	}
}
void CGameManager::m_checkIfIsNextLevelAndUpdate()
{
	if(m_usTotalTimesRowsRemoved >= m_usNextLevelRequirement && m_usCurrentLevel != 99)
	{
		m_pSoundManager->playRandomLevelUpSound();
		m_usCurrentLevel += 1;
		m_usNextLevelRequirement = m_usNextLevelRequirement += (m_usCurrentLevel + g_usLINES_TO_REMOVE_FOR_NEXT_LEVEL);
		m_pPieceUpdateTimer->SetUpdatesPerInterval(m_usPieceSpeed +=5);
	}
}
void CGameManager::m_checkIfShouldPlaySpecialSound()
{
	if(m_pScoreManager->getAmountOfComboPoints()  == 4   ||
		m_pScoreManager->getAmountOfComboPoints() == 8   ||
		m_pScoreManager->getAmountOfComboPoints() == 12  ||
		m_pScoreManager->getAmountOfComboPoints() == 15  )
	{
		m_pSoundManager->playComboSound();
	}
}
void CGameManager::m_checkIfIsGameOver()
{
	if(m_pPlayAreaManager->bIsGameOver(m_pPieceManager->getCurrentPieceReference()) )
	{
		m_pSoundManager->playMenuSong();
		m_pSoundManager->playGameOverSound();
		m_pScoreManager->addCurrentScoreToHighscoreListIfIsBiggerThenHighscore();
		m_pScoreManager->loadHighscore();
		m_pOutputManager->DrawScore(g_usSCORE_POS_X, (g_usHIGH_SCORE_POS_Y), m_pScoreManager->getHighscore()); 
		m_eCurrentGameState = GAMEOVER;
		
	}
}