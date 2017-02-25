#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

///////////////////////////////////////////////////////////////////////////
// StudentWorld Implementation
///////////////////////////////////////////////////////////////////////////

bool StudentWorld::isBlockOn(int X, int Y) {
    
    Coordinate a(X, Y);
    
    if (mapOfActors[a].empty())
        return false;
    
    for (int i = 0; i < mapOfActors[a].size(); i++) {
        
        if (mapOfActors[a][i]->canBlockMovingActor())
            return true;
        
    }
    
    return false;
    
}

void StudentWorld::recordDeadActorPosition(int X, int Y) {
    
    actorsToBeRemoved.push_back(makeCoordinate(X, Y));
    
}

void StudentWorld::removeDeadActors() {
    
    if (actorsToBeRemoved.empty())
        return;
    
    while (!actorsToBeRemoved.empty()) {
        
        Coordinate a = actorsToBeRemoved[actorsToBeRemoved.size()-1];
        
        for (int i = 0; i < mapOfActors[a].size(); i++) {
            
            if (static_cast<HPActor *>(mapOfActors[a][i])->isDead()) {
                
                delete mapOfActors[a][i];
                mapOfActors[a].erase(remove(mapOfActors[a].begin(), mapOfActors[a].end(), mapOfActors[a][i]));
                i--;
                
            }
            
        }
        
        actorsToBeRemoved.pop_back();
        
    }
    
}

bool StudentWorld::attemptToMove(MobileHPActor *caller, int destX, int destY) {
    // Checks if a Mobile HP Actor can move to a square. Returns false if it can't, returns true and remembers
    // to update the pointer position of the Actor in mapOfActors if it can.
    
    Coordinate a(destX, destY);
    
    if (mapOfActors[a].size() == 1 && mapOfActors[a][0]->canBlockMovingActor())
        return false;
    
    else {
        
        actorsToBeMoved.push_back(caller);
        return true;
        
    }
    
}

bool StudentWorld::attemptToBite(MobileHPActor *caller, int X, int Y, unsigned int damage) {
    // unsigned int to prevent negative integer passes...
    
    Coordinate a(X, Y);
    
    bool callerHasColony = caller->hasColony();
    
    if (mapOfActors[a].size() == 1)
        return false;
    
    std::vector<MobileHPActor *> biteCandidates;
    
    for (int i = 0; i < mapOfActors[a].size(); i++) {
        
        if (!mapOfActors[a][i]->isHPActor() || mapOfActors[a][i] == caller)
            continue;
        
        if (static_cast<HPActor *>(mapOfActors[a][i])->isMobile()) {
            
            if (callerHasColony && static_cast<MobileHPActor *>(mapOfActors[a][i])->hasColony() && static_cast<Ant *>(mapOfActors[a][i])->getColonyNumber() == static_cast<Ant *>(mapOfActors[a][i])->getColonyNumber())
                continue;
            
            biteCandidates.push_back(static_cast<MobileHPActor *>(mapOfActors[a][i]));
            
        }
    }
    
    if (biteCandidates.empty())
        return false;
    
    int elementToBeBitten = randInt(0, biteCandidates.size()-1);
    
    biteCandidates[elementToBeBitten]->setHitpoints(-damage);
    
    if (biteCandidates[elementToBeBitten]->hitpoints() <= 0) {
        
        biteCandidates[elementToBeBitten]->setDead();
        return true;
        
    }
    
    AdultGrasshopper *aGPointer = dynamic_cast<AdultGrasshopper *>(biteCandidates[elementToBeBitten]);
    
    if (aGPointer != nullptr)
        attemptToBite(biteCandidates[elementToBeBitten], X, Y, 50);
    
    Ant *antPointer = dynamic_cast<Ant *>(biteCandidates[elementToBeBitten]);
    
    if (antPointer != nullptr)
        antPointer->setWasBitten();
    
    return true;
    
}

bool StudentWorld::detectActorOn(Ant *caller, int X, int Y, SearchTarget target) {
    
    Coordinate a(X, Y);
    
    switch (target) {
            
        case EnemyTarget: {
            
            for (int i = 0; i < mapOfActors[a].size(); i++) {
                
                if (mapOfActors[a][i]->isHPActor() && static_cast<HPActor *>(mapOfActors[a][i])->isMobile()) {
                    
                    Ant *pToAnt = dynamic_cast<Ant *>(mapOfActors[a][i]);
                    
                    if (pToAnt != nullptr && pToAnt->getColonyNumber() == caller->getColonyNumber())
                        continue;
                    
                    return true;
                    
                }
                
            }
            
            return false;
            
        }
            
        case FoodTarget: {
            
            for (int i = 0; i < mapOfActors[a].size(); i++) {
                
                if (mapOfActors[a][i]->isEdible())
                    return true;
                
            }
            
            return false;
            
        }
            
        case AnthillTarget: {
            
            for (int i = 0; i < mapOfActors[a].size(); i++) {
                
                Anthill *pToAnthill = dynamic_cast<Anthill *>(mapOfActors[a][i]);
                
                if (pToAnthill != nullptr && pToAnthill->getColonyNumber() == caller->getColonyNumber())
                    return true;
                
            }
            
            return false;
            
        }
            
        case PheromoneTarget: {
            
            for (int i = 0; i < mapOfActors[a].size(); i++) {
                
                if (mapOfActors[a][i]->isPheromone() && static_cast<Pheromone *>(mapOfActors[a][i])->getColonyNumber() == caller->getColonyNumber())
                    return true;
                
            }
            
            return false;
            
        }
            
        case DangerTarget: {
            
            for (int i = 0; i < mapOfActors[a].size(); i++) {
                
                if (mapOfActors[a][i]->isDangerous()) {
                    
                    Ant *pToAnt = dynamic_cast<Ant *>(mapOfActors[a][i]);
                    
                    if (pToAnt != nullptr && pToAnt->getColonyNumber() == caller->getColonyNumber())
                        continue;
                    
                    return true;
                    
                }
                
            }
            
            return false;
            
        }
            
    }
    
}

