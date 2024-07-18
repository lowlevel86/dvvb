#include <stdio.h>
#include <stdlib.h> // exit()
#include "printData.h"
#include "testPatt.h"

#define TRUE 1
#define FALSE 0

struct line{
    int headSz;
    int tailSegSz;
    int tailSegCnt;
    int tailSz;
    int xPrior;
    int x, y;
    int offset;
    int remainder;
    int trunOffset; // truncated offset
    int valid;
    int validStore;
    int validPrior;
};

struct acc_patt_vars{
    struct line a[3], b[3], c[3], d[3], e[3];
    struct line *o; // output
    int x, y;
    int offset;
    int remainder;
};

static int getLinePattBit(int x, int y, int loc)
{
    double d = (double)x / y;

    if ((int)(loc * d) != (int)((loc-1) * d))
    return 1;

    return 0;
}

int pattBitLookAhead(struct line *prior, struct line *curr, int loc)
{
    if (loc >= curr->tailSz) // if in the head section
    {
        loc -= curr->tailSz;

        if (curr->headSz > curr->tailSegSz)
        {
            if (loc > curr->headSz - curr->tailSegSz)
            loc -= curr->headSz - curr->tailSegSz;
        }
        else
        loc += curr->tailSegSz - curr->headSz;
    }

    loc = loc % curr->tailSegSz;

    if (loc)
    return getLinePattBit(prior->x, prior->y, prior->offset+loc);

    return 1;
}

void getHeadTailVals(struct line *prior, struct line *curr, int lgrHead)
{
    if (lgrHead)
    curr->headSz = prior->headSz + prior->tailSz + prior->tailSegSz;
    else
    curr->headSz = prior->headSz + prior->tailSz - prior->tailSegSz;

    curr->tailSegSz = prior->headSz + prior->tailSz;
    curr->tailSz = curr->tailSegSz;
    curr->tailSegCnt = 1;
}

void getCoors(struct line *prior, struct line *curr)
{
    int pattBitLoc;
    int actualPattBit;
    int idealPattBit;
    
    if (curr->headSz > curr->tailSegSz)
    {
        curr->x = prior->x * (curr->tailSegCnt + 1) + prior->xPrior;
        curr->y = prior->y * (curr->tailSegCnt + 1) + prior->tailSegSz;
    }
    else
    {
        curr->x = prior->x * (curr->tailSegCnt + 1) - prior->xPrior;
        curr->y = prior->y * (curr->tailSegCnt + 1) - prior->tailSegSz;
    }

    curr->xPrior = prior->x;

    // account different offsets
    curr->offset = prior->offset;

    if (curr->headSz > curr->tailSegSz)
    {
        // use a single line pattern bit to find if the head/tail pattern structure is ideal
        pattBitLoc = curr->tailSz + curr->headSz;
        actualPattBit = getLinePattBit(curr->x, curr->y, pattBitLoc);
        idealPattBit = pattBitLookAhead(prior, curr, pattBitLoc - prior->offset);

        if (prior->offset + curr->tailSegSz < curr->headSz)
        {
            if (actualPattBit == idealPattBit)
            {
                actualPattBit = getLinePattBit(curr->x, curr->y, curr->tailSegSz);
                idealPattBit = pattBitLookAhead(prior, curr, curr->tailSegSz - prior->offset);
                
                if (actualPattBit != idealPattBit)
                curr->offset += curr->headSz;
            }
            else
            curr->offset += prior->tailSegSz;
        }
        else if (actualPattBit != idealPattBit)
        curr->offset += curr->tailSegSz;
    }
    else if (prior->offset) // no offset needed if no prior offset
    {
        // use a single line pattern bit to find if the head/tail pattern structure is ideal
        pattBitLoc = curr->tailSz + curr->headSz;
        actualPattBit = getLinePattBit(curr->x, curr->y, pattBitLoc);
        idealPattBit = pattBitLookAhead(prior, curr, pattBitLoc - prior->offset);

        if (prior->offset + curr->headSz > curr->tailSegSz)
        {
            if (actualPattBit == idealPattBit)
            {
                actualPattBit = getLinePattBit(curr->x, curr->y, curr->tailSegSz);
                idealPattBit = pattBitLookAhead(prior, curr, curr->tailSegSz - prior->offset);

                if (actualPattBit != idealPattBit)
                curr->offset += curr->headSz;
            }
            else
            curr->offset -= prior->tailSegSz;
        }
        else if (actualPattBit != idealPattBit)
        curr->offset += curr->tailSegSz;
    }
}

