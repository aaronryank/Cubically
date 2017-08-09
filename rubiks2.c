#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rubiks.h"

#ifdef VISUALIZER
# define dbg stdout
#else
# define dbg stderr
#endif

int cube[6][CUBESIZE][CUBESIZE];

void initcube(void)
{
    cube = malloc(6 * CUBESIZE * CUBESIZE * sizeof(int));

    int face, line, cubelet;
    for (face = 0; face < 6; face++)
        for (line = 0; line < CUBESIZE; line++)
            for (cubelet = 0; cubelet < CUBESIZE; cubelet++)
                cube[face][line][cubelet] = face;
}

char *color(int x)
{
#ifndef TIO
    switch (x) {
      case 0: return "\e[0;31m";
      case 1: return "\e[0;34m";
      case 2: return "\e[0;35m";
      case 3: return "\e[0;32m";
      case 4: return "\e[0;36m";
      case 5: return "\e[0;33m";
    }
#else
    return "";
#endif
}

void printcube(void)
{
    int face, line, cubelet, i;
    for (line = 0; line < CUBESIZE; line++) {
        for (i = 0; i < CUBESIZE; i++)
            fprintf(dbg," ");
        for (cubelet = 0; cubelet < CUBESIZE; cubelet++)
            fprintf(dbg,"%s%d",color(cube[0][line][cubelet]),cube[0][line][cubelet]);
        fprintf(dbg,"\n");
    }

    for (line = 0; line < CUBESIZE; line++) {
        for (face = 1; face < 5; face++)
            for (cubelet = 0; cubelet < CUBESIZE; cubelet++)
                fprintf(dbg,"%s%d",color(cube[face][line][cubelet]),cube[face][line][cubelet]);
        fprintf(dbg,"\n");
    }

    for (line = 0; line < CUBESIZE; line++) {
        for (i = 0; i < CUBESIZE; i++)
            fprintf(dbg," ");
        for (cubelet = 0; cubelet < CUBESIZE; cubelet++)
            fprintf(dbg,"%s%d",color(cube[5][line][cubelet]),cube[5][line][cubelet]);
        fprintf(dbg,"\n");
    }
    fprintf(dbg,"\n");
    fflush(dbg);
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void turncube(int face, int turns)
{
    int mod[4], i, j;
    if (face == UP || face == DOWN) {
        mod[0] = (face == UP) ? LEFT : RIGHT;
        mod[1] = FRONT;
        mod[2] = (face == UP) ? RIGHT : LEFT;
        mod[3] = BACK;

        int d = (face == UP ? 0 : CUBESIZE-1);
        while (turns--) {
            for (i = 0; i < 3; i++)
                for (j = 0; j < CUBESIZE; j++)
                    swap(&cube[mod[i]][d][j],&cube[mod[i+1]][d][j]);
            rotateface(face,1);
        }
    }
    else if (face == FRONT || face == BACK) {
        int d1 = (face == FRONT) ? 0 : CUBESIZE-1;
        int d2 = d1 ? 0 : CUBESIZE-1;
        while (turns--) {
            int tmp = 1;
            if (face == BACK)
                tmp = 3;
            while (tmp--) {
                for (i = 0, j = CUBESIZE-1; i < CUBESIZE; i++, j--)
                    swap(&cube[DOWN][d1][i],&cube[RIGHT][j][d1]);
                for (i = 0; i < CUBESIZE; i++)
                    swap(&cube[RIGHT][i][d1],&cube[UP][d2][i]);
                for (i = 0, j = CUBESIZE-1; i < CUBESIZE; i++, j--)
                    swap(&cube[UP][d2][i],&cube[LEFT][j][d2]);
                rotateface(face,1);
            }
        }
    }
    else if (face == RIGHT || face == LEFT) {
        mod[0] = BACK;
        mod[1] = (face == RIGHT) ? UP : DOWN;
        mod[2] = FRONT;
        mod[3] = (face == RIGHT) ? DOWN : UP;

        int d = (face == RIGHT) ? CUBESIZE-1 : 0;
        int dd = d ? 0 : CUBESIZE-1;

        while (turns--) {
            for (i = 0, j = CUBESIZE-1; i < CUBESIZE; i++, j--)
                swap(&cube[mod[0]][j][dd],&cube[mod[1]][i][d]);
            for (i = 0; i < CUBESIZE; i++)
                swap(&cube[mod[1]][i][d],&cube[mod[2]][i][d]);
            for (i = 0; i < CUBESIZE; i++)
                swap(&cube[mod[2]][i][d],&cube[mod[3]][i][d]);
            rotateface(face,1);
        }
    }
}

// cube[face][line][cubelet]
// so cube[0][1][1] is the middle cube of the top face

void rotate_array_clockwise(size_t n, int a[n][n])
{
    size_t i, j;
    int rotated[n][n];

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            rotated[i][j] = a[n - j - 1][i];

    memcpy(a, rotated, sizeof a[0][0] * n * n);
}

void rotateface(int face, int clockwise)
{
    int i;

    if (clockwise == 1 && face != BACK)
        rotate_array_clockwise(CUBESIZE,cube[face]);
    else if (clockwise == 1 && face == BACK)
        for (i = 0; i < 3; i++)
            rotate_array_clockwise(CUBESIZE,cube[face]);
}

/*
void rotateface(int face, int clockwise)
{
    if (clockwise == 1 && face != BACK) {
        swap(&cube[face][0][0], &cube[face][2][0]);
        swap(&cube[face][0][1], &cube[face][1][0]);
        swap(&cube[face][0][2], &cube[face][2][0]);
        swap(&cube[face][1][0], &cube[face][2][1]);
        swap(&cube[face][1][2], &cube[face][2][1]);
        swap(&cube[face][2][2], &cube[face][2][0]);
    }
    else if (clockwise == 1 && face == BACK) {
        swap(&cube[face][0][0], &cube[face][0][2]);
        swap(&cube[face][1][0], &cube[face][0][1]);
        swap(&cube[face][2][0], &cube[face][0][2]);
        swap(&cube[face][0][1], &cube[face][1][2]);
        swap(&cube[face][2][1], &cube[face][1][2]);
        swap(&cube[face][2][2], &cube[face][0][2]);
    }
}
*/

int issolved(void)
{
    int face, line, cubelet;
    for (face = 0; face < 6; face++)
        for (line = 0; line < 3; line++)
            for (cubelet = 0; cubelet < 3; cubelet++)
                if (cube[face][line][cubelet] != face)
                    return 0;
    return 1;
}
