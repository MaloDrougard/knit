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
    virtual void setEmptyResult();
    void setupWheel(abstractWheel wel);


    int computeLightnessAbsoluteError();


    int findNextBestPin(int pinIdx, float (colorShed::*pScoreFunction)(list<int * > ));

    virtual void computeNextStepAndDrawThreeStrings();

    float greenLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);
    float redLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);
    float blueLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);

    void computeNextRedPinAndDrawOneString();
    void computeNextGreenPinAndDrawOneString();
    void computeNextBluePinAndDrawOneString();

};


class substractiveColorShed: public colorShed {

public:

    substractiveColorShed(ofImage inputImg);

    void setEmptyResult();


    int findNextBestPin(int pinIdx, float (substractiveColorShed::*pScoreFunction)(list<int * > ));
    void computeNextStepAndDrawThreeStrings();

    float cyanLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);
    float magentaLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);
    float yellowLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);

    void computeNextCyanPinAndDrawOneString();
    void computeNextMagentaPinAndDrawOneString();
    void computeNextYellowPinAndDrawOneString();

};










#endif // COLORSHED_H











