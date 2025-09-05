#pragma once

#include <iostream>
#include <array>
#include <list>
#include <stack>
#include <memory>

#include "tile.h"
#include "bitmap.h"

class Grid
{
private:
    static char getDisplayChar(int value);

    Tile *parent;

    int iteration = 0;

    int doneCount;

    std::stack<Tile *> frontier;

    Tile *root;

#ifdef DEBUG
    // Sets each tiles walls values depending on its children
    void doWalls(Tile *tile, bool recurse = true);

#endif
public:
    Tile **matrix;
    int width;
    int height;

    Grid(int x, int y);
    ~Grid();

    // Build the maze
    void generate();
    void generate(Tile &tile);

    // 1 step in generation
    // Returns true if can continue
    bool step();

    // Returns NESW
    std::list<Tile *> neighbors(Tile &tile, bool useStages = false);

    // Returns NESW
    std::list<Tile *> neighbors(int x, int y, bool useStages = false);

    void display();

    std::unique_ptr<BMP> getBitMap();

    void displayStages();

    void displayStages(int x, int y);

    friend std::ostream &operator<<(std::ostream &out, Grid &grid);
};