#include <stdio.h>
#include "build_line.h"

#define TRUE 1
#define FALSE 0

void iniBuildLine(struct line *l)
{
   l->headSz = 0;
   l->tailSegSz = 1;
   l->tailSegCnt = 1;
   
   l->headSzPrior = 0;
   l->tailSegSzPrior = 1;
   l->tailSegCntPrior = 1;
   
   l->tailSegCntNext = 0;

   l->xPrior = 0;
   l->xPriorPrior = 0;
   
   l->x = 1;
   l->y = 1;
   l->length = 1;
   l->offset = 0;
   l->offsetFlipFlop = FALSE;
}

void newSegSm(struct line *l)
{
   int tailSzPrior;
   int x = l->x;
   
   l->x += l->x - l->xPrior;
   l->y += l->y - l->tailSegSz;

   l->xPriorPrior = l->xPrior;
   l->xPrior = x;
   
   l->headSzPrior = l->headSz;
   l->tailSegSzPrior = l->tailSegSz;
   l->tailSegCntPrior = l->tailSegCnt;

   tailSzPrior = l->tailSegSzPrior * l->tailSegCntPrior;
   l->headSz = l->headSzPrior + tailSzPrior - l->tailSegSzPrior;
   l->tailSegSz = l->headSzPrior + tailSzPrior;
   l->tailSegCnt = 1;

   l->length += l->y - l->tailSegSz;

   if (l->offsetFlipFlop)
   l->offset += l->headSz;

   // account tail segments from newSegMd()
   l->x += l->xPrior * l->tailSegCntNext;
   l->y += l->tailSegSz * l->tailSegCntNext;
   l->tailSegCnt += l->tailSegCntNext;
   l->tailSegCntNext = 0;
}

void newSegLg(struct line *l)
{
   int tailSzPrior;
   int x = l->x;
   
   l->x += l->x + l->xPrior;
   l->y += l->y + l->tailSegSz;

   l->xPriorPrior = l->xPrior;
   l->xPrior = x;
   
   l->headSzPrior = l->headSz;
   l->tailSegSzPrior = l->tailSegSz;
   l->tailSegCntPrior = l->tailSegCnt;

   tailSzPrior = l->tailSegSzPrior * l->tailSegCntPrior;
   l->headSz = l->headSzPrior + tailSzPrior + l->tailSegSzPrior;
   l->tailSegSz = l->headSzPrior + tailSzPrior;
   l->tailSegCnt = 1;

   l->length += l->y - l->tailSegSz;

   l->offsetFlipFlop = !l->offsetFlipFlop;

   if (l->offsetFlipFlop)
   l->offset += l->headSz;

   // account tail segments from newSegMd()
   l->x += l->xPrior * l->tailSegCntNext;
   l->y += l->tailSegSz * l->tailSegCntNext;
   l->tailSegCnt += l->tailSegCntNext;
   l->tailSegCntNext = 0;
}

void newSegMd(struct line *l)
{
   int tailSz = l->tailSegSz * l->tailSegCnt;

   l->length += tailSz + l->headSz;
   l->tailSegCntNext++;
}

void undoSegMd(struct line *l)
{
   int tailSz = l->tailSegSz * l->tailSegCnt;

   l->length -= tailSz + l->headSz;
   l->tailSegCntNext--;
}

void incTailSegCnt(struct line *l)
{
   l->tailSegCnt++;

   if (l->x != 1)
   l->x += l->xPrior;

   l->y += l->tailSegSz;

   l->length += l->tailSegSz * (l->tailSegCntNext + 1);
}

void decTailSegCnt(struct line *l)
{
   l->tailSegCnt--;

   if (l->x != 1)
   l->x -= l->xPrior;

   l->y -= l->tailSegSz;

   l->length -= l->tailSegSz * (l->tailSegCntNext + 1);
}

void incPriorTailSegCnt(struct line *l)
{
   int tailSzPrior;
   
   l->tailSegCntPrior++;
   tailSzPrior = l->tailSegSzPrior * l->tailSegCntPrior;

   if (l->xPrior != 1) // if prior headSz is equal to zero
   {
      l->x += l->xPriorPrior * (l->tailSegCnt + 1);
      l->xPrior += l->xPriorPrior;
   }

   l->y += l->tailSegSzPrior * (l->tailSegCnt + 1);
   
   if (l->headSz > l->tailSegSz)
   l->headSz = l->headSzPrior + tailSzPrior + l->tailSegSzPrior;
   else
   l->headSz = l->headSzPrior + tailSzPrior - l->tailSegSzPrior;
   
   l->tailSegSz = l->headSzPrior + tailSzPrior;

   // account offset
   if (l->offset >= l->tailSegSzPrior + l->headSzPrior)
   l->offset += l->tailSegSzPrior;
   
   // account length and tail segments from newSegMd()
   l->length += l->tailSegSzPrior * (l->tailSegCnt + 1) * (l->tailSegCntNext + 1);
}

void decPriorTailSegCnt(struct line *l)
{
   int tailSzPrior;
   
   l->tailSegCntPrior--;
   tailSzPrior = l->tailSegSzPrior * l->tailSegCntPrior;

   if (l->xPrior != 1) // if prior headSz is equal to zero
   {
      l->x -= l->xPriorPrior * (l->tailSegCnt + 1);
      l->xPrior -= l->xPriorPrior;
   }

   l->y -= l->tailSegSzPrior * (l->tailSegCnt + 1);
   
   if (l->headSz > l->tailSegSz)
   l->headSz = l->headSzPrior + tailSzPrior + l->tailSegSzPrior;
   else
   l->headSz = l->headSzPrior + tailSzPrior - l->tailSegSzPrior;
   
   l->tailSegSz = l->headSzPrior + tailSzPrior;

   // account offset
   if (l->offset >= l->tailSegSzPrior + l->headSzPrior)
   l->offset -= l->tailSegSzPrior;
   
   // account length and tail segments from newSegMd()
   l->length -= l->tailSegSzPrior * (l->tailSegCnt + 1) * (l->tailSegCntNext + 1);
}

void truncate(struct line *l, int sz)
{
   l->offset += sz;
   l->length -= sz;
}

void undoTruncate(struct line *l, int sz)
{
   l->offset -= sz;
   l->length += sz;
}

void extend(struct line *l, int sz)
{
   l->length += sz;
}

void convertLgToSm(struct line *l)
{
   if (l->offsetFlipFlop)
   l->offset -= l->headSz;

   l->offsetFlipFlop = !l->offsetFlipFlop;

   l->x -= l->xPriorPrior * 2;
   l->y -= l->tailSegSzPrior * 2;
   l->headSz -= l->tailSegSzPrior * 2;
   
   if (l->offsetFlipFlop)
   l->offset += l->headSz;

   l->length -= l->tailSegSzPrior * 2;
}

void convertSmToLg(struct line *l)
{
   if (l->offsetFlipFlop)
   l->offset -= l->headSz;

   l->x += l->xPriorPrior * 2;
   l->y += l->tailSegSzPrior * 2;
   l->headSz += l->tailSegSzPrior * 2;
   
   l->offsetFlipFlop = !l->offsetFlipFlop;
   
   if (l->offsetFlipFlop)
   l->offset += l->headSz;

   l->length += l->tailSegSzPrior * 2;
}
