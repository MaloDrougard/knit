#ifndef WHEEL_H
#define WHEEL_H


#include "ofMain.h"
#include "imagedrawer.h"


// Base class for all wheel
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

    void writePinPositionsToFile(string absolutFn);

    void destroy();

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
    wheelFromPolyLine(int pinsNumber = 8, ofPolyline poly = ofPolyline());
    void generatePins();

};


class wheelSquare: public wheelFromPolyLine {

public:
    wheelSquare(int pinsNumber, int w = 100);

};



class wheelFromFile: public abstractWheel {

public:
    string filename;
    wheelFromFile(string filename, int w, int h);
    void generatePins();

};





// Same as wheelCircle but add pins passed as parameters
class wheelExtra: public  abstractWheel
{

public:

    float radius;
    ofVec2f center;
    list<ofVec2f> extraPins;

    wheelExtra( int pinsNumber, int w, int h,  std::list<ofVec2f> extraPins);
    void generatePins();

};









// This class a bretton flower

class wheelTribal : public  abstractWheel
{

public:
    float radius;
    ofVec2f center;

    wheelTribal(int pinsNumber=8, int w=100, int h=100 );
    void generatePins();

};





#endif // WHEEL_H
