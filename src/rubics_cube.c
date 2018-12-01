/***************************************************************************//**
**
**  @file       rubics_cube.c
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

#include "rubics_cube.h"

#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

/******************************************************************************\
**
**  LOCAL CONSTANT DEFINITIONS
**
\******************************************************************************/

/*-------------------------------------------------------------------------*//**
**  @brief Cube colors on reset, side by side.
*/
static const CubeColor_t 
resetColors[CUBE_SIDE_COUNT]={
        CUBE_COLOR_WHITE, // Front
        CUBE_COLOR_YELLOW, // Back
        CUBE_COLOR_RED, // Top
        CUBE_COLOR_ORANGE, // Bottom
        CUBE_COLOR_BLUE, // Left
        CUBE_COLOR_GREEN // Right
};

/******************************************************************************\
**
**  LOCAL FUNCTION DEFINITIONS
**
\******************************************************************************/

/*-------------------------------------------------------------------------*//**
**  @brief Rotates a face.
**
**  @param[in] face A face to rotate.
**  @param[in] dir Rotation direction (clockwise or counter-clockwise).
*/
static void
rotate_face(
        CubeFace_t *face,
        CubeDirection_t dir
)
{
        CubeFace_t f;
        uint8_t i;
        uint8_t j;

        switch(dir){
        default:return;
        case CUBE_DIRECTION_CW:
                for(i=0;i<CUBE_SIZE;i++){
                        for(j=0;j<CUBE_SIZE;j++){
                                f.blocks[i][j]=face->blocks[j][CUBE_SIZE-i-1];
                        }
                }
                break;
        case CUBE_DIRECTION_CCW:
                for(i=0;i<CUBE_SIZE;i++){
                        for(j=0;j<CUBE_SIZE;j++){
                                f.blocks[i][j]=face->blocks[CUBE_SIZE-j-1][i];
                        }
                }
                break;
        }
        memcpy(face->blocks,f.blocks,sizeof(CubeBlocks_t));
}

/*-------------------------------------------------------------------------*//**
**  @brief Rotates a row.
**
**  @param[in] cube A cube to rotate.
**  @param[in] row A row to rotate.
**  @param[in] dir Rotation direction (left or right).
*/
static void
rotate_row(
        Cube_t *cube,
        uint8_t row,
        CubeDirection_t dir
)
{
        uint8_t i;
        CubeColor_t c;

        switch(dir){
        default:return;
        case CUBE_DIRECTION_LEFT:
                if(!row){
                        rotate_face(&cube->face[CUBE_SIDE_TOP],CUBE_DIRECTION_CW);
                }
                if(row==CUBE_SIZE-1){
                        rotate_face(&cube->face[CUBE_SIDE_BOTTOM],CUBE_DIRECTION_CCW);
                }
                for(i=0;i<CUBE_SIZE;i++){
                        c=cube->face[CUBE_SIDE_LEFT].blocks[i][row];
                        cube->face[CUBE_SIDE_LEFT].blocks[i][row]=cube->face[CUBE_SIDE_FRONT].blocks[i][row];
                        cube->face[CUBE_SIDE_FRONT].blocks[i][row]=cube->face[CUBE_SIDE_RIGHT].blocks[i][row];
                        cube->face[CUBE_SIDE_RIGHT].blocks[i][row]=cube->face[CUBE_SIDE_BACK].blocks[CUBE_SIZE-1-i][CUBE_SIZE-1-row];
                        cube->face[CUBE_SIDE_BACK].blocks[CUBE_SIZE-1-i][CUBE_SIZE-1-row]=c;
                }
                break;
        case CUBE_DIRECTION_RIGHT:
                if(!row){
                        rotate_face(&cube->face[CUBE_SIDE_TOP],CUBE_DIRECTION_CCW);
                }
                if(row==CUBE_SIZE-1){
                        rotate_face(&cube->face[CUBE_SIDE_BOTTOM],CUBE_DIRECTION_CW);
                }
                for(i=0;i<CUBE_SIZE;i++){
                        c=cube->face[CUBE_SIDE_RIGHT].blocks[i][row];
                        cube->face[CUBE_SIDE_RIGHT].blocks[i][row]=cube->face[CUBE_SIDE_FRONT].blocks[i][row];
                        cube->face[CUBE_SIDE_FRONT].blocks[i][row]=cube->face[CUBE_SIDE_LEFT].blocks[i][row];
                        cube->face[CUBE_SIDE_LEFT].blocks[i][row]=cube->face[CUBE_SIDE_BACK].blocks[CUBE_SIZE-1-i][CUBE_SIZE-1-row];
                        cube->face[CUBE_SIDE_BACK].blocks[CUBE_SIZE-1-i][CUBE_SIZE-1-row]=c;
                }
                break;
        }
}