static void copy(struct line *a, struct line *b)
{
    b->headSz = a->headSz;
    b->tailSegSz = a->tailSegSz;
    b->tailSegCnt = a->tailSegCnt;
    b->tailSz = a->tailSz;
    b->xPrior = a->xPrior;
    b->x = a->x;
    b->y = a->y;
    b->offset = a->offset;
    b->remainder = a->remainder;
    b->trunOffset = a->trunOffset;
    b->valid = a->valid;
}

static void copyAll(struct line *a, struct line *b)
{
    copy(&a[0], &b[0]);
    copy(&a[1], &b[1]);
    copy(&a[2], &b[2]);
}

static void copyTo_newHead(struct line *a, struct line *b, int lgrHead, int loc)
{
    copy(&a[0], &b[0]);
    copy(&a[1], &b[1]);
    getHeadTailVals(&b[1], &b[2], lgrHead);
    getCoors(&b[1], &b[2]);
    b[2].remainder = b[2].y-1 - loc;
    b[2].valid = TRUE;

    //b[2].trunOffset = a[2].trunOffset; //??
}

static void copyToPrior_newHead(struct line *a, struct line *b, int lgrHead, int loc)
{
    copy(&a[1], &b[0]);
    copy(&a[2], &b[1]);
    getHeadTailVals(&b[1], &b[2], lgrHead);
    getCoors(&b[1], &b[2]);

    b[2].trunOffset = a[2].trunOffset;
    b[2].remainder = b[2].y-1 - loc;
    b[2].valid = TRUE;
}

static void incPriorTail(struct line *l, int loc)
{
    int tailSegCnt;
    tailSegCnt = l[2].tailSegCnt;

    l[1].tailSz += l[1].tailSegSz;
    l[1].tailSegCnt++;
    getCoors(&l[0], &l[1]);
    getHeadTailVals(&l[1], &l[2], (l[2].headSz > l[2].tailSegSz));

    l[2].tailSegCnt = tailSegCnt;
    l[2].tailSz = l[2].tailSegSz * tailSegCnt;

    getCoors(&l[1], &l[2]);
    l[2].remainder = l[2].y-1 - loc;
    l[2].valid = TRUE;

    // NOTE: is this if-statement correct????
    if (l[2].trunOffset >= l[1].tailSegSz)
    l[2].trunOffset += l[1].tailSegSz;
}

static void decPriorTail(struct line *l, int loc)
{
    int tailSegCnt;
    tailSegCnt = l[2].tailSegCnt;

    l[1].tailSz -= l[1].tailSegSz;
    l[1].tailSegCnt--;
    getCoors(&l[0], &l[1]);
    getHeadTailVals(&l[1], &l[2], (l[2].headSz > l[2].tailSegSz));

    l[2].tailSegCnt = tailSegCnt;
    l[2].tailSz = l[2].tailSegSz * tailSegCnt;

    getCoors(&l[1], &l[2]);
    l[2].remainder = l[2].y-1 - loc;
    l[2].valid = TRUE;
}

static void incTail(struct line *l, int loc)
{
    l[2].tailSz += l[2].tailSegSz;
    l[2].tailSegCnt++;
    getCoors(&l[1], &l[2]);
    l[2].remainder = l[2].y-1 - loc;
    l[2].valid = TRUE;
}

/*static void decTail(struct line *l, int loc)
{
    l[2].tailSz -= l[2].tailSegSz;
    l[2].tailSegCnt--;
    getCoors(&l[1], &l[2]);
    l[2].remainder = l[2].y-1 - loc;
    l[2].valid = TRUE;
}*/

