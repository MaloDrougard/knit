#include "wheel.h"


abstractWheel::abstractWheel(int pinsNumber , int w , int h )
{
    this->pinsNumber = pinsNumber;
    this->pins = new ofVec2f[this->pinsNumber];
    this->w = w;
    this->h = h;

    drawer = imageDrawer();

}


void abstractWheel::setupWithRandomification()
{
    this->randomifyslightlyPosition();
    this->initializeLines();

}

void abstractWheel::setup(){

    this->initializeLines();
}


void abstractWheel::generatePins()
{
    for ( int i = 0; i < pinsNumber; i = i + 1){

       pins[i] = ofVec2f(0,0);
    }
}



void abstractWheel::initializeLines(){

    list<int*> * tempL;

    // initializing lines
    lines = new list<int*> * * [pinsNumber];

    for (int i = 0; i < pinsNumber; i++) {
        lines[i] = new list<int*> * [pinsNumber];
    }

    for(int i = 0; i < pinsNumber; i ++ ){
        for(int j = 0; j < pinsNumber; j++){

            tempL = new list<int*>;
            lines[i][j] = tempL;

            if ( i != j){
                drawer.getPixelIdxOfALineDDAAlgo(tempL, pins[i], pins[j]);
            }

        }

    }
}

void abstractWheel::destroyLines(){

    // FIXME half the first time work correctly but not the second ???

    list<int * > * tempL;



    for( int x = 0; x < pinsNumber; x++)
    {
        for( int y= 0; y < pinsNumber; y++){
            tempL = lines[x][y];
            drawer.freeListOf2Int(tempL);
            delete tempL;
        }
    }

    for (int i = 0; i < pinsNumber; i++ ){ // is it correct ? needed?
         delete[] lines[i] ;
    }
    delete[] lines;

}




void abstractWheel::deletePins()
{
     delete[] pins;
}




// move the position of the pins of one pixel
void abstractWheel::randomifyslightlyPosition()
{

    int x = 0;
    int y = 0;

    int randx = 0;
    int randy = 0;

    int newX = 0;
    int newY = 0;

    for (int i = 0; i < pinsNumber; i++ )
    {
        x = static_cast<int>((pins[i]).x);
        y = static_cast<int>((pins[i]).y);

        randx = (rand() % 8) -  3;
        randy = (rand() % 8)  - 3 ;


        newX = x + randx;
        newY = y + randy;

        if(newX >= 0 and newX < this->w) {
           (pins[i]).x = newX;
        }

        if(newY >= 0 and newY < this->h ) {
           (pins[i]).y = newY;
        }


    }

}




void abstractWheel::drawPins()
{
    pinsRepresentation.allocate(this->w, this->h, OF_IMAGE_COLOR);
    pinsRepresentation.setColor(ofColor::white);

    pinsRepresentation.update();

    int x = 0;
    int y = 0;

    for (int i = 0; i < pinsNumber; i++ )
    {
        x = static_cast<int>((pins[i]).x);
        y = static_cast<int>((pins[i]).y);


        if(x < 0 or y < 0 or x >= this->w or y >= this->h ){
            std::cout << "warring pins have an pin outside the image. Values: " << to_string(x) << ", " << to_string(y) << std::endl;
        }

        pinsRepresentation.setColor(x,y, ofColor::black);
    }


     pinsRepresentation.update();

}



void abstractWheel::drawGridRepresentation()
{

    list<int *> l;

    gridRepresentation.allocate(this->w, this->h, OF_IMAGE_COLOR);
    gridRepresentation.setColor(ofColor::white);

    for(int i = 0; i < pinsNumber; i ++ ){
        for(int j = 0; j < pinsNumber; j++){

            l = *(lines[i][j]);
            drawer.decreasePixels(gridRepresentation, l, ofColor(2,2,2));
        }
    }
    gridRepresentation.update();

}



wheelCircle::wheelCircle( int pinsNumber, int w, int h)
{


    this->pinsNumber = pinsNumber;
    this->w = w;
    this->h = h;

    this->pins = new ofVec2f[this->pinsNumber];
    this->radius = (w/2.0) - 1;
    this->center =ofVec2f(w/2.0, h/2.0);

    drawer = imageDrawer();

    generatePins();

}


// generate the position of pins in the image axis
 void wheelCircle::generatePins(){

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

/*

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

 wheelFromPolyLine::wheelFromPolyLine(int pinsNumber, ofPolyline poly)
 {
     this->pinsNumber = pinsNumber;
     this->polyline = poly.getResampledByCount(pinsNumber);     // force the polyline to have the correct number of points

     ofRectangle tempRect = polyline.getBoundingBox();
     this->center = tempRect.getCenter();


     if (tempRect.getHeight() > tempRect.getWidth() ) {
         this->w = tempRect.getHeight() ;
         this->h = tempRect.getHeight() ; // we want a square
     } else {
         this->w = tempRect.getWidth();
         this->h = tempRect.getHeight();
     }

     this->pins = new ofVec2f[pinsNumber];

     this->generatePins();

     // this->randomifyslightlyPosition();

 }

 void wheelFromPolyLine::generatePins()
 {
    std::vector<ofPoint> veritces = this->polyline.getVertices();
    int i = 0;
    for(auto it = veritces.begin(); it != veritces.end(); ++it) {

        this->pins[i] = *it;
        i++;
    }

 }
*/

