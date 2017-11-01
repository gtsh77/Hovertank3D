#include "lib.c"

FILE *grhandle;
huffnode *grhuffman;
uint8_t *buffer;
uint8_t **grsegs;
int32_t *grstarts;

static uint8_t *pictable;
static uint32_t chunkcomplen;
static grheadtype *grhead;

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
	//init multi-use [MU] buffer (AIM)
	buffer = (uint8_t *)malloc(1000);

	//init grsegs w zeros (AIM)
	grsegs = (uint8_t **)calloc(100,100);

	//
	// calculate some offsets in the header
	//	

	grhuffman = (huffnode *)( ((int8_t *)grhead)+grhead->dictionary);
	grstarts = (int32_t *)( ((int8_t *)grhead)+grhead->dataoffsets);
	OptimizeNodes(grhuffman);

	//
	// Open the graphics file, leaving it open until the game is finished
	//	

	grhandle = fopen("../DIST/EGAGRAPH."EXTENSION, "rb+");
	if(grhandle == NULL) printf("ERROR 2\n");

	//
	// load the pic and sprite headers into the data segment
	//

	// make sure this chunk never reloads
	grsegs[STRUCTPIC] = malloc(1);
	grsegs[STRUCTPIC][0] = 1;

	GetChunkLength(STRUCTPIC);
	//clear our MU-buffer (AIM)
	memset(buffer, 0, chunkcomplen);
	buffer = (uint8_t *)malloc(chunkcomplen);
	pictable = (uint8_t *)malloc(chunkcomplen);
	fread(buffer,chunkcomplen,1,grhandle);
	HuffExpand(buffer, pictable, chunkcomplen, grhuffman);
}

// CachePic(), SC_MakeShape(), CacheGrFile(), SetupGraphics(), SC_MakeShape()


/*
====================
=
= SetupGraphics
=
====================
*/

void SetupGraphics (void)
{
  uint16_t i;

  InitGrFile ();        // load the graphic file header

//
// go through the pics and make scalable shapes, the discard the pic
//

  for (i=MAN1PIC;i<DASHPIC;i++)
  {
    CachePic (STARTPICS+i);
    // SC_MakeShape(
    //   grsegs[STARTPICS+i],
    //   pictable[i].width,
    //   pictable[i].height,
    //   &scalesegs[i]
    // );
    //MMFreePtr (&grsegs[STARTPICS+i]);
  }	
}

/*
=====================
=
= CachePic
=
= Make sure a graphic chunk is in memory
=
=====================
*/

void CachePic (int picnum)
{
  int32_t expanded,compressed;     // chunk lengths
  uint8_t *bigbufferseg;          // for compressed

  if (grsegs[picnum])
    return;

  fseek(grhandle,grstarts[picnum],0);

  compressed = grstarts[picnum+1]-grstarts[picnum]-4;

  if (picnum>=STARTTILE8)
  {
  //
  // tiles are of a known size
  //
    if (picnum<STARTTILE8M)             // tile 8s are all in one chunk!
      expanded = BLOCK*NUMTILE8;
    else if (picnum<STARTTILE16)
      expanded = MASKBLOCK*NUMTILE8M;
    else if (picnum<STARTTILE16M)       // all other tiles are one/chunk
      expanded = BLOCK*4;
	 else if (picnum<STARTTILE32)
      expanded = MASKBLOCK*4;
    else if (picnum<STARTTILE32M)
      expanded = BLOCK*16;
    else
      expanded = MASKBLOCK*16;

    compressed = grstarts[picnum+1]-grstarts[picnum];
  }
  else
  {
  //
  // other things have a length header at start of chunk
  //
    //read(grhandle,&expanded,sizeof(expanded));
    printf("%d true\n",picnum);
    fread(&expanded,sizeof(expanded),1,grhandle);
    compressed = grstarts[picnum+1]-grstarts[picnum]-4;
  }

  //
  // allocate space for expanded chunk
  //

  grsegs[picnum] = (uint8_t *)malloc(expanded);
  memset(buffer, 0, compressed);
  fseek(grhandle,grstarts[picnum],0);
  fread(buffer,compressed,1,grhandle);
  HuffExpand (buffer, grsegs[picnum], expanded, grhuffman);
}