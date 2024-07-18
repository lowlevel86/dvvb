#include "build_line.h"

struct test_vars // temporary
{
   char opTracker[1024];
};
struct test_vars tv;

#define SM_INI_SEG 1 // the initial segment is smaller than the second segment
#define EQ_INI_SEG 2 // the initial segment is equal to the second segment
#define LG_INI_SEG 3 // the initial segment is larger than the second segment

#define TRUE 1
#define FALSE 0

struct modeling_phase
{
   int iniSegFound; // initial possibly partial segment, broken segment
   int iniSegSz;
   int iniSegType;
   
   int secondSegFound; // a whole segment
   int secondSegSz;
   
   int thirdSegFound; // secondSeg and thirdSeg must differ
   int thirdSegSz;
   
   int secondSegCnt;
   int thirdSegCnt;
   
   int lastSegSz;// secondSeg or thirdSeg only // last whole segment
   int lastPriorSegInc; // NOTE: maybe lastPriorSegBitInc //lastPriorTailSegInc
   
   int tailSegFound;
   int tailSegSz;
   //int tailSegCnt;
   
   // need this to keep track of segments above head
   int tailSegInc; // Tail must be already found. A head must be found.
   
   int headFound;
   int headSz;
   
   int priorHeadSz;
   int priorTailSegSz;
   int priorTailSegCnt;
   
   int iniHeadSz;
   int iniTailSegSz;
};

void iniModelingPhaseVars(struct modeling_phase *);
void accountModelingPhase(struct modeling_phase *, struct line *, int, int);
int getModelingPhase(struct modeling_phase *);
void accountNewLineSeg(struct modeling_phase *, int, int);

/*           _
           _|_|___the new iniSeg and secondSeg are found
          |_|
         _|_|___tailSeg is found, however, the tailSz is unknown
       _|_|
     _|_|___secondSeg is found
    |_|
   _|_|___iniSeg is found
  |_|
*/
