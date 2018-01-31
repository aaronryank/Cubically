#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

void strpop(char *s, int idx)
{
    memmove(&s[idx], &s[idx + 1], strlen(s) - idx);
    //strcpy(&s[idx], &s[idx + 1]);
}

char *f(char *orig)
{
    char *s = malloc(1024);
    memset(s, 0, 1024);
    strcpy(s, orig);

    int pos, len, braces;
    struct {
      int pos, inc, set;
      char since[1024];
    } lastbrace;

    do {
        memset(lastbrace.since, 0, 1024);
        for (braces = pos = lastbrace.pos = lastbrace.set = lastbrace.inc = 0, len = strlen(s); pos < len; pos++) {
            if (s[pos] == '[') {
                lastbrace.pos = pos;
                lastbrace.set = 1;
                lastbrace.inc = 0;
                memset(lastbrace.since, 0, 1024);
            } else if (s[pos] == ']') {
                braces++;
                int digit = s[pos+1] - '0' - 1;

                strpop(s, lastbrace.pos);
                strpop(s, pos);
                strpop(s, pos-1);

                while (digit--) {
                    char *tmp = malloc(strlen(s) + strlen(lastbrace.since));

                    strncpy(tmp, s, lastbrace.pos);
                    tmp[lastbrace.pos] = 0;
                    strcat(tmp, lastbrace.since);
                    strcat(tmp, s + lastbrace.pos);
                    strcpy(s, tmp);

                    free(tmp); 
                }

                break;
            } else {
                if (lastbrace.set)
                    lastbrace.since[lastbrace.inc++] = s[pos];
            }
        }
    } while (braces);

    return s;
}

int bracket(FILE *in, FILE *out)
{
    char *s = malloc(1024);
    int c, i = 0;
    while ((c = getc(in)) != EOF) {
        s[i++] = c;

        if (!(i % 1024))
            s = realloc(s, i + 1024);
    }

    fputs(f(s), out);
    free(s);
}
