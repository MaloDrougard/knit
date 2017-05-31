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




// During this process the list l is fill with index of pixel contains in the line and is intensity
// the forme is: xyi[0] = x position of the pixel, xyi[1] = y position of the pixel, xyi[3] = intensity
void imageDrawer::setPixelIdxAndIntensityOfAThickLine(list<int*> * l, ofVec2f l1, ofVec2f l2 , float width)
{

   int x0 = l1.x;
   int y0 = l1.y;
   int x1 = l2.x;
   int y1 = l2.y;

   int dx = abs(x1-x0);
   int sx = x0 < x1 ? 1 : -1; // step directio
   int dy = abs(y1-y0);
   int sy = y0 < y1 ? 1 : -1;

   int err = dx-dy;  // difference
   int e2, x2, y2;
   int intensity;

   int * xyi; // store xy position and intensity

   float ed = dx+dy == 0 ? 1 : sqrt((float)dx*dx+(float)dy*dy);

   for (width = (width+1)/2; ; )
   {                                   /* pixel loop */

       intensity =  0 > 255 * ( abs(err-dx+dy)/ed - width + 1) ? 0 : 255*(abs(e2)/ed-width+1) ;
       xyi = new int[3];
       xyi[0] = x0;
       xyi[1] = y0;
       xyi[2] =  intensity;

       l->push_back(xyi);

       e2 = err; //dx-dy
       x2 = x0;

      if (2*e2 >= -dx) {                              // test end //             /* x step */

          for (e2 += dy, y2 = y0 ; e2 < ed*width && (y1 != y2 || dx > dy); e2 += dx){

              //setPixelColor(x0, y2 += sy, max(0,255*(abs(e2)/ed-width+1)));

              intensity = 0 > 255*(abs(e2)/ed-width+1) ? 0 : 255*(abs(e2)/ed-width+1) ;
              y2 = y2 + sy;
              xyi = new int[3];
              xyi[0] = x0;
              xyi[1] = y2;
              xyi[2] = static_cast<int>( intensity );

              l->push_back(xyi);

          }

          if (x0 == x1) break;

          e2 = err;
          err -= dy;
          x0 += sx;
      }
      if (2*e2 <= dy) {                                            /* y step */

          for (e2 = dx-e2; e2 < ed*width && (x1 != x2 || dx < dy); e2 += dy){
            //setPixelColor(x2 += sx, y0, max(0,255*(abs(e2)/ed-width+1)));

              intensity = 0 > 255*(abs(e2)/ed-width+1) ? 0 : 255*(abs(e2)/ed-width+1) ;
              x2 = x2 + sx;
              xyi = new int[3];
              xyi[0] = x2;
              xyi[1] = y0;
              xyi[2] = static_cast<int>( intensity);

              l->push_back(xyi);

          }
          if (y0 == y1) break;

          err += dx;
          y0 += sy;
      }
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

    if (factor > 1) {
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

void imageDrawer::setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(list<int *> *l, ofVec2f l1, ofVec2f l2, float width)
{

    int maxIntensity = 255;
    // assume x0 to the left
    // assume x1 - x0 > 0

    int x0 = floor(l1.x);
    int y0 = floor(l1.y);
    int x1 = floor(l2.x);
    int y1 = floor(l2.y);



    int xPixelIndex = x0;
    int yPixelIndex = y0;



    float deltaV =  (y1 - y0) / (x1 - x0);
    float spaceBetweenTheBordersOfTheLineOnTheYAxis = sqrt(1 + pow(deltaV, 2)); // assuming width of 1


    // we always start at the center of pixel indexes
    float localYTopBorder = 0.5 * (-deltaV) + 0.5 + spaceBetweenTheBordersOfTheLineOnTheYAxis/2 ; // we are actually at pixel (x0, y0)
    float localYBottomBorder = localYTopBorder - spaceBetweenTheBordersOfTheLineOnTheYAxis;



    float realYatX = y0 + localYTopBorder;  // the real Y mean the value of the top border of the line at the value of the xPixelIndex (not the center but the left side)


    if (deltaV <= 0 )
    {

        float percent = -1;
        float percent1 = -1;
        float percent2 = -1;

        int zeroPercentCounter = 0;
        int* xyi;

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

                if (percent != 0 ){

                    xyi = new int[3];
                    xyi[0] = xPixelIndex;
                    xyi[1] = yPixelIndex;
                    xyi[2] = floor(percent * maxIntensity);

                    l->push_back(xyi);


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

                if (percent != 0 ){

                    xyi = new int[3];
                    xyi[0] = xPixelIndex;
                    xyi[1] = yPixelIndex;
                    xyi[2] = floor(percent * maxIntensity);

                    l->push_back(xyi);


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

void imageDrawer::setPixelIdxAndIntensityBasedOnPercentageOfAThickLineCadranA(list<int *> *l, int maxIntensity,  int x0, int y0, int x1, int y1)
{


    int switchTemp;

    // make shure  the x0 is at the left
    if ( x1 <= x0){
        switchTemp = x0;
        x0 = x1;
        x1 = switchTemp;
        switchTemp = y0;
        y0 = y1;
        y1 = switchTemp;

    }


    int xPixelIndex = x0;
    int yPixelIndex = y0;



    float deltaV =  (y1 - y0) / (x1 - x0);
    float spaceBetweenTheBordersOfTheLineOnTheYAxis = sqrt(1 + pow(deltaV, 2)); // assuming width of 1


    if ( x0 >= x1 || deltaV > 1 ){
        std::cerr << "should not be in cadran A" << std::endl;
    }



    // we always start at the center of pixel indexes
    float localYTopBorder = 0.5 * (-deltaV) + 0.5 + spaceBetweenTheBordersOfTheLineOnTheYAxis/2 ; // we are actually at pixel (x0, y0)
    float localYBottomBorder = localYTopBorder - spaceBetweenTheBordersOfTheLineOnTheYAxis;



    float realYatX = y0 + localYTopBorder;  // the real Y mean the value of the top border of the line at the value of the xPixelIndex (not the center but the left side)


    if (deltaV <= 0 )
    {

        float percent = -1;
        float percent1 = -1;
        float percent2 = -1;

        int zeroPercentCounter = 0;
        int* xyi;

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

                if (percent != 0 ){

                    xyi = new int[3];
                    xyi[0] = xPixelIndex;
                    xyi[1] = yPixelIndex;
                    xyi[2] = floor(percent * maxIntensity);

                    l->push_back(xyi);


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

                if (percent != 0 ){

                    xyi = new int[3];
                    xyi[0] = xPixelIndex;
                    xyi[1] = yPixelIndex;
                    xyi[2] = floor(percent * maxIntensity);

                    l->push_back(xyi);


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

void imageDrawer::setPixelIdxAndIntensityBasedOnPercentageOfAThickLineCadranB(list<int *> *l, int maxIntensity, int x0, int y0, int x1, int y1)
{


    // switch of the coordinate
    int switchTemp = x0;
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



    int xPixelIndex = x0;
    int yPixelIndex = y0;



    float deltaV =  (float)(y1 - y0) / (float)(x1 - x0);
    float spaceBetweenTheBordersOfTheLineOnTheYAxis = sqrt(1 + pow(deltaV, 2)); // assuming width of 1


    if ( x0 >= x1 || deltaV > 1 ){
        std::cerr << "should not be in cadran A" << std::endl;
    }



    // we always start at the center of pixel indexes
    float localYTopBorder = 0.5 * (-deltaV) + 0.5 + spaceBetweenTheBordersOfTheLineOnTheYAxis/2 ; // we are actually at pixel (x0, y0)
    float localYBottomBorder = localYTopBorder - spaceBetweenTheBordersOfTheLineOnTheYAxis;



    float realYatX = y0 + localYTopBorder;  // the real Y mean the value of the top border of the line at the value of the xPixelIndex (not the center but the left side)


    if (deltaV <= 0 )
    {

        float percent = -1;
        float percent1 = -1;
        float percent2 = -1;

        int zeroPercentCounter = 0;
        int* xyi;

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

                if (percent != 0 ){

                    xyi = new int[3];
                    xyi[1] = xPixelIndex;
                    xyi[0] = yPixelIndex;
                    xyi[2] = floor(percent * maxIntensity);

                    l->push_back(xyi);


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

                if (percent != 0 ){

                    xyi = new int[3];
                    xyi[1] = xPixelIndex;
                    xyi[0] = yPixelIndex;
                    xyi[2] = floor(percent * maxIntensity);

                    l->push_back(xyi);


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



void imageDrawer::TestSetPixelIdxAndIntensityBasedOnPercentageOfAThickLine()
{


    list<int*>  l ;

    this->setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, ofVec2f(1.5,3.5), ofVec2f(3.5,1.5), 1);

    this->printListIdxAndIntensity(l);


    l.clear();

    this->setPixelIdxAndIntensityBasedOnPercentageOfAThickLineCadranA(&l, 255, 1, 3 ,3, 1);

    this->printListIdxAndIntensity(l);



    l.clear();

    this->setPixelIdxAndIntensityBasedOnPercentageOfAThickLineCadranB(&l, 100, 0, 4 ,1, 0);

    this->printListIdxAndIntensity(l);



    l.clear();

    this->setPixelIdxAndIntensityBasedOnPercentageOfAThickLineCadranB(&l, 100, 0, 0 ,2 ,3);

    this->printListIdxAndIntensity(l);




    l.clear();

    this->setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, ofVec2f(1,1), ofVec2f(3,1), 1);

    this->printListIdxAndIntensity(l);



    l.clear();

    this->setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, ofVec2f(0.2,1.3), ofVec2f(9.09,1), 1);

    this->printListIdxAndIntensity(l);


    l.clear();

    this->setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, ofVec2f(1.5,1.5), ofVec2f(3.5,3.5), 1);

    this->printListIdxAndIntensity(l);







}




void imageDrawer::percentTester(){

    float deltaV = -0.5;
    float actualY = 0.3; // y position from bottomLeft corner
    float value = -1;  // recived value

    if ( this->percentageOfPixeBelowLine(actualY, deltaV ) > 0.089 && this->percentageOfPixeBelowLine(actualY, deltaV ) < 0.091 ) {
        std::cout << "dev-test percent, bottom boundary: succes" << std::endl;
    } else {
         std::cout << "dev-test percent, bottom boundary: fail" << std::endl;
    }

    deltaV = 0.2;
    actualY = 0.9;

    if ( this->percentageOfPixeBelowLine(actualY, deltaV) > 0.974 && this->percentageOfPixeBelowLine(actualY, deltaV ) < 0.976 ) {
        std::cout << "dev-test percent, top boundary: succes" << std::endl;
    } else {
         std::cout << "dev-test percent, top boundary: fail" << std::endl;
    }


    deltaV = -0.3;
    actualY = 0.8;

    value = this->percentageOfPixeBelowLine(actualY, deltaV );
    if ( value > 0.64 && value  < 0.66 ) {
        std::cout << "dev-test percent, oposite boundary: succes" << std::endl;
    } else {
         std::cout << "dev-test percent, oposite boundary: fail" << std::endl;
    }

    deltaV = 0.0;
    actualY = 0.0;

    value = this->percentageOfPixeBelowLine(actualY, deltaV );
    if ( value == 0 ) {
        std::cout << "dev-test percent, extrem value 0: succes" << std::endl;
    } else {
         std::cout << "dev-test percent, extrem value 0: fail" << std::endl;
    }

    deltaV = 1;
    actualY = 1;

    value = this->percentageOfPixeBelowLine(actualY, deltaV );
    if ( value == 1 ) {
        std::cout << "dev-test percent, extrem value only top left: succes" << std::endl;
    } else {
         std::cout << "dev-test percent, extrem value only top left: fail" << std::endl;
    }


    deltaV = 0;
    actualY = 1;

    value = this->percentageOfPixeBelowLine(actualY, deltaV );
    if ( value == 1 ) {
        std::cout << "dev-test percent, extrem value top boundary: succes" << std::endl;
    } else {
         std::cout << "dev-test percent, extrem value top boundary: fail" << std::endl;
    }

    deltaV = -0.4;
    actualY = 0;

    value = this->percentageOfPixeBelowLine(actualY, deltaV );
    if ( value == 0 ) {
        std::cout << "dev-test percent, extrem value bottom right boundary: succes" << std::endl;
    } else {
         std::cout << "dev-test percent, extrem value  bottom right: fail" << std::endl;
    }


    deltaV = 1;
    actualY = 0;

    value = this->percentageOfPixeBelowLine(actualY, deltaV );
    if ( value < 0.51 && value > 0.49 ) {
        std::cout << "dev-test percent, diagonal: succes" << std::endl;
    } else {
         std::cout << "dev-test percent, diagonal: fail" << std::endl;
    }



    deltaV = 0.7;
    actualY = 1.2;

    value = this->percentageOfPixeBelowLine(actualY, deltaV );
    if ( value == 1) {
        std::cout << "dev-test percent, line completely above the pixel: succes" << std::endl;
    } else {
         std::cout << "dev-test percent,  line completely above the pixel: fail" << std::endl;
    }



    deltaV = -0.7;
    actualY = -999;

    value = this->percentageOfPixeBelowLine(actualY, deltaV );
    if ( value == 0) {
        std::cout << "dev-test percent, line completely bellow the pixel: succes" << std::endl;
    } else {
         std::cout << "dev-test percent,  line completely bellow the pixel: fail" << std::endl;
    }


    deltaV = 1;
    actualY = -999;

    value = this->percentageOfPixeBelowLine(actualY, deltaV );
    if ( value == 0) {
        std::cout << "dev-test percent, line completely bellow the pixel 2: succes" << std::endl;
    } else {
         std::cout << "dev-test percent,  line completely bellow the pixel 2: fail" << std::endl;
    }


    deltaV = 0.8;
    actualY = -0.3;

    value = this->percentageOfPixeBelowLine(actualY, deltaV );
    if ( value < 0.16 && value > 0.15) {
        std::cout << "dev-test percent, line start below then cross pixel: succes" << std::endl;
    } else {
         std::cout << "dev-test percent,  line start below then cross pixel: fail" << std::endl;
    }




    deltaV = -0.8;
    actualY = 1.3;

    value = this->percentageOfPixeBelowLine(actualY, deltaV );
    if ( value < 0.844 && value > 0.843) {
        std::cout << "dev-test percent, line start above then cross pixel: succes" << std::endl;
    } else {
         std::cout << "dev-test percent,  line start above then cross pixel: fail" << std::endl;
    }



    deltaV = 3;
    actualY = -1;

    // crash test
    // value = this->percentageOfPixeBelowLine(actualY, deltaV );








}

void imageDrawer::drawPixelsWithIntensity(ofImage &img, list<int *> l)
{

    ofColor color;
    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++){

        color = ofColor( (*it)[2], (*it)[2] , (*it)[2] );
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



























void imageDrawer::freeListOf2Int(list<int *> * l )
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
        std::cout << '(' << (*it)[0] << ',' << (*it)[1] << ',' << (*it)[2] << ") , " ;

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




