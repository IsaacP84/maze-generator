#include "debug.h"
#include "tile.h"
#include "grid.h"
#include "bitmap.h"

#include <time.h>
#include <iostream>
#include <filesystem>

using namespace std;
using maze_int = unsigned int;

struct GenerationParameters
{
    const maze_int width;
    const maze_int height;
    unsigned int seed;
    std::filesystem::path output_path;
    maze_int start_x;
    maze_int start_y;
};
GenerationParameters decipherArguments(int, char *[]);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Too little arguments. Needs a width and height." << endl;
        return 1;
    }

    GenerationParameters p = decipherArguments(argc, argv);

    cout << "Tile size: " << sizeof(Tile) << endl;
    cout << endl;

    // first arg is the cwd
    // 2nd arg is width
    // 3rd arg is height

    // printf("You have entered %d arguments:\n", argc);
    // for (int i = 0; i < argc; i++) {
    //     printf("%s\n", argv[i]);
    // }

    cout << "Hello world" << endl;
#ifdef DEBUG
    cout << "DEBUG ENABLED" << endl;
#endif
    cout << endl;

    // flag -r : define root node
    // x
    // y

    // flag -o
    // a file location

    // Set seed
    srand(p.seed);
    Grid *grid = new Grid(p.width, p.height);
    grid->generate(grid->matrix[p.start_x][p.start_y]);

    cout << "Getting Bitmap" << endl;
    auto map = grid->getBitMap();
    delete grid;

    map->toFile(p.output_path);
    map.release();

    cout << "All good" << endl;

    // cout << "\33[2K\r" << "All good";

    // cout << "\33[2K\r";
    // cout << "\033[A\r";
    return 0;
}

GenerationParameters decipherArguments(int argc, char *argv[])
{

    GenerationParameters p{
        // Width
        (unsigned int)atol(argv[1]),
        // Height
        (unsigned int)atol(argv[2])};

    // Default Seed
    p.seed = time(NULL);
    p.start_x = 0;
    p.start_y = 0;

    // File Path
    p.output_path = filesystem::current_path().append("assets");

    string fileName = "maze" + to_string(p.seed) + ".bmp";

    // Check for assets folder
    // If it doesn't exist, create it
    // Attempt to create the directory
    if (!std::filesystem::is_directory(p.output_path))
    {
        cout << "assets directory doesn't exist." << endl;
        if (std::filesystem::create_directory(p.output_path))
        {
            std::cout << "Directory created successfully." << std::endl;
        }
        else
        {
            throw runtime_error("Directory creation failed.");
        }
    }

    p.output_path = p.output_path.append(fileName);

    // Check if the file exists
    if (std::filesystem::exists(p.output_path))
    {
        std::cout << p.output_path << " already exists." << std::endl;
    }

    bool seedFlag = false;
    // bool rFlag = false;
    // bool oFlag = false;

    // Run through arguments
    for (int i = 3; i < argc; i++)
    {
        // Seed
        if ((string)argv[i] == "--seed")
        {
            seedFlag = true;
            char *a = argv[++i];
            p.seed = atol(a);
        }

        // Root
        if ((string)argv[i] == "-r")
        {
            maze_int x = (unsigned int)atol(argv[++i]);
            maze_int y = (unsigned int)atol(argv[++i]);
            // rFlag = true;

            p.start_x = x;
            p.start_y = y;
        }

        // Output
        if ((string)argv[i] == "-o")
        {
            // oFlag = true;
            std::filesystem::path path_to_check = (string)argv[++i];

            if (std::filesystem::is_directory(path_to_check))
            {
                std::cout << path_to_check << " is an invalid location." << std::endl;
                continue;
            }

            if (std::filesystem::exists(path_to_check))
            {
                std::cout << path_to_check << " already exists." << std::endl;
                continue;
            }

            p.output_path = path_to_check;
        }
    }

    if (!seedFlag)
        cout << "Seed: " << p.seed << endl;
    return p;
}