
#define SM_INI_SEG 1 // the initial segment is smaller than the first segment
#define EQ_INI_SEG 2 // the initial segment is equal to the first segment
//#define LG_INI_SEG 3 // the initial segment is larger than the first segment

int returnLine(char *, int, int *, int *, int *, int *);

struct modeling
{
   int iniSegSz; // exSeg extra segment, a negative number is extra segment that is lost
   int iniSegType;
   //NOTE: perhaps allow iniSegSz to have a negative value to allow known, unfinished "firstSeg" values
   
   int firstSegSz;
   int firstSegCnt;
   
   int secondSegSz;
   int secondSegCnt;
   
   int mdSegPtr;
   int smSegPtr;
   int lgSegPtr;
   
   // last whole segment
   int lastSegSz;// firstSeg or secondSeg only
   
   
   int tailSegFound;
   int tailSegSz;
   
   // need this to keep track of segments above head
   int tailSegInc; // Tail must be already found. A head must be found.
   
   int headFound;
   int headSz;
   
   int priorHeadSz;
   int priorTailSegSz;
   int priorTailSegCnt;
   
   int iniHeadSz;
   int iniTailSegSz;
   
   int flipFlopCnt; // starts with first segment, is increment for each segment switch when a head is not found
};

struct operation_tracker // NOTE: only needed for testing
{
   char operations[1024];
};