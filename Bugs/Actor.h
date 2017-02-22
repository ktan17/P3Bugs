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
    Actor(int imageID, int startX, int startY, StudentWorld *p, Direction dir = right, int depth = 1) : GraphObject(imageID, startX, startY, dir, depth), pToWorld(p) {}
    virtual ~Actor() {} // TODO
    
    // Accessors
    StudentWorld* getPointerToWorld() const { return pToWorld; }
    
    // Mutators
    virtual void doSomething() = 0;
    
private:
    StudentWorld *pToWorld;
    
};

///////////////////////////////////////////////////////////////////////////
// HP Actor Declaration
///////////////////////////////////////////////////////////////////////////

class HPActor : public Actor {
    
public:
    // Constructor / Destructor
    HPActor(int startingHP, int imageID, int startX, int startY, StudentWorld *p, bool isMobile, Direction dir = right, int depth = 1);
    virtual ~HPActor() {} // TODO
    
    // Helper Functions
    int correctArtwork(int colonyNumber, HPActor* p) const;
    
    // Accessors
    int hitpoints() const { return m_hitpoints; }
    bool isDead() const { return m_dead; }
    bool isMobile() const { return m_isMobile; }
    
    // Mutators
    void setHitpoints(int n) { m_hitpoints += n; }
    virtual void setDead();
    virtual void doSomething() = 0;
    
private:
    int m_hitpoints;
    bool m_dead;
    bool m_isMobile;
    
};

////////////////////////////////////////////
// Static HP Actor Declarations
////////////////////////////////////////////

// TODO: Food

class Food : public HPActor {
    
public:
    // Constructor / Destructor
    Food(int posX, int posY, StudentWorld *p, int startingHP = 6000);
    virtual ~Food() {} // TODO
    
    // Mutators
    virtual void doSomething() {}
    
};

// TODO: Pheromone

class Pheromone : public HPActor {
    
public:
    // Constructor / Destructor
    Pheromone(int antColony, int posX, int posY, StudentWorld *p); // TODO
    virtual ~Pheromone() {} // TODO
    
    // Mutators
    virtual void doSomething() {} // TODO
    
};

// TODO: Anthill

class Anthill : public HPActor {
    
public:
    // Constructor / Destructor
    Anthill(int antColony, int posX, int posY, StudentWorld *p); // TODO
    virtual ~Anthill() {}
    
    // Mutators
    virtual void doSomething() {} // TODO
    
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
    virtual ~MobileHPActor() {} // TODO
    
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
    virtual void doSomething();
    virtual void specializedDoSomething() = 0;
    
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
    virtual ~Ant() {} // TODO
    
    // Mutators
    virtual void specializedDoSomething() {} // TODO
    
private:
    int m_colonyNumber;
    int m_heldFood;
    int m_lastRandomNumberHeld;
    int m_instructionCount;
    
    Compiler* m_compiler;
    
};

// TODO: Grasshopper

class Grasshopper : public MobileHPActor {
    
public:
    // Constructor / Destructor
    Grasshopper(int startingHP, int imageID, int startX, int startY, StudentWorld *p);
    virtual ~Grasshopper() {} // TODO
    
    // Mutators
    virtual void specializedDoSomething();
    virtual void grasshopperDoSomething() = 0;
    
private:
    int m_stepsToMove;
    
};

class BabyGrasshopper : public Grasshopper {
    
public:
    // Constructor / Destructor
    BabyGrasshopper(int startX, int startY, StudentWorld *p);
    virtual ~BabyGrasshopper() {} // TODO
    
    // Mutators
    virtual void grasshopperDoSomething(); // TODO
    
};

class AdultGrasshopper : public Grasshopper {
    
public:
    // Constructor / Destructor
    AdultGrasshopper(int startX, int startY, StudentWorld *p);
    virtual ~AdultGrasshopper() {} // TODO
    
    // Mutators
    virtual void grasshopperDoSomething();
    
};

///////////////////////////////////////////////////////////////////////////
// No HP Actor Declarations
///////////////////////////////////////////////////////////////////////////

// TODO: Pebble

class Pebble : public Actor {
    
public:
    // Constructor / Destructor
    Pebble(int posX, int posY, StudentWorld *p) : Actor(IID_ROCK, posX, posY, p, right, 1) {}
    virtual ~Pebble() {} // TODO
    
    // Mutators
    virtual void doSomething() {}
    
};

////////////////////////////////////////////
// Active No HP Actor Declaration
////////////////////////////////////////////

class ActiveNoHPActor : public Actor {
    
public:
    // Constructor / Destructor
    ActiveNoHPActor(int imageID, int posX, int posY, StudentWorld *p, bool isPoison, Direction dir, int depth) : Actor(imageID, posX, posY, p, dir, depth), m_isPoison(isPoison) {}
    virtual ~ActiveNoHPActor() {} // TODO
    
    // Accessors
    bool isPoison() const { return m_isPoison; }
    
    // Mutators
    virtual void doSomething();
    virtual void specializedDoSomething() {}
    
private:
    bool m_isPoison;
    
};

// TODO: Pool of Water

class WaterPool : public ActiveNoHPActor {
    
public:
    // Constructor / Destructor
    WaterPool(int posX, int posY, StudentWorld *p) : ActiveNoHPActor(IID_WATER_POOL, posX, posY, p, false, right, 2) {}
    virtual ~WaterPool() {} // TODO
    
    // Mutators
    
};

// TODO: Poison

class Poison : public ActiveNoHPActor {
    
public:
    // Constructor / Destructor
    Poison(int posX, int posY, StudentWorld *p) : ActiveNoHPActor(IID_POISON, posX, posY, p, true, right, 2) {}
    virtual ~Poison() {} // TODO
    
    // Mutators
    
};

#endif // ACTOR_H_
