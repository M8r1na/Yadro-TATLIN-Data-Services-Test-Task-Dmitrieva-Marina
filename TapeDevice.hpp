#ifndef TAPE_DEVICE_HPP
#define TAPE_DEVICE_HPP

#include "TapeInterface.hpp"
#include <chrono>
#include <fstream>
#include <nlohmann/json.hpp>
#include <thread>

const std::chrono::milliseconds DEFAULT_DELAY = std::chrono::milliseconds(0);

class TapeDevice : public TapeInterface
{
  private:
    std::fstream tape;

    // Delay between read/write operations
    std::chrono::milliseconds readDelay = DEFAULT_DELAY;
    std::chrono::milliseconds writeDelay = DEFAULT_DELAY;
    std::chrono::milliseconds moveDelay = DEFAULT_DELAY;
    std::chrono::milliseconds rewindDelay = DEFAULT_DELAY;

    // Tape parameters
    int tapeLength;
    int maxMemory;
    std::string filename;

    friend class TapeSorter;

  public:
    // CONSTRUCTORS
    TapeDevice(const std::string &filename);

    // DESTRUCTOR
    ~TapeDevice() noexcept;

    // FILE OPERATIONS
    int read() override;
    void write(const int value) override;

    // TAPE OPERATIONS
    void move() noexcept override;
    void stop() noexcept override;
    void rewind() noexcept override;
    void loadConfig(const std::string &configFile) override;
    void openTape(const std::string &filename, std::ios_base::openmode mode);

    // HELPERS
    [[nodiscard]]bool isEnd() noexcept;

    // SETTERS
    void setReadDelay(const std::chrono::milliseconds &delay) noexcept;
    void setWriteDelay(const std::chrono::milliseconds &delay) noexcept;

    // GETTERS
    [[nodiscard]] const std::string &getFilename() const noexcept;
};

#endif  // TAPE_DEVICE_HPP