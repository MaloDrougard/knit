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

    wheel wel;
    imageDrawer drawer;

    list<int*> ** lines;

    int numberString;
    list<int> stringPath;

    shed(ofImage oriImg);
    ~shed();

    void trying();

    float lineScore(list<int *> l);

    int findNextBestPin(int pinIdx);

    void computeStringPath();

    void drawString();

    void decreaseDarkness(list<int *> l, float decreasingV);
};

#endif // SHED_H
