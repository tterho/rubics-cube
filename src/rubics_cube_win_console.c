/***************************************************************************//**
**
**  @file       rubics_cube_win_console.c
**  @ingroup    rubicscube
**  @brief      Rubic's cube graphics and input drivers for Windows console.
**  @copyright  Copyright (C) 2018 Tuomas Terho. All rights reserved.
**
*******************************************************************************/
/*
**  BSD 3-Clause License
**
**  Copyright (c) 2018, Tuomas Terho
**  All rights reserved.
**
**  Redistribution and use in source and binary forms, with or without
**  modification, are permitted provided that the following conditions are met:
**
**  * Redistributions of source code must retain the above copyright notice,
**    this list of conditions and the following disclaimer.
**
**  * Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and/or other materials provided with the distribution.
**
**  * Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
**  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
**  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
**  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
**  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
**  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
**  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
**  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
**  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
**  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
**  POSSIBILITY OF SUCH DAMAGE.
**
\******************************************************************************/

#include "rubics_cube_win_console.h"

#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

/******************************************************************************\
**
**  LOCAL CONSTANTS
**
\******************************************************************************/

/// Face size in characters.
#define FACE_SIZE (CUBE_SIZE*BLOCK_SIZE)

/// A character for cube drawing.
#define CUBE_CHAR "#"

/// A character for row cursor.
#define ROW_CURSOR_CHAR ">"

/// A character for column cursor.
#define COL_CURSOR_CHAR "v"

/// Cursor color.
#define CURSOR_COLOR 0x0F // White on black.

/// Normal text color.
#define TEXT_COLOR 0x0F // White on black.

/// Text color for congrats.
#define SOLVED_COLOR 0x0A // Green on black.

/// Cube horizontal position.
#define CUBE_POS_X 5

/// Cube vertical position.
#define CUBE_POS_Y 5

/// Face offsets for drawing.
static const COORD faceOffset[CUBE_SIDE_COUNT]={
        {FACE_SIZE+FACE_SPACING,FACE_SIZE+FACE_SPACING},
        {FACE_SIZE+FACE_SPACING,3*(FACE_SIZE+FACE_SPACING)},
        {FACE_SIZE+FACE_SPACING,0},
        {FACE_SIZE+FACE_SPACING,2*(FACE_SIZE+FACE_SPACING)},
        {0,FACE_SIZE+FACE_SPACING},
        {2*(FACE_SIZE+FACE_SPACING),FACE_SIZE+FACE_SPACING}
};

/// Text colors.
static const uint16_t txtColor[CUBE_COLOR_COUNT]={
        0x11, // CUBE_COLOR_BLUE
        0x22, // CUBE_COLOR_GREEN
        0x44, // CUBE_COLOR_RED
        0xCC, // CUBE_COLOR_ORANGE
        0xEE, // CUBE_COLOR_YELLOW
        0xFF // 7 // CUBE_COLOR_WHITE
};

/******************************************************************************\
**
**  LOCAL FUNCTIONS
**
\******************************************************************************/

/**
**  @brief A function to position the text cursor.
**
**  @param[in] x Cursor horizontal position.
**  @param[in] y Cursor vertical position.
*/
static void
gotoxy(
        uint16_t x,
        uint16_t y
)
{
        COORD c={x,y};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

/**
**  @brief A function to set text color.
**
**  @param[in] color Color to set.
*/
static void
textcolor(
        uint16_t color
)
{
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
}

/**
**  @brief Draws one block.
**
**  @param[in] color Color of the block.
**  @param[in] x Horizontal position of the top left corner.
**  @param[in] y Vertical position of the top left corner.
*/
static void 
draw_block(
        CubeColor_t color,
        uint16_t x,
        uint16_t y
)
{
        uint8_t i;
        uint8_t j;

        textcolor(txtColor[color]);

        for(i=0;i<BLOCK_SIZE;i++){
                for(j=0;j<BLOCK_SIZE;j++){
                        gotoxy(x+i,y+j);
                        _cprintf(CUBE_CHAR);
                }
        }
}

/**
**  @brief Draws a cube face.
**
**  @param[in] face Face to draw.
**  @param[in] x Horizontal position of the top left corner.
**  @param[in] y Vertical position of the top left corner.
*/
static void 
draw_face(
        CubeFace_t *face,
        uint16_t x,
        uint16_t y
)
{
        uint8_t i;
        uint8_t j;

        for(i=0;i<CUBE_SIZE;i++){
                for(j=0;j<CUBE_SIZE;j++){
                        draw_block(
                                face->blocks[i][j],
                                x+i*BLOCK_SIZE,
                                y+j*BLOCK_SIZE
                        );
                }
        }
}

/**
**  @brief Clears the cursors from the screen.
**
**  @param[in] cube A pointer to a cube.
**  @param[in] x Overall horizontal position of the cube.
**  @param[in] y Overall vertical position of the cube.
*/
static void 
clear_cursors(
        Cube_t *cube,
        uint16_t x,
        uint16_t y
)
{
        uint8_t i;

        textcolor(CURSOR_COLOR);

        for(i=0;i<FACE_SIZE;i++){
                gotoxy(
                        faceOffset[CUBE_SIDE_FRONT].X+x+i,
                        faceOffset[CUBE_SIDE_FRONT].Y+y-1
                );
                _cprintf(" ");
                gotoxy(
                        faceOffset[CUBE_SIDE_FRONT].X+x-1,
                        faceOffset[CUBE_SIDE_FRONT].Y+y+i
                );
                _cprintf(" ");
        }
}

/**
**  @brief Draws the cursors to the screen.
**
**  @param[in] x Overall horizontal position of the cube.
**  @param[in] y Overall vertical position of the cube.
*/
static void 
draw_cursors(
        Cube_t *cube,
        uint16_t x,
        uint16_t y
)
{
        uint8_t i;

        textcolor(CURSOR_COLOR);

        for(i=0;i<BLOCK_SIZE;i++){
                gotoxy(
                        faceOffset[CUBE_SIDE_FRONT].X+x-1,
                        faceOffset[CUBE_SIDE_FRONT].Y+y+cube->row*BLOCK_SIZE+i
                );
                _cprintf(">");
                gotoxy(
                        faceOffset[CUBE_SIDE_FRONT].X+x+cube->col*BLOCK_SIZE+i,
                        faceOffset[CUBE_SIDE_FRONT].Y+y-1
                );
                _cprintf("v");
        }
}

/******************************************************************************\
**
**  GRAPHICS DRIVER FUNCTION DEFINITIONS
**
\******************************************************************************/

/**
**  @brief A function to init game graphics.
*/
static void
winconsole_graphics_init(
        void
)
{
        CONSOLE_CURSOR_INFO ci;

        ci.bVisible=false;

        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&ci);
        _cprintf("                                     ");
}

