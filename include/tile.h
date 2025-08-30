#ifndef TILE_H
#define TILE_H

#include <iostream>
#include <array>
#include <memory>

class Tile
{
private:
    unsigned int x;
    unsigned int y;

    // first child
    Tile *fChild = nullptr;

    // next sibling
    Tile *nSibling = nullptr;

    // previous sibling
    Tile *pSibling = nullptr;

    // A reference to a collective integer representing the number of siblings
    std::shared_ptr<int> siblings = std::make_shared<int>(int(1));
    // sets the nextSibling
    void addSibling(Tile *tile);

public:
    Tile();
    Tile(unsigned int x, unsigned int y);
    ~Tile();

    bool done = false;

    // Used in maze generation
    // Any cases used are to be added here
    // 0: not used
    // 1: frontier
    // 2: done
    // 3: Room
    int data = 0;

    const int getX();
    const int getY();
    const int getSiblings();

    bool isNorth(Tile &tile);
    bool isEast(Tile &tile);
    bool isSouth(Tile &tile);
    bool isWest(Tile &tile);

    bool northWall = true;
    bool eastWall = true;
    bool southWall = true;
    bool westWall = true;

    // Return 1 if fails
    int open(Tile *tile);
    
    Tile *firstChild();
    Tile *nextSibling();
    Tile *prevSibling();
    
    // sets the firstChild or the next available sibling
    void addChild(Tile *tile);

    // gets back a random child
    Tile *randomChild();

    void display(bool recursive = false);

    friend std::ostream& operator<<(std::ostream &out, Tile &data);
};

#endif