#include "../../include/View/MainMenuView.h"
#include <iostream>
MainMenuView::MainMenuView() {}

int MainMenuView::showMenu() {
    //system("clear");
    // Danh sách hiển thị trên menu
    std::vector<std::string> menu_entries = {
        "1. MetadataFile Handler",
        "2. MediaFile Manager",
        "3. Playlist Handler",
        "4. Playlist Manager",
        "5. Play Music",
        "0. Exit"
    };

    // Ánh xạ từ chỉ số menu (0-based index) sang giá trị logic
    std::vector<int> logic_mapping = {1, 2, 3, 4, 5, 0};

    int selected = 0; // Chỉ số được FTXUI sử dụng (0-based index)
    std::string error_message; // Lưu trữ thông báo lỗi
    int final_selected = -1; // Giá trị cuối cùng trả về (logic value)

    // Tạo menu
    auto menu = Menu(&menu_entries, &selected);

    // Tạo màn hình
    auto screen = ScreenInteractive::TerminalOutput();

    // Tạo giao diện chính
    auto main_component = Renderer(menu, [&] {
        return vbox({
            text("============Main Menu==========") | center,
            separator(),
            menu->Render() | border,
            separator(),
            text("Use UP/DOWN keys, numbers (0-9), or click to navigate. Press ENTER to select.") | dim | center,
            separator(),
            text(error_message) | color(Color::Red) | center // Hiển thị thông báo lỗi (nếu có)
        });
    });

    // Xử lý sự kiện
    main_component = CatchEvent(main_component, [&](Event event) {
        // Khi nhấn Enter
        if (event == Event::Return) {
            final_selected = logic_mapping[selected]; // Lấy giá trị logic từ ánh xạ
            screen.ExitLoopClosure()(); // Thoát vòng lặp giao diện
            return true;
        }

        // Khi nhấn phím số (0-9)
        if (event.is_character() && std::isdigit(event.character()[0])) {
            int number = event.character()[0] - '0'; // Chuyển ký tự thành số
            auto it = std::find(logic_mapping.begin(), logic_mapping.end(), number);
            if (it != logic_mapping.end()) {
                final_selected = number; // Gán giá trị logic
                screen.ExitLoopClosure()(); // Thoát giao diện
                return true;
            } else {
                error_message = "Invalid input: number not in menu!"; // Hiển thị lỗi
                return true;
            }
        }

        // Xử lý sự kiện click chuột
        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3; // Tính chỉ số mục được click
            if (clicked_index >= 0 && clicked_index < (int)menu_entries.size()) {
                final_selected = logic_mapping[clicked_index]; // Lấy giá trị logic từ ánh xạ
                screen.ExitLoopClosure()(); // Thoát giao diện
                return true;
            } else {
                error_message = "Invalid click: out of menu range!"; // Hiển thị lỗi
                return true;
            }
        }

        // Khi di chuyển bằng phím mũi tên
        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            menu->OnEvent(event); // Xử lý phím mũi tên để di chuyển trong menu
            return true;
        }

        return false; // Không xử lý sự kiện nào khác
    });

    // Hiển thị giao diện
    screen.Loop(main_component);

    return final_selected; // Trả về giá trị logic đã chọn
}

