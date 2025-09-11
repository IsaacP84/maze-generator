#pragma once
#include <cstdint>
// Structure to represent a cell in the maze
struct Cell
{
    uint32_t row, col;
    bool visited;
    bool wall_up, wall_down, wall_left, wall_right;

    Cell(uint32_t r, uint32_t c) : row(r), col(c), visited(false),
                         wall_up(true), wall_down(true), wall_left(true), wall_right(true) {}
};