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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/stat.h>

#include "xlate.h"
#include "vartable.h"
#include "parse.h"
#include "render.h"
#include "compat.h"

static void print_help_and_exit(int code)
{
    fputs("USAGE: xlate FILE...\n", stderr);
    exit(code);
}

static void print_version_and_exit(void)
{
    fputs("xlate version 0.1\n", stderr);
    exit(EXIT_SUCCESS);
}

VarTable *global_vartable = NULL;

char *make_output_filename(char *in)
{
    int len = strlen(in);
    bool is_reg_extension = strcmp(in + len - 3, ".xl") == 0;
    char *retval;
    if (is_reg_extension)
    {
        retval = malloc(len + 3);
        strcpy(retval, in);
        strcpy(retval + len - 2, "html");
    }
    else
    {
        retval = malloc(len + 6);
        strcpy(retval, in);
        strcpy(retval + len, ".html");
    }
    return retval;
}

bool file_exists(char *path)
{
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

int main(int argc, char **argv)
{
    argv++;
    for (char **a = argv; *a; a++)
    {
        if (strcmp(*a, "--help") == 0)
            print_help_and_exit(EXIT_SUCCESS);
        else if (strcmp(*a, "--version") == 0)
            print_version_and_exit();
        else if (**a == '-' && **a != '\0')
        {
            fprintf(stderr, "Unknown option: %s\n", *a);
            print_help_and_exit(EXIT_FAILURE);
        }
    }
    for (; *argv; argv++)
    {
        bool is_stdin = strcmp(*argv, "-") == 0;
        FILE *in = is_stdin
            ? stdin
            : fopen(*argv, "r");
        if (in == NULL)
        {
            perror("xlate");
            continue;
        }
        Content *c = parse(in);
        FILE *out;
        if (!is_stdin)
        {
            fclose(in);
            char *out_name = make_output_filename(*argv);
            if (file_exists(out_name))
            {
                fprintf(stderr, "File %s already exists. Would you like to override it? [y/N] ", out_name);
                fflush(stderr);
                char buf[2];
                scanf("%s", buf);
                if (strcmp(buf, "Y") != 0 && strcmp(buf, "y") != 0)
                {
                    // free allocated memory
                    continue;
                }
            }
            out = fopen(out_name, "w");
            free(out_name);
        }
        else
            out = stdout;
        render(c, out);
        if (!is_stdin)
        {
            fclose(out);
        }
    }
    return 0;
}
