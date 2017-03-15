#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


    pic.load("/home/makem/Cours/knitProject/inputPics/MMM-highcontrast.jpg");
    pic.setImageType(OF_IMAGE_COLOR);

    workshop = new shed(pic);

    numberOfCall = 0;

    allParameters.setName("Parameters");
    allParameters.add(oneRandom.set("Randomify next pin", false));
    allParameters.add(workshop->shedParameter);


    gui.setup(allParameters);


}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    // workshop->checkchange();
    
    std::cout << "call to drawOne number: " <<  numberOfCall << std::endl;

    if(oneRandom){
        workshop->randomifyNextPinAndDrawOneString();
    }

    else {
        workshop->computeNextPinAndDrawOneString();

    }

    workshop->originalImg.draw(20,20);
    workshop->result.draw(720, 20);
    gui.draw();

    numberOfCall++;
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
