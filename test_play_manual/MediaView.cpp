#include "MediaView.h"
#include <iostream>

void MediaView::displayMenu() const {
    std::cout << "\nMedia Player\n";
    std::cout << "1. Play | Pause | Resume\n";
    std::cout << "2. Next\n";
    std::cout << "3. Previous\n";
    std::cout << "4. Stop\n";
    std::cout << "5. Toggle Repeat\n";
    std::cout << "6. Exit\n";
    std::cout << "Enter your choice: ";
}

int MediaView::getUserChoice() const {
    int choice;
    std::cin >> choice;
    return choice;
}
