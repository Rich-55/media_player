#include "../../include/View/MediaFileManagerView.h"

MediaFileManagerView::MediaFileManagerView(){}

int MediaFileManagerView::showMenu() {
    std::cout << "========== Media File Menu ==========" << std::endl;
    std::cout << "1. Add File By File Path" << std::endl;
    std::cout << "2. Add File By Folder Path" << std::endl;
    std::cout << "3. Delete File" << std::endl;
    std::cout << "4. View All Media File" << std::endl;
    std::cout << "5. View All Audio Media File" << std::endl;
    std::cout << "6. View All Video Media File" << std::endl;
    std::cout << "0. Back to main menu" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Enter your choice: ";
    return 1;
}

std::string MediaFileManagerView::displayAllMediaFile(MediaFileManager MediaFileManager) {
    system("clear");
    
    auto media_files = MediaFileManager.getAllMediaFile();

    if (media_files.empty()) {
        auto screen = ScreenInteractive::TerminalOutput();
        auto empty_message = Renderer([&] {
            return vbox({
                text("No media files to display.") | bold | center,
            });
        });
        screen.Loop(empty_message);
        return "";
    }

    const int rows_per_page = 15;
    int current_page = 0;
    int total_pages = std::ceil((double)media_files.size() / rows_per_page);
    std::string error_message;
    std::string selected_filename;
    int selected_row = -1;  
    int hovered_row = -1;   
    std::string input_filename;
    bool is_exit_hovered = false;
    auto screen = ScreenInteractive::TerminalOutput();

    auto create_table_view = [&]() {
    std::vector<Element> table_rows;

    table_rows.push_back(
        hbox({
            text("STT") | bold | size(WIDTH, EQUAL, 5) | border,
            text("File Name") | bold | size(WIDTH, EQUAL, 20) | border,
            text("File Type") | bold | size(WIDTH, EQUAL, 10) | border,
            text("File Duration") | bold | size(WIDTH, EQUAL, 15) | border,
            text("File Size") | bold | size(WIDTH, EQUAL, 15) | border,
            text("File Path") | bold | size(WIDTH, EQUAL, 60) | border,
        })
    );

    int start_index = current_page * rows_per_page;
    int end_index = std::min(start_index + rows_per_page, (int)media_files.size());

    for (int i = start_index; i < end_index; ++i) {
        auto file = media_files[i];
        bool is_hovered = (i == hovered_row);
        bool is_selected = (i == selected_row);

        table_rows.push_back(
            hbox({
                text(std::to_string(i + 1)) | size(WIDTH, EQUAL, 5) | border,
                text(file->getName()) | size(WIDTH, EQUAL, 20) | border,
                text(file->getType()) | size(WIDTH, EQUAL, 10) | border,
                text(file->getDuration()) | size(WIDTH, EQUAL, 15) | border,
                text(std::to_string(file->getSize()) + " bytes") | size(WIDTH, EQUAL, 15) | border,
                text(file->getPath()) | size(WIDTH, EQUAL, 60) | border,
            }) | (is_hovered ? inverted : nothing) 
              | (is_selected ? bold : nothing)
        );
    }

    // Hiển thị nút Exit với phản hồi hover
    auto exit_button = text("Exit (Simulate 0)") | bold | center | border |
                       (is_exit_hovered ? color(Color::Red) : nothing);

    return vbox({
        text("----- All Media Files -----") | bold | center | color(Color::Blue),
        text("Page " + std::to_string(current_page + 1) + " of " + std::to_string(total_pages)) | center,
        separator(),
        vbox(std::move(table_rows)),
        separator(),
        exit_button | center,  // Hiển thị nút Exit
        separator(),
        text("Use UP/DOWN to navigate, LEFT/RIGHT to change pages, ENTER to select.") | dim | center,
        text("Hover over a row with mouse or click to select.") | dim | center,
        text("Enter filename below (or input 0 to exit):") | dim | center,
        text(input_filename) | border | center, 
        text("Selected File: " + selected_filename) | bold | center,
        text(error_message) | color(Color::Red) | center, 
    });
};

                             


    auto main_view = Renderer([&]() {
        return create_table_view();
    });

    main_view = CatchEvent(main_view, [&](Event event) {
    if (event.is_mouse()) {
        int row_height = 3;  // Chiều cao mỗi hàng
        int exit_y_position = rows_per_page + 1;  // Vị trí của nút Exit
        int y_offset = (event.mouse().y - 5) / row_height;  // Chia tọa độ y cho chiều cao mỗi hàng

        if (event.mouse().motion == Mouse::Moved) {
            // Kiểm tra nếu chuột hover vào bảng
            if (y_offset >= 0 && y_offset < rows_per_page) {
                hovered_row = y_offset + current_page * rows_per_page;
                is_exit_hovered = false;  // Không hover nút Exit
            }
            // Kiểm tra nếu chuột hover vào nút Exit
            else if (y_offset == exit_y_position) {
                hovered_row = -1;
                is_exit_hovered = true;  // Hover nút Exit
            } else {
                hovered_row = -1;
                is_exit_hovered = false;  // Không hover
            }
            return true;
        }

        // Xử lý click chuột trái
        if (event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            // Nếu click vào một hàng
            if (hovered_row >= 0 && hovered_row < (int)media_files.size()) {
                selected_row = hovered_row;
                selected_filename = media_files[selected_row]->getName();
                screen.ExitLoopClosure()();
            }
            // Nếu click vào nút Exit
            else if (is_exit_hovered) {
                selected_filename = "";
                screen.ExitLoopClosure()();  // Thoát chương trình
                return true;
            }
            return true;
        }
    }

    // Xử lý các phím Arrow
    if (event == Event::ArrowUp) {
        if (selected_row > current_page * rows_per_page) {
            --selected_row;  // Di chuyển lên hàng trước
            hovered_row = selected_row;
            error_message.clear();
        } else {
            error_message = "Already at the top of the page!";
        }
        return true;
    }

    if (event == Event::ArrowDown) {
        if (selected_row < std::min((current_page + 1) * rows_per_page - 1, (int)media_files.size() - 1)) {
            ++selected_row;  // Di chuyển xuống hàng tiếp theo
            hovered_row = selected_row;
            error_message.clear();
        } else {
            error_message = "Already at the bottom of the page!";
        }
        return true;
    }

    if (event == Event::ArrowLeft) {
        if (current_page > 0) {
            --current_page;  // Quay lại trang trước
            selected_row = current_page * rows_per_page;  // Cập nhật hàng được chọn
            hovered_row = selected_row;
            error_message.clear();
        } else {
            error_message = "Already on the first page!";
        }
        return true;
    }

    if (event == Event::ArrowRight) {
        if (current_page < total_pages - 1) {
            ++current_page;  // Chuyển sang trang tiếp theo
            selected_row = current_page * rows_per_page;  // Cập nhật hàng được chọn
            hovered_row = selected_row;
            error_message.clear();
        } else {
            error_message = "Already on the last page!";
        }
        return true;
    }

    // Giữ nguyên các sự kiện bàn phím khác
    if (event.is_character()) {
        char c = event.character()[0];
        if (std::isalnum(c) || c == '.' || c == '_' || c == '-' || c == ' ' || c == '(' || c == ')') {
            input_filename += c;
        }
        return true;
    }

    if (event == Event::Backspace && !input_filename.empty()) {
        input_filename.pop_back();
        return true;
    }

    if (event == Event::Return) {
        if (!input_filename.empty()) {
            if (input_filename == "0") {
                selected_filename = "";
                screen.ExitLoopClosure()();
                return true;
            } else if (MediaFileManager.checkFileExists(input_filename)) {
                selected_filename = input_filename;
                screen.ExitLoopClosure()();
                return true;
            } else {
                error_message = "File not found!";
                input_filename.clear();  // Xóa dữ liệu nhập khi file không tồn tại
            }
        } else if (selected_row >= 0 && selected_row < (int)media_files.size()) {
            selected_filename = media_files[selected_row]->getName();
            screen.ExitLoopClosure()();
            return true;
        } else {
            error_message = "No file selected!";
        }
        return true;
    }

    return false;
});


    screen.Loop(main_view);
    return selected_filename;
}



