#include "../../include/utils/UARTSetup.h"

UARTManager::UARTManager() : serial_port(nullptr) {}

UARTManager::~UARTManager() {
    stopUART();
}

void UARTManager::asyncHandleUart(asio::serial_port& serial, std::shared_ptr<PlayerController>& playerController) {
    auto buffer = std::make_shared<asio::streambuf>();

    asio::async_read_until(serial, *buffer, '\n',
        [buffer, &serial, &playerController](const boost::system::error_code& error, std::size_t bytes_transferred) {
            (void)bytes_transferred;
            if (!error) {
                std::istream input_stream(buffer.get());
                std::string received_data;
                std::getline(input_stream, received_data);

                if (playerController) {
                    if (received_data == "play") {
                        playerController->togglePlayback();
                    } else if (received_data == "pause") {
                        playerController->togglePlayback();
                    } else if (received_data == "SW3") {
                        playerController->stop();
                    } else if (received_data == "next") {
                        playerController->playNext();
                    } else if (received_data == "previous") {
                        playerController->playPrevious();
                    } else {
                        try {
                            int newVolume = std::stoi(received_data);
                            playerController->setVolume(newVolume);
                        } catch (...) {
                            // Invalid input, ignore
                        }
                    }
                }

                asyncHandleUart(serial, playerController);
            } else {
                std::cerr << "UART has been shutdown: " << error.message() << std::endl;
            }
        });
}

void UARTManager::setUpUART(const std::string& port, unsigned int baud_rate, std::shared_ptr<PlayerController>& playerController) {
    try {
        serial_port = std::make_unique<asio::serial_port>(io_context, port);
        serial_port->set_option(asio::serial_port_base::baud_rate(baud_rate));
        serial_port->set_option(asio::serial_port_base::character_size(8));
        serial_port->set_option(asio::serial_port_base::parity(asio::serial_port_base::parity::none));
        serial_port->set_option(asio::serial_port_base::stop_bits(asio::serial_port_base::stop_bits::one));
        serial_port->set_option(asio::serial_port_base::flow_control(asio::serial_port_base::flow_control::none));

        asyncHandleUart(*serial_port, playerController);

        uartThread = std::thread([this]() {
            try {
                io_context.run();
            } catch (const std::exception& e) {
                std::cerr << "UART thread error: " << e.what() << std::endl;
            }
        });

        std::cout << "UART setup completed on port " << port << " with baud rate " << baud_rate << ".\n";
    } catch (const std::exception& e) {
        std::cerr << "Failed to set up UART: " << e.what() << std::endl;
    }
}

void UARTManager::stopUART() {
    try {
        if (serial_port) {
            serial_port->close();
        }
        io_context.stop();
        if (uartThread.joinable()) {
            uartThread.join();
        }
        std::cout << "UART stopped successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error stopping UART: " << e.what() << std::endl;
    }
}
