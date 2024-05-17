#include "TapeDevice.hpp"
#include "TapeExceptions.hpp"
#include <iostream>

// CONSTRUCTOR

TapeDevice::TapeDevice(const std::string &filename) : filename(filename)
{
    openTape(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!tape) {
        openTape(filename, std::ios::out);
        openTape(filename, std::ios::in | std::ios::out | std::ios::binary);
    }
    if (!tape) {
        throw TapeReadException(std::string("Failed to open tape: ") + filename);
    }
}

// DESTRUCTOR

TapeDevice::~TapeDevice() noexcept
{
    if (tape.is_open()) {
        tape.close();
    }
}

// FILE OPERATIONS

int TapeDevice::read()
{
    if (isEnd()) {
        throw ReadFromEmptyTapeException();
    }
    int value;

    tape.read(reinterpret_cast<char *>(&value), sizeof(int));
    if (!tape.good()) {
        throw TapeReadException(filename);
    }
    std::this_thread::sleep_for(readDelay);

    // Move the write pointer to the position of the read pointer
    tape.seekp(tape.tellg());
    return value;
}

void TapeDevice::write(const int value)
{
    // Clear any error state flags and switch to write mode
    tape.clear();
    tape.seekp(0, std::ios::end);

    tape.write(reinterpret_cast<const char *>(&value), sizeof(int));
    tape.flush();
    if (!tape.good()) {
        throw TapeWriteException(filename);
    }
    std::this_thread::sleep_for(writeDelay);

    // Switch back to read mode
    tape.clear();
    tape.seekg(tape.tellp());
}

// TAPE OPERATIONS

void TapeDevice::move() noexcept
{
    tape.seekg(sizeof(int), std::ios::cur);
    std::this_thread::sleep_for(moveDelay);
}

void TapeDevice::stop() noexcept { tape.close(); }

void TapeDevice::rewind() noexcept
{
    tape.seekg(0, std::ios::beg);
    std::this_thread::sleep_for(rewindDelay);
}

void processConfigParameters(const nlohmann::json &config)
{
    if (config.find("readDelay") == config.end() || config.find("writeDelay") == config.end()
        || config.find("moveDelay") == config.end() || config.find("rewindDelay") == config.end()
        || config.find("N") == config.end() || config.find("M") == config.end()) {
        throw MissingConfigParameterException();
    }
}

void setConfigParameters(const nlohmann::json &config, std::chrono::milliseconds &readDelay,
                         std::chrono::milliseconds &writeDelay,
                         std::chrono::milliseconds &moveDelay,
                         std::chrono::milliseconds &rewindDelay, int &tapeLength, int &maxMemory)
{
    readDelay = std::chrono::milliseconds(config["readDelay"]);
    writeDelay = std::chrono::milliseconds(config["writeDelay"]);
    moveDelay = std::chrono::milliseconds(config["moveDelay"]);
    rewindDelay = std::chrono::milliseconds(config["rewindDelay"]);

    tapeLength = config["N"];
    maxMemory = config["M"];
}

void TapeDevice::loadConfig(const std::string &configFile)
{
    std::ifstream file(configFile);
    if (!file.is_open()) {
        throw ConfigLoadException(configFile);
    }

    nlohmann::json config;
    file >> config;

    processConfigParameters(config);
    setConfigParameters(config, readDelay, writeDelay, moveDelay, rewindDelay, tapeLength, maxMemory);

    file.close();
}

void TapeDevice::openTape(const std::string &filename, std::ios_base::openmode mode)
{
    tape.clear();
    tape.open(filename, mode);
    if (!tape) {
        tape.close();
    }
}

// HELPERS

bool TapeDevice::isEnd() noexcept
{
    int value;
    tape.read(reinterpret_cast<char *>(&value), sizeof(int));
    bool isEnd = tape.eof();
    tape.seekg(-sizeof(int), std::ios::cur);
    return isEnd;
}

// SETTERS

void TapeDevice::setReadDelay(const std::chrono::milliseconds &delay) noexcept
{
    readDelay = delay;
}

void TapeDevice::setWriteDelay(const std::chrono::milliseconds &delay) noexcept
{
    writeDelay = delay;
}

// GETTERS

const std::string &TapeDevice::getFilename() const noexcept { return filename; }
