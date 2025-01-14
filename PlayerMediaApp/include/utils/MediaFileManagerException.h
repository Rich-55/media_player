#ifndef MEDIAFILE_MANAGER_EXCEPTION_H
#define MEDIAFILE_MANAGER_EXCEPTION_H

#include <exception>
#include <string>
class MediaFileManagerException : public std::exception {
public:

    explicit MediaFileManagerException(const std::string& msg) : message(msg) {}

    virtual ~MediaFileManagerException() noexcept = default;

    virtual const char* what() const noexcept override {
        return message.c_str();
    }

protected:
    std::string message;
};

class UnsupportedFileTypeException : public MediaFileManagerException {
public:
    explicit UnsupportedFileTypeException(const std::string& fileName)
        : MediaFileManagerException("Unsupported file type: " + fileName) {}
};

class FileNotFoundException : public MediaFileManagerException {
public:
    explicit FileNotFoundException(const std::string& fileName)
        : MediaFileManagerException("File not found: " + fileName) {}
};

class NoMediaFilesException : public MediaFileManagerException {
public:
    NoMediaFilesException()
        : MediaFileManagerException("No media files to display.") {}
};

class InvalidFilePathException : public MediaFileManagerException {
public:
    explicit InvalidFilePathException(const std::string& filePath)
        : MediaFileManagerException("Invalid file path: " + filePath) {}
};

class FileAlreadyExistsException : public MediaFileManagerException {
public:
    explicit FileAlreadyExistsException(const std::string& fileName)
        : MediaFileManagerException("File already exists: " + fileName) {}
};

class DatabaseFileException : public MediaFileManagerException {
public:
    explicit DatabaseFileException(const std::string& fileName)
        : MediaFileManagerException("Error opening database file: " + fileName) {}
};

#endif

//MediaFileManager Controller Exception
//MediaFileView Exception
