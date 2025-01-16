#include "../../include/View/MainMenuView.h"
#include <iostream>
MainMenuView::MainMenuView() {}




int MainMenuView::showMenu() {
    system("clear");
    // Danh sách hiển thị trên menu
    std::vector<std::string> menu_entries = {
        "1. MetadataFile Handler",
        "2. MediaFile Manager",
        "3. Playlist Handler",
        "4. Playlist Manager",
        "0. Exit"
    };

    // Ánh xạ từ chỉ số menu (0-based index) sang giá trị logic
    std::vector<int> logic_mapping = {1, 2, 3, 4, 0};

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