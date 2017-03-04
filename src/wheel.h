#ifndef WHEEL_H
#define WHEEL_H


#include "ofMain.h"


class wheel
{
public:

    ofImage img;

    int pinsNumber;
    ofVec2f pins[];

    wheel( ofImage img, int pinsNumber = 16 );

    bool isPointOnLine(ofVec2f p1, ofVec2f startLine, ofVec2f endLine, float tolerance);
    void generatePins(ofVec2f pins[], int pinsSize, float rayon, ofVec2f center);

    ofImage getImgWithPins();

};




#endif // WHEEL_H