int checkEstablishedBits(struct acc_patt_vars *apv, int bit, int loc) // temp
{
    char patt[1024];

    patt[loc-1] = bit;

    if (!(loc-1))
    return 0;

    //printf("apv->a[2].remainder+1: %i\n", apv->a[2].remainder+1);
    //printf("apv->a[2].y: %i\n", apv->a[2].y);
    //printf("patt[0]:%i patt[1]:%i\n", patt[0], patt[1]);

    if (matchLineToPatt(apv->a[2].x, apv->a[2].y, apv->a[2].offset+apv->a[2].trunOffset, apv->a[2].remainder+1, patt, loc-1))
    {
        printf("a ERROR, established bits do not match\n");
        return 1;
    }
    
    if (matchLineToPatt(apv->b[2].x, apv->b[2].y, apv->b[2].offset+apv->b[2].trunOffset, apv->b[2].remainder+1, patt, loc-1))
    {
        printf("b ERROR, established bits do not match\n");
        return 1;
    }
    
    if (matchLineToPatt(apv->c[2].x, apv->c[2].y, apv->c[2].offset+apv->c[2].trunOffset, apv->c[2].remainder+1, patt, loc-1))
    {
        printf("c ERROR, established bits do not match\n");
        return 1;
    }
    
    if (matchLineToPatt(apv->d[2].x, apv->d[2].y, apv->d[2].offset+apv->d[2].trunOffset, apv->d[2].remainder+1, patt, loc-1))
    {
        printf("d ERROR, established bits do not match\n");
        return 1;
    }
    
    return 0;
}

