////////////////////////////////////////////////////////////////////////////////
//																			  //
//	File: startris.h															  //
//																			  //
//	Original author: Tomas Sandnes / NITH / 2008-2011						  //
//	Changed by:	Kristian Skarseth											  //
//				-added constants											  //
//																			  //
//	Description: A few enums and constants that are							  //
//				 used throughout the game.									  //
//																			  //
//	Special notes: ---														  //
//																			  //
////////////////////////////////////////////////////////////////////////////////

#ifndef NITHRIS_H  // Inclusion guard.
#define NITHRIS_H

// D E F I N E S   /   C O N S T A N T S   /   . . .
// =================================================

enum ETileColor { TC_NO_DRAW = -1,
				  TC_BACKGROUND,
				  TC_ORANGE,
				  TC_RED,
				  TC_GREEN,
				  TC_BLUE,
				  TC_CYAN,
				  TC_MAGENTA,
				  TC_YELLOW       
				};


		
const unsigned short g_usPLAYAREA_TILEWIDTH = 10;  // Width, in tiles, of playarea.
const unsigned short g_usSCOREAREA_TILEWIDTH = 8;  // Width, in tiles, of scorearea.
const unsigned short g_usGAME_TILEHEIGHT = 20;     // Height, in tiles, of application.
const unsigned short g_usPREVIEWPIECE_POS_X = 12;  // X-pos (measured in tiles) for upper left corner of preview piece.
const unsigned short g_usPREVIEWPIECE_POS_Y = 2;   // Y-pos (measured in tiles) for upper left corner of preview piece.
const unsigned short g_usSCORE_POS_X = 13;		   // X-pos (measured in tiles) for most significant digit of score value.
const unsigned short g_usSCORE_POS_Y = 13;		   // Y-pos (measured in tiles) for most significant digit of score value.

						/*	    Constants added by Kristian Skarseth		*/
const unsigned short g_usHIGH_SCORE_POS_Y = g_usSCORE_POS_Y + 3; // Y-pos for most significant digit of highscore value
const unsigned short g_usCOMBO_POINTS_POS_X = g_usSCORE_POS_X - 2;
const unsigned short g_usCOMBO_POINTS_POS_Y = g_usSCORE_POS_Y - 3;
const unsigned short g_usPIECE_ARRAY_SIZE = 4;
const unsigned short g_usPIECE_STARTPOS_X = (g_usPLAYAREA_TILEWIDTH / 2) - 2; // X-starting position, is window size independant.
const short			 g_sPIECE_STARTPOS_Y = -1;			// Y-starting pos for piece. Making it a variable so this can be changed to allow piece starting outside
														 // the board if wanted, without needing to change all collision-checking. Also window size independant obviously

const unsigned short g_usLEVEL_COUNTER_POS_X = g_usSCORE_POS_X - 2; // x pos for level counter
const unsigned short g_usLEVEL_COUNTER_POS_Y = 7;					// y pos for level counter
const unsigned short g_usLEVEL_BACKGROUND_POS_X = g_usLEVEL_COUNTER_POS_X + 1;
const unsigned short g_usLEVEL_BACKGROUND_POS_Y = g_usLEVEL_COUNTER_POS_Y - 1;

const unsigned short g_usLINES_TO_REMOVE_FOR_NEXT_LEVEL = 5;	// Base amount of lines to remove for level-up
const unsigned short g_usPIECE_STARTING_SPEED = 20;				// Piece starting speed
const unsigned short g_usGENERAL_TIMER_UPI = 60;

#endif  // NITHRIS_H