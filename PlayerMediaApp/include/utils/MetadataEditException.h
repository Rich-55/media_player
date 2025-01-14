#ifndef METADATAEDITEXCEPTION_H
#define METADATAEDITEXCEPTION_H

#include <exception>
#include <string>

class MetadataEditException : public std::exception {
public:
    explicit MetadataEditException(const std::string& msg) : message(msg) {}

    virtual ~MetadataEditException() noexcept = default;

    virtual const char* what() const noexcept override {
        return message.c_str();
    }

protected:
    std::string message;
};

class KeyNotFoundException : public MetadataEditException {
public:
    explicit KeyNotFoundException(const std::string& key)
        : MetadataEditException("Key not found: " + key) {}
};

class InvalidInputException : public MetadataEditException {
public:
    InvalidInputException()
        : MetadataEditException("Invalid input provided.") {}
};

class KeyNotAllowedException : public MetadataEditException {
public:
    KeyNotAllowedException()
        : MetadataEditException("Invalid key entered. Key is not allowed.") {}
};

class InvalidValueFormatException : public MetadataEditException {
public:
    explicit InvalidValueFormatException(const std::string& message, const std::string& key = "")
        : MetadataEditException("Invalid value format for key '" + key + "': " + message) {}
};

#endif
