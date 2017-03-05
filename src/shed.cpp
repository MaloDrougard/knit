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

    wel = wheel(32, radius, centerWheel);

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
        for(int j = 0; j < wel.pinsNumber; j++){
            drawer.drawPixels(sketchImg, lines[i][j]);
        }
    }

}
