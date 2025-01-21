#include "../../include/View/MediaFileHandlerView.h"

MediaFileHandlerView::MediaFileHandlerView(){}

int MediaFileHandlerView::showMenu() 
{
    std::vector<std::string> menu_entries = {
        "1. Add metadata",
        "2. Edit metadata",
        "3. Delete metadata",
        "0. Back to main menu"
    };

    std::vector<int> logic_mapping = {1, 2, 3, 0};

    int selected = 0; 
    std::string error_message; 
    int final_selected = -1; 

    auto menu = Menu(&menu_entries, &selected);

    auto screen = ScreenInteractive::TerminalOutput();

    auto main_component = Renderer(menu, [&] {
        return vbox({
            text("========== Media File Menu ==========") | bold | center,
            separator(),
            menu->Render() | border, 
            separator(),
            text("Use UP/DOWN keys, numbers (0-9), or click to navigate. Press ENTER to select.") | dim | center,
            separator(),
            text(error_message) | color(Color::Red) | center 
        });
    });

    main_component = CatchEvent(main_component, [&](Event event) {
        if (event == Event::Return) {
            final_selected = logic_mapping[selected]; 
            screen.ExitLoopClosure()(); 
            return true;
        }

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

        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3; 
            if (clicked_index >= 0 && clicked_index < (int)menu_entries.size()) {
                final_selected = logic_mapping[clicked_index]; 
                screen.ExitLoopClosure()(); 
                return true;
            } else {
                error_message = "Invalid click: out of menu range!"; 
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

    return final_selected; 
}

void MediaFileHandlerView::showNotificationMessage(std::string message, std::string type){BaseView::showNotificationMessage(message, type);}

bool MediaFileHandlerView::showConfirmMenu(std::string message){return BaseView::showConfirmMenu(message);}

void MediaFileHandlerView::displayDetailMediaFile(std::shared_ptr<MediaFile> mediaFile, std::string message) {
    system("clear");

    auto general_info = vbox({
        hbox({ text("File Name: ") | bold, text(mediaFile->getName()) }),
        hbox({ text("File Path: ") | bold, text(mediaFile->getPath()) }),
        hbox({ text("File Type: ") | bold, text(mediaFile->getType()) }),
        hbox({ text("File Size: ") | bold, text(std::to_string(mediaFile->getSize()) + " bytes") }),
        hbox({ text("File Duration: ") | bold, text(mediaFile->getDuration()) }),
        hbox({ text("File Date Created: ") | bold, text(mediaFile->getDateCreated()) }),
    }) | border | color(Color::White);

    Elements metadata_elements;
    for (const auto& [key, value] : mediaFile->getAllMetadata()) {
        metadata_elements.push_back(hbox({ text(key + ": ") | bold, text(value) }));
    }

    auto metadata_info = vbox(std::move(metadata_elements)) | border | color(Color::Green);

    auto metadata_header = hbox({
        text((mediaFile->getType() == "Audio") ? "Audio Metadata:" : "Video Metadata:") | bold | color(Color::Yellow)
    });

    Elements layout_elements = {
        text("Media File Details") | bold | center | color(Color::Cyan),
        separator(),
        general_info,
        separator(),
        metadata_header,
        metadata_info,
    };

    if (!message.empty()) {
        layout_elements.push_back(separator());
        layout_elements.push_back(text(message) | bold | hcenter | color(Color::Green));
    }

    auto layout = vbox(std::move(layout_elements)) | border;

    auto centered_layout = vbox({
        hbox({ filler(), layout | hcenter, filler() }),
        filler(),
    }) | vcenter;

    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(centered_layout));
    Render(screen, centered_layout);
    screen.Print(); 
}

std::pair<std::string, std::string> MediaFileHandlerView::displayMenuAddMetadata(
    std::shared_ptr<MediaFile> mediaFile, std::string exception) 
    {
    std::string selected_key;
    int selected_index = 0;
    std::string error_message;
    std::string success_message;

    if (!exception.empty()) {
        error_message = "Error: " + exception;
    }

    std::vector<std::string> keys = {
        "title", "artist", "album", "genre", "comment", "year", "track", "Exit"};  

    auto menu = Menu(&keys, &selected_index);
    auto screen = ScreenInteractive::TerminalOutput();

    std::pair<std::string, std::string> result;  

    auto main_component = Renderer(menu, [&] {
        return vbox({
            text("Add new key to media file: ") | bold | center,
            text(mediaFile->getName()) | bold | center | color(Color::Blue),
            separator(),
            text("List of keys available in taglib:") | dim | center,
            menu->Render() | border | center,
            separator(),
            text(error_message) | color(Color::Red) | hcenter,
            text(success_message) | color(Color::Green) | hcenter,
            separator(),
            text("Use UP/DOWN to navigate, ENTER to select.") | dim | center,
        });
    });

    main_component = CatchEvent(main_component, [&](Event event) {
    if (event == Event::Return) {
        if (selected_index >= 0 && selected_index < (int)keys.size()) {
            selected_key = keys[selected_index];

            if (selected_key == "Exit") {
                result = std::make_pair("0", "");
                screen.ExitLoopClosure()();
                return true;
            }

            if (!mediaFile->getMetadata(selected_key).empty()) {
                error_message = "Error: Key '" + selected_key + "' already exists.";
                success_message.clear();
            } else {
                error_message.clear();

                std::string value;
                auto input_component = Renderer([&] {
                    return vbox({
                        text("Enter value for key: " + selected_key) | bold | center,
                        separator(),
                        text("Value: ") | dim | hcenter,
                        text(value) | hcenter,
                        separator(),
                        text("Press ENTER to confirm.") | dim | center,
                    });
                });

                screen.Loop(CatchEvent(input_component, [&](Event event) {
                    if (event == Event::Return) {
                        if (!value.empty()) {
                            result = std::make_pair(selected_key, value);  
                            screen.ExitLoopClosure()();  
                            return true;
                        } else {
                            error_message = "Error: Value cannot be empty.";
                        }
                    } else if (event.is_character()) {
                        value += event.character();
                    } else if (event == Event::Backspace && !value.empty()) {
                        value.pop_back();
                    }
                    return false;
                }));
                screen.ExitLoopClosure()(); 
                return true;
            }
        }
    }

    if (event.is_mouse() && event.mouse().button == Mouse::Left &&
        event.mouse().motion == Mouse::Pressed) {
        int menu_start_y = 5;
        int clicked_index = event.mouse().y - menu_start_y;

        if (clicked_index >= 0 && clicked_index < (int)keys.size()) {
            selected_index = clicked_index;
            selected_key = keys[selected_index];

            if (selected_key == "Exit") {
                result = std::make_pair("0", "");
                screen.ExitLoopClosure()();
                return true;
            }

            if (!mediaFile->getMetadata(selected_key).empty()) {
                error_message = "Error: Key '" + selected_key + "' already exists.";
                success_message.clear();
            } else {
                error_message.clear();

                std::string value;
                auto input_component = Renderer([&] {
                    return vbox({
                        text("Enter value for key: " + selected_key) | bold | center,
                        separator(),
                        text("Value: ") | dim | hcenter,
                        text(value) | hcenter,
                        separator(),
                        text("Press ENTER to confirm.") | dim | center,
                    });
                });

                screen.Loop(CatchEvent(input_component, [&](Event event) {
                    if (event == Event::Return) {
                        if (!value.empty()) {
                            result = std::make_pair(selected_key, value); 
                            screen.ExitLoopClosure()(); 
                            return true;
                        } else {
                            error_message = "Error: Value cannot be empty.";
                        }
                    } else if (event.is_character()) {
                        value += event.character();
                    } else if (event == Event::Backspace && !value.empty()) {
                        value.pop_back();
                    }
                    return false;
                }));
                screen.ExitLoopClosure()(); 
                return true;
            }
        } else {
            error_message = "Error: Clicked outside valid options.";
            success_message.clear();
        }
    }

    if (event == Event::ArrowUp || event == Event::ArrowDown) {
        if (menu->OnEvent(event)) { 
            error_message.clear();
            success_message.clear();
        }
        return true; 
    }

    return false;
});


    screen.Loop(main_component);

    return result; 
}

std::pair<std::string, std::string> MediaFileHandlerView::displayMenuEditMetadata(
    std::shared_ptr<MediaFile> mediaFile, std::string exception) 
    {

    std::string selected_key;
    int selected_index = 0;
    std::string error_message;

    if (!exception.empty()) {
        error_message = "Error: " + exception;
    }

    std::vector<std::string> keys;
    for (const auto& [key, value] : mediaFile->getAllMetadata()) {
        keys.push_back(key);
    }
    keys.push_back("Exit");

    auto menu = Menu(&keys, &selected_index);
    auto screen = ScreenInteractive::TerminalOutput();

    std::pair<std::string, std::string> result;

    auto main_component = Renderer(menu, [&] {
        return vbox({
            text("Edit metadata for: ") | bold | center,
            text(mediaFile->getName()) | bold | center | color(Color::Blue),
            separator(),
            text("Select a key to edit:") | dim | center,
            menu->Render() | border | center,
            separator(),
            text(error_message) | color(Color::Red) | hcenter,
            separator(),
            text("Use UP/DOWN to navigate, ENTER to select, or CLICK to choose.") | dim | center,
        });
    });

    main_component = CatchEvent(main_component, [&](Event event) {
    if (event == Event::Return) {
        if (selected_index >= 0 && selected_index < (int)keys.size()) {
            selected_key = keys[selected_index];

            if (selected_key == "Exit") {
                result = std::make_pair("0", "");
                screen.ExitLoopClosure()();
                return true;
            }

            if (mediaFile->getMetadata(selected_key).empty()) {
                error_message = "Error: Key '" + selected_key + "' does not exist.";
            } else {
                error_message.clear();

                std::string value = mediaFile->getMetadata(selected_key);
                auto input_component = Renderer([&] {
                    return vbox({
                        text("Editing key: " + selected_key) | bold | center,
                        separator(),
                        text("Current value: " + value) | dim | hcenter,
                        separator(),
                        text("Enter new value: ") | dim | hcenter,
                        text(value) | hcenter,
                        separator(),
                        text("Press ENTER to confirm.") | dim | center,
                    });
                });

                screen.Loop(CatchEvent(input_component, [&](Event event) {
                    if (event == Event::Return) {
                        if (!value.empty()) {
                            result = std::make_pair(selected_key, value); 
                            screen.ExitLoopClosure()();
                            return true;
                        } else {
                            error_message = "Error: Value cannot be empty.";
                        }
                    } else if (event.is_character()) {
                        value += event.character();
                    } else if (event == Event::Backspace && !value.empty()) {
                        value.pop_back();
                    }
                    return false;
                }));
                screen.ExitLoopClosure()(); 
                return true;
            }
        }
    }

    if (event == Event::ArrowUp || event == Event::ArrowDown) {
        if (menu->OnEvent(event)) { 
            error_message.clear();
        }
        return true; 
    }

    if (event.is_mouse() && event.mouse().button == Mouse::Left &&
        event.mouse().motion == Mouse::Pressed) {
        int menu_start_y = 5; 
        int clicked_index = event.mouse().y - menu_start_y;

        if (clicked_index >= 0 && clicked_index < (int)keys.size()) {
            selected_index = clicked_index;
            selected_key = keys[selected_index];

            if (selected_key == "Exit") {
                result = std::make_pair("0", "");
                screen.ExitLoopClosure()();
                return true;
            }

            if (mediaFile->getMetadata(selected_key).empty()) {
                error_message = "Error: Key '" + selected_key + "' does not exist.";
            } else {
                error_message.clear();

                std::string value = mediaFile->getMetadata(selected_key);
                auto input_component = Renderer([&] {
                    return vbox({
                        text("Editing key: " + selected_key) | bold | center,
                        separator(),
                        text("Current value: " + value) | dim | hcenter,
                        separator(),
                        text("Enter new value: ") | dim | hcenter,
                        text(value) | hcenter,
                        separator(),
                        text("Press ENTER to confirm.") | dim | center,
                    });
                });

                screen.Loop(CatchEvent(input_component, [&](Event event) {
                    if (event == Event::Return) {
                        if (!value.empty()) {
                            result = std::make_pair(selected_key, value);  
                            screen.ExitLoopClosure()();
                            return true;
                        } else {
                            error_message = "Error: Value cannot be empty.";
                        }
                    } else if (event.is_character()) {
                        value += event.character();
                    } else if (event == Event::Backspace && !value.empty()) {
                        value.pop_back();
                    }
                    return false;
                }));
                screen.ExitLoopClosure()(); 
                return true;
            }
        } else {
            error_message = "Error: Clicked outside valid options.";
        }
    }

    return false;
});


    screen.Loop(main_component);

    return result;
}

std::string MediaFileHandlerView::displayMenuDeleteMetadata(std::shared_ptr<MediaFile> mediaFile) 
{
    std::string selected_key;
    int selected_index = 0;
    std::string error_message;

    std::vector<std::string> keys;
    for (const auto& [key, value] : mediaFile->getAllMetadata()) {
        keys.push_back(key);
    }
    keys.push_back("Exit");

    auto menu = Menu(&keys, &selected_index);
    auto screen = ScreenInteractive::TerminalOutput();

    std::string result;

    auto main_component = Renderer(menu, [&] {
        return vbox({
            text("Delete metadata key for: ") | bold | center,
            text(mediaFile->getName()) | bold | center | color(Color::Blue),
            separator(),
            text("Select a key to delete:") | dim | center,
            menu->Render() | border | center,
            separator(),
            text(error_message) | color(Color::Red) | hcenter,
            separator(),
            text("Use UP/DOWN to navigate, ENTER to select, or CLICK to choose.") | dim | center,
        });
    });

    main_component = CatchEvent(main_component, [&](Event event) {
        if (event == Event::Return) {
            if (selected_index >= 0 && selected_index < (int)keys.size()) {
                selected_key = keys[selected_index];

                if (selected_key == "Exit") {
                    result = "0";
                    screen.ExitLoopClosure()();
                    return true;
                }

                if (mediaFile->getMetadata(selected_key).empty()) {
                    error_message = "Error: Key '" + selected_key + "' does not exist.";
                } else {
                    error_message.clear();
                    result = selected_key; 
                    screen.ExitLoopClosure()();
                    return true;
                }
            }
        }

        if (event.is_mouse() && event.mouse().button == Mouse::Left &&
            event.mouse().motion == Mouse::Pressed) {
            int menu_start_y = 5; 
            int clicked_index = event.mouse().y - menu_start_y;

            if (clicked_index >= 0 && clicked_index < (int)keys.size()) {
                selected_index = clicked_index;
                selected_key = keys[selected_index];

                if (selected_key == "Exit") {
                    result = "0";
                    screen.ExitLoopClosure()();
                    return true;
                }

                if (mediaFile->getMetadata(selected_key).empty()) {
                    error_message = "Error: Key '" + selected_key + "' does not exist.";
                } else {
                    error_message.clear();
                    result = selected_key;
                    screen.ExitLoopClosure()();
                    return true;
                }
            } else {
                error_message = "Error: Clicked outside valid options.";
            }
        }

        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            if (menu->OnEvent(event)) {
                error_message.clear();
            }
            return true;
        }

        return false;
    });

    screen.Loop(main_component);

    return result;
}
