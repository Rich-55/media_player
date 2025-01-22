#include "../../include/View/ScanView.h"

ScanView::ScanView() {}

int ScanView::showMenu() {
    showFileAdded(); // Hiển thị file đã thêm (giả sử đây là một hàm sẵn có).

    std::vector<std::string> menu_entries = {
        "1. Scan Home Directory",
        "2. Scan USB Devices",
        "0. Back to Main Menu"
    };

    std::vector<int> logic_mapping = {1, 2, 0}; // Liên kết mục menu với logic tương ứng.

    int selected = 0; // Vị trí được chọn ban đầu.
    std::string error_message; // Lưu thông báo lỗi nếu có.
    int final_selected = -1; // Kết quả trả về cuối cùng.

    auto menu = Menu(&menu_entries, &selected);
    auto screen = ScreenInteractive::TerminalOutput();

    auto main_component = Renderer(menu, [&] {
        return vbox({
            text("=========== Menu Scan View ===========") | bold | center,
            separator(),
            menu->Render() | border, // Hiển thị menu
            separator(),
            text("Use UP/DOWN keys, numbers (0-9), or click to navigate. Press ENTER to select.") | dim | center,
            separator(),
            text(error_message) | color(Color::Red) | center // Hiển thị lỗi nếu có.
        });
    });

    main_component = CatchEvent(main_component, [&](Event event) {
        if (event == Event::Return) {
            final_selected = logic_mapping[selected]; // Gán giá trị mục được chọn.
            screen.ExitLoopClosure()(); // Thoát khỏi giao diện.
            return true;
        }

        if (event.is_character() && std::isdigit(event.character()[0])) {
            int number = event.character()[0] - '0'; // Lấy số từ ký tự.
            auto it = std::find(logic_mapping.begin(), logic_mapping.end(), number);
            if (it != logic_mapping.end()) {
                final_selected = number; // Gán kết quả theo số được nhập.
                screen.ExitLoopClosure()(); // Thoát khỏi giao diện.
                return true;
            } else {
                error_message = "Invalid input: number not in menu!"; // Số nhập không hợp lệ.
                return true;
            }
        }

        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3; // Căn chỉnh nếu cần.
            if (clicked_index >= 0 && clicked_index < (int)menu_entries.size()) {
                final_selected = logic_mapping[clicked_index]; // Gán kết quả theo mục nhấn chuột.
                screen.ExitLoopClosure()(); // Thoát khỏi giao diện.
                return true;
            } else {
                error_message = "Invalid click: out of menu range!"; // Click ngoài menu.
                return true;
            }
        }

        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            menu->OnEvent(event); // Xử lý sự kiện mũi tên.
            return true;
        }

        return false; // Không xử lý sự kiện khác.
    });

    screen.Loop(main_component);

    return final_selected; // Trả về giá trị mục được chọn.
}

int ScanView::showListFolder(std::vector<std::string> listFolder) {
    if (listFolder.empty()) {
        std::cout << "No folders found.\n";
        return -1;
    }

    int selected_index = 0;
    std::string error_message;
    int final_selected = -1;
    std::string input_buffer; // Buffer để lưu giá trị nhập từ người dùng.

    // Thêm mục vào danh sách
    std::vector<std::string> menu_entries;
    for (size_t i = 0; i < listFolder.size(); ++i) {
        menu_entries.push_back(std::to_string(i + 1) + ". " + listFolder[i]);
    }
    menu_entries.push_back("0. Exit"); // Thêm tùy chọn "0. Exit"

    auto menu = Menu(&menu_entries, &selected_index);
    auto screen = ScreenInteractive::TerminalOutput();

    auto main_component = Renderer(menu, [&] {
        return vbox({
            text("========= List of Folders =========") | bold | center,
            separator(),
            menu->Render() | border | center,
            separator(),
            hbox({
                text("Enter your choice: ") | bold,
                text(input_buffer) | bold | dim,
            }),
            separator(),
            text("Use UP/DOWN keys or click to navigate. Press ENTER to select.") | dim | center,
            separator(),
            text(error_message) | color(Color::Red) | center,
        });
    });

    main_component = CatchEvent(main_component, [&](Event event) {
        if (event == Event::Return) {
            // Nếu input buffer không trống, xử lý nhập liệu
            if (!input_buffer.empty()) {
                try {
                    int number = std::stoi(input_buffer); // Chuyển đổi input thành số
                    if (number == 0) {
                        final_selected = -1; // Chọn "0. Exit"
                    } else if (number > 0 && number <= (int)listFolder.size()) {
                        final_selected = number - 1; // Chọn mục theo số nhập
                    } else {
                        error_message = "Invalid input: number not in menu!";
                    }
                } catch (const std::exception &) {
                    error_message = "Invalid input: please enter a valid number!";
                }
                input_buffer.clear(); // Xóa input buffer sau khi xử lý
                screen.ExitLoopClosure()();
                return true;
            }

            // Nếu không có input, chọn mục hiện tại
            if (selected_index == (int)listFolder.size()) {
                final_selected = -1; // Chọn "0. Exit"
            } else {
                final_selected = selected_index; // Chọn thư mục
            }
            screen.ExitLoopClosure()();
            return true;
        }

        if (event.is_character()) {
            input_buffer += event.character(); // Thêm ký tự vào input buffer
            return true;
        }

        if (event == Event::Backspace && !input_buffer.empty()) {
            input_buffer.pop_back(); // Xóa ký tự cuối cùng trong buffer
            return true;
        }

        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3; // Điều chỉnh tùy theo độ lệch UI
            if (clicked_index >= 0 && clicked_index < (int)menu_entries.size()) {
                if (clicked_index == (int)listFolder.size()) {
                    final_selected = -1; // Chọn "0. Exit"
                } else {
                    final_selected = clicked_index; // Lựa chọn theo click chuột
                }
                screen.ExitLoopClosure()();
                return true;
            } else {
                error_message = "Invalid click: out of menu range!";
                return true;
            }
        }

        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            menu->OnEvent(event); // Xử lý mũi tên lên/xuống
            return true;
        }

        return false;
    });

    screen.Loop(main_component);

    return final_selected; // Trả về chỉ mục thư mục được chọn hoặc -1 nếu chọn "0. Exit"
}

