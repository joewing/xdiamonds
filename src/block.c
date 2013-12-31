/**
 * @file block.c
 * @author Joe Wingbermuehle
 */

#include "xdiamonds.h"
#include "block.h"
#include "image.h"
#include "background.h"
#include "main.h"

#include "images/blank.xpm"
#include "images/begin.xpm"
#include "images/end.xpm"
#include "images/killer.xpm"
#include "images/red.xpm"
#include "images/red-mode.xpm"
#include "images/green.xpm"
#include "images/green-mode.xpm"
#include "images/blue.xpm"
#include "images/blue-mode.xpm"
#include "images/yellow.xpm"
#include "images/yellow-mode.xpm"
#include "images/key.xpm"
#include "images/lock.xpm"
#include "images/block.xpm"
#include "images/swap.xpm"

typedef struct {
   char **data;
   ImageType *image;
} BlockNode;

static BlockNode blocks[] = {
   { blank_xpm,       NULL },
   { begin_xpm,       NULL },
   { end_xpm,         NULL },
   { killer_xpm,      NULL },
   { red_xpm,         NULL },
   { red_mode_xpm,    NULL },
   { green_xpm,       NULL },
   { green_mode_xpm,  NULL },
   { blue_xpm,        NULL },
   { blue_mode_xpm,   NULL },
   { yellow_xpm,      NULL },
   { yellow_mode_xpm, NULL },
   { key_xpm,         NULL },
   { lock_xpm,        NULL },
   { block_xpm,       NULL },
   { swap_xpm,        NULL },
   { NULL,            NULL }
};

static int blockCount = 0;

/** Load the block images. */
void LoadBlocks() {
   int x;

   for(x = 1; blocks[x].data; x++) {
      blocks[x].image = LoadImage(blocks[x].data);
   }
   blockCount = x;

}

/** Unload the block images. */
void UnloadBlocks() {
   int x;

   for(x = 1; blocks[x].data; x++) {
      DestroyImage(blocks[x].image);
   }

}

/** Draw a block at a level location. */
void DrawBlock(BlockType type, int x, int y) {
   if(type < blockCount && type > 0) {
      ClearArea(x * BLOCK_WIDTH, y * BLOCK_HEIGHT,
         BLOCK_WIDTH, BLOCK_HEIGHT);
      DrawImage(buffer, bufferGC, blocks[type].image,
         x * BLOCK_WIDTH, y * BLOCK_HEIGHT);
   } else if(type == BLOCK_NONE) {
      ClearArea(x * BLOCK_WIDTH, y * BLOCK_HEIGHT,
         BLOCK_WIDTH, BLOCK_HEIGHT);
   }
}

/** Draw a block at a pixel location. */
void DrawBlockExact(BlockType type, int x, int y) {
   if(type < blockCount && type > 0) {
      DrawImage(buffer, bufferGC, blocks[type].image, x, y);
   }
}

