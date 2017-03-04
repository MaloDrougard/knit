#include "wheel.h"

wheel::wheel(ofImage img, int pinsNumber)
{
    this->img = img;

    int w = this->img.getWidth();
    int h = this->img.getHeight();

    if (w != h){
        cerr << "image should be square!";
    }

    ofVec2f shiftDistance = ofVec2f( 0.5 + w/2, 0.5 + w/2 );
    this->generatePins(this->pins,this->pinsNumber, w/2, shiftDistance);


}


// return true if the perpendicular distance between p1 and the line given by l1 and l2 is less than tolerance
bool  wheel::isPointOnLine(ofVec2f p1, ofVec2f l1, ofVec2f l2, float tolerance) {

    // translate axis on p1
    ofVec2f newP1 = p1 - l1;
    ofVec2f newL2 = l2 -l1;

    // rotate the line on axe x
    float angleNewL2X =  newL2.angleRad(ofVec2f(1,0));
    ofVec2f newnewL2 = newL2.getRotatedRad(angleNewL2X); // should be on the x axis
    ofVec2f newnewP1 = newP1.getRotatedRad(angleNewL2X);


    float distanceFromLine = newnewP1.y; // y give the perpendicular distance from the original line

    return distanceFromLine < tolerance;

}

// generate the position of pins in the image axis
 void wheel::generatePins(ofVec2f pins[], int pinsSize, float rayon, ofVec2f center){

     float angleIncrement = (2 * M_PI) / pinsSize;
     float angle = 0;
     ofVec2f baseVec = ofVec2f(1,0);

     for ( int i = 0; i < pinsSize; i = i + 1){
        baseVec.set(1,0);
        baseVec.scale(rayon);
        pins[i] = baseVec.getRotatedRad(angle) + center;

        angle = angle + angleIncrement;
     }

 }


 ofImage wheel::getImgWithPins(){

     ofImage ret = this->img.clone();
     ofPixels retPixels = ret.getPixels();

     int idx = retPixels.getPixelIndex(0,0);
     retPixels[idx] = 0;


 }