/**
**  @brief A function to draw a cube.
**
**  @param[in] cube A cube to draw.
*/
static void
winconsole_graphics_draw_cube(
        Cube_t *cube
)
{
        uint8_t i;

        for(i=0;i<CUBE_SIDE_COUNT;i++){
                draw_face(
                        &cube->face[i],
                        faceOffset[i].X+CUBE_POS_X,
                        faceOffset[i].Y+CUBE_POS_Y
                );
        }
        clear_cursors(cube,CUBE_POS_X,CUBE_POS_Y);
        draw_cursors(cube,CUBE_POS_X,CUBE_POS_Y);
}

/**
**  @brief Print game statistics.
**
**  @param[in] turns Turn count.
**  @param[in] time Time.
**  @param[in] player Player name.
*/
static void
winconsole_graphics_print_statistics(
        uint32_t turns,
        time_t time,
        int8_t *player
)
{
        textcolor(TEXT_COLOR);
        gotoxy(0,0);
        _cprintf("Turns:         ");
        gotoxy(0,0);
        _cprintf("Turns: %d\n",turns);
}

/**
**  @brief A function to draw a cube solved notification.
*/
static void
winconsole_graphics_cube_solved(
        void
)
{
        gotoxy(0,1);
        textcolor(SOLVED_COLOR);
        _cprintf("Congratulations! You solved the cube!");
}

/******************************************************************************\
**
**  INPUT DRIVER FUNCTION DEFINITIONS
**
\******************************************************************************/

/**
**  @brief Initializes an input interface.
*/
static void
winconsole_input_init(
        void
)
{
}

/**
**  @brief Gets an input control.
**
**  @return An input control value.
*/
static CubeGameControl_t
winconsole_input_get(
        void
)
{
        switch(_getch()){
        default:return GAME_CONTROL_UNKNOWN;
        case 'w':return GAME_CONTROL_MOVE_CURSOR_UP;
        case 's':return GAME_CONTROL_MOVE_CURSOR_DOWN;
        case 'a':return GAME_CONTROL_MOVE_CURSOR_LEFT;
        case 'd':return GAME_CONTROL_MOVE_CURSOR_RIGHT;
        case 'W':return GAME_CONTROL_ROTATE_CUBE_UP;
        case 'S':return GAME_CONTROL_ROTATE_CUBE_DOWN;
        case 'A':return GAME_CONTROL_ROTATE_CUBE_LEFT;
        case 'D':return GAME_CONTROL_ROTATE_CUBE_RIGHT;
        case 'i':
        case 'I':return GAME_CONTROL_ROTATE_COL_UP;
        case 'k':
        case 'K':return GAME_CONTROL_ROTATE_COL_DOWN;
        case 'j':
        case 'J':return GAME_CONTROL_ROTATE_ROW_LEFT;
        case 'l':
        case 'L':return GAME_CONTROL_ROTATE_ROW_RIGHT;
        case 'u':
        case 'U':return GAME_CONTROL_ROTATE_FRONT_FACE_CCW;
        case 'o':
        case 'O':return GAME_CONTROL_ROTATE_FRONT_FACE_CW;
        case 'n':return GAME_CONTROL_NEW;
        case 27:return GAME_CONTROL_EXIT;
        }
}

/******************************************************************************\
**
**  API FUNCTION DEFINITIONS
**
\******************************************************************************/

void
cube_game_win_console_run(
        CubeGame_t *game
)
{
        cube_game_setup_graphics_interface(
                game,
                winconsole_graphics_init,
                winconsole_graphics_draw_cube,
                winconsole_graphics_print_statistics,
                winconsole_graphics_cube_solved
        );
        cube_game_setup_input_interface(
                game,
                winconsole_input_init,
                winconsole_input_get
        );
        cube_game_init(game);
        while(cube_game_run(game));
}

/* EOF */