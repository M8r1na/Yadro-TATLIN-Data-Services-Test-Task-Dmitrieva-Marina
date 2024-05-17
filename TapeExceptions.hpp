#ifndef TAPE_EXCEPTIONS_HPP
#define TAPE_EXCEPTIONS_HPP

#include <exception>
#include <string>

class TapeException : public std::exception
{
  private:
    std::string message;

  public:
    TapeException(const std::string &message) : message(message) {}

    const char *what() const noexcept override { return message.c_str(); }
};

struct TapeOpenException : public TapeException {
    TapeOpenException(const std::string &message) 
        : TapeException("Failed to open tape: " + message) {}
};

struct TapeReadException : public TapeException {
    TapeReadException(const std::string &message)
        : TapeException("Failed to read from tape: " + message) {}
};

struct ReadFromEmptyTapeException : public TapeException {
    ReadFromEmptyTapeException() 
    : TapeException("Attempted to read from an empty tape") {}
};

struct TapeWriteException : public TapeException {
    TapeWriteException(const std::string &message)
        : TapeException("Failed to write to tape: " + message) {}
};

struct ConfigLoadException : public TapeException {
    ConfigLoadException(const std::string &message)
        : TapeException("Failed to load config: " + message) {}
};

struct MissingConfigParameterException : public TapeException {
    MissingConfigParameterException()
        : TapeException("Config file is missing some parameters") {}
};

struct CreateTmpDirectoryException : public TapeException {
    CreateTmpDirectoryException()
        : TapeException("Failed to create tmp directory") {}
};

struct CreateFileException : public TapeException {
    CreateFileException(const std::string &message)
        : TapeException("Failed to create file: " + message) {}
};

struct NotEnoughTempTapesException : public TapeException {
    NotEnoughTempTapesException()
        : TapeException("Not enough temporary tapes to sort the data") {}
};

struct InvalidNumberOfArgumentsException : public TapeException {
    InvalidNumberOfArgumentsException(const int expected, const int actual)
        : TapeException("Invalid number of arguments. Expected: " + std::to_string(expected)
                        + ", actual: " + std::to_string(actual)) {}
};

struct FileOpenException : public TapeException {
    FileOpenException()
        : TapeException("Failed to open file") {}
};

struct InvalidFileNameException : public TapeException {
    InvalidFileNameException()
        : TapeException("Invalid file name") {}
};

#endif  // TAPE_EXCEPTIONS_HPP