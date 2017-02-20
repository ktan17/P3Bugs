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

bool StudentWorld::attemptToMove(Actor *caller, int startX, int startY, int destX, int destY) {
    
    Coordinate a(destX, destY);
    
    if (mapOfActors[a].size() == 1 && dynamic_cast<Pebble *>(mapOfActors[a][0]) != nullptr) {
        
        if (emptyCoordinates.find(a) != emptyCoordinates.end())
            emptyCoordinates.erase(a);
            
        mapOfActors[a].push_back(caller);
        
        Coordinate b(startX, startY);
        mapOfActors[b].erase(remove(mapOfActors[b].begin(), mapOfActors[b].end(), caller));
        
        if (mapOfActors[b].empty())
            emptyCoordinates.insert(b);
            
        return true;
            
    }
    
    else
        return false;
    
}

void StudentWorld::createFoodOn(int X, int Y) {
    
    Coordinate a(X, Y);
        
    for (int i = 0; i < mapOfActors[a].size(); i++) {
        
        if (dynamic_cast<Food *>(mapOfActors[a][i]) != nullptr) {
            
            mapOfActors[a][i]->setHitpoints(100);
            return;
            
        }
        
    }
    
    mapOfActors[a].push_back(new Food(X, Y, 100));
    
}

int StudentWorld::attemptToEat(int X, int Y, int amount) {
    
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
