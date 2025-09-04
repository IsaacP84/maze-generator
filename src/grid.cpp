#include "tile.h"
#include "grid.h"
#include "bitmap.h"

#include <array>
#include <stack>
#include <list>
#include <iostream>
#include <memory>

using namespace std;

Grid::Grid(int x, int y)
{
    width = x;
    height = y;
    matrix = new Tile *[width];
    for (int i = 0; i < width; i++)
    {
        matrix[i] = new Tile[height];
        for (int j = 0; j < height; j++)
        {
            matrix[i][j] = Tile(i, j);
        }
    }

    doneCount = 0;
}

Grid::~Grid()
{
    // deallocate
    for (int i = 0; i < width; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void Grid::generate()
{
    generate(this->matrix[0][0]);
}

void Grid::generate(Tile &root)
{
    cout << "\33[2K\r"
         << "Beginning Generation";

    // add to frontier
    this->root = &root;
    root.data = 1;
    frontier.push(&root);

    // Set parent
    parent = nullptr;
    cout << "\33[2K\r"
         << "Generating" << endl;
    while (step(false))
        ; // Purposefully empty
    cout << "\33[2K\r"
         << "\r\033[A";
    cout << "Finished Generation";
}

bool Grid::step(bool showDebug)
{
    if (showDebug)
    {
        cout << "Step: " << iteration++ << endl;
        cout << "doneCount: " << doneCount << endl;
    }
    else
    {
        if (width * height / 100 > 0)
            if (doneCount % (width * height / 100) == 0)
                cout << "\33[2K\r" << (doneCount + 0.0) / (width * height) * 100 << "%";
    }

    Tile *child = frontier.top();

    int x = child->getX();
    int y = child->getY();
    if (showDebug)
    {
        if (parent)
            cout << "\tCurrent Parent: " << *parent << endl;
        cout << "\tCurrent Child: " << *child << endl;
        cout << "Before: Printing a 2D Array:\n";
        displayStages(x, y);
    }

    // valid neighbors
    auto n = neighbors(*child, true);
    int validNeighborsLength = n.size();
    if (showDebug)
        cout << "Valid Neighbors Count: " << validNeighborsLength << endl;

    if (validNeighborsLength != 0)
    {
        int index = rand() % validNeighborsLength;
        for (int i = 0; i < index; i++)
        {
            n.pop_front();
        }
        Tile *next = n.front();

        // add to frontier
        frontier.push(next);
        if (showDebug)
            cout << "Pushed: " << *frontier.top() << endl;
        next->data = 1;

        if (child->data != 2)
        {
            child->data = 2;
            doneCount++;

            if (parent && parent != child)
            {
                parent->addChild(child);
            }
        }

        parent = child;
    }
    else
    {
        if (child->data != 2)
        {
            parent->addChild(child);
            doneCount++;
            child->data = 2;
        }

        // back up
        if (showDebug)
            cout << "Popped: " << *frontier.top() << endl;
        frontier.pop();
        parent = frontier.top();
    }

    // if (iteration > MAX_ITERATIONS) return false;
    if (doneCount == width * height)
        return false;
    return true;
}

void Grid::doWalls(Tile *tile, bool recurse)
{
    Tile *child = tile->firstChild();

    while (child)
    {
        tile->open(child);
        // cout << *tile << "->" << *child << endl;
        doWalls(child);
        child = child->nextSibling();
    }
}

void Grid::display()
{
    // Using 3 x 3 pixel cells;
    int mapWidth = width * 3;
    int mapHeight = height * 3;

    // allocate array

    // 2d Array of int
    int **pixelMap = new int *[mapWidth];
    for (int i = 0; i < mapWidth; i++)
    {
        pixelMap[i] = new int[mapHeight];
        for (int j = 0; j < mapHeight; j++)
        {
            int x = i / 3;
            int y = j / 3;
            pixelMap[i][j] = 1;
            // gets the corners of a 3 x 3 cell
            if (
                (j % 3 == 0 || j % 3 == 2) &&
                (i % 3 == 0 || i % 3 == 2))
            {
                continue;
            }

            if ((i % 3 == 1) && (j % 3 == 2))
            {
                // North Wall
                if (matrix[x][y].northWall == false)
                    pixelMap[i][j] = 0;
            }

            if ((i % 3 == 0) && (j % 3 == 1))
            {
                // East Wall
                if (matrix[x][y].eastWall == false)
                    pixelMap[i][j] = 0;
            }

            if ((i % 3 == 1) && (j % 3 == 0))
            {
                // South Wall
                if (matrix[x][y].southWall == false)
                    pixelMap[i][j] = 0;
            }

            if ((i % 3 == 2) && (j % 3 == 1))
            {
                // West Wall
                if (matrix[x][y].westWall == false)
                    pixelMap[i][j] = 0;
            }

            if (j % 3 == 1 && i % 3 == 1)
            {
                pixelMap[i][j] = 0;
                if (&matrix[x][y] == root)
                {
                    pixelMap[i][j] = 6;
                }
            }
        }
    }

    for (int j = 0; j < mapHeight; j++)
    {
        for (int i = 0; i < mapWidth; i++)
        {
            cout << getDisplayChar(pixelMap[i][j]);
        }
        cout << endl;
    }

    // Run through the map and check for errors
    for (int j = 0; j < mapHeight; j++)
    {
        for (int i = 0; i < mapWidth; i++)
        {
            if (getDisplayChar(pixelMap[i][j]) == '!')
            {
                cout << "! " << i << "," << j << ": " << pixelMap[i][j] << endl;
                cout << "  " << i << "," << j << ": " << &pixelMap[i][j] << endl;
                cout << "  " << i << "," << j << ": " << sizeof(pixelMap[i][j]) << endl;
            }
        }
    }

    for (int i = 0; i < width; i++)
    {
        delete[] pixelMap[i];
    }
    delete[] pixelMap;
}

std::unique_ptr<BMP> Grid::getBitMap()
{
    // Using 3 x 3 pixel cells;
    unsigned int mapWidth = width * 3;
    unsigned int mapHeight = height * 3;
    unique_ptr<BMP> map = make_unique<BMP>(mapWidth, mapHeight, 1);
    map->addColor(255, 255, 255);
    map->addColor(0, 0, 0);

    for (unsigned int i = 0; i < mapWidth; i++)
    {
        for (unsigned int j = 0; j < mapHeight; j++)
        {
            map->data[i][j] = 1;
            // gets the corners of a 3 x 3 cell
            if (
                (j % 3 == 0 || j % 3 == 2) &&
                (i % 3 == 0 || i % 3 == 2))
            {
                // continue;
            }

            unsigned int x = i / 3;
            unsigned int y = j / 3;

            if ((i % 3 == 1) && (j % 3 == 2))
            {
                // North Wall
                map->data[i][j] = matrix[x][y].northWall;
            }

            if ((i % 3 == 0) && (j % 3 == 1))
            {
                // East Wall
                map->data[i][j] = matrix[x][y].eastWall;
            }

            if ((i % 3 == 1) && (j % 3 == 0))
            {
                // South Wall
                map->data[i][j] = matrix[x][y].southWall;
            }

            if ((i % 3 == 2) && (j % 3 == 1))
            {
                // West Wall
                map->data[i][j] = matrix[x][y].westWall;
            }

            if (j % 3 == 1 && i % 3 == 1)
            {
                map->data[i][j] = 0;
            }
        }
    }

    return map;
}

void Grid::displayStages()
{
    for (int j = 0; j < height; j++)
    {
        cout << "Row:";
        for (int i = 0; i < width; i++)
        {
            cout << "|" << matrix[i][j].data << "|";
        }
        cout << endl;
    }
}

void Grid::displayStages(int x, int y)
{
    for (int j = 0; j < height; j++)
    {
        cout << "Row:";
        for (int i = 0; i < width; i++)
        {
            cout << "|";
            if (parent)
            {
                if (i == parent->getX() && j == parent->getY())
                {
                    cout << "P";
                    cout << "|";
                    continue;
                }
            }
            if (i == x && j == y)
            {
                cout << "C";
                cout << "|";
                continue;
            }
            if (matrix[i][j].data == 2)
            {
                cout << "*";
                cout << "|";
                continue;
            }
            if (matrix[i][j].data == 1)
            {
                cout << "%";
                cout << "|";
                continue;
            }
            if (matrix[i][j].data == 0)
            {
                cout << " ";
                cout << "|";
                continue;
            }
            cout << "!";
            cout << "|";
        }
        cout << endl;
    }
}

char Grid::getDisplayChar(int value)
{
    switch (value)
    {
    case 0:
        return '.';
    case 1:
        // Full Block
        return '\xDB';
    case 2:
        // Top Half Block
        return '\xDF';
    case 3:
        // Left Half Block
        return '\xDD';
    case 4:
        // Bottom Half Block
        return '\xDC';
    case 5:
        // Right Half Block
        return '\xDE';
    case 6:
        // Right Half Block
        return '*';
    default:
        return '!';
    }

    return '!';
}

std::list<Tile *> Grid::neighbors(Tile &tile, bool useStages)
{
    // Send to actual function
    return neighbors(tile.getX(), tile.getY(), useStages);
}

std::list<Tile *> Grid::neighbors(int x, int y, bool useStages)
{
    std::list<Tile *> n;

    // NESW
    Tile **tiles = new Tile *[4];

    // cout << "check: Is this a valid x and y? " << x << " " << y << endl;

    tiles[0] = &matrix[x][y + 1];
    // cout << "check: Is this a valid y? " << (y + 1)  << endl;
    if (y + 1 >= height)
        tiles[0] = nullptr;

    tiles[1] = &matrix[x - 1][y];
    // cout << "check: Is this a valid x? " << (x - 1)  << endl;
    if (x <= 0)
        tiles[1] = nullptr;

    tiles[2] = &matrix[x][y - 1];
    // cout << "check: Is this a valid y? " << (y - 1)  << endl;
    if (y <= 0)
        tiles[2] = nullptr;

    tiles[3] = &matrix[x + 1][y];
    // cout << "check: Is this a valid x? " << (x + 1) << endl;
    if (x + 1 >= width)
        tiles[3] = nullptr;

    for (int i = 0; i < 4; i++)
    {
        if (tiles[i] == nullptr)
            continue;
        if (tiles[i]->data == 2 && useStages)
            continue;

        n.push_front(tiles[i]);
    }
    delete[] tiles;
    return n;
}

std::ostream &operator<<(std::ostream &out, Grid &grid)
{
    for (int i = 0; i < grid.width; i++)
    {
        for (int j = 0; j < grid.height; j++)
        {
            cout << grid.matrix[i][j];
        }
    }
    return out;
}