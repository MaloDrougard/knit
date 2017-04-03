#include "shed.h"


shed::shed(ofImage oriImg)
{



    originalImg = oriImg;
    setOriginalImgCrop();


    w = originalImgCrop.getWidth();
    h = originalImgCrop.getHeight();

    setEmptyResult();

    // calulate the "error" between the two image
    computeDiffOrignalResult();

    setupParameter();


    // initialize the sketch image who is use to perform the computation
    setSketch();
    setBrushedImg();

    setEmptyGridImg();
    initializeMask();


    // initialize drawer, it's a utility tool to access image
    drawer = imageDrawer();



    currentPinIdx1 = 0;
    nextPinIdx1 = -1;

}

// differ the setup from the init to get a chance to adjust parameters :)
void shed::setup(){

    // set wheel that contains pins position
    setWheel();

    // set lines who represent all the strings possibilities betweens pins
    initializeLines();

}


// here we use the grid with a central pin also!
// differ the setup from the init to get a chance to adjust parameters :)
void shed::setup2(){

    // set wheel that contains pins position
    ofVec2f centerWheel = ofVec2f( w/2 , w/2 );
    float radius = (w-1)/2.0 ;    // we want not to be at border but inside

    ofVec2f tempA[] = {ofVec2f(400, 500), ofVec2f(500, 400), ofVec2f(300, 500), ofVec2f(200, 400)};
    list<ofVec2f> tempL (tempA, tempA+4);
    this->wel = wheelExtra(numberPinsP, radius, centerWheel, tempL);
    // set lines who represent all the strings possibilities betweens pins

    initializeLines();



}


shed::~shed()
{
    wel.deletePins();
    destroyLine();
}


void shed::setupParameter(){

    // divide setup in start ans onFly

    globalP.setName("Global Algorithm Parameters");
    globalP.add(numberPinsP.set("#pins",240, 4, 1200));
    globalP.add(maxNumberStringP.set("max #strings", 19002, -1, 20000) );

    inFlyP.setName("In Fly Algorithm");
    inFlyP.add(algoOpacityP.set("algo opacity",9,0,255));
    inFlyP.add(drawOpacityP.set("draw opacity",9,0,255));

    infoP.setName("Process infos");
    infoP.add(numberStringP.set("#strings",0, 0, 20000));
    int temp = diffError; // because we allready set the value
    infoP.add(diffError.set("error",temp,0,temp+1000));
}






/*
 * Crop the original Img if needed
*/
void shed::setOriginalImgCrop(){

    int w = originalImg.getWidth();
    int h = originalImg.getHeight();

    originalImgCrop.clone(originalImg);

    int diff = 0;
    if( w > h ){
        diff = w - h;
        originalImgCrop.crop(diff/2, 0, h, h);
    } else {
        diff = h - w;
        originalImgCrop.crop(0, diff/2,  w, w);
    }

    originalImgCrop.update();
}


void shed::setSketch()
{
    sketchImg.clone(originalImgCrop);
    sketchImg.update();
}

void shed::setEmptyResult()
{
    result.allocate(w, h , OF_IMAGE_COLOR);
    result.setColor(ofColor::white);
    result.update();
}

void shed::setEmptyGridImg()
{
    gridImg.allocate(w, h , OF_IMAGE_COLOR);
    gridImg.setColor(ofColor::white);
    gridImg.update();

}

void shed::setBrushedImg()
{
    brushedImg.clone(originalImgCrop);
    brushedImg.update();
}


/*
 * Draw the grid on gridImg
 */
void shed::drawGridOnImg()
{

    list<int *> l;


    for(int i = 0; i < wel.pinsNumber; i ++ ){
        for(int j = 0; j < wel.pinsNumber; j++){

            l = *(lines[i][j]);
            drawer.decreasePixels(gridImg, l, ofColor(2,2,2));
        }
    }

    gridImg.update();

}

void shed::computeDiffOrignalResult()
{
    int diff  = 0;
    int tempLigthnessDiff = 0;

    for(int x = 0; x < w; x++ ){
        for(int y = 0; y < h; y++){
               tempLigthnessDiff = (originalImgCrop.getColor(x,y)).getLightness() - (result.getColor(x,y)).getLightness();
               tempLigthnessDiff = abs(tempLigthnessDiff);
               diff += tempLigthnessDiff;
        }
    }

    diffError.set(diff);
}



