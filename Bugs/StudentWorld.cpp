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

bool StudentWorld::isFoodOn(int X, int Y) {
    
    Coordinate a(X, Y);
    
    if (mapOfActors[a].empty())
        return false;
    
    for (int i = 0; i < mapOfActors[a].size(); i++) {
        
        if (dynamic_cast<Food *>(mapOfActors[a][i]) != nullptr)
            return true;
        
    }
    
    return false;
    
}

void StudentWorld::setDisplayText() {
    
    int ticks = m_tickCount;
    int antsAnt0, antsAnt1, antsAnt2, antsAnt3; int winningAntNumber;
    
    //TODO!
    /*antsAnt0 = getNumberOfAntsForAnt(0);
    antsAnt1 = getNumberOfAntsForAnt(1);
    antsAnt2 = getNumberOfAntsForAnt(2);
    antsAnt3 = getNumberOfAntsForAnt(3);
    
    winningAntNumber = getWinningAntNumber();

    string s = someFunctionToFormatThingsNicely(ticks, antsAnt0,
                                                antsAnt1, antsAnt2, antsAnt3, winningAntNumber );
    
    // Finally, update the display text at the top of the screen with your
    // newly created stats
    setGameStatText(s); // calls our provided GameWorld::setGameStatText*/
    
    
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
        
        if (mapOfActors[a].empty())
            emptyCoordinates.insert(a);
        
        actorsToBeRemoved.pop_back();
        
    }
    
}

bool StudentWorld::attemptToMove(MobileHPActor *caller, int startX, int startY, int destX, int destY) {
    
    Coordinate a(destX, destY);
    
    if (mapOfActors[a].size() == 1 && dynamic_cast<Pebble *>(mapOfActors[a][0]) != nullptr)
        return false;
    
    else {
        
        actorsToBeMoved.push_back(caller);
        return true;
        
    }
    
}

bool StudentWorld::attemptToBite(MobileHPActor *caller, int X, int Y, int damage) {
    
    return false;
    
}

int StudentWorld::attemptToEat(int X, int Y, int amount) {
    // Called by an Actor. Function only modifies the Food Actor if found at position X, Y and returns
    // the amount of food eaten. Else, if no Food Actor found at X, Y, nothing is modified and function
    // returns -1.
    
    Coordinate a(X, Y);
    
    for (int i = 0; i < mapOfActors[a].size(); i++) {
        
        if (dynamic_cast<Food *>(mapOfActors[a][i]) != nullptr && !static_cast<Food *>(mapOfActors[a][i])->isDead()) {
            
            Food* foodPointer = static_cast<Food *>(mapOfActors[a][i]);
            
            if (foodPointer->hitpoints() > 200) {
                
                foodPointer->setHitpoints(-200);
                return 200;
                
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
    
    if (actorsToBeMoved.empty())
        return;
    
    for (int i = 0; i < actorsToBeMoved.size(); i++) {
        
        Coordinate start(actorsToBeMoved[i]->getStartX(), actorsToBeMoved[i]->getStartY());
        Coordinate dest(actorsToBeMoved[i]->getX(), actorsToBeMoved[i]->getY());
        
        if (emptyCoordinates.find(dest) != emptyCoordinates.end())
            emptyCoordinates.erase(dest);
        
        mapOfActors[dest].push_back(actorsToBeMoved[i]);
        
        mapOfActors[start].erase(remove(mapOfActors[start].begin(), mapOfActors[start].end(), actorsToBeMoved[i]));
        
        if (mapOfActors[start].empty())
            emptyCoordinates.insert(start);
    
    }
    
    unsigned long size = actorsToBeMoved.size();
    
    for (int i = 0; i < size; i++)
        actorsToBeMoved.pop_back();
    
}

void StudentWorld::createFoodOn(int X, int Y) {
    
    Coordinate a(X, Y);
    
    for (int i = 0; i < mapOfActors[a].size(); i++) {
        
        if (dynamic_cast<Food *>(mapOfActors[a][i]) != nullptr) {
            
            static_cast<Food *>(mapOfActors[a][i])->setHitpoints(100);
            return;
            
        }
        
    }
    
    mapOfActors[a].push_back(new Food(X, Y, this, 100));
    
}

void StudentWorld::growUpGrasshopper(int X, int Y) {
    
    Coordinate a(X, Y);
    mapOfActors[a].push_back(new AdultGrasshopper(X, Y, this));
    
}

void StudentWorld::stunActors(ActiveNoHPActor *caller, int X, int Y) {
    
    Coordinate a(X, Y);
    
    if (mapOfActors[a].size() == 1)
        return;
    
    for (int i = 1; i < mapOfActors[a].size(); i++) {
        
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
