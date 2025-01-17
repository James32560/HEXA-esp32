#include <vector>
#include <Adafruit_PWMServoDriver.h>
#include <VL53L0X.h>

#include "Leg.h"

class Hexa
{
public:
    Adafruit_PWMServoDriver pca1 = Adafruit_PWMServoDriver(0x40);
    Adafruit_PWMServoDriver pca2 = Adafruit_PWMServoDriver(0x41);
    VL53L0X tof;

    String gait = "Stance", mode = "Car";
    float targetJoyX = 0, targetJoyY = 0, targetHeight = 0, targetDuration = 0;
    float joyX = 0, joyY = 0, height = 0, duration = 0;
    float timer = 0, stanceProportion = 0;
    std::vector<float> gaitOffset = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    const float forwardMultiplier = 30.0;
    const float turnAngle = radians(25);

    std::vector<Leg> legs =
    {
        Leg(pca1, {3, 4, 5},    Right, {85, 0, 0},    radians(0)),
        Leg(pca1, {0, 1, 2},    Right, {66, 88, 0},   radians(45)),
        Leg(pca2, {15, 14, 13}, Left,  {-66, 88, 0},  radians(45)),
        Leg(pca2, {12, 11, 10}, Left,  {-85, 0, 0},   radians(0)),
        Leg(pca2, {9, 8, 7},    Left,  {-66, -88, 0}, radians(-45)),
        Leg(pca1, {6, 7, 8},    Right, {66, -88, 0},  radians(-45))
    };

    Hexa();
    void begin();
    void inputSmoothing();
    void gaitGenerate();
};

//      2     1      //  (15, 14, 13)  (0, 1, 2)  //
//       \___/       //                           //
//       /   \       //                           //
//   3--|     |--0   //  (12, 11, 10)  (3, 4, 5)  //
//       \___/       //                           //
//       /   \       //                           //
//      4     5      //  (9, 8, 7)     (6, 7, 8)  //