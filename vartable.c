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

#include <stdlib.h>
#include <string.h>

#include "xlate.h"

VarTable *append_vartable(VarTable *parent, char *k, char *v)
{
    if (parent == NULL)
        parent = calloc(sizeof(VarTable), 1);
    if (parent->name == NULL)
    {
        parent->name = k;
        parent->val = v;
    }
    else
    {
    int cmp = strcmp(parent->name, k);
        if (cmp < 0)
            parent->left = append_vartable(parent->left, k, v);
        else if (cmp == 0)
            parent->val = v;
        else
            parent->right = append_vartable(parent->right, k, v);
    }
    return parent;
}

static char *EMPTY_STR = "";

char *lookup_vartable(VarTable *parent, char *name)
{
    if (parent == NULL) return EMPTY_STR;
    int cmp = strcmp(parent->name, name);
    if (cmp == 0) return parent->val;
    else if (cmp < 0) return lookup_vartable(parent->left, name);
    else return lookup_vartable(parent->right, name);
}
