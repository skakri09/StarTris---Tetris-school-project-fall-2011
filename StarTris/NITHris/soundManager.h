/********************************************************
*	File: soundManager.h
*	Orginal Author:	Kristian Skarrseth / NITH / 2011
*	Changed by: -
*
*	Description: Handles sound output through SDL. 
*
*	Special notes: 
*				- Requires SDL.h and SDL_mixer to run.
*				- All sound and music currently being used here
*				  belongs to Blizzard Etertainment and is 
*				  not to be used for profit.
/
********************************************************/

#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <time.h>

// Including iostream and std::cout/endl 
// for exception handling
#include <iostream> 
using std::cout;
using std::endl;

class CSoundManager
{
public:
	CSoundManager();
	~CSoundManager();

	// Initializes needed SDL sub-systems and the sounds
	bool initSounds();
	
	// Plays the newGameSound on EFFECTS_CHANNEL
	void playNewGameSound() const;
	
	// Plays the lineRemoved sound on EFFECTS_CHANNEL
	// Will not play if anything from LEVELUP_CHANNEL
	// is currently playing
	void playLineRemovedSound() const;
	
	// Plays the resume from pause sound on EFFECTS_CHANNEL
	// Will not play if anything from LEVELUP_CHANNEL
	// is currently playing
	void playPauseResumeSound() const;
	
	// Plays the combo-sound sound on EFFECTS_CHANNEL
	// Will not play if anything from _LEVELUP_CHANNEL
	// is currently playing
	void playComboSound() const;
	
	// Plays gameover sound. Will halt any other sounds apart from
	// music before playing
	void playGameOverSound() const;
	
	// Plays a random of several levelup sounds. Halts
	// any currently playing sounds, apart from music, before
	// playing.
	void playRandomLevelUpSound();
	
	// Plays the Menu-song. Halts any song currently playing 
	// on music channel before playing
	void playMenuSong() const;
	
	// Plays the Game-song. Halts any song currently playing
	// on music channel before playing
	void playGameSong() const;

private:
	static const short EFFECTS_CHANNEL = 2;
	static const short LEVELUP_CHANNEL = 3;
	static const short GAME_MUSIC_VOLUME = 100;

	unsigned short m_usRandomNumber;

	Mix_Chunk* m_LineRemovedSound;
	Mix_Chunk* m_NewGameSound;
	Mix_Chunk* m_PausResumeSound;
	Mix_Chunk* m_TetrisSound;
	Mix_Chunk* m_GameOverSound;
	
	Mix_Chunk* m_LevelUpMsg1;
	Mix_Chunk* m_LevelUpMsg2;
	Mix_Chunk* m_LevelUpMsg3;
	Mix_Chunk* m_LevelUpMsg4;
	Mix_Chunk* m_LevelUpMsg5;
	Mix_Chunk* m_LevelUpMsg6;
	Mix_Chunk* m_LevelUpMsg7;

	Mix_Chunk* m_RandomLevelUpSound;

	Mix_Music* m_MenuSong;
	Mix_Music* m_GameSong;

};

#endif