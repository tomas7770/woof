//
//  Copyright (C) 1999 by
//  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
// DESCRIPTION:  none
//
//-----------------------------------------------------------------------------

#ifndef __HULIB__
#define __HULIB__

// We are referring to patches.
#include "r_defs.h"
#include "v_video.h"  //jff 2/16/52 include color range defs

#define CR_ORIG (-1) // [FG] reset to original color

// background and foreground screen numbers
// different from other modules.
#define BG      1
#define FG      0

// font stuff
// #define HU_CHARERASE    KEY_BACKSPACE // not used               // phares

#define HU_MAXLINES   4
#define HU_MAXLINELENGTH  80
#define HU_REFRESHSPACING 8 /*jff 2/26/98 space lines in text refresh widget*/
//jff 2/26/98 maximum number of messages allowed in refresh list
#define HU_MAXMESSAGES 8

//
// Typedefs of widgets
//

// Text Line widget
//  (parent of Scrolling Text and Input Text widgets)
typedef struct
{
  // left-justified position of scrolling text window
  int   x;
  int   y;

  patch_t ***f;                         // font
  int   sc;                             // start character
  char *cr;                         //jff 2/16/52 output color range

  // killough 1/23/98: Support multiple lines:
  #define MAXLINES 25

  int   linelen;
  char  l[HU_MAXLINELENGTH*MAXLINES+1]; // line of text
  int   len;                            // current line length

  // whether this line needs to be udpated
  int   needsupdate;        

  int width;
  boolean visible;
  void (*builder) (void);

} hu_textline_t;

typedef enum {
  align_topleft,
  align_topleft_exclusive,
  align_topright,
  align_topcenter,
  align_bottomleft,
  align_bottomright,
  align_bottomcenter,
  align_direct,
  num_aligns,
} align_t;

typedef struct {
  hu_textline_t *line;
  align_t align;
  int x, y;
} widget_t;


// Scrolling Text window widget
//  (child of Text Line widget)
typedef struct
{
  hu_textline_t l[HU_MAXLINES]; // text lines to draw
  int     h;                    // height in lines
  int     cl;                   // current line number

  // pointer to boolean stating whether to update window
  boolean*    on;
  boolean   laston;             // last value of *->on.

} hu_stext_t;

//jff 2/26/98 new widget to display last hud_msg_lines of messages
// Message refresh window widget
typedef struct
{
  hu_textline_t l[HU_MAXMESSAGES]; // text lines to draw
  int     nl;                          // height in lines
  int     nr;                          // total height in rows
  int     cl;                          // current line number
  int     x;                           // current line x

  // pointer to boolean stating whether to update window
  boolean*    on;
  boolean   laston;             // last value of *->on.

} hu_mtext_t;



// Input Text Line widget
//  (child of Text Line widget)
typedef struct
{
  hu_textline_t l;    // text line to input on

  // left margin past which I am not to delete characters
  int     lm;

  // pointer to boolean stating whether to update window
  boolean*    on; 
  boolean   laston;   // last value of *->on;

} hu_itext_t;


//
// Widget creation, access, and update routines
//

// initializes heads-up widget library
void HUlib_init(void);

//
// textline code
//

// clear a line of text
void HUlib_clearTextLine(hu_textline_t *t);

void HUlib_initTextLine
(
  hu_textline_t *t,
  int x,
  int y,
  patch_t ***f,
  int sc,
  char *cr,    //jff 2/16/98 add color range parameter
  void (*builder)(void)
);

// returns success
boolean HUlib_addCharToTextLine(hu_textline_t *t, char ch);
void HUlib_addStringToTextLine(hu_textline_t *t, char *s);

// draws tline
void HUlib_drawTextLine(hu_textline_t *l, align_t align, boolean drawcursor);
void HUlib_resetAlignOffsets();

// erases text line
void HUlib_eraseTextLine(hu_textline_t *l); 


//
// Scrolling Text window widget routines
//

// initialize an stext widget
void HUlib_initSText
( hu_stext_t* s,
  int   x,
  int   y,
  int   h,
  patch_t ***font,
  int   startchar,
  char *cr,   //jff 2/16/98 add color range parameter
  boolean*  on );

// add a text message to an stext widget
void HUlib_addMessageToSText
( hu_stext_t* s,
  char*   prefix,
  char*   msg );

// draws stext
void HUlib_drawSText(hu_stext_t* s, align_t align);

// erases all stext lines
void HUlib_eraseSText(hu_stext_t* s);

//jff 2/26/98 message refresh widget
// initialize refresh text widget
void HUlib_initMText
( hu_mtext_t *m,
  int x,
  int y,
  patch_t ***font,
  int startchar,
  char *cr,
  boolean *on
);

//jff 2/26/98 message refresh widget
// add a text message to refresh text widget
void HUlib_addMessageToMText
( hu_mtext_t* m,
  char*   prefix,
  char*   msg );

//jff 2/26/98 message refresh widget
// draws mtext
void HUlib_drawMText(hu_mtext_t* m, align_t align);

//jff 4/28/98 erases behind message list
void HUlib_eraseMText(hu_mtext_t* m);

// Input Text Line widget routines
void HUlib_initIText
( hu_itext_t* it,
  int   x,
  int   y,
  patch_t ***font,
  int   startchar,
  char *cr,   //jff 2/16/98 add color range parameter
  boolean*  on );

// resets line and left margin
void HUlib_resetIText(hu_itext_t* it);

// whether eaten
boolean HUlib_keyInIText
( hu_itext_t* it,
  unsigned char ch );

void HUlib_drawIText(hu_itext_t* it, align_t align);

// erases all itext lines
void HUlib_eraseIText(hu_itext_t* it); 

#endif


//----------------------------------------------------------------------------
//
// $Log: hu_lib.h,v $
// Revision 1.9  1998/05/11  10:13:31  jim
// formatted/documented hu_lib
//
// Revision 1.8  1998/04/28  15:53:53  jim
// Fix message list bug in small screen mode
//
// Revision 1.7  1998/02/26  22:58:44  jim
// Added message review display to HUD
//
// Revision 1.6  1998/02/19  16:55:19  jim
// Optimized HUD and made more configurable
//
// Revision 1.5  1998/02/18  00:58:58  jim
// Addition of HUD
//
// Revision 1.4  1998/02/15  02:48:09  phares
// User-defined keys
//
// Revision 1.3  1998/01/26  19:26:52  phares
// First rev with no ^Ms
//
// Revision 1.2  1998/01/26  05:50:24  killough
// Support more lines, and tab stops, in messages
//
// Revision 1.1.1.1  1998/01/19  14:02:55  rand
// Lee's Jan 19 sources
//
//
//----------------------------------------------------------------------------

