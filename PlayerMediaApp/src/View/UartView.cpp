#include "../../include/View/UartView.h"

UartView::UartView() {}



std::pair<std::string, unsigned int> UartView::getPortAndBaudRate(std::vector<std::string> portList, std::vector<std::string> baudRateOptions) {
    system("clear");
    std::string selected_port;
    unsigned int selected_baud_rate = 9600;
    int selected_port_index = 0;
    int selected_baud_index = 3;
    std::string error_message;

    ScreenInteractive screen = ScreenInteractive::TerminalOutput();

    auto port_menu = Menu(&portList, &selected_port_index) | vscroll_indicator | frame;
    auto baud_rate_menu = Menu(&baudRateOptions, &selected_baud_index);

    auto confirm_button = Button("Confirm", [&] {
        if (portList.empty() || portList[0] == "No UART ports found.") {
            error_message = "Error: No UART ports available.";
            return;
        }
        selected_port = portList[selected_port_index];
        selected_baud_rate = std::stoul(baudRateOptions[selected_baud_index]);
        screen.ExitLoopClosure()();
    });

    auto quit_button = Button("Quit", [&] {
        selected_port = "exit";
        screen.ExitLoopClosure()();
    });

    auto container = Container::Vertical({
        port_menu,
        baud_rate_menu,
        confirm_button,
        quit_button,
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
                   text("Select UART Port and Baud Rate") | bold | center,
                   separator(),
                   hbox(text("Available Ports:") | bold, port_menu->Render() | border | size(HEIGHT, LESS_THAN, 12)),
                   hbox(text("Baud Rates:") | bold, baud_rate_menu->Render() | border),
                   separator(),
                   hbox(confirm_button->Render() | center, quit_button->Render() | center),
                   separator(),
                   text(error_message) | color(Color::Red) | center,
               }) |
               border;
    });

    screen.Loop(renderer);

    if (selected_port == "exit") {
        return {"exit", 0};
    }

    return std::make_pair(selected_port, selected_baud_rate);
}

