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

#ifndef XLATE_H
#define XLATE_H

typedef enum
{
    ORIGINAL, TRANSLATED
} Type;

typedef struct _Content
{
    Type type;
    char *content;
    struct _Content *next;
} Content;

typedef struct _VarTable
{
    char *name;
    char *val;
    struct _VarTable *left;
    struct _VarTable *right;
} VarTable;

extern VarTable *global_vartable;

#endif
