#include "test.h"

test::test()
{





}




void test::imageDrawerPercent(){

    imageDrawer drawer = imageDrawer();

    float deltaV = -0.5;
    float actualY = 0.3; // y position from bottomLeft corner
    float value = -1;  // recived value

    if ( drawer.percentageOfPixeBelowLine(actualY, deltaV ) > 0.089 && drawer.percentageOfPixeBelowLine(actualY, deltaV ) < 0.091 ) {
        std::cout << "***dev-test percent, bottom boundary: succes" << std::endl;
    } else {
         std::cout << "***dev-test percent, bottom boundary: fail" << std::endl;
    }

    deltaV = 0.2;
    actualY = 0.9;

    if ( drawer.percentageOfPixeBelowLine(actualY, deltaV) > 0.974 && drawer.percentageOfPixeBelowLine(actualY, deltaV ) < 0.976 ) {
        std::cout << "***dev-test percent, top boundary: succes" << std::endl;
    } else {
         std::cout << "***dev-test percent, top boundary: fail" << std::endl;
    }


    deltaV = -0.3;
    actualY = 0.8;

    value = drawer.percentageOfPixeBelowLine(actualY, deltaV );
    if ( value > 0.64 && value  < 0.66 ) {
        std::cout << "***dev-test percent, oposite boundary: succes" << std::endl;
    } else {
         std::cout << "***dev-test percent, oposite boundary: fail" << std::endl;
    }

    deltaV = 0.0;
    actualY = 0.0;

    value = drawer.percentageOfPixeBelowLine(actualY, deltaV );
    if ( value == 0 ) {
        std::cout << "***dev-test percent, extrem value 0: succes" << std::endl;
    } else {
         std::cout << "***dev-test percent, extrem value 0: fail" << std::endl;
    }

    deltaV = 1;
    actualY = 1;

    value = drawer.percentageOfPixeBelowLine(actualY, deltaV );
    if ( value == 1 ) {
        std::cout << "***dev-test percent, extrem value only top left: succes" << std::endl;
    } else {
         std::cout << "***dev-test percent, extrem value only top left: fail" << std::endl;
    }


    deltaV = 0;
    actualY = 1;

    value = drawer.percentageOfPixeBelowLine(actualY, deltaV );
    if ( value == 1 ) {
        std::cout << "***dev-test percent, extrem value top boundary: succes" << std::endl;
    } else {
         std::cout << "***dev-test percent, extrem value top boundary: fail" << std::endl;
    }

    deltaV = -0.4;
    actualY = 0;

    value = drawer.percentageOfPixeBelowLine(actualY, deltaV );
    if ( value == 0 ) {
        std::cout << "***dev-test percent, extrem value bottom right boundary: succes" << std::endl;
    } else {
         std::cout << "***dev-test percent, extrem value  bottom right: fail" << std::endl;
    }


    deltaV = 1;
    actualY = 0;

    value = drawer.percentageOfPixeBelowLine(actualY, deltaV );
    if ( value < 0.51 && value > 0.49 ) {
        std::cout << "***dev-test percent, diagonal: succes" << std::endl;
    } else {
         std::cout << "***dev-test percent, diagonal: fail" << std::endl;
    }



    deltaV = 0.7;
    actualY = 1.2;

    value = drawer.percentageOfPixeBelowLine(actualY, deltaV );
    if ( value == 1) {
        std::cout << "***dev-test percent, line completely above the pixel: succes" << std::endl;
    } else {
         std::cout << "***dev-test percent,  line completely above the pixel: fail" << std::endl;
    }



    deltaV = -0.7;
    actualY = -999;

    value = drawer.percentageOfPixeBelowLine(actualY, deltaV );
    if ( value == 0) {
        std::cout << "***dev-test percent, line completely bellow the pixel: succes" << std::endl;
    } else {
         std::cout << "***dev-test percent,  line completely bellow the pixel: fail" << std::endl;
    }


    deltaV = 1;
    actualY = -999;

    value = drawer.percentageOfPixeBelowLine(actualY, deltaV );
    if ( value == 0) {
        std::cout << "***dev-test percent, line completely bellow the pixel 2: succes" << std::endl;
    } else {
         std::cout << "***dev-test percent,  line completely bellow the pixel 2: fail" << std::endl;
    }


    deltaV = 0.8;
    actualY = -0.3;

    value = drawer.percentageOfPixeBelowLine(actualY, deltaV );
    if ( value < 0.16 && value > 0.15) {
        std::cout << "***dev-test percent, line start below then cross pixel: succes" << std::endl;
    } else {
         std::cout << "***dev-test percent,  line start below then cross pixel: fail" << std::endl;
    }




    deltaV = -0.8;
    actualY = 1.3;

    value = drawer.percentageOfPixeBelowLine(actualY, deltaV );
    if ( value < 0.844 && value > 0.843) {
        std::cout << "***dev-test percent, line start above then cross pixel: succes" << std::endl;
    } else {
         std::cout << "***dev-test percent,  line start above then cross pixel: fail" << std::endl;
    }



    deltaV = 3;
    actualY = -1;

    // crash test
    // value = drawer.percentageOfPixeBelowLine(actualY, deltaV );



}





