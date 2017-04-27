#include "scriptutility.h"


scriptUtility::scriptUtility()
{

}

void scriptUtility::run()
{

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

        shed * workshop = new shed(pic);

        abstractWheel wel = wheelFromFile(pinPositionsInputFn, workshop->w, workshop->h);
        wel.setup();

        workshop->setupWheel(wel);

        while ( workshop->numberStringP < workshop->maxNumberStringP ){
            workshop->computeNextPinAndDrawOneString();
            if ((workshop->numberStringP % 1000) == 1){

                string name = this->getSavingName(*workshop, imageFn);
                workshop->computeDiffOrignalResult();
                workshop->result.save(outputFolder + name);
                std::cout << "result saved in " << outputFolder + name << std::endl;
            }
        }

        wel.destroy();
        delete workshop;

    }





}


string scriptUtility::getSavingName(shed workshop, string imageFn)
{

    char buff[30];
    time_t now = time(NULL);
    strftime(buff, 30, "d:%Y%m%d-h:%H%M%S", localtime(&now));

    string para =   "-n:" + imageFn
                    + "-s:"+ std::to_string(workshop.numberStringP)
                    + "-p:"+ std::to_string(workshop.numberPinsP)
                    + "-ao:" + std::to_string(workshop.algoOpacityP)
                    + "-do:" + std::to_string(workshop.drawOpacityP)
                    + "-sf:" + "wheigtedExtremity3"
                    + "-e:"+ std::to_string(workshop.diffError);

    string name =    std::string(buff)
                    + "-i:result"
                    + para
                    + ".jpg";

    return name;

}

