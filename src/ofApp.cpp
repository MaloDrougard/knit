#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


    pic.load("/home/makem/Cours/knitProject/inputPics/el-greco-bart-face-1.jpg");
    pic.setImageType(OF_IMAGE_COLOR);

    workshop = new shed(pic);

    numberOfCall = 0;

    allParameters.setName("Parameters");
    allParameters.add(oneRandom.set("Randomify next pin", false));
    allParameters.add(workshop->shedParameter);


    gui.setup(allParameters);


    setupBrush();


    zoneA = zone();
    zoneA.setup(workshop->w, workshop->h, 20, 20 );


    zoneB = zone();
    zoneB.setup(workshop->w, workshop->h, workshop->w + 40 , 20 );

    buttonA = zone();
    buttonA.setup(40, 40,  20, workshop->h + 40);


    ofAddListener(zoneA.dragInside, //the ofEvent that we want to listen to. In this case exclusively to the circleEvent of redCircle (red circle) object.
                  this, //pointer to the class that is going to be listening. it can be a pointer to any object. There's no need to declare the listeners within the class that's going to listen.
                  &ofApp::onMouseInZoneA);//pointer to the method that's going to be called when a new event is broadcasted (callback method). The parameters of the event are passed to this method.


    ofAddListener(buttonA.mousePressedInside, //the ofEvent that we want to listen to. In this case exclusively to the circleEvent of redCircle (red circle) object.
                  this, //pointer to the class that is going to be listening. it can be a pointer to any object. There's no need to declare the listeners within the class that's going to listen.
                  &ofApp::onMousePressedOnButtonA);//pointer to the method that's going to be called when a new event is broadcasted (callback method). The parameters of the event are passed to this method.



    workshop->setGrid();


}




void ofApp::setupBrush()
{
    brushSize = 13;
    brush = new float * [brushSize];
    for (int i = 0; i < brushSize; i++ ){
        brush[i] = new float [brushSize];
    }

    for (int j = 0; j < brushSize; j++ ){
        for (int h = 0; h < brushSize; h++ ){
            brush[j][h] = 6;
        }
    }
}




//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    // workshop->checkchange();
    
    // std::cout << "call to drawOne number: " <<  numberOfCall << std::endl;

    /*
    if(oneRandom){
        workshop->randomifyNextPinAndDrawOneString();
    }

    else {
        workshop->computeNextPinAndDrawOneString();

    }
*/
    //workshop->computeLeftDisplayImg();
    //workshop->sketchImg.update();

    zoneA.drawImageInZone(workshop->gridImg) ;//workshop->sketchImg);
    zoneB.drawImageInZone(workshop->result);


    gui.draw();
    buttonA.drawBackground();

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
    std::cout << "mouseDragges "<< x << ":"<< y << std::endl;

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

void ofApp::onMouseInZoneA( ofVec2f & relPos){

    std::cout << "mouseDraggesInZoneA "<< relPos[0] << ":"<< relPos[1] << std::endl;
    workshop->brushMask( relPos[0] , relPos[1] , brush, brushSize);
}


void ofApp::onMousePressedOnButtonA(ofVec2f & relPos){

    std::cout << "mousePressedInButtonA "<< relPos[0] << ":"<< relPos[1] << std::endl;
    time_t now = time(0);
    string fn = std::string("/home/makem/Cours/knitProject/outputPics/") + ctime(&now) + ".jpg";
    workshop->gridImg.save(fn);

}
