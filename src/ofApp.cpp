#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


    pic.load("/home/makem/Cours/knitProject/inputPics/4px.jpg");
    pic.setImageType(OF_IMAGE_GRAYSCALE);


    // crop the image with the bigest square
    int w = pic.getWidth();
    int h = pic.getHeight();
    int diff = 0;

    if( w > h ){
        diff = w - h;
        pic.crop(diff/2, 0, h, h);
    } else {
        diff = h - w;
        pic.crop(0, diff/2,  w, w);
    }

    pic.update();

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPoint* verts = new ofPoint[5];
// make a pentagon
    float size = 500.f;
    float X1 = 0.125*sqrt(10 + 2*sqrt(5)) * size;
    float X2 = 0.125*sqrt(10 - 2*sqrt(5)) * size;
    float Y1 = 0.125*(sqrt(5) - 1) * size;
    float Y2 = 0.125*(sqrt(5) + 1) * size;
    verts[0] = ofPoint(0, -0.5 * size);
    verts[1] = ofPoint(-X1, -Y1);
    verts[2] = ofPoint(-X2, Y2);
    verts[3] = ofPoint(X2, Y2);
    verts[4] = ofPoint(X1, -Y1);

    line.addVertices(verts, 5);
    line.draw();


    pic.draw(200,200);

    ofPoint p1 ;
    p1.set(-0.5, 3 );

    ofPoint l1;
    l1.set(-1,1);

    ofPoint l2;
    l2.set(1,3);

    float t = 90;

    wheel w = wheel(pic);

    ofVec2f pins[8] ;
    w.generatePins(pins, 8, float (1.0), ofVec2f(1,1) );






}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
