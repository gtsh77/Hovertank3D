#include "lib.c"

FILE *grhandle;
int32_t *grstarts;
uint32_t chunkcomplen;
grheadtype *grhead;
huffnode *grhuffman;


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
	FILE* file;
	uint16_t length;
	file = fopen("../DIST/EGAHEAD."EXTENSION,"rb+");
	if(file == NULL) printf("ERROR 1\n");
	fseek(file, 0, 2);
	length = ftell(file);
	grhead = malloc(length);
	fseek(file, 0, 0);
	fread(grhead,length,1,file);
	fclose(file);
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
  int32_t len;
  fseek(grhandle,grstarts[chunk],0);
  fread(&len,sizeof(len),1,grhandle);
  chunkcomplen = grstarts[chunk+1]-grstarts[chunk]-4;
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
	printf("headersize: %d\n",grhead->headersize);
	printf("dictionary: %d\n",grhead->dictionary);
	printf("dataoffsets: %d\n",grhead->dataoffsets);	

	grhuffman = (huffnode *)( ((int8_t *)grhead)+grhead->dictionary);
	grstarts = (int32_t *)( ((int8_t *)grhead)+grhead->dataoffsets);
	OptimizeNodes(grhuffman);

	grhandle = fopen("../DIST/EGAGRAPH."EXTENSION, "rb+");
	if(grhandle == NULL) printf("ERROR 2\n");
	printf("length of STRUCTPIC: %d\n",GetChunkLength(STRUCTPIC));
}

