#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


    pic.load("/home/makem/Cours/knitProject/inputPics/el-greco-bart-face-1.jpg");
    pic.setImageType(OF_IMAGE_COLOR);

    workshop = new shed(pic);

    numberOfCall = 0;

    allParameters.setName("Parameters");
    allParameters.add(oneRandom.set("Randomify next pin", false));
    allParameters.add(stopAlgo.set("Stop Algorithm", false));
    allParameters.add(workshop->shedParameter);
    guiAlgo.setup(allParameters);


    leftImgParameters.setName("Left image interface");
    leftImgParameters.add(displaySketch.set("Display sketching image", true));
    leftImgParameters.add(displayOriginal.set("Display original image", false));
    leftImgParameters.add(displayGrid.set("Display grid", false));
    leftImgParameters.add(brushingMode.set("Allow to brush", false));

    guiLeftImg.setup(leftImgParameters);
    guiLeftImg.add(saveLeftImgBtn.setup("Save Image"));



    restartParamaters.setName("Restart");
    restartParamaters.add(pinsNumberP.set("#pins", 200, 2, 600));

    guiRestart.setup(restartParamaters);
    guiRestart.add(restartBtn.setup("Restart"));




    zoneA = zone();
    zoneA.setup(workshop->w, workshop->h, 20, 20 );

    zoneB = zone();
    zoneB.setup(workshop->w, workshop->h, workshop->w + 40 , 20 );

    guiAlgo.setPosition(workshop->w + 30, 10);
    guiRestart.setPosition(workshop->w +30, workshop->h +30);

    ofAddListener(zoneA.dragInside, //the ofEvent that we want to listen to. In this case exclusively to the circleEvent of redCircle (red circle) object.
                  this, //pointer to the class that is going to be listening. it can be a pointer to any object. There's no need to declare the listeners within the class that's going to listen.
                  &ofApp::onMouseInZoneA);//pointer to the method that's going to be called when a new event is broadcasted (callback method). The parameters of the event are passed to this method.

    restartBtn.addListener(this, &ofApp::onRestartPressed);



    setupBrush();
    //workshop->drawGridOnImg();

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




    //  Algo and right image display -------------------------------------

    if(oneRandom && !stopAlgo)
    {
        workshop->randomifyNextPinAndDrawOneString();
    }
    else if (! stopAlgo)
    {
        workshop->computeNextPinAndDrawOneString();
    }

    zoneB.drawImageInZone(workshop->result);



   //    Left image display -------------------------------------

   if ( displaySketch ) {

        workshop->sketchImg.update();
        zoneA.drawImageInZone(workshop->sketchImg);

    }else if (displayOriginal) {

        zoneA.drawImageInZone(workshop->originalImgCrop);

    }else if (displayGrid){

        //workshop->setGrid();
        zoneA.drawImageInZone(workshop->gridImg);

    }else if (brushingMode){

        workshop->computeBrushedImg();
        zoneA.drawImageInZone(workshop->brushedImg);

    }


    if ((numberOfCall %100 )== 0){ // not clean
        workshop->computeDiffOrignalResult();
    }


    guiAlgo.draw();
    guiLeftImg.draw();
    guiRestart.draw();

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

    if ( brushingMode){
        std::cout << "mouseDraggesInZoneA "<< relPos[0] << ":"<< relPos[1] << std::endl;
        workshop->drawWithBrushOnMask( relPos[0] , relPos[1] , brush, brushSize);
    }

}

void ofApp::onRestartPressed()
{
    workshop->restart(pinsNumberP);
}

