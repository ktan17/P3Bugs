#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////////////////////////////////////////////////
// HP Actor Implementation
///////////////////////////////////////////////////////////////////////////

HPActor::HPActor(int startingHP, int imageID, int startX, int startY, StudentWorld *p, bool isMobile,Direction dir, int depth) :
Actor(imageID, startX, startY, p, dir, depth) {
    
    m_hitpoints = startingHP;
    m_dead = false;
    m_isMobile = isMobile;
    
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

void HPActor::setDead() {
    
    m_dead = true;
    getPointerToWorld()->recordDeadActorPosition(getX(), getY());
    
}

///////////////////////////////////////////////////////////////////////////
// Food Implementation
///////////////////////////////////////////////////////////////////////////

Food::Food(int posX, int posY, StudentWorld *p, int startingHP) :
HPActor(startingHP, IID_FOOD , posX, posY, p, false, right, 2) {
    
}

///////////////////////////////////////////////////////////////////////////
// Pheromone Implementation
///////////////////////////////////////////////////////////////////////////

Pheromone::Pheromone(int antColony, int posX, int posY, StudentWorld *p) :
HPActor(256, correctArtwork(antColony, this), posX, posY, p, false, right, 2) {
    
}

///////////////////////////////////////////////////////////////////////////
// Anthill Implementation
///////////////////////////////////////////////////////////////////////////

Anthill::Anthill(int antColony, int posX, int posY, StudentWorld *p) :
HPActor(8999, IID_ANT_HILL, posX, posY, p, true, right, 2) {
    
    m_colonyNumber = antColony;
    
}

///////////////////////////////////////////////////////////////////////////
// Mobile HP Actor Implementation
///////////////////////////////////////////////////////////////////////////

MobileHPActor::MobileHPActor(int startingHP, int imageID, int startX, int startY, StudentWorld *p, int depth) :
HPActor(startingHP, imageID, startX, startY, p, true, generateRandomDirection(), depth) {
    
    m_ticksToSleep = 0;
    m_startX = startX;
    m_startY = startY;
    m_stunned = false;
    
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

void MobileHPActor::adjustTicksToSleep(int n) {
    
    m_ticksToSleep += n;
    
}

void MobileHPActor::setStunned(bool stunned) {
    
    m_stunned = stunned;
    
}

void MobileHPActor::setDead() {
    
    HPActor::setDead();
    getPointerToWorld()->createFoodOn(getX(), getY());
    
}

void MobileHPActor::doSomething() {
    
    setHitpoints(-1);
    m_startX = getX();
    m_startY = getY();
    
    if (hitpoints() == 0) {
        
        MobileHPActor::setDead();
        return;
        
    }
    
    if (m_ticksToSleep > 0) {
        
        m_ticksToSleep--;
        return;
        
    }
    
    specializedDoSomething();
    
}

///////////////////////////////////////////////////////////////////////////
// Ant Implementation
///////////////////////////////////////////////////////////////////////////

Ant::Ant(int colonyNumber, int startX, int startY, StudentWorld *p) :
MobileHPActor(1500, correctArtwork(colonyNumber, this), startX, startY, p, 1){
    
    m_colonyNumber = colonyNumber;
    m_heldFood = 0;
    m_lastRandomNumberHeld = 0;
    m_instructionCount = 0;
    
}

///////////////////////////////////////////////////////////////////////////
// Grasshopper Implementation
///////////////////////////////////////////////////////////////////////////

Grasshopper::Grasshopper(int startingHP, int imageID, int startX, int startY, StudentWorld *p) :
MobileHPActor(startingHP, imageID, startX, startY, p, 1) {
    
    m_stepsToMove = randInt(2, 10);
    
}

void Grasshopper::specializedDoSomething() {
    
    grasshopperDoSomething();
    
    if (!isDead()) {
        
        int foodAte = getPointerToWorld()->attemptToEat(getX(), getY(), 200);
        
        if (foodAte != -1) {
            
            setHitpoints(foodAte);
            
            if (randInt(0, 1) == 1) {
                
                adjustTicksToSleep(2);
                return;
                
            }
            
        }
        
        setStunned(false);
        
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
        
        if (getPointerToWorld()->attemptToMove(this, getX(), getY(), destX, destY)) {
            
            moveTo(destX, destY);
            m_stepsToMove--;
            
        }
        
        else
            m_stepsToMove = 0;
        
        adjustTicksToSleep(2);
        
    }
    
}

///////////////////////////////////////////////////////////////////////////
// Baby Grasshopper Implementation
///////////////////////////////////////////////////////////////////////////

BabyGrasshopper::BabyGrasshopper(int startX, int startY, StudentWorld *p) :
Grasshopper(500, IID_BABY_GRASSHOPPER, startX, startY, p) {}

void BabyGrasshopper::grasshopperDoSomething() {
    
    if (hitpoints() >= 1600) {
        
        getPointerToWorld()->growUpGrasshopper(getX(), getY());
        setDead();
        return;
        
    }
    
}

///////////////////////////////////////////////////////////////////////////
// Adult Grasshopper Implementation
///////////////////////////////////////////////////////////////////////////

AdultGrasshopper::AdultGrasshopper(int startX, int startY, StudentWorld *p) :
Grasshopper(1600, IID_ADULT_GRASSHOPPER, startX, startY, p) {
    
    
}

///////////////////////////////////////////////////////////////////////////
// Active No HP Actor Implementation
///////////////////////////////////////////////////////////////////////////

void ActiveNoHPActor::doSomething() {
    
    
    
}
