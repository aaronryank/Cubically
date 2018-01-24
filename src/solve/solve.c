#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "include/search.h"

extern int *cube;
extern int CUBESIZE;
#define CUBE(x,y,z) cube[((x)*CUBESIZE*CUBESIZE) + ((y)*CUBESIZE) + (z)]

char *solvecube(void)
{
    if (CUBESIZE != 3) {
        fprintf(stderr, "Error: Can only solve a 3x3xx3 cube.\n");
        return;
    }

    char *facelets = malloc(54);

    int i, facemap[6] = {0,3,2,5,1,4};

    for (i = 0; i < 54; i++) {
        int f1 = facemap[(int) floor(i / 9)];
        int f2 = (int) floor(i / 3) % 3;
        int f3 = i % 3;
        facelets[i] = "ULFRBD"[CUBE(f1,f2,f3)];
    }
#ifdef _WIN32
    return solution(facelets, 24, 1000, 0, "C:\\tmp");
#else
    return solution(facelets, 24, 1000, 0, "/tmp/cubically-cache");
#endif
}
