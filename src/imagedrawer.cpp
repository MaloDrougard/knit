#include "imagedrawer.h"

imageDrawer::imageDrawer()
{


}
// return true if the perpendicular distance between p1 and the line given by l1 and l2 is less than tolerance
bool  imageDrawer::isPointOnLine(ofVec2f p1, ofVec2f l1, ofVec2f l2, float tolerance) {

    // translate axis on p1
    ofVec2f newP1 = p1 - l1;
    ofVec2f newL2 = l2 -l1;

    // rotate the line on axe x
    float angleNewL2X =  newL2.angleRad(ofVec2f(1,0));
    ofVec2f newnewL2 = newL2.getRotatedRad(angleNewL2X); // should be on the x axis
    ofVec2f newnewP1 = newP1.getRotatedRad(angleNewL2X);


    float distanceFromLine = abs(newnewP1.y); // y give the perpendicular distance from the original line

    return distanceFromLine < tolerance;

}



void imageDrawer::drawALine(ofImage &img, ofVec2f l1, ofVec2f l2, float tolerance){

    ofColor color(134,34,34);
    int w = img.getWidth();
    int h = img.getHeight();

    ofVec2f tempPoint(0,0);

    for(int x = 0 ; x < w; x++){
        for(int y = 0; y < h; y++){

            tempPoint.set(x + 0.5,y + 0.5); // we want the center of the pixel
            if( this->isPointOnLine(tempPoint, l1, l2, tolerance) ){
                img.setColor(x,y, color);
            }
        }
    }

    img.update();

}







// return the pixel index of the pixel touch by the line
void imageDrawer::getPixelIdxOfALineDDAAlgo(list<int*> * l, ofVec2f l1, ofVec2f l2){

    int * xy ;

    int x0 =  static_cast<int> (l1.x);
    int y0 = static_cast<int> (l1.y);

    int x1 =  static_cast<int> (l2.x);
    int y1 = static_cast<int> (l2.y);

    int dx = x1 - x0;
    int dy = y1 - y0;

    int steps = 0;
    if( abs(dx) > abs(dy) ){
        steps = abs(dx);
    } else {
        steps = abs(dy);
    }

    float xIncrement = dx / (float) steps;
    float yIncrement = dy / (float) steps;

   float x = static_cast<float>(x0);
   float y = static_cast<float>(y0);

   for(int v=0; v < steps; v++)
    {

       xy = new int[2];
       xy[0] = static_cast<int>(x);
       xy[1] = static_cast<int>(y);

       l->push_back(xy);

       x = x + xIncrement;
       y = y + yIncrement;
    }
}






// calculate the percentage of the pixel above the line

float imageDrawer::percentageOfPixeBelowLine(float localStartY, float deltaV )
{


    // assume   pixelHeight = 1.0 in the computatution but var for name clarity;
    // assume  pixelWidth = 1.0;
    // assume deltaH = 1;

    float  pixelHeight = 1.0;
    float  pixelWidth = 1.0;


    float factor = abs(deltaV) ;
    float percent = -1;


    float localEndY = localStartY + deltaV;

    if (factor > 1.002) {
        std::cerr << "lines with gradient higher than 45 degree should be turn" << std::cerr;
        throw -5;
    }


    // case where the line cross the left boundary first
    if (localStartY >= 0 && localStartY <= 1) {

        if ( localEndY < 0  ) // then cross the bottom boundary
        {
            percent = abs(((localStartY / factor) * localStartY) ) / 2    ;
        }
        else if ( localEndY > 1 ) // then cross the top boundary
        {
            float inverseY = 1 - localStartY ;
            percent =  1  - ( (( inverseY / factor) * inverseY ) / 2  ) ;

        }
        else if ( (localEndY >= 0) &&  (localEndY) <= 1 ) // then cross the opposite boundary
        {
            percent = localStartY * 1 + ( deltaV / 2) ;
        }
        else {
            std::cerr << "This path must never happend -> look for the bug" << std::endl;
            throw -7;
        }
    }
    // case where the line is completely above the pixel
    else if ( localStartY > 1 && localEndY >= 1)
    {
        percent = 1;
    }
    else if (localStartY > 1 && localEndY < 1 && localEndY > 0  )
    {
            percent = 1 - ( ( ( (1-localEndY) / factor) * (1- localEndY) ) / 2 ) ;

    }
    else if (localStartY < 0 && localEndY <= 0)
    {
        percent = 0;
    }
    else if ( localStartY < 0 && localEndY > 0 && localEndY < 1 )
    {
        percent = (( localEndY/factor ) * localEndY ) / 2;
    }

    else {
        std::cerr << "This path must never happend -> look for the bug" << std::endl;
        throw -7;

    }



   return percent;


}

