#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////////////////////////////////////////////////
// HP Actor Implementation
///////////////////////////////////////////////////////////////////////////

HPActor::HPActor(int startingHP, int imageID, int startX, int startY, Direction dir) :
Actor(imageID, startX, startY, dir) {
    
    m_hitpoints = startingHP;
    m_dead = false;

}

int HPActor::correctArtwork(int colonyNumber, HPActor *p) const {
    
    if (dynamic_cast<Ant*>(p) != nullptr) {
        
        switch (colonyNumber) {
                
            case 0:
                return IID_ANT_TYPE0;
            case 1:
                return IID_ANT_TYPE1;
            case 2:
                return IID_ANT_TYPE2;
            case 3:
                return IID_ANT_TYPE3;
            default:
                break;
                
        }
        
    }
    
    else if (dynamic_cast<Pheromone*>(p) != nullptr) {
        
        switch (colonyNumber) {
                
            case 0:
                return IID_PHEROMONE_TYPE0;
            case 1:
                return IID_PHEROMONE_TYPE1;
            case 2:
                return IID_PHEROMONE_TYPE2;
            case 3:
                return IID_PHEROMONE_TYPE3;
            default:
                break;
                
        }
        
    }
    
    return 0; // Will never be excuted, but necessary for compilation.
    
}

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
// Ant Implementation
///////////////////////////////////////////////////////////////////////////

Ant::Ant(int colonyNumber, int startX, int startY) :
MobileHPActor(1500, correctArtwork(colonyNumber, this), startX, startY){
    
    m_colonyNumber = colonyNumber;
    m_heldFood = 0;
    m_lastRandomNumberHeld = 0;
    m_instructionCount = 0;
    
}

///////////////////////////////////////////////////////////////////////////
// Grasshopper Implementation
///////////////////////////////////////////////////////////////////////////

Grasshopper::Grasshopper(int startingHP, int imageID, int startX, int startY) :
MobileHPActor(startingHP, imageID, startX, startY) {

    stepsToMove = randInt(2, 10);

}

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
