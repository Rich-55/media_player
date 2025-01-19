#include "../../include/View/MediaFileManagerView.h"

MediaFileManagerView::MediaFileManagerView(){}

int MediaFileManagerView::showMenu() 
{
    return -1; 
}

int MediaFileManagerView::showMenuWithMediaList(MediaFileManager mediaFileManager) {
    
    std::vector<std::string> menu_entries = {
        "1. Add File By File Path",
        "2. Add File By Folder Path",
        "3. Delete File",
        "4. View All Media File",
        "5. View All Audio Media File",
        "6. View All Video Media File",
        "0. Back to main menu"
    };

    std::vector<int> logic_mapping = {1, 2, 3, 4, 5, 6, 0};// Liên kết mục menu với logic tương ứng
    int selected = 0; // Vị trí được chọn ban đầu
    std::string error_message; // Lưu thông báo lỗi nếu có
    int final_selected = -1; // Kết quả trả về cuối cùng

    // Phân trang danh sách media
    const int rows_per_page = 25; // Số lượng media mỗi trang
    const int scroll_visible_rows = 10; // Số hàng hiển thị khi cuộn
    int current_page = 0;         // Trang hiện tại
    int total_pages = std::ceil((double)mediaFileManager.getAllMediaFile().size() / rows_per_page);
    int scroll_offset = 0; // Offset khi cuộn trong trang

    auto create_table_view = [&]() {
        std::vector<ftxui::Element> rows;

        // Kiểm tra nếu playlist rỗng
        if (mediaFileManager.getAllMediaFile().empty()) {
            rows.push_back(ftxui::text("No media files in the playlist.") | ftxui::bold | ftxui::center);
        } else {
            // Tiêu đề bảng
            rows.push_back(ftxui::hbox({
                    text("STT") | bold | size(WIDTH, EQUAL, 5) | border,
                    text("File Name") | bold | size(WIDTH, EQUAL, 30) | border,
                    text("File Type") | bold | size(WIDTH, EQUAL, 20) | border,
                    text("Duration") | bold | size(WIDTH, EQUAL, 15) | border,
                    text("Date Created") | bold | size(WIDTH, EQUAL, 20) | border,
                    text("File Size") | bold | size(WIDTH, EQUAL, 20) | border,
                })
            );

            // Lấy danh sách media files trên trang hiện tại
            int start_index = current_page * rows_per_page;
            int end_index = std::min(start_index + rows_per_page, (int)mediaFileManager.getAllMediaFile().size());

            for (int i = start_index + scroll_offset;
                 i < std::min(start_index + scroll_offset + scroll_visible_rows, end_index);
                 ++i) {
                auto mediaFile = mediaFileManager.getAllMediaFile()[i];
                rows.push_back(ftxui::hbox({
                        text(std::to_string(i + 1)) | size(WIDTH, EQUAL, 5) | border,
                        text(mediaFile->getName()) | size(WIDTH, EQUAL, 30) | border,
                        text(mediaFile->getType()) | size(WIDTH, EQUAL, 20) | border,
                        text(mediaFile->getDuration()) | size(WIDTH, EQUAL, 15) | border,
                        text(mediaFile->getDateCreated()) | size(WIDTH, EQUAL, 20) | border,
                        text(std::to_string(mediaFile->getSize()) + " bytes") | size(WIDTH, EQUAL, 20) | border,
                    }) 
                );
            }
        }

        return ftxui::vbox({
            ftxui::text("----- All Media File -----") | ftxui::bold | ftxui::center | ftxui::color(ftxui::Color::Blue),
            ftxui::text("Page " + std::to_string(current_page + 1) + " of " + std::to_string(total_pages)) | ftxui::center,
            ftxui::separator(),
            ftxui::vbox(std::move(rows)) | ftxui::vscroll_indicator | ftxui::frame | ftxui::border,
        });
    };

    auto menu = Menu(&menu_entries, &selected);
    auto main_component = Renderer(menu, [&]() {
        return ftxui::vbox({
            ftxui::text("============ Media File Manager ===========") | ftxui::bold | ftxui::center,
            ftxui::separator(),
            menu->Render() | ftxui::border,
            ftxui::separator(),
            create_table_view(),
            ftxui::separator(),
            ftxui::text("Use UP/DOWN keys to navigate menu. Use LEFT/RIGHT keys to navigate pages.") | ftxui::dim | ftxui::center,
            ftxui::text(error_message) | ftxui::color(ftxui::Color::Red) | ftxui::center
        });
    });

    auto screen = ScreenInteractive::TerminalOutput();
    main_component = CatchEvent(main_component, [&](Event event) {
        // Xử lý chọn menu chính bằng phím Enter
        if (event == Event::Return) {
            final_selected = logic_mapping[selected];
            screen.ExitLoopClosure()();
            return true;
        }

        // Xử lý nhập số từ bàn phím để chọn menu
        if (event.is_character() && std::isdigit(event.character()[0])) {
            int number = event.character()[0] - '0';
            auto it = std::find(logic_mapping.begin(), logic_mapping.end(), number);
            if (it != logic_mapping.end()) {
                final_selected = number;
                screen.ExitLoopClosure()();
                return true;
            } else {
                error_message = "Invalid input: number not in menu!";
                return true;
            }
        }

        // Xử lý điều hướng menu bằng phím mũi tên lên/xuống
        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            menu->OnEvent(event);
            return true;
        }

        // Xử lý click chuột vào menu chính
        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3; // Căn chỉnh tọa độ Y (thay đổi nếu cần)
            if (clicked_index >= 0 && clicked_index < (int)menu_entries.size()) {
                selected = clicked_index; // Cập nhật vị trí được chọn
                final_selected = logic_mapping[selected]; // Trả về logic tương ứng
                screen.ExitLoopClosure()();
                return true;
            } else {
                error_message = "Invalid click: out of menu range!";
                return true;
            }
        }

        // Xử lý chuyển trang danh sách media
        if (event == Event::ArrowLeft) {
            if (current_page > 0) {
                --current_page;
                scroll_offset = 0; // Reset offset khi chuyển trang
                error_message.clear();
            } else {
                error_message = "Already on the first page!";
            }
            return true;
        }

        if (event == Event::ArrowRight) {
            if (current_page < total_pages - 1) {
                ++current_page;
                scroll_offset = 0; // Reset offset khi chuyển trang
                error_message.clear();
            } else {
                error_message = "Already on the last page!";
            }
            return true;
        }

        // Xử lý cuộn danh sách media
        if (event.mouse().button == Mouse::WheelUp) {
            if (scroll_offset > 0) {
                --scroll_offset;
            }
            return true;
        }

        if (event.mouse().button == Mouse::WheelDown) {
            int max_scroll = std::min(rows_per_page, (int)mediaFileManager.getAllMediaFile().size() - current_page * rows_per_page);
            if (scroll_offset + scroll_visible_rows < max_scroll) {
                ++scroll_offset;
            }
            return true;
        }

        return false;
    });

    screen.Loop(main_component);
    return final_selected;
}

