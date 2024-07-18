#include <stdio.h>
#include "survey.h"
#include "stamp_test.h"

#define TRUE 1
#define FALSE 0

static void vert_left_trace(char *rast, int width, int height, int x, int y, unsigned int *traceBuff, int threshold)
{
   int down_allow;
   int down_left_allow;
   
   int start_ptr = y*width + x;
   int ptr = start_ptr;
   
   // check if already been traced
   if (traceBuff[ptr])
   return;
   
   while (TRUE)
   {
      down_allow = FALSE;
      down_left_allow = FALSE;
      
      if (VL_diagonal_exists(rast, traceBuff, width, height, x, y, threshold))
      down_left_allow = TRUE;
      else if (VL_straight_exists(rast, traceBuff, width, height, x, y, threshold))
      down_allow = TRUE;
      
      if (down_allow)
      {
         traceBuff[ptr] |= trace_straight;
         traceBuff[start_ptr] |= patt_0_exists;
         
         if (ptr != start_ptr)
         {
            traceBuff[ptr] |= using_ptr;
            traceBuff[ptr] &= data_bits; // erase pointer bits
            traceBuff[ptr] |= start_ptr;
         }
         
         y++;
         ptr += width;
         continue;
      }
      
      if (down_left_allow)
      {
         traceBuff[ptr] |= trace_diagonal;
         traceBuff[start_ptr] |= patt_1_exists;
         
         if (ptr != start_ptr)
         {
            traceBuff[ptr] |= using_ptr;
            traceBuff[ptr] &= data_bits; // erase pointer bits
            traceBuff[ptr] |= start_ptr;
         }
         
         x--;
         y++;
         ptr += width - 1;
         continue;
      }
      
      break;
   }
}

void vert_left_scan(char *rast, int width, int height, unsigned int *traceBuff, int threshold)
{
   int x, y;
   
   for (y=0; y < height-1; y++)
   for (x=width-2; x >= 0; x--)
   vert_left_trace(rast, width, height, x, y, traceBuff, threshold);
}
