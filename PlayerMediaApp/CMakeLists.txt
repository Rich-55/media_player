# Tên project và phiên bản
cmake_minimum_required(VERSION 3.10)
project(PlayerMediaApp VERSION 1.0 LANGUAGES CXX)

# Cấu hình phiên bản C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Bật các cảnh báo và tùy chọn biên dịch
add_compile_options(-Wall -Wextra -Werror)

# Thêm thư viện FTXUI (FetchContent tải về tự động)
include(FetchContent)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
  GIT_TAG main # Thay đổi tag này theo phiên bản bạn cần
)
FetchContent_MakeAvailable(ftxui)

# Thư mục include và source
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Tìm tất cả các tệp nguồn trong src
file(GLOB_RECURSE SRC_FILES
    ${SRC_DIR}/*.cpp
)

# Đặt thư mục đầu ra của tệp thực thi
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Tạo target chính
add_executable(${PROJECT_NAME} main.cpp ${SRC_FILES})

# Thêm đường dẫn include
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})

find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2main)

find_package(PkgConfig REQUIRED)
pkg_check_modules(AVFORMAT REQUIRED libavformat)
pkg_check_modules(AVCODEC REQUIRED libavcodec)
pkg_check_modules(AVUTIL REQUIRED libavutil)
pkg_check_modules(SWSCALE REQUIRED libswscale)
pkg_check_modules(SWRESAMPLE REQUIRED libswresample)

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

# Liên kết các thư viện cần thiết
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    tag # Thư viện libtag
    avformat
    avcodec
    avutil
    swscale
    swresample
    SDL2
    SDL2_mixer
    ftxui::screen # LiênS kết FTXUI
    ftxui::dom
    ftxui::component
)
