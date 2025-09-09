add_library(
    Grid
    tile.cpp
    grid.cpp
)

target_include_directories(Grid PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_link_libraries(Grid PRIVATE bitmap)