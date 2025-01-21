#include <boost/asio.hpp>
#include <iostream>
#include <string>

namespace asio = boost::asio;

void handle_uart(asio::serial_port& serial) {
    asio::streambuf buffer;
    std::istream input_stream(&buffer);

    while (true) {
        // Đọc một dòng dữ liệu từ UART (kết thúc bằng '\n')
        asio::read_until(serial, buffer, '\n');

        // Lấy dữ liệu từ buffer
        std::string received_data;
        std::getline(input_stream, received_data);

        if (received_data == "play") {
            // Xử lý khi nhận được play
            std::cout << "Play" << std::endl;
      
        } else if (received_data == "pause") {
            // Xử lý khi nhận được pause
            std::cout << "Pause" << std::endl;

        } else if (received_data == "SW3") {
            // Xử lý khi nhận được SW3
            std::cout << "Stop" << std::endl;
        } else if (received_data == "next") {
            // Xử lý khi nhận được next
            std::cout << "next" << std::endl;
        } else if (received_data == "previous") {
            // Xử lý khi nhận được previous
            std::cout << "previous" << std::endl;          
        } else {
            try {
                // Chuyển chuỗi sang số nguyên (volume)
                int volume = std::stoi(received_data);
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

    try {
        asio::serial_port serial(io_context, "/dev/ttyACM2"); // Cổng chính xác
        serial.set_option(asio::serial_port_base::baud_rate(115200));
        serial.set_option(asio::serial_port_base::character_size(8));
        serial.set_option(asio::serial_port_base::parity(asio::serial_port_base::parity::none));
        serial.set_option(asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::one));
        serial.set_option(asio::serial_port_base::flow_control(asio::serial_port_base::flow_control::none));

        std::cout << "UART connected successfully!" << std::endl;

        // Xử lý dữ liệu UART
        handle_uart(serial);

    } catch (const boost::system::system_error& e) {
        std::cerr << "Error opening port: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}