#ifndef WHEEL_H
#define WHEEL_H


#include "ofMain.h"
#include "imagedrawer.h"

class abstractWheel
{
public:

    int pinsNumber;
    int w; // width of the grid
    int h;  // high of the grid
    imageDrawer drawer; // utility to draw image

    ofVec2f* pins; // array name are pointers
    list<int*> * ** lines; // all lines between all pins
    ofImage pinsRepresentation;
    ofImage gridRepresentation;

    virtual void generatePins();

    void deletePins();
    void drawPins();
    void drawGridRepresentation();
    void randomifyslightlyPosition(); // try to avoid morrié effect
    void initializeLines();
    void destroyLines();

    void setup();
    void setupWithRandomification();

    abstractWheel(int pinsNumber = 8, int w = 100 , int h =100);

};


class wheelCircle: public  abstractWheel {

public:


    float radius;
    ofVec2f center;

    wheelCircle( int pinsNumber, int w , int h  ) ;
    void generatePins();


};




class wheelFromPolyLine: public  abstractWheel {

public:
    ofPolyline polyline;
    wheelFromPolyLine(int pinsNumber, ofPolyline poly);
    void generatePins();

};




/*
 *This class take  a list of ofVec and add pins from it
 */
class wheelExtra: public  abstractWheel
{

public:
    list<ofVec2f> extraPins;
    wheelExtra( int pinsNumber, float radius, ofVec2f center, std::list<ofVec2f> extraPins );
    void generatePins(); 

};


/*
 * This class a bretton flower
 */
class wheelTribal : public  abstractWheel
{

public:
    wheelTribal( int pinsNumber=8, float radius=100, ofVec2f center=ofVec2f(0,0) );
    void generatePins();

};





#endif // WHEEL_H
