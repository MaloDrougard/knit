#include "shed.h"


shed::shed(ofImage oriImg)
{
    setupParameter();

    originalImg = oriImg;

    // initialize the sketch image who is use to perform the computation
    setSketch();

    // initialize drawer, it's a utility tool to access image
    drawer = imageDrawer();

    // set wheel that contains pins position
    setWheel();

    // set lines who represent all the strings possibilities betweens pins
    initializeLines();


    setEmptyResult();
    currentPinIdx1 = 0;
    nextPinIdx1 = -1;

}

void shed::setSketch(){

    int w = originalImg.getWidth();
    int h = originalImg.getHeight();

    sketchImg.clone(originalImg);

    int diff = 0;
    if( w > h ){
        diff = w - h;
        sketchImg.crop(diff/2, 0, h, h);
    } else {
        diff = h - w;
        sketchImg.crop(0, diff/2,  w, w);
    }

    sketchImg.update();
}

void shed::setEmptyResult(){

    result.allocate(sketchImg.getWidth(), sketchImg.getHeight(), OF_IMAGE_COLOR);
    result.setColor(ofColor::white);

}

void shed::setWheel(){
    int w = sketchImg.getWidth();
    int h = sketchImg.getHeight();


    ofVec2f centerWheel = ofVec2f( w/2 , w/2 );
    float radius = (w-1)/2.0 ;    // we want not to be at border but inside

    wel = wheel(numberPinsP, radius, centerWheel);
}


void shed::initializeLines(){

    // initializing lines
    lines = new list<int*> * [wel.pinsNumber];
    for (int i = 0; i < wel.pinsNumber; i++) {
        lines[i] = new list<int*> [wel.pinsNumber];
    }

    for(int i = 0; i < wel.pinsNumber; i ++ ){
        for(int j = 0; j < wel.pinsNumber; j++){

            if ( i != j){
                lines[i][j] = drawer.getPixelIdxOfALineDDAAlgo(sketchImg, wel.pins[i], wel.pins[j]);
            } else {
                lines[i][j] = * (new list<int *>);  //case where the line is not well definite
            }

        }

    }
}

void shed::destroyLine(){


    int * temp;
    // FIXME

    for( int x = 0; x < wel.pinsNumber; x++)
    {
        for( int y= 0; y < wel.pinsNumber; y++){
            list<int * > l = lines[x][y];

            while (! l.empty()) {
                temp = l.front();
                delete [] temp;
                l.pop_front();

            }

        }
    }

    for (int i = 0; i < wel.pinsNumber; i++ ){ // is it correct ? needed?
         delete [] lines[i] ;
    }
    delete [] lines;

}



void shed::setupParameter(){

    shedParameter.setName("Shed Parameters");
    shedParameter.add(numberStringP.set("#strings",0, 0, 20000));
    shedParameter.add(numberPinsP.set("#pins",360, 4, 1200));
    shedParameter.add(algoOpacityP.set("algo opacity",56,0,255));
    shedParameter.add(drawOpacityP.set("draw opacity",36,0,255));

    shedParameter.add(stopIncrementationP.set("stop drawing", false));


    numberStringReal = numberStringP;
    algoOpacityReal = algoOpacityP;
}

void shed::checkchange(){

    if(numberPinsP != wel.pinsNumber){
        destroyLine();
        setWheel();
        setSketch();
        initializeLines();
        computeStringPath();
        drawString();
    }

    if(numberStringReal != numberStringP){
        numberStringReal = numberStringP;
        setSketch();
        computeStringPath();
        drawString();
    }

    if(algoOpacityReal != algoOpacityP){
        algoOpacityReal = algoOpacityP;
        setSketch();
        computeStringPath();
        drawString();
    }

    if(drawOpacityReal != drawOpacityP){
        drawOpacityReal = drawOpacityP;
        drawString();
    }


}

shed::~shed(){

    for (int i = 0; i < wel.pinsNumber; i++ ){ // is it correct ? needed?
         delete [] lines[i] ;
    }
    delete [] lines;

}

