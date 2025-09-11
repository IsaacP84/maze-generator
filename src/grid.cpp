#include "grid.h"
#include "node.h"
#include "cell.h"
#include "debug.h"

#include <array>
#include <stack>
#include <list>
#include <iostream>
#include <memory>

using namespace std;

Grid::Grid(uint32_t x, uint32_t y)
    : width(x), height(y)
{
    matrix = new Tile *[width];
    for (uint32_t i = 0; i < width; i++)
    {
        matrix[i] = new Tile[height];
        for (uint32_t j = 0; j < height; j++)
        {
            matrix[i][j] = Tile(i, j);
        }
    }

    doneCount = 0;
}

Grid::~Grid()
{
    // deallocate
    for (uint32_t i = 0; i < width; i++)
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
    // cout << "\33[2K\r"
    //      << "Beginning Generation";

    const uint32_t length = width * height;

    // whenever the vector expands it reallocates the nodes
    // this means it moves to a new pointer invalidating all other pointers
    // use shared pointers
    Node *graph = new Node[length];
    for (uint32_t i = 0; i < length; i++)
    {
        graph[i].tile = &matrix[i / width][i % width];
    }

    // i can convert the x and y into an index
    // x * width + y

    // keep the index of the node
    std::stack<int> frontier;
    int current_index = root.getX() * width + root.getY();

    // map tiles to nodes
    // add to frontier
    graph[current_index].tile = &root;
    graph[current_index].visited = true;

    // Node rootNode;
    // rootNode
    // this->root = &root;
    // root.data = 1;
    frontier.push(current_index);

    // Set parent
    // cout << "\33[2K\r"
    //      << "Generating" << endl;

    srand(0);

    bool running = true;
    while (running)
    {
        current_index = frontier.top();

        // graph[current_index].visited = true;
        LOG("Starting: " << *graph[current_index].tile)
        // valid neighbors
        auto n = neighbors(*graph[current_index].tile);
        int validNeighborsLength = 0;
        LOG("Valid Neighbors")
        // making a new block for clarity
        {
            auto it = n.begin();
            while (it != n.end())
            {
                Tile *tp = *it;
                ASSERT(it == n.end());
                uint32_t index = (*it)->getX() * width + (*it)->getY();
                if (graph[index].visited)
                {
                    n.erase(it++);

                    continue;
                }
                LOG(**it)
                validNeighborsLength++;
                ++it;
            }
        }

        if (validNeighborsLength != 0)
        {
            int index = rand() % validNeighborsLength;

            auto it = n.begin();
            advance(it, index);

            // for (auto it = n.begin(); it != n.end(); ++it)
            // {

            //     uint32_t index = (*it)->getX() * width + (*it)->getY();
            //     cout << *graph[index].tile << endl;

            // }

            Tile *next = *it;
            uint32_t next_index = next->getX() * width + next->getY();

            Node &nextNode = graph[next_index];
            ASSERT(!graph[next_index].visited);
            nextNode.tile = next;
            nextNode.visited = true;

            LOG("Chose: " << *nextNode.tile)
            // Node  currentNode = graph[current_index];
            graph[current_index].addChild(&nextNode);

            // add to frontier
            frontier.push(next_index);
        }
        else
        {
            // back up
            frontier.pop();
        }

        // doneCount++;
        // completed[completed_index] = graph[node_index].tile;
        // cout << "completed: " << completed[completed_index] << endl;

        if (frontier.size() == 0)
        {
            running = false;
            break;
        }

        // if (iteration > MAX_ITERATIONS) return false;
        // if (doneCount == width * height)
        // {
        //     running = false;
        //     break;
        // }
    }

    delete[] graph;

    cout << "\33[2K\r"
         << "\r\033[A";
    cout << "Finished Generation";
}

// bool Grid::step()
// {
// #ifdef DEBUG
//     cout << "Step: " << iteration++ << endl;
//     cout << "doneCount: " << doneCount << endl;
// #else
//     if (width * height / 100 > 0)
//         if (doneCount % (width * height / 100) == 0)
//             cout << "\33[2K\r" << (doneCount + 0.0) / (width * height) * 100 << "%";
// #endif

//     Tile *child = frontier.top();

//     int x = child->getX();
//     int y = child->getY();

// #ifdef DEBUG
//     if (parent)
//         cout << "\tCurrent Parent: " << *parent << endl;
//     cout << "\tCurrent Child: " << *child << endl;
//     cout << "Before: Printing a 2D Array:\n";
//     displayStages(x, y);

// #endif

//     // valid neighbors
//     auto n = neighbors(*child, true);
//     int validNeighborsLength = n.size();
// #ifdef DEBUG
//     cout << "Valid Neighbors Count: " << validNeighborsLength << endl;
// #endif

//     if (validNeighborsLength != 0)
//     {
//         int index = rand() % validNeighborsLength;
//         for (int i = 0; i < index; i++)
//             n.pop_front();

//         Tile *next = n.front();

//         // add to frontier
//         frontier.push(next);
// #ifdef DEBUG
//         cout << "Pushed: " << *frontier.top() << endl;
// #endif
//         next->data = 1;

//         if (child->data != 2)
//         {
//             child->data = 2;
//             doneCount++;

//             if (parent && parent != child)
//             {
//                 parent->addChild(child);
//             }
//         }

//         parent = child;
//     }
//     else
//     {
//         if (child->data != 2)
//         {
//             parent->addChild(child);
//             doneCount++;
//             child->data = 2;
//         }

//         LOG("Popped: " << *frontier.top());
//         // back up
//         frontier.pop();
//         parent = frontier.top();
//     }

//     // if (iteration > MAX_ITERATIONS) return false;
//     if (doneCount == width * height)
//         return false;

//     // if nothing in stack
//     // if(frontier.size() == 0)
//     //     return false;
//     return true;
// }

#ifdef DEBUG
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
#endif

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

#ifdef DEBUG
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

#endif

std::list<Tile *> Grid::neighbors(Tile &tile)
{
    // Send to actual function
    return neighbors(tile.getX(), tile.getY());
}

std::list<Tile *> Grid::neighbors(uint32_t x, uint32_t y)
{
    std::list<Tile *> n;

    // NESW
    Tile *tiles[4] = {nullptr, nullptr, nullptr, nullptr};

    // North
    if (y + 1 < height && y < y + 1)
        tiles[0] = &matrix[x][y + 1];

    if (x > x - 1)
        tiles[1] = &matrix[x - 1][y];

    if (y > y - 1)
        tiles[2] = &matrix[x][y - 1];

    if (x + 1 < width && x < x + 1)
        tiles[3] = &matrix[x + 1][y];

    for (int i = 0; i < 4; i++)
    {
        if (tiles[i] == nullptr)
            continue;

        n.push_front(tiles[i]);
    }
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
