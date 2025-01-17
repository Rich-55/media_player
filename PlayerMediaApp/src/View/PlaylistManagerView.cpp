#include "../../include/View/PlaylistManagerView.h"

PlaylistManagerView::PlaylistManagerView(){}
        
int PlaylistManagerView::showMenu() {
    std::vector<std::string> menu_entries = {
        "1. Create new playlist",
        "2. Delete media file from playlist",
        "3. Show all playlists",
        "0. Exit"
    };

    std::vector<int> logic_mapping = {1, 2, 3, 0}; // Liên kết mục menu với logic tương ứng

    int selected = 0; // Vị trí được chọn ban đầu
    std::string error_message; // Lưu thông báo lỗi nếu có
    int final_selected = -1; // Kết quả trả về cuối cùng

    auto menu = Menu(&menu_entries, &selected);
    auto screen = ScreenInteractive::TerminalOutput();

    auto main_component = Renderer(menu, [&] {
        return vbox({
            text("============ Playlist Manager ===========") | bold | center,
            separator(),
            menu->Render() | border, // Hiển thị menu
            separator(),
            text("Use UP/DOWN keys, numbers (0-9), or click to navigate. Press ENTER to select.") | dim | center,
            separator(),
            text(error_message) | color(Color::Red) | center // Hiển thị lỗi nếu có
        });
    });

    main_component = CatchEvent(main_component, [&](Event event) {
        if (event == Event::Return) {
            final_selected = logic_mapping[selected]; // Gán giá trị mục được chọn
            screen.ExitLoopClosure()(); // Thoát khỏi giao diện
            return true;
        }

        if (event.is_character() && std::isdigit(event.character()[0])) {
            int number = event.character()[0] - '0'; // Lấy số từ ký tự
            auto it = std::find(logic_mapping.begin(), logic_mapping.end(), number);
            if (it != logic_mapping.end()) {
                final_selected = number; // Gán kết quả theo số được nhập
                screen.ExitLoopClosure()(); // Thoát khỏi giao diện
                return true;
            } else {
                error_message = "Invalid input: number not in menu!"; // Số nhập không hợp lệ
                return true;
            }
        }

        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3; // Căn chỉnh nếu cần
            if (clicked_index >= 0 && clicked_index < (int)menu_entries.size()) {
                final_selected = logic_mapping[clicked_index]; // Gán kết quả theo mục nhấn chuột
                screen.ExitLoopClosure()(); // Thoát khỏi giao diện
                return true;
            } else {
                error_message = "Invalid click: out of menu range!"; // Click ngoài menu
                return true;
            }
        }

        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            menu->OnEvent(event); // Xử lý sự kiện mũi tên
            return true;
        }

        return false; // Không xử lý sự kiện khác
    });

    screen.Loop(main_component);

    return final_selected; // Trả về giá trị mục được chọn
}


void PlaylistManagerView::displayAllPlaylist(PlaylistManager playlistManager){
    if(!playlistManager.checkPlaylist()){
        std::cerr << "Playlist is empty.\n";
        return;
    }
    std::cout << "============All Playlist==========" << std::endl;
    for(auto playlist : playlistManager.getAllPlaylist()){
        std::cout << playlist->getName() << std::endl;
        std::cout << std::string(50, '-') << std::endl;
    }
}