/*-------------------------------------------------------------------------*//**
**  @brief Rotates a column.
**
**  @param[in] cube A cube to rotate.
**  @param[in] col A column to rotate.
**  @param[in] dir Rotation direction (up or down).
*/
static void
rotate_column(
        Cube_t *cube,
        uint8_t col,
        CubeDirection_t dir
)
{
        uint8_t i;
        CubeColor_t c;

        switch(dir){
        default:return;
        case CUBE_DIRECTION_UP:
                if(!col){
                        rotate_face(&cube->face[CUBE_SIDE_LEFT],CUBE_DIRECTION_CCW);
                }
                if(col==CUBE_SIZE-1){
                        rotate_face(&cube->face[CUBE_SIDE_RIGHT],CUBE_DIRECTION_CW);
                }
                for(i=0;i<CUBE_SIZE;i++){
                        c=cube->face[CUBE_SIDE_TOP].blocks[col][i];
                        cube->face[CUBE_SIDE_TOP].blocks[col][i]=cube->face[CUBE_SIDE_FRONT].blocks[col][i];
                        cube->face[CUBE_SIDE_FRONT].blocks[col][i]=cube->face[CUBE_SIDE_BOTTOM].blocks[col][i];
                        cube->face[CUBE_SIDE_BOTTOM].blocks[col][i]=cube->face[CUBE_SIDE_BACK].blocks[col][i];
                        cube->face[CUBE_SIDE_BACK].blocks[col][i]=c;
                }
                break;
        case CUBE_DIRECTION_DOWN:
                if(!col){
                        rotate_face(&cube->face[CUBE_SIDE_LEFT],CUBE_DIRECTION_CW);
                }
                if(col==CUBE_SIZE-1){
                        rotate_face(&cube->face[CUBE_SIDE_RIGHT],CUBE_DIRECTION_CCW);
                }
                for(i=0;i<CUBE_SIZE;i++){
                        c=cube->face[CUBE_SIDE_BOTTOM].blocks[col][i];
                        cube->face[CUBE_SIDE_BOTTOM].blocks[col][i]=cube->face[CUBE_SIDE_FRONT].blocks[col][i];
                        cube->face[CUBE_SIDE_FRONT].blocks[col][i]=cube->face[CUBE_SIDE_TOP].blocks[col][i];
                        cube->face[CUBE_SIDE_TOP].blocks[col][i]=cube->face[CUBE_SIDE_BACK].blocks[col][i];
                        cube->face[CUBE_SIDE_BACK].blocks[col][i]=c;
                }
                break;
        }
}

/*-------------------------------------------------------------------------*//**
**  @brief Checks is a face solved.
**
**  @param[in] face A face to check.
**
**  @retval true Face is solved.
**  @retval false Face is not solved.
**
**  A face is solved when all of its blocks are of the same color.
*/
static bool
check_face(
        CubeFace_t *face
)
{
        uint8_t i;
        uint8_t j;
        CubeColor_t c;

        c=face->blocks[0][0];
        for(i=0;i<CUBE_SIZE;i++){
                for(j=0;j<CUBE_SIZE;j++){
                        if(face->blocks[i][j]!=c){
                                return false;
                        }
                }
        }
        return true;
}

/*-------------------------------------------------------------------------*//**
**  @brief Copies a face to another face.
**
**  @param[in] src Source where from to copy.
**  @param[out] dest Destination where to copy.
*/
static inline void
copy_face(
        CubeFace_t *src,
        CubeFace_t *dest
)
{
        memcpy(dest,src,sizeof(CubeFace_t));
}

/******************************************************************************\
**
**  API FUNCTION DEFINITIONS
**
\******************************************************************************/