void MediaFileManagerView::showNotificationMessage(std::string message, std::string type){BaseView::showNotificationMessage(message, type);}

bool MediaFileManagerView::showConfirmMenu(std::string message){return BaseView::showConfirmMenu(message);}

std::string MediaFileManagerView::displayAllMediaFile(MediaFileManager MediaFileManager) 
{
    system("clear");

    try {
        auto media_files = MediaFileManager.getAllMediaFile();

        if (media_files.empty()) {
            auto screen = ftxui::ScreenInteractive::TerminalOutput();
            auto empty_message = ftxui::Renderer([&] {
                return ftxui::vbox({
                    ftxui::text("No media files to display.") | ftxui::bold | ftxui::center,
                    ftxui::text("Press any key to exit.") | ftxui::dim | ftxui::center,
                });
            });

            auto component = ftxui::CatchEvent(empty_message, [&](ftxui::Event event) {
                if (event.is_character() || event == ftxui::Event::Escape || event == ftxui::Event::Return) {
                    screen.ExitLoopClosure()();
                    return true;
                }
                return false;
            });

            screen.Loop(component);
            return "";
        }


        const int rows_per_page = 25; // Số lượng hàng tối đa mỗi trang
        const int scroll_visible_rows = 10; // Số hàng hiển thị khi cuộn
        int current_page = 0;
        int total_pages = std::ceil((double)media_files.size() / rows_per_page);
        int selected_index = -1;  // Không có hàng nào được chọn ban đầu
        int hovered_index = -1;  // Hover index cho di chuyển chuột
        int scroll_offset = 0;   // Offset cuộn trong trang
        std::string input_buffer;
        std::string result_filename;
        bool is_exit_hovered = false;
        std::string error_message;
        auto screen = ScreenInteractive::TerminalOutput();

        // Giao diện chính
        auto render_view = [&]() -> Element {
            Elements table_rows;

            // Tiêu đề bảng
            table_rows.push_back(
                hbox({
                    text("STT") | bold | size(WIDTH, EQUAL, 5) | border,
                    text("File Name") | bold | size(WIDTH, EQUAL, 30) | border,
                    text("File Type") | bold | size(WIDTH, EQUAL, 20) | border,
                    text("Duration") | bold | size(WIDTH, EQUAL, 15) | border,
                    text("Date Created") | bold | size(WIDTH, EQUAL, 20) | border,
                    text("File Size") | bold | size(WIDTH, EQUAL, 20) | border,
                })
            );

            // Dữ liệu hàng trên trang hiện tại
            int start_index = current_page * rows_per_page;
            int end_index = std::min(start_index + rows_per_page, (int)media_files.size());
            for (int i = start_index + scroll_offset;
                 i < std::min(start_index + scroll_offset + scroll_visible_rows, end_index);
                 ++i) {
                auto mediaFile = media_files[i];
                bool is_selected = (i == selected_index);
                bool is_hovered = (i == hovered_index);

                table_rows.push_back(
                    hbox({
                        text(std::to_string(i + 1)) | size(WIDTH, EQUAL, 5) | border,
                        text(mediaFile->getName()) | size(WIDTH, EQUAL, 30) | border,
                        text(mediaFile->getType()) | size(WIDTH, EQUAL, 20) | border,
                        text(mediaFile->getDuration()) | size(WIDTH, EQUAL, 15) | border,
                        text(mediaFile->getDateCreated()) | size(WIDTH, EQUAL, 20) | border,
                        text(std::to_string(mediaFile->getSize()) + " bytes") | size(WIDTH, EQUAL, 20) | border,
                    }) | (is_selected ? inverted : nothing) // Highlight selected row
                      | (is_hovered && !is_selected ? dim | color(Color::Yellow) : nothing) // Highlight hovered row
                );
            }

            // Danh sách cuộn với thanh cuộn
            auto scrollable_list = vbox(std::move(table_rows)) | vscroll_indicator | frame | border | size(HEIGHT, EQUAL, 35);

            // Nút Exit
            auto exit_button = text("Exit (Simulate 0)") | bold | center | border |
                               (is_exit_hovered ? color(Color::Red) : nothing);

            // Giao diện tổng hợp
            return vbox({
                separator(),
                text("Use UP/DOWN to navigate rows, LEFT/RIGHT to change pages, ENTER to select.") | dim | center,
                separator(),
                hbox({
                    text("Enter filename: ") | bold,
                    text(input_buffer) | border | size(WIDTH, EQUAL, 50),
                }) | center,
                text("Selected File: " + (selected_index >= 0 && selected_index < (int)media_files.size() ? media_files[selected_index]->getName() : "")) | bold | center,
                text(error_message) | color(Color::Red) | center,
                separator(),
                text("----- All Media Files-----") | bold | center | color(Color::Blue),
                separator(),
                text("Page " + std::to_string(current_page + 1) + " of " + std::to_string(total_pages)) | center,
                scrollable_list,
                separator(),
                exit_button | center,

            });
        };

        // Container cho danh sách scroll
        auto container = Container::Vertical({
            Renderer(render_view),
        });

        auto component = CatchEvent(container, [&](Event event) -> bool {
            if (event == Event::ArrowUp) {
                if (selected_index > 0) {
                    --selected_index;
                    if (selected_index < current_page * rows_per_page + scroll_offset) {
                        --scroll_offset;
                    }
                }
                return true;
            }

            if (event == Event::ArrowDown) {
                if (selected_index < std::min((current_page + 1) * rows_per_page, (int)media_files.size()) - 1) {
                    ++selected_index;
                    if (selected_index >= current_page * rows_per_page + scroll_offset + scroll_visible_rows) {
                        ++scroll_offset;
                    }
                }
                return true;
            }

            if (event == Event::ArrowLeft) {
                if (current_page > 0) {
                    --current_page;
                    selected_index = current_page * rows_per_page;
                    scroll_offset = 0; 
                }
                return true;
            }

            if (event == Event::ArrowRight) {
                if (current_page < total_pages - 1) {
                    ++current_page;
                    selected_index = current_page * rows_per_page;
                    scroll_offset = 0;
                }
                return true;
            }

            if (event.mouse().button == Mouse::WheelUp) {
                if (scroll_offset > 0) {
                    --scroll_offset;
                }
                return true;
            }

            if (event.mouse().button == Mouse::WheelDown) {
                if (scroll_offset + scroll_visible_rows < std::min(rows_per_page, (int)media_files.size() - current_page * rows_per_page)) {
                    ++scroll_offset;
                }
                return true;
            }

            if (event.mouse().motion == Mouse::Moved) {
    // Xác định giới hạn vùng danh sách hiển thị
    const int list_y_start = 16;  // Tọa độ Y bắt đầu vùng danh sách
    const int row_height = 3;     // Chiều cao thực tế mỗi hàng
    const int list_y_end = list_y_start + scroll_visible_rows * row_height; // Tọa độ Y kết thúc vùng danh sách

    // Tính toán vị trí hàng hover
    if (event.mouse().y >= list_y_start && event.mouse().y < list_y_end) {
        int relative_y = event.mouse().y - list_y_start;
        int hovered_row = (relative_y / row_height) + (scroll_offset + current_page * rows_per_page);

        // Chỉ hover nếu chỉ số hàng hợp lệ
        if (hovered_row >= current_page * rows_per_page &&
            hovered_row < std::min((current_page + 1) * rows_per_page, (int)media_files.size())) {
            hovered_index = hovered_row;
        } else {
            hovered_index = -1; // Ngoài phạm vi hợp lệ
        }
    } else {
        hovered_index = -1; // Ngoài phạm vi danh sách
    }

    // Kiểm tra hover vào nút Exit
    const int exit_button_y_start = 47;  // Tọa độ Y bắt đầu của nút Exit
    const int exit_button_y_end = exit_button_y_start + 3; // Chiều cao của nút Exit
    is_exit_hovered = (event.mouse().y >= exit_button_y_start && event.mouse().y < exit_button_y_end);

    return true;
}

if (event.is_character()) {
    char c = event.character()[0];
    if (std::isalnum(c) || c == '.' || c == '_' || c == '-' || c == ' ' || c == '(' || c == ')') {
        input_buffer += c;
        hovered_index = -1; // Vô hiệu hóa trạng thái hover khi nhập
        return true;
    }
}

if (event == Event::Backspace && !input_buffer.empty()) {
    input_buffer.pop_back();
    hovered_index = -1; // Vô hiệu hóa trạng thái hover khi nhập
    return true;
}

if (event.mouse().button == Mouse::Left) {
    if (is_exit_hovered) {
        hovered_index = -1;
        result_filename = "";
        screen.ExitLoopClosure()();
        return true;
    }

    if (hovered_index != -1) {
        selected_index = hovered_index;
        result_filename = media_files[selected_index]->getName();
        screen.ExitLoopClosure()();
        return true;
    }
}

if (event == Event::Return) {
    if (!input_buffer.empty()) {
        if (input_buffer == "0") {
            result_filename = "";
            screen.ExitLoopClosure()();
            return true;
        }
        auto it = std::find_if(media_files.begin(), media_files.end(),
            [&](const auto& file) { return file->getName() == input_buffer; });
        if (it != media_files.end()) {
            selected_index = std::distance(media_files.begin(), it);
            current_page = selected_index / rows_per_page;
            scroll_offset = selected_index % rows_per_page;
            result_filename = media_files[selected_index]->getName();
            screen.ExitLoopClosure()();
        } else {
            error_message = "File not found!";
        }
    } else if (selected_index != -1) {
        result_filename = media_files[selected_index]->getName();
        screen.ExitLoopClosure()();
    }
    return true;
}


            if (event.is_character()) {
                char c = event.character()[0];
                if (std::isalnum(c) || c == '.' || c == '_' || c == '-' || c == ' ' || c == '(' || c == ')') {
                    input_buffer += c;
                    hovered_index = -1;
                    return true;
                }
            }

            if (event == Event::Backspace && !input_buffer.empty()) {
                input_buffer.pop_back();
                hovered_index = -1;
                return true;
            }

            return false;
        });

        screen.Loop(component);
        return result_filename;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return "";
    }
}

