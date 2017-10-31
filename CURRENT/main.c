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


//handle to egagraph, kept open allways
FILE *grhandle;
// array of offsets in egagraph, -1 for sparse
u_int *grstarts;
// compressed length of a chunk
u_int chunkcomplen;

typedef struct
{
  short int headersize;
  int dictionary;
  int dataoffsets;
} grheadtype;

grheadtype *grhead;


// FUNCS

/*
============================
=
= LoadNearData
=
= Load stuff into data segment before memory manager is
= started (which takes all available memory, near and far)
=
============================
*/

void LoadNearData (void)
{
	u_int length;
	FILE *handle;

	handle = fopen("../DIST/EGAHEAD."EXTENSION,"rb+");
	if(handle == NULL) printf("ERROR 1\n");

	grhandle = fopen("../DIST/EGAGRAPH."EXTENSION, "rb");
	if(grhandle == NULL) printf("ERROR 2\n");

	//get filelength
	fseek(handle, 0L, SEEK_END);
	length = ftell(handle);
	//printf("egahead length: %d\n",length);	
	//alloc mem
	grhead = malloc(length);
	//return cursor to the beginning
	rewind(handle);
	//read stream to grhead
	fread(grhead,sizeof(u_int),length,handle);
	//close stream
	fclose(handle);
}

/*
============================
=
= GetChunkLength
=
= Seeks into the igrab data file at the start of the given chunk and
= reads the uncompressed length (first four bytes).  The file pointer is
= positioned so the compressed data can be read in next.
= ChunkCompLen is set to the calculated compressed length
=
============================
*/

long GetChunkLength (u_int chunk)
{
  u_int len;
  fseek(grhandle,grstarts[chunk],SEEK_SET);
  fread(&len,sizeof(len),1,grhandle);
  chunkcomplen = grstarts[chunk+1]-grstarts[chunk]-4; //???
  printf("chunkcomplen: %d\n",chunkcomplen);
  return len;
}


/////////////////////////////////////////////////////////
//
// InitGrFile
//
/////////////////////////////////////////////////////////


void InitGrFile (void)
{

}


int main (void)
{
	LoadNearData();
	grstarts = (u_int *)( ((char *)grhead)+grhead->dataoffsets); //???
	//grstarts = (u_int *)malloc(sizeof(u_int)*10);

	printf("length of STRUCTPIC: %d\n",GetChunkLength(STRUCTPIC));
	printf("length of STRUCTPICM: %d\n",GetChunkLength(STRUCTPICM));

	printf("headersize: %d\n",grhead->headersize);
	printf("dictionary: %d\n",grhead->dictionary);
	printf("dataoffsets: %d\n",grhead->dataoffsets);
	printf("address: %p\n",(u_int *)((char *)grhead+grhead->dataoffsets));

	return 0;
}


/*
============================
=
= LoadNearData
=
= Load stuff into data segment before memory manager is
= started (which takes all available memory, near and far)
=
============================
*/

// void LoadNearData (void)
// {
//   FILE *handle;
//   u_int length;

// //
// // load egahead.ext (offsets and dictionary for graphics file)
// //
//   handle = fopen("../DIST/EGAHEAD."EXTENSION,"rb+");

//   length = filelength(handle);
//   grhead = malloc(length);

//   read(handle, grhead, length);

//   close(handle);


// }