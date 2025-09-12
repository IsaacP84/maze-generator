#pragma once

#include <iostream>
#include <array>
#include <list>
#include <stack>
#include <vector>
#include <memory>


#include "cell.h"
#include "tile.h"
#include "bitmap.h"

class Grid
{
private:
    static char getDisplayChar(int value);

    Tile *parent;

    int iteration = 0;

    int doneCount;

    

    Tile *root;

#ifdef DEBUG
    // Sets each tiles walls values depending on its children
    void doWalls(Tile *tile, bool recurse = true);

#endif
public:
    Tile **matrix;
    const uint32_t width;
    const uint32_t height;

    // storage for connecting grids
    

    Grid(uint32_t x, uint32_t y);
    ~Grid();

    // Build the maze
    void generate();
    void generate(Tile &tile);

    // 1 step in generation
    // Returns true if can continue
    bool step();

    // Returns NESW
    std::list<Tile *> neighbors(Tile &tile);

    // Returns NESW
    std::list<Tile *> neighbors(uint32_t x, uint32_t y);

    void display();

    std::unique_ptr<BMP> getBitMap();

    void displayStages();

    void displayStages(int x, int y);

    friend std::ostream &operator<<(std::ostream &out, Grid &grid);
};

void randomizedDFS(std::vector<std::vector<Cell>>& maze, int start_row, int start_col);