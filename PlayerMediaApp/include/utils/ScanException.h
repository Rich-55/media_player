#ifndef SCAN_EXCEPTION_H
#define SCAN_EXCEPTION_H

#include <exception>
#include <string>
#include <limits>

class ScanException : public std::exception {
public:

    explicit ScanException(const std::string& msg) : message(msg) {}

    virtual ~ScanException() noexcept = default;

    virtual const char* what() const noexcept override {
        return message.c_str();
    }

protected:
    std::string message;
};

class DirectoryOpenException : public ScanException {
public:
    explicit DirectoryOpenException(const std::string& path)
        : ScanException("Error opening directory: " + path) {}
};

class HomeDirectoryException : public ScanException {
public:
    HomeDirectoryException()
        : ScanException("Unable to determine HOME directory.") {}
};

class NoFoldersFoundException : public ScanException {
public:
    explicit NoFoldersFoundException(const std::string& path)
        : ScanException("No folders found in directory: " + path) {}
};


class NoMediaFilesFoundException : public ScanException {
public:
    explicit NoMediaFilesFoundException(const std::string& path)
        : ScanException("No media files found in directory: " + path) {}
};

class NoUSBDevicesFoundException : public ScanException {
public:
    NoUSBDevicesFoundException()
        : ScanException("No USB devices found.") {}
};


#endif

