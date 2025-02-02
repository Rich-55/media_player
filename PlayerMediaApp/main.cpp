#include "include/Model/ModelManager.h"
#include "include/View/ViewManager.h"
#include "include/Controller/ControllerManager.h"
#include "include/utils/UARTSetup.h"

int main() {
    ModelManager model;
    ViewManager view;
    auto uartManager = std::make_shared<UARTManager>();
    
    ControllerManager controller(model, view, uartManager);

    // if(!controller.checkUart()){
    //     return 0;
    // }
    controller.ScanData();
    controller.runApp();

    return 0;
}