////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: outputManager.cpp													  //
//																			  //
//	Original author: Tomas Sandnes / NITH / 2007-2011						  //
//	Changed by:	Kristian Skarseth / NITH / 2011								  //
//			* added a few draw-functions									  //
//			* Changed score drawing function from short to int				  //
//																			  //
//	Description: Takes care of graphics output (drawing tiles to the screen,  //
//				 blitting, etc.).											  //
//																			  //
//	Special notes: Requires the SDL library and previous SDL_Init call.		  //
//				   Calls, if necessary, SDL_InitSubSystem(SDL_INIT_VIDEO)	  //
//				   on use of own Init() function. (Assumes external			  //
//				   SDL_Quit() AFTER destruction of own object.)				  //
//																			  //
////////////////////////////////////////////////////////////////////////////////


// I N C L U D E S
// ===============

#include <iostream>  // For cout, endl.
using std::cout;
using std::endl;
#include "outputManager.h"



// D E F I N E S   /   C O N S T A N T S   /   . . .
// =================================================

// (NOTE: Static globals are only visible (only have scope) in this file.)
static const unsigned short g_usTILE_PIXELWIDTH =    32;  // Tiles will be this amount of pixels large (squared).
static const unsigned short g_usWINDOW_PIXELWIDTH =  (g_usPLAYAREA_TILEWIDTH + g_usSCOREAREA_TILEWIDTH) * g_usTILE_PIXELWIDTH; // Window width in pixels.
static const unsigned short g_usWINDOW_PIXELHEIGHT = g_usGAME_TILEHEIGHT * g_usTILE_PIXELWIDTH; // Window height in pixels.
static const unsigned short g_usWINDOW_BIT_DEPTH =   16;  // Our bit depth (number of colors and such).
static const unsigned short g_usINFO_PAUSE_GAMEOVER_YPOS = (g_usGAME_TILEHEIGHT / 100) * 38;
static const unsigned int   g_uiSDL_FLAGS =          SDL_HWSURFACE | SDL_DOUBLEBUF;  // Flags for SDL: We're using graphic card memory (opposed to system memory), and double-buffering.

static const char* g_pcTILE_BMP =			 "..//Graphics//tiles_32x32.bmp";		  // Tiles as one bmp.
static const char* g_pcPAUSE_BMP =			 "..//Graphics//pause.bmp";				  // Pause background image
static const char* g_pcNUMBER_BMP =			 "..//Graphics//numbers_32x32.bmp";		  // Scoreboard numbers as one bmp.
static const char* g_pcSTARTUP_BMP =		 "..//Graphics//startup.bmp";			  // Startup (main menu) image.
static const char* g_pcGAMEOVER_BMP =		 "..//Graphics//gameover.bmp";			  // Game over image.
static const char* g_pcBACKGROUND_BMP =		 "..//Graphics//scoreboard_back.bmp";	  // Scoreboard background image.
static const char* g_pcNEXTPIECE_WRITEOVER_BMP = "..//Graphics//nextpiece_writeover.bmp"; // Solid blue to overwrite old "nextpiece"
static const char* g_pcMENU_BACKGROUND_BMP = "..//Graphics//menu_background.bmp";	  // Background for menu

// P U B L I C   F U N C T I O N S
// ===============================

COutputManager::COutputManager()
{
	// NOTE: It's important to set the surface pointers to 0 in the constructor. 
	//       Se comments in the destructor below for more information.
	m_poScreenSurface = 0;
	m_poTileSurface = 0;
	m_poNumberSurface = 0;
	m_poStartupSurface = 0;
	m_poGameOverSurface = 0;
	m_poBackgroundSurface = 0;
	m_poPauseSurface = 0;
	m_poNextPieceWriteoverSurface = 0;
	m_poMenuBackgroundSurface = 0 ;
}  // END COutputManager



