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
#include <stdlib.h>

#include "xlate.h"
#include "vartable.h"

static char *sanitize_text(char *str)
{
    int npoison = 0;
    for (char *s = str; *s; s++)
        if (*s == '<' || *s == '>')
            npoison++;
    int len = strlen(str) + npoison * 3 + 1;
    char *san = malloc(len);
    int index = 0;
    for (; *str; str++)
    {
        if (*str == '<')
        {
            san[index++] = '&';
            san[index++] = 'l';
            san[index++] = 't';
            san[index++] = ';';
        }
        else if (*str == '>')
        {
            san[index++] = '&';
            san[index++] = 'g';
            san[index++] = 't';
            san[index++] = ';';
        }
        else
            san[index++] = *str;
    }
    san[index] = '\0';
    return san;
}

static void render_head(FILE *dst)
{
    fputs("<!doctype html>\n"
          "<html>\n"
          "<head>\n"
          "  <meta charset=\"utf-8\">\n",
          dst);
    char *title = sanitize_text(lookup_vartable(global_vartable, "__TITLE__"));
    fprintf(dst, "  <title>%s</title>\n", title);
    fputs("  <style>\n"
          "    body {\n"
          "      font-family: sans-serif;\n"
          "    }\n"
          "    .original {\n"
          "      font-family: monospace;\n"
          "      background-color: #ddd;\n"
          "      border-radius: 2px;\n"
          "      padding: 10px;\n"
          "      margin-bottom: 10px;\n"
          "    }\n"
          "    .translated {\n"
          "      margin-bottom: 30px;\n"
          "      margin-left: 5px;\n"
          "      color: #333;\n"
          "    }\n"
          "  </style>\n",dst);
    fputs("</head>\n"
          "<body>\n", dst);
    fprintf(dst, "<h1>%s</h1>\n", title);
    free(title);
}

static void render_foot(FILE *dst)
{
    fputs("</body>\n"
          "</html>\n", dst);
}

void render(Content *c, FILE *dst)
{
    render_head(dst);
    while (c != NULL)
    {
        fprintf(dst, "  <div class=\"%s\">\n",
                c->type == ORIGINAL
                ? "original"
                :"translated");
        char *content = sanitize_text(c->content);
        fprintf(dst, "    %s\n", content);
        free(content);
        fputs("  </div>\n", dst);
        c = c->next;
    }
    render_foot(dst);
}