void shed::initializeMask()
{

    mask = new float * [w];

    for (int i = 0 ; i < w; i++){
        mask[i] = new float [h];
    }

    for (int x = 0; x < w; x++){
        for(int y = 0; y < h; y++){
            mask[x][y] = 1;
        }
    }

}

void shed::setWheel(){

    ofVec2f centerWheel = ofVec2f( w/2 , w/2 );
    float radius = (w-1)/2.0 ;    // we want not to be at border but inside

    this->wel = wheel(numberPinsP, radius, centerWheel);
}

void shed::initializeLines(){

    list<int*> * tempL;

    // initializing lines
    lines = new list<int*> * * [wel.pinsNumber];

    for (int i = 0; i < wel.pinsNumber; i++) {
        lines[i] = new list<int*> * [wel.pinsNumber];
    }

    for(int i = 0; i < wel.pinsNumber; i ++ ){
        for(int j = 0; j < wel.pinsNumber; j++){

            tempL = new list<int*>;
            lines[i][j] = tempL;

            if ( i != j){
                drawer.getPixelIdxOfALineDDAAlgo(tempL, sketchImg, wel.pins[i], wel.pins[j]);
            }

        }

    }
}

void shed::destroyLine(){

    // FIXME half the first time work correctly but not the second ???

    list<int * > * tempL;

    for( int x = 0; x < wel.pinsNumber; x++)
    {
        for( int y= 0; y < wel.pinsNumber; y++){
            tempL = lines[x][y];
            drawer.freeListOf2Int(tempL);
            delete tempL;
        }
    }

    for (int i = 0; i < wel.pinsNumber; i++ ){ // is it correct ? needed?
         delete[] lines[i] ;
    }
    delete[] lines;

}




// decrease the lightness of the pixel who index are contain in l
void shed::decreaseDarkness(list<int*> l, float decreasingV) {

    ofColor color(decreasingV,decreasingV,decreasingV);
    drawer.incrementPixels(sketchImg, l, color);

    sketchImg.update();

}


