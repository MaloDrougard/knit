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
    ofImage originalImgCrop;
    ofImage sketchImg; // use by the algo
    ofImage result;
    ofImage brushedImg; // image use for the left diplay
    ofImage gridImg; //

    float ** mask;  // mask of the preference factor


    ofParameterGroup shedParameter;

    ofParameter<int> numberStringP;
    ofParameter<int> numberPinsP;
    ofParameter<int> algoOpacityP;
    ofParameter<int> drawOpacityP;



    wheel wel;
    imageDrawer drawer;

    list<int*> * ** lines;
    list<int> stringPath;

    shed(ofImage oriImg);

    void setupParameter();

    void setOriginalImgCrop();
    void setSketch();
    void setEmptyResult();
    void setEmptyGridImg();
    void initializeLines();
    void destroyLine();
    void setWheel();
    void setBrushedImg();
    void initializeMask();



    float lineScore(list<int *> l);
    float lineScoreWeighByMaskFactor(list<int *> l);
    float lineScoreWeighByMaskFactorCumulative(list<int *> l);
    float lineScoreDelta(list<int *> l);

    int findNextBestPin(int pinIdx);


    void randomifyNextPinAndDrawOneString();
    void computeNextPinAndDrawOneString();
    int currentPinIdx1; // used in computeNextPinAndDrawOneString()
    int nextPinIdx1; // used in computeNextPinAndDrawOneString()


    void decreaseDarkness(list<int *> l, float decreasingV);



    void drawStrings(); // deprecated


    void drawWithBrushOnMask(int x, int y, float ** brushType, int sizeBrush);
    void computeBrushedImg();

    void drawGridOnImg();


    void restart(int pinsNumber);

};


#endif // SHED_H
