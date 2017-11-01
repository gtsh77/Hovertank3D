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
#include <string.h>
#include <unistd.h>
#include "graphdef.h"

#define EXTENSION "HOV"

#define BLOCK 32
#define MASKBLOCK 40

typedef struct
{
  uint16_t headersize;
  uint32_t dictionary;
  uint32_t dataoffsets;
} __attribute__((packed)) grheadtype;

typedef struct
{
  uint8_t bit0,bit1;	// 0-255 is a character, > is a pointer to a node
} __attribute__((packed)) huffnode;


extern uint8_t *buffer;
extern huffnode *grhuffman;
extern FILE *grhandle;
extern int32_t *grstarts;
extern huffnode *grhuffman;
extern uint8_t **grsegs;

//protos
void LoadNearData(void);
void InitGrFile(void);
void OptimizeNodes (huffnode *);
void HuffExpand (uint8_t *, uint8_t *, int32_t, huffnode *);
void SetupGraphics (void);
void CachePic (int picnum);