std::string MediaFileManagerView::displayAllMediaFileOfAudio(MediaFileManager MediaFileManager) 
{
    system("clear");

    try {
        auto media_files = MediaFileManager.getAllAudioFiles();

        if (media_files.empty()) {
            auto screen = ftxui::ScreenInteractive::TerminalOutput();
            auto empty_message = ftxui::Renderer([&] {
                return ftxui::vbox({
                    ftxui::text("No media files to display.") | ftxui::bold | ftxui::center,
                    ftxui::text("Press any key to exit.") | ftxui::dim | ftxui::center,
                });
            });

            auto component = ftxui::CatchEvent(empty_message, [&](ftxui::Event event) {
                if (event.is_character() || event == ftxui::Event::Escape || event == ftxui::Event::Return) {
                    screen.ExitLoopClosure()();
                    return true;
                }
                return false;
            });

            screen.Loop(component);
            return "";
        }


        const int rows_per_page = 25; // Số lượng hàng tối đa mỗi trang
        const int scroll_visible_rows = 10; // Số hàng hiển thị khi cuộn
        int current_page = 0;
        int total_pages = std::ceil((double)media_files.size() / rows_per_page);
        int selected_index = -1;  // Không có hàng nào được chọn ban đầu
        int hovered_index = -1;  // Hover index cho di chuyển chuột
        int scroll_offset = 0;   // Offset cuộn trong trang
        std::string input_buffer;
        std::string result_filename;
        bool is_exit_hovered = false;
        std::string error_message;
        auto screen = ScreenInteractive::TerminalOutput();

        // Giao diện chính
        auto render_view = [&]() -> Element {
            Elements table_rows;

            // Tiêu đề bảng
            table_rows.push_back(
                hbox({
                    text("STT") | bold | size(WIDTH, EQUAL, 5) | border,
                    text("File Name") | bold | size(WIDTH, EQUAL, 30) | border,
                    text("File Type") | bold | size(WIDTH, EQUAL, 20) | border,
                    text("Duration") | bold | size(WIDTH, EQUAL, 15) | border,
                    text("Date Created") | bold | size(WIDTH, EQUAL, 20) | border,
                    text("File Size") | bold | size(WIDTH, EQUAL, 20) | border,
                })
            );

            // Dữ liệu hàng trên trang hiện tại
            int start_index = current_page * rows_per_page;
            int end_index = std::min(start_index + rows_per_page, (int)media_files.size());
            for (int i = start_index + scroll_offset;
                 i < std::min(start_index + scroll_offset + scroll_visible_rows, end_index);
                 ++i) {
                auto mediaFile = media_files[i];
                bool is_selected = (i == selected_index);
                bool is_hovered = (i == hovered_index);

                table_rows.push_back(
                    hbox({
                        text(std::to_string(i + 1)) | size(WIDTH, EQUAL, 5) | border,
                        text(mediaFile->getName()) | size(WIDTH, EQUAL, 30) | border,
                        text(mediaFile->getType()) | size(WIDTH, EQUAL, 20) | border,
                        text(mediaFile->getDuration()) | size(WIDTH, EQUAL, 15) | border,
                        text(mediaFile->getDateCreated()) | size(WIDTH, EQUAL, 20) | border,
                        text(std::to_string(mediaFile->getSize()) + " bytes") | size(WIDTH, EQUAL, 20) | border,
                    }) | (is_selected ? inverted : nothing) // Highlight selected row
                      | (is_hovered && !is_selected ? dim | color(Color::Yellow) : nothing) // Highlight hovered row
                );
            }

            // Danh sách cuộn với thanh cuộn
            auto scrollable_list = vbox(std::move(table_rows)) | vscroll_indicator | frame | border | size(HEIGHT, EQUAL, 35);

            // Nút Exit
            auto exit_button = text("Exit (Simulate 0)") | bold | center | border |
                               (is_exit_hovered ? color(Color::Red) : nothing);

            // Giao diện tổng hợp
            return vbox({
                separator(),
                text("Use UP/DOWN to navigate rows, LEFT/RIGHT to change pages, ENTER to select.") | dim | center,
                separator(),
                hbox({
                    text("Enter filename: ") | bold,
                    text(input_buffer) | border | size(WIDTH, EQUAL, 50),
                }) | center,
                text("Selected File: " + (selected_index >= 0 && selected_index < (int)media_files.size() ? media_files[selected_index]->getName() : "")) | bold | center,
                text(error_message) | color(Color::Red) | center,
                separator(),
                text("----- All Audio Media Files-----") | bold | center | color(Color::Blue),
                separator(),
                text("Page " + std::to_string(current_page + 1) + " of " + std::to_string(total_pages)) | center,
                scrollable_list,
                separator(),
                exit_button | center,

            });
        };

        // Container cho danh sách scroll
        auto container = Container::Vertical({
            Renderer(render_view),
        });

        auto component = CatchEvent(container, [&](Event event) -> bool {
            if (event == Event::ArrowUp) {
                if (selected_index > 0) {
                    --selected_index;
                    if (selected_index < current_page * rows_per_page + scroll_offset) {
                        --scroll_offset;
                    }
                }
                return true;
            }

            if (event == Event::ArrowDown) {
                if (selected_index < std::min((current_page + 1) * rows_per_page, (int)media_files.size()) - 1) {
                    ++selected_index;
                    if (selected_index >= current_page * rows_per_page + scroll_offset + scroll_visible_rows) {
                        ++scroll_offset;
                    }
                }
                return true;
            }

            if (event == Event::ArrowLeft) {
                if (current_page > 0) {
                    --current_page;
                    selected_index = current_page * rows_per_page;
                    scroll_offset = 0; 
                }
                return true;
            }

            if (event == Event::ArrowRight) {
                if (current_page < total_pages - 1) {
                    ++current_page;
                    selected_index = current_page * rows_per_page;
                    scroll_offset = 0;
                }
                return true;
            }

            if (event.mouse().button == Mouse::WheelUp) {
                if (scroll_offset > 0) {
                    --scroll_offset;
                }
                return true;
            }

            if (event.mouse().button == Mouse::WheelDown) {
                if (scroll_offset + scroll_visible_rows < std::min(rows_per_page, (int)media_files.size() - current_page * rows_per_page)) {
                    ++scroll_offset;
                }
                return true;
            }

            if (event.mouse().motion == Mouse::Moved) {
    // Xác định giới hạn vùng danh sách hiển thị
    const int list_y_start = 16;  // Tọa độ Y bắt đầu vùng danh sách
    const int row_height = 3;     // Chiều cao thực tế mỗi hàng
    const int list_y_end = list_y_start + scroll_visible_rows * row_height; // Tọa độ Y kết thúc vùng danh sách

    // Tính toán vị trí hàng hover
    if (event.mouse().y >= list_y_start && event.mouse().y < list_y_end) {
        int relative_y = event.mouse().y - list_y_start;
        int hovered_row = (relative_y / row_height) + (scroll_offset + current_page * rows_per_page);

        // Chỉ hover nếu chỉ số hàng hợp lệ
        if (hovered_row >= current_page * rows_per_page &&
            hovered_row < std::min((current_page + 1) * rows_per_page, (int)media_files.size())) {
            hovered_index = hovered_row;
        } else {
            hovered_index = -1; // Ngoài phạm vi hợp lệ
        }
    } else {
        hovered_index = -1; // Ngoài phạm vi danh sách
    }

    // Kiểm tra hover vào nút Exit
    const int exit_button_y_start = 47;  // Tọa độ Y bắt đầu của nút Exit
    const int exit_button_y_end = exit_button_y_start + 3; // Chiều cao của nút Exit
    is_exit_hovered = (event.mouse().y >= exit_button_y_start && event.mouse().y < exit_button_y_end);

    return true;
}

if (event.is_character()) {
    char c = event.character()[0];
    if (std::isalnum(c) || c == '.' || c == '_' || c == '-' || c == ' ' || c == '(' || c == ')') {
        input_buffer += c;
        hovered_index = -1; // Vô hiệu hóa trạng thái hover khi nhập
        return true;
    }
}

if (event == Event::Backspace && !input_buffer.empty()) {
    input_buffer.pop_back();
    hovered_index = -1; // Vô hiệu hóa trạng thái hover khi nhập
    return true;
}

if (event.mouse().button == Mouse::Left) {
    if (is_exit_hovered) {
        hovered_index = -1;
        result_filename = "";
        screen.ExitLoopClosure()();
        return true;
    }

    if (hovered_index != -1) {
        selected_index = hovered_index;
        result_filename = media_files[selected_index]->getName();
        screen.ExitLoopClosure()();
        return true;
    }
}

if (event == Event::Return) {
    if (!input_buffer.empty()) {
        if (input_buffer == "0") {
            result_filename = "";
            screen.ExitLoopClosure()();
            return true;
        }
        auto it = std::find_if(media_files.begin(), media_files.end(),
            [&](const auto& file) { return file->getName() == input_buffer; });
        if (it != media_files.end()) {
            selected_index = std::distance(media_files.begin(), it);
            current_page = selected_index / rows_per_page;
            scroll_offset = selected_index % rows_per_page;
            result_filename = media_files[selected_index]->getName();
            screen.ExitLoopClosure()();
        } else {
            error_message = "File not found!";
        }
    } else if (selected_index != -1) {
        result_filename = media_files[selected_index]->getName();
        screen.ExitLoopClosure()();
    }
    return true;
}


            if (event.is_character()) {
                char c = event.character()[0];
                if (std::isalnum(c) || c == '.' || c == '_' || c == '-' || c == ' ' || c == '(' || c == ')') {
                    input_buffer += c;
                    hovered_index = -1;
                    return true;
                }
            }

            if (event == Event::Backspace && !input_buffer.empty()) {
                input_buffer.pop_back();
                hovered_index = -1;
                return true;
            }

            return false;
        });

        screen.Loop(component);
        return result_filename;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return "";
    }
}

