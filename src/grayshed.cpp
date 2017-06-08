#include "grayshed.h"


grayShed::grayShed(ofImage oriImg, string imageName) : genericShed(oriImg, imageName)
{
    type = "grayShed";

    setEmptyResult(ofColor::white);

    setupGrayOfParameters();

    // initialize the sketch image who is use to perform the computation
    setSketch();
    setBrushedImg();
    initialMaskFactor = 0;
    initializeMask(initialMaskFactor);

    currentPinIdx1 = 0;
    nextPinIdx1 = -1;

}


void grayShed::setSketch()
{
    sketchImg.clone(originalImgCrop);
    sketchImg.update();
}

void grayShed::setBrushedImg()
{
    brushedImg.clone(originalImgCrop);
    brushedImg.update();
}

void grayShed::initializeMask(int initValue)
{

    initialMaskFactor = initValue;
    mask = new float * [w];

    for (int i = 0 ; i < w; i++){
        mask[i] = new float [h];
    }

    for (int x = 0; x < w; x++){
        for(int y = 0; y < h; y++){
            mask[x][y] = initialMaskFactor;
        }
    }

}

void grayShed::setupGrayOfParameters()
{


    globalP.setName("Global Algorithm Parameters");
    globalP.add(maxStepsNumberP.set("max #steps", 20000, -1, 40000) );
    inFlyP.setName("In Fly Parameters");
    inFlyP.add(scoreFuntionToUse.set("ScoreFunction",4,1,7));
    inFlyP.add(drawOpacityP.set("Draw Opacity", 9, 0, 255));
    inFlyP.add(algoOpacityP.set("Algo Opacity (only use with some score function", 9, 0, 255));
    infoP.setName("Process infos");
    infoP.add(stepsNumberP.set("#steps",0, 0, 100000));
    int temp = this->computeLightnessAbsoluteError();
    infoP.add(lightnessAbsoluteError.set("absolutError",temp,0,temp+1000));
}








/*
 * Go through a the display image and set the original image if there is no preference factor at these position
*/
void grayShed::computeBrushedImg()
{

    setBrushedImg();

    ofColor color = ofColor::lavender;

    for( int x = 0 ; x < brushedImg.getWidth(); x++ ){
        for ( int y = 0; y < brushedImg.getHeight(); y++ ){
            if (mask[x][y] != initialMaskFactor) {
               brushedImg.setColor(x,y, color);
            }

        }
    }

    brushedImg.update();

}


/*
 *  Draw on mask using the brush given in parameter
 * The brush type must be of the form float **
 *
 * example you can give an array like this:
 *
 *  0 1 0   or  1 1
 *  1 1 1       0 3
 *  0 1 0
 *
 *
 *
*/
void grayShed::drawWithBrushOnMask( int x, int y ,float ** brushType, int sizeBrush){

    int tempIdxX = 0;
    int tempIdxY = 0;

    if ((x > w -1 ) or (y > h -1) or (x < 0) or (y < 0)){
        std::cout << "brush mask must be call for pixel inside the sketch image" << std::endl;
        throw 20;
    }

    int middle = sizeBrush / 2;

    for( int i = 0; i < sizeBrush; i++ ){
        for( int j = 0; j < sizeBrush; j++ ){
            tempIdxX = x - middle + i;
            tempIdxY = y - middle + j;
            if  (!( (tempIdxX < 0) or (tempIdxX > w -1 ) or (tempIdxY < 0 ) or (tempIdxY > h -1 ) ) ) { // border case of the brush
                mask[tempIdxX][tempIdxY] = brushType[i][j];
            }
         }

    }



}




/*
 * Calculate the score of the line based on the darkness
 * More the line is dark on the sketch image, more the score is high
 *
 * math: sum of the darkness / #pixel
 *
 * arg: list of pixel in the line
 * return: the score as float
 *
 *
*/
float grayShed::lineScore( list<int*> l){

    ofColor color;
    float lightness;

    int numberOfPixel = 0;
    float score = 0;

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
    {
        color = sketchImg.getColor( (*it)[0], (*it)[1] );
        lightness = color.getLightness();
        score =  score   + (color.limit() - lightness) ;  // 255 lightness is white
        numberOfPixel = numberOfPixel + 1;

    }

    score = score / (float) numberOfPixel; // to not advantage long line

    return score;

}



// Same as line score, but we shift the darkness to have negative value if the pixel is totaly white
float grayShed::lineScoreEquilibrate( list<int*> l){

    ofColor color;
    float lightness;

    int numberOfPixel = 0;
    float score = 0;
    float tempScore = 0;

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
    {
        color = sketchImg.getColor( (*it)[0], (*it)[1] );
        lightness = color.getLightness();

        tempScore =  ( color.limit() - lightness) -  (color.limit() / 3 ) ;
        score =  score   + tempScore ;

        numberOfPixel = numberOfPixel + 1;

    }

    score = score / (float) numberOfPixel; // to not advantage long line

    return score;

}




float grayShed::lineScoreSignedDifferenceBetweenOriginalAndResult( list<int*> l){

    ofColor colorOri;
    ofColor colorRes;

    float darknessOri;
    float darknessRes;

    int numberOfPixel = 0;
    float tempScore = 0;
    float score = 0;

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
    {
        colorOri = originalImgCrop.getColor( (*it)[0], (*it)[1] );
        colorRes = result.getColor((*it)[0], (*it)[1] );

        darknessOri = colorOri.limit() - colorOri.getLightness();
        darknessRes = colorRes.limit() - colorRes.getLightness();

        tempScore = darknessOri - darknessRes;
        score += tempScore;
        numberOfPixel = numberOfPixel + 1;

    }

    score = score / (float) numberOfPixel; // to not advantage long line

    return score;

}