// return the lightness score adaptation of the pixel contain in l
float shed::lineScore( list<int*> l){

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

float shed::lineScoreDelta( list<int*> l){

    ofColor color1;
    ofColor color2;

    float lightness1;
    float lightness2;


    int numberOfPixel = 0;
    float score = 0;

    float scoreTemp = 0;

    std::list<int * >::iterator next_it = l.begin();
    next_it++;

    for (std::list<int * >::iterator it = l.begin(); next_it != l.end(); it++)
    {
        color1 = sketchImg.getColor( (*it)[0], (*it)[1] );
        lightness1 = color1.getLightness();

        color2 = sketchImg.getColor( (*next_it)[0], (*next_it)[1] );
        lightness2 = color2.getLightness();

        scoreTemp =  color1.limit() - abs(lightness1 - lightness2) ;  // 255 lightness is white
        score += scoreTemp;

        numberOfPixel = numberOfPixel + 1;
        next_it++;

    }

    score = score / (float) numberOfPixel; // to not advantage long line

    return score;

}



// return the lightness score adaptation of the pixel contain in l
float shed::lineScoreWeighByMaskFactor( list<int*> l){



        ofColor color;
        float lightness;

        int numberOfPixel = 0;
        float score = 0;
        float tempScore = 0;

        for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
        {
            color = sketchImg.getColor( (*it)[0], (*it)[1] );
            lightness = color.getLightness();

            // calculate score using negative value for white and positive value for black
            tempScore =  ( color.limit() - lightness) -  (color.limit() / 3 ) ;
            // multiply by the mask factor
            tempScore *= mask[(*it)[0]][(*it)[1]];

            score += tempScore;
            numberOfPixel = numberOfPixel + 1;

        }

        score = score / (float) numberOfPixel; // to not advantage long line

        return score;

}


// return the lightness score adaptation of the pixel contain in l
float shed::lineScoreWeighByMaskFactorCumulative( list<int*> l){



        ofColor color;
        float lightness;

        int numberOfPixel = 0;
        float score = 0;
        float tempScore = 0;

        for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
        {
            color = sketchImg.getColor( (*it)[0], (*it)[1] );
            lightness = color.getLightness();

            // calculate score using negative value for white and positive value for black
            tempScore =  ( color.limit() - lightness)  ;
            // multiply by the mask factor
            tempScore *= mask[(*it)[0]][(*it)[1]];

            score += tempScore;
            numberOfPixel = numberOfPixel + 1;

        }

        score = score / (float) numberOfPixel; // to not advantage long line

        return score;

}


int shed::findNextBestPin(int pinIdx){
// go through all the pins and determine the next best one
// the Score function only need to be positive monotone


    float bestScore = INT_MIN;
    float tempScore = 0;

    int bestNextIdx = 0;
    int tempIdx = 0;


    for( int i = 0; i < wel.pinsNumber; i++){
        tempIdx = ( i + pinIdx) % wel.pinsNumber;
        tempScore = lineScoreWeighByMaskFactor(*(lines[pinIdx][tempIdx]));


        if (tempScore > bestScore){
            bestScore = tempScore;
            bestNextIdx = tempIdx;
        }
    }

    return bestNextIdx;

}




void shed::computeNextPinAndDrawOneString(){

    if ((maxNumberStringP == -1 ) or (numberStringP < maxNumberStringP )) {

        float decreaseV = algoOpacityP;
        int opacity = drawOpacityP;

        nextPinIdx1 = findNextBestPin(currentPinIdx1);

        // decrease the value of the pixel that are under the line
        decreaseDarkness(*(lines[currentPinIdx1][nextPinIdx1]), decreaseV);

        // draw the line
        drawer.decreasePixels(result, *(lines[currentPinIdx1][nextPinIdx1]), ofColor(opacity,opacity,opacity));

        std::cout << "step: " << currentPinIdx1 << ":" << nextPinIdx1 << std::endl;

        numberStringP++;


        // save path
        stringPath.push_back(currentPinIdx1);

        // update the pin
        currentPinIdx1 = nextPinIdx1;
    }
}

void shed::randomifyNextPinAndDrawOneString(){


    if ((maxNumberStringP == -1 ) or (numberStringP < maxNumberStringP )) {

            float decreaseV = algoOpacityP;
            int opacity = drawOpacityP;


            nextPinIdx1 = rand() % numberPinsP ;

            // decrease the value of the pixel that are under the line
            decreaseDarkness(*(lines[currentPinIdx1][nextPinIdx1]), decreaseV);

            // draw the line
            drawer.decreasePixels(result, *(lines[currentPinIdx1][nextPinIdx1]), ofColor(opacity,opacity,opacity));
            numberStringP++;

            // save path
            stringPath.push_back(currentPinIdx1);

            // update the pin
            currentPinIdx1 = nextPinIdx1;
    }

}






void shed::drawStrings(){

    result.allocate(sketchImg.getWidth(), sketchImg.getHeight(), OF_IMAGE_COLOR);
    result.setColor(ofColor::white);

    int opacity = drawOpacityP;

    std::list<int>::iterator next_it = stringPath.begin();
    next_it++;

    for (std::list<int>::iterator it = stringPath.begin();  next_it  != stringPath.end()  ; it++)
    {
        drawer.decreasePixels(result, *(lines[*it][*next_it]), ofColor(opacity,opacity,opacity));
        next_it++;

    }

}


/*
 * Go through a the display image and set the original image if there is no preference factor at these position
*/
void shed::computeBrushedImg()
{

    setBrushedImg();

    ofColor color = ofColor::lavender;

    for( int x = 0 ; x < brushedImg.getWidth(); x++ ){
        for ( int y = 0; y < brushedImg.getHeight(); y++ ){
            if (mask[x][y] != 1) {
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
void shed::drawWithBrushOnMask( int x, int y ,float ** brushType, int sizeBrush){

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
                mask[tempIdxX][tempIdxY] *= brushType[i][j];
            }
         }

    }



}

















