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

    std::thread uartThread;

    static void asyncHandleUart(asio::serial_port& serial, std::shared_ptr<PlayerController>& playerController);
public:
    asio::io_context io_context;
    std::unique_ptr<asio::serial_port> serial_port;

    UARTManager();
    ~UARTManager();

    virtual void runMediaUart(std::shared_ptr<PlayerController>& playerController);
    void stopUART();
    virtual bool checkPortConnection(const std::string& port, unsigned int baud_rate);

    virtual std::vector<std::string> getPortList();
    virtual std::vector<std::string> getBaudRateOptions();
};

#endif 
