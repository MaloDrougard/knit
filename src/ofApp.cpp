
#include "ofApp.h"

//--------------------------------------------------------------

void ofApp::setup(){
    // setup is divid into to part
    // one before the type of shed is know and one after

    //File
    outputFolder = "/home/makem/Cours/knitProject/outputPics/spider/";
    imageFn = "starik2";                                               // but the input file name here!
    pic.load("/home/makem/Cours/knitProject/inputPics/" + imageFn + ".jpg");
    pic.setImageType(OF_IMAGE_COLOR);


    // Type of shed setup
    typeOfShedValidated = false;
    guiStart.setup();
    guiStart.setName("Initialize your shed");
    guiStart.add(typeOfShed.set("gray-additive-sub ", 1,1,3));
    guiStart.add(typeOfShedValidatedBtn.setup("validate your shed"));
    guiStart.setPosition(200,200);
    typeOfShedValidatedBtn.addListener(this, &ofApp::onTypeOfShedValidatedPressed);


}

void ofApp::onTypeOfShedValidatedPressed(){


    if (typeOfShed == 1){
        workshop = new grayShed(pic, imageFn);
    }
    else if (typeOfShed == 2){
        workshop = new colorShed(pic, imageFn);
    }else if (typeOfShed == 3){
        workshop = new substractiveColorShed(pic, imageFn);
    }

    this->setupSecondePart();
    typeOfShedValidated = true;

}



void ofApp::setupSecondePart(){

    saveOption = true;  // if true -> save at each 1000 steps the result image into the output dir
    extraPins =  std::list<ofVec2f> ();

    pinPositionsSaverFn = outputFolder + "pinPositions.dat";
    pinPositionsInputFn = outputFolder + "inputPinPositions.dat";


    numberOfCall = 0;
    computeGridNeeded = true;


    allParameters.setName("Parameters"); 

    allParameters.add(workshop->globalP);
    allParameters.add(workshop->inFlyP);
    allParameters.add(workshop->infoP);


    guiAlgo.setup(allParameters);
    guiAlgo.add(startBtn.setup("Start"));
    guiAlgo.add(launchScript.setup("Lauch background script"));


    guiAlgo.setPosition(workshop->w + 30, 10);


    leftImgParameters.setName("Left image interface");
    leftImgParameters.add(displayOriginal.set("Display original image", true));
    leftImgParameters.add(displayGrid.set("Display grid", false));
    leftImgParameters.add(brushingMode.set("Brushing Mode", false));
    leftImgParameters.add(pinsSettingsMode.set("Pins setting mode", false));
    guiLeftImg.setup(leftImgParameters);
    guiLeftImg.add(saveImagesBtn.setup("Save Images"));

    zoneA = zone();
    zoneA.setup(workshop->w, workshop->h, 20, 20 );

    zoneB = zone();
    zoneB.setup(workshop->w, workshop->h, workshop->w + 40 , 20 );


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
    guiGrid.add(typeWheelInfo.setup("", "1:circle,2:square,3:tribal,4:extra,5:file"));
    typeWheelInfo.setSize(guiGridWidth, typeWheelInfo.getHeight());
    guiGrid.add(typeOfWheel.set("type of grid",1,1,5));
    guiGrid.add(randomifySlightlyPinPositions.set("randomify slightly pin positions", true));
    guiGrid.add(gridValidationBtn.setup("Validtate grid"));

    gridValidationBtn.addListener(this, &ofApp::gridValidation);

    guiGrid.setPosition(10, workshop->h - 100);


    guiBrush.setup();
    guiBrush.setName("Brushing mode parameters");
    guiBrush.add(initialMaskFactorValue.set("Initial mask value", 0,-20,40));
    guiBrush.add(initializeMaskBtn.setup("ReInitialize mask"));
    guiBrush.add(brushValue.set("Brush value",6,-20,40));
    guiBrush.add(initializeBrushBtn.setup("Reinitialize brush"));
    initializeBrushBtn.addListener(this, &ofApp::setupBrush);
    initializeMaskBtn.addListener(this, &ofApp::initializeMask);
    guiBrush.setPosition(10,200);

    wel = NULL;

    stopAlgo = true;
    setupBrush();
}





