#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////////////////////////////////////////////////
// HP Actor Implementation
///////////////////////////////////////////////////////////////////////////

HPActor::HPActor(int startingHP, int imageID, int startX, int startY, StudentWorld *p, bool isMobile, Direction dir, int depth) :
Actor(imageID, startX, startY, p, true, dir, depth) {
    
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

void HPActor::doSomething() {
    
    if (!this->isEdible())
        setHitpoints(-1);
    
    if (!this->isDead()) {
        
        if (hitpoints() == 0) {
            // If dead, setDead and return.
            
            setDead();
            return;
            
        }
        
        HPDoSomething();
        
    }
}

///////////////////////////////////////////////////////////////////////////
// Food Implementation
///////////////////////////////////////////////////////////////////////////

Food::Food(int posX, int posY, StudentWorld *p, int startingHP) :
HPActor(startingHP, IID_FOOD , posX, posY, p, false, right, 2) {
    
    setIsEdible();
    
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

void Anthill::HPDoSomething() {
    
    int foodAte = getPointerToWorld()->attemptToEat(getX(), getY(), 10000);
    
    if (foodAte != -1)
        setHitpoints(foodAte);
    
    if (hitpoints() >= 2000) {
        
        getPointerToWorld()->createAnt(this);
        setHitpoints(-1500);
        
    }
    
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

void MobileHPActor::getDestinationCoordinate(int &destX, int &destY) {
    
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

void MobileHPActor::HPDoSomething() {
    // For all Mobile HP Actors i.e. Ants and Grasshoppers
    
    if (!isDead()) {
        
        // Remember the position that this Actor started on.
        m_startX = getX();
        m_startY = getY();
        
        if (m_ticksToSleep > 0) {
            // If sleeping, decrement ticksToSleep and return.
            
            m_ticksToSleep--;
            return;
            
        }
        
        // Otherwise, the actor is going to do something...
        mobileDoSomething();
    
    }
        
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
    m_wasBitten = false;
    m_wasBlockedFromMoving = false;
    
    m_compiler = getPointerToWorld()->getCompiler(colonyNumber);
    
}

void Ant::mobileDoSomething() {
    
    Compiler::Command cmd;
    
    for (int i = 0; i < 10; i++) {
    
        if (m_compiler->getCommand(m_instructionCount, cmd)) {
            
            switch (cmd.opcode) {
                    
                case Compiler::moveForward:
                    break;
                    
            }
            
        }
        
        else {
            
            setDead();
            return;
            
        }
        
    }
    
}

///////////////////////////////////////////////////////////////////////////
// Grasshopper Implementation
///////////////////////////////////////////////////////////////////////////

Grasshopper::Grasshopper(int startingHP, int imageID, int startX, int startY, StudentWorld *p) :
MobileHPActor(startingHP, imageID, startX, startY, p, 1) {
    
    m_stepsToMove = randInt(2, 10);
    m_didBiteOrJump = false;
    
}

void Grasshopper::mobileDoSomething() {
    
    // New tick, so reset didBiteOrJump.
    m_didBiteOrJump = false;
    
    // Perform the differentiated Adult and Baby Grasshopper instructions.
    grasshopperDoSomething();
    
    // Grasshopper may have died/bitten/jumped is grasshopperDoSomething(); spec requires we immediately
    // return in that case.
    if (!isDead() && !m_didBiteOrJump) {
        
        int foodAte = getPointerToWorld()->attemptToEat(getX(), getY(), 200);
        
        if (foodAte != -1) {
            
            setHitpoints(foodAte);
            
            if (randInt(0, 1) == 1) {
                
                adjustTicksToSleep(2);
                return;
                
            }
            
        }
        
        // setStunned is only reset to false if the grasshopper will move a square.
        setStunned(false);
        
        if (m_stepsToMove == 0) {
            
            setDirection(generateRandomDirection());
            m_stepsToMove = randInt(2, 10);
            
        }
        
        // Based on the direction, figure out where the destination square will be...
        int destX = 0, destY = 0;
        
        getDestinationCoordinate(destX, destY);
        
        // ... and attempt to move there.
        if (getPointerToWorld()->attemptToMove(this, destX, destY)) {
            
            moveTo(destX, destY);
            m_stepsToMove--;
            
        }
        
        else
            m_stepsToMove = 0;  // Object that can block actor has blocked the move.
        
        // Grasshopper has done something, now it must sleep for 2 ticks.
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

void AdultGrasshopper::grasshopperDoSomething() {
    
    if (randInt(1, 3) == 1 && getPointerToWorld()->attemptToBite(this, getX(), getY(), 50)) {
        
        setDidBiteOrJump(true);
        return;
    
    }
    
    if (randInt(1, 10) == 1) {
        
        double theta = randInt(0, 359) * M_PI / 180;
        int radius = randInt(1, 10);
        
        int posX = getX() + round(radius * cos(theta));
        int posY = getY() + round(radius * sin(theta));
        
    while ((posX < 0 || posX > 63 || posY < 0 || posY > 63) || getPointerToWorld()->isBlockOn(posX, posY)) {
        
            theta = randInt(0, 359) * M_PI / 180;
            radius = randInt(1, 10);
            posX = getX() + round(radius * cos(theta));
            posY = getY() + round(radius * sin(theta));
        
    }
        
        moveTo(posX, posY);
        getPointerToWorld()->recordJump(this, getStartX(), getStartY(), getX(), getY());
        setDidBiteOrJump(true);
        return;
        
    }
    
}

///////////////////////////////////////////////////////////////////////////
// Active No HP Actor Implementation
///////////////////////////////////////////////////////////////////////////

void ActiveNoHPActor::doSomething() {
    
    getPointerToWorld()->stunActors(this, getX(), getY());
    
}
