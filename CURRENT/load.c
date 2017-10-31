#include "lib.c"

//handle to egagraph, kept open allways
static FILE *grhandle;
// array of offsets in egagraph, -1 for sparse
int *grstarts;
// compressed length of a chunk
u_int chunkcomplen;

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
	fread(grhead,sizeof(length),length,handle);
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
	grhuffman = (huffnode *)( ((u_char *)grhead)+grhead->dictionary);
	grstarts = (u_int *)( ((u_char *)grhead)+grhead->dataoffsets);
	OptimizeNodes(grhuffman);

	printf("length of STRUCTPIC: %d\n",GetChunkLength(STRUCTPIC));

	printf("headersize: %d\n",grhead->headersize);
	printf("dictionary: %d\n",grhead->dictionary);
	printf("dataoffsets: %d\n",grhead->dataoffsets);
	//printf("address: %p\n",(u_int *)((char *)grhead+grhead->dataoffsets));	
}