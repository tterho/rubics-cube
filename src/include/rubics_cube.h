/***************************************************************************//**
**
**  @defgroup  rubicscube Rubics cube.
**
**  A Rubic's cube library.
**
**  @file       rubics_cube.h
**  @ingroup    rubicscube
**  @brief      Rubic's cube library.
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

#ifndef rubics_cube_H
#define rubics_cube_H

#include <inttypes.h>
#include <stdbool.h>

/******************************************************************************\
**
**  CONSTANTS
**
\******************************************************************************/

/// Cube size in blocks per side. The minimum size is 2 blocks (2x2x2 cube) and 
///the maximum size is 255 blocks. The original size is 3 blocks (3x3x3 cube).
#define CUBE_SIZE 3

/******************************************************************************\
**
**  TYPE DEFINES
**
\******************************************************************************/

/**
**  @brief Colors for cube blocks.
**
**  A Rubic's cube comprises six faces with six different colors. These colors 
**  are typically blue, green, red, orange, yellow and white.
*/
typedef enum 
CubeColor_t{
        /// Blue.
        CUBE_COLOR_BLUE=0,
        /// Green.
        CUBE_COLOR_GREEN,
        /// Red.
        CUBE_COLOR_RED,
        /// Orange.
        CUBE_COLOR_ORANGE,
        /// Yellow.
        CUBE_COLOR_YELLOW,
        /// White.
        CUBE_COLOR_WHITE,
        /// Color count.
        CUBE_COLOR_COUNT
} CubeColor_t;

/**
**  @brief Sides of the positioned cube.
**
**  These sides are used to access the different faces of the cube. The 
**  orientation of the cube is always the front side faced to the player.
*/
typedef enum
CubeSide_t{
        /// Front side.
        CUBE_SIDE_FRONT=0,
        /// Left side.
        CUBE_SIDE_BACK,
        /// Top side.
        CUBE_SIDE_TOP,
        /// Bottom side.
        CUBE_SIDE_BOTTOM,
        /// Left side.
        CUBE_SIDE_LEFT,
        /// Right side.
        CUBE_SIDE_RIGHT,
        /// Side count.
        CUBE_SIDE_COUNT
} CubeSide_t;

/**
**  @brief Rotating directions of row, column, or entire cube.
*/
typedef enum
CubeDirection_t{
        /// Rotate cube or row left.
        CUBE_DIRECTION_LEFT,
        /// Rotate cube or row right.
        CUBE_DIRECTION_RIGHT,
        /// Rotate cube or column up.
        CUBE_DIRECTION_UP,
        /// Rotate cube or column down.
        CUBE_DIRECTION_DOWN,
        /// Rotate the front face clockwise.
        CUBE_DIRECTION_CW,
        /// Rotate the front face counter-clockwise.
        CUBE_DIRECTION_CCW,
        /// Direction count.
        CUBE_DIRECTION_COUNT
} CubeDirection_t;

/**
**  @brief Colors of the face blocks.
**
**  One face is a two-dimensional array of blocks, each having its individual 
**  color.
*/
typedef CubeColor_t 
CubeBlocks_t[CUBE_SIZE][CUBE_SIZE];

/**
**  @brief Cube face.
*/
typedef struct
CubeFace_t{
        /// Face blocks.
        CubeBlocks_t blocks;
}CubeFace_t;

/**
**  @brief The Rubic's cube main structure.
*/
typedef struct
Cube_t{
        /// Cube faces.
        CubeFace_t face[CUBE_SIDE_COUNT];
        /// Current row.
        uint8_t row;
        /// Current column.
        uint8_t col;
} Cube_t;

/******************************************************************************\
**
**  API FUNCTION DECLARATIONS
**
\******************************************************************************/

/*-------------------------------------------------------------------------*//**
**  @brief Resets a cube.
**
**  @param[in] cube A pointer to a Rubic's cube.
*/
void 
cube_reset(
        Cube_t *cube
);

/*-------------------------------------------------------------------------*//**
**  @brief Shuffles a cube.
**
**  @param[in] cube A pointer to a cube.
*/
void 
cube_shuffle(
        Cube_t *cube
);

/*-------------------------------------------------------------------------*//**
**  @brief Rotates the whole cube.
**
**  @param[in] cube A pointer to a cube.
**  @param[in] dir Direction to rotate (up, down, left or right).
*/
void
cube_rotate(
        Cube_t *cube,
        CubeDirection_t dir
);

/*-------------------------------------------------------------------------*//**
**  @brief Rotates a row.
**
**  @param[in] cube A pointer to a cube.
**  @param[in] dir Direction to rotate (left or right).
*/
void 
cube_rotate_row(
        Cube_t *cube,
        CubeDirection_t dir
);

/*-------------------------------------------------------------------------*//**
**  @brief Rotates a column.
**
**  @param[in] cube A pointer to a cube.
**  @param[in] dir Direction to rotate (up or down).
*/
void 
cube_rotate_column(
        Cube_t *cube,
        CubeDirection_t dir
);

/*-------------------------------------------------------------------------*//**
**  @brief Rotates the front face.
**
**  @param[in] cube A pointer to a cube.
**  @param[in] dir Direction to rotate (clockwise or counter-clockwise).
*/
void 
cube_rotate_front_face(
        Cube_t *cube,
        CubeDirection_t dir
);

/*-------------------------------------------------------------------------*//**
**  @brief Checks if the cube is solved.
**
**  @param[in] cube A pointer to a cube.
**
**  @retval true Cube is solved.
**  @retval false Cube is not solved.
*/
bool 
cube_is_solved(
        Cube_t *cube
);

/*-------------------------------------------------------------------------*//**
**  @brief Moves the row and column cursors.
**
**  @param[in] cube A pointer to a cube.
**  @param[in] dir Direction to move the cursors.
*/
void 
cube_move_cursor(
        Cube_t *cube,
        CubeDirection_t dir
);

#endif // ifndef rubics_cube_H

/* EOF */