cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

# Avoid source tree pollution
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are discouraged. Make a separate folder for building:\nmkdir build; cd build; cmake ..;cd ..\nBefore that, remove the files already created:\nrm -rf CMakeCache.txt CMakeFiles")
endif()

if(GENERATOR_IS_MULTI_CONFIG)
    # Remove RelWithDebInfo and MinSizeRel configuration types (call this before project())
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
else()
    # Monoconfig generator
    set(REAPER_DEFAULT_BUILD_TYPE "Debug")
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "No build type provided, defaulting to: ${REAPER_DEFAULT_BUILD_TYPE}")
        set(CMAKE_BUILD_TYPE ${REAPER_DEFAULT_BUILD_TYPE} CACHE STRING "" FORCE)
    endif()
    # Set the valid options for cmake-gui drop-down list
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY HELPSTRING "Choose the type of build")
endif()

project(Raytracer
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "Simple Raytracer")

# Export compile commands in a json file. Useful for clang-based tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Override binary output paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Raytracer_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Raytracer_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Raytracer_BINARY_DIR})

add_subdirectory(src)
