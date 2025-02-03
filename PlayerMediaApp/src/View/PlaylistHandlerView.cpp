#include "../../include/View/PlaylistHandlerView.h"

PlaylistHandlerView::PlaylistHandlerView() {}

void PlaylistHandlerView::showNotificationMessage(std::string message,
                                                  std::string type) {
  BaseView::showNotificationMessage(message, type);
}

int PlaylistHandlerView::showMenu() { return -1; }

bool PlaylistHandlerView::showConfirmMenu(std::string message) {
  return BaseView::showConfirmMenu(message);
}

std::string PlaylistHandlerView::showMenuCreatePlaylist() {

  std::string input = "";
  std::string error_message = "";
  auto isValidName = [](const std::string &name) {
    std::regex valid_regex("^[a-zA-Z0-9 _-]+$");
    return std::regex_match(name, valid_regex);
  };
  auto screen = ScreenInteractive::TerminalOutput();

  auto input_box = Input(&input, "Enter playlist name...");

  auto confirm_button = Button("Confirm", [&] {
    if (input.empty()) {
      error_message = "Error: Playlist name cannot be empty!";
    } else if (input == "0") {
      screen.ExitLoopClosure()();
      input = "exit";
    } else if (!isValidName(input)) {
      error_message = "Error: Playlist name contains invalid characters!";
    } else {
      error_message = "";
      screen.ExitLoopClosure()();
    }
  });

  auto cancel_button = Button("Cancel (Return 0)", [&] {
    input = "exit";
    screen.ExitLoopClosure()();
  });

  auto layout = Renderer(
      Container::Vertical({
          input_box,
          confirm_button,
          cancel_button,
      }),
      [&] {
        return vbox({
            text("Create New Playlist") | bold | center,
            separator(),
            text("Enter the name of your playlist or press '0' to cancel.") |
                center,
            text("Note: Only letters, numbers, spaces, underscores, and dashes "
                 "are allowed.") |
                dim | center,
            separator(),
            hbox({
                text("Playlist Name: "),
                input_box->Render(),
            }) | center,
            separator(),
            hbox({
                confirm_button->Render(),
                text(" "),
                cancel_button->Render(),
            }) | center,
            separator(),
            text(error_message) | color(Color::Red) | center,
        });
      });

  screen.Loop(layout);

  return input;
}

