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


void imageDrawer::freeListOf2Int(list<int *> * l )
{
    for(std::list<int *>::iterator it = l->begin(); it  != l->end(); it++){
        delete[] *it;
    }
    l->clear();



}


void imageDrawer::printListIdx(list<int * > l ){


    std::cout << "print list:\n";

    for (std::list<int * >::iterator it = l.begin(); it != l.end(); it++)
        std::cout << (*it)[0] << ',' << (*it)[1] << '-';

    std::cout << '\n';

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