int MainMenuView::showMenuWithPlayer(MediaFileManager mediaFileManager, std::shared_ptr<PlayerController>& playerController, std::string typePlay, std::string& singleMedia, std::vector<std::string>& listMedia) {
    
    static bool clear_required = true; // Đảm bảo xóa màn hình chỉ xảy ra 1 lần

    // Xóa màn hình khi bắt đầu hiển thị
    if (clear_required) {
        system("clear");
        clear_required = false;
    }
    std::shared_ptr<MediaFile> media = nullptr;
    if (!singleMedia.empty() && typePlay == "single") {
        media = mediaFileManager.getMediaFileByPath(singleMedia);
    }

    // Danh sách menu
    std::vector<std::string> menu_entries = {
        "1. MetadataFile Handler",
        "2. MediaFile Manager",
        "3. Playlist Handler",
        "4. Playlist Manager",
        "5. Play Music",
        "6. Play Playlist",
        "0. Exit"
    };

    int selected = 0; // Chỉ số menu (0-based index)
    auto menu = Menu(&menu_entries, &selected);
    const int rows_per_page = 25; // Số lượng media mỗi trang
    const int scroll_visible_rows = 10; // Số hàng hiển thị khi cuộn
    int current_page = 0;         // Trang hiện tại
    int total_pages = std::ceil((double)listMedia.size() / rows_per_page);
    int scroll_offset = 0;
    // Biến âm lượng
    int volume = 50; // Giá trị mặc định của âm lượng (từ 0-100)

    // Kết quả cuối cùng trả về
    int button_result = -1;

    // Tạo các nút điều khiển
    auto play_button = Button("Play", [&] {
    if (playerController) {
        playerController->togglePlayback();
        ScreenInteractive::Active()->PostEvent(Event::Custom);
    }
});

auto stop_button = Button("Stop", [&] {
    if (playerController) {
        playerController->stop();
        ScreenInteractive::Active()->PostEvent(Event::Custom);
    }
});

auto repeat_button = Button("Repeat", [&] {
    if (playerController) {
        playerController->toggleRepeat();
        ScreenInteractive::Active()->PostEvent(Event::Custom);
    }
});

auto next_button = Button("Next", [&] {
    if (playerController) {
        playerController->playNext();
        ScreenInteractive::Active()->PostEvent(Event::Custom);
    }
});

auto previous_button = Button("Previous", [&] {
    if (playerController) {
        playerController->playPrevious();
        ScreenInteractive::Active()->PostEvent(Event::Custom);
    }
});

auto volume_up_button = Button("+ Volume", [&] {
    if (playerController) {
        playerController->increaseVolume(10);
        ScreenInteractive::Active()->PostEvent(Event::Custom);
    }
});

auto volume_down_button = Button("- Volume", [&] {
    if (playerController) {
        playerController->decreaseVolume(10);
        ScreenInteractive::Active()->PostEvent(Event::Custom);
    }
});

    // Tạo container cho các nút
    auto button_container = Container::Horizontal({
        play_button, stop_button, repeat_button, next_button, previous_button, volume_up_button, volume_down_button
    });

    // Renderer cho trạng thái nhạc và thông tin file nhạc
    auto media_info_renderer = Renderer([&] {
        std::vector<Element> metadata_elements;
        if (typePlay == "noplay") {
            // Trường hợp không có bài hát nào đang phát
            return vbox({
                text("=== Music Status ===") | bold | center,
                text("No music is playing.") | center,
                separator(),
                text("Volume: " + std::to_string(volume)) | center
            }) | border;
        } else if (typePlay == "single" && media) {
            // Trường hợp phát một bài hát
            for (const auto& [key, value] : media->getAllMetadata()) {
                metadata_elements.push_back(text(key + ": " + value));
            }

            return vbox({
                text("=== Music Status ===") | bold | center,
                text("Currently playing: " + singleMedia) | center,
                text("Duration: " + media->getDuration()) | center,
                separator(),
                text("Volume: " + std::to_string(volume)) | center,
                separator(),
                text("Media Info:") | bold,
                vbox(std::move(metadata_elements))
            }) | border;
        } else if (typePlay == "playlist" && !listMedia.empty()) {
            // Tạo giao diện danh sách media
            std::vector<ftxui::Element> rows;

            if (listMedia.empty()) {
                rows.push_back(ftxui::text("No media files in the playlist.") | ftxui::bold | ftxui::center);
            } else {
                rows.push_back(ftxui::hbox({
                    text("STT") | bold | size(WIDTH, EQUAL, 5) | border,
                    text("File Name") | bold | size(WIDTH, EQUAL, 30) | border,
                }));

                int start_index = current_page * rows_per_page;
                int end_index = std::min(start_index + rows_per_page, (int)listMedia.size());

                for (int i = start_index + scroll_offset;
                    i < std::min(start_index + scroll_offset + scroll_visible_rows, end_index);
                    ++i) {
                    rows.push_back(ftxui::hbox({
                        text(std::to_string(i + 1)) | size(WIDTH, EQUAL, 5) | border,
                        text(listMedia[i]) | size(WIDTH, EQUAL, 30) | border,
                    }));
                }
            }

            return ftxui::vbox({
                ftxui::text("----- All Media File -----") | ftxui::bold | ftxui::center | ftxui::color(ftxui::Color::Blue),
                ftxui::text("Page " + std::to_string(current_page + 1) + " of " + std::to_string(total_pages)) | ftxui::center,
                ftxui::separator(),
                ftxui::vbox(std::move(rows)) | ftxui::vscroll_indicator | ftxui::frame | ftxui::border,
            });
        }


        // Trường hợp không xác định
        return vbox({
            text("=== Music Status ===") | bold | center,
            text("No valid playback type.") | center,
            separator(),
            text("Volume: " + std::to_string(volume)) | center
        }) | border;
    });

    // Layout tổng thể
    auto layout = Container::Vertical({
        menu,
        button_container,
        media_info_renderer
    });

    auto main_component = Renderer(layout, [&] {
        return vbox({
            text("============ Main Menu ============") | center,
            separator(),
            menu->Render() | border,
            separator(),
            hbox({
                play_button->Render(),
                stop_button->Render(),
                repeat_button->Render(),
                next_button->Render(),
                previous_button->Render(),
                volume_up_button->Render(),
                volume_down_button->Render(),
            }) | center,
            separator(),
            media_info_renderer->Render()
        });
    });

    system("clear");
    auto screen = ScreenInteractive::TerminalOutput();

    // Xử lý sự kiện
    auto final_component = CatchEvent(main_component, [&](Event event) {
        // Xử lý khi nhấn Enter
        if (event == Event::Return) {
            if (selected == static_cast<int>(menu_entries.size() - 1)) { // Nếu chọn "0. Exit"
                button_result = 0; // Trả về 0
            } else {
                button_result = selected + 1; // Chọn mục trong menu
            }
            screen.ExitLoopClosure()();
            return true;
        }

        // Khi nhấn phím số (0-9)
        if (event.is_character() && std::isdigit(event.character()[0])) {
            int number = event.character()[0] - '0';
            if (number >= 0 && number < static_cast<int>(menu_entries.size())) {
                button_result = number == 0 ? 0 : number; // Nếu nhấn '0', trả về 0
                screen.ExitLoopClosure()();
                return true;
            }
        }

        // Khi di chuyển bằng phím mũi tên
        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            menu->OnEvent(event); // Di chuyển trong menu
            return true;
        }

        // Xử lý chuột cho menu
        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3; // Tính toán chỉ số mục được click
            if (clicked_index >= 0 && clicked_index < static_cast<int>(menu_entries.size())) {
                button_result = clicked_index == static_cast<int>(menu_entries.size() - 1) ? 0 : clicked_index + 1; // Nếu click vào "0. Exit", trả về 0
                screen.ExitLoopClosure()();
                return true;
            }
        }
        if (event == Event::ArrowLeft) {
            if (current_page > 0) {
                --current_page;
                scroll_offset = 0; // Reset offset khi chuyển trang
            } 
            return true;
        }
        if (event == Event::ArrowRight) {
            if (current_page < total_pages - 1) {
                ++current_page;
                scroll_offset = 0; // Reset offset khi chuyển trang
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
            int max_scroll = std::min(rows_per_page, (int)mediaFileManager.getAllMediaFile().size() - current_page * rows_per_page);
            if (scroll_offset + scroll_visible_rows < max_scroll) {
                ++scroll_offset;
            }
            return true;
        }

        return false; // Không xử lý các sự kiện khác
    });

    // Bắt đầu vòng lặp giao diện
    screen.Loop(final_component);

    // Trả về lựa chọn từ menu hoặc nút
    return button_result;
}
