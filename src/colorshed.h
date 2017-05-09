#ifndef COLORSHED_H
#define COLORSHED_H

#include "ofMain.h"
#include "wheel.h"
#include "genericshed.h"

class colorShed :public genericShed
{
public:
    colorShed(ofImage inputImg, string imageNameInput);

    int currentPinIdxRed;
    int currentPinIdxGreen;
    int currentPinIdxBlue;


    int findNextBestPin(int pinIdx, float (colorShed::*pScoreFunction)(list<int * > ));
    void computeAndDrawOneStep();

    float greenLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);
    float redLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);
    float blueLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);

    void computeNextRedPinAndDrawOneString();
    void computeNextGreenPinAndDrawOneString();
    void computeNextBluePinAndDrawOneString();

};


class substractiveColorShed: public genericShed {

public:

    substractiveColorShed(ofImage inputImg, string imageName);


    int currentPinIdxCyan;
    int currentPinIdxMagenta;
    int currentPinIdxYellow;


    int findNextBestPin(int pinIdx, float (substractiveColorShed::*pScoreFunction)(list<int * > ));
    void computeAndDrawOneStep();

    float cyanLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);
    float magentaLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);
    float yellowLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l);

    void computeNextCyanPinAndDrawOneString();
    void computeNextMagentaPinAndDrawOneString();
    void computeNextYellowPinAndDrawOneString();

};








#endif // COLORSHED_H











