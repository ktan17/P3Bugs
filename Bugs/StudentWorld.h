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
    : GameWorld(assetDir)
    {
        
        m_tickCount = 0;
        
    }
    
    ~StudentWorld() {
        
    }
    
    // Required functions
    virtual int init()
    {
        
        std::string fieldFile = getFieldFilename();
        Field f;
        
        if (f.loadField(fieldFile) != Field::LoadResult::load_success)
            return GWSTATUS_LEVEL_ERROR;
        
        for (int i = 0; i < VIEW_WIDTH; i++)
            for (int j = 0; j < VIEW_HEIGHT; i++) {
                
                switch (f.getContentsOf(i, j)) {
                        
                    case Field::empty:
                        emptyCoordinates.insert(makeCoordinate(i, j));
                        break;
                        
                    case Field::rock:
                        mapOfActors[makeCoordinate(i, j)].push_back(new Pebble(i, j));
                        break;
                        
                    case Field::water:
                        mapOfActors[makeCoordinate(i, j)].push_back(new WaterPool(i, j));
                        break;
                        
                    case Field::poison:
                        mapOfActors[makeCoordinate(i, j)].push_back(new Poison(i, j));
                        break;
                        
                    case Field::food:
                        mapOfActors[makeCoordinate(i, j)].push_back(new Food(i, j));
                        break;
                        
                    case Field::anthill0:
                        mapOfActors[makeCoordinate(i, j)].push_back(new Anthill(0, i, j));
                        break;
                        
                    case Field::anthill1:
                        mapOfActors[makeCoordinate(i, j)].push_back(new Anthill(1, i, j));
                        break;
                        
                    case Field::anthill2:
                        mapOfActors[makeCoordinate(i, j)].push_back(new Anthill(2, i, j));
                        break;
                        
                    case Field::anthill3:
                        mapOfActors[makeCoordinate(i, j)].push_back(new Anthill(3, i, j));
                        break;
                        
                    case Field::grasshopper:
                        mapOfActors[makeCoordinate(i, j)].push_back(new BabyGrasshopper(i, j));
                        break;
                        
                }
                
            }
        
        return GWSTATUS_CONTINUE_GAME;
    }
    
    virtual int move()
    {
        // This code is here merely to allow the game to build, run, and terminate after you hit enter.
        // Notice that the return value GWSTATUS_NO_WINNER will cause our framework to end the simulation.
        return GWSTATUS_NO_WINNER;
    }
    
    virtual void cleanUp()
    {
        std::map<Coordinate, std::vector<Actor *>>::iterator it;
        
        for (it = mapOfActors.begin(); it != mapOfActors.end(); it++)
            for (int i = 0; i < (*it).second.size(); i++)
                delete (*it).second[i];
    }
    
    // Accessors
    bool isFoodOn(int X, int Y);
    
    // Mutators
    void recordDeadActorPosition(int X, int Y);
    bool attemptToMove(Actor *caller, int startX, int startY, int destX, int destY);
    void createFoodOn(int X, int Y);
    int attemptToEat(int X, int Y, int amount);
    
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
    
    Coordinate makeCoordinate(int X, int Y) const {
        
        Coordinate a(X, Y);
        return a;
        
    }
    
    int m_tickCount;
    
};

#endif // STUDENTWORLD_H_
