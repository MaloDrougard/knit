#include "wheel.h"

wheel::wheel( int pinsNumber, float radius, ofVec2f center)
{
    this->pinsNumber = pinsNumber;
    this->pins = new ofVec2f[this->pinsNumber];
    this->radius = radius;
    this->center = center;

    this->generatePins();
}




// generate the position of pins in the image axis
 void wheel::generatePins(){

     float angleIncrement = (2 * M_PI) / pinsNumber;
     float angle = 0;
     ofVec2f baseVec = ofVec2f(1,0);

     for ( int i = 0; i < pinsNumber; i = i + 1){
        baseVec.set(1,0);
        baseVec.scale(radius);
        pins[i] = baseVec.getRotatedRad(- angle) + center; // - angle to get a counter clocwise indexing for image coordinate system
        angle = angle + angleIncrement;
     }

 }






