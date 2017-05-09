#ifndef SCRIPTUTILITY_H
#define SCRIPTUTILITY_H

#include "ofMain.h"
#include "grayshed.h"
#include "wheel.h"


class scriptUtility
{
public:

    scriptUtility();
    void run();
    string getSavingName(genericShed *workshop, string imageFn);
};

#endif // SCRIPTUTILITY_H
