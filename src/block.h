/**
 * @file block.h
 * @author Joe Wingbermuehle
 */

#ifndef BLOCK_H
#define BLOCK_H

/** Block types. */
typedef enum {

   BLOCK_NONE          = 0,  /* 0  0 */
   BLOCK_BEGIN         = 1,  /* 1  1 */
   BLOCK_END           = 2,  /* 6  2 */ 
   BLOCK_KILLER        = 3,  /* 7  6 */
   BLOCK_RED           = 4,  /* 4  8 */
   BLOCK_RED_CHANGE    = 5,  /* 5  9 */
   BLOCK_GREEN         = 6,  /* 8  E */
   BLOCK_GREEN_CHANGE  = 7,  /* 9  F */
   BLOCK_BLUE          = 8,  /* 2  A */
   BLOCK_BLUE_CHANGE   = 9,  /* 3  B */
   BLOCK_YELLOW        = 10, /* D  C */
   BLOCK_YELLOW_CHANGE = 11, /* E  D */
   BLOCK_KEY           = 12, /* C    */
   BLOCK_LOCK          = 13, /* B    */
   BLOCK_BLOCK         = 14, /* A  3 */
   BLOCK_SWAP          = 15  /* F    */

} BlockType;


/** Load the block images. */
void LoadBlocks();

/** Unload the block images. */
void UnloadBlocks();

/** Draw a block (at a level location).
 * @param type The type of block.
 * @param x The x-coordinate of the block (in level units).
 * @param y The y-coordinate of the block (in level units).
 */
void DrawBlock(BlockType type, int x, int y);

/** Draw a block (at a pixel location).
 * @param type The type of block.
 * @param x The x-coordinate of the block (in pixels).
 * @param y The y-coordinate of the block (in pixels).
 */
void DrawBlockExact(BlockType type, int x, int y);

#endif /* BLOCK_H */

