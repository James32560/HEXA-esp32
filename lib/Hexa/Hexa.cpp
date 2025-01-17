#include "Hexa.h"

Hexa::Hexa() {}

void Hexa::begin()
{
    pca1.begin(), pca1.setPWMFreq(50);
    pca2.begin(), pca2.setPWMFreq(50);

    //tof.init();

    for(int i = 0;i < 6;i++) legs[i].setdPosition({0, 0, 0});
}

void Hexa::gaitGenerate()
{
    if(gait == "Stance" || (joyX == 0 && joyY == 0))
    {
        for(int i = 0;i < 6;i++) legs[i].setdPosition({0, 0, 0});
        return;
    }
    else if(gait == "Tripod")
    {
        this->gaitOffset = {0, 1.0/2.0, 0, 1.0/2.0, 0, 1.0/2.0};
        this->stanceProportion = 3.0/6.0;
    }
    else if(gait == "Wave")
    {
        this->gaitOffset = {0, 1.0/6.0, 2.0/6.0, 3.0/6.0, 4.0/6.0, 5.0/6.0};
        this->stanceProportion = 5.0/6.0;
    }
    else if(gait == "Ripple")
    {
        this->gaitOffset = {0, 4.0/6.0, 2.0/6.0, 5.0/6.0, 1.0/6.0, 3.0/6.0};
        this->stanceProportion = 3.2/6.0;
    }
    else if(gait == "Bi")
    {
        this->gaitOffset = {0, 1.0/3.0, 2.0/3.0, 0, 1.0/3.0, 2.0/3.0};
        this->stanceProportion = 2.1/6.0;
    }

    float proportion = timer / duration;
    for(int i = 0;i < 6;i++)
    {
        float timeProportion = proportion + gaitOffset[i];
        timeProportion = timeProportion > 1 ? timeProportion - 1 : timeProportion;

        float x = 0, y = 0, z = 0;
        float forwardX = 0, forwardY = 0, forwardZ = 0;
        float pivotX = 0, pivotY = 0, pivotZ = 0;
        float angle, rotation, radius = pow(legs[i].defaultPosition[0] * legs[i].defaultPosition[0] +
                                            legs[i].defaultPosition[1] * legs[i].defaultPosition[1], 0.5);

        if(0 <= timeProportion && timeProportion <= stanceProportion) // stance
        {
            forwardY = cos(timeProportion * PI / stanceProportion);

            angle = timeProportion / stanceProportion * turnAngle - (turnAngle / 2);
        }
        else if(stanceProportion < timeProportion && timeProportion <= 1) // swing
        {
            forwardY = cos((timeProportion - 1) * PI / (1 - stanceProportion));
            forwardZ = -sin((timeProportion - 1) * PI / (1 - stanceProportion));

            angle = (timeProportion - 1) / (stanceProportion - 1) * turnAngle - (turnAngle / 2);
        }

        if(legs[i].leftRight == Right) rotation = legs[i].rotation;
        else rotation = radians(180) - legs[i].rotation;
        pivotX = (cos(angle + rotation) - cos(rotation)) * radius;
        pivotY = (sin(angle + rotation) - sin(rotation)) * radius;

        if(mode == "Car")
        {
            x = (forwardX * forwardMultiplier * joyY) + (pivotX * joyX);
            y = (forwardY * forwardMultiplier * joyY) + (pivotY * joyX);
            z = forwardZ * height;
        }
        else if(mode == "Crab")
        {
            x = forwardY * forwardMultiplier * joyX;
            y = forwardY * forwardMultiplier * joyY;
            z = forwardZ * height;
        }

        legs[i].setdPosition({x, y, z});
    }
}