#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


    pic.load("/home/makem/Cours/knitProject/inputPics/640px.jpg");
    pic.setImageType(OF_IMAGE_COLOR);


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

    // We will, always use the index of pixel has coordinate.
    // Thus, the y axis are incresing in the oposite direction as usual

    ofVec2f centerWheel = ofVec2f( w/2 , w/2 );
    float radius = (w-1)/2.0 ;    // we want not to be at border but inside

    wheel wel = wheel(16, radius, centerWheel);

    imageDrawer drawer = imageDrawer();
    drawer.drawPins(pic, wel.pins, wel.pinsNumber);

    list<int * > l = drawer.getPixelIdxOfALine(pic, wel.pins[wel.pinsNumber -1 ], wel.pins[0]);
    drawer.printListIdx(l);

    for (int i = 0; i < wel.pinsNumber - 1 ; i++){
       drawer.drawALine(pic, wel.pins[i], wel.pins[i + 1]);
    }
    drawer.drawALine(pic, wel.pins[wel.pinsNumber -1 ], wel.pins[0]);


}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofPoint* verts = new ofPoint[5];


    pic.draw(300,300);






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
