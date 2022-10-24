#include "wheel.h"


abstractWheel::abstractWheel(int pinsNumber , int w , int h )
{
    this->type = "abstract";
    this->pinsNumber = pinsNumber;
    this->pins = new ofVec2f[this->pinsNumber];
    this->w = w;
    this->h = h;

    drawer = imageDrawer();

    this->generatePins();

}

string abstractWheel::getInfos()
{
    return  "-wt:" + type + "-wp:" + std::to_string(pinsNumber) ;
}


void abstractWheel::setupWithRandomification()
{
    this->randomifyslightlyPosition();
    this->initializeLines();

}


void abstractWheel::setup(){

    this->initializeLines();
}


void abstractWheel::writePinPositionsToFile(string absolutFn)
{

    std::ofstream output(absolutFn);
    assert(output.is_open());

    for (int i = 0; i < pinsNumber; i++){
        output << (pins[i]).x << "," << (pins[i]).y << std::endl;
    }
    output.close();
}

void abstractWheel::destroy()
{
    this->destroyLines();
    this->deletePins();
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
        for(int j = 0; j <= i; j++){

            tempL = new list<int*>;
            lines[i][j] = tempL;

            if ( i != j){
                drawer.getPixelIdxOfALineDDAAlgo(tempL, pins[i], pins[j]);
            }
        }

    }

    // make the symetrie only with the pointers to save memory
    for(int i = 0; i < pinsNumber; i ++ ){
        for(int j = i + 1; j < pinsNumber; j++){

            lines[i][j] = lines[j][i];
        }

    }





}

void abstractWheel::intializeThickLines()
{

    list<int*> * tempL;

    // initializing thickLines
    thickLines = new list<int*> * * [pinsNumber];

    for (int i = 0; i < pinsNumber; i++) {
        thickLines[i] = new list<int*> * [pinsNumber];
    }

    for(int i = 0; i < pinsNumber; i ++ ){
        for(int j = 0; j <= i; j++){

            tempL = new list<int*>;
            thickLines[i][j] = tempL;

            if ( i != j){

                drawer.setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(tempL, 100, w-1 , h-1, pins[i], pins[j]);

            }
        }

    }

    // make the symetrie only with the pointers to save memory
    for(int i = 0; i < pinsNumber; i ++ ){
        for(int j = i + 1; j < pinsNumber; j++){

            thickLines[i][j] = thickLines[j][i];
        }

    }






}






void abstractWheel::destroyLines(){

    // FIXME half the first time work correctly but not the second ???

    list<int * > * tempL;



    for( int x = 0; x < pinsNumber; x++)
    {
        for( int y= x; y < pinsNumber; y++){ // rember we had some optimization and we have a triangle matrix
            tempL = lines[x][y];
            drawer.freeListOfIntArray(tempL);
            delete tempL;
        }
    }

    for (int i = 0; i < pinsNumber; i++ ){ // is it correct ? needed?
         delete[] lines[i] ;
    }

    delete [] lines;



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
        x = floor((pins[i]).x);  // imagine you have a origin at the top right with y in direction of the bottom -> then you transform the position into the index where the position stand in
        y = floor((pins[i]).y);


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



void abstractWheel::drawThickGridRepresentation()
{

    list<int *> l;

    gridRepresentation.allocate(this->w, this->h, OF_IMAGE_COLOR);
    gridRepresentation.setColor(ofColor::white);

    for(int i = 0; i < pinsNumber; i ++ ){
        for(int j = 0; j < pinsNumber; j++){

            l = *(thickLines[i][j]);
            drawer.decreasePixelsWithIntensity(gridRepresentation, l);
        }
    }
    gridRepresentation.update();

}



wheelCircle::wheelCircle( int pinsNumber, int w, int h)
{

    this->type = "circle";
    this->pinsNumber = pinsNumber;
    this->w = w;
    this->h = h;

    this->pins = new ofVec2f[this->pinsNumber];
    this->radius = (w/2.0) - 1;
    this->center =ofVec2f(w/2.0, h/2.0);

    drawer = imageDrawer();

    generatePins();

}


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



 wheelExtra::wheelExtra(int pinsNumber, int w, int h, std::list<ofVec2f> extraPins)
 {
     this->type = "extra";
     this->extraPins = extraPins;
     this->pinsNumber = pinsNumber + extraPins.size() ; // we allow memory for the center pin
     this->pins = new ofVec2f[this->pinsNumber];

     this->w = w;
     this->h = h;

     this->radius = (w/2.0) - 1;
     this->center =ofVec2f(w/2.0, h/2.0);

     drawer = imageDrawer();

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



 wheelTribal::wheelTribal(int pinsNumber, int w, int h)
 {
     this->type = "tribal";
     this->pinsNumber = (2 * pinsNumber) ; // we allow memory for the center pin
     this->pins = new ofVec2f[this->pinsNumber];

     this->w = w;
     this->h = h;

     this->radius = (w/2.0) - 1;
     this->center =ofVec2f(w/2.0, h/2.0);

     drawer = imageDrawer();

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
     this->type = "polyline";
     this->pinsNumber = pinsNumber;
     this->polyline = poly.getResampledByCount(pinsNumber);     // force the polyline to have the correct number of points

     ofRectangle tempRect = polyline.getBoundingBox();



     if (tempRect.getHeight() > tempRect.getWidth() ) {
         this->w = tempRect.getHeight() ;
         this->h = tempRect.getHeight() ; // we want a square
     } else {
         this->w = tempRect.getWidth();
         this->h = tempRect.getHeight();
     }

     this->pins = new ofVec2f[pinsNumber];

     drawer = imageDrawer();

     this->generatePins();

     // this->randomifyslightlyPosition();

 }

 void wheelFromPolyLine::generatePins()
 {
    auto veritces = this->polyline.getVertices();
    int i = 0;
    for(auto it = veritces.begin(); it != veritces.end(); ++it) {

        this->pins[i] = *it;
        i++;
    }

 }



 wheelSquare::wheelSquare(int pinsNumber, int w)
 {
     this->type = "square";

     this->pinsNumber = pinsNumber;
     this->w = w;
     this->h = w;

     polyline.addVertex(0,0);
     polyline.lineTo(w -1,0);
     polyline.lineTo(w-1,h-1);
     polyline.lineTo(0,h-1);
     polyline.lineTo(0,0);

     polyline = polyline.getResampledByCount(pinsNumber);     // force the polyline to have the correct number of points

     this->pins = new ofVec2f[pinsNumber];

     drawer = imageDrawer();

     this->generatePins();


 }

 wheelFromFile::wheelFromFile(string filename, int w, int h)
 {
     this->type = "fromfile";
     this->filename = filename;

     this->w = w;
     this->h = h;

     int numberOfLine = 0;
     string line;

     std::ifstream input(filename);
     assert(input.is_open());

     while (! input.eof())
     {
        std::getline(input, line);
        if (line != ""){
            numberOfLine++;
        }

     }
     input.close();

     pinsNumber = numberOfLine;
     pins = new ofVec2f[pinsNumber];

     this->generatePins();

 }


 void wheelFromFile::generatePins()
 {

     std::ifstream input(filename);
     assert(input.is_open());



     int i = 0;
     string v1;
     string v2;

     float f1;
     float f2;

     while (! input.eof() )
     {
        std::getline(input, v1, ',');
        std::getline(input, v2 );

        if (v1 != "" and v2  != "") {

             f1 = std::strtof(v1.c_str(), 0);
             f2 = std::strtof(v2.c_str(), 0);

             pins[i] = ofVec2f(f1,f2);
             i++;

        }
     }
     input.close();

 }
