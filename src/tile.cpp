#include "tile.h"

#include <iostream>
#include <array>
#include <string>

using namespace std;

Tile::Tile()
{
}

Tile::Tile(uint32_t x, uint32_t y)
:x(x), y(y)
{
    // this->x = x;
    // this->y = y;
}



uint32_t Tile::getX()
{
    return this->x;
}

uint32_t Tile::getY()
{
    return this->y;
}



bool Tile::isNorth(Tile &tile)
{
    if (tile.y == (y + 1) && tile.x == x)
        return true;
    return false;
}

bool Tile::isEast(Tile &tile)
{
    if ((x - 1) == tile.x && tile.y == y)
        return true;
    return false;
}

bool Tile::isSouth(Tile &tile)
{
    if (tile.x == x && tile.y == (y - 1))
        return true;
    return false;
}

bool Tile::isWest(Tile &tile)
{
    if (tile.x == (x + 1) && tile.y == y)
        return true;
    return false;
}

bool Tile::open(Tile *tile)
{
    if (isNorth(*tile))
    {
        northWall = false;
        tile->southWall = false;
        return 0;
    }
    if (isEast(*tile))
    {
        eastWall = false;
        tile->westWall = false;
        return 0;
    }
    if (isSouth(*tile))
    {
        southWall = false;
        tile->northWall = false;
        return 0;
    }
    if (isWest(*tile))
    {
        westWall = false;
        tile->eastWall = false;
        return 0;
    }

    return 1;
}



// void Tile::display(bool recursive)
// {
//     // display as node tree
//     cout << *this;
//     if (!fChild)
//         return;
//     cout << endl;
//     int sibNum = fChild->getSiblings();
//     Tile *child = fChild;
//     child = fChild;
//     for (int i = 0; i < sibNum; i++)
//     {
//         child->display(recursive);
//         if (i + 1 < sibNum)
//         {
//             cout << " - ";
//         }
//         child = child->nextSibling();
//     }
// }

std::ostream &operator<<(std::ostream &out, Tile &data)
{
    out << '(' << data.x << ',' << data.y << ')';
    return out;
}