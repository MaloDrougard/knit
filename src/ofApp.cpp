#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


    pic.load("/home/makem/Cours/knitProject/inputPics/MMM-highcontrast.jpg");
    pic.setImageType(OF_IMAGE_COLOR);

    workshop = new shed(pic);

    workshop->computeStringPath();
    workshop->drawString();

    gui.setup(workshop->shedParameter);



}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    workshop->checkchange();

    workshop->originalImg.draw(50,50);
    workshop->result.draw(749, 50);

    gui.draw();

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
