#include "colorshed.h"



colorShed::colorShed(ofImage inputImg)
{
    originalImg = inputImg;
    setOriginalImgCrop();
    setWandH();
    setEmptyResult();
    setupOfParameter();
    currentPinIdxRed = 0;
    currentPinIdxGreen = 0;
    currentPinIdxBlue = 0;
}


void colorShed::setWandH()
{
    if ( ! originalImgCrop.isAllocated()  ){
        std::cerr << "Error in colorShed: to call setupWandH originalImgCrop must be allocate. \nUse setOriginalImgCrop()" << std::endl;
        throw -7;
    }
    w = originalImgCrop.getWidth();
    h = originalImgCrop.getHeight();

}

void colorShed::setupOfParameter()
{

    globalP.setName("Global Algorithm Parameters");
    globalP.add(numberPinsP.set("#pins",240, 4, 1200));
    globalP.add(maxNumberStringP.set("max #strings", 30003, -1, 20000) );

    infoP.setName("Process infos");
    infoP.add(numberStringP.set("#strings",0, 0, 20000));

    int temp = this->computeLightnessAbsoluteError();
    infoP.add(LightnessAbsoluteError.set("error",temp,0,temp+1000));
}


void colorShed::setOriginalImgCrop()
{
    int oriW = originalImg.getWidth();
    int oriH = originalImg.getHeight();

    originalImgCrop.clone(originalImg);

    int diff = 0;
    if( oriW > oriH ){
        diff = oriW - oriH;
        originalImgCrop.crop(diff/2, 0, oriH, oriH);
    } else {
        diff = oriH - oriW;
        originalImgCrop.crop(0, diff/2,  oriW, oriW);
    }
    originalImgCrop.update();
}

void colorShed::setEmptyResult()
{
    result.allocate(w, h , OF_IMAGE_COLOR);
    result.setColor(ofColor::black);
    result.update();

}


void colorShed::setupWheel(abstractWheel wel)
{

    // becarful lines is create using *new* keyword
    // It'is the responsability of the caller of this function to destroy lines at the right moment

    this->wel = wel;
    this->lines = wel.lines;

}

int colorShed::computeLightnessAbsoluteError()
{
    if  ( (!originalImgCrop.isAllocated()) or (!result.isAllocated()) ) {
        std::cerr << "Error in colorShep: To call computeLightnessAbsoluteError() originalImgCrop and result image must be allocate";
        throw -7;
    }


    int diff  = 0;
    int tempLigthnessDiff = 0;

    for(int x = 0; x < w; x++ ){
        for(int y = 0; y < h; y++){
               tempLigthnessDiff = (originalImgCrop.getColor(x,y)).getLightness() - (result.getColor(x,y)).getLightness();
               tempLigthnessDiff = abs(tempLigthnessDiff);
               diff += tempLigthnessDiff;
        }
    }

    LightnessAbsoluteError.set(diff);

    return diff;

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




int colorShed::findNextBestRedPin(int pinIdx){
// go through all the pins and determine the next best one
// the Score function only need to be positive monotone


    float bestScore = INT_MIN;
    float tempScore = 0;

    int bestNextIdx = 0;
    int tempIdx = 0;


    for( int i = 0; i < wel.pinsNumber; i++){
        tempIdx = ( i + pinIdx) % wel.pinsNumber;
        tempScore = redLineScoreSignedDifferenceBetweenOriginalAndResult(*(lines[pinIdx][tempIdx]));


        if (tempScore > bestScore){
            bestScore = tempScore;
            bestNextIdx = tempIdx;
        }
    }

    return bestNextIdx;

}


int colorShed::findNextBestGreenPin(int pinIdx){
// go through all the pins and determine the next best one
// the Score function only need to be positive monotone


    float bestScore = INT_MIN;
    float tempScore = 0;

    int bestNextIdx = 0;
    int tempIdx = 0;


    for( int i = 0; i < wel.pinsNumber; i++){
        tempIdx = ( i + pinIdx) % wel.pinsNumber;
        tempScore = greenLineScoreSignedDifferenceBetweenOriginalAndResult(*(lines[pinIdx][tempIdx]));


        if (tempScore > bestScore){
            bestScore = tempScore;
            bestNextIdx = tempIdx;
        }
    }

    return bestNextIdx;

}




int colorShed::findNextBestBluePin(int pinIdx){
// go through all the pins and determine the next best one
// the Score function only need to be positive monotone


    float bestScore = INT_MIN;
    float tempScore = 0;

    int bestNextIdx = 0;
    int tempIdx = 0;


    for( int i = 0; i < wel.pinsNumber; i++){
        tempIdx = ( i + pinIdx) % wel.pinsNumber;
        tempScore = blueLineScoreSignedDifferenceBetweenOriginalAndResult(*(lines[pinIdx][tempIdx]));


        if (tempScore > bestScore){
            bestScore = tempScore;
            bestNextIdx = tempIdx;
        }
    }

    return bestNextIdx;

}



void colorShed::computeNextRedPinAndDrawOneString(){

    if ((maxNumberStringP == -1 ) or (numberStringP < maxNumberStringP )) {

        int nextPinIdxRed = -1;

        float decreaseV = 9;
        int opacity = 9;

        nextPinIdxRed = findNextBestRedPin(currentPinIdxRed );

        // draw the line
        drawer.increasePixels(result, *(lines[currentPinIdxRed][nextPinIdxRed]), ofColor(opacity,0,0));

        std::cout << "red step: " << currentPinIdxRed << ":" << nextPinIdxRed << std::endl;

        numberStringP++;


        // save path
        stringPath.push_back(currentPinIdxRed);

        // update the pin
        currentPinIdxRed = nextPinIdxRed;
    }
}


void colorShed::computeNextGreenPinAndDrawOneString(){

    if ((maxNumberStringP == -1 ) or (numberStringP < maxNumberStringP )) {

        int nextPinIdx1 = -1;

        float decreaseV = 9;
        int opacity = 9;

        nextPinIdx1 = findNextBestGreenPin(currentPinIdxGreen);

        // draw the line
        drawer.increasePixels(result, *(lines[currentPinIdxGreen][nextPinIdx1]), ofColor(0,opacity,0));

        std::cout << "green step: " << currentPinIdxGreen << ":" << nextPinIdx1 << std::endl;

        numberStringP++;


        // save path
        stringPath.push_back(currentPinIdxGreen);

        // update the pin
        currentPinIdxGreen = nextPinIdx1;
    }
}



void colorShed::computeNextBluePinAndDrawOneString(){

    if ((maxNumberStringP == -1 ) or (numberStringP < maxNumberStringP )) {

        int nextPinIdx1 = -1;

        float decreaseV = 9;
        int opacity = 9;

        nextPinIdx1 = findNextBestBluePin(currentPinIdxBlue);

        // draw the line
        drawer.increasePixels(result, *(lines[currentPinIdxBlue][nextPinIdx1]), ofColor(0,0,opacity));

        std::cout << "blue step: " << currentPinIdxBlue << ":" << nextPinIdx1 << std::endl;

        numberStringP++;


        // save path
        stringPath.push_back(currentPinIdxBlue);

        // update the pin
        currentPinIdxBlue = nextPinIdx1;
    }
}
