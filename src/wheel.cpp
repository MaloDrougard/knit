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

 void wheel::drawPins()
 {
     representation.allocate(2*radius + 1, 2*radius + 1, OF_IMAGE_COLOR);
     representation.setColor(ofColor::white);

     representation.update();

     int x = 0;
     int y = 0;

     for (int i = 0; i < pinsNumber; i++ )
     {
         x = static_cast<int>((pins[i]).x);
         y = static_cast<int>((pins[i]).y);


         if(x < 0 or y < 0 or x > (2*radius-1) or y >( 2*radius -1)){
             std::cout << "warring pins have an pin outside the image. Values: " << to_string(x) << ", " << to_string(y) << std::endl;
         }

         representation.setColor(x,y, ofColor::black);
     }


      representation.update();

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


     float angleIncrement = (2 * M_PI) / (pinsNumber - (extraPins.size()));
     float angle = 0;
     ofVec2f baseVec = ofVec2f(1,0);


     for ( int i = count; i < pinsNumber; i = i + 1){
        baseVec.set(1,0);
        baseVec.scale(radius);
        pins[i] = baseVec.getRotatedRad(- angle) + center; // - angle to get a counter clocwise indexing for image coordinate system
        angle = angle + angleIncrement;
     }

 }

 wheelTribal::wheelTribal(int pinsNumber, float radius, ofVec2f center)
 {
     this->pinsNumber = (2 * pinsNumber) ; // we allow memory for the center pin
     this->pins = new ofVec2f[this->pinsNumber];
     this->radius = radius;
     this->center = center;
     this->representation = ofImage();

     this->generatePins();
 }

 void wheelTribal::generatePins()
 {
     float angleIncrement = (2 * M_PI) / (pinsNumber / 2);
     float angle = 0;
     ofVec2f baseVec = ofVec2f(1,0);

     for ( int i = 0; i < (pinsNumber)/2; i = i + 1){
        baseVec.set(1,0);
        baseVec.scale(radius);
        pins[i] = baseVec.getRotatedRad(- angle) + center; // - angle to get a counter clocwise indexing for image coordinate system
        angle = angle + angleIncrement;
     }


     // draw the firs 1/3
     angle = 0;
     baseVec.set((sqrt(3)/2), -0.5);
     ofVec2f newCenter = ofVec2f(center.x, center.y + radius  );
     ofVec2f tempRes = ofVec2f(-1,-1);
     int marge = (pinsNumber /2); // where to start to put int pins


     for ( int i = 0; i < (pinsNumber/(2*3) ); i = i + 1){
        baseVec.set((sqrt(3)/2), -0.5);
        baseVec.scale(radius);
        tempRes = baseVec.getRotatedRad(- angle) + newCenter;
        if(tempRes.x < 0 or tempRes.y < 0 or tempRes.x > (2*radius-1) or tempRes.y >( 2*radius -1)){
            std::cout << "warring pins have an pin outside the image. Values: " << to_string(tempRes.x) << ", " << to_string(tempRes.y) << std::endl;
        }
        pins[marge + i] = tempRes; // - angle to get a counter clocwise indexing for image coordinate system
        angle = angle + angleIncrement;
     }


     // draw the 2/3
     angle = 0;
     baseVec.set(0,-1);
     baseVec.scale(radius);
     baseVec = baseVec.getRotatedRad( ( M_PI / 3) );
     newCenter = baseVec + center;


     marge = (pinsNumber /2) + (pinsNumber/(2*3)); // where to start to put int pins

     tempRes = ofVec2f(-1,-1);

     for ( int i = 0; i < ((pinsNumber/(2*3) -1) ); i = i + 1){
        baseVec.set(0,1);
        baseVec.scale(radius);
        tempRes = baseVec.getRotatedRad(angle) + newCenter;
        if(tempRes.x < 0 or tempRes.y < 0 or tempRes.x > (2*radius) or tempRes.y > (2*radius)){
            std::cout << "warring pins have an pin outside the image. Values: " << to_string(tempRes.x) << ", " << to_string(tempRes.y) << std::endl;
        }
        pins[marge + i] = tempRes; // - angle to get a counter clocwise indexing for image coordinate system
        angle = angle + angleIncrement;
     }


     // draw the 3/3
     angle = 0;
     baseVec.set(0, -1);
     baseVec.scale(radius);
     baseVec = baseVec.getRotatedRad(  -( M_PI / 3) );
     newCenter = baseVec +  center;

     marge = (pinsNumber /2) + 2*(pinsNumber/(2*3)); // where to start to put int pins
     tempRes = ofVec2f(-1,-1);

     for ( int i = 0; i < ((pinsNumber/(2*3) -1) ); i = i + 1){
        baseVec.set(0, 1);
        baseVec.scale(radius);
        tempRes = baseVec.getRotatedRad(- angle) + newCenter;
        if(tempRes.x < 0 or tempRes.y < 0 or tempRes.x > (2*radius) or tempRes.y >( 2*radius)){
            std::cout << "warring pins have an pin outside the image. Values: " << to_string(tempRes.x) << ", " << to_string(tempRes.y) << std::endl;
        }
        pins[marge + i] = tempRes; // - angle to get a counter clocwise indexing for image coordinate system
        angle = angle + angleIncrement;
     }


 }
