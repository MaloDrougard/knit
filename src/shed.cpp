#include "shed.h"

shed::shed(ofImage oriImg)
{
    originalImg = oriImg;

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

    originalImg.update();

    // We will, always use the index of pixel has coordinate.
    // Thus, the y axis are incresing in the oposite direction as usual

    ofVec2f centerWheel = ofVec2f( w/2 , w/2 );
    float radius = (w-1)/2.0 ;    // we want not to be at border but inside

    wel = wheel(360, radius, centerWheel);

    numberString = 2800;

    drawer = imageDrawer();

    // initializing lines
    lines = new list<int * > * [wel.pinsNumber];
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


int shed::findNextBestPin(int pinIdx){

    float bestScore = 0;
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

    float decreaseV = 25;

    stringPath.clear();
    stringPath.push_back(0);

    int currentPinIdx = 0;
    int nextPinIdx = -1;

    for( int i = 0 ; i< numberString; i++ ){
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


    std::list<int>::iterator next_it = stringPath.begin();
    next_it++;

    for (std::list<int>::iterator it = stringPath.begin();  next_it  != stringPath.end()  ; it++)
    {
        drawer.decreasePixels(result, lines[*it][*next_it], ofColor(25,25,25));
        next_it++;

    }

}


























