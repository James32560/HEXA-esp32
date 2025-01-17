#include <Arduino.h>
#include <math.h>

// Size = 3600 => ~22% of RAM
#define LUT_SIZE 3600

extern float sinLUT[LUT_SIZE];
extern float cosLUT[LUT_SIZE];
extern float asinLUT[LUT_SIZE];
extern float acosLUT[LUT_SIZE];
extern float atanLUT[LUT_SIZE];

void initLUT();