#ifndef FRAMEWORK_H
#define FRAMEWORK_H
#pragma once
#include <string>
#include "raylib.h"
#include <cmath>

#include <unordered_set>

class Demo {

public:
	Demo(const int screenWidth=1024, const int screenHeight=768, 
		const std::string title="SimpleFramework::Demo");
	virtual ~Demo();

	virtual void init()     = 0;
	virtual void shutdown() = 0;
	virtual void draw()		= 0;
	virtual void update( )	= 0;

	virtual void run();

protected:
	int screenWidth, screenHeight;
	int framesCounter;
	bool pause;

	const Color TRANSPARENT_WHITE;
};

class BallCollisionDemo :public Demo
{
public:
	BallCollisionDemo(const int screenWidth, const int screenHeight, const std::string title);
	virtual ~BallCollisionDemo();

	void init();
	void shutdown();
	void draw();
	void update( );

private:
	Vector2 ballPosition;
	Vector2 ballSpeed;
	int ballRadius;
};

#include <vector>

#include <unordered_map>


class Hash {
public:
    double spacing;
    size_t tableSize; // Changed from int to size_t
    std::vector<size_t> cellStart; // Changed from int to size_t
    std::vector<int> cellEntries;
    std::vector<int> queryIds;
    int querySize;

    Hash(double spacing, size_t maxNumObjects) // Changed from int to size_t
        : spacing(spacing), tableSize(2 * maxNumObjects),
          cellStart(tableSize + 1, 0), cellEntries(maxNumObjects, 0),
          queryIds(maxNumObjects, 0), querySize(0) {}

    // Hash() : spacing(1.0), tableSize(1000), querySize(0) {
    //     cellStart.assign(tableSize + 1, 0);
    //     cellEntries.assign(tableSize, 0);
    //     queryIds.assign(tableSize, 0);
    // }

    size_t hashCoords(Vector2 pos) { // Changed from int to size_t
        int xi = static_cast<int>(pos.x / spacing);
        int yi = static_cast<int>(pos.y / spacing);
        int h = (xi * 92837111) ^ (yi * 689287499); // fantasy function
        return static_cast<size_t>(std::abs(h) % tableSize);
    }

    int intCoord(double coord) {
        return static_cast<float>(std::floor(coord / spacing));
    }

    size_t hashPos(std::vector<Vector2>& pos, size_t nr) { // Changed from int to size_t
        size_t i = nr;
        return hashCoords(pos[i]);
    }

    void create(std::vector<Vector2>& pos) { // Changed from std::vector<double>& to std::vector<Vector2>&
        size_t numObjects = std::min(pos.size(), cellEntries.size()); // Changed from int to size_t

        // Determine cell sizes
        cellStart.assign(tableSize + 1, 0);
        cellEntries.assign(cellEntries.size(), 0);

        for (size_t i = 0; i < numObjects; i++) { // Changed from int to size_t
            size_t h = hashPos(pos, i);
            cellStart[h]++;
        }

        // Determine cells starts
        size_t start = 0;
        for (size_t i = 0; i < tableSize; i++) { // Changed from int to size_t
            start += cellStart[i];
            cellStart[i] = start;
        }
        cellStart[tableSize] = start; // guard

        // Fill in objects ids
        for (size_t i = 0; i < numObjects; i++) { // Changed from int to size_t
            size_t h = hashPos(pos, i);
            cellStart[h]--;
            cellEntries[cellStart[h]] = static_cast<int>(i);
        }
    }

    void query(std::vector<Vector2>& pos, size_t nr, double maxDist) { // Changed from std::vector<double>& to std::vector<Vector2>&
        Vector2 p = pos[nr];
        int x0 = intCoord(p.x - maxDist);
        int y0 = intCoord(p.y - maxDist);
        int x1 = intCoord(p.x + maxDist);
        int y1 = intCoord(p.y + maxDist);

        querySize = 0;

        for (int xi = x0; xi <= x1; xi++) {
            for (int yi = y0; yi <= y1; yi++) {
                size_t h = hashCoords({ static_cast<float>(xi * spacing), static_cast<float>(yi * spacing) });
                size_t start = cellStart[h];
                size_t end = cellStart[h + 1];

                for (size_t i = start; i < end; i++) {
                    queryIds[querySize] = cellEntries[i];
                    querySize++;
                }
            }
        }
    }
};

class NBallsCollisionDemo :public Demo
{


public:
	NBallsCollisionDemo(const int screenWidth, const int screenHeight, const std::string title, const int MAX_BALLS=5000);
	virtual ~NBallsCollisionDemo();

	void init();
	void shutdown();
	void draw();
	void update();
	

private:
	std::vector<Vector2> ballPositions;
	std::vector<Vector2> ballSpeeds;
	std::vector<bool>    ballColliding;
	int 				 ballRadius, 
						 ballMinDistance, 
						 ballMinDistance2, 
						 totalCollisions;
	const int 			 MAX_BALLS;
	bool 				 useSpatialHashing; 
    bool                 HashCreate;
    // Hash                 hash;
    

};






#endif



