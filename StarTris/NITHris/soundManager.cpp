/********************************************************
*	File: soundManager.cpp
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

#include "soundManager.h"


			
			/* Levelup sounds */
static const char* LEVEL_UP_SOUND_1 =		"..//Sounds//levelupmsg_1.wav";
static const char* LEVEL_UP_SOUND_2 =		"..//Sounds//levelupmsg_2.wav";
static const char* LEVEL_UP_SOUND_3 =		"..//Sounds//levelupmsg_3.wav";
static const char* LEVEL_UP_SOUND_4 =		"..//Sounds//levelupmsg_4.wav";
static const char* LEVEL_UP_SOUND_5 =		"..//Sounds//levelupmsg_5.wav";
static const char* LEVEL_UP_SOUND_6 =		"..//Sounds//levelupmsg_6.wav";
static const char* LEVEL_UP_SOUND_7 =		"..//Sounds//levelupmsg_7.wav";
		
		   /* Other sounds */
static const char* LINE_REMOVED =			"..//Sounds//line_removed.wav";	// Sound for a line removed
static const char* NEW_GAME		=			"..//Sounds//new_game.wav";		// Playing when starting a new game
static const char* PAUSE_RESUME =			"..//Sounds//pause_resume.wav";	// Playing when resuming after pause
static const char* COMBO_SOUND	=			"..//Sounds//combo_sound.wav";	// Playing when gaining tetris (4 lines removed)
static const char* GAME_OVER	=			"..//Sounds//game_over.wav";	// Playing on game over

		  /* Music */
static const char* MENU_SONG	=			"..//Sounds//menu_song.ogg";	// Song running while in menues
static const char* GAME_SONG	=			"..//Sounds//game_song.ogg";	// Song running while playing

/*|---------------------------------------|*/
/*|  P U B L I C    F U N C T I O N S     |*/
/*|									      |*/


CSoundManager::CSoundManager()
{
	//Seeds srand with timme, casting to int to avoid unnecessary warning
	srand(static_cast<int>(time(0)));

	// Setting all pointers to 0:
	m_LineRemovedSound = 0;
	m_NewGameSound = 0;
	m_PausResumeSound = 0;
	m_TetrisSound = 0;
	m_GameOverSound = 0;
	
	m_LevelUpMsg1 = 0;
	m_LevelUpMsg2 = 0;
	m_LevelUpMsg3 = 0;
	m_LevelUpMsg4 = 0;
	m_LevelUpMsg5 = 0;
	m_LevelUpMsg6 = 0;
	m_LevelUpMsg7 = 0;


	m_MenuSong = 0;
	m_GameSong = 0;
}

CSoundManager::~CSoundManager()
{
	// Frees all chunks of sound and music and sets
	// the pointers to 0.
	if(m_LineRemovedSound)
	{
		Mix_FreeChunk(m_LineRemovedSound);
		m_LineRemovedSound = 0;
	}
	if(m_NewGameSound)
	{
		Mix_FreeChunk(m_NewGameSound);
		m_NewGameSound = 0;
	}
	if(m_PausResumeSound)
	{
		Mix_FreeChunk(m_PausResumeSound);
		m_PausResumeSound = 0;
	}
	if(m_TetrisSound)
	{
		Mix_FreeChunk(m_TetrisSound);
		m_TetrisSound = 0;
	}
	if(m_GameOverSound)
	{
		Mix_FreeChunk(m_GameOverSound);
		m_GameOverSound = 0;
	}

	if(m_LevelUpMsg1)
	{
		Mix_FreeChunk(m_LevelUpMsg1);
		m_LevelUpMsg1 = 0;
	}
	if(m_LevelUpMsg2)
	{
		Mix_FreeChunk(m_LevelUpMsg2);
		m_LevelUpMsg2 = 0;
	}
	if(m_LevelUpMsg3)
	{
		Mix_FreeChunk(m_LevelUpMsg3);
		m_LevelUpMsg3 = 0;
	}
	if(m_LevelUpMsg4)
	{
		Mix_FreeChunk(m_LevelUpMsg4);
		m_LevelUpMsg4 = 0;
	}
	if(m_LevelUpMsg5)
	{
		Mix_FreeChunk(m_LevelUpMsg5);
		m_LevelUpMsg5 = 0;
	}
	if(m_LevelUpMsg6)
	{
		Mix_FreeChunk(m_LevelUpMsg6);
		m_LevelUpMsg6 = 0;
	}
	if(m_LevelUpMsg7)
	{
		Mix_FreeChunk(m_LevelUpMsg7);
		m_LevelUpMsg7 = 0;
	}


	if(m_MenuSong)
	{
		Mix_FreeMusic(m_MenuSong);
		m_MenuSong = 0;
	}
	if(m_GameSong)
	{
		Mix_FreeMusic(m_GameSong);
		m_GameSong = 0;
	}
	Mix_CloseAudio();
}

