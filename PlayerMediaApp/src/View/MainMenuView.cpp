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

int MainMenuView::showMenuWithPlayer(MediaFileManager mediaFileManager, std::string fileName) {
    system("clear");

    std::shared_ptr<MediaFile> media = nullptr;
    if (!fileName.empty()) {
        media = mediaFileManager.getMediaFile(fileName);
    }

    // Danh sách menu
    std::vector<std::string> menu_entries = {
        "1. MetadataFile Handler",
        "2. MediaFile Manager",
        "3. Playlist Handler",
        "4. Playlist Manager",
        "5. Play Music",
        "0. Exit"
    };

    int selected = 0; // Chỉ số được FTXUI sử dụng (0-based index)
    int final_selected = -1; // Giá trị cuối cùng trả về (logic value)

    auto menu = Menu(&menu_entries, &selected);

    // Nút điều khiển
    int button_result = -1; // Giá trị mặc định nếu chọn các nút
    auto play_button = Button("Play", [&] { button_result = -2; });
    auto stop_button = Button("Stop", [&] { button_result = -3; });
    auto pause_button = Button("Pause", [&] { button_result = -4; });

    int volume = 50; // Âm lượng mặc định
    auto volume_up_button = Button("+ Volume", [&] { volume = std::min(100, volume + 10); });
    auto volume_down_button = Button("- Volume", [&] { volume = std::max(0, volume - 10); });

    // Đặt tất cả các nút vào một Container
    auto button_container = Container::Horizontal({
        play_button, stop_button, pause_button, volume_up_button, volume_down_button
    });

    // Thông tin media
    auto media_info_renderer = Renderer([&] {
        std::vector<Element> metadata_elements;

        if (media) {
            for (const auto& [key, value] : media->getAllMetadata()) {
                metadata_elements.push_back(text(key + ": " + value));
            }
        }

        return vbox(Elements{
            text("Media Info") | bold | center,
            text("Name: " + (media ? media->getName() : "N/A")),
            text("Duration: " + (media ? media->getDuration() : "N/A")),
            separator(),
            media ? vbox(std::move(metadata_elements)) | border : text("No Metadata Available") | center
        });
    });

    // Kết hợp menu và các nút trong một container chính
    auto layout = Container::Vertical({
        menu,
        button_container
    });

    // Renderer cho giao diện chính
    auto main_component = Renderer(layout, [&] {
        return vbox(Elements{
            text("============Main Menu==========") | center,
            separator(),
            menu->Render() | border,
            separator(),
            hbox({
                text("Controls: ") | bold,
                play_button->Render(),
                stop_button->Render(),
                pause_button->Render(),
                volume_up_button->Render(),
                volume_down_button->Render(),
            }) | center,
            separator(),
            media_info_renderer->Render()
        });
    });

    // Màn hình tương tác
    auto screen = ScreenInteractive::TerminalOutput();

    // Sử dụng `CatchEvent` để xử lý sự kiện
    auto final_component = CatchEvent(main_component, [&](Event event) {
        // Xử lý khi nhấn Enter
        if (event == Event::Return) {
            // Nếu chọn "0. Exit", trả về 0
            if (selected == static_cast<int>(menu_entries.size() - 1)) { // Ép kiểu `menu_entries.size()` sang `int`
                final_selected = 0;
            } else {
                final_selected = selected + 1; // Lấy mục đang chọn (menu)
            }
            screen.ExitLoopClosure()();
            return true;
        }

        // Khi nhấn phím số (0-9)
        if (event.is_character() && std::isdigit(event.character()[0])) {
            int number = event.character()[0] - '0';
            if (number >= 0 && number < static_cast<int>(menu_entries.size())) { // Ép kiểu `menu_entries.size()` sang `int`
                final_selected = number == 0 ? 0 : number; // Nếu nhấn '0', trả về 0
                screen.ExitLoopClosure()();
                return true;
            }
        }

        // Xử lý khi di chuyển bằng phím mũi tên
        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            menu->OnEvent(event); // Di chuyển trong menu
            return true;
        }

        // Không xử lý sự kiện chuột trong `CatchEvent` cho button (vì đã nằm trong container)
        return false; // Không xử lý các sự kiện khác
    });

    // Bắt đầu vòng lặp giao diện
    screen.Loop(final_component);

    // Trả về giá trị từ các nút hoặc menu
    return button_result != -1 ? button_result : final_selected;
}