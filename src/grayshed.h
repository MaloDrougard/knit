#ifndef SHED_H
#define SHED_H


#include "genericshed.h"

class grayShed : public genericShed
{
public:

    grayShed(ofImage oriImg, string imageName);


    ofImage sketchImg; // use by the algo
    ofImage brushedImg; // image use for the left diplay
    float ** mask;  // mask of the preference factor
    ofParameter<int> algoOpacityP; // used by lineScore, lineScoreEquilibrate, l

    ofParameter<int> scoreFuntionToUse;


    int currentPinIdx1; // used in computeNextPinAndDrawOneString()
    int nextPinIdx1; // used in computeNextPinAndDrawOneString()


    void setSketch();
    void setBrushedImg();
    void initializeMask();
    void setupGrayOfParameters(); // set the parameters relativily to this shed


    void drawWithBrushOnMask(int x, int y, float ** brushType, int sizeBrush);
    void computeBrushedImg();


    // possible line score function
    float lineScore(list<int *> l);  // use sketchImg // 1
    float lineScoreEquilibrate(list<int *> l);  // use sketchImg  //2
    float lineScoreDelta(list<int *> l);  // strange    //3
    float lineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);    //4
    float lineScoreWeightedExtremity(list<int *> l);    //5
    float lineScoreWeighByMaskFactor(list<int *> l);  // use mask    //6
    float lineScoreRandom(list<int *> l);    //7


    float (grayShed::*pScoreFunctionInUse)(list<int * > ); // this is the score function use by findNextBestPin ( take a look to pointer to member function, if you don't know this grammar)

    int findNextBestPin(int pinIdx);
    void computeAndDrawOneStep();


};


#endif // SHED_H