void shed::trying()
{

    for (int i = 0; i < wel.pinsNumber; i++ ){

            drawer.drawPixels(sketchImg, lines[i][3]);

    }

    for (int i = 0; i < wel.pinsNumber; i++ ){

            drawer.drawPixels(sketchImg, lines[i][207]);

    }

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




float shed::lineWeightScore(list<int*> l){
    // wheigt the importance of pixel  relativly to the center

    ofColor color;
    float lightness;

    int numberOfPixel = 0;
    float score = 0;

    int w = sketchImg.getWidth();
    int h = sketchImg.getHeight();
    float diagonal = sqrt(pow(w/2.0, 2) + pow(h/2.0, 2)); // should be the max norm

    float dx = -1;
    float dy = -1;
    float norm = -1;
    float factor = -1;



    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
    {
        color = sketchImg.getColor( (*it)[0], (*it)[1] );
        // get deltas of center
        dx = (*it)[0] - (w/2.0) ;
        dy = (*it)[1] - (w/2.0);
        // compute norm from the center to the pixel position
        norm = sqrt(pow(dx, 2) + pow(dy, 2));
        factor = diagonal - norm;
        // compute the standard score from lightness
        lightness = color.getLightness();
        score =  score   + factor * (color.limit() - lightness) ;  //  weight the score

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
        tempScore = lineScore(lines[pinIdx][tempIdx]);


        if (tempScore > bestScore){
            bestScore = tempScore;
            bestNextIdx = tempIdx;
        }
    }

    return bestNextIdx;

}

// decrease the lightness of the pixel who index are contain in l
void shed::decreaseDarkness(list<int*> l, float decreasingV) {

    ofColor color(decreasingV,decreasingV,decreasingV);
    drawer.incrementPixels(sketchImg, l, color);

    sketchImg.update();

}


void shed::computeStringPath(){

    float decreaseV = algoOpacityReal;

    stringPath.clear();
    stringPath.push_back(0);

    int currentPinIdx = 0;
    int nextPinIdx = -1;

    for( int i = 0 ; i< numberStringReal; i++ ){
        nextPinIdx = findNextBestPin(currentPinIdx);
        // decrease the value of the pixel that are under the line

        decreaseDarkness(lines[currentPinIdx][nextPinIdx], decreaseV);

        stringPath.push_back(nextPinIdx);
        currentPinIdx = nextPinIdx;
    }

}


void shed::drawString(){

    result.allocate(sketchImg.getWidth(), sketchImg.getHeight(), OF_IMAGE_COLOR);
    result.setColor(ofColor::white);

    int opacity = drawOpacityReal;

    std::list<int>::iterator next_it = stringPath.begin();
    next_it++;

    for (std::list<int>::iterator it = stringPath.begin();  next_it  != stringPath.end()  ; it++)
    {
        drawer.decreasePixels(result, lines[*it][*next_it], ofColor(opacity,opacity,opacity));
        next_it++;

    }

}



void shed::computeNextPinAndDrawOneString(){


    std::cout << "  we are inside drawOne "<< std::endl;
    if (! stopIncrementationP){


        float decreaseV = algoOpacityP;
        int opacity = drawOpacityP;


        nextPinIdx1 = findNextBestPin(currentPinIdx1);

        // decrease the value of the pixel that are under the line
        decreaseDarkness(lines[currentPinIdx1][nextPinIdx1], decreaseV);

        // draw the line
        drawer.decreasePixels(result, lines[currentPinIdx1][nextPinIdx1], ofColor(opacity,opacity,opacity));



        std::cout << "      we are computing in drawOne.    currentPin: "<<  currentPinIdx1 << ", nextPin: " << nextPinIdx1 << std::endl;

        numberStringP++;

        // update the pin
        currentPinIdx1 = nextPinIdx1;


    }


}


void shed::randomifyNextPinAndDrawOneString(){


    std::cout << "  we are inside drawOne "<< std::endl;
    if (! stopIncrementationP){

        float decreaseV = algoOpacityP;
        int opacity = drawOpacityP;


        nextPinIdx1 = rand() % numberPinsP ;

        // decrease the value of the pixel that are under the line
        decreaseDarkness(lines[currentPinIdx1][nextPinIdx1], decreaseV);

        // draw the line
        drawer.decreasePixels(result, lines[currentPinIdx1][nextPinIdx1], ofColor(opacity,opacity,opacity));



        std::cout << "      we are computing in drawOne.    currentPin: "<<  currentPinIdx1 << ", nextPin: " << nextPinIdx1 << std::endl;

        numberStringP++;

        // update the pin
        currentPinIdx1 = nextPinIdx1;


    }


}





















