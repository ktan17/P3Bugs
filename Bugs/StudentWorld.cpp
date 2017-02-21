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

void StudentWorld::recordDeadActorPosition(int X, int Y) {
    
    actorsToBeRemoved.push_back(makeCoordinate(X, Y));
    
}

void StudentWorld::removeDeadActors() {
    
    if (actorsToBeRemoved.empty())
        return;
    
    while (!actorsToBeRemoved.empty()) {
        
        Coordinate a = actorsToBeRemoved[actorsToBeRemoved.size()-1];
        
        for (int i = 0; i < mapOfActors[a].size(); i++) {
            
            if (mapOfActors[a][i]->isDead()) {
                
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

int StudentWorld::attemptToEat(int X, int Y, int amount) {
    // Called by an Actor. Function only modifies the Food Actor if found at position X, Y and returns
    // the amount of food eaten. Else, if no Food Actor found at X, Y, nothing is modified and function
    // returns -1.
    
    Coordinate a(X, Y);
    
    for (int i = 0; i < mapOfActors[a].size(); i++) {
        
        if (dynamic_cast<Food *>(mapOfActors[a][i]) != nullptr && !mapOfActors[a][i]->isDead()) {
            
            if (mapOfActors[a][i]->hitpoints() > 200) {
                
                mapOfActors[a][i]->setHitpoints(-200);
                return 200;
                
            }
            
            else {
                
                int foodLeft = mapOfActors[a][i]->hitpoints();
                mapOfActors[a][i]->setHitpoints(-mapOfActors[a][i]->hitpoints());
                mapOfActors[a][i]->setDead();
                
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
            
            mapOfActors[a][i]->setHitpoints(100);
            return;
            
        }
        
    }
    
    mapOfActors[a].push_back(new Food(X, Y, this, 100));
    
}

void StudentWorld::growUpGrasshopper(int posX, int posY) {
    
    Coordinate a(posX, posY);
    mapOfActors[a].push_back(new AdultGrasshopper(posX, posY, this));
    
}
