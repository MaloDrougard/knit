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
    ofImage representation;


    wheel( int pinsNumber=8, float radius=100, ofVec2f center=ofVec2f(0,0) );

    void generatePins();
    void deletePins();
    void drawPins();

};




class wheelWithCenter : public wheel
{

public:
    wheelWithCenter( int pinsNumber=8, float radius=100, ofVec2f center=ofVec2f(0,0) );
    void generatePins();

};


/*
 *This class take  a list of ofVec and add pins from it
 */
class wheelExtra: public wheel
{

public:
    list<ofVec2f> extraPins;
    wheelExtra( int pinsNumber, float radius, ofVec2f center, std::list<ofVec2f> extraPins );
    void generatePins();


};



class wheelTribal : public wheel
{

public:
    wheelTribal( int pinsNumber=8, float radius=100, ofVec2f center=ofVec2f(0,0) );
    void generatePins();

};





#endif // WHEEL_H
