#ifndef IMAGEDRAWER_H
#define IMAGEDRAWER_H

#include "ofMain.h"


class imageDrawer
{
public:
    imageDrawer();


    bool isPointOnLine(ofVec2f p1, ofVec2f startLine, ofVec2f endLine, float tolerance);

    void drawALine(ofImage& img, ofVec2f l1, ofVec2f l2, float tolerance = 0.5);

    void drawPins(ofImage &img, ofVec2f* pins, int pinsNumber);

    list<int *> getPixelIdxOfALine(ofImage &img, ofVec2f l1, ofVec2f l2, float tolerence = 0.5);

    void printListIdx(list<int *> l);

    void drawPixels(ofImage &img, list<int *>, ofColor color = ofColor(23,56,77));

    list<int *> getPixelIdxOfALineDDAAlgo(ofImage &img, ofVec2f l1, ofVec2f l2);
};

#endif // IMAGEDRAWER_H
