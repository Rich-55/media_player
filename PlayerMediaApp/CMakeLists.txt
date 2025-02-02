# Project name and version
cmake_minimum_required(VERSION 3.10)
project(PlayerMediaApp VERSION 1.0 LANGUAGES CXX)

# Configure C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

<<<<<<< HEAD
# Enable warnings and compilation options
add_compile_options(-Wall -Wextra -Werror)

# Add FTXUI library (FetchContent will automatically download it)
=======
# Enable warnings and compile options
add_compile_options(-Wall -Wextra -Werror)

# Add the FTXUI library (FetchContent downloads it automatically)
>>>>>>> refs/remotes/origin/test
include(FetchContent)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
<<<<<<< HEAD
  GIT_TAG main # newest
=======
  GIT_TAG main # Change this tag to the version you need
>>>>>>> refs/remotes/origin/test
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

<<<<<<< HEAD
# Add include directories
=======
# Add include paths
>>>>>>> refs/remotes/origin/test
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)

find_package(PkgConfig REQUIRED)
pkg_check_modules(AVFORMAT REQUIRED libavformat)
pkg_check_modules(AVCODEC REQUIRED libavcodec)
pkg_check_modules(AVUTIL REQUIRED libavutil)
pkg_check_modules(SWSCALE REQUIRED libswscale)
pkg_check_modules(SWRESAMPLE REQUIRED libswresample)

<<<<<<< HEAD
# Find all lib of FFMPEG
=======
# Find the FFmpeg libraries
>>>>>>> refs/remotes/origin/test
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
<<<<<<< HEAD
# Link necessary libraries
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    tag #Taglib
=======
# Link the necessary libraries
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    tag # libtag library
>>>>>>> refs/remotes/origin/test
    avformat
    avcodec
    avutil
    swscale
    swresample
    SDL2
    SDL2_mixer
<<<<<<< HEAD
    ftxui::screen # FTXUI
    ftxui::dom
    ftxui::component
)
# Gtest
=======
    ftxui::screen # Link FTXUI
    ftxui::dom
    ftxui::component
)
# Add Google Test
>>>>>>> refs/remotes/origin/test
enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

# Find all test files
file(GLOB_RECURSE TEST_FILES ${CMAKE_SOURCE_DIR}/test/*.cpp)

# Create a target for unit tests
add_executable(tests ${TEST_FILES})

<<<<<<< HEAD
# Link Google Test libraries
=======
# Link the Google Test library
>>>>>>> refs/remotes/origin/test
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main)

# Add include directories for tests
target_include_directories(tests PRIVATE ${INCLUDE_DIR})

<<<<<<< HEAD
# Register test cases
gtest_discover_tests(tests)
=======
# Register the tests
gtest_discover_tests(tests)
>>>>>>> refs/remotes/origin/test
