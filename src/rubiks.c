#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rubiks.h"

#ifdef _WIN32
# include <io.h>
# define is_a_tty _isatty
#else
# include <unistd.h>
# define is_a_tty isatty
#endif

#ifdef VISUALIZER
# define dbg stdout
#else
# define dbg stderr
#endif

int *cube;
int CUBESIZE;

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
    if (is_a_tty(fileno(stderr))) {
        switch (x) {
          case 0: return "\e[0;31m";
          case 1: return "\e[0;34m";
          case 2: return "\e[0;35m";
          case 3: return "\e[0;32m";
          case 4: return "\e[0;36m";
          case 5: return "\e[0;33m";
        }
    } else {
        return "";
    }
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

#define depth_offset(x) x ? (x -= depth) : (x += depth)

void turncube(int face, int turns, int depth)
{
    if (depth >= CUBESIZE || depth < 0)
        return;
    if (turns <= 0)
        return;

    turns %= 4;

    int mod[4], i, j;
    if (face == UP || face == DOWN) {
        mod[0] = (face == UP) ? LEFT : RIGHT;
        mod[1] = FRONT;
        mod[2] = (face == UP) ? RIGHT : LEFT;
        mod[3] = BACK;

        int d = (face == UP ? 0 : CUBESIZE-1);

        depth_offset(d);

        while (turns--) {
            for (i = 0; i < 3; i++)
                for (j = 0; j < CUBESIZE; j++)
                    swap(&CUBE(mod[i],d,j),&CUBE(mod[i+1],d,j));

            if (depth == 0)
                rotateface(face,1);
            else if (depth == CUBESIZE-1)
                rotateface(face == UP ? DOWN : UP,3);
        }
    }
    else if (face == FRONT || face == BACK) {
        int d1 = (face == FRONT) ? 0 : CUBESIZE-1;
        int d2 = (face == FRONT) ? CUBESIZE-1 : 0;

        depth_offset(d1);
        depth_offset(d2);

        while (turns--) {
            int tmp = 1;
            if (face == BACK)
                tmp = 3;
            while (tmp--) {
                for (i = 0, j = CUBESIZE-1; i < CUBESIZE; i++, j--)
                    swap(&CUBE(DOWN,d1,i),&CUBE(RIGHT,j,d1));
                for (i = 0; i < CUBESIZE; i++)
                    swap(&CUBE(RIGHT,i,d1),&CUBE(UP,d2,i));
                for (i = 0, j = CUBESIZE-1; i < CUBESIZE; i++, j--)
                    swap(&CUBE(UP,d2,i),&CUBE(LEFT,j,d2));

                if (depth == 0)
                    rotateface(face,1);
                else if (depth == CUBESIZE-1)
                    rotateface(face == FRONT ? BACK : FRONT,3);
            }
        }
    }
    else if (face == RIGHT || face == LEFT) {
        mod[0] = BACK;
        mod[1] = (face == RIGHT) ? UP : DOWN;
        mod[2] = FRONT;
        mod[3] = (face == RIGHT) ? DOWN : UP;

        int d  = (face == RIGHT) ? CUBESIZE-1 : 0;
        int dd = (face == RIGHT) ? 0 : CUBESIZE-1;

        depth_offset(d);
        depth_offset(dd);

        while (turns--) {
            for (i = 0, j = CUBESIZE-1; i < CUBESIZE; i++, j--)
                swap(&CUBE(mod[0],j,dd),&CUBE(mod[1],i,d));
            for (i = 0; i < CUBESIZE; i++)
                swap(&CUBE(mod[1],i,d),&CUBE(mod[2],i,d));
            for (i = 0; i < CUBESIZE; i++)
                swap(&CUBE(mod[2],i,d),&CUBE(mod[3],i,d));

            if (depth == 0)
                rotateface(face,1);
            else if (depth == CUBESIZE-1)
                rotateface(face == RIGHT ? LEFT : RIGHT,3);
        }
    }
}

int asize, *rotated;

void rotate_face_clockwise(size_t n, int face)
{
    size_t i, j;
    memcpy(rotated,cube,asize);
    #define ROTATED(x,y,z) rotated[(x*n*n) + (y*n) + z]

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            ROTATED(face,i,j) = CUBE(face,n - j - 1,i);

    memcpy(cube,rotated,asize);
}

void rotateface(int face, int clockwise)
{
    int i;

    while (clockwise--)
    {
        if (face != BACK)
            rotate_face_clockwise(CUBESIZE,face);
        else if (face == BACK)
            for (i = 0; i < 3; i++)
                rotate_face_clockwise(CUBESIZE,face);
    }
}

int issolved(void)
{
    int face, line, cubelet;
    for (face = 0; face < 6; face++)
        for (line = 0; line < CUBESIZE; line++)
            for (cubelet = 0; cubelet < CUBESIZE; cubelet++)
                if (CUBE(face,line,cubelet) != face)
                    return 0;
    return 1;
}

int readcube(FILE *in)
{
    if (!in) {
        fprintf(stderr, "Error opening file for reading cube\n");
        return 1;
    }

    int face, line, cubelet, i;
    for (line = 0; line < CUBESIZE; line++) {
        for (i = 0; i < CUBESIZE; i++)
            getc(in);
        for (cubelet = 0; cubelet < CUBESIZE; cubelet++)
            CUBE(0,line,cubelet) = getc(in) - '0';
        getc(in);
    }

    for (line = 0; line < CUBESIZE; line++) {
        for (face = 1; face < 5; face++)
            for (cubelet = 0; cubelet < CUBESIZE; cubelet++)
                CUBE(face,line,cubelet) = getc(in) - '0';
        getc(in);
    }

    for (line = 0; line < CUBESIZE; line++) {
        for (i = 0; i < CUBESIZE; i++)
            getc(in);
        for (cubelet = 0; cubelet < CUBESIZE; cubelet++)
            CUBE(5,line,cubelet) = getc(in) - '0';
        getc(in);
    }

    return 0;
}