void
cube_shuffle(
        Cube_t *cube
)
{
        uint32_t i;
        uint8_t j;
        uint8_t row;
        uint8_t col;
        CubeDirection_t dir;
        time_t t;

        srand((unsigned int)time(&t));
        for(i=0;i<1000;i++){
                row=rand()%(CUBE_SIZE-1);
                col=rand()%(CUBE_SIZE-1);
                dir=rand()%(CUBE_DIRECTION_COUNT-1);
                for(j=0;j<rand()%(CUBE_SIZE-1);j++){
                        rotate_row(cube,row,dir);
                        rotate_column(cube,col,dir);
                        cube_rotate_front_face(cube,dir);
                }
        }
}

void 
cube_reset(
        Cube_t *cube
)
{
        uint8_t i;
        uint8_t j;
        uint8_t k;

        for(i=0;i<CUBE_SIDE_COUNT;i++){
                for(j=0;j<CUBE_SIZE;j++){
                        for(k=0;k<CUBE_SIZE;k++){
                                cube->face[i].blocks[j][k]=resetColors[i];
                        }
                }
        }
        cube->row=0;
        cube->col=0;
}

void 
cube_rotate(
        Cube_t *cube,
        CubeDirection_t dir
)
{
        CubeFace_t f;

        switch(dir){
        default:return;
        case CUBE_DIRECTION_UP:
                copy_face(&cube->face[CUBE_SIDE_TOP],&f);
                copy_face(&cube->face[CUBE_SIDE_FRONT],&cube->face[CUBE_SIDE_TOP]);
                copy_face(&cube->face[CUBE_SIDE_BOTTOM],&cube->face[CUBE_SIDE_FRONT]);
                copy_face(&cube->face[CUBE_SIDE_BACK],&cube->face[CUBE_SIDE_BOTTOM]);
                copy_face(&f,&cube->face[CUBE_SIDE_BACK]);
                rotate_face(&cube->face[CUBE_SIDE_RIGHT],CUBE_DIRECTION_CW);
                rotate_face(&cube->face[CUBE_SIDE_LEFT],CUBE_DIRECTION_CCW);
                break;
        case CUBE_DIRECTION_DOWN:
                copy_face(&cube->face[CUBE_SIDE_BOTTOM],&f);
                copy_face(&cube->face[CUBE_SIDE_FRONT],&cube->face[CUBE_SIDE_BOTTOM]);
                copy_face(&cube->face[CUBE_SIDE_TOP],&cube->face[CUBE_SIDE_FRONT]);
                copy_face(&cube->face[CUBE_SIDE_BACK],&cube->face[CUBE_SIDE_TOP]);
                copy_face(&f,&cube->face[CUBE_SIDE_BACK]);
                rotate_face(&cube->face[CUBE_SIDE_RIGHT],CUBE_DIRECTION_CCW);
                rotate_face(&cube->face[CUBE_SIDE_LEFT],CUBE_DIRECTION_CW);
                break;
        case CUBE_DIRECTION_LEFT:
                copy_face(&cube->face[CUBE_SIDE_LEFT],&f);
                copy_face(&cube->face[CUBE_SIDE_FRONT],&cube->face[CUBE_SIDE_LEFT]);
                copy_face(&cube->face[CUBE_SIDE_RIGHT],&cube->face[CUBE_SIDE_FRONT]);
                rotate_face(&cube->face[CUBE_SIDE_BACK],CUBE_DIRECTION_CW);
                rotate_face(&cube->face[CUBE_SIDE_BACK],CUBE_DIRECTION_CW);
                copy_face(&cube->face[CUBE_SIDE_BACK],&cube->face[CUBE_SIDE_RIGHT]);
                copy_face(&f,&cube->face[CUBE_SIDE_BACK]);
                rotate_face(&cube->face[CUBE_SIDE_BACK],CUBE_DIRECTION_CW);
                rotate_face(&cube->face[CUBE_SIDE_BACK],CUBE_DIRECTION_CW);
                rotate_face(&cube->face[CUBE_SIDE_TOP],CUBE_DIRECTION_CW);
                rotate_face(&cube->face[CUBE_SIDE_BOTTOM],CUBE_DIRECTION_CCW);
                break;
        case CUBE_DIRECTION_RIGHT:
                copy_face(&cube->face[CUBE_SIDE_RIGHT],&f);
                copy_face(&cube->face[CUBE_SIDE_FRONT],&cube->face[CUBE_SIDE_RIGHT]);
                copy_face(&cube->face[CUBE_SIDE_LEFT],&cube->face[CUBE_SIDE_FRONT]);
                rotate_face(&cube->face[CUBE_SIDE_BACK],CUBE_DIRECTION_CW);
                rotate_face(&cube->face[CUBE_SIDE_BACK],CUBE_DIRECTION_CW);
                copy_face(&cube->face[CUBE_SIDE_BACK],&cube->face[CUBE_SIDE_LEFT]);
                copy_face(&f,&cube->face[CUBE_SIDE_BACK]);
                rotate_face(&cube->face[CUBE_SIDE_BACK],CUBE_DIRECTION_CW);
                rotate_face(&cube->face[CUBE_SIDE_BACK],CUBE_DIRECTION_CW);
                rotate_face(&cube->face[CUBE_SIDE_TOP],CUBE_DIRECTION_CCW);
                rotate_face(&cube->face[CUBE_SIDE_BOTTOM],CUBE_DIRECTION_CW);
                break;
        }
}

