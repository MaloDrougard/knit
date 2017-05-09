
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){



    saveOption = false;
    extraPins =  std::list<ofVec2f> ();


    outputFolder = "/home/makem/Cours/knitProject/outputPics/";
    imageFn = "elgreco2";
    pic.load("/home/makem/Cours/knitProject/inputPics/" + imageFn + ".jpg");
    pic.setImageType(OF_IMAGE_COLOR);


    pinPositionsSaverFn = outputFolder + "pinPositions.dat";
    pinPositionsInputFn = outputFolder + "inputPinPositions.dat";

    workshop = new substractiveColorShed(pic);

    numberOfCall = 0;
    computeGridNeeded = true;

    allParameters.setName("Parameters"); 

    allParameters.add(workshop->globalP);
    allParameters.add(workshop->infoP);


    guiAlgo.setup(allParameters);
    guiAlgo.add(startBtn.setup("Start"));
    guiAlgo.add(launchScript.setup("Lauch background script"));


    leftImgParameters.setName("Left image interface");
    leftImgParameters.add(displayOriginal.set("Display original image", false));
    leftImgParameters.add(displayGrid.set("Display grid", false));
    guiLeftImg.setup(leftImgParameters);
    guiLeftImg.add(saveImagesBtn.setup("Save Images"));




    zoneA = zone();
    zoneA.setup(workshop->w, workshop->h, 20, 20 );

    zoneB = zone();
    zoneB.setup(workshop->w, workshop->h, workshop->w + 40 , 20 );

    guiAlgo.setPosition(workshop->w + 30, 10);

    ofAddListener(zoneA.dragInside, //the ofEvent that we want to listen to. In this case exclusively to the circleEvent of redCircle (red circle) object.
                  this, //pointer to the class that is going to be listening. it can be a pointer to any object. There's no need to declare the listeners within the class that's going to listen.
                  &ofApp::onMouseInZoneA);//pointer to the method that's going to be called when a new event is broadcasted (callback method). The parameters of the event are passed to this method.

    ofAddListener(zoneA.mousePressedInside,  this, &ofApp::onMousePressedInZoneA);

    saveImagesBtn.addListener(this, &ofApp::onSaveImagesPressed);
    startBtn.addListener(this, &ofApp::onStartPressed );
    launchScript.addListener(this, &ofApp::runScript );


    int guiGridWidth = 300;

    guiGrid.setup();
    guiGrid.setSize(guiGridWidth, 100);
    guiGrid.setName("Grid parameters");
    guiGrid.add(numberOfPins.set("#pins", 240, 4, 720));
    guiGrid.add(typeWheelInfo.setup("", "1=circle,2=square,3=tribal,4=extra,5=file"));
    typeWheelInfo.setSize(guiGridWidth, typeWheelInfo.getHeight());
    guiGrid.add(typeOfWheel.set("type of grid",1,1,5));
    guiGrid.add(randomifySlightlyPinPositions.set("randomify slightly pin positions", true));
    guiGrid.add(gridValidationBtn.setup("Validtate grid"));

    gridValidationBtn.addListener(this, &ofApp::gridValidation);

    guiGrid.setPosition(260, 20);

    wel = NULL;

    oneRandom = false;
    stopAlgo = true;

}





//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    // workshop->checkchange();WeilghByMaskFactor
    
    // std::cout << "call to drawOne number: " <<  numberOfCall << std::endl;




    //  Algo and right image display -------------------------------------

    if(oneRandom && !stopAlgo)
    {
       // workshop->randomifyNextPinAndDrawOneString();
    }
    else if (! stopAlgo)
    {
        workshop->computeNextStepAndDrawThreeStrings();
    }

    zoneB.drawImageInZone(workshop->result);



   //    Left image display -------------------------------------

   if ( displaySketch ) {

        //workshop->sketchImg.update();
        //zoneA.drawImageInZone(workshop->sketchImg);

    }else if (displayOriginal) {

        zoneA.drawImageInZone(workshop->originalImgCrop);

    }else if (displayGrid){


       if (computeGridNeeded){
           wel->drawGridRepresentation();
           computeGridNeeded = false;
        }

        zoneA.drawImageInZone(wel->gridRepresentation);

    }else if (brushingMode){

//        workshop->computeBrushedImg();
//        zoneA.drawImageInZone(workshop->brushedImg);

    }
   else if (pinsSettingsMode){

 //      wel->drawPins();
 //      zoneA.drawImageInZone(wel->pinsRepresentation);

   }


    if ((numberOfCall %100 )== 0){ // not clean
        workshop->computeLightnessAbsoluteError();
    }

    if (((workshop->numberStringP % 1000 ) == 1) and saveOption ){

        this->onSaveImagesPressed();

    }



    guiAlgo.draw();
    guiLeftImg.draw();
    guiGrid.draw();

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
        //workshop->drawWithBrushOnMask( relPos[0] , relPos[1] , brush, brushSize);
    }
    else if( pinsSettingsMode)
    {
          std::cout << "mousePressInZoneA "<< relPos[0] << ":"<< relPos[1] << std::endl;
          //extraPins.push_front(ofVec2f(relPos[0], relPos[1]));

    }

}