int accPattBit(struct acc_patt_vars *apv, int bit, int loc)
{
    // decrement/calculate remainders
    apv->a[2].remainder = apv->a[2].y-1 - loc;
    apv->b[2].remainder = apv->b[2].y-1 - loc;
    apv->c[2].remainder = apv->c[2].y-1 - loc;
    apv->d[2].remainder = apv->d[2].y-1 - loc;
    
    apv->e[2].remainder = apv->e[2].y-1 - loc;
    printf("LOC:%i\n", loc);

    if ((!apv->a[2].valid) || (!apv->b[2].valid) || (!apv->c[2].valid) || (!apv->d[2].valid))
    {
        apv->a[2].validPrior = apv->a[2].validStore;
        apv->a[2].validStore = apv->a[2].valid;
        apv->b[2].validPrior = apv->b[2].validStore;
        apv->b[2].validStore = apv->b[2].valid;
        apv->c[2].validPrior = apv->c[2].validStore;
        apv->c[2].validStore = apv->c[2].valid;
        apv->d[2].validPrior = apv->d[2].validStore;
        apv->d[2].validStore = apv->d[2].valid;
    }

    printf("invalid: ");
    if (!apv->a[2].valid)
    printf("a ");
    if (!apv->b[2].valid)
    printf("b ");
    if (!apv->c[2].valid)
    printf("c ");
    if (!apv->d[2].valid)
    printf("d");
    printf("\n");
    printf("short: ");
    if (apv->a[2].remainder < apv->a[2].headSz)
    printf("a ");
    if (apv->b[2].remainder < 0)
    printf("b ");
    if (apv->c[2].remainder < 0) // if (apv->c[2].remainder < apv->c[2].headSz) ???
    printf("c ");
    if (apv->d[2].remainder < 0)
    printf("d");
    printf("\n");


if (apv->e[2].valid)
{printf("apv->e[2].valid == TRUE!!!!!!!!!\n");
    //return -1;
    copyAll(apv->e, apv->b);
    copyTo_newHead(apv->b, apv->b, FALSE, loc);
    incPriorTail(apv->b, loc);
    incPriorTail(apv->b, loc);
    //printf("----> e trunoffset %i\n", apv->e[2].trunOffset);
    //apv->b[2].trunOffset = apv->e[2].trunOffset;//apv->b[1].tailSegSz;
    //incPriorTail(apv->b, loc);
    //apv->b[2].remainder = apv->b[2].y-1 - loc;
    //l[2].valid = TRUE;

    copyAll(apv->b, apv->a);
    //printf("----> %i %i\n", apv->a[2].trunOffset, apv->a[1].tailSegSz);
    incPriorTail(apv->a, loc);
    //printf("----> %i %i\n", apv->a[2].trunOffset, apv->a[1].tailSegSz);
    //apv->a[2].trunOffset += apv->a[1].tailSegSz;

    copyAll(apv->e, apv->c);

    copyAll(apv->e, apv->d);
    incPriorTail(apv->d, loc);
    incTail(apv->d, loc);

    //NOTE: e is no longer needed???
    apv->e[2].valid = FALSE;
}
else
{
    if (apv->a[2].valid)
    {
        if (!apv->b[2].valid)
        {
            if (apv->a[2].remainder < apv->a[2].headSz)
            incPriorTail(apv->b, loc);
            else
            copyTo_newHead(apv->a, apv->b, FALSE, loc);
        }

        if (!apv->c[2].valid)
        {
            if ((apv->d[2].valid) && (apv->b[2].remainder >= 0))
            incTail(apv->c, loc);
            else
            {
                copyTo_newHead(apv->b, apv->c, TRUE, loc);
                 /*   if (apv->c[2].tailSegSz == apv->b[2].tailSegSz)
                        printf("here !!!!!!!!!!!!!!\n");
                        else
                        printf("not equal!!!!!!!!!!!!!\n");
                    if (apv->b[2].remainder < 0)
                    {
                        printf("b is short!!!!!\n");
                        //copyToPrior_newHead(apv->b, apv->b, FALSE, loc);
                        copyTo_newHead(apv->b, apv->c, TRUE, loc);
                    }
                    else
                    {
                        printf("b is not short\n");
                        copyTo_newHead(apv->b, apv->c, TRUE, loc);
                    }*/
                /*
                if (apv->b[2].remainder < 0)
                {
                    if (apv->c[2].tailSegSz != apv->b[2].tailSegSz)
                    {
                        printf("here !!!!!!!!!!!!!!\n");
                        //copyAll(apv->b, apv->c);
                        //copyToPrior_newHead(apv->c, apv->c, TRUE, loc);
                        copyTo_newHead(apv->b, apv->c, TRUE, loc);
                    }
                    else
                    printf("not equal!!!!!!!!!!!!!\n");
                    //printf("c ");printLinePattVis2(apv->c[2].x, apv->c[2].y, apv->c[2].offset+apv->c[2].trunOffset, apv->c[2].remainder);
                    //copyAll(apv->b, apv->c);
                    //copyToPrior_newHead(apv->c, apv->c, TRUE, loc);
                    //printf("c ");printLinePattVis2(apv->c[2].x, apv->c[2].y, apv->c[2].offset+apv->c[2].trunOffset, apv->c[2].remainder);
                }
                else
                {
                    copyTo_newHead(apv->b, apv->c, TRUE, loc);
                }*/
            }

        }
        
        if (apv->b[2].remainder < 0)
        copyToPrior_newHead(apv->b, apv->b, FALSE, loc);

        if (!apv->d[2].valid)
        {
            copyAll(apv->c, apv->d);
            incTail(apv->d, loc);
        }
        
        if (apv->a[2].remainder < apv->a[2].headSz)
        incPriorTail(apv->a, loc);

        if (apv->a[2].y == apv->b[2].y)
        incPriorTail(apv->a, loc);

        if (apv->c[2].y == apv->d[2].y)
        incTail(apv->d, loc);
    }
    else
    {
        if (apv->c[2].valid)
        {
            // if a, b, and d are invalid then use trunOffset?
            if (!apv->d[2].valid)
            {
                copyAll(apv->c, apv->e);
                incPriorTail(apv->e, loc);
                apv->e[2].trunOffset = apv->c[2].trunOffset + apv->e[1].tailSegSz;
                return -1;
            }

            if (!apv->b[2].valid)
            {
                if (apv->b[2].remainder >= 0)
                {
                    copyToPrior_newHead(apv->c, apv->b, FALSE, loc);
                    copyToPrior_newHead(apv->c, apv->a, FALSE, loc);
                    incPriorTail(apv->a, loc);
                }
                else
                {
                    incPriorTail(apv->b, loc);
                    copyAll(apv->b, apv->a);
                    incPriorTail(apv->a, loc);
                }
            }
        }

        if (!apv->c[2].valid)
        if (apv->b[2].valid)
        {/*
            if (apv->b[2].remainder >= 0){
            if (!apv->a[2].valid)
            copyToPrior_newHead(apv->b, apv->a, FALSE, loc);}*/
            //else
            copyTo_newHead(apv->b, apv->c, TRUE, loc);
            //}
        }

        if (apv->b[2].remainder < 0)
        {
            copyToPrior_newHead(apv->b, apv->b, FALSE, loc);
            
            if (!apv->a[2].valid)
            {
                copyToPrior_newHead(apv->b, apv->a, FALSE, loc);
                incPriorTail(apv->a, loc);
            }
        }

        if (!apv->a[2].valid)
        {
            if (apv->b[2].valid)
            {
            printf("----> %i %i\n", apv->a[2].trunOffset, apv->a[1].tailSegSz);
            printf("----> %i %i\n", apv->b[2].trunOffset, apv->b[1].tailSegSz);
                copyToPrior_newHead(apv->b, apv->a, FALSE, loc);
                printf("----> %i %i\n", apv->a[2].trunOffset, apv->a[1].tailSegSz);
                if (!apv->a[2].trunOffset)// < apv->a[1].tailSegSz) // truncated offset does not exists
                {incPriorTail(apv->a, loc);printf("here //////////\n");}
            }
        }

        if (!apv->d[2].valid)
        {
            if (apv->b[2].valid)
            {
                copyToPrior_newHead(apv->b, apv->d, TRUE, loc);
                incTail(apv->d, loc);
            }
        }
    }
}

//printf("a-> headSz:%i tailSegSz:%i tailSz:%i tailSegCnt:%i\n", apv->a[2].headSz, apv->a[2].tailSegSz, apv->a[2].tailSz, apv->a[2].tailSegCnt);
//printf("x:%i, y:%i, offset:%i, remainder:%i\n", apv->a[2].x, apv->a[2].y, apv->a[2].offset, apv->a[2].remainder);

    printf("a ");printLinePattVis2(apv->a[2].x, apv->a[2].y, apv->a[2].offset+apv->a[2].trunOffset, apv->a[2].remainder);
    printf("b ");printLinePattVis2(apv->b[2].x, apv->b[2].y, apv->b[2].offset+apv->b[2].trunOffset, apv->b[2].remainder);
    printf("c ");printLinePattVis2(apv->c[2].x, apv->c[2].y, apv->c[2].offset+apv->c[2].trunOffset, apv->c[2].remainder);
    printf("d ");printLinePattVis2(apv->d[2].x, apv->d[2].y, apv->d[2].offset+apv->d[2].trunOffset, apv->d[2].remainder);
    if (apv->e[2].valid)
    {printf("e ");printLinePattVis2(apv->e[2].x, apv->e[2].y, apv->e[2].offset+apv->e[2].trunOffset, apv->e[2].remainder);}

    if (apv->a[2].remainder < 0)
    {
        printf("ERROR, a is short\n");
        return 1;
    }
    if (apv->b[2].remainder < 0)
    {
        printf("ERROR, b is short\n");
        return 1;
    }
    if (apv->c[2].remainder < 0)
    {
        printf("ERROR, c is short\n");
        return 1;
    }
    if (apv->d[2].remainder < 0)
    {
        printf("ERROR, d is short\n");
        return 1;
    }


    if ((apv->a[2].x == apv->b[2].x) && (apv->a[2].y == apv->b[2].y) && (apv->a[2].offset == apv->b[2].offset))
    {
        printf("ERROR, a == b\n");
        return 1;
    }
    if ((apv->a[2].x == apv->c[2].x) && (apv->a[2].y == apv->c[2].y) && (apv->a[2].offset == apv->c[2].offset))
    {
        printf("ERROR, a == c\n");
        return 1;
    }
    if ((apv->a[2].x == apv->d[2].x) && (apv->a[2].y == apv->d[2].y) && (apv->a[2].offset == apv->d[2].offset))
    {
        printf("ERROR, a == d\n");
        return 1;
    }

    if ((apv->b[2].x == apv->c[2].x) && (apv->b[2].y == apv->c[2].y) && (apv->b[2].offset == apv->c[2].offset))
    {
        printf("ERROR, b == c\n");
        return 1;
    }
    if ((apv->b[2].x == apv->d[2].x) && (apv->b[2].y == apv->d[2].y) && (apv->b[2].offset == apv->d[2].offset))
    {
        printf("ERROR, b == d\n");
        return 1;
    }

    if ((apv->c[2].x == apv->d[2].x) && (apv->c[2].y == apv->d[2].y) && (apv->c[2].offset == apv->d[2].offset))
    {
        printf("ERROR, c == d\n");
        return 1;
    }

    if (checkEstablishedBits(apv, bit, loc))
    return 1;

    // test
    apv->o = 0;

    if (apv->a[2].valid)
    {
        if (getLinePattBit(apv->a[2].x, apv->a[2].y, apv->a[2].offset+apv->a[2].trunOffset+loc) == bit)
        apv->o = &apv->a[2];
        else
        apv->a[2].valid = FALSE;
    }

    if (apv->b[2].valid)
    {
        if (getLinePattBit(apv->b[2].x, apv->b[2].y, apv->b[2].offset+apv->b[2].trunOffset+loc) == bit)
        apv->o = &apv->b[2];
        else
        apv->b[2].valid = FALSE;
    }

    if (apv->c[2].valid)
    {
        if (getLinePattBit(apv->c[2].x, apv->c[2].y, apv->c[2].offset+apv->c[2].trunOffset+loc) == bit)
        apv->o = &apv->c[2];
        else
        apv->c[2].valid = FALSE;
    }

    if (apv->d[2].valid)
    {
        if (getLinePattBit(apv->d[2].x, apv->d[2].y, apv->d[2].offset+apv->d[2].trunOffset+loc) == bit)
        apv->o = &apv->d[2];
        else
        apv->d[2].valid = FALSE;
    }

    if (apv->e[2].valid)
    {printf("apv->e[2].trunOffset:%i\n", apv->e[2].trunOffset);
        if (getLinePattBit(apv->e[2].x, apv->e[2].y, apv->e[2].offset+apv->e[2].trunOffset+loc) == bit)
        apv->o = &apv->e[2];
        else
        apv->e[2].valid = FALSE;
    }

    if (apv->o)
    {
        apv->x = apv->o->x; // NOTE: add to remainder instead of using vars??
        apv->y = apv->o->y;
        apv->offset = apv->o->offset + apv->o->trunOffset;
        apv->remainder = apv->o->remainder;
        return 0;
    }
    
    printf("ERROR, All are invalid.\n");
    return 1;
}

