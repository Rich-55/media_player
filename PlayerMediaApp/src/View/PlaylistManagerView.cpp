#include "../../include/View/PlaylistManagerView.h"

PlaylistManagerView::PlaylistManagerView() {}

void PlaylistManagerView::showNotificationMessage(std::string message, std::string type) {BaseView::showNotificationMessage(message, type);}

bool PlaylistManagerView::showConfirmMenu(std::string message) {return BaseView::showConfirmMenu(message);}
// not use
int PlaylistManagerView::showMenu() { return -1; }

std::string PlaylistManagerView::showMenuCreatePlaylist() {

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
      error_message = "exit";
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

int PlaylistManagerView::showMenuWithPlaylist(
    std::vector<std::shared_ptr<Playlist>> listPlaylist) 
{

  std::vector<std::string> menu_entries = {
      "1. Create new playlist", 
      "2. Delete playlist", 
      "3. Show all playlists",
      "0. Back to main menu"
  };
  std::vector<int> logic_mapping = {1, 2, 3, 0}; 
  int selected = 0;                   
  std::string error_message;            
  int final_selected = -1;              

  const int rows_per_page = 25;       
  const int scroll_visible_rows = 10; 
  int current_page = 0;               
  int total_pages = std::ceil((double)listPlaylist.size() / rows_per_page);
  int scroll_offset = 0;

  auto create_table_view = [&]() {
    std::vector<ftxui::Element> rows;

    if (listPlaylist.empty()) {
      rows.push_back(ftxui::text("No playlist.") | ftxui::bold | ftxui::center);
    } else {
      rows.push_back(ftxui::hbox({
          ftxui::text("STT") | ftxui::bold |
              ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 5) | ftxui::border,
          ftxui::text("Playlist Name") | ftxui::bold |
              ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 30) | ftxui::border,
      }));

      int start_index = current_page * rows_per_page;
      int end_index =
          std::min(start_index + rows_per_page, (int)listPlaylist.size());

      for (int i = start_index + scroll_offset;
           i < std::min(start_index + scroll_offset + scroll_visible_rows,
                        end_index);
           ++i) {
        auto playlist = listPlaylist[i];
        rows.push_back(ftxui::hbox({
            ftxui::text(std::to_string(i + 1)) |
                ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 5) | ftxui::border,
            ftxui::text(playlist->getPlaylistName()) |
                ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 20) | ftxui::border,
        }));
      }
    }

    return ftxui::vbox({
        ftxui::text("----- List Playlist: -----") | ftxui::bold |
            ftxui::center | ftxui::color(ftxui::Color::Blue),
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
        {ftxui::text("============ Playlist Manager ===========") |
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
      int clicked_index =
          event.mouse().y - 3; 
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
          std::min(rows_per_page,
                   (int)listPlaylist.size() - current_page * rows_per_page);
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

std::string
PlaylistManagerView::displayAllPlaylist(PlaylistManager playlistManager) 
{
  system("clear");
  try {
    auto listPlaylists = playlistManager.getAllPlaylist();

    if (listPlaylists.empty()) {
      auto screen = ftxui::ScreenInteractive::TerminalOutput();
      auto empty_message = ftxui::Renderer([&] {
        return ftxui::vbox({
            ftxui::text("No playlists to display.") | ftxui::bold |
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
    int total_pages = std::ceil((double)listPlaylists.size() / rows_per_page);
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
          text("Playlist Name") | bold | size(WIDTH, EQUAL, 30) | border,
      }));

      int start_index = current_page * rows_per_page;
      int end_index =
          std::min(start_index + rows_per_page, (int)listPlaylists.size());
      for (int i = start_index + scroll_offset;
           i < std::min(start_index + scroll_offset + scroll_visible_rows,
                        end_index);
           ++i) {
        auto playlist = listPlaylists[i];
        bool is_selected = (i == selected_index);
        bool is_hovered = (i == hovered_index);

        table_rows.push_back(
            hbox({
                text(std::to_string(i + 1)) | size(WIDTH, EQUAL, 5) | border,
                text(playlist->getPlaylistName()) | size(WIDTH, EQUAL, 30) | border,
            }) |
            (is_selected ? inverted : nothing) 
            | (is_hovered && !is_selected ? dim | color(Color::Yellow)
                                          : nothing) 
        );
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
          text(
              "Selected File: " +
              (selected_index >= 0 && selected_index < (int)listPlaylists.size()
                   ? listPlaylists[selected_index]->getPlaylistName()
                   : "")) |
              bold | center,
          text(error_message) | color(Color::Red) | center,
          separator(),
          text("----- All Playlist-----") | bold | center | color(Color::Blue),
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
                                      (int)listPlaylists.size()) -
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
            std::min(rows_per_page, (int)listPlaylists.size() -
                                        current_page * rows_per_page)) {
          ++scroll_offset;
        }
        return true;
      }

      if (event.mouse().motion == Mouse::Moved) {
        const int list_y_start = 16; 
        const int row_height = 3;  
        const int list_y_end =
            list_y_start + scroll_visible_rows *
                               row_height; 

        if (event.mouse().y >= list_y_start && event.mouse().y < list_y_end) {
          int relative_y = event.mouse().y - list_y_start;
          int hovered_row = (relative_y / row_height) +
                            (scroll_offset + current_page * rows_per_page);

          if (hovered_row >= current_page * rows_per_page &&
              hovered_row < std::min((current_page + 1) * rows_per_page,
                                     (int)listPlaylists.size())) {
            hovered_index = hovered_row;
          } else {
            hovered_index = -1; 
          }
        } else {
          hovered_index = -1; 
        }

        const int exit_button_y_start = 47; 
        const int exit_button_y_end =
            exit_button_y_start + 3; 
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
          result_filename = listPlaylists[selected_index]->getPlaylistName();
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
          auto it = std::find_if(listPlaylists.begin(), listPlaylists.end(),
                                 [&](const auto &playlist) {
                                   return playlist->getPlaylistName() == input_buffer;
                                 });
          if (it != listPlaylists.end()) {
            selected_index = std::distance(listPlaylists.begin(), it);
            current_page = selected_index / rows_per_page;
            scroll_offset = selected_index % rows_per_page;
            result_filename = listPlaylists[selected_index]->getPlaylistName();
            screen.ExitLoopClosure()();
          } else {
            error_message = "File not found!";
          }
        } else if (selected_index != -1) {
          result_filename = listPlaylists[selected_index]->getPlaylistName();
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