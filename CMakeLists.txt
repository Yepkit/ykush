cmake_minimum_required(VERSION 3.13)

project(ykushcmd
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# HIDAPI subtree
add_subdirectory(hidapi)

# Add src directory
add_subdirectory(src)

install(TARGETS ykushcmd
    RUNTIME DESTINATION bin
)

