add_executable(
    maze_test
    main.cpp
)

target_compile_features(maze_test PUBLIC cxx_std_17)

# target_include_directories(test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_link_libraries(maze_test PRIVATE bitmap)
target_link_libraries(maze_test PRIVATE Grid)
target_link_libraries(maze_test PRIVATE GTest::gtest GTest::gtest_main)

include(GoogleTest)

include(CTest)
gtest_discover_tests(maze_test)



# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)



add_test(
    NAME runUnitTests
    COMMAND maze_test
)

# Add a custom command to copy runtime DLLs after building the executable
if (WIN32) # Ensure this only runs on Windows where DLLs are relevant
    add_custom_command(
        TARGET maze_test
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:maze_test>
                $<TARGET_FILE_DIR:maze_test>
        COMMAND_EXPAND_LISTS # Important for expanding the list of DLLs
    )
endif()