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

int *cube;
int CUBESIZE;

#define CUBE(x,y,z) cube[((x)*CUBESIZE*CUBESIZE) + ((y)*CUBESIZE) + (z)]

void initcube(void)
{
    cube = (int*) malloc(6 * CUBESIZE * CUBESIZE * sizeof(int));

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
          case -1: return "\e[0m";
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

void printcube(FILE *dbg)
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
    fprintf(dbg,"%s\n",color(-1));
    fflush(dbg);
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

#define depth_offset(x) x ? (x -= depth) : (x += depth)

/* here be dragons */

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

        int t = turns;
        while (t--)
            for (i = 0; i < 3; i++)
                for (j = 0; j < CUBESIZE; j++)
                    swap(&CUBE(mod[i],d,j),&CUBE(mod[i+1],d,j));

        if (depth == 0)
            rotateface(face,turns);
        else if (depth == CUBESIZE-1)
            rotateface(face == UP ? DOWN : UP,((turns + 2) % 4) ?: 2);
    }
    else if (face == FRONT || face == BACK) {
        int d1 = (face == FRONT) ? 0 : CUBESIZE-1;
        int d2 = (face == FRONT) ? CUBESIZE-1 : 0;

        depth_offset(d1);
        depth_offset(d2);

        int t = turns;
        while (t--) {
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
            }
        }
        if (depth == 0)
            rotateface(face,face == BACK ? (((turns + 2) % 4) ?: 2) : turns);
        else if (depth == CUBESIZE-1)
            rotateface(face == FRONT ? BACK : FRONT,((turns + 2) % 4) ?: 2);
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

        int t = turns;
        while (t--) {
            for (i = 0, j = CUBESIZE-1; i < CUBESIZE; i++, j--)
                swap(&CUBE(mod[0],j,dd),&CUBE(mod[1],i,d));
            for (i = 0; i < CUBESIZE; i++)
                swap(&CUBE(mod[1],i,d),&CUBE(mod[2],i,d));
            for (i = 0; i < CUBESIZE; i++)
                swap(&CUBE(mod[2],i,d),&CUBE(mod[3],i,d));
        }
        if (depth == 0)
            rotateface(face,turns);
        else if (depth == CUBESIZE-1)
            rotateface(face == UP ? DOWN : UP,((turns + 2) % 4) ?: 2);
    }
}

void rotate_face_clockwise(size_t n, int face)
{
    size_t i, j;

    for (i = 0; i < n / 2; i++) {
        int last = n - 1 - i;
        for (j = i; j < last; j++) {
            int top = CUBE(face,i,j);
            CUBE(face,i,j) = CUBE(face,last-j,i);
            CUBE(face,last-j,i) = CUBE(face,last,last-j);
            CUBE(face,last,last-j) = CUBE(face,j,last);
            CUBE(face,j,last) = top;
        }
    }
}

void rotate_face_counterclockwise(size_t n, int face)
{
    size_t i, j;

    for (i = 0; i < n / 2; i++) {
        int last = n - 1 - i;
        for (j = i; j < last; j++) {
            int top = CUBE(face,j,last);
            CUBE(face,j,last) = CUBE(face,last,last-j);
            CUBE(face,last,last-j) = CUBE(face,last-j,i);
            CUBE(face,last-j,i) = CUBE(face,i,j);
            CUBE(face,i,j) = top;
        }
    }
}

void rotateface(int face, int clockwise)
{
    if (face == BACK)
        clockwise = ((clockwise + 2) % 4) ?: 2;

    if (clockwise == 1)
        rotate_face_clockwise(CUBESIZE,face);
    else if (clockwise == 2)
        rotate_face_clockwise(CUBESIZE,face), rotate_face_clockwise(CUBESIZE,face);
    else
        rotate_face_counterclockwise(CUBESIZE,face);
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

void readcube(FILE *in)
{
    if (!in) {
        fprintf(stderr, "Error opening file for reading cube\n");
        return;
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
}