//--------------------------------------------------------------
void ofApp::update(){

}



void ofApp::draw(){

    if( !typeOfShedValidated )
    {
        guiStart.draw();
    }
    else if(! gridIsValidated){
       if (displayOriginal) {
            zoneA.drawImageInZone(workshop->originalImgCrop);
       }else if (pinsSettingsMode){
           zoneA.drawImageInZone(workshop->originalImgCrop);
       }

       guiLeftImg.draw();
       guiGrid.draw();
    }


    else
    {
        this->draw1();
    }
}



//--------------------------------------------------------------



void ofApp::draw1(){
    // draw procedure if the grid and type of shed are validated



    //  Algo and right image display -------------------------------------

    if (! stopAlgo)
    {
        workshop->computeAndDrawOneStep();
    }

    zoneB.drawImageInZone(workshop->result);



   //Left image display -------------------------------------

    if (displayOriginal) {

        zoneA.drawImageInZone(workshop->originalImgCrop);

    }else if (displayGrid){

       if (computeGridNeeded){
           wel->drawGridRepresentation();
           computeGridNeeded = false;
        }

        zoneA.drawImageInZone(wel->gridRepresentation);

    }

    else if (brushingMode){

        if( workshop->type == "grayShed") {
            ((grayShed*)workshop)->computeBrushedImg();
            zoneA.drawImageInZone( ((grayShed*) workshop)->brushedImg);
            guiBrush.draw();

        }else{
            std::cerr << "Warrning::only gray shed allow brushing mode" << std::endl;
        }

    }
   else if (pinsSettingsMode ){
         wel->drawPins();
         zoneA.drawImageInZone(wel->pinsRepresentation);


    }

    if ((numberOfCall %100 )== 0){ // not clean
        workshop->computeLightnessAbsoluteError();
    }

    if (((workshop->stepsNumberP % 1000 ) == 1) and saveOption ){

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
        if (workshop->type == "grayShed"){
           ((grayShed*) workshop)->drawWithBrushOnMask( relPos[0] , relPos[1] , brush, brushSize);
        }

    }
    else if( pinsSettingsMode)
    {
          if ( rand() % 10 == 0 ) // to do not overcharge the grid
          {
              std::cout << "mouseDragInZoneA "<< relPos[0] << ":"<< relPos[1] << std::endl;
              extraPins.push_front(ofVec2f(relPos[0], relPos[1]));

          }
    }

}



void ofApp::onMousePressedInZoneA(ofVec2f & relPos)
{
    if( pinsSettingsMode)
    {
          std::cout << "mousePressInZoneA "<< relPos[0] << ":"<< relPos[1] << std::endl;
          extraPins.push_front(ofVec2f(relPos[0], relPos[1]));
    }

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
            brush[j][h] = brushValue;
        }
    }
}

void ofApp::initializeMask()
{

    if( workshop->type == "grayShed") {
        ((grayShed*)workshop)->initializeMask(initialMaskFactorValue);
    }else{
        std::cerr << "Warrning::only gray shed allow brushing mode" << std::endl;
    }
}




void ofApp::onSaveImagesPressed()
{



    if (computeGridNeeded){
         wel->drawGridRepresentation();
         computeGridNeeded = false;
     }


    wel->drawPins();
    wel->pinsRepresentation.save(outputFolder + "pins.jpg");

    wel->gridRepresentation.save(outputFolder + "grid.jpg" );
    workshop->saveResultImage(outputFolder);


}

void ofApp::runScript()
{
    scriptUtility s = scriptUtility();
    s.run();

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


        this->wel->initializeLines();
        computeGridNeeded = true;




        gridIsValidated = true;





}

void ofApp::onStartPressed()
{
    stopAlgo = false;

}

