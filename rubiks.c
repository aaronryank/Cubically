#include <stdio.h>

enum { UP, LEFT, FRONT, RIGHT, BACK, DOWN };

int cube[6][3][3];

void initcube(void), printcube(void), turncube(int, int), rotateface(int, int);

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
      case 1: return "0;32m";
      case 2: return "0;35m";
      case 3: return "0;34m";
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
    int mod[4], i;
    if (face == UP || face == DOWN) {
        mod[0] = LEFT;
        mod[1] = FRONT;
        mod[2] = RIGHT;
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
    else if (face == LEFT) {
        mod[0] = UP;
        mod[1] = FRONT;
        mod[2] = DOWN;
        mod[3] = BACK;

        while (turns--) {
            for (i = 0; i < 3; i++) {
                int c = (mod[i+1] == BACK) ? 2 : 0;
                swap(&cube[mod[i]][0][0],&cube[mod[i+1]][0][c]);
                swap(&cube[mod[i]][1][0],&cube[mod[i+1]][1][c]);
                swap(&cube[mod[i]][2][0],&cube[mod[i+1]][2][c]);
            }
            rotateface(face,1);
        }
    }
    else if (face == FRONT) {
        while (turns--) {
            for (i = 0; i < 3; i++)
                swap(&cube[DOWN][0][i],&cube[RIGHT][i][0]);
            for (i = 0; i < 3; i++)
                swap(&cube[RIGHT][i][0],&cube[UP][2][i]);
            for (i = 0; i < 3; i++)
                swap(&cube[UP][2][i],&cube[LEFT][i][2]);

            rotateface(face,1);
        }
    }
    else if (face == BACK) {
        while (turns--) {
            for (i = 0; i < 3; i++)
                swap(&cube[UP][0][i],&cube[RIGHT][i][2]);
            for (i = 0; i < 3; i++)
                swap(&cube[RIGHT][i][2],&cube[DOWN][2][i]);
            for (i = 0; i < 3; i++)
                swap(&cube[DOWN][2][i],&cube[LEFT][i][0]);

            rotateface(face,1);
        }
    }
    else if (face == RIGHT) {
        mod[0] = BACK;
        mod[1] = UP;
        mod[2] = FRONT;
        mod[3] = DOWN;

        while (turns--) {
            for (i = 0; i < 3; i++) {
                int c = (mod[i] == BACK) ? 0 : 2;
                swap(&cube[mod[i]][0][c],&cube[mod[i+1]][0][2]);
                swap(&cube[mod[i]][1][c],&cube[mod[i+1]][1][2]);
                swap(&cube[mod[i]][2][c],&cube[mod[i+1]][2][2]);
            }
            rotateface(face,1);
        }
    }
}

#define rubiksnotation(x) (x == 'U' ? 0 : x == 'L' ? 1 : x == 'F' ? 2 : x == 'R' ? 3 : x == 'B' ? 4 : x == 'D' ? 5 : -1)

int main(void)
{
    initcube();

    while (1) {
        printf("\e[H\e[2J");
        printcube();
        printf("\e[0m\n\nMoves: ");
        int move = getchar();
        move = rubiksnotation(move);
        turncube(move,1);
    }

/* this will be used in the language
    FILE *in = fopen("code.txt","r");

    while (!feof(in)) {
        int face  = getc(in);
        int turns = getc(in);
        face  = face["ULFRBD"];
        turns = (turns == '\'') ? 3 : turns - '0';
        turncube(face,turns);
    }
*/
}

void rotateface(int face, int clockwise)
{
    if (clockwise == 1) {
        swap(&cube[face][0][0], &cube[face][2][0]);
        swap(&cube[face][0][1], &cube[face][1][0]);
        swap(&cube[face][0][2], &cube[face][2][0]);
        swap(&cube[face][1][0], &cube[face][2][1]);
        swap(&cube[face][1][2], &cube[face][2][1]);
        swap(&cube[face][2][2], &cube[face][2][0]);
    }
}
