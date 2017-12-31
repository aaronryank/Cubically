#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "lang.h"

#define iswhitespace(c)    (c == L' ' || c == L'\t' || c == L'\n' || c == '\r')
#define L_isdigit(c)       (c >= L'0' && c <= L'9')
#define isintegraldigit(c) (c == L'\'')

int undigit(wint_t c)
{
    if (L_isdigit(c))
        return c - L'0';
    else if (c == L'\'')
        return 3;
    return 0;
}

wint_t *parse_file(FILE *in)
{
    wint_t c, *source;
    int p = 0;

    source = malloc(1024 * sizeof(wint_t));
    memset(source,0,1024);

    if (!source) {
        fprintf(stderr,"Error allocating memory for source buffer: %s\n",strerror(errno));
        return NULL;
    }

    while ((c = getwc(in)) != WEOF) {
        if (p && !(p % 1024))
            source = realloc(source, sizeof(wint_t) * (p + 1024));
        if (!iswhitespace(c))
            source[p++] = c;
    }

    return source;
}

command *parse_commands(wint_t *source)
{
    wint_t cmd = 0;
    int idx = 0, i = 0, arg = 0;
    command *commands = NULL;
    commands = malloc(1024 * sizeof(command));
    memset(commands,0,1024);

    if (!commands) {
        fprintf(stderr,"Error allocating memory for command buffer: %s\n",strerror(errno));
        return NULL;
    }

    while (get_command(source, &idx, &cmd, &arg)) {
        commands[i].command = cmd;
        commands[i].arg = arg;
        i++;

        if (i && !(i % 1024))
            commands = realloc(commands, (i + 1024) * sizeof(command));
    }

    return commands;
}

int conv9(int digit, int command)
{
    if (command == '~' || command == '$')
        return digit;
    else
        return digit == 0 ? 9 : digit;
}

int get_command(wint_t *source, int *idx, wint_t *command, int *arg)
{
    *command = 0;
    *arg = -1;

    while (source[*idx]) {
        if (L_isdigit(source[*idx]) || isintegraldigit(source[*idx])) {
            if (*arg < 0)
                *arg = 0;
            (*arg) *= 10;
            (*arg) += conv9(undigit(source[*idx]), *command);
        }
        else {
            if (*command)
                return 1;
            else
                *command = source[*idx];
        }

        (*idx)++;
    }

    return !!(*command);
}
