#include <stdio.h>
#include "survey.h"
#include "stamp_test.h"

#define TRUE 1
#define FALSE 0

static void hori_up_trace(char *rast, int width, int height, int x, int y, unsigned int *traceBuff, int threshold)
{
   int right_allow;
   int up_right_allow;
   
   int start_ptr = y*width + x;
   int ptr = start_ptr;
   
   // check if already been traced
   if (traceBuff[ptr])
   return;
   
   while (TRUE)
   {
      right_allow = FALSE;
      up_right_allow = FALSE;
      
      if (HU_diagonal_exists(rast, traceBuff, width, height, x, y, threshold))
      up_right_allow = TRUE;
      else if (HU_straight_exists(rast, traceBuff, width, height, x, y, threshold))
      right_allow = TRUE;
      
      if (right_allow)
      {
         traceBuff[ptr] |= trace_straight;
         traceBuff[start_ptr] |= patt_0_exists;
         
         if (ptr != start_ptr)
         {
            traceBuff[ptr] |= using_ptr;
            traceBuff[ptr] &= data_bits; // erase pointer bits
            traceBuff[ptr] |= start_ptr;
         }
         
         x++;
         ptr++;
         continue;
      }
      
      if (up_right_allow)
      {
         traceBuff[ptr] |= trace_diagonal;
         traceBuff[start_ptr] |= patt_1_exists;
         
         if (ptr != start_ptr)
         {
            traceBuff[ptr] |= using_ptr;
            traceBuff[ptr] &= data_bits; // erase pointer bits
            traceBuff[ptr] |= start_ptr;
         }
         
         x++;
         y--;
         ptr -= width - 1;
         continue;
      }
      
      break;
   }
}

void hori_up_scan(char *rast, int width, int height, unsigned int *traceBuff, int threshold)
{
   int x, y;
   
   for (x=0; x < width-1; x++)
   for (y=height-2; y >= 0; y--)
   hori_up_trace(rast, width, height, x, y, traceBuff, threshold);
}
