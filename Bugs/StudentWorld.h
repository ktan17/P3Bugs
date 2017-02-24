#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include "Actor.h"
#include <string>
#include <set>
#include <map>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    // Constructor / Destructor
    StudentWorld(std::string assetDir)
    : GameWorld(assetDir), m_tickCount(0)
    {
    }
    
    ~StudentWorld() {
        
        std::map<Coordinate, std::vector<Actor *>>::iterator it;
        
        for (it = mapOfActors.begin(); it != mapOfActors.end(); it++)
            for (int i = 0; i < (*it).second.size(); i++)
                if ((*it).second[i] != nullptr)
                    delete (*it).second[i];
            
    }
    
    // Required functions
    virtual int init()
    {
        
        m_tickCount = 0;
        
        std::string fieldFile = getFieldFilename();
        Field f;
        std::string error;
        
        if (f.loadField(fieldFile, error) != Field::LoadResult::load_success) {
            
            setError(fieldFile + " " + error);
            return GWSTATUS_LEVEL_ERROR;
            
        }
        
        for (int i = 0; i < VIEW_WIDTH; i++) {
            
            for (int j = 0; j < VIEW_HEIGHT; j++) {
                
                switch (f.getContentsOf(i, j)) {
                        
                    case Field::empty:
                        emptyCoordinates.insert(makeCoordinate(i, j));
                        break;
                        
                    case Field::rock:
                        mapOfActors.insert({makeCoordinate(i, j), {new Pebble(i, j, this)}});
                        break;
                        
                    case Field::water:
                        mapOfActors.insert({makeCoordinate(i, j), {new WaterPool(i, j, this)}});
                        break;
                        
                    case Field::poison:
                        mapOfActors.insert({makeCoordinate(i, j), {new Poison(i, j, this)}});
                        break;
                        
                    case Field::food:
                        mapOfActors.insert({makeCoordinate(i, j), {new Food(i, j, this)}});
                        break;
                        
                    case Field::anthill0:
                        mapOfActors.insert({makeCoordinate(i, j), {new Anthill(0, i, j, this)}});
                        m_antCount.push_back(0);
                        break;
                        
                    case Field::anthill1:
                        mapOfActors.insert({makeCoordinate(i, j), {new Anthill(1, i, j, this)}});
                        m_antCount.push_back(0);
                        break;
                        
                    case Field::anthill2:
                        mapOfActors.insert({makeCoordinate(i, j), {new Anthill (2, i, j, this)}});
                        m_antCount.push_back(0);
                        break;
                        
                    case Field::anthill3:
                        mapOfActors.insert({makeCoordinate(i, j), {new Anthill (3, i, j, this)}});
                        m_antCount.push_back(0);
                        break;
                        
                    case Field::grasshopper:
                        mapOfActors.insert({makeCoordinate(i, j), {new BabyGrasshopper(i, j, this)}});
                        break;
                        
                }
                
            }
            
        }
        
        return GWSTATUS_CONTINUE_GAME;
    }
    
    virtual int move()
    {
        
        m_tickCount++;
        
        std::map<Coordinate, std::vector<Actor *>>::iterator it;
        
        for (it = mapOfActors.begin(); it != mapOfActors.end(); it++) {
            
            for (int i = 0; i < (*it).second.size(); i++) 
                (*it).second[i]->doSomething();
            
        }
        
        removeDeadActors();
        moveActors();
        
        if (m_tickCount < 2000)
            return GWSTATUS_CONTINUE_GAME;
        else
            return GWSTATUS_PLAYER_WON;
        
    }
    
    virtual void cleanUp()
    {
        std::map<Coordinate, std::vector<Actor *>>::iterator it;
        
        for (it = mapOfActors.begin(); it != mapOfActors.end(); it++)
            for (int i = 0; i < (*it).second.size(); i++) {
                
                delete (*it).second[i];
                (*it).second[i] = nullptr;
                
            }
    }
    
    // Accessors
    bool isBlockOn(int X, int Y);
    
    // Mutators
    void setDisplayText();
    
    // For HP Actors
    void recordDeadActorPosition(int X, int Y);
    void removeDeadActors();
    
    // For Ants and Grasshoppers
    bool attemptToMove(MobileHPActor *caller, int destX, int destY);
    bool attemptToBite(MobileHPActor *caller, int X, int Y, unsigned int damage);
    int attemptToEat(int X, int Y, int amount);
    void moveActors();
    void createFoodOn(int X, int Y);
    void growUpGrasshopper(int X, int Y);
    void recordJump(AdultGrasshopper *caller, int startX, int startY, int destX, int destY);
    void createAnt(Anthill* caller);
    
    // For Poison and Water Pools
    void stunActors(ActiveNoHPActor *caller, int X, int Y);
    
private:
    
    struct Coordinate {
        
    public:
        Coordinate(int X, int Y) : m_x(X), m_y(Y) {}
        bool operator< (const Coordinate &other) const {
            
            if (m_x < other.m_x)
                return true;
            
            else if (m_x > other.m_x)
                return false;
            
            else {
                
                if (m_y < other.m_y)
                    return true;
                
                else
                    return false;
                
            }
            
        }
        
        int getX() const { return m_x; }
        int getY() const { return m_y; }
        
    private:
        int m_x;
        int m_y;
        
    };
    
    std::set<Coordinate> emptyCoordinates;
    std::map<Coordinate, std::vector<Actor *>> mapOfActors;
    std::vector<Coordinate> actorsToBeRemoved;
    std::vector<MobileHPActor *> actorsToBeMoved;
    
    Coordinate makeCoordinate(int X, int Y) const {
        
        Coordinate a(X, Y);
        return a;
        
    }
    
    int m_tickCount;
    std::vector<int> m_antCount;
    
};

#endif // STUDENTWORLD_H_
