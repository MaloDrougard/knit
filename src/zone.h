#ifndef ZONE_H
#define ZONE_H



#include "ofMain.h"


/*
 * Define an zone that fire an event "dragInside" if the mouse is drag inside this zone
*/

class zone
{
public:
    zone();


    ofEvent<ofVec2f> dragInside; // fire the coordinate relativily to this zone

    int w;
    int h;
    int startPosX;
    int startPosY;

    int relativeX;
    int relativeY;


    //We need to declare all this mouse events methods to be able to listen to mouse events.
    //All this must be declared even if we are just going to use only one of this methods.
    void mouseMoved(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void mousePressed(ofMouseEventArgs & args);
    void mouseReleased(ofMouseEventArgs & args);
    void mouseScrolled(ofMouseEventArgs & args);
    void mouseEntered(ofMouseEventArgs & args);
    void mouseExited(ofMouseEventArgs & args);


    void setup(int w, int h, int starPosX, int startPosY);
    void clear();


    void drawImageInZone(ofImage & img );

    bool inside(int x, int y);


    ~zone();


protected:
        bool bRegisteredEvents;





};

#endif // ZONE_H