// Initializes needed SDL sub-systems and the sounds
bool CSoundManager::initSounds()
{
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		return false;
	}
	m_LineRemovedSound = Mix_LoadWAV(LINE_REMOVED);
	m_NewGameSound = Mix_LoadWAV(NEW_GAME);
	m_PausResumeSound = Mix_LoadWAV(PAUSE_RESUME);
	m_TetrisSound = Mix_LoadWAV(COMBO_SOUND);
	m_GameOverSound = Mix_LoadWAV(GAME_OVER);
	
	m_LevelUpMsg1 = Mix_LoadWAV(LEVEL_UP_SOUND_1);
	m_LevelUpMsg2 = Mix_LoadWAV(LEVEL_UP_SOUND_2);
	m_LevelUpMsg3 = Mix_LoadWAV(LEVEL_UP_SOUND_3);
	m_LevelUpMsg4 = Mix_LoadWAV(LEVEL_UP_SOUND_4);
	m_LevelUpMsg5 = Mix_LoadWAV(LEVEL_UP_SOUND_5);
	m_LevelUpMsg6 = Mix_LoadWAV(LEVEL_UP_SOUND_6);
	m_LevelUpMsg7 = Mix_LoadWAV(LEVEL_UP_SOUND_7);

	m_MenuSong = Mix_LoadMUS(MENU_SONG);
	m_GameSong = Mix_LoadMUS(GAME_SONG);	
	Mix_Volume(EFFECTS_CHANNEL, MIX_MAX_VOLUME);
	Mix_Volume(LEVELUP_CHANNEL, MIX_MAX_VOLUME);
	
	if(m_LineRemovedSound == 0	||
		m_NewGameSound == 0		||
		m_PausResumeSound == 0	||
		m_TetrisSound == 0		||
		m_GameOverSound == 0	||
		m_LevelUpMsg1 == 0		||
		m_LevelUpMsg2 == 0		||
		m_LevelUpMsg3 == 0		||
		m_LevelUpMsg4 == 0		||
		m_LevelUpMsg5 == 0		||
		m_LevelUpMsg6 == 0		||
		m_LevelUpMsg7 == 0		||
		m_MenuSong == 0			||
		m_GameSong == 0			)
	{
		cout << "CSoundManager::initSounds - Missing sound(s): " << SDL_GetError() << endl;
		return false;
	}
	return true;
}

// Plays the newGameSound on EFFECTS_CHANNEL
void CSoundManager::playNewGameSound() const
{
	Mix_HaltChannel(EFFECTS_CHANNEL);	// Halts any sound on EFFECTS_CHANNEL before playing following sound
	Mix_PlayChannel(EFFECTS_CHANNEL, m_NewGameSound, 0);
}

// Plays the lineRemoved sound on EFFECTS_CHANNEL
// Will not play if anything from LEVELUP_CHANNEL
// is currently playing
void CSoundManager::playLineRemovedSound() const
{
	if(!Mix_Playing(LEVELUP_CHANNEL)) // Makes sure no levelup sound is playing before playing lineRemovedSound
	{
		Mix_PlayChannel(3, m_LineRemovedSound, 0);
	}
}
// Plays the resume from pause sound on EFFECTS_CHANNEL
// Will not play if anything from LEVELUP_CHANNEL
// is currently playing
void CSoundManager::playPauseResumeSound() const
{
	if(!Mix_Playing(LEVELUP_CHANNEL))// Makes sure no levelup sound is playing before playing pauseResumeSound
	{
		Mix_HaltChannel(EFFECTS_CHANNEL); // Halts any sound on EFFECTS_CHANNEL before playing following sound
		Mix_PlayChannel(EFFECTS_CHANNEL, m_PausResumeSound, 0);
	}
}

// Plays the combo-sound sound on EFFECTS_CHANNEL
// Will not play if anything from _LEVELUP_CHANNEL
// is currently playing
void CSoundManager::playComboSound() const
{
	if(!Mix_Playing(LEVELUP_CHANNEL))// Makes sure no levelup sound is playing before playing tetrisSound
	{
		Mix_HaltChannel(EFFECTS_CHANNEL); // Halts any sound on EFFECTS_CHANNEL2 before playing following sound
		Mix_PlayChannel(EFFECTS_CHANNEL, m_TetrisSound, 0);
	}
}

// Plays gameover sound. Will halt any other sounds apart from
// music before playing
void CSoundManager::playGameOverSound() const
{
	Mix_HaltChannel(LEVELUP_CHANNEL);	// Halts any sound on LEVELUP_CHANNEL before playing following sound
	Mix_HaltChannel(EFFECTS_CHANNEL);	// Halts any sound on EFFECTS_CHANNEL before playing following sound
	Mix_PlayChannel(EFFECTS_CHANNEL, m_GameOverSound, 0);
}

// Plays a random of several levelup sounds. Halts
// any currently playing sounds, apart from music, before
// playing.
void CSoundManager::playRandomLevelUpSound()
{
	m_usRandomNumber = (rand() % 6+1);
	switch(m_usRandomNumber)
	{
	case 1:
		m_RandomLevelUpSound = m_LevelUpMsg1;
		break;
	case 2:
		m_RandomLevelUpSound = m_LevelUpMsg2;
		break;
	case 3:
		m_RandomLevelUpSound = m_LevelUpMsg3;
		break;
	case 4:
		m_RandomLevelUpSound = m_LevelUpMsg4;
		break;
	case 5:
		m_RandomLevelUpSound = m_LevelUpMsg5;
		break;
	case 6:
		m_RandomLevelUpSound = m_LevelUpMsg6;
		break;
	case 7:
		m_RandomLevelUpSound = m_LevelUpMsg7;
		break;
	}
	//Halts any currently playing sounds (not music)
	Mix_HaltChannel(EFFECTS_CHANNEL);
	Mix_HaltChannel(LEVELUP_CHANNEL);

	Mix_PlayChannel(LEVELUP_CHANNEL, m_RandomLevelUpSound, 0);
}

// Plays the Menu-song. Halts any song currently playing 
// on music channel before playing
void CSoundManager::playMenuSong() const
{
	Mix_HaltMusic();// Halts any currently playing music before staring new song
	Mix_PlayMusic(m_MenuSong, -1);

}

// Plays the Game-song. Halts any song currently playing
// on music channel before playing
void CSoundManager::playGameSong() const
{
	Mix_HaltMusic();// Halts any currently playing music before staring new song
	Mix_PlayMusic(m_GameSong, -1);
	Mix_VolumeMusic(GAME_MUSIC_VOLUME);
}