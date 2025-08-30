add_executable(
    maze
    tile.cpp
    grid.cpp
    main.cpp
)

target_compile_features(maze PUBLIC cxx_std_17)

target_include_directories(maze PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_link_libraries(maze PRIVATE bitmap)

# Add a custom command to copy runtime DLLs after building the executable
if (WIN32) # Ensure this only runs on Windows where DLLs are relevant
    add_custom_command(
        TARGET maze
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:maze>
                $<TARGET_FILE_DIR:maze>
        COMMAND_EXPAND_LISTS # Important for expanding the list of DLLs
    )
endif()