#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

///////////////////////////////////////////////////////////////////////////
// Abstract Actor Base Class
///////////////////////////////////////////////////////////////////////////

class Actor : public GraphObject {
    
public:
    // Constructor / Destructor
    Actor(int imageID, int startX, int startY, StudentWorld *p, bool isHPActor, Direction dir = right, int depth = 1, bool canBlockMovingActor = false, bool isEdible = false) : GraphObject(imageID, startX, startY, dir, depth), pToWorld(p), m_isHPActor(isHPActor) { m_canBlockMovingActor = canBlockMovingActor; m_isEdible = isEdible; }
    virtual ~Actor() {}
    
    // Accessors
    bool isHPActor() const { return m_isHPActor; }
    bool canBlockMovingActor() const { return m_canBlockMovingActor; }
    bool isEdible() const { return m_isEdible; }
    StudentWorld* getPointerToWorld() const { return pToWorld; }
    
    // Mutators
    void setIsEdible() { m_isEdible = true; }
    virtual void doSomething() = 0;
    
private:
    bool m_isHPActor;
    bool m_canBlockMovingActor;
    bool m_isEdible;
    StudentWorld *pToWorld;
    
};

///////////////////////////////////////////////////////////////////////////
// HP Actor Declaration
///////////////////////////////////////////////////////////////////////////

class HPActor : public Actor {
    
public:
    // Constructor / Destructor
    HPActor(int startingHP, int imageID, int startX, int startY, StudentWorld *p, bool isMobile, Direction dir = right, int depth = 1);
    virtual ~HPActor() {}
    
    // Helper Functions
    int correctArtwork(int colonyNumber, HPActor* p) const;
    
    // Accessors
    int hitpoints() const { return m_hitpoints; }
    bool isDead() const { return m_dead; }
    bool isMobile() const { return m_isMobile; }
    
    // Mutators
    void setHitpoints(int n) { m_hitpoints += n; }
    virtual void setDead();
    virtual void doSomething();
    virtual void HPDoSomething() = 0;
    
private:
    int m_hitpoints;
    bool m_dead;
    bool m_isMobile;
    
};

////////////////////////////////////////////
// Static HP Actor Declarations
////////////////////////////////////////////

//////////////////////
// Food: *COMPLETE* //
//////////////////////

class Food : public HPActor {
    
public:
    // Constructor / Destructor
    Food(int posX, int posY, StudentWorld *p, int startingHP = 6000);
    virtual ~Food() {}
    
    // Mutators
    virtual void HPDoSomething() {}
    
};

// TODO: Pheromone

class Pheromone : public HPActor {
    
public:
    // Constructor / Destructor
    Pheromone(int antColony, int posX, int posY, StudentWorld *p); // TODO
    virtual ~Pheromone() {}
    
    // Mutators
    virtual void HPDoSomething() {} // TODO
    
};

/////////////////////////
// Anthill: *COMPLETE* //
/////////////////////////

class Anthill : public HPActor {
    
public:
    // Constructor / Destructor
    Anthill(int antColony, int posX, int posY, StudentWorld *p); // TODO
    virtual ~Anthill() {}
    
    // Accessors
    int getColonyNumber() const { return m_colonyNumber; }
    
    // Mutators
    virtual void HPDoSomething();
    
private:
    int m_colonyNumber;
    
};

////////////////////////////////////////////
// Mobile HP Actor Declaration
////////////////////////////////////////////

class MobileHPActor : public HPActor {
    
public:
    // Constructor / Destructor
    MobileHPActor(int startingHP, int imageID, int startX, int startY, StudentWorld *p, int depth);
    virtual ~MobileHPActor() {}
    
    // Helper Functions
    Direction generateRandomDirection();
    
    // Accessors
    int ticksToSleep() const { return m_ticksToSleep; }
    int getStartX() const { return m_startX; }
    int getStartY() const { return m_startY; }
    bool isStunned() const { return m_stunned; }
    
