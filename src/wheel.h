#ifndef WHEEL_H
#define WHEEL_H


#include "ofMain.h"


class wheel
{
public:

    int pinsNumber;
    float radius;
    ofVec2f center;
    ofVec2f* pins; // array name are pointers


    wheel( int pinsNumber=8, float radius=100, ofVec2f center=ofVec2f(0,0) );

    void generatePins();

};




#endif // WHEEL_H
