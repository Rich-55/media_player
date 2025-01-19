#include "include/Model/ModelManager.h"
#include "include/View/ViewManager.h"
#include "include/Controller/ControllerManager.h"
#include "include/Controller/PlayerController.h"
#include <thread>

int main() {
    ModelManager model;
    ViewManager view;
    ControllerManager controller(model, view);

    // std::atomic<bool> isRunning(true);

    // std::thread musicThread(playMusic, "drum.mp3", std::ref(isRunning));

    controller.ScanData();
    controller.runApp();

    // isRunning = false;

    // if (musicThread.joinable()) {
    //     musicThread.join();
    // }

    return 0;
}