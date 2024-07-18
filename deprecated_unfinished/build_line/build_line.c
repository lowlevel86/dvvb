#include <stdio.h>
#include "build_line.h"

void iniBuildLine(struct line *l)
{
   l->headSz = 2;
   l->tailSegSz = 1;
   l->tailSegCnt = 1;
   l->tailSz = 1;
   
   l->headSzPrior = 0;
   l->tailSegSzPrior = 1;
   l->tailSegCntPrior = 1;
   l->tailSzPrior = 1;
   
   l->xPrior = 1;
   l->xPriorPrior = 0;
   
   l->x = 2;
   l->y = 3;
   l->offset = 1;
   l->remainder = 4;
   
   l->xParent = 5;
   l->yParent = 8;
}

void newHeadLg(struct line *l) //Note: or Sm
{
   l->xPriorPrior = l->xPrior;
   l->xPrior = l->x;
   l->x = l->x * 2 + l->xPriorPrior;
   l->y = l->y * 2 + l->tailSegSz;
   l->offset += l->tailSegSz;

   l->headSzPrior = l->headSz;
   l->tailSegSzPrior = l->tailSegSz;
   l->tailSegCntPrior = l->tailSegCnt;
   l->tailSzPrior = l->tailSz;

   l->headSz = l->headSzPrior + l->tailSzPrior + l->tailSegSzPrior;
   l->tailSegSz = l->headSzPrior + l->tailSzPrior;
   l->tailSz = l->tailSegSz;
   l->tailSegCnt = 1;
   
   l->xParent = l->x * 3 - l->xPrior;
   l->yParent = l->y * 3 - l->tailSegSz;
   l->remainder = l->yParent - (l->y + l->offset);
}

void incTailSegCnt(struct line *l)
{
   l->tailSz += l->tailSegSz;
   l->tailSegCnt++;
   l->x += l->xPrior;
   l->y += l->tailSegSz;
   
   l->xParent = l->x * 3 - l->xPrior;
   l->yParent = l->y * 3 - l->tailSegSz;
   l->remainder = l->yParent - (l->y + l->offset);
}

void incPriorTailSegCnt(struct line *l)
{
   l->tailSzPrior += l->tailSegSzPrior;
   l->tailSegCntPrior++;
   l->x += l->xPriorPrior * (l->tailSegCnt + 1);
   l->y += l->tailSegSzPrior * (l->tailSegCnt + 1);
   l->xPrior += l->xPriorPrior;
   
   l->headSz = l->headSzPrior + l->tailSzPrior + l->tailSegSzPrior;
   l->tailSegSz = l->headSzPrior + l->tailSzPrior;
   l->tailSz = l->tailSegSz * l->tailSegCnt;
   
   l->xParent = l->x * 3 - l->xPrior;
   l->yParent = l->y * 3 - l->tailSegSz;
   l->remainder = l->yParent - (l->y + l->offset);
}

void convertLgToSm(struct line *l) //Note: or SmToLg
{
   l->headSz -= l->tailSegSzPrior * 2;
   l->offset -= l->tailSegSzPrior;
   
   l->x -= l->xPriorPrior * 2;
   l->y -= l->tailSegSzPrior * 2;
   
   l->xParent = l->x * 3 - l->xPrior;
   l->yParent = l->y * 3 - l->tailSegSz;
   l->remainder = l->yParent - (l->y + l->offset);
}
