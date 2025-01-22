#include "../../include/utils/UARTSetup.h"

UARTManager::UARTManager() : serial_port(nullptr) {}

UARTManager::~UARTManager() { stopUART(); }

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
  } catch (const std::exception &e) {
    std::cerr << "Error stopping UART: " << e.what() << std::endl;
  }
}

bool UARTManager::isConnectionActive() const {
  if (serial_port && serial_port->is_open()) {
    return true;
  } else {
    std::cerr << "UART connection is not active.\n";
    return false;
  }
}

bool UARTManager::checkPortConnection(const std::string &port, unsigned int baud_rate) {
    try {
      serial_port = std::make_unique<asio::serial_port>(io_context, port);
      serial_port->set_option(asio::serial_port_base::baud_rate(baud_rate));
      serial_port->set_option(asio::serial_port_base::character_size(8));
      serial_port->set_option(
          asio::serial_port_base::parity(asio::serial_port_base::parity::none));
      serial_port->set_option(asio::serial_port_base::stop_bits(
          asio::serial_port_base::stop_bits::one));
      serial_port->set_option(asio::serial_port_base::flow_control(
          asio::serial_port_base::flow_control::none));

        std::cout << "Port connection successful: " << port << " at " << baud_rate << " baud.\n";
        return true;
    } catch (const boost::system::system_error &e) {
        std::cerr << "Boost System Error: " << e.what() << '\n';
    } catch (const std::exception &e) {
        std::cerr << "General Exception: " << e.what() << '\n';
    }

    return false;
}

std::vector<std::string> UARTManager::getPortList() {
  std::vector<std::string> ports;
  for (const auto &entry : std::filesystem::directory_iterator("/dev")) {
    if (entry.path().string().find("ttyACM") != std::string::npos) {
      ports.push_back(entry.path().string());
    }
  }
  if (ports.empty()) {
    ports.push_back("No UART ports found.");
  }
  return ports;
}

std::vector<std::string> UARTManager::getBaudRateOptions() {
  return {
      "1200",  "2400",  "4800",   "9600",   "14400",  "19200",
      "38400", "57600", "115200", "230400", "460800", "921600",
  };
}

void UARTManager::asyncHandleUart(asio::serial_port &serial, std::shared_ptr<PlayerController> &playerController) {
    auto buffer = std::make_shared<asio::streambuf>();

    asio::async_read_until(serial, *buffer, '\n',
        [buffer, &serial, &playerController](const boost::system::error_code &error, std::size_t) {
            if (!error) {
                try {
                    std::istream input_stream(buffer.get());
                    std::string received_data;
                    std::getline(input_stream, received_data);

                    received_data.erase(0, received_data.find_first_not_of(" \t\r\n"));
                    received_data.erase(received_data.find_last_not_of(" \t\r\n") + 1);


                    if (!received_data.empty()) {
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
                            } else if (received_data.find("Value") == std::string::npos) {
                                try {
                                    int newVolume = std::stoi(received_data);
                                    if (newVolume >= 0 && newVolume <= 100) {
                                        playerController->setVolume(newVolume);
                                    }
                                } catch (const std::invalid_argument &) {
                                    std::cerr << "Invalid argument: '" << received_data << "'" << std::endl;
                                } catch (const std::out_of_range &) {
                                    std::cerr << "Out of range: '" << received_data << "'" << std::endl;
                                }
                            } else {
                                std::cerr << "Ignored invalid data: '" << received_data << "'" << std::endl;
                            }
                        }
                    } else {
                        std::cerr << "Empty data received." << std::endl;
                    }
                } catch (const std::exception &e) {
                    std::cerr << "Error processing UART data: " << e.what() << std::endl;
                }

                UARTManager::asyncHandleUart(serial, playerController);
            } else {
                std::cerr << "UART has been shutdown: " << error.message() << std::endl;
            }
        });
}

void UARTManager::runMediaUart(std::shared_ptr<PlayerController> &playerController) {
    try {
        asyncHandleUart(*serial_port, playerController);

        uartThread = std::thread([this]() {
            try {
                io_context.run();
            } catch (const std::exception &e) {
                std::cerr << "UART thread error: " << e.what() << std::endl;
            }
        });
        uartThread.detach(); 
    } catch (const std::exception &e) {
        std::cerr << "Failed to set up UART: " << e.what() << std::endl;
    }
}

