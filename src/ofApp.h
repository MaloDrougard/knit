#pragma once

#include <ctime>
#include <chrono>
#include <iostream>

#include "ofMain.h"
#include "wheel.h"
#include "imagedrawer.h"
#include "shed.h"
#include "zone.h"
#include "scriptutility.h"
#include "colorshed.h"


#include "ofxGui.h"

class ofApp : public ofBaseApp{

public:

    ofParameterGroup allParameters;
    ofParameter<bool> oneRandom ;
    ofParameter<bool> stopAlgo;
    ofxButton startBtn;
    ofxButton launchScript;



    ofParameterGroup leftImgParameters;
    ofParameter<bool> displaySketch ;
    ofParameter<bool> brushingMode; // display the brushed image and allow user to draw with the brush
    ofParameter<bool> displayGrid ;
    bool computeGridNeeded; // to know if we need to calculate the grid
    ofParameter<bool> displayOriginal;
    ofxButton saveImagesBtn;


    string outputFolder;
    string pinPositionsSaverFn; // where the position of pins are saved in case of save
    string pinPositionsInputFn; // where the wheelFromFile read is file
    ofParameter<bool> saveOption;
    string imageFn;
    ofImage pic;
    colorShed * workshop;
    int numberOfCall;

    ofxPanel guiAlgo;
    ofxPanel guiLeftImg;





    zone zoneA;
    zone zoneB;

    float ** brush ;
    int brushSize ;



    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void setupBrush();
    void onMouseInZoneA(ofVec2f &relPos);
    void onStartPressed();

    void onSaveImagesPressed();

    void runScript();


    // for pins settings
    ofParameter<bool> pinsSettingsMode;
    std::list<ofVec2f> extraPins;
    void onMousePressedInZoneA(ofVec2f &relPos);


    ofxPanel guiGrid;

    ofParameter<int> numberOfPins;
    ofxLabel typeWheelInfo;
    ofParameter<int> typeOfWheel;
    ofParameter<bool> randomifySlightlyPinPositions;
    ofxButton gridValidationBtn;

    abstractWheel * wel;


    void gridValidation();

};