int PlaylistHandlerView::showMenuWithMediaListInPlaylist(
    std::shared_ptr<Playlist> playlist) {
  std::vector<std::string> menu_entries = {
      "1. Add MediaFile", 
      "2. Add MediaFile By Folder", 
      "3. Delete MediaFile",
      "4. Rename Playlist",
      "0. Back to main menu"
  };

  std::vector<int> logic_mapping = {1, 2, 3, 4, 0};
  int selected = 0;
  std::string error_message;
  int final_selected = -1;

  const int rows_per_page = 25;
  const int scroll_visible_rows = 10;
  int current_page = 0;
  int total_pages =
      std::ceil((double)playlist->getListMediaFiles().size() / rows_per_page);
  int scroll_offset = 0;

  auto create_table_view = [&]() {
    std::vector<ftxui::Element> rows;

    if (playlist->getListMediaFiles().empty()) {
      rows.push_back(ftxui::text("No media files in the playlist.") |
                     ftxui::bold | ftxui::center);
    } else {
      rows.push_back(ftxui::hbox({
          ftxui::text("No") | ftxui::bold |
              ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 5) | ftxui::border,
          ftxui::text("File Name") | ftxui::bold |
              ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 25) | ftxui::border,
          ftxui::text("File Type") | ftxui::bold |
              ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 10) | ftxui::border,
          ftxui::text("Title") | ftxui::bold |
              ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20) | ftxui::border,
          ftxui::text("Album") | ftxui::bold |
              ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20) | ftxui::border,
          ftxui::text("Artist") | ftxui::bold |
              ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20) | ftxui::border,
          ftxui::text("Duration") | ftxui::bold |
              ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 15) | ftxui::border,
          ftxui::text("Date Created") | ftxui::bold |
              ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20) | ftxui::border,
      }));

      int start_index = current_page * rows_per_page;
      int end_index = std::min(start_index + rows_per_page,
                               (int)playlist->getListMediaFiles().size());

      for (int i = start_index + scroll_offset;
           i < std::min(start_index + scroll_offset + scroll_visible_rows,
                        end_index);
           ++i) {
        auto mediaFile = playlist->getListMediaFiles()[i];
        rows.push_back(ftxui::hbox({
            ftxui::text(std::to_string(i + 1)) |
                ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 5) | ftxui::border,
            ftxui::text(mediaFile->getFileName()) |
                ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 25) | ftxui::border,
            ftxui::text(mediaFile->getType()) |
                ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 10) | ftxui::border,
            ftxui::text(mediaFile->getMetadata("title")) |
                ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20) | ftxui::border,
            ftxui::text(mediaFile->getMetadata("album")) |
                ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20) | ftxui::border,
            ftxui::text(mediaFile->getMetadata("artist")) |
                ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20) | ftxui::border,
            ftxui::text(mediaFile->getDuration()) |
                ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 15) | ftxui::border,
            ftxui::text(mediaFile->getDateCreated()) |
                ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20) | ftxui::border,
        }));
      }
    }

    return ftxui::vbox({
        ftxui::text("----- Media Files in Playlist: " + playlist->getPlaylistName() +
                    " -----") |
            ftxui::bold | ftxui::center | ftxui::color(ftxui::Color::Blue),
        ftxui::text("Page " + std::to_string(current_page + 1) + " of " +
                    std::to_string(total_pages)) |
            ftxui::center,
        ftxui::separator(),
        ftxui::vbox(std::move(rows)) | ftxui::vscroll_indicator | ftxui::frame |
            ftxui::border,
    });
  };

  auto menu = Menu(&menu_entries, &selected);
  auto main_component = Renderer(menu, [&]() {
    return ftxui::vbox(
        {ftxui::text("============ Playlist Handler ===========") |
             ftxui::bold | ftxui::center,
         ftxui::separator(), menu->Render() | ftxui::border, ftxui::separator(),
         create_table_view(), ftxui::separator(),
         ftxui::text("Use UP/DOWN keys to navigate menu. Use LEFT/RIGHT keys "
                     "to navigate pages.") |
             ftxui::dim | ftxui::center,
         ftxui::text(error_message) | ftxui::color(ftxui::Color::Red) |
             ftxui::center});
  });

  auto screen = ScreenInteractive::TerminalOutput();
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

    if (event == Event::ArrowUp || event == Event::ArrowDown) {
      menu->OnEvent(event);
      return true;
    }

    if (event.is_mouse() && event.mouse().button == Mouse::Left &&
        event.mouse().motion == Mouse::Pressed) {
      int clicked_index = event.mouse().y - 3;
      if (clicked_index >= 0 && clicked_index < (int)menu_entries.size()) {
        selected = clicked_index;
        final_selected = logic_mapping[selected];
        screen.ExitLoopClosure()();
        return true;
      } else {
        error_message = "Invalid click: out of menu range!";
        return true;
      }
    }

    if (event == Event::ArrowLeft) {
      if (current_page > 0) {
        --current_page;
        scroll_offset = 0;
        error_message.clear();
      } else {
        error_message = "Already on the first page!";
      }
      return true;
    }

    if (event == Event::ArrowRight) {
      if (current_page < total_pages - 1) {
        ++current_page;
        scroll_offset = 0;
        error_message.clear();
      } else {
        error_message = "Already on the last page!";
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
      int max_scroll =
          std::min(rows_per_page, (int)playlist->getListMediaFiles().size() -
                                      current_page * rows_per_page);
      if (scroll_offset + scroll_visible_rows < max_scroll) {
        ++scroll_offset;
      }
      return true;
    }

    return false;
  });

  screen.Loop(main_component);
  return final_selected;
}