float grayShed::lineScoreWeightedExtremity( list<int*> l){
// same as lineScoreSignedDiff but give more wheight to the extreme diff


    ofColor colorOri;
    ofColor colorRes;

    float darknessOri;
    float darknessRes;

    int numberOfPixel = 0;
    float score = 0;
    float tempScore = 0;
    float tempFactor = 0;
    float maxFactor = 3;

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
    {

        colorOri = originalImgCrop.getColor( (*it)[0], (*it)[1] );
        colorRes = result.getColor((*it)[0], (*it)[1] );

        darknessOri = colorOri.limit() - colorOri.getLightness();
        darknessRes = colorRes.limit() - colorRes.getLightness();

        tempScore = darknessOri - darknessRes;

        tempFactor = abs(tempScore);
        tempFactor = 1/(colorOri.limit()) * tempFactor;
        tempFactor = pow(maxFactor, tempFactor);
        tempScore = tempScore * tempFactor;


        score =  score   + tempScore ;

        numberOfPixel = numberOfPixel + 1;

    }

    score = score / (float) numberOfPixel; // to not advantage long line

    return score;

}



float grayShed::lineScoreDelta( list<int*> l){

    ofColor color1;
    ofColor color2;

    ofColor colorOriTemp;
    ofColor colorResTemp;

    float lightness1;
    float lightness2;


    int numberOfPixel = 0;
    float score = 0;

    float scoreTemp = 0;

    std::list<int * >::iterator next_it = l.begin();
    next_it++;

    for (std::list<int * >::iterator it = l.begin(); next_it != l.end(); it++)
    {
        colorOriTemp = originalImgCrop.getColor( (*it)[0], (*it)[1] );
        colorResTemp = result.getColor((*it)[0], (*it)[1] );

        lightness1 = colorOriTemp.getLightness() - colorResTemp.getLightness();

        colorOriTemp = originalImgCrop.getColor( (*next_it)[0], (*next_it)[1] );
        colorResTemp = result.getColor((*next_it)[0], (*next_it)[1] );

        lightness2 = colorOriTemp.getLightness() - colorResTemp.getLightness();

        scoreTemp =  color1.limit() - abs(lightness1 - lightness2) ;  // 255 lightness is white
        score += scoreTemp;

        numberOfPixel = numberOfPixel + 1;
        next_it++;

    }

    score = score / (float) numberOfPixel; // to not advantage long line

    return score;

}



float grayShed::lineScoreWeighByMaskFactor( list<int*> l){

    ofColor colorOri;
    ofColor colorRes;

    float darknessOri;
    float darknessRes;

    int numberOfPixel = 0;
    float tempScore = 0;
    float score = 0;

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
    {
        colorOri = originalImgCrop.getColor( (*it)[0], (*it)[1] );
        colorRes = result.getColor((*it)[0], (*it)[1] );

        darknessOri = colorOri.limit() - colorOri.getLightness();
        darknessRes = colorRes.limit() - colorRes.getLightness();

        tempScore = darknessOri - darknessRes;

        tempScore = tempScore * mask[(*it)[0]][(*it)[1]];

        score += tempScore;
        numberOfPixel = numberOfPixel + 1;

    }

    score = score / (float) numberOfPixel; // to not advantage long line

    return score;


}

float grayShed::lineScoreRandom(list<int *> l)
{

    return rand() % 100 ;


}


int grayShed::findNextBestPin(int pinIdx){
// go through all the pins and determine the next best one
// the Score function only need to be positive monotone


    float bestScore = INT_MIN;
    float tempScore = 0;

    int bestNextIdx = 0;
    int tempIdx = 0;


    for( int i = 0; i < wel.pinsNumber; i++){
        tempIdx = ( i + pinIdx) % wel.pinsNumber;
        tempScore = (this->*pScoreFunctionInUse)(*(wel.lines[pinIdx][tempIdx]));


        if (tempScore > bestScore){
            bestScore = tempScore;
            bestNextIdx = tempIdx;
        }
    }

    return bestNextIdx;

}






void grayShed::computeAndDrawOneStep()
{

    // set the score function to use depending on the ScoreFunctionP paramter
    switch (scoreFuntionToUse ) {

        case 1:
            pScoreFunctionInUse = &grayShed::lineScore;
                break;
        case 2:
            pScoreFunctionInUse = &grayShed::lineScoreEquilibrate;
            break;
        case 3:
            pScoreFunctionInUse = &grayShed::lineScoreDelta;
            break;
        case 4:
            pScoreFunctionInUse = &grayShed::lineScoreSignedDifferenceBetweenOriginalAndResult;
            break;
        case 5:
            pScoreFunctionInUse = &grayShed::lineScoreWeightedExtremity;
            break;
        case 6:
            pScoreFunctionInUse = &grayShed::lineScoreWeighByMaskFactor;
            break;
         case 7:
            pScoreFunctionInUse = &grayShed::lineScoreRandom;
            break;

    }


    if ((maxStepsNumberP == -1 ) or (stepsNumberP < maxStepsNumberP )) {

        nextPinIdx1 = findNextBestPin(currentPinIdx1);

        // increase the value of the pixel that are under the line in sketch image (in case we use this lineScore function)
        drawer.increasePixels(sketchImg, *(wel.lines[currentPinIdx1][nextPinIdx1]), ofColor(algoOpacityP, algoOpacityP, algoOpacityP));

        // draw the line on result
        drawer.decreasePixels(result, *(wel.lines[currentPinIdx1][nextPinIdx1]), ofColor(drawOpacityP,drawOpacityP,drawOpacityP));

        std::cout << "step: " << currentPinIdx1 << ":" << nextPinIdx1 << std::endl;

        // update the pin
        currentPinIdx1 = nextPinIdx1;

        stepsNumberP++;

    }

}














