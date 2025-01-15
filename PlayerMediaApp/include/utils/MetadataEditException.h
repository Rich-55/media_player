#ifndef METADATA_EDIT_EXCEPTION_H
#define METADATA_EDIT_EXCEPTION_H

#include <exception>
#include <string>
#include <limits>

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
        : MetadataEditException("Key not found or is unknown: " + key) {}
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

class EditFailException : public MetadataEditException {
public:
    EditFailException()
        : MetadataEditException("Error: Failed to save changes to the file.") {}
};

class InvalidValueFormatException : public MetadataEditException {
public:
    explicit InvalidValueFormatException(const std::string& message, const std::string& key = "")
        : MetadataEditException("Invalid value format for key '" + key + "': " + message) {}
};


#endif
