#include "include/Model/ModelManager.h"
#include "include/View/CLIManager.h"
#include "include/Controller/ControllerManager.h"


int main() {
    ModelManager model;
    CLIManager view;
    ControllerManager controller(model,view);
    //init data
    controller.ScanData();

    int choice;
    while(1){
        std::cout << "============Main Menu==========" << std::endl;
        std::cout << "1. Metadata Manager" << std::endl;
        std::cout << "2. " << std::endl;
        std::cout << "0. Exit" << std::endl;

        std::cin >> choice;

        switch (choice)
        {
        case 1:
            controller.metadataFileHandler();
            break;
        case 0:
            return 0;
        default:
            break;
        }
    }
    
    return 0;
}
