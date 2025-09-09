


add_executable(
    app
    main.cpp
)

target_compile_features(app PUBLIC cxx_std_17)
target_compile_definitions(app PRIVATE
    # Add a DEBUG_BUILD definition only for the Debug configuration
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(app PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_link_libraries(app PRIVATE bitmap)
target_link_libraries(app PRIVATE Grid)


# Add a custom command to copy runtime DLLs after building the executable
if (WIN32) # Ensure this only runs on Windows where DLLs are relevant
    add_custom_command(
        TARGET app
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:app>
                $<TARGET_FILE_DIR:app>
        COMMAND_EXPAND_LISTS # Important for expanding the list of DLLs
    )
endif()