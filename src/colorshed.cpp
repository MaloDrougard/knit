#include "colorshed.h"



colorShed::colorShed(ofImage inputImg, string imageNameInput) : genericShed(inputImg, imageNameInput)
{
    type = "additiveColor";

    setEmptyResult(ofColor::black);
    setupOfParameter();

    currentPinIdxRed = 0;
    currentPinIdxGreen = 0;
    currentPinIdxBlue = 0;
}


float colorShed::redLineScoreSignedDifferenceBetweenOriginalAndResult( list<int*> l){


    ofColor colorOri;
    ofColor colorRes;

    float redOri;
    float redRes;

    int numberOfPixel = 0;
    float tempScore = 0;
    float score = 0;

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
    {
        colorOri = originalImgCrop.getColor( (*it)[0], (*it)[1] );
        colorRes = result.getColor((*it)[0], (*it)[1] );

        redOri = colorOri.r;
        redRes = colorRes.r;

        tempScore = redOri - redRes;
        score += tempScore;
        numberOfPixel = numberOfPixel + 1;

    }

    score = score / (float) numberOfPixel; // to not advantage long line

    return score;

}


float colorShed::greenLineScoreSignedDifferenceBetweenOriginalAndResult( list<int*> l){


    ofColor colorOri;
    ofColor colorRes;

    float redOri;
    float redRes;

    int numberOfPixel = 0;
    float tempScore = 0;
    float score = 0;

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
    {
        colorOri = originalImgCrop.getColor( (*it)[0], (*it)[1] );
        colorRes = result.getColor((*it)[0], (*it)[1] );

        redOri = colorOri.g;
        redRes = colorRes.g;

        tempScore = redOri - redRes;
        score += tempScore;
        numberOfPixel = numberOfPixel + 1;

    }

    score = score / (float) numberOfPixel; // to not advantage long line

    return score;

}

float colorShed::blueLineScoreSignedDifferenceBetweenOriginalAndResult( list<int*> l){


    ofColor colorOri;
    ofColor colorRes;

    float redOri;
    float redRes;

    int numberOfPixel = 0;
    float tempScore = 0;
    float score = 0;

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
    {
        colorOri = originalImgCrop.getColor( (*it)[0], (*it)[1] );
        colorRes = result.getColor((*it)[0], (*it)[1] );

        redOri = colorOri.b;
        redRes = colorRes.b;

        tempScore = redOri - redRes;
        score += tempScore;
        numberOfPixel = numberOfPixel + 1;

    }

    score = score / (float) numberOfPixel; // to not advantage long line

    return score;

}




void colorShed::computeNextRedPinAndDrawOneString(){

    if ((maxStepsNumberP == -1 ) or (stepsNumberP < maxStepsNumberP )) {

        int nextPinIdxRed = -1;

        nextPinIdxRed = findNextBestPin(currentPinIdxRed, &colorShed::redLineScoreSignedDifferenceBetweenOriginalAndResult );

        // draw the line
        drawer.increasePixels(result, *(wel.lines[currentPinIdxRed][nextPinIdxRed]), ofColor(drawOpacityP ,0,0));

        std::cout << "red step: " << currentPinIdxRed << ":" << nextPinIdxRed << std::endl;

        // update the pin
        currentPinIdxRed = nextPinIdxRed;
    }
}


void colorShed::computeNextGreenPinAndDrawOneString(){

    if ((maxStepsNumberP == -1 ) or (stepsNumberP < maxStepsNumberP )) {

        int nextPinIdx1 = -1;

        nextPinIdx1 = findNextBestPin(currentPinIdxGreen, &colorShed::greenLineScoreSignedDifferenceBetweenOriginalAndResult );

        // draw the line
        drawer.increasePixels(result, *(wel.lines[currentPinIdxGreen][nextPinIdx1]), ofColor(0,drawOpacityP,0));

        std::cout << "green step: " << currentPinIdxGreen << ":" << nextPinIdx1 << std::endl;

        // update the pin
        currentPinIdxGreen = nextPinIdx1;
    }
}



