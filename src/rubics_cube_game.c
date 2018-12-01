/***************************************************************************//**
**
**  @file       rubics_cube_game.c
**  @ingroup    rubicscube
**  @brief      Rubic's cube game library.
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

#include "rubics_cube_game.h"

#include <time.h>

/******************************************************************************\
**
**  LOCAL FUNCTION DEFINITIONS
**
\******************************************************************************/

/**
**  @brief Resets a game.
**
**  @param[in] game A game to reset.
*/
static void
reset_game(
        CubeGame_t *game
)
{
        cube_reset(&game->cube);
        cube_shuffle(&game->cube);
        game->turns=0;
        time(&game->time);
        game->isSolved=false;
}

/******************************************************************************\
**
**  API FUNCTION DEFINITIONS
**
\******************************************************************************/

void
cube_game_setup_graphics_interface(
        CubeGame_t *game,
        CubeGame_Graphics_Init_t funcInit,
        CubeGame_Graphics_DrawCube_t funcDrawCube,
        CubeGame_Graphics_PrintStatistics_t funcPrintStatistics,
        CubeGame_Graphics_CubeSolved_t funcCubeSolved
)
{
        game->graphics.funcInit=funcInit;
        game->graphics.functDrawCube=funcDrawCube;
        game->graphics.funcPrintStatistics=funcPrintStatistics;
        game->graphics.funcCubeSolved=funcCubeSolved;
}

void
cube_game_setup_input_interface(
        CubeGame_t *game,
        CubeGame_Input_Init_t funcInit,
        CubeGame_Input_Get_t funcGet
)
{
        game->input.funcInit=funcInit;
        game->input.funcGet=funcGet;
}

void
cube_game_init(
        CubeGame_t *game
)
{
        game->graphics.funcInit();
        game->input.funcInit();
        reset_game(game);
}

bool
cube_game_run(
        CubeGame_t *game
)
{
        CubeGameControl_t c;

        game->graphics.functDrawCube(&game->cube);
        game->graphics.funcPrintStatistics(game->turns,game->time,"");
        c=game->input.funcGet();

        switch(c){
        case GAME_CONTROL_NEW:
                reset_game(game);
                break;
        case GAME_CONTROL_EXIT:
                return false;
        }
        if(game->isSolved){
                return true;
        }
        switch(c){
        case GAME_CONTROL_MOVE_CURSOR_UP:
                cube_move_cursor(&game->cube,CUBE_DIRECTION_UP);
                break;
        case GAME_CONTROL_MOVE_CURSOR_DOWN:
                cube_move_cursor(&game->cube,CUBE_DIRECTION_DOWN);
                break;
        case GAME_CONTROL_MOVE_CURSOR_LEFT:
                cube_move_cursor(&game->cube,CUBE_DIRECTION_LEFT);
                break;
        case GAME_CONTROL_MOVE_CURSOR_RIGHT:
                cube_move_cursor(&game->cube,CUBE_DIRECTION_RIGHT);
                break;
        case GAME_CONTROL_ROTATE_CUBE_UP:
                cube_rotate(&game->cube,CUBE_DIRECTION_UP);
                break;
        case GAME_CONTROL_ROTATE_CUBE_DOWN:
                cube_rotate(&game->cube,CUBE_DIRECTION_DOWN);
                break;
        case GAME_CONTROL_ROTATE_CUBE_LEFT:
                cube_rotate(&game->cube,CUBE_DIRECTION_LEFT);
                break;
        case GAME_CONTROL_ROTATE_CUBE_RIGHT:
                cube_rotate(&game->cube,CUBE_DIRECTION_RIGHT);
                break;
        case GAME_CONTROL_ROTATE_COL_UP:
                game->turns++;
                cube_rotate_column(&game->cube,CUBE_DIRECTION_UP);
                break;
        case GAME_CONTROL_ROTATE_COL_DOWN:
                game->turns++;
                cube_rotate_column(&game->cube,CUBE_DIRECTION_DOWN);
                break;
        case GAME_CONTROL_ROTATE_ROW_LEFT:
                game->turns++;
                cube_rotate_row(&game->cube,CUBE_DIRECTION_LEFT);
                break;
        case GAME_CONTROL_ROTATE_ROW_RIGHT:
                game->turns++;
                cube_rotate_row(&game->cube,CUBE_DIRECTION_RIGHT);
                break;
        case GAME_CONTROL_ROTATE_FRONT_FACE_CCW:
                game->turns++;
                cube_rotate_front_face(&game->cube,CUBE_DIRECTION_CCW);
                break;
        case GAME_CONTROL_ROTATE_FRONT_FACE_CW:
                game->turns++;
                cube_rotate_front_face(&game->cube,CUBE_DIRECTION_CW);
                break;
        default:break;
        }
        game->isSolved=cube_is_solved(&game->cube);
        if(game->isSolved){
                game->graphics.funcCubeSolved();
        }
        return true;
}

/* EOF */