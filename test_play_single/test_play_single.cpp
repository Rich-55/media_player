#include "MediaController.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void displayMenu() {
    std::cout << "\nMedia Controller Test\n";
    std::cout << "1. Play | Pause\n";
    std::cout << "2. Stop\n";
    std::cout << "3. Set Volume\n";
    std::cout << "4. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    const std::string filePath = "song.mp3";
    std::vector<std::string> mediaFiles = {filePath};

    MediaController mediaController(mediaFiles);
    int choice;
    bool running = true;

    while (running) {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:  // Play or Resume or Pause
                mediaController.togglePlayback();
                break;
            case 2:  // Stop
                mediaController.stop();
                break;
            case 3: {  // Set Volume
                int volume;
                std::cout << "Enter volume (0-128): ";
                std::cin >> volume;
                mediaController.setVolume(volume);
                break;
            }
            case 4:  // Exit
                mediaController.stop();
                running = false;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }

        // Add a small delay for better UI experience
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}



// g++ -o test_play_single test_play_single.cpp MediaController.cpp -std=c++20 $(pkg-config --cflags --libs sdl2 SDL2_mixer)