void colorShed::computeNextBluePinAndDrawOneString(){

    if ((maxStepsNumberP == -1 ) or (stepsNumberP < maxStepsNumberP )) {

        int nextPinIdx1 = -1;

        nextPinIdx1 = findNextBestPin(currentPinIdxBlue, &colorShed::blueLineScoreSignedDifferenceBetweenOriginalAndResult );

        // draw the line
        drawer.increasePixels(result, *(wel.lines[currentPinIdxBlue][nextPinIdx1]), ofColor(0,0,drawOpacityP));

        std::cout << "blue step: " << currentPinIdxBlue << ":" << nextPinIdx1 << std::endl;

        // update the pin
        currentPinIdxBlue = nextPinIdx1;
    }
}



int colorShed::findNextBestPin(int pinIdx, float (colorShed::*pScoreFunction)(list<int *>))
{

    float bestScore = INT_MIN;
    float tempScore = 0;

    int bestNextIdx = 0;
    int tempIdx = 0;


    for( int i = 0; i < wel.pinsNumber; i++){
        tempIdx = ( i + pinIdx) % wel.pinsNumber;
        tempScore = (this->*pScoreFunction)(*(wel.lines[pinIdx][tempIdx]));


        if (tempScore > bestScore){
            bestScore = tempScore;
            bestNextIdx = tempIdx;
        }
    }

    return bestNextIdx;

}

void colorShed::computeAndDrawOneStep()
{
    this->computeNextRedPinAndDrawOneString();
    this->computeNextGreenPinAndDrawOneString();
    this->computeNextBluePinAndDrawOneString();

    ++ stepsNumberP;
}


// ----------------------------- SUBSTRACTIVE COLOR SHED -------------------------------------------//


substractiveColorShed::substractiveColorShed(ofImage inputImg, string imageName): genericShed(inputImg, imageName)
{
    type = "substractiveColor";

    setEmptyResult(ofColor::white);
    setupOfParameter();
    currentPinIdxCyan = 0;
    currentPinIdxMagenta = 0;
    currentPinIdxYellow = 0;
}


int substractiveColorShed::findNextBestPin(int pinIdx, float (substractiveColorShed::*pScoreFunction)(list<int *>))
{
    float bestScore = INT_MIN;
    float tempScore = 0;

    int bestNextIdx = 0;
    int tempIdx = 0;


    for( int i = 0; i < wel.pinsNumber; i++){
        tempIdx = ( i + pinIdx) % wel.pinsNumber;
        tempScore = (this->*pScoreFunction)(*(wel.lines[pinIdx][tempIdx]));


        if (tempScore > bestScore){
            bestScore = tempScore;
            bestNextIdx = tempIdx;
        }
    }

    return bestNextIdx;
}


void substractiveColorShed::computeAndDrawOneStep()
{
    this->computeNextCyanPinAndDrawOneString();
    this->computeNextMagentaPinAndDrawOneString();
    this->computeNextYellowPinAndDrawOneString();

    ++ stepsNumberP;
}


float substractiveColorShed::cyanLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l)
{

    ofColor colorOri;
    ofColor colorRes;

    float redOri;
    float redRes;

    float cOri;
    float cRes;

    int numberOfPixel = 0;
    float tempScore = 0;
    float score = 0;

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
    {
        colorOri = originalImgCrop.getColor( (*it)[0], (*it)[1] );
        colorRes = result.getColor((*it)[0], (*it)[1] );

        redOri = colorOri.r;
        redRes = colorRes.r;

        cOri = 1 - (redOri / colorOri.limit() );
        cRes = 1 - (redRes / colorOri.limit() );

        tempScore = cOri - cRes;
        score += tempScore;
        numberOfPixel = numberOfPixel + 1;
    }

    score = score / (float) numberOfPixel; // to not advantage long line

    return score;

}