std::string MediaFileManagerView::displayAllMediaFileOfVideo(MediaFileManager MediaFileManager) 
{
    system("clear");

    try {
        auto media_files = MediaFileManager.getAllVideoFiles();

        if (media_files.empty()) {
            auto screen = ftxui::ScreenInteractive::TerminalOutput();
            auto empty_message = ftxui::Renderer([&] {
                return ftxui::vbox({
                    ftxui::text("No media files to display.") | ftxui::bold | ftxui::center,
                    ftxui::text("Press any key to exit.") | ftxui::dim | ftxui::center,
                });
            });

            auto component = ftxui::CatchEvent(empty_message, [&](ftxui::Event event) {
                if (event.is_character() || event == ftxui::Event::Escape || event == ftxui::Event::Return) {
                    screen.ExitLoopClosure()();
                    return true;
                }
                return false;
            });

            screen.Loop(component);
            return "";
        }


        const int rows_per_page = 25; // Số lượng hàng tối đa mỗi trang
        const int scroll_visible_rows = 10; // Số hàng hiển thị khi cuộn
        int current_page = 0;
        int total_pages = std::ceil((double)media_files.size() / rows_per_page);
        int selected_index = -1;  // Không có hàng nào được chọn ban đầu
        int hovered_index = -1;  // Hover index cho di chuyển chuột
        int scroll_offset = 0;   // Offset cuộn trong trang
        std::string input_buffer;
        std::string result_filename;
        bool is_exit_hovered = false;
        std::string error_message;
        auto screen = ScreenInteractive::TerminalOutput();

        // Giao diện chính
        auto render_view = [&]() -> Element {
            Elements table_rows;

            // Tiêu đề bảng
            table_rows.push_back(
                hbox({
                    text("STT") | bold | size(WIDTH, EQUAL, 5) | border,
                    text("File Name") | bold | size(WIDTH, EQUAL, 30) | border,
                    text("File Type") | bold | size(WIDTH, EQUAL, 20) | border,
                    text("Duration") | bold | size(WIDTH, EQUAL, 15) | border,
                    text("Date Created") | bold | size(WIDTH, EQUAL, 20) | border,
                    text("File Size") | bold | size(WIDTH, EQUAL, 20) | border,
                })
            );

            // Dữ liệu hàng trên trang hiện tại
            int start_index = current_page * rows_per_page;
            int end_index = std::min(start_index + rows_per_page, (int)media_files.size());
            for (int i = start_index + scroll_offset;
                 i < std::min(start_index + scroll_offset + scroll_visible_rows, end_index);
                 ++i) {
                auto mediaFile = media_files[i];
                bool is_selected = (i == selected_index);
                bool is_hovered = (i == hovered_index);

                table_rows.push_back(
                    hbox({
                        text(std::to_string(i + 1)) | size(WIDTH, EQUAL, 5) | border,
                        text(mediaFile->getName()) | size(WIDTH, EQUAL, 30) | border,
                        text(mediaFile->getType()) | size(WIDTH, EQUAL, 20) | border,
                        text(mediaFile->getDuration()) | size(WIDTH, EQUAL, 15) | border,
                        text(mediaFile->getDateCreated()) | size(WIDTH, EQUAL, 20) | border,
                        text(std::to_string(mediaFile->getSize()) + " bytes") | size(WIDTH, EQUAL, 20) | border,
                    }) | (is_selected ? inverted : nothing) // Highlight selected row
                      | (is_hovered && !is_selected ? dim | color(Color::Yellow) : nothing) // Highlight hovered row
                );
            }

            // Danh sách cuộn với thanh cuộn
            auto scrollable_list = vbox(std::move(table_rows)) | vscroll_indicator | frame | border | size(HEIGHT, EQUAL, 35);

            // Nút Exit
            auto exit_button = text("Exit (Simulate 0)") | bold | center | border |
                               (is_exit_hovered ? color(Color::Red) : nothing);

            // Giao diện tổng hợp
            return vbox({
                separator(),
                text("Use UP/DOWN to navigate rows, LEFT/RIGHT to change pages, ENTER to select.") | dim | center,
                separator(),
                hbox({
                    text("Enter filename: ") | bold,
                    text(input_buffer) | border | size(WIDTH, EQUAL, 50),
                }) | center,
                text("Selected File: " + (selected_index >= 0 && selected_index < (int)media_files.size() ? media_files[selected_index]->getName() : "")) | bold | center,
                text(error_message) | color(Color::Red) | center,
                separator(),
                text("----- All Video Media Files-----") | bold | center | color(Color::Blue),
                separator(),
                text("Page " + std::to_string(current_page + 1) + " of " + std::to_string(total_pages)) | center,
                scrollable_list,
                separator(),
                exit_button | center,

            });
        };

        // Container cho danh sách scroll
        auto container = Container::Vertical({
            Renderer(render_view),
        });

        auto component = CatchEvent(container, [&](Event event) -> bool {
            if (event == Event::ArrowUp) {
                if (selected_index > 0) {
                    --selected_index;
                    if (selected_index < current_page * rows_per_page + scroll_offset) {
                        --scroll_offset;
                    }
                }
                return true;
            }

            if (event == Event::ArrowDown) {
                if (selected_index < std::min((current_page + 1) * rows_per_page, (int)media_files.size()) - 1) {
                    ++selected_index;
                    if (selected_index >= current_page * rows_per_page + scroll_offset + scroll_visible_rows) {
                        ++scroll_offset;
                    }
                }
                return true;
            }

            if (event == Event::ArrowLeft) {
                if (current_page > 0) {
                    --current_page;
                    selected_index = current_page * rows_per_page;
                    scroll_offset = 0; 
                }
                return true;
            }

            if (event == Event::ArrowRight) {
                if (current_page < total_pages - 1) {
                    ++current_page;
                    selected_index = current_page * rows_per_page;
                    scroll_offset = 0;
                }
                return true;
            }

            if (event.mouse().button == Mouse::WheelUp) {
                if (scroll_offset > 0) {
                    --scroll_offset;
                }
                return true;
            }

            if (event.mouse().button == Mouse::WheelDown) {
                if (scroll_offset + scroll_visible_rows < std::min(rows_per_page, (int)media_files.size() - current_page * rows_per_page)) {
                    ++scroll_offset;
                }
                return true;
            }

            if (event.mouse().motion == Mouse::Moved) {
    // Xác định giới hạn vùng danh sách hiển thị
    const int list_y_start = 16;  // Tọa độ Y bắt đầu vùng danh sách
    const int row_height = 3;     // Chiều cao thực tế mỗi hàng
    const int list_y_end = list_y_start + scroll_visible_rows * row_height; // Tọa độ Y kết thúc vùng danh sách

    // Tính toán vị trí hàng hover
    if (event.mouse().y >= list_y_start && event.mouse().y < list_y_end) {
        int relative_y = event.mouse().y - list_y_start;
        int hovered_row = (relative_y / row_height) + (scroll_offset + current_page * rows_per_page);

        // Chỉ hover nếu chỉ số hàng hợp lệ
        if (hovered_row >= current_page * rows_per_page &&
            hovered_row < std::min((current_page + 1) * rows_per_page, (int)media_files.size())) {
            hovered_index = hovered_row;
        } else {
            hovered_index = -1; // Ngoài phạm vi hợp lệ
        }
    } else {
        hovered_index = -1; // Ngoài phạm vi danh sách
    }

    // Kiểm tra hover vào nút Exit
    const int exit_button_y_start = 47;  // Tọa độ Y bắt đầu của nút Exit
    const int exit_button_y_end = exit_button_y_start + 3; // Chiều cao của nút Exit
    is_exit_hovered = (event.mouse().y >= exit_button_y_start && event.mouse().y < exit_button_y_end);

    return true;
}

if (event.is_character()) {
    char c = event.character()[0];
    if (std::isalnum(c) || c == '.' || c == '_' || c == '-' || c == ' ' || c == '(' || c == ')') {
        input_buffer += c;
        hovered_index = -1; // Vô hiệu hóa trạng thái hover khi nhập
        return true;
    }
}

if (event == Event::Backspace && !input_buffer.empty()) {
    input_buffer.pop_back();
    hovered_index = -1; // Vô hiệu hóa trạng thái hover khi nhập
    return true;
}

if (event.mouse().button == Mouse::Left) {
    if (is_exit_hovered) {
        hovered_index = -1;
        result_filename = "";
        screen.ExitLoopClosure()();
        return true;
    }

    if (hovered_index != -1) {
        selected_index = hovered_index;
        result_filename = media_files[selected_index]->getName();
        screen.ExitLoopClosure()();
        return true;
    }
}

if (event == Event::Return) {
    if (!input_buffer.empty()) {
        if (input_buffer == "0") {
            result_filename = "";
            screen.ExitLoopClosure()();
            return true;
        }
        auto it = std::find_if(media_files.begin(), media_files.end(),
            [&](const auto& file) { return file->getName() == input_buffer; });
        if (it != media_files.end()) {
            selected_index = std::distance(media_files.begin(), it);
            current_page = selected_index / rows_per_page;
            scroll_offset = selected_index % rows_per_page;
            result_filename = media_files[selected_index]->getName();
            screen.ExitLoopClosure()();
        } else {
            error_message = "File not found!";
        }
    } else if (selected_index != -1) {
        result_filename = media_files[selected_index]->getName();
        screen.ExitLoopClosure()();
    }
    return true;
}


            if (event.is_character()) {
                char c = event.character()[0];
                if (std::isalnum(c) || c == '.' || c == '_' || c == '-' || c == ' ' || c == '(' || c == ')') {
                    input_buffer += c;
                    hovered_index = -1;
                    return true;
                }
            }

            if (event == Event::Backspace && !input_buffer.empty()) {
                input_buffer.pop_back();
                hovered_index = -1;
                return true;
            }

            return false;
        });

        screen.Loop(component);
        return result_filename;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return "";
    }
}

