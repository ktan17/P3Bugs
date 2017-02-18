#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////////////////////////////////////////////////
// Abstract Actor Base Class
///////////////////////////////////////////////////////////////////////////

class Actor : public GraphObject {
    
public:
    // Constructor / Destructor
    Actor(int imageID, int startX, int startY, Direction dir = none) : GraphObject(imageID, startX, startY, dir) {}
    virtual ~Actor() {} // TODO
    
    // Mutators
    virtual void doSomething() = 0;
    
    
};

///////////////////////////////////////////////////////////////////////////
// HP Actor Declaration
///////////////////////////////////////////////////////////////////////////

class HPActor : public Actor {
    
public:
    // Constructor / Destructor
    HPActor(int startingHP, int imageID, int startX, int startY, Direction dir = none) : Actor(imageID, startX, startY, dir) { m_hitpoints = startingHP; }
    virtual ~HPActor() {} // TODO
    
    // Accessors
    int hitpoints() const { return m_hitpoints; }
    
    // Mutators
    virtual void doSomething() = 0;
    
private:
    int m_hitpoints;
    
};

////////////////////////////////////////////
// Static HP Actor Declaration
////////////////////////////////////////////

class StaticHPActor : public HPActor {
    
public:
    // Constructor / Destructor
    StaticHPActor(int startingHP, int imageID, int posX, int posY); // TODO
    virtual ~StaticHPActor() {} // TODO
    
    // Mutators
    virtual void doSomething() = 0;
    
};

// TODO: Food

class Food : public StaticHPActor {
    
public:
    // Constructor / Destructor
    Food(int posX, int posY); // TODO
    virtual ~Food(); // TODO
    
    // Mutators
    virtual void doSomething(); // TODO;
    
};

// TODO: Pheromone

class Pheromone : public StaticHPActor {
    
public:
    // Constructor / Destructor
    Pheromone(int antColony, int posX, int posY); // TODO
    virtual ~Pheromone(); // TODO
    
    // Mutators
    virtual void doSomething(); // TODO
    
};

// TODO: Anthill

class Anthill : public StaticHPActor {
    
public:
    // Constructor / Destructor
    Anthill(int antColony, int posX, int postY); // TODO
    virtual ~Anthill();
    
    // Mutators
    virtual void doSomething(); // TODO
    
};

////////////////////////////////////////////
// Mobile HP Actor Declaration
////////////////////////////////////////////

class MobileHPActor : public HPActor {
    
public:
    // Constructor / Destructor
    MobileHPActor(int startingHP, int imageID, int startX, int startY);
    virtual ~MobileHPActor() {} // TODO
    
    // Helper Functions
    Direction generateRandomDirection();
    
    // Mutators
    virtual void doSomething() = 0;
    
private:
    int m_ticksToSleep;
    
};

// TODO: Ant

class Compiler;

class Ant : public MobileHPActor {
    
public:
    // Constructor / Destructor
    Ant(int startX, int startY); // TODO
    virtual ~Ant(); // TODO
    
    // Mutators
    virtual void doSomething(); // TODO
    
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
    Grasshopper(int startingHP, int imageID, int startX, int startY) : MobileHPActor(startingHP, imageID, startX, startY) {}
    virtual ~Grasshopper() {} // TODO
    
    // Mutators
    virtual void doSomething() = 0;
    
};

class BabyGrasshopper : public Grasshopper {
    
public:
    // Constructor / Destructor
    BabyGrasshopper(int startX, int startY); // TODO
    virtual ~BabyGrasshopper() {} // TODO
    
    // Mutators
    virtual void doSomething(); // TODO
    
};

class AdultGrasshopper : public Grasshopper {
    
public:
    // Constructor / Destructor
    AdultGrasshopper(int startX, int startY); // TODO
    virtual ~AdultGrasshopper() {} // TODO
    
    // Mutators
    virtual void doSomething(); // TODO
    
};

///////////////////////////////////////////////////////////////////////////
// No HP Actor Declaration
///////////////////////////////////////////////////////////////////////////

class NoHPActor : public Actor {
    
public:
    // Constructor / Destructor
    NoHPActor(int imageID, int startX, int startY) : Actor(imageID, startX, startY) {}
    virtual ~NoHPActor(); // TODO
    
    // Mutators
    virtual void doSomething() = 0;
    
};

// TODO: Pebble

class Pebble : public NoHPActor {
    
public:
    // Constructor / Destructor
    Pebble(int posX, int posY) : NoHPActor(IID_ROCK, posX, posY) {}
    virtual ~Pebble(); // TODO
    
    // Mutators
    virtual void doSomething() {}
    
};

// TODO: Pool of Water

class WaterPool : public NoHPActor {
    
public:
    // Constructor / Destructor
    WaterPool(int posX, int posY) : NoHPActor(IID_WATER_POOL, posX, posY) {}
    virtual ~WaterPool(); // TODO
    
    // Mutators
    virtual void doSomething(); // TODO
    
};

// TODO: Poison

class Poison : public NoHPActor {
    
public:
    // Constructor / Destructor
    Poison(int posX, int posY) : NoHPActor(IID_POISON, posX, posY) {}
    virtual ~Poison(); // TODO
    
    // Mutators
    virtual void doSomething(); // TODO
    
};

#endif // ACTOR_H_
