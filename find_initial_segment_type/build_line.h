struct line {
   int headSz;
   int tailSegSz;
   int tailSegCnt;
   
   int headSzPrior;
   int tailSegSzPrior;
   int tailSegCntPrior;
   
   int tailSegCntNext;

   int xPrior;
   int xPriorPrior;
   
   int x, y;
   int length;
   int offset;
   int offsetFlipFlop;
   
   struct line *extraSmHead;
   struct line *extraMdHead;
   struct line *extraLgHead;
};

void iniBuildLine(struct line *); 
void newSegSm(struct line *);
void newSegMd(struct line *);
void undoSegMd(struct line *);
void newSegLg(struct line *);
void incTailSegCnt(struct line *);
void decTailSegCnt(struct line *);
void incPriorTailSegCnt(struct line *);
void decPriorTailSegCnt(struct line *);
void truncate(struct line *, int);
void undoTruncate(struct line *, int);
void extend(struct line *, int);
void convertLgToSm(struct line *l);
void convertSmToLg(struct line *l);
