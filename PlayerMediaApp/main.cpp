#include "include/Model/ModelManager.h"
#include "include/View/ViewManager.h"
#include "include/Controller/ControllerManager.h"
#include "include/utils/UARTSetup.h"

int main() {
    ModelManager model;
    ViewManager view;
    auto uartManager = std::make_shared<UARTManager>();

    std::string port = "/dev/ttyACM0";
    unsigned int baud_rate = 115200;

    if (uartManager->checkPortConnection(port, baud_rate)) {
        std::cout << "Port " << port << " is available at " << baud_rate << " baud.\n";
    } else {
        std::cout << "Failed to connect to port " << port << ".\n";
    }

    
    ControllerManager controller(model, view, uartManager);
    
    controller.ScanData();
    controller.runApp();

    return 0;
}