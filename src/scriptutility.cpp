#include "scriptutility.h"


scriptUtility::scriptUtility()
{

}

void scriptUtility::run()
{

    string imageFn = "not set yet";

    //pics settings
    string outputFolder = "/home/makem/Cours/knitProject/outputPics/";
    string imagesFn [6] =  { "abstract2", "elgreco2" , "dance2", "starik2", "tree32", "tree42"};
    int imagesFnSize = 6;


    // position settings
    string pinPositionsSaverFn = outputFolder + "pinPositions.dat";
    string pinPositionsInputFn = outputFolder + "inputPinPositions.dat";


    for ( int i = 0; i < imagesFnSize; i++ ){

        ofImage pic;
        string imageFn = imagesFn[i];
        pic.load("/home/makem/Cours/knitProject/inputPics/" + imageFn + ".jpg");
        pic.setImageType(OF_IMAGE_COLOR);

        genericShed * workshop = new substractiveColorShed(pic, imageFn);

        abstractWheel wel = wheelFromFile(pinPositionsInputFn, workshop->w, workshop->h);
        wel.setup();

        workshop->setupWheel(wel);

        while ( workshop->stepsNumberP < workshop->maxStepsNumberP ){
            workshop->computeAndDrawOneStep();
            if ((workshop->stepsNumberP % 1000) == 1){

                workshop->saveResultImage(outputFolder);

            }
        }

        wel.destroy();
        delete workshop;

    }


}