void test::setPixelIdxAndIntensityBasedOnPercentageOfAThickLine()
{

    imageDrawer drawer = imageDrawer();
    list<int*>  l ;

    std::cout << "For the following tests, the expected result was computed using geometric drawing, so it's normal to get some small differences" << std::endl;



    std::cout << "***dev-test intensity line: -45 degree " << std::endl;
    drawer.setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, 100, 1, ofVec2f(1.5,3.5), ofVec2f(3.5,1.5));
    std::cout << "result: ";
    drawer.printListIdxAndIntensity(l);
    std::cout << "expected result:\n(1,4,23) (1,3,88) (1,2,23) (2,3,23) (2,2,88) (2,1,23) (3,2,23) (3,1,88) (3,0,23) " << std::endl;

    drawer.freeListOfIntArray(&l);
    l.clear();



    std::cout << "***dev-test intensity line: -45 degree (invert) " << std::endl; // we invert the sense of the parameter of the line
    drawer.setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, 100, 1, ofVec2f(3.5,1.5),  ofVec2f(1.5,3.5));
    std::cout << "result: ";
    drawer.printListIdxAndIntensity(l);
    std::cout << "expected result:\n(1,4,23) (1,3,88) (1,2,23) (2,3,23) (2,2,88) (2,1,23) (3,2,23) (3,1,88) (3,0,23) " << std::endl;

    drawer.freeListOfIntArray(&l);
    l.clear();



    std::cout << "***dev-test intensity line: 45 degree " << std::endl;
    drawer.setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, 100, 1, ofVec2f(1.5,1.5), ofVec2f(3.5,3.5));
    std::cout << "result: ";
    drawer.printListIdxAndIntensity(l);
    std::cout << "expected result:\n(1,0,23) (1,1,88) (1,2,23) (2,1,23) (2,2,88) (2,3,23) (3,2,23) (3,3,88) (3,4,23) " << std::endl;



    drawer.freeListOfIntArray(&l);
    l.clear();

    // this time we give the corner -> should be the same as before because we alwas take the center of the pixel as starting point
    std::cout << "***dev-test intensity line: 45 degree (bis) " << std::endl;
    drawer.setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, 100, 1, ofVec2f(1,1), ofVec2f(3,3));
    std::cout << "result: ";
    drawer.printListIdxAndIntensity(l);
    std::cout << "expected result:\n(1,0,23) (1,1,88) (1,2,23) (2,1,23) (2,2,88) (2,3,23) (3,2,23) (3,3,88) (3,4,23) " << std::endl;




    drawer.freeListOfIntArray(&l);
    l.clear();

    std::cout << "***dev-test intensity line: 45 degree (invert) " << std::endl;
    drawer.setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, 100, 1, ofVec2f(3,3),  ofVec2f(1,1));
    std::cout << "result: ";
    drawer.printListIdxAndIntensity(l);
    std::cout << "expected result:\n(1,0,23) (1,1,88) (1,2,23) (2,1,23) (2,2,88) (2,3,23) (3,2,23) (3,3,88) (3,4,23) " << std::endl;




    drawer.freeListOfIntArray(&l);
    l.clear();

    std::cout << "***dev-test intensity line: 45 degree (invert) " << std::endl;
    drawer.setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, 100, 1, ofVec2f(3,3),  ofVec2f(1,1));
    std::cout << "result: ";
    drawer.printListIdxAndIntensity(l);
    std::cout << "expected result:\n(1,0,23) (1,1,88) (1,2,23) (2,1,23) (2,2,88) (2,3,23) (3,2,23) (3,3,88) (3,4,23) " << std::endl;


    drawer.freeListOfIntArray(&l);
    l.clear();

    std::cout << "***dev-test intensity line: line form (0,0) to (2,3) " << std::endl;
    drawer.setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, 100, 1, ofVec2f(0,0),  ofVec2f(2,3));
    std::cout << "result: ";
    drawer.printListIdxAndIntensity(l);
    std::cout << "expected result:\n(-1,0,15) (0,0,92) (1,0,15) (0,1,45) (1,1,77) (2,2,1) (0,2,1) (1,2,77) ... " << std::endl;


    drawer.freeListOfIntArray(&l);
    l.clear();

    std::cout << "***dev-test intensity line: line form (0,3) to (3,1) " << std::endl;
    drawer.setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, 100, 1, ofVec2f(0,3),  ofVec2f(3,1));
    std::cout << "result: ";
    drawer.printListIdxAndIntensity(l);
    std::cout << "expected result:\n(0,4,15) (0,3,92) (0,2,15) (1,3,45) (1,2,77) (1,1,1) (2,3,1) (2,2,77) ... " << std::endl;



    drawer.freeListOfIntArray(&l);
    l.clear();

    std::cout << "***dev-test intensity line: line form (0,1) to (4,0) " << std::endl;
    drawer.setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, 100, 1, ofVec2f(0,1),  ofVec2f(4,0));
    std::cout << "result: ";
    drawer.printListIdxAndIntensity(l);
    std::cout << "expected result:\n(0,2,4) (0,1,96) (0,0,4) (1,1,75) (1,0,25) (2,1,50) (2,0,50) (3,1,75) (3,0,25) (4,1,4) (4,0,96) (4,-1,4) " << std::endl;




    drawer.freeListOfIntArray(&l);
    l.clear();

    std::cout << "***dev-test intensity line: line form (0,0) to (1,4) " << std::endl;
    drawer.setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, 100, 1, ofVec2f(0,0),  ofVec2f(1,4));
    std::cout << "result: ";
    drawer.printListIdxAndIntensity(l);
    std::cout << "expected result:\n(-1,0,4) (0,0,96) (1,0,4) (0,1,75) (1,1,25) ... " << std::endl;




    drawer.freeListOfIntArray(&l);
    l.clear();

    std::cout << "***dev-test intensity line: line form (0,0) to (0,3) " << std::endl;
    drawer.setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, 100, 1, ofVec2f(0,0),  ofVec2f(0,3));
    std::cout << "result: ";
    drawer.printListIdxAndIntensity(l);
    std::cout << "expected result:\n(0,0,100) (0,1, 100) (0,2,100) (0,3,100) " << std::endl;





    drawer.freeListOfIntArray(&l);
    l.clear();

    std::cout << "***dev-test intensity line: line form (-2,0) to (2,0) " << std::endl;
    drawer.setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, 100, 1, ofVec2f(-2,0),  ofVec2f(2,0));
    std::cout << "result: ";
    drawer.printListIdxAndIntensity(l);
    std::cout << "expected result:\n(-2,0,100) (-1,0, 100) (0,0,100) (1,0,100) (2,0,100) " << std::endl;





    drawer.freeListOfIntArray(&l);
    l.clear();

    std::cout << "***dev-test intensity line: line form (0,0) to (,0) " << std::endl;
    drawer.setPixelIdxAndIntensityBasedOnPercentageOfAThickLine(&l, 100, 1, ofVec2f(0,0),  ofVec2f(0,0));
    std::cout << "result: ";
    drawer.printListIdxAndIntensity(l);
    std::cout << "expected result: warning"<< std::endl;









}