std::string PlaylistHandlerView::displayAllMediaFileInPlaylist(
    std::shared_ptr<Playlist> playlist) {
  system("clear");
  try {

    auto media_files = playlist->getListMediaFiles();

    if (playlist->getListMediaFiles().empty()) {
      auto screen = ftxui::ScreenInteractive::TerminalOutput();
      auto empty_message = ftxui::Renderer([&] {
        return ftxui::vbox({
            ftxui::text("No media files in the playlist.") | ftxui::bold |
                ftxui::center,
            ftxui::text("Press any key to exit.") | ftxui::dim | ftxui::center,
        });
      });

      auto component =
          ftxui::CatchEvent(empty_message, [&](ftxui::Event event) {
            if (event.is_character() || event == ftxui::Event::Escape ||
                event == ftxui::Event::Return) {
              screen.ExitLoopClosure()();
              return true;
            }
            return false;
          });

      screen.Loop(component);
      return "exit";
    }

    const int rows_per_page = 25;
    const int scroll_visible_rows = 10;
    int current_page = 0;
    int total_pages = std::ceil((double)media_files.size() / rows_per_page);
    int selected_index = -1;
    int hovered_index = -1;
    int scroll_offset = 0;
    std::string input_buffer;
    std::string result_filename;
    bool is_exit_hovered = false;
    std::string error_message;
    auto screen = ScreenInteractive::TerminalOutput();

    auto render_view = [&]() -> Element {
      Elements table_rows;

      table_rows.push_back(hbox({
          text("STT") | bold | size(WIDTH, EQUAL, 5) | border,
          text("File Name") | bold | size(WIDTH, EQUAL, 30) | border,
          text("Title") | bold | size(WIDTH, EQUAL, 30) | border,
          text("Artist") | bold | size(WIDTH, EQUAL, 30) | border,
          text("Duration") | bold | size(WIDTH, EQUAL, 15) | border,
          text("Date Created") | bold | size(WIDTH, EQUAL, 20) | border,
      }));

      int start_index = current_page * rows_per_page;
      int end_index =
          std::min(start_index + rows_per_page, (int)media_files.size());
      for (int i = start_index + scroll_offset;
           i < std::min(start_index + scroll_offset + scroll_visible_rows,
                        end_index);
           ++i) {
        auto mediaFile = media_files[i];
        bool is_selected = (i == selected_index);
        bool is_hovered = (i == hovered_index);

        table_rows.push_back(
            hbox({
                text(std::to_string(i + 1)) | size(WIDTH, EQUAL, 5) | border,
                text(mediaFile->getFileName()) | size(WIDTH, EQUAL, 30) | border,
                text(mediaFile->getMetadata("title")) | size(WIDTH, EQUAL, 30) |
                    border,
                text(mediaFile->getMetadata("artist")) |
                    size(WIDTH, EQUAL, 30) | border,
                text(mediaFile->getDuration()) | size(WIDTH, EQUAL, 15) |
                    border,
                text(mediaFile->getDateCreated()) | size(WIDTH, EQUAL, 20) |
                    border,
            }) |
            (is_selected ? inverted : nothing) |
            (is_hovered && !is_selected ? dim | color(Color::Yellow)
                                        : nothing));
      }

      auto scrollable_list = vbox(std::move(table_rows)) | vscroll_indicator |
                             frame | border | size(HEIGHT, EQUAL, 35);

      auto exit_button = text("Exit (Simulate 0)") | bold | center | border |
                         (is_exit_hovered ? color(Color::Red) : nothing);

      return vbox({
          separator(),
          text("Use UP/DOWN to navigate rows, LEFT/RIGHT to change pages, "
               "ENTER to select.") |
              dim | center,
          separator(),
          hbox({
              text("Enter filename: ") | bold,
              text(input_buffer) | border | size(WIDTH, EQUAL, 50),
          }) | center,
          text("Selected File: " +
               (selected_index >= 0 && selected_index < (int)media_files.size()
                    ? media_files[selected_index]->getFileName()
                    : "")) |
              bold | center,
          text(error_message) | color(Color::Red) | center,
          separator(),
          text("----- All Media Files In Playlist: " + playlist->getPlaylistName() +
               "-----") |
              bold | center | color(Color::Blue),
          separator(),
          text("Page " + std::to_string(current_page + 1) + " of " +
               std::to_string(total_pages)) |
              center,
          scrollable_list,
          separator(),
          exit_button | center,

      });
    };

    auto container = Container::Vertical({
        Renderer(render_view),
    });

    auto component = CatchEvent(container, [&](Event event) -> bool {
      if (event == Event::ArrowUp) {
        if (selected_index > 0) {
          --selected_index;
          if (selected_index < current_page * rows_per_page + scroll_offset) {
            --scroll_offset;
          }
        }
        return true;
      }

      if (event == Event::ArrowDown) {
        if (selected_index < std::min((current_page + 1) * rows_per_page,
                                      (int)media_files.size()) -
                                 1) {
          ++selected_index;
          if (selected_index >= current_page * rows_per_page + scroll_offset +
                                    scroll_visible_rows) {
            ++scroll_offset;
          }
        }
        return true;
      }

      if (event == Event::ArrowLeft) {
        if (current_page > 0) {
          --current_page;
          selected_index = current_page * rows_per_page;
          scroll_offset = 0;
        }
        return true;
      }

      if (event == Event::ArrowRight) {
        if (current_page < total_pages - 1) {
          ++current_page;
          selected_index = current_page * rows_per_page;
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
        if (scroll_offset + scroll_visible_rows <
            std::min(rows_per_page,
                     (int)media_files.size() - current_page * rows_per_page)) {
          ++scroll_offset;
        }
        return true;
      }

      if (event.mouse().motion == Mouse::Moved) {
        const int list_y_start = 16;
        const int row_height = 3;
        const int list_y_end = list_y_start + scroll_visible_rows * row_height;

        if (event.mouse().y >= list_y_start && event.mouse().y < list_y_end) {
          int relative_y = event.mouse().y - list_y_start;
          int hovered_row = (relative_y / row_height) +
                            (scroll_offset + current_page * rows_per_page);
          if (hovered_row >= current_page * rows_per_page &&
              hovered_row < std::min((current_page + 1) * rows_per_page,
                                     (int)media_files.size())) {
            hovered_index = hovered_row;
          } else {
            hovered_index = -1;
          }
        } else {
          hovered_index = -1;
        }

        const int exit_button_y_start = 47;
        const int exit_button_y_end = exit_button_y_start + 3;
        is_exit_hovered = (event.mouse().y >= exit_button_y_start &&
                           event.mouse().y < exit_button_y_end);

        return true;
      }

      if (event.is_character()) {
        char c = event.character()[0];
        if (std::isalnum(c) || c == '.' || c == '_' || c == '-' || c == ' ' ||
            c == '(' || c == ')') {
          input_buffer += c;
          hovered_index = -1;
          return true;
        }
      }

      if (event == Event::Backspace && !input_buffer.empty()) {
        input_buffer.pop_back();
        hovered_index = -1;
        return true;
      }

      if (event.mouse().button == Mouse::Left) {
        if (is_exit_hovered) {
          hovered_index = -1;
          result_filename = "exit";
          screen.ExitLoopClosure()();
          return true;
        }

        if (hovered_index != -1) {
          selected_index = hovered_index;
          result_filename = media_files[selected_index]->getFileName();
          screen.ExitLoopClosure()();
          return true;
        }
      }

      if (event == Event::Return) {
        if (!input_buffer.empty()) {
          if (input_buffer == "0") {
            result_filename = "exit";
            screen.ExitLoopClosure()();
            return true;
          }
          auto it = std::find_if(media_files.begin(), media_files.end(),
                                 [&](const auto &file) {
                                   return file->getFileName() == input_buffer;
                                 });
          if (it != media_files.end()) {
            selected_index = std::distance(media_files.begin(), it);
            current_page = selected_index / rows_per_page;
            scroll_offset = selected_index % rows_per_page;
            result_filename = media_files[selected_index]->getFileName();
            screen.ExitLoopClosure()();
          } else {
            error_message = "File not found!";
          }
        } else if (selected_index != -1) {
          result_filename = media_files[selected_index]->getFileName();
          screen.ExitLoopClosure()();
        }
        return true;
      }

      if (event.is_character()) {
        char c = event.character()[0];
        if (std::isalnum(c) || c == '.' || c == '_' || c == '-' || c == ' ' ||
            c == '(' || c == ')') {
          input_buffer += c;
          hovered_index = -1;
          return true;
        }
      }

      if (event == Event::Backspace && !input_buffer.empty()) {
        input_buffer.pop_back();
        hovered_index = -1;
        return true;
      }

      return false;
    });

    screen.Loop(component);
    return result_filename;
  } catch (const std::exception &e) {
    std::cerr << "An error occurred: " << e.what() << std::endl;
    return "exit";
  }
}

std::pair<std::string, std::string> PlaylistHandlerView::displayAllFolder(
    std::pair<std::unordered_set<std::string>, std::unordered_set<std::string>>
        listFolderDirectoryAndUSB) 
{

  std::vector<std::string> listFolderDirectory(
      listFolderDirectoryAndUSB.first.begin(),
      listFolderDirectoryAndUSB.first.end());
  std::vector<std::string> listFolderUSB(
      listFolderDirectoryAndUSB.second.begin(),
      listFolderDirectoryAndUSB.second.end());

  std::vector<std::string> mainMenuEntries = {"1. Directory", "2. USB",
                                              "3. Exit"};

  int selectedIndex = 0;
  int finalSelected = -1;
  auto menu = Menu(&mainMenuEntries, &selectedIndex);
  auto screen = ScreenInteractive::TerminalOutput();

  auto mainComponent = Renderer(menu, [&] {
    return vbox(
        {text("Select an option:") | bold | center, separator(),
         menu->Render() | border | center, separator(),
         text("Use UP/DOWN keys or click to navigate. Press ENTER to select.") |
             dim | center});
  });

  mainComponent = CatchEvent(mainComponent, [&](Event event) {
    if (event == Event::Return) {
      screen.ExitLoopClosure()();
      return true;
    }

    if (event.is_mouse() && event.mouse().button == Mouse::Left &&
        event.mouse().motion == Mouse::Pressed) {
      int clickedIndex = event.mouse().y - 3;
      if (clickedIndex >= 0 && clickedIndex < (int)mainMenuEntries.size()) {
        selectedIndex = clickedIndex;
        screen.ExitLoopClosure()();
        return true;
      }
    }

    menu->OnEvent(event);
    return false;
  });

  screen.Loop(mainComponent);

  finalSelected = selectedIndex;
  if (finalSelected == 2) {
    return {"exit", ""};
  }

  std::vector<std::string> *selectedList = nullptr;
  std::string sourceType;

  if (finalSelected == 0) {
    selectedList = &listFolderDirectory;
    sourceType = "Directory";
  } else if (finalSelected == 1) {
    selectedList = &listFolderUSB;
    sourceType = "USB";
  }

  if (selectedList->empty()) {
    std::cout << "No folders found in " << sourceType << ".\n";
    return {sourceType, ""};
  }

  int folderSelectedIndex = 0;
  std::vector<std::string> folderMenuEntries;
  for (size_t i = 0; i < selectedList->size(); ++i) {
    folderMenuEntries.push_back(std::to_string(i + 1) + ". " +
                                (*selectedList)[i]);
  }
  folderMenuEntries.push_back("0. Exit");

  auto folderMenu = Menu(&folderMenuEntries, &folderSelectedIndex);
  std::string inputBuffer;
  std::string errorMessage;

  auto folderComponent = Renderer(folderMenu, [&] {
    return vbox({text("========= List of Folders =========") | bold | center,
                 separator(), folderMenu->Render() | border | center,
                 separator(),
                 hbox({
                     text("Enter your choice: ") | bold,
                     text(inputBuffer) | bold | dim,
                 }),
                 separator(),
                 text("Use UP/DOWN keys, click to navigate, or type the number "
                      "and press ENTER.") |
                     dim | center,
                 text(errorMessage) | color(Color::Red) | center});
  });

  folderComponent = CatchEvent(folderComponent, [&](Event event) {
    if (event == Event::Return) {
      if (!inputBuffer.empty()) {
        try {
          int number = std::stoi(inputBuffer);
          if (number == 0) {
            folderSelectedIndex = (int)folderMenuEntries.size() - 1;
          } else if (number > 0 && number <= (int)selectedList->size()) {
            folderSelectedIndex = number - 1;
          } else {
            errorMessage = "Invalid input: number out of range!";
            return true;
          }
        } catch (...) {
          errorMessage = "Invalid input: please enter a valid number!";
          return true;
        }
        screen.ExitLoopClosure()();
        return true;
      }
      screen.ExitLoopClosure()();
      return true;
    }

    if (event.is_character()) {
      inputBuffer += event.character();
      return true;
    }

    if (event == Event::Backspace && !inputBuffer.empty()) {
      inputBuffer.pop_back();
      return true;
    }

    if (event.is_mouse() && event.mouse().button == Mouse::Left &&
        event.mouse().motion == Mouse::Pressed) {
      int clickedIndex = event.mouse().y - 3;
      if (clickedIndex >= 0 && clickedIndex < (int)folderMenuEntries.size()) {
        folderSelectedIndex = clickedIndex;
        screen.ExitLoopClosure()();
        return true;
      } else {
        errorMessage = "Invalid click: out of range!";
        return true;
      }
    }

    folderMenu->OnEvent(event);
    return false;
  });

  screen.Loop(folderComponent);

  if (folderSelectedIndex == (int)folderMenuEntries.size() - 1) {
    return {sourceType, ""};
  }

  return {sourceType, (*selectedList)[folderSelectedIndex]};
}
