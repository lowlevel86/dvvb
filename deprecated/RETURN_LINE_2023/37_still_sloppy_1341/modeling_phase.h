#include "build_line.h"
//modeling_controller.h

struct test_vars // temporary
{
   char opTracker[1024];
};
struct test_vars tv;

#define SM_INI_SEG 1 // the initial segment is smaller than the second segment
#define EQ_INI_SEG 2 // the initial segment is equal to the second segment
#define LG_INI_SEG 3 // the initial segment is larger than the second segment

#define increment_tail_segment 0
#define initial_new_medium_top_segment 1
#define lengthen_second_segment_size 2
//#define remove_medium_top_segment_add_large_head 3 // NOTE: maybe normalize instead

#define small_second_segment_found_unknown_head 3
#define large_third_segment_found_unknown_head 4

#define increment_tail_segment_truncate 5
#define new_medium_top_segment 6

#define small_third_segment_found_unknown_head 7
#define small_third_head_segment_found 8
//#define large_third_head_segment_found 9 // NOTE: this is not needed. use large_third_segment_found_unknown_head instead
#define convert_to_small_head 10

#define convert_to_small_head_add_large_head 11

#define TRUE 1
#define FALSE 0

struct modeling_phase
{
   int iniSegFound; // initial possibly partial segment, unknown segment
   int iniSegSz;
   int iniSegType;
   
   //NOTE: "firstSeg" and "secondSeg" seems more easier to understand
   
   int secondSegFound; // a whole segment
   int secondSegSz;
   
   int thirdSegFound; // secondSeg and thirdSeg must differ
   int thirdSegSz;
   
   int secondSegCnt;
   int thirdSegCnt;
   
   int lastSegSz;// secondSeg or thirdSeg only // last whole segment
   int lastPriorSegInc; // NOTE: maybe lastPriorSegBitInc
   
   int tailSegFound;
   int tailSegSz;
   
   // need this to keep track of segments above head
   int tailSegInc; // Tail must be already found. A head must be found.
   
   int headFound;
   int headSz;
   
   // this may not be needed
   //int headInc; // The head does not need to be found. secondSeg and thirdSeg must be found
   
   int priorHeadSz;
   int priorTailSegSz;
   int priorTailSegCnt;
   
   int iniHeadSz;
   int iniTailSegSz;
   
   int build_line_op; // build line operation 
   
   int flipFlopCnt; // starts with second segment, is increment for each segment switch when a head is not found
};
/*
struct modelB
{
   int headSz;
   int headCnt;
   int tailSegSz;
   int tailSegCntA; //disregard iniSegSz unless larger than headSz or tailSegSz  
   int tailSegCntB;
   int flipFlopCnt;
};
*/
void iniModelingPhaseVars(struct modeling_phase *);
void findIniSeg(struct modeling_phase *, int, int);
void accountModelingPhase(struct modeling_phase *, struct line *, int, int);
int getModelingPhase(struct modeling_phase *);
void accountNewLineSeg(struct modeling_phase *, int, int, int, int, int, int, int, int);

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
