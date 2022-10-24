#include "zone.h"


zone::zone() {
    bRegisteredEvents = false;
}

zone::~zone() {
    clear();
}

void zone::setup(int w, int h, int starPosX, int startPosY){

    this->w = w;
    this->h = h;
    this->startPosX = starPosX;
    this->startPosY = startPosY;
    this->bgColor = ofColor::orange;

    if(!bRegisteredEvents) {
        ofRegisterMouseEvents(this); // this will enable our zone class to listen to the mouse events.
        bRegisteredEvents = true;
    }
}

void zone::clear() {
    if(bRegisteredEvents) {
        ofUnregisterMouseEvents(this); // disable listening to mouse events.
        bRegisteredEvents = false;
    }
}

void zone::drawBackground() {

    ofPushStyle();
    ofSetColor(this->bgColor);
    ofDrawRectangle(startPosX, startPosY, w, h );
    ofPopStyle();
}

void zone::drawBackground(ofColor color) {

    ofPushStyle();
    ofSetColor(color);
    ofDrawRectangle(startPosX, startPosY, w, h );
    ofPopStyle();
}


void zone::drawImageInZone(ofImage &img)
{
    img.draw(startPosX, startPosY);
}

void zone::mouseMoved(ofMouseEventArgs & args){}

void zone::mouseDragged(ofMouseEventArgs & args){

    if (inside(args.x, args.y)) { // insside make sure that the relativex and Y are set

        ofVec2f mousePos = ofVec2f(relativeX, relativeY);
        ofNotifyEvent(dragInside, mousePos, this);

    }

}


void zone::mousePressed(ofMouseEventArgs & args){

    if (inside(args.x, args.y)) { // insside make sure that the relativex and Y are set

        ofVec2f mousePos = ofVec2f(relativeX, relativeY);
        ofNotifyEvent(mousePressedInside, mousePos, this);

    }

}
void zone::mouseReleased(ofMouseEventArgs & args){}
void zone::mouseScrolled(ofMouseEventArgs & args){}
void zone::mouseEntered(ofMouseEventArgs & args){}
void zone::mouseExited(ofMouseEventArgs & args){}

//this function checks if the passed arguments are inside the zone.
bool zone::inside(int x, int y ){
    bool ret = false;

    relativeX = x - startPosX;
    relativeY = y - startPosY;

    if (( relativeX < w ) and ( relativeX >= 0 ) and (relativeY < h) and (relativeY >= 0 ) ){
        ret = true;
    }

    return ret ;
}
