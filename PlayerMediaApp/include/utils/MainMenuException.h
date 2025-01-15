#ifndef MAIN_MENU_EXCEPTION_H
#define MAIN_MENU_EXCEPTION_H

#include <exception>
#include <string>
#include <limits>

// Base exception class
class MainMenuException : public std::exception {
public:
    explicit MainMenuException(const std::string& msg) : message(msg) {}

    virtual ~MainMenuException() noexcept = default;

    virtual const char* what() const noexcept override {
        return message.c_str();
    }

protected:
    std::string message;
};


#endif