void 
cube_rotate_row(
        Cube_t *cube,
        CubeDirection_t dir
)
{
        rotate_row(cube,cube->row,dir);
}

void 
cube_rotate_column(
        Cube_t *cube,
        CubeDirection_t dir
)
{
        rotate_column(cube,cube->col,dir);
}

void 
cube_rotate_front_face(
        Cube_t *cube,
        CubeDirection_t dir
)
{
        uint8_t i;
        CubeColor_t c;

        switch(dir){
        default:return;
        case CUBE_DIRECTION_CCW:
                for(i=0;i<CUBE_SIZE;i++){
                        c=cube->face[CUBE_SIDE_TOP].blocks[i][CUBE_SIZE-1];
                        cube->face[CUBE_SIDE_TOP].blocks[i][CUBE_SIZE-1]=cube->face[CUBE_SIDE_RIGHT].blocks[0][i];
                        cube->face[CUBE_SIDE_RIGHT].blocks[0][i]=cube->face[CUBE_SIDE_BOTTOM].blocks[CUBE_SIZE-1-i][0];
                        cube->face[CUBE_SIDE_BOTTOM].blocks[CUBE_SIZE-1-i][0]=cube->face[CUBE_SIDE_LEFT].blocks[CUBE_SIZE-1][CUBE_SIZE-1-i];
                        cube->face[CUBE_SIDE_LEFT].blocks[CUBE_SIZE-1][CUBE_SIZE-1-i]=c;
                }
                break;
        case CUBE_DIRECTION_CW:
                for(i=0;i<CUBE_SIZE;i++){
                        c=cube->face[CUBE_SIDE_TOP].blocks[i][CUBE_SIZE-1];
                        cube->face[CUBE_SIDE_TOP].blocks[i][CUBE_SIZE-1]=cube->face[CUBE_SIDE_LEFT].blocks[CUBE_SIZE-1][CUBE_SIZE-1-i];
                        cube->face[CUBE_SIDE_LEFT].blocks[CUBE_SIZE-1][CUBE_SIZE-1-i]=cube->face[CUBE_SIDE_BOTTOM].blocks[CUBE_SIZE-1-i][0];
                        cube->face[CUBE_SIDE_BOTTOM].blocks[CUBE_SIZE-1-i][0]=cube->face[CUBE_SIDE_RIGHT].blocks[0][i];
                        cube->face[CUBE_SIDE_RIGHT].blocks[0][i]=c;
                }
                break;
        }
        rotate_face(&cube->face[CUBE_SIDE_FRONT],dir);
}

bool
cube_is_solved(
        Cube_t *cube
)
{
        uint8_t i;

        for(i=0;i<CUBE_SIDE_COUNT;i++){
                if(!check_face(&cube->face[i])){
                        return false;
                }
        }
        return true;
}

void 
cube_move_cursor(
        Cube_t *cube,
        CubeDirection_t dir
)
{
        switch(dir){
        default:return;
        case CUBE_DIRECTION_UP:
                if(!cube->row){
                        break;
                }
                cube->row--;
                break;
        case CUBE_DIRECTION_DOWN:
                if(cube->row==CUBE_SIZE-1){
                        break;
                }
                cube->row++;
                break;
        case CUBE_DIRECTION_LEFT:
                if(!cube->col){
                        break;
                }
                cube->col--;
                break;
        case CUBE_DIRECTION_RIGHT:
                if(cube->col==CUBE_SIZE-1){
                        break;
                }
                cube->col++;
                break;
        }
}

/* EOF */