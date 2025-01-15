#include "include/Model/ModelManager.h"
#include "include/View/ViewManager.h"
#include "include/Controller/ControllerManager.h"


int main() {
    ModelManager model;
    ViewManager view;
    ControllerManager controller(model, view);

    controller.ScanData();

    controller.runApp();

    return 0;
}
