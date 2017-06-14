#include <stdio.h>
#include "rubiks.h"

int cube[6][3][3];

void initcube(void)
{
    int face, line, cubelet;
    for (face = 0; face < 6; face++)
        for (line = 0; line < 3; line++)
            for (cubelet = 0; cubelet < 3; cubelet++)
                cube[face][line][cubelet] = face;
}

char *color(int x)
{
    switch (x) {
      case 0: return "0;31m";
      case 1: return "0;34m";
      case 2: return "0;35m";
      case 3: return "0;32m";
      case 4: return "0;36m";
      case 5: return "0;33m";
    }
}

void printcube(void)
{
    int face, line, cubelet;
    for (line = 0; line < 3; line++) {
        int o0 = cube[0][line][0];
        int o1 = cube[0][line][1];
        int o2 = cube[0][line][2];
        printf("   \e[%s%d\e[%s%d\e[%s%d\n",color(o0),o0,color(o1),o1,color(o2),o2);
    }

    for (line = 0; line < 3; line++) {
        for (face = 1; face < 5; face++)
            for (cubelet = 0; cubelet < 3; cubelet++)
                printf("\e[%s%d",color(cube[face][line][cubelet]),cube[face][line][cubelet]);
        putchar('\n');
    }

    for (line = 0; line < 3; line++) {
        int o0 = cube[5][line][0];
        int o1 = cube[5][line][1];
        int o2 = cube[5][line][2];
        printf("   \e[%s%d\e[%s%d\e[%s%d\n",color(o0),o0,color(o1),o1,color(o2),o2);
    }
    putchar('\n');
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

        int d = (face == UP ? 0 : 2);
        while (turns--) {
            for (i = 0; i < 3; i++) {
                swap(&cube[mod[i]][d][0],&cube[mod[i+1]][d][0]);
                swap(&cube[mod[i]][d][1],&cube[mod[i+1]][d][1]);
                swap(&cube[mod[i]][d][2],&cube[mod[i+1]][d][2]);
            }
            rotateface(face,1);
        }
    }
    else if (face == FRONT || face == BACK) {
        int d1 = (face == FRONT) ? 0 : 2;
        int d2 = d1 ? 0 : 2;
        while (turns--) {
            int tmp = 1;
            if (face == BACK)
                tmp = 3;
            while (tmp--) {
                for (i = 0, j = 2; i < 3; i++, j--)
                    swap(&cube[DOWN][d1][i],&cube[RIGHT][j][d1]);
                for (i = 0; i < 3; i++)
                    swap(&cube[RIGHT][i][d1],&cube[UP][d2][i]);
                for (i = 0, j = 2; i < 3; i++, j--)
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

        int d = (face == RIGHT) ? 2 : 0;
        int dd = d ? 0 : 2;

        while (turns--) {
            for (i = 0, j = 2; i < 3; i++, j--)
                swap(&cube[mod[0]][j][dd],&cube[mod[1]][i][d]);
            for (i = 0; i < 3; i++)
                swap(&cube[mod[1]][i][d],&cube[mod[2]][i][d]);
            for (i = 0; i < 3; i++)
                swap(&cube[mod[2]][i][d],&cube[mod[3]][i][d]);
            rotateface(face,1);
        }
    }
}

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
