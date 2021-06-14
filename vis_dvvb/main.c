#include <stdio.h>
#include "drawPixPatSvg.h"

int main(void)
{
   int headSz[4] = {1, 3, 8, 21};
   int tailSz[4] = {2, 5, 13, 34};
   int tailCnt[4] = {2, 2, 2, 2};
   
   drawPixPatSvg("imgs/01_01.svg", headSz, tailSz, tailCnt, 1);
   drawPixPatSvg("imgs/01_02.svg", headSz, tailSz, tailCnt, 2);
   drawPixPatSvg("imgs/01_03.svg", headSz, tailSz, tailCnt, 3);
   drawPixPatSvg("imgs/01_04.svg", headSz, tailSz, tailCnt, 4);
   
   /*
   int headSz[4] = {2, 5, 17, 56};
   int tailSz[4] = {1, 4, 13, 43};
   int tailCnt[4] = {2, 2, 2, 2};
   
   drawPixPatSvg("imgs/02_01.svg", headSz, tailSz, tailCnt, 1);
   drawPixPatSvg("imgs/02_02.svg", headSz, tailSz, tailCnt, 2);
   drawPixPatSvg("imgs/02_03.svg", headSz, tailSz, tailCnt, 3);
   drawPixPatSvg("imgs/02_04.svg", headSz, tailSz, tailCnt, 4);
   */
   return 0;
}
