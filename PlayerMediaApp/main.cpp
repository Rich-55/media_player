#include "include/Model/ModelManager.h"
#include "include/View/CLIManager.h"
#include "include/Controller/ControllerManager.h"


int main() {
    ModelManager model;
    CLIManager view;
    ControllerManager controller(model,view);

    controller.ScanData();

    return 0;
}
