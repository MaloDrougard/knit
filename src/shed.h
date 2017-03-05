#ifndef SHED_H
#define SHED_H

#include "wheel.h"
#include "imagedrawer.h"

class shed
{
public:

    ofImage originalImg;
    ofImage sketchImg;

    wheel wel;
    imageDrawer drawer;

    list<int*> ** lines;

    shed(ofImage oriImg);
    ~shed();

    void trying();


};

#endif // SHED_H
