#include "../../include/View/BaseView.h"

BaseView::BaseView(){}
        
void BaseView::showNotificationMessage(std::string message, std::string type) {

    Color message_color = (type == "success") ? Color::Green : Color::Red;
    std::string title = (type == "success") ? "Success!" : "Error!";

    auto notification_message = vbox({
        text(title) | bold | color(message_color) | center,
        separator(),
        text(message) | center,
    }) | border | center;

    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(notification_message));
    Render(screen, notification_message);
    screen.Print();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    screen.Clear();
    std::cout << "\033[2J\033[H"; 
}

bool BaseView::showConfirmMenu(std::string message)
{
    int selected_index = 0;
    bool confirmed = false;

    std::vector<std::string> options = {"Yes", "No"};
    auto menu = Menu(&options, &selected_index);

    auto screen = ScreenInteractive::TerminalOutput();

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

    main_component = CatchEvent(main_component, [&](Event event) {
        if (event == Event::Return) {
            confirmed = (selected_index == 0); 
            screen.ExitLoopClosure()();
            return true;
        }

        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int menu_start_y = 5;  
            int menu_height = options.size(); 

            int clicked_index = event.mouse().y - menu_start_y;  

           
            std::cout << "Mouse clicked at: (" << event.mouse().x << ", " << event.mouse().y << ")\n";
            std::cout << "Menu start y: " << menu_start_y << ", clicked index: " << clicked_index << "\n";

            if (clicked_index >= 0 && clicked_index < menu_height) {
                selected_index = clicked_index;
                confirmed = (selected_index == 0); 
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

int BaseView::showMenuWithPlayer(MediaFileManager, std::shared_ptr<PlayerController>&, std::string , std::string&){return -999;}

//view for media file manager
std::string BaseView::displayAllMediaFile(MediaFileManager){return "";}
std::string BaseView::displayAllMediaFileOfAudio(MediaFileManager){return "";}
std::string BaseView::displayAllMediaFileOfVideo(MediaFileManager){return "";}
int BaseView::showMenuWithMediaList(MediaFileManager){return -1;}

//view for metadata file
void BaseView::displayDetailMediaFile(std::shared_ptr<MediaFile>, std::string){}
std::pair<std::string, std::string> BaseView::displayMenuAddMetadata(std::shared_ptr<MediaFile>, std::string){return {};}
std::pair<std::string, std::string> BaseView::displayMenuEditMetadata(std::shared_ptr<MediaFile>, std::string){return {};}
std::string BaseView::displayMenuDeleteMetadata(std::shared_ptr<MediaFile>){return "";}
//view for playlist handler
std::string BaseView::displayAllMediaFileInPlaylist(std::shared_ptr<Playlist>){return "";}
int BaseView::showMenuWithMediaListInPlaylist(std::shared_ptr<Playlist>){return -1;}
std::pair<std::string, std::string> BaseView::displayAllFolder(std::pair<std::unordered_set<std::string> , std::unordered_set<std::string> >){return {};}
//view for playlist manager
int BaseView::showMenuWithPlaylist(std::vector<std::shared_ptr<Playlist>>){return -1;}
std::string BaseView::showMenuCreatePlaylist(){return "";}
std::string BaseView::displayAllPlaylist(PlaylistManager){return "";}

//view for scan manager
int BaseView::showListFolder(std::vector<std::string>){return -1;}
int BaseView::showListUSBName(std::vector<std::string>) {return -1;}
void BaseView::setListPathNameIsAdded(std::unordered_set<std::string>){}
void BaseView::showFileAdded(){}

BaseView::~BaseView(){}
