# Project name and version
cmake_minimum_required(VERSION 3.10)
project(PlayerMediaApp VERSION 1.0 LANGUAGES CXX)

# Configure C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable warnings and compilation options
add_compile_options(-Wall -Wextra -Werror)

# Add FTXUI library (FetchContent will automatically download it)
include(FetchContent)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
  GIT_TAG main # newest
)
FetchContent_MakeAvailable(ftxui)

# Include and source directories
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Find all source files in the src directory
file(GLOB_RECURSE SRC_FILES
    ${SRC_DIR}/*.cpp
)
include_directories(
    /usr/include
    /usr/local/include
)
# Set the output directory for the executable file
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Create the main target
add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)

find_package(PkgConfig REQUIRED)
pkg_check_modules(AVFORMAT REQUIRED libavformat)
pkg_check_modules(AVCODEC REQUIRED libavcodec)
pkg_check_modules(AVUTIL REQUIRED libavutil)
pkg_check_modules(SWSCALE REQUIRED libswscale)
pkg_check_modules(SWRESAMPLE REQUIRED libswresample)

# Find all lib of FFMPEG
find_library(AVFORMAT_LIB avformat REQUIRED)
find_library(AVCODEC_LIB avcodec REQUIRED)
find_library(AVUTIL_LIB avutil REQUIRED)
find_library(SWSCALE_LIB swscale REQUIRED)
find_library(SWRESAMPLE_LIB swresample REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${AVFORMAT_INCLUDE_DIRS}
  ${AVCODEC_INCLUDE_DIRS}
  ${AVUTIL_INCLUDE_DIRS}
  ${SWSCALE_INCLUDE_DIRS}
  ${SWRESAMPLE_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  ${AVFORMAT_LIBRARIES}
  ${AVCODEC_LIBRARIES}
  ${AVUTIL_LIBRARIES}
  ${SWSCALE_LIBRARIES}
  ${SWRESAMPLE_LIBRARIES}
)

message(STATUS "AVFormat libraries: ${AVFORMAT_LIBRARIES}")
# Link necessary libraries
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    tag #Taglib
    avformat
    avcodec
    avutil
    swscale
    swresample
    SDL2
    SDL2_mixer
    ftxui::screen # FTXUI
    ftxui::dom
    ftxui::component
)
# Gtest
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

# Find all test files
file(GLOB_RECURSE TEST_FILES ${CMAKE_SOURCE_DIR}/test/*.cpp)

# Create a target for unit tests
add_executable(tests ${TEST_FILES})

# Link Google Test libraries
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main)

# Add include directories for tests
target_include_directories(tests PRIVATE ${INCLUDE_DIR})

# Register test cases
gtest_discover_tests(tests)