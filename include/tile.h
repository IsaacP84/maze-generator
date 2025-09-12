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

public: 
    enum TYPE
    {
        EMPTY,
        WALL,
        ROOM
    };

    TYPE data = EMPTY;

    
    Tile();
    Tile(uint32_t x, uint32_t y);
    // ~Tile();

    uint32_t getX();
    uint32_t getY();

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

    void display(bool recursive = false);

    friend std::ostream &operator<<(std::ostream &out, Tile &data);
};

#endif