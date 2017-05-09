#include "genericshed.h"

genericShed::genericShed(ofImage inputImg, string imageNameInput)
{
    // take only care of the inputImage
    originalImg = inputImg;
    imageName = imageNameInput;

    type = "generic";

    setOriginalImgCrop();
    setWandH();

    drawer = imageDrawer();

}


void genericShed::setWandH()
{
    if ( ! originalImgCrop.isAllocated()  ){
        std::cerr << "Error in genericShed: to call setupWandH originalImgCrop must be allocate. \nUse setOriginalImgCrop()" << std::endl;
        throw -7;
    }
    w = originalImgCrop.getWidth();
    h = originalImgCrop.getHeight();

}

void genericShed::setupOfParameter()
{

    globalP.setName("Global Algorithm Parameters");
    globalP.add(maxStepsNumberP.set("max #steps", 100003, -1, 40000) );
    inFlyP.setName("In Fly Parameters");
    inFlyP.add(drawOpacityP.set("Draw Opacity", 9, 0, 255));
    infoP.setName("Process infos");
    infoP.add(stepsNumberP.set("#steps",0, 0, 100000));
    int temp = this->computeLightnessAbsoluteError();
    infoP.add(lightnessAbsoluteError.set("absolutError",temp,0,temp+1000));
}


void genericShed::setOriginalImgCrop()
{
    int oriW = originalImg.getWidth();
    int oriH = originalImg.getHeight();

    originalImgCrop.clone(originalImg);

    int diff = 0;
    if( oriW > oriH ){
        diff = oriW - oriH;
        originalImgCrop.crop(diff/2, 0, oriH, oriH);
    } else {
        diff = oriH - oriW;
        originalImgCrop.crop(0, diff/2,  oriW, oriW);
    }
    originalImgCrop.update();
}


void genericShed::setEmptyResult(ofColor backgroundColor)
{
    result.allocate(w, h , OF_IMAGE_COLOR);
    result.setColor(backgroundColor);
    result.update();

}



void genericShed::setupWheel(abstractWheel wel)
{

    // becarefull lines in wel is create using *new* keyword
    // It'is the responsability of the caller of this function to destroy lines at the right moment

    this->wel = wel;
}

string genericShed::getBasicInfos()
{
    this->computeLightnessAbsoluteError();

    string para =   "-si:" + imageName    // image name
                    + "-ss:" + std::to_string(stepsNumberP)  // steps number
                    + "-st:" + type     // shed type
                    + "-so:" + std::to_string(drawOpacityP)
                    + "-se:"+ std::to_string(lightnessAbsoluteError);
    return para;

}

string genericShed::getInfos()
{
    return getBasicInfos();
}


int genericShed::saveResultImage(string folder)
{

    char buff[30];
    time_t now = time(NULL);
    strftime(buff, 30, "d:%Y%m%d-h:%H%M%S", localtime(&now));

    string para =   getInfos()
                    + wel.getInfos();

    string name =    std::string(buff)
                    + para
                    + ".jpg";


    result.save(folder + name);
    std::cout << "result image saved in: " << folder + name << std::endl;

    return 1;
}


void genericShed::computeAndDrawOneStep()
{
    std::cout << "computeAndDrawOneStep must be implemented in the derivative class, thx" << std::endl;
}

int genericShed::computeLightnessAbsoluteError()
{

    if  ( (!originalImgCrop.isAllocated()) or (!result.isAllocated()) ) {
        std::cerr << "Error in genericShed: To call computeAbsoluteError() originalImgCrop and result image must be allocate";
        throw -7;
    }

    int diff  = 0;
    int tempLigthnessDiff = 0;

    for(int x = 0; x < w; x++ ){
        for(int y = 0; y < h; y++){
               tempLigthnessDiff = (originalImgCrop.getColor(x,y)).getLightness() - (result.getColor(x,y)).getLightness();
               tempLigthnessDiff = abs(tempLigthnessDiff);
               diff += tempLigthnessDiff;
        }
    }

    lightnessAbsoluteError.set(diff);

    return diff;

}