float imageDrawer::percentageOfPixeAboveLine(float localStartY, float deltaV)
{
    return (1 - percentageOfPixeBelowLine(localStartY, deltaV) );
}

void imageDrawer::setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(list<int *> *l, int maxIntensity, int maxXPixelIdx, int maxYPixelIdx, ofVec2f l1, ofVec2f l2 )
{
    // draw a line of 3 pixel width with 60 percent of probabilty of the line at the center

    float x0 = l1.x;
    float y0 = l1.y;
    float x1 = l2.x;
    float y1 = l2.y;


    if ( floor(x0) == floor(x1) && floor(y0) == floor(y1) )
    {
        std::cerr << "Warning in imageDrawer::setPixelIdxAndIntensityBasedOnPercentageOfAThickLine: The line passed as argument is on one pixel" << std::endl;
    }
    else if ( abs(x1 - x0) >= abs(y1 -y0))
    {

        float deltaV =  (y1 - y0) / (x1 - x0);
        float spaceBetweenTheBordersOfTheLineOnTheYAxis = sqrt(1 + pow(deltaV, 2)); // assuming width of 1


        this->setPixelIdxAndIntensityBasedOnPercentageOf1PixelWidthLineCadranA(l, 0.2 * maxIntensity, maxXPixelIdx, maxYPixelIdx, x0, y0 + spaceBetweenTheBordersOfTheLineOnTheYAxis, x1, y1 + spaceBetweenTheBordersOfTheLineOnTheYAxis);
        this->setPixelIdxAndIntensityBasedOnPercentageOf1PixelWidthLineCadranA(l, 0.6 * maxIntensity,  maxXPixelIdx, maxYPixelIdx, x0, y0, x1, y1);
        this->setPixelIdxAndIntensityBasedOnPercentageOf1PixelWidthLineCadranA(l, 0.2 * maxIntensity,  maxXPixelIdx, maxYPixelIdx, x0, y0 - spaceBetweenTheBordersOfTheLineOnTheYAxis, x1, y1 -spaceBetweenTheBordersOfTheLineOnTheYAxis);



    }
    else if ( abs(x1 - x0) < abs(y1 -y0) )
    {


        float deltaH =   (x1 - x0)/(y1 - y0);
        float spaceBetweenTheBordersOfTheLineOnTheXAxis = sqrt(1 + pow(deltaH, 2)); // assuming width of 1


        this->setPixelIdxAndIntensityBasedOnPercentageOf1PixelWidthLineCadranB(l, 0.2 * maxIntensity,  maxXPixelIdx, maxYPixelIdx, x0 + spaceBetweenTheBordersOfTheLineOnTheXAxis, y0, x1 + spaceBetweenTheBordersOfTheLineOnTheXAxis, y1);
        this->setPixelIdxAndIntensityBasedOnPercentageOf1PixelWidthLineCadranB(l, 0.6 * maxIntensity, maxXPixelIdx, maxYPixelIdx, x0, y0, x1, y1);
        this->setPixelIdxAndIntensityBasedOnPercentageOf1PixelWidthLineCadranB(l, 0.2 * maxIntensity,  maxXPixelIdx, maxYPixelIdx, x0 - spaceBetweenTheBordersOfTheLineOnTheXAxis, y0, x1 - spaceBetweenTheBordersOfTheLineOnTheXAxis, y1);


    }
    else {
        std::cerr << "Error in imageDrawer::setPixelIdxAndIntensityBasedOnPercentageOfAThickLine: unexpected branche" << std::endl;
    }



}