float substractiveColorShed::magentaLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l)
{

    ofColor colorOri;
    ofColor colorRes;

    float greenOri;
    float greenRes;

    float cOri;
    float cRes;

    int numberOfPixel = 0;
    float tempScore = 0;
    float score = 0;

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
    {
        colorOri = originalImgCrop.getColor( (*it)[0], (*it)[1] );
        colorRes = result.getColor((*it)[0], (*it)[1] );

        greenOri = colorOri.g;
        greenRes = colorRes.g;

        cOri = 1 - (greenOri / colorOri.limit() );
        cRes = 1 - (greenRes / colorOri.limit() );

        tempScore = cOri - cRes;
        score += tempScore;
        numberOfPixel = numberOfPixel + 1;
    }

    score = score / (float) numberOfPixel; // to not advantage long line

    return score;

}


float substractiveColorShed::yellowLineScoreSignedDifferenceBetweenOriginalAndResult(list<int *> l)
{

    ofColor colorOri;
    ofColor colorRes;

    float blueOri;
    float blueRes;

    float yOri;
    float yRes;

    int numberOfPixel = 0;
    float tempScore = 0;
    float score = 0;

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
    {
        colorOri = originalImgCrop.getColor( (*it)[0], (*it)[1] );
        colorRes = result.getColor((*it)[0], (*it)[1] );

        blueOri = colorOri.b;
        blueRes = colorRes.b;

        yOri = 1 - (blueOri / colorOri.limit() );
        yRes = 1 - (blueRes / colorOri.limit() );

        tempScore = yOri - yRes;
        score += tempScore;
        numberOfPixel = numberOfPixel + 1;
    }

    score = score / (float) numberOfPixel; // to not advantage long line

    return score;


}


void substractiveColorShed::computeNextCyanPinAndDrawOneString()
{
    if ((maxStepsNumberP == -1 ) or (stepsNumberP < maxStepsNumberP )) {

        int nextPinIdx1 = -1;

        nextPinIdx1 = findNextBestPin(currentPinIdxCyan, &substractiveColorShed::cyanLineScoreSignedDifferenceBetweenOriginalAndResult );

        // draw the line //  decrease red value -> enforce cyan value
        drawer.decreasePixels(result, *(wel.lines[currentPinIdxCyan][nextPinIdx1]), ofColor(drawOpacityP, 0, 0)); // decrease red parameter give cyan color to apears
        std::cout << "cyan step: " << currentPinIdxCyan << ":" << nextPinIdx1 << std::endl;

        // update the pin
        currentPinIdxCyan = nextPinIdx1;
    }
}


void substractiveColorShed::computeNextMagentaPinAndDrawOneString()
{
    if ((maxStepsNumberP == -1 ) or (stepsNumberP < maxStepsNumberP )) {

        int nextPinIdx1 = -1;

        nextPinIdx1 = findNextBestPin(currentPinIdxMagenta, &substractiveColorShed::magentaLineScoreSignedDifferenceBetweenOriginalAndResult );

        // draw the line
        drawer.decreasePixels(result, *(wel.lines[currentPinIdxMagenta][nextPinIdx1]), ofColor(0, drawOpacityP, 0));
        std::cout << "magenta step: " << currentPinIdxMagenta << ":" << nextPinIdx1 << std::endl;

        // update the pin
        currentPinIdxMagenta = nextPinIdx1;
    }
}


void substractiveColorShed::computeNextYellowPinAndDrawOneString()
{
    if ((maxStepsNumberP == -1 ) or (stepsNumberP < maxStepsNumberP )) {

        int nextPinIdx1 = -1;

        nextPinIdx1 = findNextBestPin(currentPinIdxYellow, &substractiveColorShed::yellowLineScoreSignedDifferenceBetweenOriginalAndResult );

        // draw the line  //
        drawer.decreasePixels(result, *(wel.lines[currentPinIdxYellow][nextPinIdx1]), ofColor(0, 0, drawOpacityP));
        std::cout << "yellow step: " << currentPinIdxYellow << ":" << nextPinIdx1 << std::endl;

        // update the pin
        currentPinIdxYellow = nextPinIdx1;
    }
}


