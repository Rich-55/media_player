#ifndef UART_MANAGER_H
#define UART_MANAGER_H

#include <boost/asio.hpp>
#include <thread>
#include <memory>
#include <iostream>
#include "../Controller/PlayerController.h"

namespace asio = boost::asio;
class UARTManager {

private:
    asio::io_context io_context;
    std::unique_ptr<asio::serial_port> serial_port;
    std::thread uartThread;

    static void asyncHandleUart(asio::serial_port& serial, std::shared_ptr<PlayerController>& playerController);
public:
    UARTManager();
    ~UARTManager();

    void runMediaUart(std::shared_ptr<PlayerController>& playerController);
    void stopUART();
    bool isConnectionActive() const; // New method to check connection status
    virtual bool checkPortConnection(const std::string& port, unsigned int baud_rate);

    virtual std::vector<std::string> getPortList();
    virtual std::vector<std::string> getBaudRateOptions();
};

#endif 