void imageDrawer::setPixelIdxAndIntensityBasedOnPercentageOf1PixelWidthLineCadranA(list<int *> *l, int maxIntensity,  int maxXPixelIdx, int maxYPixelIdx, float x0, float y0, float x1, float y1)
{


    float switchTemp;

    // make shure  the x0 is at the left
    if ( x1 <= x0){
        switchTemp = x0;
        x0 = x1;
        x1 = switchTemp;
        switchTemp = y0;
        y0 = y1;
        y1 = switchTemp;

    }


    int xPixelIndex = floor(x0);
    int yPixelIndex = floor(y0);



    float deltaV =  (y1 - y0) / (x1 - x0);
    float spaceBetweenTheBordersOfTheLineOnTheYAxis = sqrt(1 + pow(deltaV, 2)); // assuming width of 1


    if ( x0 >= x1 || deltaV > 1 ){
        std::cerr << "should not be in cadran A" << std::endl;
    }



    float localYTopBorder = (x0 - (float) xPixelIndex) * (-deltaV) + spaceBetweenTheBordersOfTheLineOnTheYAxis/2 ; // we are actually at pixel (xPixelIndex, yPixelIndex)
    float localYBottomBorder = localYTopBorder - spaceBetweenTheBordersOfTheLineOnTheYAxis;



    float realYatX = y0 + localYTopBorder;  // the real Y mean the value of the top border of the line at the value of the xPixelIndex (not the center but the left side)


    if (deltaV <= 0 )
    {

        float percent = -1;
        float percent1 = -1;
        float percent2 = -1;

        int zeroPercentCounter = 0;
        int* xyi;

        bool pixelAlreadyInLine = false;

        while  (xPixelIndex <= x1) {

            // initialize the loop
            yPixelIndex = floor(realYatX);
            localYTopBorder = realYatX - yPixelIndex;
            localYBottomBorder = localYTopBorder - spaceBetweenTheBordersOfTheLineOnTheYAxis;

            zeroPercentCounter = 0; // because a zerro percent can happend at the begin if the the border is on the grid


            // do until line is no more on the pixel at index xPixelIndex, yPixelIndex
            do {

                percent1 = percentageOfPixeBelowLine( localYTopBorder, deltaV);
                percent2 = percentageOfPixeAboveLine( localYBottomBorder, deltaV);
                percent = 1 - (1 -percent1) - (1- percent2);

                if (percent != 0 && xPixelIndex <= maxXPixelIdx && yPixelIndex <= maxYPixelIdx && xPixelIndex > 0 && yPixelIndex > 0 ){


                    // check if we already have this pixel in the line
                    pixelAlreadyInLine = false;
                    for (std::list<int * >::iterator it = l->begin();(it != l->end()) && (pixelAlreadyInLine == false); it++){

                        if( (*it)[0] == xPixelIndex && (*it)[1] == yPixelIndex ) {
                           (*it)[2] += floor(percent * maxIntensity);
                            pixelAlreadyInLine = true;
                        }

                    }
                    // case where the pixel is not already in line
                    if (! pixelAlreadyInLine){


                        xyi = new int[3];
                        xyi[0] = xPixelIndex;
                        xyi[1] = yPixelIndex;
                        xyi[2] = floor(percent * maxIntensity);
                        l->push_back(xyi);
                    }





                } else {
                    zeroPercentCounter++;
                }

                localYTopBorder = localYTopBorder + 1; // we slide from one pixel
                localYBottomBorder = localYBottomBorder + 1;

                --yPixelIndex;

            } while ( percent != 0 || zeroPercentCounter < 2 );

            // go on step to the right
            xPixelIndex = xPixelIndex + 1;
            realYatX = realYatX + deltaV;

        }



   }

    else if (deltaV > 0 )
    {

        float percent = -1;
        float percent1 = -1;
        float percent2 = -1;

        int zeroPercentCounter = 0;
        int* xyi;

        bool pixelAlreadyInLine = false;

        while  (xPixelIndex <= x1) {

            // initialize the loop
            yPixelIndex = floor(realYatX - spaceBetweenTheBordersOfTheLineOnTheYAxis); // this time we start at the lowest point
            localYTopBorder = realYatX - yPixelIndex;
            localYBottomBorder = localYTopBorder - spaceBetweenTheBordersOfTheLineOnTheYAxis;

            zeroPercentCounter = 0; // because a zerro percent can happend at the begin if the the border is on the grid


            // do until line is no more on the pixel at index xPixelIndex, yPixelIndex
            do {

                percent1 = percentageOfPixeBelowLine( localYTopBorder, deltaV);
                percent2 = percentageOfPixeAboveLine( localYBottomBorder, deltaV);
                percent = 1 - (1 -percent1) - (1- percent2);

                if (percent != 0 && xPixelIndex <= maxXPixelIdx && yPixelIndex <= maxYPixelIdx && xPixelIndex > 0 && yPixelIndex > 0 ){

                    // check if we already have this pixel in the line
                    pixelAlreadyInLine = false;
                    for (std::list<int * >::iterator it = l->begin(); (it != l->end()) && (pixelAlreadyInLine == false); it++){

                        if( (*it)[0] == xPixelIndex && (*it)[1] == yPixelIndex ) {
                           (*it)[2] += floor(percent * maxIntensity);
                            pixelAlreadyInLine = true;
                        }

                    }
                    // case where the pixel is not already in line
                    if (! pixelAlreadyInLine){


                        xyi = new int[3];
                        xyi[0] = xPixelIndex;
                        xyi[1] = yPixelIndex;
                        xyi[2] = floor(percent * maxIntensity);
                        l->push_back(xyi);
                    }


                } else {
                    zeroPercentCounter++;
                }

                localYTopBorder = localYTopBorder - 1; // we slide from one pixel
                localYBottomBorder = localYBottomBorder - 1;

                yPixelIndex++;

            } while ( percent != 0 || zeroPercentCounter < 2 );

            // go on step to the right
            xPixelIndex = xPixelIndex + 1;
            realYatX = realYatX + deltaV;

        }


    }



}

