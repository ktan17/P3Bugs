#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////////////////////////////////////////////////
// Mobile HP Actor Implementation
///////////////////////////////////////////////////////////////////////////

MobileHPActor::MobileHPActor(int startingHP, int imageID, int startX, int startY) :
HPActor(startingHP, imageID, startX, startY, generateRandomDirection()) {

    m_ticksToSleep = 0;
    
}

GraphObject::Direction MobileHPActor::generateRandomDirection() {
    
    switch (randInt(1, 4)) {
            
        case 1:
            return up;
        case 2:
            return right;
        case 3:
            return down;
        case 4:
            return left;
        default:
            break;
    }
    
    return none; // Will never be executed but needed to compile.
    
}

///////////////////////////////////////////////////////////////////////////
// Grasshopper Implementation
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// Baby Grasshopper Implementation
///////////////////////////////////////////////////////////////////////////

BabyGrasshopper::BabyGrasshopper(int startX, int startY) :
Grasshopper(500, IID_BABY_GRASSHOPPER, startX, startY) {
    
    
}

///////////////////////////////////////////////////////////////////////////
// Adult Grasshopper Implementation
///////////////////////////////////////////////////////////////////////////

AdultGrasshopper::AdultGrasshopper(int startX, int startY) :
Grasshopper(1600, IID_ADULT_GRASSHOPPER, startX, startY) {
    
    
}
