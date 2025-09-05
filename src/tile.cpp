#include "tile.h"

#include <iostream>
#include <array>
#include <string>

using namespace std;

Tile::Tile()
{
}

Tile::Tile(unsigned int x, unsigned int y)
{
    this->x = x;
    this->y = y;
}

Tile::~Tile()
{
    (*siblings)--;
    if (nSibling)
        nSibling->pSibling = pSibling;
    if (pSibling)
        pSibling->nSibling = nSibling;
}

int Tile::getX()
{
    return this->x;
}

int Tile::getY()
{
    return this->y;
}

int Tile::getSiblings()
{
    return *siblings;
}

Tile *Tile::firstChild()
{
    return fChild;
}

Tile *Tile::nextSibling()
{
    return nSibling;
}

Tile *Tile::prevSibling()
{
    return pSibling;
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

int Tile::open(Tile *tile)
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

void Tile::addChild(Tile *tile)
{
    open(tile);
    if (fChild)
    {
        fChild->addSibling(tile);
        return;
    }

    fChild = tile;
    // this is working now
    // it was saying the integer was 0
    // i dont know why
    *(fChild->siblings) = 1;
}

void Tile::addSibling(Tile *tile)
{
    // do a check to see if the tile already has siblings
    if (nSibling)
    {
        nSibling->addSibling(tile);
        return;
    }

    // keep this one and delete the tiles
    // not 100% sure about this
    // might make memory leak
    // its using a shared pointer tho
    tile->siblings = siblings;

    // siblings
    nSibling = tile;
    tile->pSibling = this;

    // update sibling counts
    (*siblings)++;
}

Tile *Tile::randomChild()
{
    int sibNum = fChild->getSiblings();
    if (sibNum == 1)
        return fChild;

    int index = rand() % sibNum;
    cout << "Random index: " << index << endl;

    Tile **children = new Tile *[sibNum];

    Tile *child = fChild;
    for (int i = 0; i < sibNum; i++)
    {
        children[i] = child;
        child = child->nextSibling();
    }

    child = children[index];
    delete[] children;
    return child;
}

void Tile::display(bool recursive)
{
    // display as node tree
    cout << *this;
    if (!fChild)
        return;
    cout << endl;
    int sibNum = fChild->getSiblings();
    Tile *child = fChild;
    child = fChild;
    for (int i = 0; i < sibNum; i++)
    {
        child->display(recursive);
        if (i + 1 < sibNum)
        {
            cout << " - ";
        }
        child = child->nextSibling();
    }
}

std::ostream &operator<<(std::ostream &out, Tile &data)
{
    out << '(' << data.x << ',' << data.y << ')';
    return out;
}