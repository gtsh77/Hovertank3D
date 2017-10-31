/* Hovertank 3-D Source Code
 * Copyright (C) 1993-2014 Flat Rock Software
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include "graphdef.h"

#define EXTENSION "HOV"

typedef unsigned long u_long;
typedef unsigned int u_int;
typedef unsigned short int u_short;
typedef unsigned char u_char;

typedef struct
{
  u_short headersize;
  u_char dictionary;
  u_short dataoffsets;
} grheadtype;

typedef struct
{
  unsigned long bit0,bit1;	// 0-255 is a character, > is a pointer to a node
} huffnode;

extern huffnode *grhuffman;

//protos
void LoadNearData(void);
void InitGrFile(void);
void OptimizeNodes (huffnode *);