void ofApp::onSaveImagesPressed()
{

/*
    char buff[30];
    time_t now = time(NULL);
    strftime(buff, 30, "d:%Y%m%d-h:%H%M%S", localtime(&now));

    string para =   "-n:" + imageFn
                    + "-s:"+ std::to_string(workshop->numberStringP)
                    + "-p:"+ std::to_string(workshop->numberPinsP)
                    + "-ao:" + std::to_string(workshop->algoOpacityP)
                    + "-do:" + std::to_string(workshop->drawOpacityP)
                    + "-sf:" + "equilibrate"
                    + "-e:"+ std::to_string(workshop->diffError);

    string name =    std::string(buff)
                    + "-i:result"
                    + para
                    + ".jpg";

    workshop->result.save(outputFolder + name);
    std::cout << "Image: " << name << " is saved in " << outputFolder << std::endl;

    name =    std::string(buff)
                    + "-i:sketch"
                    + para
                    + ".jpg";

    workshop->sketchImg.save(outputFolder + name);
    std::cout << "Image: " << name << " is saved in " << outputFolder << std::endl;



    name =    std::string(buff)
                    + "-i:originalcrop"
                    + para
                    + ".jpg";

    workshop->originalImgCrop.save(outputFolder + name);
    std::cout << "Image: " << name << " is saved in " << outputFolder << std::endl;


    if (computeGridNeeded){
        wel->drawGridRepresentation();
        computeGridNeeded = false;
     }


    name =    std::string(buff)
                    + "-i:grid"
                    + para
                    + ".jpg";
    wel->gridRepresentation.save(outputFolder + name);
    std::cout << "Image: " << name << " is saved in " << outputFolder << std::endl;
*/

}

void ofApp::runScript()
{
    scriptUtility s = scriptUtility();
    s.run();

}

void ofApp::onMousePressedInZoneA(ofVec2f & relPos)
{
    if( pinsSettingsMode)
    {
          std::cout << "mousePressInZoneA "<< relPos[0] << ":"<< relPos[1] << std::endl;
          extraPins.push_front(ofVec2f(relPos[0], relPos[1]));
    }

}

void ofApp::gridValidation()
{

        if (wel == NULL) {
            wel = new abstractWheel();
        }else{
            this->wel->destroy();
        }

        int w = workshop->w;
        int h = workshop->h;

        switch (typeOfWheel){

            case 1: *wel = wheelCircle(numberOfPins, w ,h );
                    break;
            case 2: *wel = wheelSquare(numberOfPins, w);
                    break;
            case 3: *wel = wheelTribal(numberOfPins, w, h);
                    break;
            case 4: *wel = wheelExtra(numberOfPins, w, h, extraPins);
                    break;
            case 5: *wel = wheelFromFile(pinPositionsInputFn, w, h);
                    break;
        }

        numberOfPins = this->wel->pinsNumber; // in some case the number of pins change during the implementation of the whell

        if (randomifySlightlyPinPositions){
            this->wel->setupWithRandomification();
        } else {
            this->wel->setup();
        }

        this->workshop->setupWheel(*wel);
        this->wel->writePinPositionsToFile(pinPositionsSaverFn);




}

void ofApp::onStartPressed()
{
    stopAlgo = false;

}

