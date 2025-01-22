#ifndef UART_MANAGER_H
#define UART_MANAGER_H

#include <boost/asio.hpp>
#include <thread>
#include <memory>
#include <iostream>
#include "../Controller/PlayerController.h"

namespace asio = boost::asio;
class UARTManager {
public:
    UARTManager();
    ~UARTManager();

    void setUpUART(const std::string& port, unsigned int baud_rate, std::shared_ptr<PlayerController>& playerController);
    void stopUART();
    bool isConnectionActive() const; // New method to check connection status
    bool checkPortConnection(const std::string& port, unsigned int baud_rate);

private:
    asio::io_context io_context;
    std::unique_ptr<asio::serial_port> serial_port;
    std::thread uartThread;

    static void asyncHandleUart(asio::serial_port& serial, std::shared_ptr<PlayerController>& playerController);
};

#endif 
