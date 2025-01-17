#include "../../include/View/BaseView.h"

BaseView::BaseView(){}
        
void BaseView::showSuccessMessage(std::string message)
{
    system("clear");
    auto success_message = vbox({
        text("Success!") | bold | color(Color::Green) | center,
        separator(),
        text(message) | center,
    }) | border | center;

    // Tạo màn hình và hiển thị thông báo
    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(success_message));
    Render(screen, success_message);
    screen.Print(); // In giao diện ra console
}

bool BaseView::showConfirmMenu(std::string message)
{
    // Chỉ mục được chọn
    int selected_index = 0;
    bool confirmed = false;

    // Tùy chọn "Yes" và "No"
    std::vector<std::string> options = {"Yes", "No"};
    auto menu = Menu(&options, &selected_index);

    auto screen = ScreenInteractive::TerminalOutput();

    // Tạo giao diện xác nhận
    auto main_component = Renderer(menu, [&] {
        return vbox({
            text("Confirm Action") | bold | center,
            separator(),
            text(message) | center | color(Color::Red),
            separator(),
            menu->Render() | border | center,
            separator(),
            text("Use UP/DOWN keys or click to select. Press ENTER to confirm.") | dim | center,
        });
    });

    // Xử lý sự kiện
    main_component = CatchEvent(main_component, [&](Event event) {
        if (event == Event::Return) {
            // Xác nhận lựa chọn
            confirmed = (selected_index == 0); // Yes -> true, No -> false
            screen.ExitLoopClosure()();
            return true;
        }

        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            // Tính toán vùng menu dựa trên dòng nơi menu bắt đầu hiển thị
            int menu_start_y = 5;  // Số dòng từ đầu giao diện đến menu
            int menu_height = options.size();  // Số mục trong menu

            int clicked_index = event.mouse().y - menu_start_y;  // Tính chỉ mục được click

            // Debug để kiểm tra tọa độ chuột
            std::cout << "Mouse clicked at: (" << event.mouse().x << ", " << event.mouse().y << ")\n";
            std::cout << "Menu start y: " << menu_start_y << ", clicked index: " << clicked_index << "\n";

            // Kiểm tra nếu click trong phạm vi menu
            if (clicked_index >= 0 && clicked_index < menu_height) {
                selected_index = clicked_index;
                confirmed = (selected_index == 0); // Yes -> true, No -> false
                screen.ExitLoopClosure()();
                return true;
            }
        }

        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            menu->OnEvent(event);
            return true;
        }

        return false;
    });

    screen.Loop(main_component);

    return confirmed;
}

std::string BaseView::displayAllMediaFile(MediaFileManager){return "";}

std::string BaseView::displayAllMediaFileOfAudio(MediaFileManager){return "";}

std::string BaseView::displayAllMediaFileOfVideo(MediaFileManager){return "";}

void BaseView::displayDetailMediaFile(std::shared_ptr<MediaFile>, std::string){}
std::pair<std::string, std::string> BaseView::displayMenuAddMetadata(std::shared_ptr<MediaFile>, std::string){return {};}
std::pair<std::string, std::string> BaseView::displayMenuEditMetadata(std::shared_ptr<MediaFile>, std::string){return {};}
std::string BaseView::displayMenuDeleteMetadata(std::shared_ptr<MediaFile>){return "";}

void BaseView::displayAllPlaylist(PlaylistManager){}
void BaseView::displayAllMediaFileInPlaylist(std::shared_ptr<Playlist>){}

int BaseView::showListFolder(std::vector<std::string>){return -1;}
int BaseView::showListUSBName(std::vector<std::string>) {return -1;}
void BaseView::setListPathNameIsAdded(std::unordered_set<std::string>){}
void BaseView::showFileAdded(){}



BaseView::~BaseView(){}
