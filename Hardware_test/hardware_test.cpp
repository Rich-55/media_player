#include "MediaController.h"
#include "MediaModel.h"
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

namespace asio = boost::asio;

void displayMenu() {
    std::cout << "\nMedia Controller Test\n";
    std::cout << "1. Play | Pause\n";
    std::cout << "2. Stop\n";
    std::cout << "3. Set Volume\n";
    std::cout << "4. Exit\n";
    std::cout << "Enter your choice: ";
}

void handle_uart(asio::serial_port& serial, MediaController& mediaController) {
    asio::streambuf buffer;
    std::istream input_stream(&buffer);

    bool running = true;
    while (running) {
        // Đọc một dòng dữ liệu từ UART (kết thúc bằng '\n')
        asio::read_until(serial, buffer, '\n');

        // Lấy dữ liệu từ buffer
        std::string received_data;
        std::getline(input_stream, received_data);

        if (received_data == "SW1") {
            mediaController.togglePlayback();
            std::cout << "Play" << std::endl;

        } else if (received_data == "SW1") {
            mediaController.togglePlayback();
            std::cout << "Pause" << std::endl;

        } else if (received_data == "SW3") {
            mediaController.stop();
            std::cout << "Stop" << std::endl;
            
        } else if (received_data == "next") {
            mediaController.playNext();
            std::cout << "next" << std::endl;
            
        } else if (received_data == "previous") {
            mediaController.playPrevious();
            std::cout << "previous" << std::endl;
      
        } else {
            try {
                int volume = std::stoi(received_data);
                mediaController.setVolume(volume);
                std::cout << "Volume Received: " << volume << std::endl;

            } catch (const std::invalid_argument&) {
                // Bỏ qua các chuỗi không hợp lệ
                continue;
            } catch (const std::out_of_range&) {
                // Bỏ qua các giá trị ngoài phạm vi
                continue;
            }
        }
    }
}

int main() {

    asio::io_context io_context;

    MediaModel model;
    std::string directory;

    std::cout << "Enter the path to the directory containing media files: ";
    int cho;
    std::cin >> cho;
    if(cho == 0) { directory = "/home/giaupvt/Videos"; }
    else { directory = "/home/giaupvt/Music"; }

    model.loadMediaFiles(directory);

    const auto& mediaFiles = model.getMediaFiles();
    std::cout << "Loaded Media Files:\n";
    if (mediaFiles.empty()) {
        std::cerr << "No supported media files found in the specified directory.\n";
        return -1;
    }

    for (const auto& file : mediaFiles) {
        std::cout << " - " << file << "\n";
    }

    MediaController mediaController(mediaFiles);


    try {
        asio::serial_port serial(io_context, "/dev/ttyACM0"); // Cổng chính xác
        serial.set_option(asio::serial_port_base::baud_rate(115200));
        serial.set_option(asio::serial_port_base::character_size(8));
        serial.set_option(asio::serial_port_base::parity(asio::serial_port_base::parity::none));
        serial.set_option(asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::one));
        serial.set_option(asio::serial_port_base::flow_control(asio::serial_port_base::flow_control::none));

        std::cout << "UART connected successfully!" << std::endl;

        handle_uart(serial, mediaController);

    } catch (const boost::system::system_error& e) {
        std::cerr << "Error opening port: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
