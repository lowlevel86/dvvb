#include <stdio.h>
#include <string.h>
#include "build_line.h"

#define TRUE 1
#define FALSE 0

void update_extraHead_line(struct line *l)
{
   if (!l->extraSmHead) // if extraSmHead.extraSmHead == 0
   return;
   
   if (!l->extraMdHead) // if extraMdHead.extraMdHead == 0
   return;
   
   if (!l->extraLgHead) // if extraLgHead.extraLgHead == 0
   return;
   
   memcpy(l->extraSmHead, l, sizeof(struct line) - sizeof(struct line *));
   newSegSm(l->extraSmHead);
   
   memcpy(l->extraMdHead, l, sizeof(struct line) - sizeof(struct line *));
   newSegMd(l->extraMdHead);
   
   memcpy(l->extraLgHead, l, sizeof(struct line) - sizeof(struct line *));
   newSegLg(l->extraLgHead);
}

void iniBuildLine(struct line *l)
{
   static struct line extraSmHead;
   static struct line extraMdHead;
   static struct line extraLgHead;
   
   l->extraSmHead = &extraSmHead;
   extraSmHead.extraSmHead = 0;
   
   l->extraMdHead = &extraMdHead;
   extraMdHead.extraMdHead = 0;
   
   l->extraLgHead = &extraLgHead;
   extraLgHead.extraLgHead = 0;
   
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
   
   update_extraHead_line(l);
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
   
   update_extraHead_line(l);
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
   
   update_extraHead_line(l);
}

void newSegMd(struct line *l)
{
   int tailSz = l->tailSegSz * l->tailSegCnt;

   l->length += tailSz + l->headSz;
   l->tailSegCntNext++;
   
   update_extraHead_line(l);
}

void undoSegMd(struct line *l)
{
   int tailSz = l->tailSegSz * l->tailSegCnt;

   l->length -= tailSz + l->headSz;
   l->tailSegCntNext--;
   
   update_extraHead_line(l);
}

void incTailSegCnt(struct line *l)
{
   l->tailSegCnt++;

   if (l->x != 1)
   l->x += l->xPrior;

   l->y += l->tailSegSz;

   l->length += l->tailSegSz * (l->tailSegCntNext + 1);
   
   update_extraHead_line(l);
}

void decTailSegCnt(struct line *l)
{
   l->tailSegCnt--;

   if (l->x != 1)
   l->x -= l->xPrior;

   l->y -= l->tailSegSz;

   l->length -= l->tailSegSz * (l->tailSegCntNext + 1);
   
   update_extraHead_line(l);
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
   
   update_extraHead_line(l);
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
   
   update_extraHead_line(l);
}

void truncate(struct line *l, int sz)
{
   l->offset += sz;
   l->length -= sz;
   
   update_extraHead_line(l);
}

void undoTruncate(struct line *l, int sz)
{
   l->offset -= sz;
   l->length += sz;
   
   update_extraHead_line(l);
}

void extend(struct line *l, int sz)
{
   l->length += sz;
   
   update_extraHead_line(l);
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
   
   update_extraHead_line(l);
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
   
   update_extraHead_line(l);
}
