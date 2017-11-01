/*
===============
=
= OptimizeNodes
=
= Goes through a huffman table and changes the 256-511 node numbers to the
= actular address of the node.  Must be called before HuffExpand
=
===============
*/

void OptimizeNodes (huffnode *table)
{
  huffnode *node;
  uint8_t i;

  node = table;

  for (i=0;i<255;i++)
  {
    if (node->bit0 >= 256)
      node->bit0 = (uint64_t)(table+(node->bit0-256));
    if (node->bit1 >= 256)
      node->bit1 = (uint64_t)(table+(node->bit1-256));
    node++;
  }
}

/*
======================
=
= HuffExpand
= Modern ver taken from 
= Codes4Fun/SDL_Hovertank3D
======================
*/

void HuffExpand (uint8_t *source, uint8_t *dest,
  int32_t length,huffnode *hufftable)
{
  uint32_t bit,byte,code;
  huffnode *nodeon,*headptr;

  headptr = hufftable+254;  // head node is allways node 254
  nodeon = headptr;

  bit = 1;
  byte = *source++;

  while (length)
  {
    if (byte&bit)
      code = nodeon->bit1;
    else
      code = nodeon->bit0;

    bit<<=1;
    if (bit==256)
    {
      bit=1;
      byte = *source++;
    }

    if (code<256)
    {
      *dest++=code;
      printf("%d\n",code);
      nodeon=headptr;
      length--;
    }
    else
    {
      nodeon = &hufftable[code-256];
    }
  }

#if 0
  huffnode *headptr;
  headptr = Hufftable+254;  // head node is allways node 254

  huffnode *curhuffnode = headptr;
  int i, j;
  int written = 0;
  i = 0;
  while (written < length) {
    uint8_t srcbyte = Source[i++];
    for (j = 0; j < 8; j++) {
      unsigned short b = curhuffnode->bit0;
      if (srcbyte&1) {
        b = curhuffnode->bit1;
      }
      srcbyte = srcbyte>>1;
      if (b < 256) {
        Dest[written++] = (uint8_t)b;
        curhuffnode = headptr;
        if (written == length) {
          break;
        }
      } else {
        assert(b-256 >= 0);
        curhuffnode = &Hufftable[b-256];
      }
    }
  }
#endif
}