/********************************************************
*	File: Score.h
*	Orginal Author:	Kristian Skarseth / NITH / 2011
*	Changed by: -
*
*	Description: Handles score managment for a tetris game. 
*				 Can save highscore to external .txt file, 
*				 and read it again.
*
*	Special notes: 
*
/
********************************************************/

#include "Score.h"


/*|---------------------------------------|*/
/*|  P U B L I C    F U N C T I O N S     |*/
/*|									      |*/

CScore::CScore()
{
	// Just making sure score, combo points and highscore is set to 0 before
	// using them. Especially important with highscore as its compared to 
	// external highscore list on game init
	m_uiCurrentScore = 0;
	m_uiComboPoints = 0;
	m_uiHighScore = 0;
}

CScore::~CScore()
{
}

// Does the score calculating. Its basically a set amount of score 
// gained each row removed, mulitiplied by the number of combopoints+1
// that you currently have
void CScore::updateScoreAfterPieceLanded(unsigned short numberOfRowsRemoved, unsigned short currentLevel)
{
	 if(numberOfRowsRemoved != 0 && m_uiCurrentScore != 999999)
	 {
		 
		 if(numberOfRowsRemoved == 1)
		 {
			 m_uiCurrentScore = m_uiCurrentScore + ( (100*numberOfRowsRemoved) * (m_uiComboPoints+1) * (currentLevel / 2) );
		 }
		 else if(numberOfRowsRemoved == 2)
		 {
			 m_uiCurrentScore = m_uiCurrentScore + ( (300*numberOfRowsRemoved) * (m_uiComboPoints+1) * (currentLevel / 2) );
		 }
		 else if(numberOfRowsRemoved == 3)
		 {
			 m_uiCurrentScore = m_uiCurrentScore + ( (500*numberOfRowsRemoved) * (m_uiComboPoints+1) * (currentLevel / 2) );
		 }
		 else
		 {
			 m_uiCurrentScore = m_uiCurrentScore + ( (800*numberOfRowsRemoved) * (m_uiComboPoints+1) * (currentLevel / 2) );
		 }
	 }
}

// A second way to get score. Each time you hard-drop a piece, you gain
// a little extra score based on the amount of cells you hard-dropped
void CScore::updateScoreAfterHardDrop(unsigned short numberOfCellsDropped) 
{
	m_uiCurrentScore = m_uiCurrentScore + numberOfCellsDropped;
}

// Loads the highscore from the external txt file into m_uiHighScore
void CScore::loadHighscore()
{
	ifstream iFileIn("HighscoreList.txt");

	while (!iFileIn.eof())
	{
		iFileIn >> m_uiHighScore;
	}
}

// Increses the combo points by 1
void CScore::increseAmountOfComboPoints()
{
	++m_uiComboPoints;
}

// Returns the current score
unsigned int CScore::getCurrentScore()  const
{
	return m_uiCurrentScore;
}

// Returns amount of combo points
unsigned int CScore::getAmountOfComboPoints() const
{
	return m_uiComboPoints;
}

//returns the highscore
unsigned int CScore::getHighscore()  const
{
	return m_uiHighScore;
}

// Checks if current score is bigger then the highscore
// and if so it overwrites the existing highscore in external
// txt file
void CScore::addCurrentScoreToHighscoreListIfIsBiggerThenHighscore()
{
	ofstream oFileOut("HighscoreList.txt");
	if(m_uiCurrentScore > m_uiHighScore)
	{
		oFileOut << m_uiCurrentScore;
	}

}

// Resets combo points and score to 0
void CScore::resetCurrentScoreAndComboPoints()
{
	m_uiCurrentScore = 0;
	m_uiComboPoints = 0;
}

// Resets combo points alone
void CScore::resetComboPoints()
{
	m_uiComboPoints = 0;
}