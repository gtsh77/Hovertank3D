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
=
======================
*/

// void HuffExpand (u_char *source, u_char *dest,
//   long length,huffnode *hufftable)
// {
//   u_long bit,byte,node,code;
//   u_long sourceseg,sourceoff,destseg,destoff,endseg,endoff;
//   huffnode *nodeon,*headptr;

//   headptr = hufftable+254;  // head node is allways node 254

// //#if0
//   bit = 1;
//   byte = *source++;

//   while (length)
//   {
//     if (byte&bit)
//       code = nodeon->bit1;
//     else
//       code = nodeon->bit0;

//     bit<<=1;
//     if (bit==256)
//     {
//       bit=1;
//       byte = *source++;
//     }

//     if (code<256)
//     {
//       *dest++=code;
//       nodeon=headptr;
//       length--;
//     }
//     else
//       nodeon = (huffnode *)code;
//   }

// //#endif

//   source++; // normalize
//   source--;
//   dest++;
//   dest--;

//   sourceseg = FP_SEG(source);
//   sourceoff = FP_OFF(source);
//   destseg = FP_SEG(dest);
//   destoff = FP_OFF(dest);

//   length--;
// //
// // al = source byte
// // cl = bit in source (1,2,4,8,...)
// // dx = code
// //
// // ds:si source
// // es:di dest
// // ss:bx node pointer
// //

// asm     mov bx,[headptr]
// asm mov cl,1

// asm mov si,[sourceoff]
// asm mov di,[destoff]
// asm mov es,[destseg]
// asm mov ds,[sourceseg]

// asm lodsb     // load first byte

// expand:
// asm test  al,cl   // bit set?
// asm jnz bit1
// asm mov dx,[ss:bx]  // take bit0 path from node
// asm jmp gotcode
// bit1:
// asm mov dx,[ss:bx+2]  // take bit1 path

// gotcode:
// asm shl cl,1    // advance to next bit position
// asm jnc sourceup
// asm lodsb
// asm cmp si,0x10   // normalize ds:si
// asm   jb  sinorm
// asm mov cx,ds
// asm inc cx
// asm mov ds,cx
// asm xor si,si
// sinorm:
// asm mov cl,1    // back to first bit

// sourceup:
// asm or  dh,dh   // if dx<256 its a byte, else move node
// asm jz  storebyte
// asm mov bx,dx   // next node = (huffnode *)code
// asm jmp expand

// storebyte:
// asm mov [es:di],dl
// asm inc di    // write a decopmpressed byte out
// asm mov bx,[headptr]  // back to the head node for next bit

// asm cmp di,0x10   // normalize es:di
// asm   jb  dinorm
// asm mov dx,es
// asm inc dx
// asm mov es,dx
// asm xor di,di
// dinorm:

// asm sub [WORD PTR ss:length],1
// asm jnc expand
// asm   dec [WORD PTR ss:length+2]
// asm jns expand    // when length = ffff ffff, done

// asm mov ax,ss
// asm mov ds,ax

// }
