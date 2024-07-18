// last updated 08-16-2023
/*
pattern visual   _  ___ tailSegCntB:2
                |_|
                |_|
               _|_|
              |_|
              |_|
             _|_| ___ flipFlopCnt:2, headCnt:3
            |_|
           _|_|
          |_|
          |_|
         _|_| ___ flipFlopCnt:1, headCnt:2
        |_|
       _|_|
      |_|
      |_|
     _|_| ___ headCnt:1
    |_|
   _|_| ___ tailSegCntA:0
  |_|
*/

int headSz = 0;
int headCnt = 0;
int tailSegSz = 0;
int tailSegCntA = 0;
int tailSegCntB = 0;
int flipFlopCnt = 0;

if ((phase.headFound) && (phase.tailSegFound))
{
   /*
   initial segment type:

   headSz > tailSegSz
             |_|          |_|          |_|
             |_|          |_|          |_|
            _|_|         _|_|         _|_|
           |_|          |_|          |_|
          _|_|         _|_|         _|_|
         |_|          |_|          |_|
        _|_|         _|_|         _|_| tail
       |_|          |_|          |_|
                    |_|          |_|
                                 |_|
      small        equal        large

   headSz > tailSegSz
              _|_|         _|_|         _|_|
             |_|          |_|          |_|
            _|_|         _|_|         _|_|
           |_|          |_|          |_|
          _|_|         _|_|         _|_|
         |_|          |_|          |_|
         |_|          |_|          |_|
        _|_|         _|_|         _|_| head
       |_|          |_|          |_|
                    |_|          |_|
                                 |_|
      small        small        not a line

   headSz < tailSegSz
           |_|        |_|        |_|
          _|_|       _|_|       _|_|
         |_|        |_|        |_|  
         |_|        |_|        |_|  
        _|_|       _|_|       _|_|  
       |_|        |_|        |_|    
       |_|        |_|        |_|    
      _|_|       _|_|       _|_| tail
     |_|        |_|        |_|      
                |_|        |_|      
                           |_|      
    small      small      equal

   headSz < tailSegSz
           |_|        |_|        |_|
           |_|        |_|        |_|
          _|_|       _|_|       _|_|
         |_|        |_|        |_|  
         |_|        |_|        |_|  
        _|_|       _|_|       _|_|  
       |_|        |_|        |_|    
      _|_|       _|_|       _|_| head
     |_|        |_|        |_|      
                |_|        |_|      
                           |_|      
    small      equal      large
   */

   if (phase.tailSegSz == phase.secondSegSz)
   {
      headSz = phase.thirdSegSz;
      tailSegSz = phase.secondSegSz;
      
      headCnt = phase.thirdSegCnt;
      
      if (phase.iniSegType == LG_INI_SEG)
      headCnt++;
      
      tailSegCntA = phase.secondSegCnt - phase.tailSegInc;
      
      if (headCnt >= 2)
      if (phase.secondSegSz > phase.thirdSegSz)
      tailSegCntA++;
   }
   else
   {
      headSz = phase.secondSegSz;
      tailSegSz = phase.thirdSegSz;
      
      headCnt = phase.secondSegCnt;
      
      tailSegCntA = phase.thirdSegCnt - phase.tailSegInc;
      
      //if (phase.iniSegType == LG_INI_SEG)
      //tailSegCntA++;
   }

   tailSegCntB = phase.tailSegInc;

   if (phase.lastSegSz == tailSegSz)
   flipFlopCnt = headCnt - 1;
}
/*
Normalization Concept:

The same operations should be able to work on
any line structure within the same category.
*/

/*
printf("--- Line Structure ---\n");
printf("headSz:%i\n", headSz);
printf("tailSegSz:%i\n", tailSegSz);
printf("headCnt:%i\n", headCnt);
printf("tailSegCntA:%i\n", tailSegCntA);
printf("tailSegCntB:%i\n", tailSegCntB);
printf("flipFlopCnt:%i\n", flipFlopCnt);*/