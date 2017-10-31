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
  u_char i;

  node = table;

  //printf("%d\n",(node->bit1));

  for (i=0;i<255;i++)
  {
   // printf("%d\n",node->bit0);
    if (node->bit0 >= 256)
      node->bit0 = (u_long)(table+(node->bit0-256));
    if (node->bit1 >= 256)
      node->bit1 = (u_long)(table+(node->bit1-256));
    node++;
  }
}