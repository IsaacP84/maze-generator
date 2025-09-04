add_library(
    Grid
    tile.cpp
    grid.cpp
)

target_include_directories(Grid PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_link_libraries(Grid PRIVATE bitmap)


add_executable(
    demo
    main.cpp
)

target_compile_features(demo PUBLIC cxx_std_17)

target_include_directories(demo PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_link_libraries(demo PRIVATE bitmap)
target_link_libraries(demo PRIVATE Grid)


# Add a custom command to copy runtime DLLs after building the executable
if (WIN32) # Ensure this only runs on Windows where DLLs are relevant
    add_custom_command(
        TARGET demo
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:demo>
                $<TARGET_FILE_DIR:demo>
        COMMAND_EXPAND_LISTS # Important for expanding the list of DLLs
    )
endif()