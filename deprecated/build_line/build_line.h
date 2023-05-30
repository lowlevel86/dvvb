struct line {
   int headSz;
   int tailSegSz;
   int tailSegCnt;
   int tailSz;
   
   int headSzPrior;
   int tailSegSzPrior;
   int tailSegCntPrior;
   int tailSzPrior;
   
   int xPrior;
   int xPriorPrior;
   
   int x, y;
   int offset;
   int remainder;
   
   int xParent;
   int yParent;
};

void iniBuildLine(struct line *); 
void newHeadLg(struct line *);
void incTailSegCnt(struct line *);
void incPriorTailSegCnt(struct line *);
void convertLgToSm(struct line *);