void MediaFileManagerView::displayAllMediaFileOfAudio(MediaFileManager MediaFileManager) {
    auto audioFiles = MediaFileManager.getAllAudioFiles();
    if (audioFiles.empty()) {
        std::cerr << "No audio files to display.\n";
        return;
    }

    std::cout << "----- All Audio Files -----" << std::endl;
    for (const auto& file : audioFiles) {
        std::cout << "File Name:   " << file->getName() << std::endl;
        std::cout << "File Path:   " << file->getPath() << std::endl;
        std::cout << "File Type:   " << file->getType() << std::endl;
        std::cout << "File Size:   " << file->getSize() << " bytes" << std::endl;

        std::cout << "Audio Metadata:" << std::endl;
        for (const auto& [key, value] : file->getAllMetadata()) {
            std::cout << "  " << key << ": " << value << std::endl;
        }

        std::cout << std::string(50, '-') << std::endl;
    }
}

void MediaFileManagerView::displayAllMediaFileOfVideo(MediaFileManager MediaFileManager) {
    auto videoFiles = MediaFileManager.getAllVideoFiles();
    if (videoFiles.empty()) {
        std::cerr << "No video files to display.\n";
        return;
    }

    std::cout << "----- All Video Files -----" << std::endl;
    for (const auto& file : videoFiles) {
        std::cout << "File Name:   " << file->getName() << std::endl;
        std::cout << "File Path:   " << file->getPath() << std::endl;
        std::cout << "File Type:   " << file->getType() << std::endl;
        std::cout << "File Size:   " << file->getSize() << " bytes" << std::endl;

        std::cout << "Video Metadata:" << std::endl;
        for (const auto& [key, value] : file->getAllMetadata()) {
            std::cout << "  " << key << ": " << value << std::endl;
        }

        std::cout << std::string(50, '-') << std::endl;
    }
}

