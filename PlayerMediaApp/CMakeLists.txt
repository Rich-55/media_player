# Tên project và phiên bản
cmake_minimum_required(VERSION 3.10)
project(PlayerMediaApp VERSION 1.0 LANGUAGES CXX)

# Cấu hình phiên bản C++
set(CMAKE_CXX_STANDARD 17)
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

# Liên kết các thư viện cần thiết
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    tag # Thư viện libtag
    ftxui::screen # LiênS kết FTXUI
    ftxui::dom
    ftxui::component
)
