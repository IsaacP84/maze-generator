#include "cell.h"
#include "grid.h"

#include <chrono>




// Function to perform Randomized DFS for maze generation
void randomizedDFS(std::vector<std::vector<Cell>> &maze, int start_row, int start_col)
{
    int rows = maze.size();
    int cols = maze[0].size();

    std::stack<Cell *> s;
    Cell *current_cell = &maze[start_row][start_col];
    current_cell->visited = true;
    s.push(current_cell);

    // Seed the random number generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);

    while (!s.empty())
    {
        current_cell = s.top();

        // Get unvisited neighbors
        std::vector<Cell *> unvisited_neighbors;
        if (current_cell->row > 0 && !maze[current_cell->row - 1][current_cell->col].visited)
        {
            unvisited_neighbors.push_back(&maze[current_cell->row - 1][current_cell->col]);
        }
        if (current_cell->row < rows - 1 && !maze[current_cell->row + 1][current_cell->col].visited)
        {
            unvisited_neighbors.push_back(&maze[current_cell->row + 1][current_cell->col]);
        }
        if (current_cell->col > 0 && !maze[current_cell->row][current_cell->col - 1].visited)
        {
            unvisited_neighbors.push_back(&maze[current_cell->row][current_cell->col - 1]);
        }
        if (current_cell->col < cols - 1 && !maze[current_cell->row][current_cell->col + 1].visited)
        {
            unvisited_neighbors.push_back(&maze[current_cell->row][current_cell->col + 1]);
        }

        if (!unvisited_neighbors.empty())
        {
            // Randomly select one unvisited neighbor
            std::uniform_int_distribution<int> distribution(0, unvisited_neighbors.size() - 1);
            Cell *next_cell = unvisited_neighbors[distribution(generator)];

            // Remove wall between current_cell and next_cell
            if (next_cell->row < current_cell->row)
            { // Moving UP
                current_cell->wall_up = false;
                next_cell->wall_down = false;
            }
            else if (next_cell->row > current_cell->row)
            { // Moving DOWN
                current_cell->wall_down = false;
                next_cell->wall_up = false;
            }
            else if (next_cell->col < current_cell->col)
            { // Moving LEFT
                current_cell->wall_left = false;
                next_cell->wall_right = false;
            }
            else
            { // Moving RIGHT
                current_cell->wall_right = false;
                next_cell->wall_left = false;
            }

            next_cell->visited = true;
            s.push(next_cell);
        }
        else
        {
            s.pop(); // Backtrack
        }
    }
}
