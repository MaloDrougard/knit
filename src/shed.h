#ifndef SHED_H
#define SHED_H

#include "wheel.h"
#include "imagedrawer.h"

class shed
{
public:

    ofImage originalImg;
    ofImage sketchImg;
    ofImage result;


    ofParameterGroup shedParameter;
    ofParameter<int> numberStringP;
    ofParameter<int> numberPinsP;
    ofParameter<int> algoOpacityP;
    ofParameter<int> drawOpacityP;

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
    void destroyLine();
};

#endif // SHED_H
