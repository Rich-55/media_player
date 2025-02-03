cmake_minimum_required(VERSION 3.10)
project(PlayerMediaApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror)

# ===============================
# FTXUI Setup
# ===============================

include(FetchContent)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
  GIT_TAG main
)
FetchContent_MakeAvailable(ftxui)

# Đảm bảo thư viện có sẵn
if (NOT TARGET ftxui::screen OR NOT TARGET ftxui::dom OR NOT TARGET ftxui::component)
    message(FATAL_ERROR "FTXUI library was not properly built or linked!")
endif()

set(CMAKE_INSTALL_RPATH "${CMAKE_BINARY_DIR}/_deps/ftxui-build")
link_directories(${CMAKE_BINARY_DIR}/_deps/ftxui-build)

# ===============================
# Include & Source
# ===============================

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp)

add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR} /usr/include /usr/local/include)

# ===============================
# SDL2 Setup
# ===============================

set(CMAKE_PREFIX_PATH "/usr/local/lib/cmake")
find_package(SDL2 REQUIRED)

# ===============================
# FFmpeg Setup
# ===============================

find_library(AVFORMAT_LIB avformat PATHS /usr/lib/x86_64-linux-gnu/ REQUIRED)
find_library(AVCODEC_LIB avcodec PATHS /usr/lib/x86_64-linux-gnu/ REQUIRED)
find_library(AVUTIL_LIB avutil PATHS /usr/lib/x86_64-linux-gnu/ REQUIRED)
find_library(SWSCALE_LIB swscale PATHS /usr/lib/x86_64-linux-gnu/ REQUIRED)
find_library(SWRESAMPLE_LIB swresample PATHS /usr/lib/x86_64-linux-gnu/ REQUIRED)

# ===============================
# Link Libraries
# ===============================

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    ftxui::screen
    ftxui::dom
    ftxui::component
    avformat
    avcodec
    avutil
    swscale
    swresample
    tag
    SDL2_mixer
)

# ===============================
# GTest & GMock Setup
# ===============================

enable_testing()
include(FetchContent)

# Fetch GoogleTest & GoogleMock
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

# Ensure GTest & GMock exist
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

# Find all test files
file(GLOB_RECURSE TEST_FILES ${CMAKE_SOURCE_DIR}/test/*.cpp)

# Create unit_tests target
add_executable(unit_tests ${TEST_FILES} ${SRC_FILES})

# Include directories for tests
target_include_directories(unit_tests PRIVATE ${INCLUDE_DIR} ${SRC_DIR})

# Link GTest & GMock
target_link_libraries(unit_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
    SDL2::SDL2
    SDL2::SDL2main
    ftxui::screen
    ftxui::dom
    ftxui::component
    avformat
    avcodec
    avutil
    swscale
    swresample
    tag
    SDL2_mixer
)

# Auto-discover tests
gtest_discover_tests(unit_tests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/..)
