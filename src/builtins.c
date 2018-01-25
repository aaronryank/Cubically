#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *scramble(void)
{
    char *retval = malloc(26);
    memset(retval, 0, 26);

    int i, n, m, r, s, t, p;

    for (i = p = 0; i < 26; i++)
    {
        for(n = m, t = 1; t; t = ((m*n) ==9)) {
            r = rand() % 15;
            m = r / 3 + 1;
        }
        s += m;

        if (i > 1) {
            retval[p++] = "URFDLB"[s%6];

            if (r == 0) {
                retval[p++] = "URFDLB"[s%6];
                retval[p++] = "URFDLB"[s%6];
            } else if (r == 1) {
                retval[p++] = "URFDLB"[s%6];
            }
        }
    }

    retval[p] = 0;
    return retval;
}
