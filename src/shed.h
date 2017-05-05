#ifndef SHED_H
#define SHED_H

#include "wheel.h"
#include "imagedrawer.h"
#include <climits>

class shed
{
public:

    shed(ofImage oriImg);

    int w ; // width of the image we use
    int h ; // height of the image we use

    ofImage originalImg;
    ofImage originalImgCrop;
    ofImage sketchImg; // use by the algo
    ofImage result;
    ofImage brushedImg; // image use for the left diplay


    float ** mask;  // mask of the preference factor


    ofParameterGroup infoP;
    ofParameter<int> numberStringP; // number of string currentlly in use
    ofParameter<int> diffError;

    ofParameterGroup globalP;
    ofParameter<int> numberPinsP;
    ofParameter<int> maxNumberStringP; // set -1 for infinity

    ofParameterGroup inFlyP;
    ofParameter<int> algoOpacityP;
    ofParameter<int> drawOpacityP;




    abstractWheel wel;
    imageDrawer drawer;

    list<int*> * ** lines;
    list<int> stringPath;

    void setupParameter();

    void setOriginalImgCrop();
    void setSketch();
    void setEmptyResult();



    void setBrushedImg();
    void initializeMask();




    float lineScore(list<int *> l);
    float lineScoreEquilibrate(list<int *> l);
    float lineScoreWeightedExtremity(list<int *> l);
    float lineScoreWeighByMaskFactor(list<int *> l);
    float lineScoreWeighByMaskFactorCumulative(list<int *> l);
    float lineScoreDelta(list<int *> l);
    float lineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);

    int findNextBestPin(int pinIdx);


    void randomifyNextPinAndDrawOneString();
    void computeNextPinAndDrawOneString();
    int currentPinIdx1; // used in computeNextPinAndDrawOneString()
    int nextPinIdx1; // used in computeNextPinAndDrawOneString()


    void decreaseDarkness(list<int *> l, float decreasingV);
    void drawStrings(); // deprecated

    void drawWithBrushOnMask(int x, int y, float ** brushType, int sizeBrush);
    void computeBrushedImg();
    void computeDiffOrignalResult();// differance between the result and the original (counting the lightness differance)



    void setupWheel(abstractWheel wel);

};


#endif // SHED_H
