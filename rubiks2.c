#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rubiks2.h"

#ifdef VISUALIZER
# define dbg stdout
#else
# define dbg stderr
#endif

//int cube[6][CUBESIZE][CUBESIZE];
int *cube;

#define CUBE(x,y,z) cube[((x)*CUBESIZE*CUBESIZE) + ((y)*CUBESIZE) + (z)]

void initcube(void)
{
    cube = malloc(6 * CUBESIZE * CUBESIZE * sizeof(int));

    int face, line, cubelet;
    for (face = 0; face < 6; face++)
        for (line = 0; line < CUBESIZE; line++)
            for (cubelet = 0; cubelet < CUBESIZE; cubelet++)
                CUBE(face,line,cubelet) = face;
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
            fprintf(dbg,"%s%d",color(CUBE(0,line,cubelet)),CUBE(0,line,cubelet));
        fprintf(dbg,"\n");
    }

    for (line = 0; line < CUBESIZE; line++) {
        for (face = 1; face < 5; face++)
            for (cubelet = 0; cubelet < CUBESIZE; cubelet++)
                fprintf(dbg,"%s%d",color(CUBE(face,line,cubelet)),CUBE(face,line,cubelet));
        fprintf(dbg,"\n");
    }

    for (line = 0; line < CUBESIZE; line++) {
        for (i = 0; i < CUBESIZE; i++)
            fprintf(dbg," ");
        for (cubelet = 0; cubelet < CUBESIZE; cubelet++)
            fprintf(dbg,"%s%d",color(CUBE(5,line,cubelet)),CUBE(5,line,cubelet));
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
                    swap(&CUBE(mod[i],d,j),&CUBE(mod[i+1],d,j));
                    //swap(&cube[mod[i]][d][j],&cube[mod[i+1]][d][j]);
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
                    swap(&CUBE(DOWN,d1,i),&CUBE(RIGHT,j,d1));
                    //swap(&cube[DOWN][d1][i],&cube[RIGHT][j][d1]);
                for (i = 0; i < CUBESIZE; i++)
                    swap(&CUBE(RIGHT,i,d1),&CUBE(UP,d2,i));
                    //swap(&cube[RIGHT][i][d1],&cube[UP][d2][i]);
                for (i = 0, j = CUBESIZE-1; i < CUBESIZE; i++, j--)
                    swap(&CUBE(UP,d2,i),&CUBE(LEFT,j,d2));
                    //swap(&cube[UP][d2][i],&cube[LEFT][j][d2]);
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
                swap(&CUBE(mod[0],j,dd),&CUBE(mod[1],i,d));
                //swap(&cube[mod[0]][j][dd],&cube[mod[1]][i][d]);
            for (i = 0; i < CUBESIZE; i++)
                swap(&CUBE(mod[1],i,d),&CUBE(mod[2],i,d));
                //swap(&cube[mod[1]][i][d],&cube[mod[2]][i][d]);
            for (i = 0; i < CUBESIZE; i++)
                swap(&CUBE(mod[2],i,d),&CUBE(mod[3],i,d));
                //swap(&cube[mod[2]][i][d],&cube[mod[3]][i][d]);
            rotateface(face,1);
        }
    }
}

// cube[face][line][cubelet]
// so cube[0][1][1] is the middle cube of the top face

void rotate_face_clockwise(size_t n, int face)
{
    printf("before rotation\n");
    printcube();
    size_t i, j;
    int *rotated = malloc(6 * n * n * sizeof(int));
    memcpy(rotated,cube,6 * n * n * sizeof(int));
    #define ROTATED(x,y,z) rotated[(x*n*n) + (y*n) + z]

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            ROTATED(face,i,j) = CUBE(face,n - j - 1,i);
            //rotated[i][j] = a[n - j - 1][i];

    //memcpy(a, rotated, sizeof a[0][0] * n * n);
    memcpy(cube, rotated, 6 * n * n * sizeof(int));

    printf("after rotation\n");
    printcube();
}

void rotateface(int face, int clockwise)
{
    int i;

    if (clockwise == 1 && face != BACK)
        rotate_face_clockwise(CUBESIZE,face);
    else if (clockwise == 1 && face == BACK)
        for (i = 0; i < 3; i++)
            rotate_face_clockwise(CUBESIZE,face);
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
                if (CUBE(face,line,cubelet) != face)
                    return 0;
    return 1;
}