void imageDrawer::setPixelIdxAndIntensityBasedOnPercentageOf1PixelWidthLineCadranB(list<int *> *l, int maxIntensity, int maxXPixelIdx, int maxYPixelIdx, float x0, float y0, float x1, float y1)
{


    // switch of the coordinate
    float switchTemp = x0;
    x0 = y0;
    y0 = switchTemp;
    switchTemp = x1;
    x1 = y1;
    y1 =switchTemp;

    // make shure  the x0 is at the left
    if ( x1 <= x0){
        switchTemp = x0;
        x0 = x1;
        x1 = switchTemp;
        switchTemp = y0;
        y0 = y1;
        y1 = switchTemp;

    }



    int xPixelIndex = floor(x0);
    int yPixelIndex = floor(y0);



    float deltaV =  (y1 - y0) / (x1 - x0);
    float spaceBetweenTheBordersOfTheLineOnTheYAxis = sqrt(1 + pow(deltaV, 2)); // assuming width of 1


    if ( x0 >= x1 || deltaV > 1 ){
        std::cerr << "should not be in cadran A" << std::endl;
    }



    // we always start at the center of pixel indexes
    float localYTopBorder = (x0 - (float) xPixelIndex ) * (-deltaV) + spaceBetweenTheBordersOfTheLineOnTheYAxis/2 ; // we are actually at pixel (x0, y0)
    float localYBottomBorder = localYTopBorder - spaceBetweenTheBordersOfTheLineOnTheYAxis;



    float realYatX = (float) y0 + localYTopBorder;  // the real Y mean the value of the top border of the line at the value of the xPixelIndex (not the center but the left side)


    if (deltaV <= 0 )
    {

        float percent = -1;
        float percent1 = -1;
        float percent2 = -1;

        int zeroPercentCounter = 0;
        int* xyi;


        bool pixelAlreadyInLine = false;

        while  (xPixelIndex <= x1) {

            // initialize the loop
            yPixelIndex = floor(realYatX);
            localYTopBorder = realYatX - yPixelIndex;
            localYBottomBorder = localYTopBorder - spaceBetweenTheBordersOfTheLineOnTheYAxis;

            zeroPercentCounter = 0; // because a zerro percent can happend at the begin if the the border is on the grid


            // do until line is no more on the pixel at index xPixelIndex, yPixelIndex
            do {

                percent1 = percentageOfPixeBelowLine( localYTopBorder, deltaV);
                percent2 = percentageOfPixeAboveLine( localYBottomBorder, deltaV);
                percent = 1 - (1 -percent1) - (1- percent2);

                if (percent != 0 && xPixelIndex <= maxYPixelIdx && yPixelIndex <= maxXPixelIdx && xPixelIndex > 0 && yPixelIndex > 0  ){ // do not forgget we change the orientation of axises

                    // check if we already have this pixel in the line
                    pixelAlreadyInLine = false;
                    for (std::list<int * >::iterator it = l->begin(); (it != l->end()) && (pixelAlreadyInLine == false); it++){

                        if( (*it)[1] == xPixelIndex && (*it)[0] == yPixelIndex ) {
                           (*it)[2] += floor(percent * maxIntensity);
                            pixelAlreadyInLine = true;
                        }

                    }
                    // case where the pixel is not already in line
                    if (! pixelAlreadyInLine){


                        xyi = new int[3];
                        xyi[1] = xPixelIndex;
                        xyi[0] = yPixelIndex;
                        xyi[2] = floor(percent * maxIntensity);
                        l->push_back(xyi);
                    }

                } else {
                    zeroPercentCounter++;
                }

                localYTopBorder = localYTopBorder + 1; // we slide from one pixel
                localYBottomBorder = localYBottomBorder + 1;

                --yPixelIndex;

            } while ( percent != 0 || zeroPercentCounter < 2 );

            // go on step to the right
            xPixelIndex = xPixelIndex + 1;
            realYatX = realYatX + deltaV;

        }



   }

    else if (deltaV > 0 )
    {

        float percent = -1;
        float percent1 = -1;
        float percent2 = -1;

        int zeroPercentCounter = 0;
        int* xyi;


        bool pixelAlreadyInLine = false;

        while  (xPixelIndex <= x1) {

            // initialize the loop
            yPixelIndex = floor(realYatX - spaceBetweenTheBordersOfTheLineOnTheYAxis); // this time we start at the lowest point
            localYTopBorder = realYatX - yPixelIndex;
            localYBottomBorder = localYTopBorder - spaceBetweenTheBordersOfTheLineOnTheYAxis;

            zeroPercentCounter = 0; // because a zerro percent can happend at the begin if the the border is on the grid


            // do until line is no more on the pixel at index xPixelIndex, yPixelIndex
            do {

                percent1 = percentageOfPixeBelowLine( localYTopBorder, deltaV);
                percent2 = percentageOfPixeAboveLine( localYBottomBorder, deltaV);
                percent = 1 - (1 -percent1) - (1- percent2);

                if (percent != 0 && xPixelIndex <= maxYPixelIdx && yPixelIndex <= maxXPixelIdx && xPixelIndex > 0 && yPixelIndex > 0){ // do not forgget we change the orientation of axises

                    // check if we already have this pixel in the line
                    pixelAlreadyInLine = false;
                    for (std::list<int * >::iterator it = l->begin(); (it != l->end()) && (pixelAlreadyInLine == false); it++){

                        if( (*it)[1] == xPixelIndex && (*it)[0] == yPixelIndex ) {
                           (*it)[2] += floor(percent * maxIntensity);
                            pixelAlreadyInLine = true;
                        }

                    }
                    // case where the pixel is not already in line
                    if (!pixelAlreadyInLine){


                        xyi = new int[3];
                        xyi[1] = xPixelIndex;
                        xyi[0] = yPixelIndex;
                        xyi[2] = floor(percent * maxIntensity);
                        l->push_back(xyi);
                    }


                } else {
                    zeroPercentCounter++;
                }

                localYTopBorder = localYTopBorder - 1; // we slide from one pixel
                localYBottomBorder = localYBottomBorder - 1;

                yPixelIndex++;

            } while ( percent != 0 || zeroPercentCounter < 2 );

            // go on step to the right
            xPixelIndex = xPixelIndex + 1;
            realYatX = realYatX + deltaV;

        }


    }


}


