#ifndef COLORSHED_H
#define COLORSHED_H

#include "ofMain.h"
#include "wheel.h"
#include "imagedrawer.h"

class colorShed
{
public:
    colorShed(ofImage inputImg);


    int w ; // width of the image we use
    int h ; // height of the image we use

    ofImage originalImg;
    ofImage originalImgCrop;
    ofImage result;


    ofParameterGroup infoP;
    ofParameter<int> numberStringP; // number of string currentlly in use
    ofParameter<int> LightnessAbsoluteError;

    ofParameterGroup globalP;
    ofParameter<int> numberPinsP;
    ofParameter<int> maxNumberStringP; // set -1 for infinity


    abstractWheel wel;
    imageDrawer drawer;


    list<int*> * ** lines;
    list<int> stringPath;

    int currentPinIdxRed; // used in computeNextPinAndDrawOneString()
    int currentPinIdxGreen; // used in computeNextPinAndDrawOneString()
    int currentPinIdxBlue; // used in computeNextPinAndDrawOneString()



    void setWandH();
    void setupOfParameter();
    void setOriginalImgCrop();
    void setEmptyResult();
    void setupWheel(abstractWheel wel);


    int computeLightnessAbsoluteError();


    float lineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);

    float greenLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);
    float redLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);
    float blueLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);


    int findNextBestPin(int pinIdx);

    int findNextBestRedPin(int pinIdx);
    int findNextBestGreenPin(int pinIdx);
    int findNextBestBluePin(int pinIdx);

    void computeNextPinAndUpdateResult();

    void computeNextRedPinAndDrawOneString();
    void computeNextGreenPinAndDrawOneString();
    void computeNextBluePinAndDrawOneString();

};

#endif // COLORSHED_H
