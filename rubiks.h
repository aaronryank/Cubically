#pragma once

enum { UP, LEFT, FRONT, RIGHT, BACK, DOWN };

extern int cube[6][3][3];

extern void initcube(void), printcube(void), turncube(int, int), swap(int*, int*), rotateface(int, int);
extern char *color(int);
extern int issolved(void);
