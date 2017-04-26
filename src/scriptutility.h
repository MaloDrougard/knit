#ifndef SCRIPTUTILITY_H
#define SCRIPTUTILITY_H

#include "ofMain.h"
#include "shed.h"
#include "wheel.h"


class scriptUtility
{
public:

    scriptUtility();
    void run();
    string getSavingName(shed workshop, string imageFn);
};

#endif // SCRIPTUTILITY_H
