#ifndef UART_VIEW_H
#define UART_VIEW_H

#include "BaseView.h"

class UartView : public BaseView{
    private:

    public:
        UartView();

        int showMenu() override { return -1; }

        std::pair<std::string, unsigned int> getPortAndBaudRate(std::vector<std::string>, std::vector<std::string>) override;
};

#endif