    // Mutators
    void adjustTicksToSleep(int n);
    void setStunned(bool stunned);
    virtual void setDead();
    virtual void HPDoSomething();
    virtual void mobileDoSomething() = 0;
    
private:
    int m_ticksToSleep;
    int m_startX;
    int m_startY;
    bool m_stunned;
    
};

// TODO: Ant

class Compiler;

class Ant : public MobileHPActor {
    
public:
    // Constructor / Destructor
    Ant(int colonyNumber, int startX, int startY, StudentWorld *p);
    virtual ~Ant() {}
    
    // Mutators
    virtual void mobileDoSomething() {} // TODO
    
private:
    int m_colonyNumber;
    int m_heldFood;
    int m_lastRandomNumberHeld;
    int m_instructionCount;
    
    Compiler* m_compiler;
    
};

/////////////////////////////
// Grasshopper: *COMPLETE* //
/////////////////////////////

class Grasshopper : public MobileHPActor {
    
public:
    // Constructor / Destructor
    Grasshopper(int startingHP, int imageID, int startX, int startY, StudentWorld *p);
    virtual ~Grasshopper() {}
    
    // Mutators
    void setDidBiteOrJump(bool didBiteOrJump) { m_didBiteOrJump = didBiteOrJump; }
    virtual void mobileDoSomething();
    virtual void grasshopperDoSomething() = 0;
    
private:
    int m_stepsToMove;
    bool m_didBiteOrJump;
    
};

class BabyGrasshopper : public Grasshopper {
    
public:
    // Constructor / Destructor
    BabyGrasshopper(int startX, int startY, StudentWorld *p);
    virtual ~BabyGrasshopper() {}
    
    // Mutators
    virtual void grasshopperDoSomething();
    
};

class AdultGrasshopper : public Grasshopper {
    
public:
    // Constructor / Destructor
    AdultGrasshopper(int startX, int startY, StudentWorld *p);
    virtual ~AdultGrasshopper() {}
    
    // Mutators
    virtual void grasshopperDoSomething();
    
};

///////////////////////////////////////////////////////////////////////////
// No HP Actor Declarations
///////////////////////////////////////////////////////////////////////////

////////////////////////
// Pebble: *COMPLETE* //
////////////////////////

class Pebble : public Actor {
    
public:
    // Constructor / Destructor
    Pebble(int posX, int posY, StudentWorld *p) : Actor(IID_ROCK, posX, posY, p, false, right, 1, true) {}
    virtual ~Pebble() {}
    
    // Mutators
    virtual void doSomething() {}
    
};

////////////////////////////////////////////
// Active No HP Actor Declaration
////////////////////////////////////////////

class ActiveNoHPActor : public Actor {
    
public:
    // Constructor / Destructor
    ActiveNoHPActor(int imageID, int posX, int posY, StudentWorld *p, bool isPoison, Direction dir, int depth) : Actor(imageID, posX, posY, p, false, dir, depth), m_isPoison(isPoison) {}
    virtual ~ActiveNoHPActor() {}
    
    // Accessors
    bool isPoison() const { return m_isPoison; }
    
    // Mutators
    virtual void doSomething();
    virtual void specializedDoSomething() {}
    
private:
    bool m_isPoison;
    
};

///////////////////////////
// Waterpool: *COMPLETE* //
///////////////////////////

class WaterPool : public ActiveNoHPActor {
    
public:
    // Constructor / Destructor
    WaterPool(int posX, int posY, StudentWorld *p) : ActiveNoHPActor(IID_WATER_POOL, posX, posY, p, false, right, 2) {}
    virtual ~WaterPool() {}
    
    // Mutators
    
};

////////////////////////
// Poison: *COMPLETE* //
////////////////////////

class Poison : public ActiveNoHPActor {
    
public:
    // Constructor / Destructor
    Poison(int posX, int posY, StudentWorld *p) : ActiveNoHPActor(IID_POISON, posX, posY, p, true, right, 2) {}
    virtual ~Poison() {}
    
    // Mutators
    
};

#endif // ACTOR_H_
