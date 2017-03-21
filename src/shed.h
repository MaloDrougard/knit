#ifndef SHED_H
#define SHED_H

#include "wheel.h"
#include "imagedrawer.h"
#include <climits>

class shed
{
public:

    int w ; // width of the image we use
    int h ; // height of the image we use

    ofImage originalImg;
    ofImage sketchImg;
    ofImage result;
    ofImage displayImg; // image use for the left diplay

    float ** mask;  // mask of the preference factor


    ofParameterGroup shedParameter;
    ofParameter<int> numberStringP;
    ofParameter<int> numberPinsP;
    ofParameter<int> algoOpacityP;
    ofParameter<int> drawOpacityP;

    ofParameter<bool> startIncrementationP;
    ofParameter<bool> stopIncrementationP;



    int drawOpacityReal;
    int algoOpacityReal;
    int numberStringReal;





    wheel wel;
    imageDrawer drawer;

    list<int*> ** lines;
    list<int> stringPath;

    shed(ofImage oriImg);
    ~shed();

    void trying();

    float lineScore(list<int *> l);

    int findNextBestPin(int pinIdx);



    void computeStringPath();

    void drawString();

    void decreaseDarkness(list<int *> l, float decreasingV);

    void setupParameter();

    void checkchange();
    void initializeLines();
    void setWheel();

    void setSketch();
    void setEmptyResult();
    void setDisplayImg();

    void initializeMask();

    void destroyLine();


    float lineWeightScore(list<int *> l);


    void computeNextPinAndDrawOneString();
    int currentPinIdx1; // used in computeNextPinAndDrawOneString()
    int nextPinIdx1; // used in computeNextPinAndDrawOneString()


    void randomifyNextPinAndDrawOneString();

    void computeLeftDisplayImg();

    void brushMask(int x, int y, float ** brushType, int sizeBrush);
};


#endif // SHED_H
