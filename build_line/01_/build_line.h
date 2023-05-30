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
};

void iniBuildLine(struct line *); 
void newSegSm(struct line *);
void newSegMd(struct line *);
void newSegLg(struct line *);
void incTailSegCnt(struct line *);
void incPriorTailSegCnt(struct line *);
void truncate(struct line *);
void extend(struct line *);