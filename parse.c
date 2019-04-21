// xlate --- side-by-side translation prettifier
// Copyright (C) 2019  Koki Fukuda
//
// xlate is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// xlate is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#include "xlate.h"
#include "vartable.h"
#include "compat.h"

#define BUF_SIZE 2048
#define VAR_NAME_LIMIT 32

static int linum = 1;

static void syntax_error(char *msg)
{
    fprintf(stderr, "Syntax error: %s at %d\n", msg, linum);
    exit(EXIT_FAILURE);
}

static char *skip_whitespace(char *str)
{
    for (; str; str++)
        if (!isspace(*str))
            break;
    return str;
}

static bool search_defvar(char *str)
{
    str = skip_whitespace(str);
    if (*str != '$') return false;
    str++;
    char key[VAR_NAME_LIMIT];
    int i = 0;
    for (;; str++)
    {
        if (i >= VAR_NAME_LIMIT)
            syntax_error("Variable name exceeds length limit.");
        if (isalpha(*str) || isdigit(*str) || *str == '_')
        {
            key[i] = *str;
            i++;
        }
        else if (*str == '$') break;
        else
            syntax_error("Variable name must be consist of A-Z, a-z, 0-9 or _.");
    }
    if (i <= 0)
        syntax_error("Variable name must contain 1 or more chars.");
    key[i] = '\0';
    str++;
    str = skip_whitespace(str);
    if (*str != '=') syntax_error("Variable definision must be following format: $name$ = value");
    str++;
    str = skip_whitespace(str);
    global_vartable = append_vartable(global_vartable, strdup(key), strdup(str));
    return true;
}

static void remove_return(char *str)
{
    for (int i = strlen(str); i >= 0; i--)
        if (str[i] == '\n' || str[i] == '\r')
            str[i] = '\0';
}

void kill_all_contents(Content *garbage)
{
    if (garbage == NULL) return;
    free(garbage->content);
    kill_all_contents(garbage->next);
    free(garbage);
}

Content *parse(FILE *f)
{
    char buf[BUF_SIZE];
    Content *first = NULL;
    Content *prev = NULL;
    for (;; linum++)
    {
        if (fgets(buf, BUF_SIZE, f) == NULL)
            break;
        remove_return(buf);
        if (buf[0] == '\0' || buf[0] == '#') continue;
        if (search_defvar(buf)) continue;
        Content *c = calloc(sizeof (Content), 1);
        if (prev != NULL) prev->next = c;
        if (first == NULL) first = c;
        if (buf[0] == '!')
        {
            c->type = TRANSLATED;
            c->content = strdup(buf + 1);
        }
        else
        {
            c->type = ORIGINAL;
            c->content = strdup(buf);
        }
        prev = c;
    }
    return first;
}
