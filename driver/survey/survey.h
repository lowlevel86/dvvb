#ifndef survey_h
#define survey_h

// route data flags
#define patt_0_exists  0b1  // straight
#define patt_1_exists  0b10 // diagonal

// using pointer address instead of route data
#define using_ptr 0b00010000000000000000000000000000

// edge trace type
#define trace_straight 0b01000000000000000000000000000000
#define trace_diagonal 0b00100000000000000000000000000000

// pointer address bits
#define ptr_bits  0b00001111111111111111111111111111
#define data_bits 0b11110000000000000000000000000000

// trace indicator
#define trace_flag 0b10000000000000000000000000000000

struct survey_data
{
   unsigned int *HD; // holds route data and pointer address
   unsigned int *HU;
   unsigned int *VL;
   unsigned int *VR;
};

void hori_down_scan(char *, int, int, unsigned int *, int);
void hori_up_scan(char *, int, int, unsigned int *, int);
void vert_left_scan(char *, int, int, unsigned int *, int);
void vert_right_scan(char *, int, int, unsigned int *, int);

#endif