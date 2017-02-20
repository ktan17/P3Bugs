#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////////////////////////////////////////////////
// HP Actor Implementation
///////////////////////////////////////////////////////////////////////////

HPActor::HPActor(int startingHP, int imageID, int startX, int startY, Direction dir, int depth) :
Actor(imageID, startX, startY, dir, depth) {
    
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
// Food Implementation
///////////////////////////////////////////////////////////////////////////

Food::Food(int posX, int posY, int startingHP) :
HPActor(startingHP, IID_FOOD , posX, posY, right, 2) {
    
}

///////////////////////////////////////////////////////////////////////////
// Pheromone Implementation
///////////////////////////////////////////////////////////////////////////

Pheromone::Pheromone(int antColony, int posX, int posY) :
HPActor(256, correctArtwork(antColony, this), posX, posY, right, 2) {
    
}

///////////////////////////////////////////////////////////////////////////
// Anthill Implementation
///////////////////////////////////////////////////////////////////////////

Anthill::Anthill(int antColony, int posX, int posY) :
HPActor(8999, IID_ANT_HILL, posX, posY, right, 2) {
    
    m_colonyNumber = antColony;
    
}

///////////////////////////////////////////////////////////////////////////
// Mobile HP Actor Implementation
///////////////////////////////////////////////////////////////////////////

MobileHPActor::MobileHPActor(int startingHP, int imageID, int startX, int startY, int depth) :
HPActor(startingHP, imageID, startX, startY, generateRandomDirection(), depth) {

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

void MobileHPActor::doSomething() {
    
    setHitpoints(-1);
    
    if (hitpoints() == 0) {
        
        pToWorld->createFoodOn(getX(), getY());
        setDead();
        pToWorld->recordDeadActorPosition(getX(), getY());
        return;
        
    }
    
    if (m_ticksToSleep > 0) {
        
        m_ticksToSleep--;
        return;
        
    }
    
    specializedDoSomething();
    
}

void MobileHPActor::adjustTicksToSleep(int n) {
    
    m_ticksToSleep += n;
    
}

///////////////////////////////////////////////////////////////////////////
// Ant Implementation
///////////////////////////////////////////////////////////////////////////

Ant::Ant(int colonyNumber, int startX, int startY) :
MobileHPActor(1500, correctArtwork(colonyNumber, this), startX, startY, 1){
    
    m_colonyNumber = colonyNumber;
    m_heldFood = 0;
    m_lastRandomNumberHeld = 0;
    m_instructionCount = 0;
    
}

///////////////////////////////////////////////////////////////////////////
// Grasshopper Implementation
///////////////////////////////////////////////////////////////////////////

Grasshopper::Grasshopper(int startingHP, int imageID, int startX, int startY) :
MobileHPActor(startingHP, imageID, startX, startY, 1) {

    m_stepsToMove = randInt(2, 10);

}

void Grasshopper::specializedDoSomething() {
    
    grasshopperDoSomething();
    
    int foodAte = getPointerToWorld()->attemptToEat(getX(), getY(), 200);
    
    if (foodAte != -1) {
        
        setHitpoints(foodAte);
        
        if (randInt(0, 1) == 1) {
            
            adjustTicksToSleep(2);
            return;
            
        }
        
    }
        
    if (m_stepsToMove == 0) {
        
        setDirection(generateRandomDirection());
        m_stepsToMove = randInt(2, 10);
        
    }
    
    int destX = 0, destY = 0;
    
    switch (getDirection()) {
            
        case up:
            destX = getX();
            destY = getY()+1;
            break;
            
        case right:
            destX = getX()+1;
            destY = getY();
            break;
            
        case down:
            destX = getX();
            destY = getY()-1;
            break;
            
        case left:
            destX = getX()-1;
            destY = getY();
            break;
            
        default:
            break;
            
    }
    
    bool didMove = getPointerToWorld()->attemptToMove(this, getX(), getY(), destX, destY);
    
    if (didMove)
        moveTo(destX, destY);
    
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