int StudentWorld::attemptToEat(int X, int Y, int amount) {
    // Called by an Actor. Function only modifies the Food Actor if found at position X, Y and returns
    // the amount of food eaten/picked up. Else, if no Food Actor found at X, Y, nothing is modified and
    // function returns -1.
    
    Coordinate a(X, Y);
    
    for (int i = 0; i < mapOfActors[a].size(); i++) {
        
        if (mapOfActors[a][i]->isEdible() && !static_cast<Food *>(mapOfActors[a][i])->isDead()) {
            
            Food* foodPointer = static_cast<Food *>(mapOfActors[a][i]);
            
            if (foodPointer->hitpoints() > amount) {
                
                foodPointer->setHitpoints(-amount);
                return amount;
                
            }
            
            else {
                
                int foodLeft = foodPointer->hitpoints();
                foodPointer->setHitpoints(-foodPointer->hitpoints());
                foodPointer->setDead();
                
                return foodLeft;
                
            }
            
        }
        
    }
    
    return -1;
    
}

void StudentWorld::moveActors() {
    // Moves the Actor pointers AFTER all doSomething() methods have been called during a tick.
    
    if (actorsToBeMoved.empty())
        return;
    
    for (int i = 0; i < actorsToBeMoved.size(); i++) {
        
        Coordinate start(actorsToBeMoved[i]->getStartX(), actorsToBeMoved[i]->getStartY());
        Coordinate dest(actorsToBeMoved[i]->getX(), actorsToBeMoved[i]->getY());
        
        mapOfActors[dest].push_back(actorsToBeMoved[i]);
        mapOfActors[start].erase(remove(mapOfActors[start].begin(), mapOfActors[start].end(), actorsToBeMoved[i]));
    
    }
    
    unsigned long size = actorsToBeMoved.size();
    
    for (int i = 0; i < size; i++)
        actorsToBeMoved.pop_back();
    
}

void StudentWorld::createFoodOn(int X, int Y, int amount) {
    
    Coordinate a(X, Y);
    
    for (int i = 0; i < mapOfActors[a].size(); i++) {
        
        if (mapOfActors[a][i]->isEdible()) {
            
            static_cast<Food *>(mapOfActors[a][i])->setHitpoints(amount);
            return;
            
        }
        
    }
    
    mapOfActors[a].push_back(new Food(X, Y, this, amount));
    
}

void StudentWorld::createPheromoneOn(Ant *caller) {
    
    Coordinate a(caller->getX(), caller->getY());
    
    for (int i = 0; i < mapOfActors[a].size(); i++) {
        
        if (mapOfActors[a][i]->isPheromone()) {
            
            Pheromone *pToPheromone = static_cast<Pheromone *>(mapOfActors[a][i]);
            
            if (pToPheromone->getColonyNumber() != caller->getColonyNumber())
                continue;
            
            if (pToPheromone->hitpoints() <= 512)
                pToPheromone->setHitpoints(256);
            
            else
                pToPheromone->setHitpoints(768 - pToPheromone->hitpoints());
                
            return;
            
        }
        
    }
    
    mapOfActors[a].push_back(new Pheromone(caller->getColonyNumber(), a.getX(), a.getY(), this));
    
}

void StudentWorld::growUpGrasshopper(int X, int Y) {
    
    Coordinate a(X, Y);
    mapOfActors[a].push_back(new AdultGrasshopper(X, Y, this));
    
}

void StudentWorld::recordJump(AdultGrasshopper *caller, int startX, int startY, int destX, int destY) {
    
    actorsToBeMoved.push_back(caller);
    
}

void StudentWorld::createAnt(Anthill *caller) {
    
    const int callerColony = caller->getColonyNumber();
    
    Coordinate a(caller->getX(), caller->getY());
    mapOfActors[a].push_back(new Ant(callerColony, a.getX(), a.getY(), this));
    
    m_antCount[callerColony]++;
    
    if (m_tickCount > 5) {
    
        bool isLargestColony = true;
        
        for (int i = 0; i < m_antCount.size(); i++) {
            
            if (callerColony != i && m_antCount[callerColony] <= m_antCount[i])
                isLargestColony = false;
            
        }
        
        if (isLargestColony)
            m_winningAntColony = callerColony;
        
    }
    
}

void StudentWorld::stunActors(ActiveNoHPActor *caller, int X, int Y) {
    
    Coordinate a(X, Y);
    
    if (mapOfActors[a].size() == 1)
        return;
    
    for (int i = 1; i < mapOfActors[a].size(); i++) {
        // i = 1 because Poison object is always the first element of the vector.
        
        if (!static_cast<HPActor *>(mapOfActors[a][i])->isMobile())
            continue;
        
        MobileHPActor *mHPActorPointer = static_cast<MobileHPActor *>(mapOfActors[a][i]);
        
        if (caller->isPoison()) {
            
            if (dynamic_cast<AdultGrasshopper *>(mHPActorPointer) != nullptr)
                continue;
            
            mHPActorPointer->setHitpoints(-150);
            
            if (mHPActorPointer->hitpoints() <= 0)
                mHPActorPointer->setDead();
            
        }
        
        else {
            
            if (!mHPActorPointer->isStunned()) {
                
                mHPActorPointer->adjustTicksToSleep(2);
                mHPActorPointer->setStunned(true);
                
            }
            
        }
        
    }
    
}
