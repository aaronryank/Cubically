#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#include <ctype.h>

void strpop(char *s, int idx)
{
    memmove(&s[idx], &s[idx + 1], strlen(s) - idx);
    //strcpy(&s[idx], &s[idx + 1]);
}

char *f(char *orig, int size)
{
    char *s = malloc(1024 * size);
    memset(s, 0, 1024 * size);
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
                int repeat = 0;
                while (isdigit(s[pos+1])) {
                    repeat *= 10;
                    repeat += s[pos+1] - '0';
                    strpop(s, pos+1);
                }

                strpop(s, lastbrace.pos);
                //strpop(s, pos);
                strpop(s, pos-1);

                char *tmp = malloc((strlen(s) + (strlen(lastbrace.since) + 1) * repeat));
                s = realloc(s, (strlen(s) + strlen(lastbrace.since) + 1) * repeat);

                while (--repeat) {
                    strncpy(tmp, s, lastbrace.pos);
                    tmp[lastbrace.pos] = 0;
                    strcat(tmp, lastbrace.since);
                    strcat(tmp, s + lastbrace.pos);
                    strcpy(s, tmp);
                }

                free(tmp);

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
    int c, i = 0, brackets = 0;

    while ((c = getc(in)) != EOF) {
        s[i++] = c;

        brackets += (c == ']');

        if (!(i % 1024))
            s = realloc(s, i + 1024);
    }
    s[i] = 0;

    if (brackets)
        fputs(f(s, i / 1024 + 1), out);
    else
        fputs(s, out);
    free(s);
}