int ScanView::showListUSBName(std::vector<std::string> usbDevices) {
    if (usbDevices.empty()) {
        std::cout << "No USB devices found.\n";
        return -1; // Trả về -1 nếu không có USB
    }

    int selected_index = 0; // Chỉ mục được chọn
    std::string error_message;
    int final_choice = -1; // Chỉ mục cuối cùng được trả về

    // Thêm chỉ mục vào danh sách USB Devices
    std::vector<std::string> menu_entries;
    for (size_t i = 0; i < usbDevices.size(); ++i) {
        menu_entries.push_back(std::to_string(i + 1) + ". " + usbDevices[i]);
    }
    menu_entries.push_back("0. Exit"); // Thêm tùy chọn "0. Exit"

    auto menu = Menu(&menu_entries, &selected_index);
    auto screen = ScreenInteractive::TerminalOutput();

    auto main_component = Renderer(menu, [&] {
        return vbox({
            text("========== USB Devices ==========") | bold | center,
            separator(),
            menu->Render() | border,
            separator(),
            text("Use UP/DOWN keys, numbers (0-9), or click to select a USB.") | dim | center,
            separator(),
            text(error_message) | color(Color::Red) | hcenter,
        });
    });

    main_component = CatchEvent(main_component, [&](Event event) {
        if (event == Event::Return) {
            if (selected_index == (int)usbDevices.size()) {
                // Nếu chọn "0. Exit"
                final_choice = -1;
            } else {
                // Chọn USB
                final_choice = selected_index;
            }
            screen.ExitLoopClosure()();
            return true;
        }

        if (event.is_character() && std::isdigit(event.character()[0])) {
            int number = event.character()[0] - '0'; // Chuyển ký tự thành số
            if (number == 0) {
                final_choice = -1; // Chọn "0. Exit"
                screen.ExitLoopClosure()();
                return true;
            } else if (number >= 1 && number <= (int)usbDevices.size()) {
                final_choice = number - 1; // Lựa chọn theo số nhập
                screen.ExitLoopClosure()();
                return true;
            } else {
                error_message = "Invalid input: number not in range!";
                return true;
            }
        }

        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3; // Điều chỉnh tùy theo độ lệch UI
            if (clicked_index >= 0 && clicked_index < (int)menu_entries.size()) {
                if (clicked_index == (int)usbDevices.size()) {
                    final_choice = -1; // Chọn "0. Exit"
                } else {
                    final_choice = clicked_index; // Lựa chọn theo click chuột
                }
                screen.ExitLoopClosure()();
                return true;
            } else {
                error_message = "Invalid click: out of menu range!";
                return true;
            }
        }

        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            menu->OnEvent(event); // Xử lý mũi tên lên/xuống
            error_message.clear();
            return true;
        }

        return false;
    });

    screen.Loop(main_component);

    return final_choice; // Trả về chỉ mục USB được chọn hoặc -1 nếu chọn "0. Exit"
}

void ScanView::setListPathNameIsAdded(std::unordered_set<std::string> listPathName)
{
    this->listPathNameIsAdded = listPathName;
}

void ScanView::showFileAdded()
{
    if (this->listPathNameIsAdded.empty()) {
        // Nếu không có file được thêm
        auto empty_view = vbox({
            text("========== List of New Files Added ==========") | bold | center,
            separator(),
            text("No files added.") | center | color(Color::Red),
            separator(),
        });

        auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(empty_view));
        Render(screen, empty_view);
        screen.Print();
        return;
    }

    // Nếu có file được thêm
    std::vector<Element> file_elements;
    for (const auto& path : this->listPathNameIsAdded) {
        file_elements.push_back(text(path));
    }

    auto file_view = vbox({
        text("========== List of New Files ==========") | bold | center,
        separator(),
        vbox(std::move(file_elements)) | border | vscroll_indicator,
        separator(),
    });

    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(file_view));
    Render(screen, file_view);
    screen.Print();
}




