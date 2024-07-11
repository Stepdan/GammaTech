cmake_minimum_required(VERSION 3.25)

set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/conan_provider.cmake")

project(gammatech LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF) # the flag used to set the language standard (e.g. -std=c++11 rather than -std=gnu++11).
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/fb_schemas)

set(CMAKE_BUILD_TYPE "Debug")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

set(GAMMA_LIB_DIR "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(GAMMA_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/bin")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GAMMA_BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${GAMMA_LIB_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GAMMA_BIN_DIR})

set(CONAN_PROFILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/conan/profile_${CMAKE_BUILD_TYPE}")

unset(CONAN_HOST_PROFILE CACHE)
unset(CONAN_BUILD_PROFILE CACHE)
set(CONAN_HOST_PROFILE "${CONAN_PROFILE_PATH}" CACHE STRING "Conan host profile from NNToolKit")
set(CONAN_BUILD_PROFILE "${CONAN_PROFILE_PATH}" CACHE STRING "Conan build profile from NNToolKit")

set(CONAN_BIN_DIR "${CMAKE_BINARY_DIR}/conan/build/${CMAKE_BUILD_TYPE}/bin")

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(flatbuffers REQUIRED)
find_package(Qt6 REQUIRED)

include(windeployqt)

set(GAMMA_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/resources)

# Flatbuffers API generation
include(flatbuffers_helper)

include_directories(src)

add_subdirectory(src)