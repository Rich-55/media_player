#include "../../include/View/ScanView.h"

ScanView::ScanView() {}

int ScanView::showMenu() {
    showFileAdded(); 

    std::vector<std::string> menu_entries = {
        "1. Scan Home Directory",
        "2. Scan USB Devices",
        "0. Back to Main Menu"
    };

    std::vector<int> logic_mapping = {1, 2, 0}; 

    int selected = 0; 
    std::string error_message; 
    int final_selected = -1; 

    auto menu = Menu(&menu_entries, &selected);
    auto screen = ScreenInteractive::TerminalOutput();

    auto main_component = Renderer(menu, [&] {
        return vbox({
            text("=========== Menu Scan View ===========") | bold | center,
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

int ScanView::showListFolder(std::vector<std::string> listFolder) {
    if (listFolder.empty()) {
        std::cout << "No folders found.\n";
        return -1;
    }

    int selected_index = 0;
    std::string error_message;
    int final_selected = -1;
    std::string input_buffer;

    std::vector<std::string> menu_entries;
    for (size_t i = 0; i < listFolder.size(); ++i) {
        menu_entries.push_back(std::to_string(i + 1) + ". " + listFolder[i]);
    }
    menu_entries.push_back("0. Exit"); 

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
            if (!input_buffer.empty()) {
                try {
                    int number = std::stoi(input_buffer); 
                    if (number == 0) {
                        final_selected = -1;
                    } else if (number > 0 && number <= (int)listFolder.size()) {
                        final_selected = number - 1;
                    } else {
                        error_message = "Invalid input: number not in menu!";
                    }
                } catch (const std::exception &) {
                    error_message = "Invalid input: please enter a valid number!";
                }
                input_buffer.clear();
                screen.ExitLoopClosure()();
                return true;
            }

            if (selected_index == (int)listFolder.size()) {
                final_selected = -1; 
            } else {
                final_selected = selected_index; 
            }
            screen.ExitLoopClosure()();
            return true;
        }

        if (event.is_character()) {
            input_buffer += event.character(); 
            return true;
        }

        if (event == Event::Backspace && !input_buffer.empty()) {
            input_buffer.pop_back();
            return true;
        }

        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3; 
            if (clicked_index >= 0 && clicked_index < (int)menu_entries.size()) {
                if (clicked_index == (int)listFolder.size()) {
                    final_selected = -1; 
                } else {
                    final_selected = clicked_index; 
                }
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

int ScanView::showListUSBName(std::vector<std::string> usbDevices) {
    if (usbDevices.empty()) {
        std::cout << "No USB devices found.\n";
        return -1; 
    }

    int selected_index = 0; 
    std::string error_message;
    int final_choice = -1; 

    std::vector<std::string> menu_entries;
    for (size_t i = 0; i < usbDevices.size(); ++i) {
        menu_entries.push_back(std::to_string(i + 1) + ". " + usbDevices[i]);
    }
    menu_entries.push_back("0. Exit"); 

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
                final_choice = -1;
            } else {
                final_choice = selected_index;
            }
            screen.ExitLoopClosure()();
            return true;
        }

        if (event.is_character() && std::isdigit(event.character()[0])) {
            int number = event.character()[0] - '0';
            if (number == 0) {
                final_choice = -1;
                screen.ExitLoopClosure()();
                return true;
            } else if (number >= 1 && number <= (int)usbDevices.size()) {
                final_choice = number - 1;
                screen.ExitLoopClosure()();
                return true;
            } else {
                error_message = "Invalid input: number not in range!";
                return true;
            }
        }

        if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
            int clicked_index = event.mouse().y - 3;
            if (clicked_index >= 0 && clicked_index < (int)menu_entries.size()) {
                if (clicked_index == (int)usbDevices.size()) {
                    final_choice = -1;
                } else {
                    final_choice = clicked_index;
                }
                screen.ExitLoopClosure()();
                return true;
            } else {
                error_message = "Invalid click: out of menu range!";
                return true;
            }
        }

        if (event == Event::ArrowUp || event == Event::ArrowDown) {
            menu->OnEvent(event);
            error_message.clear();
            return true;
        }

        return false;
    });

    screen.Loop(main_component);

    return final_choice; 
}

void ScanView::setListPathNameIsAdded(std::unordered_set<std::string> listPathName)
{
    this->listPathNameIsAdded = listPathName;
}

void ScanView::showFileAdded()
{
    if (this->listPathNameIsAdded.empty()) {
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




