#include "../../include/View/MainMenuView.h"
#include <iostream>


MainMenuView::MainMenuView() {}


int MainMenuView::showMenu() {return -1;}

int MainMenuView::showMenuWithPlayer(MediaFileManager mediaFileManager, std::shared_ptr<PlayerController>& playerController, 
    std::string typePlay, std::string& error) {
    
    static bool clear_required = true; 
    int volume = playerController ? playerController->getVolume() : 100;
    bool is_repeat = playerController ? playerController->isRepeat() : false;
    bool is_pause = playerController ? playerController->isPause() : true;
    bool is_stoped = false;
    int selected = 0;

    const int rows_per_page = 25; 
    const int scroll_visible_rows = 8;
    int current_page = 0;       
    int total_pages = playerController ? std::ceil((double)playerController->getMediaFiles().size() / rows_per_page) : 1;
    int scroll_offset = 0;
    int current_index = -1;

    int button_result = -1;
    std::string error_message;


    if (!error.empty()) {
        error_message = error; 
        error = "";
        
        std::thread([&] {
            std::this_thread::sleep_for(std::chrono::seconds(5)); 
            error_message = ""; 
            ScreenInteractive::Active()->PostEvent(Event::Custom); 
        }).detach(); 
    }
        

    if (clear_required) {
        system("clear");
        clear_required = false;
    }

    if (playerController) {
        playerController->addObserverIndex([&](int newIndex) {
            current_index = newIndex;
            ScreenInteractive::Active()->PostEvent(Event::Custom); 
        });
        static bool observer_added = false;
        if (!observer_added) {
            playerController->addObserverState([&] {
                ScreenInteractive::Active()->PostEvent(Event::Custom); 
            });
            observer_added = true;
        }
    }
    
    std::shared_ptr<MediaFile> media = nullptr;
    if (typePlay == "single") {
        std::string pathName = playerController->getMediaFiles()[0];
        media = mediaFileManager.getMediaFileByPath(pathName);
    }

    if(typePlay == "playlist") {
        current_index = playerController->getCurrentIndex();
    }


    std::vector<std::string> menu_entries = {
        "1. MetadataFile Handler",
        "2. MediaFile Manager",
        "3. Playlist Handler",
        "4. Playlist Manager",
        "5. Play Music",
        "6. Play Playlist",
        "7. Play Video",
        "0. Back to main menu"
    };

    auto menu = Menu(&menu_entries, &selected);

    auto play_button = Button("Play", [&] {
        if(typePlay == "noplay"){
            error_message = "No media files to play.";
        }else if (playerController ) {
            playerController->togglePlayback();
            is_pause = playerController->isPause(); 
            is_stoped = false;
            ScreenInteractive::Active()->PostEvent(Event::Custom);
        }
    });

    auto stop_button = Button("Stop", [&] {
        if(typePlay == "noplay"){
            error_message = "No media files to play.";
        }else if (playerController) {
            playerController->stop();
            is_stoped = true;
            ScreenInteractive::Active()->PostEvent(Event::Custom);
        }
    });

    auto repeat_button = Button("Repeat", [&] {
        if(typePlay == "noplay"){
            error_message = "No media files to play.";
        }else if (playerController) {
            playerController->toggleRepeat();
            is_repeat = playerController->isRepeat();
            ScreenInteractive::Active()->PostEvent(Event::Custom);
        }
    });

    auto next_button = Button("Next", [&] {
        if(typePlay == "noplay"){
            error_message = "No media files to play.";
        }else if (typePlay == "single" ) {
            error_message = "Cannot use 'Next' while playing a single track!";
        } else if (playerController) {
            playerController->playNext();
            error_message.clear(); 
            ScreenInteractive::Active()->PostEvent(Event::Custom);
        }
    });

    auto previous_button = Button("Previous", [&] {
        if(typePlay == "noplay"){
            error_message = "No media files to play.";
        }else if (typePlay == "single") {
            error_message = "Cannot use 'Previous' while playing a single track!";
        }else if (playerController) {
            playerController->playPrevious();
            error_message.clear(); 
            ScreenInteractive::Active()->PostEvent(Event::Custom);
        }
    });

    auto volume_up_button = Button("+ Volume", [&] {
        if (playerController) {
            playerController->increaseVolume(10);
            volume = playerController->getVolume(); 
            ScreenInteractive::Active()->PostEvent(Event::Custom);
        }
    });

    auto volume_down_button = Button("- Volume", [&] {
        if (playerController) {
            playerController->decreaseVolume(10);
            volume = playerController->getVolume(); 
            ScreenInteractive::Active()->PostEvent(Event::Custom);
        }
    });

    auto button_container = Container::Horizontal({
        play_button, stop_button, repeat_button, next_button, previous_button, volume_up_button, volume_down_button
    });

    auto media_info_renderer = Renderer([&] {
        std::vector<Element> metadata_elements;

        std::string music_status;
        
        if (playerController) {
            if (playerController->isPlaying()) {
                music_status = playerController->isPause() ? "Paused" : "Playing";
            } else {
                music_status = "Stopped";
            }
        } else {
            music_status = "No PlayerController";
        }
        std::string duration = std::to_string(playerController ? playerController->getDuration() : 0) + "s";

        
        if (typePlay == "noplay") {
            return vbox({
                text("=== Music Status ===") | bold | center,
                text("No music is playing.") | center,
                separator(),
                text("Volume: " + std::to_string(volume)) | center,
                separator(),
                text(error_message) | color(Color::Red) | center 
            }) | border;
        } else if (typePlay == "single" && media) {
            for (const auto& [key, value] : media->getAllMetadata()) {
                metadata_elements.push_back(text(key + ": " + value));
            }

            return vbox({
                text("=== Music Status ===") | bold | center,
                text("Currently playing: " + media->getName()) | center,
                text("Current State: " + music_status) | center,
                text("Repeat: " + std::string(is_repeat ? "ON" : "OFF")) | center,
                text("Duration: " + duration + "/" + media->getDuration()) | center,
                separator(),
                text("Volume: " + std::to_string(volume)) | center,
                separator(),
                text("Media Info:") | bold,
                vbox(std::move(metadata_elements)),
                separator(),
                text(error_message) | color(Color::Red) | center 
            }) | border;
        }else if (typePlay == "playlist") {
            std::vector<ftxui::Element> rows;
                rows.push_back(ftxui::hbox({
                    text("STT") | bold | size(WIDTH, EQUAL, 5) | border,
                    text("File Name") | bold | size(WIDTH, EQUAL, 120) | border,
                    text("Duration") | bold | size(WIDTH, EQUAL, 20) | border,
                }));

                int start_index = current_page * rows_per_page;
                int end_index = std::min(start_index + rows_per_page, (int)playerController->getMediaFiles().size());

                for (int i = start_index + scroll_offset;
                    i < std::min(start_index + scroll_offset + scroll_visible_rows, end_index);
                    ++i) {
                    bool is_current = (i == current_index); 
                    rows.push_back(ftxui::hbox({
                        text(std::to_string(i + 1)) | size(WIDTH, EQUAL, 5) | border,
                        text(playerController->getMediaFiles()[i]) | size(WIDTH, EQUAL, 120) | border,
                        text(mediaFileManager.getMediaFileByPath(playerController->getMediaFiles()[i])->getDuration()) | size(WIDTH, EQUAL, 20) | border,
                    }) | (is_current ? bgcolor(Color::Green) : nothing)); 
                }
            

            return ftxui::vbox({
                text("=== Music Status ===") | bold | center,
                text("Current State: " + music_status) | center,
                text("Repeat: " + std::string(is_repeat ? "ON" : "OFF")) | center,
                text("Volume: " + std::to_string(volume)) | center,
                separator(),
                text("Currently Playing: " + (current_index >= 0 && current_index < (int)playerController->getMediaFiles().size() ? playerController->getMediaFiles()[current_index] : "None")) | center | color(Color::Yellow),
                text("Duration: " + duration + "/" + mediaFileManager.getMediaFileByPath(playerController->getMediaFiles()[current_index])->getDuration()) | center,
                separator(),
                text("----- All Media File -----") | ftxui::bold | ftxui::center | ftxui::color(ftxui::Color::Blue),
                text("Page " + std::to_string(current_page + 1) + " of " + std::to_string(total_pages)) | ftxui::center,
                ftxui::separator(),
                ftxui::vbox(std::move(rows)) | ftxui::vscroll_indicator | ftxui::frame | ftxui::border,
                separator(),
                text(error_message) | color(Color::Red) | center 
            });
        }

        return vbox({
            text("=== Music Status ===") | bold | center,
            text("No valid playback type.") | center,
            separator(),
            text("Volume: " + std::to_string(volume)) | center,
            separator(),
            text(error_message) | color(Color::Red) | center 
        }) | border;
    });

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

    auto final_component = CatchEvent(main_component, [&](Event event) {
        if (event == Event::Return) {
            if (selected == static_cast<int>(menu_entries.size() - 1)) {
                button_result = 0;
            } else {
                button_result = selected + 1;
            }
            screen.ExitLoopClosure()();
            return true;
        }

        if (event.is_character() && std::isdigit(event.character()[0])) {
            int number = event.character()[0] - '0';
            if (number >= 0 && number < static_cast<int>(menu_entries.size())) {
                button_result = number == 0 ? 0 : number;
                screen.ExitLoopClosure()();
                return true;
            }
        }

        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            menu->OnEvent(event);
            return true;
        }

        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3;
            if (clicked_index >= 0 && clicked_index < static_cast<int>(menu_entries.size())) {
                button_result = clicked_index == static_cast<int>(menu_entries.size() - 1) ? 0 : clicked_index + 1;
                screen.ExitLoopClosure()();
                return true;
            }
        }

        if (event == Event::ArrowLeft) {
            if (current_page > 0) {
                --current_page;
                scroll_offset = 0;
            }
            return true;
        }

        if (event == Event::ArrowRight) {
            if (current_page < total_pages - 1) {
                ++current_page;
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
            int max_scroll = std::min(rows_per_page, (int)mediaFileManager.getAllMediaFile().size() - current_page * rows_per_page);
            if (scroll_offset + scroll_visible_rows < max_scroll) {
                ++scroll_offset;
            }
            return true;
        }

        return false;
    });

    screen.Loop(final_component);

    return button_result;
}
