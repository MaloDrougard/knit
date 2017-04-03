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

 void wheel::deletePins()
 {
     delete[] pins;
 }





 wheelWithCenter::wheelWithCenter(int pinsNumber, float radius, ofVec2f center)
 {
     this->pinsNumber = pinsNumber + 1 ; // we allow memory for the center pin
     this->pins = new ofVec2f[this->pinsNumber];
     this->radius = radius;
     this->center = center;

     this->generatePins();
 }

 void wheelWithCenter::generatePins()
 {

     float angleIncrement = (2 * M_PI) / (pinsNumber - 1);
     float angle = 0;
     ofVec2f baseVec = ofVec2f(1,0);

     pins[0] = center;

     for ( int i = 1; i < pinsNumber; i = i + 1){
        baseVec.set(1,0);
        baseVec.scale(radius);
        pins[i] = baseVec.getRotatedRad(- angle) + center; // - angle to get a counter clocwise indexing for image coordinate system
        angle = angle + angleIncrement;
     }


 }

 wheelExtra::wheelExtra(int pinsNumber, float radius, ofVec2f center, std::list<ofVec2f> extraPins)
 {
     this->extraPins = extraPins;
     this->pinsNumber = pinsNumber + extraPins.size() ; // we allow memory for the center pin
     this->pins = new ofVec2f[this->pinsNumber];
     this->radius = radius;
     this->center = center;


     this->generatePins();
 }

 void wheelExtra::generatePins()
 {

     int count = 0;

     for (std::list<ofVec2f>::iterator it=extraPins.begin(); it != extraPins.end(); ++it)
     {
        pins[count] = *it;
        count++;
     }


     float angleIncrement = (2 * M_PI) / (pinsNumber - 1);
     float angle = 0;
     ofVec2f baseVec = ofVec2f(1,0);


     for ( int i = count; i < pinsNumber; i = i + 1){
        baseVec.set(1,0);
        baseVec.scale(radius);
        pins[i] = baseVec.getRotatedRad(- angle) + center; // - angle to get a counter clocwise indexing for image coordinate system
        angle = angle + angleIncrement;
     }

 }