int getLine(char *patt, int pattSz, int *x, int *y, int *offset, int *remainder)
{
    int i, ret = 0;
    struct acc_patt_vars apv = {0};

    // initialize
    apv.a[0].headSz = 0; apv.a[0].tailSegSz = 1; apv.a[0].tailSz = 1; apv.a[0].tailSegCnt = 1;
    apv.a[0].x = 0; apv.a[0].y = 1; apv.a[0].xPrior = -1; apv.a[0].offset = 0; apv.a[0].remainder = 0;
    apv.a[1].headSz = 0; apv.a[1].tailSegSz = 1; apv.a[1].tailSz = 2; apv.a[1].tailSegCnt = 2;
    apv.a[1].x = 1; apv.a[1].y = 2; apv.a[1].xPrior = 0; apv.a[1].offset = 0; apv.a[1].remainder = 0;
    apv.a[2].headSz = 2; apv.a[2].tailSegSz = 3; apv.a[2].tailSz = 3; apv.a[2].tailSegCnt = 1;
    apv.a[2].x = 2; apv.a[2].y = 5; apv.a[2].offset = 0;// apv.a[2].remainder = 3;?
    apv.a[2].xPrior = apv.a[1].x;

    apv.b[0].headSz = 0; apv.b[0].tailSegSz = 1; apv.b[0].tailSz = 1; apv.b[0].tailSegCnt = 1;
    apv.b[0].x = 0; apv.b[0].y = 1; apv.b[0].xPrior = -1; apv.b[0].offset = 0; apv.b[0].remainder = 0;
    apv.b[1].headSz = 0; apv.b[1].tailSegSz = 1; apv.b[1].tailSz = 2; apv.b[1].tailSegCnt = 2;
    apv.b[1].x = 1; apv.b[1].y = 2; apv.b[1].xPrior = 0; apv.b[1].offset = 0; apv.b[1].remainder = 0;
    apv.b[2].headSz = 1; apv.b[2].tailSegSz = 2; apv.b[2].tailSz = 2; apv.b[2].tailSegCnt = 1;
    apv.b[2].x = 2; apv.b[2].y = 3; apv.b[2].offset = 0;// apv.b[2].remainder = 1;
    apv.b[2].xPrior = apv.b[1].x;

    apv.c[0].headSz = 0; apv.c[0].tailSegSz = 1; apv.c[0].tailSz = 1; apv.c[0].tailSegCnt = 1;
    apv.c[0].x = 0; apv.c[0].y = 1; apv.c[0].xPrior = -1; apv.c[0].offset = 0; apv.c[0].remainder = 0;
    apv.c[1].headSz = 0; apv.c[1].tailSegSz = 1; apv.c[1].tailSz = 1; apv.c[1].tailSegCnt = 1;
    apv.c[1].x = 1; apv.c[1].y = 1; apv.c[1].xPrior = 0; apv.c[1].offset = 0; apv.c[1].remainder = 0;
    apv.c[2].headSz = 2; apv.c[2].tailSegSz = 1; apv.c[2].tailSz = 1; apv.c[2].tailSegCnt = 1;
    apv.c[2].x = 2; apv.c[2].y = 3; apv.c[2].offset = 2;// apv.c[2].remainder = 2;
    apv.c[2].xPrior = apv.c[1].x;

    apv.d[0].headSz = 0; apv.d[0].tailSegSz = 1; apv.d[0].tailSz = 1; apv.d[0].tailSegCnt = 1;
    apv.d[0].x = 0; apv.d[0].y = 1; apv.d[0].xPrior = -1; apv.d[0].offset = 0; apv.d[0].remainder = 0;
    apv.d[1].headSz = 0; apv.d[1].tailSegSz = 1; apv.d[1].tailSz = 1; apv.d[1].tailSegCnt = 1;
    apv.d[1].x = 1; apv.d[1].y = 1; apv.d[1].xPrior = 0; apv.d[1].offset = 0; apv.d[1].remainder = 0;
    apv.d[2].headSz = 2; apv.d[2].tailSegSz = 1; apv.d[2].tailSz = 2; apv.d[2].tailSegCnt = 2;
    apv.d[2].x = 3; apv.d[2].y = 4; apv.d[2].offset = 2;// apv.d[2].remainder = 2;
    apv.d[2].xPrior = apv.d[1].x;

    apv.a[2].valid = TRUE;
    apv.b[2].valid = TRUE;
    apv.c[2].valid = TRUE;
    apv.d[2].valid = TRUE;
    apv.e[2].valid = FALSE;
    apv.a[2].validStore = TRUE;
    apv.b[2].validStore = TRUE;
    apv.c[2].validStore = TRUE;
    apv.d[2].validStore = TRUE;
    apv.e[2].validStore = FALSE;
    apv.a[2].validPrior = TRUE;
    apv.b[2].validPrior = TRUE;
    apv.c[2].validPrior = TRUE;
    apv.d[2].validPrior = TRUE;
    apv.e[2].validPrior = FALSE;

    for (i=0; i < pattSz; i++)
    if ((ret = accPattBit(&apv, patt[i], i+1)))
    break;
    
    if (apv.o == &apv.a[2])
    printf("A ");
    if (apv.o == &apv.b[2])
    printf("B ");
    if (apv.o == &apv.c[2])
    printf("C ");
    if (apv.o == &apv.d[2])
    printf("D ");
    if (apv.o == &apv.e[2])
    printf("E ");
    //if ((apv.o != &apv.a[2]) && (apv.o != &apv.b[2]) && (apv.o != &apv.c[2]) && (apv.o != &apv.d[2]))
    if (apv.o == 0)
    printf("NONE ");
    printf("x:%i y:%i offset:%i remainder:%i\n", apv.x, apv.y, apv.offset, apv.remainder);
    printf("\n");

    *x = apv.x;//struct line?
    *y = apv.y;
    *offset = apv.offset;
    *remainder = apv.remainder;

    return ret;
}
