#ifndef GENERICSHED_H
#define GENERICSHED_H


#include "wheel.h"
#include "imagedrawer.h"

class genericShed
{
public:
    genericShed(ofImage oriImg, string imageNameInput);

    string type ; // type of the shed

    int w ; // width of the image we use
    int h ; // height of the image we use

    string imageName;  // the name of the image in use

    ofImage originalImg;
    ofImage originalImgCrop; // original image resize to be square
    ofImage result; // result image, note that the image is updating during the process

    ofParameterGroup infoP;
    ofParameter<int> stepsNumberP; // number of step untily this point, a step is a call of computeAndDrawNextStep
    ofParameter<int> lightnessAbsoluteError; // absoult error between the orignal image and the result

    ofParameterGroup globalP;
    ofParameter<int> maxStepsNumberP; // set -1 for infinity

    ofParameterGroup inFlyP;
    ofParameter<int> drawOpacityP; // the string opacity


    abstractWheel wel; // the grid
    imageDrawer drawer; // utility to acces images


    void setWandH();
    void setupOfParameter();
    void setOriginalImgCrop();
    void setEmptyResult(ofColor backgroundColor);
    void setupWheel(abstractWheel wel);

    virtual int saveResultImage(string folder);
    string getBasicInfos();
    virtual string getInfos();

    virtual void computeAndDrawOneStep();

    int computeLightnessAbsoluteError();// differance between the result and the original (counting the lightness differance)

};

#endif // GENERICSHED_H
