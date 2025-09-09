cmake_minimum_required(VERSION 3.30.1)
project(maze-generator
    VERSION 1.0.1
    LANGUAGES CXX
)


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")

if(NOT TARGET bitmap)
    add_subdirectory(lib/bitmap)
endif()

# The executable code is here
add_subdirectory(src)

# The executable app is here
add_subdirectory(app)



if(CMAKE_BUILD_TYPE STREQUAL Release)
    # Other stuff potentially
else()
    # Testing only available if this is the main app
    # Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
    if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
        AND BUILD_TESTING)
        enable_testing()

        include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        release-1.12.1 # Or a specific commit hash
        )

        
        FetchContent_MakeAvailable(googletest)
        add_subdirectory(test)
    endif()

endif()
