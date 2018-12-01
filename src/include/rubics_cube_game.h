/***************************************************************************//**
**
**  @file       rubics_cube_game.h
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

#ifndef rubics_cube_game_H
#define rubics_cube_game_H

#include "rubics_cube.h"

/******************************************************************************\
**
**  TYPE DEFINITIONS
**
\******************************************************************************/

/**
**  @brief Game controls.
*/
typedef enum
CubeGameControl_t{
        /// Start a new game.
        GAME_CONTROL_NEW=0,
        /// Exit the game.
        GAME_CONTROL_EXIT,
        /// Move the cursor up.
        GAME_CONTROL_MOVE_CURSOR_UP,
        /// Move the cursor down.
        GAME_CONTROL_MOVE_CURSOR_DOWN,
        /// Move the cursor left.
        GAME_CONTROL_MOVE_CURSOR_LEFT,
        /// Move the cursor right.
        GAME_CONTROL_MOVE_CURSOR_RIGHT,
        /// Rotate a row left.
        GAME_CONTROL_ROTATE_ROW_LEFT,
        /// Rotate a row right.
        GAME_CONTROL_ROTATE_ROW_RIGHT,
        /// Rotate a column up.
        GAME_CONTROL_ROTATE_COL_UP,
        /// Rotate a column down.
        GAME_CONTROL_ROTATE_COL_DOWN,
        /// Rotate the cube up.
        GAME_CONTROL_ROTATE_CUBE_UP,
        /// Rotate the cube down.
        GAME_CONTROL_ROTATE_CUBE_DOWN,
        /// Rotate the cube left.
        GAME_CONTROL_ROTATE_CUBE_LEFT,
        /// Rotate the cube right.
        GAME_CONTROL_ROTATE_CUBE_RIGHT,
        /// Rotate the front face clockwise.
        GAME_CONTROL_ROTATE_FRONT_FACE_CW,
        /// Rotate the front face counter-clockwise.
        GAME_CONTROL_ROTATE_FRONT_FACE_CCW,
        /// An unknown control.
        GAME_CONTROL_UNKNOWN
} CubeGameControl_t;

/******************************************************************************\
**
**  GAME GRAPHICS INTERFACE DEFINITION
**
\******************************************************************************/

/**
**  @brief A function to init game graphics.
*/
typedef void
(*CubeGame_Graphics_Init_t)(
        void
);

/**
**  @brief A function to draw a cube.
**
**  @param[in] cube A cube to draw.
*/
typedef void
(*CubeGame_Graphics_DrawCube_t)(
        Cube_t *cube
);

/**
**  @brief Print game statistics.
**
**  @param[in] turns Turn count.
**  @param[in] time Time.
**  @param[in] player Player name.
*/
typedef void
(*CubeGame_Graphics_PrintStatistics_t)(
        uint32_t turns,
        time_t time,
        int8_t *player
);

/**
**  @brief A function to draw a cube solved notification.
*/
typedef void
(*CubeGame_Graphics_CubeSolved_t)(
        void
);

/**
**  @brief Game graphics interface.
*/
typedef struct
CubeGameGraphics_t{
        /// Initializes the graphics driver.
        CubeGame_Graphics_Init_t funcInit;
        /// Draws a cube.
        CubeGame_Graphics_DrawCube_t functDrawCube;
        /// Prints game statistics.
        CubeGame_Graphics_PrintStatistics_t funcPrintStatistics;
        /// Prints cube solved notification.
        CubeGame_Graphics_CubeSolved_t funcCubeSolved;
} CubeGameGraphics_t;

/******************************************************************************\
**
**  GAME INPUT INTERFACE DEFINITION
**
\******************************************************************************/

/**
**  @brief Initializes an input interface.
*/
typedef void
(*CubeGame_Input_Init_t)(
        void
);

/**
**  @brief Gets an input control.
**
**  @return An input control value.
*/
typedef CubeGameControl_t
(*CubeGame_Input_Get_t)(
        void
);

/**
**  @brief Game input interface.
*/
typedef struct
CubeGameInput_t{
        /// Initializes the input driver.
        CubeGame_Input_Init_t funcInit;
        /// Gets input control.
        CubeGame_Input_Get_t funcGet;
} CubeGameInput_t;

/******************************************************************************\
**
**  GAME TYPE DEFINITION
**
\******************************************************************************/

/**
**  @brief Game instance data.
*/
typedef struct
CubeGame_t{
        /// A pointer to a graphics driver.
        CubeGameGraphics_t graphics;
        /// A pointer to an input driver.
        CubeGameInput_t input;
        /// Turn counter.
        uint32_t turns;
        /// Time counter.
        time_t time;
        /// Rubic's cube.
        Cube_t cube;
        /// Solved state.
        bool isSolved;
} CubeGame_t;

/******************************************************************************\
**
**  API FUNCTION DECLARATIONS
**
\******************************************************************************/

/*-------------------------------------------------------------------------*//**
**  @brief Sets up a graphics interface.
**
**  @param[in] game A pointer to a game instance.
**  @param[in] funcInit A graphics driver function pointer.
**  @param[in] funcDrawCube A graphics driver function pointer.
**  @param[in] funcPrintStatistics A graphics driver function pointer.
**  @param[in] funcCubeSolved A graphics driver function pointer.
*/
void
cube_game_setup_graphics_interface(
        CubeGame_t *game,
        CubeGame_Graphics_Init_t funcInit,
        CubeGame_Graphics_DrawCube_t funcDrawCube,
        CubeGame_Graphics_PrintStatistics_t funcPrintStatistics,
        CubeGame_Graphics_CubeSolved_t funcCubeSolved
);

/*-------------------------------------------------------------------------*//**
**  @brief Sets up an input interface.
**
**  @param[in] game A pointer to a game instance.
**  @param[in] funcInit An input driver function pointer.
**  @param[in] funcGet An input driver function pointer.
*/
void
cube_game_setup_input_interface(
        CubeGame_t *game,
        CubeGame_Input_Init_t funcInit,
        CubeGame_Input_Get_t funcGet
);

/*-------------------------------------------------------------------------*//**
**  @brief Initializes a game.
**
**  @param[in] game A pointer to a game instance.
*/
void
cube_game_init(
        CubeGame_t *game
);

/*-------------------------------------------------------------------------*//**
**  @brief Runs a game.
**
**  @param[in] game A pointer to a game instance.
**
**  @retval true Continue game.
**  @retval false Exit game.
*/
bool
cube_game_run(
        CubeGame_t *game
);

#endif // ifndef rubics_cube_game_H

/* EOF */