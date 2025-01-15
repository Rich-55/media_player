#include "../../include/View/MainMenuView.h"
#include <iostream>

MainMenuView::MainMenuView() {}


void MainMenuView::showMenu() {
    while (true) {
        system("clear"); // Xóa màn hình
        std::cout << "\nMain Menu:\n";
        std::cout << "1. Scan Data\n";
        std::cout << "2. View All Data\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";

        int choice = 0;
        std::cin >> choice;

    }
}