COutputManager::~COutputManager()
{
	// Free all surfaces.
	// NOTE: It's important to set the surface pointers to 0 in the constructor. 
	//       If we fail during init (graphics in the wrong place, for example), 
	//       pointers with no content will then still be 0 when we enter the destructor to clean up, 
	//       and by checking for null-pointers here, we only free initialized stuff. 
	//       (Without null-pointers to start with, we would crash here if we failed during init.)
	if (m_poBackgroundSurface)
	{
		SDL_FreeSurface(m_poBackgroundSurface);
		m_poBackgroundSurface = 0;
	}
	if (m_poGameOverSurface)
	{
		SDL_FreeSurface(m_poGameOverSurface);
		m_poGameOverSurface = 0;
	}
	if (m_poStartupSurface)
	{
		SDL_FreeSurface(m_poStartupSurface);
		m_poStartupSurface = 0;
	}
	if (m_poNumberSurface)
	{
		SDL_FreeSurface(m_poNumberSurface);
		m_poNumberSurface = 0;
	}
	if (m_poTileSurface)
	{
		SDL_FreeSurface(m_poTileSurface);
		m_poTileSurface = 0;
	}
	if (m_poScreenSurface)
	{
		SDL_FreeSurface(m_poScreenSurface);
		m_poScreenSurface = 0;
	}
	if(m_poPauseSurface)
	{
		SDL_FreeSurface(m_poPauseSurface);
		m_poPauseSurface = 0;
	}
	if(m_poNextPieceWriteoverSurface)
	{
		SDL_FreeSurface(m_poNextPieceWriteoverSurface);
		m_poNextPieceWriteoverSurface = 0;
	}
	if(m_poMenuBackgroundSurface)
	{
		SDL_FreeSurface(m_poMenuBackgroundSurface);
		m_poMenuBackgroundSurface = 0;
	}

}  // END ~COutputManager



