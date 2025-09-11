add_library(
    Grid
    tile.cpp
    node.cpp
    grid.cpp
)

target_compile_definitions(Grid PRIVATE
    # Add a DEBUG_BUILD definition only for the Debug configuration
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(Grid PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_link_libraries(Grid PRIVATE bitmap)