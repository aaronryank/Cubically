#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "lang.h"

#define iswhitespace(c)    (c == L' ' || c == L'\t' || c == L'\n' || c == '\r')
#define L_isdigit(c)       (c >= L'0' && c <= L'9')
#define isintegraldigit(c) (c == L'\'' || (c >= 0 && c <= 0x19) || (c >= 0x90 && c <= 0x99))

int undigit(wint_t c)
{
    if (L_isdigit(c))
        return c - L'0';
    else if (c == L'\'')
        return 3;
    else if (isintegraldigit(c))
        return c + 100;
    return 0;
}

int *parse_file(FILE *in)
{
    wint_t c;
    int *source;
    int p = 0;

    source = malloc(sizeof(int) * 1024);
    memset(source,0,1024);

    if (!source) {
        fprintf(stderr,"Error allocating memory for source buffer: %s\n",strerror(errno));
        return NULL;
    }

    while ((c = getwc(in)) != WEOF) {
        if (p && !(p % 1024))
            source = realloc(source, sizeof(int) * (p + 1024));
        if (!iswhitespace(c))
            source[p++] = sbcs_convert(c);
    }

    return source;
}

int *parse_string(char *str)
{
    int c;
    int *source;
    int p = 0;

    source = malloc(1024 * sizeof(int));
    memset(source, 0, 1024);

    if (!source) {
        fprintf(stderr, "Error allocating memory for source buffer: %s\n", strerror(errno));
        return NULL;
    }

    while ((c = btowc(*str))) {
        if (p && !(p % 1024))
            source = realloc(source, sizeof(int) * (p + 1024));
        if (!iswhitespace(c))
            source[p++] = sbcs_convert(c);
        str++;
    }

    return source;
}

command *parse_commands(int *source)
{
    int cmd = 0;
    int idx = 0, j = 0, i = 0, argc = 0, arg[100] = {0};
    command *commands = NULL;
    commands = malloc(1024 * sizeof(command));
    memset(commands, 0, 1024);

    if (!commands) {
        fprintf(stderr, "Error allocating memory for command buffer: %s\n", strerror(errno));
        return NULL;
    }

    while (get_command(source, &idx, &cmd, arg, &argc)) {
        commands[i].command = cmd;
        commands[i].argc = argc;
        for (j = 0; j < argc; j++)
            commands[i].arg[j] = arg[j];
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
        return digit ?: 9;
}

int get_command(int *source, int *idx, int *command, int arg[100], int *argc)
{
    *command = 0;
    *argc = 0;

    while (source[*idx]) {
        if (L_isdigit(source[*idx]) || isintegraldigit(source[*idx])) {
            arg[(*argc)++] = conv9(undigit(source[*idx]), *command);
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