bool COutputManager::Init(const char* pcWindowTitle)
{
	bool r_bRetVal = true;

	// Initialize wanted SDL module for COutputManager (We only really need video - add other modules if you want to add sounds, etc. :-)
	if (!SDL_WasInit(SDL_INIT_VIDEO))
	{
		if (0 > SDL_Init(SDL_INIT_VIDEO))
		{
			cout << "COutputManager::Init - Cannot Initialize SDL Video SubSystem: " << SDL_GetError() << endl;
			r_bRetVal = false;
		}
	}

	if (r_bRetVal)
	{
		// Create a graphics output screen/window.
		m_poScreenSurface = SDL_SetVideoMode(g_usWINDOW_PIXELWIDTH, g_usWINDOW_PIXELHEIGHT, g_usWINDOW_BIT_DEPTH, g_uiSDL_FLAGS);
		if (!m_poScreenSurface) 
		{
			cout << "COutputManager::Init - Cannot set Video Mode:" << SDL_GetError() << endl;
			r_bRetVal = false;
		}
		else
		{
			//Set the Window Caption.
			SDL_WM_SetCaption(pcWindowTitle, pcWindowTitle);

			// Init bitmaps. (As long as we don't get any error, we move on to the next.)
			r_bRetVal = _LoadAndFormatBMP(&m_poTileSurface, g_pcTILE_BMP);
			if(r_bRetVal) 
			{
				r_bRetVal = _LoadAndFormatBMP(&m_poNumberSurface, g_pcNUMBER_BMP);
				if(r_bRetVal) 
				{
					r_bRetVal = _LoadAndFormatBMP(&m_poStartupSurface, g_pcSTARTUP_BMP);
					if(r_bRetVal) 
					{
						r_bRetVal = _LoadAndFormatBMP(&m_poGameOverSurface, g_pcGAMEOVER_BMP);
						if(r_bRetVal) 
						{
							r_bRetVal = _LoadAndFormatBMP(&m_poBackgroundSurface, g_pcBACKGROUND_BMP);
							if(r_bRetVal)
							{
								r_bRetVal = _LoadAndFormatBMP(&m_poPauseSurface, g_pcPAUSE_BMP);
								if(r_bRetVal)
								{
									r_bRetVal = _LoadAndFormatBMP(&m_poNextPieceWriteoverSurface, g_pcNEXTPIECE_WRITEOVER_BMP);
									if(r_bRetVal)
									{
										r_bRetVal = _LoadAndFormatBMP(&m_poMenuBackgroundSurface, g_pcMENU_BACKGROUND_BMP);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return r_bRetVal;

}  // END Init



void COutputManager::DrawBackground() const
{
	// Paste the scoreboard background image.
	SDL_Rect oBackRect;
	oBackRect.x = g_usPLAYAREA_TILEWIDTH * g_usTILE_PIXELWIDTH;
	oBackRect.y = 0;
	SDL_BlitSurface(m_poBackgroundSurface, 0, m_poScreenSurface, &oBackRect);

}  // END DrawBackground
void COutputManager::DrawMenuBackground() const
{
	SDL_Rect sScreenRect;
	sScreenRect.x = 0;
	sScreenRect.y = 0;
	SDL_BlitSurface(m_poMenuBackgroundSurface, 0, m_poScreenSurface, &sScreenRect);
}

void COutputManager::DrawGameOverMessage() const
{
	SDL_Rect sScreenRect;  // Part of screen we want to draw to.

	sScreenRect.x = 30; 
	sScreenRect.y = 245;

	// Blit Game over image to the screen.
	SDL_BlitSurface(m_poGameOverSurface, 0, m_poScreenSurface, &sScreenRect);

}  // END DrawGameOverMessage

void COutputManager::DrawPauseMessage() const
{
	SDL_Rect sScreenRect; // Part of screen we want to draw to.

	sScreenRect.x = 30;
	sScreenRect.y = 245;

	SDL_BlitSurface(m_poPauseSurface, 0, m_poScreenSurface, &sScreenRect);
}

void COutputManager::DrawStartupMessage() const
{
	SDL_Rect sScreenRect;  // Part of screen we want to draw to.

	sScreenRect.x = 30; 
	sScreenRect.y = 245;

	// Blit Game over image to the screen.
	SDL_BlitSurface(m_poStartupSurface, 0, m_poScreenSurface, &sScreenRect);

}  // END DrawStartupMessage

void COutputManager::DrawOverOldNextPiece() const
{
	SDL_Rect sScreenRect;

	sScreenRect.x = g_usPLAYAREA_TILEWIDTH * g_usTILE_PIXELWIDTH;
	sScreenRect.y = 0;

	SDL_BlitSurface(m_poNextPieceWriteoverSurface, 0, m_poScreenSurface, &sScreenRect);
}

void COutputManager::DrawTile(short      sPosX, 
									 short      sPosY, 
									 ETileColor eColor) const
{
	if (TC_NO_DRAW != eColor)
	{
		SDL_Rect oScreenRect;  // Part of screen we want to draw to.
		SDL_Rect oTileRect;  // Part of tileset (all tiles are stored in a single bmp, and we use a small part of it) we want to draw from.

		// Setting up where on screen we want to draw/blit.
		oScreenRect.x = sPosX * g_usTILE_PIXELWIDTH; 
		oScreenRect.y = sPosY * g_usTILE_PIXELWIDTH;

		// Setting up what part of the tile-bmp we want to use.
		oTileRect.x = eColor * g_usTILE_PIXELWIDTH;   
		oTileRect.y = 0;
		oTileRect.w = g_usTILE_PIXELWIDTH;   
		oTileRect.h = g_usTILE_PIXELWIDTH;

		// When positions are set, blit the wanted tile to the screen.
		SDL_BlitSurface(m_poTileSurface, &oTileRect, m_poScreenSurface, &oScreenRect);
	}

}  // END DrawTile



void COutputManager::DrawScore(short          sPosX, 
								short          sPosY, 
								unsigned int uiScore) const
{
	// Render the scoretext output.
	unsigned int usTempScore = uiScore;
	unsigned short usDigit = 0;
	for (unsigned short usDigitSpot = 0; usDigitSpot < 6; ++usDigitSpot)
	{
		usDigit = usTempScore % 10;
		usTempScore /= 10;
		_DrawScoreDigit(sPosX + (3 - usDigitSpot), sPosY, usDigit);
	}

}  // END DrawScore
void COutputManager::DrawComboPoints(short sPosX,
								     short sPosY,
								     unsigned short usComboPoints) const
{
	// Render the scoretext output.
	unsigned int usTempScore = usComboPoints;
	unsigned short usDigit = 0;
	for (unsigned short usDigitSpot = 0; usDigitSpot < 2; ++usDigitSpot)
	{
		usDigit = usTempScore % 10;
		usTempScore /= 10;
		_DrawScoreDigit(sPosX + (3 - usDigitSpot), sPosY, usDigit);
	}
}


void COutputManager::OutputGraphics() const
{
	//Redraw the entire Display.
	SDL_Flip(m_poScreenSurface);

}  // END OutputGraphics



// P R I V A T E   F U N C T I O N S
// =================================

bool COutputManager::_LoadAndFormatBMP(SDL_Surface** r_ppoBitmapSurface, 
											  const char*   pcBitmapName      ) const
{
	bool r_bRetVal = true;

	// First we load the bitmap into a temporary storage.
	SDL_Surface *poTempSurface = SDL_LoadBMP(pcBitmapName);
	if(!poTempSurface) 
	{
		cout << "COutputManager::Init - File not Found: " << pcBitmapName << endl;
		r_bRetVal = false;
	}
	else
	{
		// Then we convert the temporary stored bitmap to the ideal format and put it in the variable pointed to in the param list.
		*r_ppoBitmapSurface = SDL_DisplayFormat(poTempSurface);

		// Finally, we free the dynamically created temporary storage again.
		SDL_FreeSurface(poTempSurface);
	}

	return r_bRetVal;

}  // END _LoadAndFormatBMP

bool COutputManager::_LoadAndFormatPNG(SDL_Surface** r_ppoBitmapSurface, 
											  const char*   pcBitmapName      ) const
{
	bool r_bRetVal = true;

	// First we load the bitmap into a temporary storage.
	SDL_Surface *poTempSurface = SDL_LoadBMP(pcBitmapName);
	if(!poTempSurface) 
	{
		cout << "COutputManager::Init - File not Found: " << pcBitmapName << endl;
		r_bRetVal = false;
	}
	else
	{
		// Then we convert the temporary stored bitmap to the ideal format and put it in the variable pointed to in the param list.
		*r_ppoBitmapSurface = SDL_DisplayFormat(poTempSurface);

		// Finally, we free the dynamically created temporary storage again.
		SDL_FreeSurface(poTempSurface);
	}

	return r_bRetVal;

}  // END _LoadAndFormatPNG


void COutputManager::_DrawScoreDigit(short          sPosX, 
											short          sPosY, 
											unsigned short usDigit) const
{
	SDL_Rect oScreenRect;  // Part of screen we want to draw to.
	SDL_Rect oNumberRect;  // Part of the number-bmp-file (all numbers are stored in a single bmp, and we use a small part of it) we want to draw from.

	oScreenRect.x = sPosX * g_usTILE_PIXELWIDTH; 
	oScreenRect.y = sPosY * g_usTILE_PIXELWIDTH;

	oNumberRect.x = usDigit * g_usTILE_PIXELWIDTH;   
	oNumberRect.y = 0;
	oNumberRect.w = g_usTILE_PIXELWIDTH;   
	oNumberRect.h = g_usTILE_PIXELWIDTH;

	// When positions are set, blit the wanted tile to the screen.
	SDL_BlitSurface(m_poNumberSurface, &oNumberRect, m_poScreenSurface, &oScreenRect);

}  // END _DrawScoreDigit