void imageDrawer::drawPixelsWithIntensity(ofImage &img, list<int *> l)
{

    ofColor color;
    int value;
    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++){

        value = 255 - (*it)[2];
        color = ofColor( value, value, value );
        img.setColor((*it)[0],  (*it)[1], color);
    }

    img.update();
}


// this function take a list of the forme xyi (position x, position y, intensity) as input
void imageDrawer::increasePixelsWithIntensity(ofImage &img, list<int *> l)
{

    ofColor color;
    ofColor oldColor;
    float newLightness;

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++){

        color = ofColor( (*it)[2], (*it)[2] , (*it)[2] );
        oldColor = img.getColor((*it)[0],  (*it)[1] );
        newLightness = oldColor.getLightness() - (color.limit() - color.getLightness() );
        newLightness = 0 > newLightness ? 0 : newLightness;

        color = ofColor(newLightness, newLightness, newLightness);
        img.setColor((*it)[0],  (*it)[1], color);
    }

    img.update();
}

void imageDrawer::decreasePixelsWithIntensity(ofImage &img, list<int *> l)
{
    ofColor color;
    ofColor oldColor;
    int value;
    int xIdx;
    int yIdx;

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++){

        value = (*it)[2];
        color = ofColor( value, value, value );
        xIdx = (*it)[0];
        yIdx = (*it)[1];

        oldColor = img.getColor(xIdx, yIdx);
        img.setColor(xIdx,  yIdx, oldColor - color);

      }

    img.update();

}



























