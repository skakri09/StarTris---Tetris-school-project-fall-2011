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
#ifndef SCORE_H
#define SCORE_H

#include <fstream>
#include <limits>
using namespace std;
class CScore
{
public:
	CScore();
	~CScore();
	
	// Does the score calculating. Its basically a set amount of score 
	// gained each row removed, mulitiplied by the number of combopoints+1
	// that you currently have
	void updateScoreAfterPieceLanded(unsigned short numberOfRowsRemoved, unsigned short currentLevel);
	
	// A second way to get score. Each time you hard-drop a piece, you gain
	// a little extra score based on the amount of cells you hard-dropped
	void updateScoreAfterHardDrop(unsigned short numberOfCellsDropped);
	
	// Checks if current score is bigger then the highscore
	// and if so it overwrites the existing highscore in external
	// txt file
	void addCurrentScoreToHighscoreListIfIsBiggerThenHighscore();

	// Loads the highscore from the external txt file into m_uiHighScore
	void loadHighscore();
	
	// Resets combo points and score to 0
	void resetCurrentScoreAndComboPoints();
	
	// Resets combo points alone
	void resetComboPoints();
	
	// Increses the combo points by 1
	void increseAmountOfComboPoints();

	// Returns amount of combo points
	unsigned int getAmountOfComboPoints() const;
	
	// Returns the current score
	unsigned int getCurrentScore() const;
	
	//returns the highscore
	unsigned int getHighscore() const;

private:
	unsigned int m_uiCurrentScore;	// Holds current score
	unsigned int m_uiHighScore;		// Holds highscore, based of external txt file
	unsigned int m_uiComboPoints;	// Holds combopoints

	
};

#endif