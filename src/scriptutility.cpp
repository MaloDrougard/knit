#include "scriptutility.h"


scriptUtility::scriptUtility()
{

}

void scriptUtility::run()
{
/*
    string imageFn = "not set yet";

    //pics settings
    string outputFolder = "/home/makem/Cours/knitProject/outputPics/";
    string imagesFn [7] =  { "abstract1", "city1", "dance1", "elgreco1", "starik1", "tree3", "tree4"};
    int imagesFnSize = 7;


    // position settings
    string pinPositionsSaverFn = outputFolder + "pinPositions.dat";
    string pinPositionsInputFn = outputFolder + "inputPinPositions.dat";


    for ( int i = 0; i < imagesFnSize; i++ ){

        ofImage pic;
        string imageFn = imagesFn[i];
        pic.load("/home/makem/Cours/knitProject/inputPics/" + imageFn + ".jpg");
        pic.setImageType(OF_IMAGE_COLOR);

        genericShed * workshop = new grayShed(pic, imageFn);

        abstractWheel wel = wheelFromFile(pinPositionsInputFn, workshop->w, workshop->h);
        wel.setup();

        workshop->setupWheel(wel);

        while ( workshop->numberStringP < workshop->maxNumberStringP ){
            workshop->computeNextPinAndDrawOneString();
            if ((workshop->numberStringP % 1000) == 1){

                string name = this->getSavingName(workshop, imageFn);
                workshop->computeDiffOrignalResult();
                workshop->result.save(outputFolder + name);
                std::cout << "result saved in " << outputFolder + name << std::endl;
            }
        }

        wel.destroy();
        delete workshop;

    }


*/


}


string scriptUtility::getSavingName(genericShed*  workshop, string imageFn)
{



}