void imageDrawer::freeListOfIntArray(list<int *> * l )
{
    for(std::list<int *>::iterator it = l->begin(); it  != l->end(); it++){
        delete[] (*it);
    }
    l->clear();



}


void imageDrawer::printListIdx(list<int * > l ){


    std::cout << "print list:\n";

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
        std::cout << '(' << (*it)[0] << ',' << (*it)[1] << ") , " ;

    std::cout << std::endl;


}

void imageDrawer::printListIdxAndIntensity(list<int *> l)
{

    std::cout << "print list:\n";

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
        std::cout << '(' << (*it)[0] << ',' << (*it)[1] << ',' << (*it)[2] << ") " ;

    std::cout << std::endl;
}

void imageDrawer::drawPins(ofImage &img, ofVec2f* pins, int pinsNumber){

    ofColor color(45,45,45);

    int x = 0;
    int y = 0;
    float pinx = 0;
    float piny = 0;

    for(int i = 0; i < pinsNumber; i++){

       pinx = pins[i].x;
       piny = pins[i].y;
       x = static_cast<int>( pinx); // take the floor of the number
       y = static_cast<int>( piny);

       img.setColor( x, y , color);
       img.setColor( x + 1, y , color);
       img.setColor( x, y + 1 , color);
       img.setColor( x - 1,  y , color);
       img.setColor( x, y - 1 , color);

    }

    img.update();


}


void imageDrawer::drawPixels(ofImage &img, list<int * > l, ofColor color)
{


    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++){

        img.setColor((*it)[0],  (*it)[1], color);
    }

    img.update();

}



void imageDrawer::incrementPixels(ofImage &img, list<int * > l, ofColor inColor)
{
    ofColor color = ofColor::white;

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++){

        color = img.getColor((*it)[0],  (*it)[1]);
        color = color + inColor;
        img.setColor((*it)[0],  (*it)[1], color);
    }

    img.update();

}




void imageDrawer::decreasePixels(ofImage &img, list<int * > l, ofColor color)
{
    ofColor tempColor;
    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++){

        tempColor = img.getColor((*it)[0],  (*it)[1]);
        tempColor = tempColor - color;
        img.setColor((*it)[0],  (*it)[1], tempColor);
    }

    img.update();

}


void imageDrawer::increasePixels(ofImage &img, list<int * > l, ofColor color)
{
    ofColor tempColor;
    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++){

        tempColor = img.getColor((*it)[0],  (*it)[1]);
        tempColor = tempColor + color;
        img.setColor((*it)[0],  (*it)[1], tempColor);
    }

    img.update();

}




