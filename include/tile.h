#ifndef TILE_H
#define TILE_H

#include <iostream>
#include <array>
#include <memory>
#include <cstdint>


class Tile
{
    uint32_t x;
    uint32_t y;

    // Used in maze generation
    // Any cases used are to be added here
    // 0: not used
    // 1: frontier
    // 2: done
    // 3: Room
    
    

public: 
    enum TYPE
    {
        EMPTY,
        WALL
    };

    TYPE data = EMPTY;

    
    Tile();
    Tile(uint32_t x, uint32_t y);
    // ~Tile();

    bool done = false;

    

    uint32_t getX();
    uint32_t getY();
    int getSiblings();

    bool isNorth(Tile &tile);
    bool isEast(Tile &tile);
    bool isSouth(Tile &tile);
    bool isWest(Tile &tile);

    bool northWall = true;
    bool eastWall = true;
    bool southWall = true;
    bool westWall = true;

    // Return 1 if fails
    bool open(Tile *tile);

    Tile *firstChild();
    Tile *nextSibling();
    Tile *prevSibling();

    // sets the firstChild or the next available sibling
    void addChild(Tile *tile);

    // gets back a random child
    Tile *randomChild();

    void display(bool recursive = false);

    friend std::ostream &operator<<(std::ostream &out, Tile &data);
};

#endif