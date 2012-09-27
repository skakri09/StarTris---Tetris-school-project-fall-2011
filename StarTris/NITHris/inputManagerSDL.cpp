////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: inputManagerSDL.cpp												  //
//																			  //
//	Original author: Tomas Sandnes / NITH / 2008-2011						  //
//	Changed by:      Kristian Skarseth										  //
//			- Added functionality to check if key is held down				  //
//																			  //
//	Descr.: A class for handling keyboard input. Keypresses are returned	  //
//			with values that matches those in the SDL file <SDL_keysym.h>.	  //
//			(Which returns the ASCII value for normal keys - however: letter  //
//			keys (a-z) are ALWAYS treated as lowercase!) Other events we 	  //
//			handle (the QUIT event) are given a NEGATIVE value (see the		  //
//			const values in "inputManagerSDL.h") as they aren't represented	  //
//			in the normal <SDL_keysym.h> range.								  //
//																			  //
//	Special notes: 1) Requires the SDL library and previous SDL_Init call.	  //
//					  Calls, if necessary, SDL_InitSubSystem(SDL_INIT_VIDEO)  //
//					  on use of own Init() function. (Assumes external		  //
//					  SDL_Quit() AFTER destruction of own object.)			  //
//				   2) To identify returned key values, see the SDL file		  //
//				      <SDL_keysym.h> for key codes (enums), as well as the	  //
//					  consts in "inputManagerSDL.h".						  //
//				   2) Letter keys (a-z) are ALWAYS treated as lowercase!	  //
//																			  //
////////////////////////////////////////////////////////////////////////////////



// I N C L U D E S
// ===============

#include <iostream>  // To use cout & endl for debug output.
using std::cout;
using std::endl;
#include <SDL.h>
#include "inputManagerSDL.h"


// P U B L I C   F U N C T I O N S
// ===============================

CInputManagerSDL::CInputManagerSDL()
{
}  // END CInputManagerSDL



CInputManagerSDL::~CInputManagerSDL()
{
}  // END ~CInputManagerSDL



bool CInputManagerSDL::Init(int iKeyDelay,			 // Defaults to 250.
							int iKeyInterval) const  // Defaults to 50.
{
	

	bool r_bRetVal = true;

	// Initialize wanted SDL module for CInputManagerSDL (We somehow need video for keyRepeat to work ...)
	if (!SDL_WasInit(SDL_INIT_VIDEO))
	{
		if (0 > SDL_Init(SDL_INIT_VIDEO))
		{
			cout << "CInputManagerSDL::Init - Cannot Initialize SDL Video SubSystem: " << SDL_GetError() << endl;
			r_bRetVal = false;
		}
	}

	if (r_bRetVal)
	{
		SDL_EnableKeyRepeat(iKeyDelay, iKeyInterval);
	}

	return r_bRetVal;

}  // END Init.

// Returns true if arrow Left key is presed
bool CInputManagerSDL::IsLeftKeyPressed() const
{
	Uint8 *keystates = SDL_GetKeyState(NULL);
	if(keystates[SDLK_LEFT])
	{
		return true;
	}
	return false;
}
// Returns true if arrow right key is presed
bool CInputManagerSDL::IsRightKeyPressed() const
{
	Uint8 *keystates = SDL_GetKeyState(NULL);
	if(keystates[SDLK_RIGHT])
	{
		return true;
	}
	return false;
}
// Returns true if arrow down key is presed
bool CInputManagerSDL::IsDownKeyPressed() const
{
	Uint8 *keystates = SDL_GetKeyState(NULL);
	if(keystates[SDLK_DOWN])
	{
		return true;
	}
	return false;
}

bool CInputManagerSDL::GetNextKey(int* r_piKeyPressed) const  // Return value for key pressed, if any. 
{
	return _GetTheKey(r_piKeyPressed, false);

}  // END GetNextKey



void CInputManagerSDL::GetNextKeyWait(int* r_piKeyPressed) const  // Return value for key pressed, if any. 
{
	_GetTheKey(r_piKeyPressed, true);

}  // END GetNextKeyWait



// P R I V A T E   F U N C T I O N S
// =================================

bool CInputManagerSDL::_GetTheKey(int* r_piKeyPressed,		  // Return value for actual keypress.
								  bool bWaitIfNoneYet) const  // If true, dont return - wait for first coming keypress.
{
	bool r_bAnyKeyPressed = false;
	SDL_Event oEvent;
	*r_piKeyPressed = g_iEVENT_NONE;

	// Get the last cached keypress. 
	// NOTE: An if-statement would get the first keypress not acted upon yet, keeping the rest in queue. 
	//       As we want to flush the queue and get the last keypress not acted upon, we do a while-statement instead.
	while (SDL_PollEvent(&oEvent))
	{
		if (SDL_QUIT == oEvent.type) 
		{
			// Exit program event detected.
			r_bAnyKeyPressed = true;
			*r_piKeyPressed = g_iEVENT_QUIT;
			break;
		}
		else if (SDL_KEYDOWN == oEvent.type)
		{
			// A key has been pressed.
			r_bAnyKeyPressed = true;
			*r_piKeyPressed = oEvent.key.keysym.sym;
		}
	}

	// If no key was cached, wait for input if relevant.
	if (bWaitIfNoneYet && !r_bAnyKeyPressed)
	{
		while (!r_bAnyKeyPressed && SDL_WaitEvent(&oEvent))
		{
			if (SDL_QUIT == oEvent.type) 
			{
				// Exit program event detected.
				r_bAnyKeyPressed = true;
				*r_piKeyPressed = g_iEVENT_QUIT;
			}
			else if (SDL_KEYDOWN == oEvent.type)
			{
				// A key has been pressed.
				r_bAnyKeyPressed = true;
				*r_piKeyPressed = oEvent.key.keysym.sym;
			}
		}
	}

	return r_bAnyKeyPressed;

}  // END _GetTheKey



