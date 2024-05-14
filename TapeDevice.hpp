#ifndef TAPE_DEVICE_HPP
#define TAPE_DEVICE_HPP

#include "TapeInterface.hpp"
#include <chrono>
#include <fstream>
#include <nlohmann/json.hpp>
#include <thread>

const std::chrono::milliseconds DEFAULT_DELAY = std::chrono::milliseconds(0);

class TapeDevice : public TapeInterface {
  private:
    std::fstream tape;

    std::chrono::milliseconds readDelay = DEFAULT_DELAY;
    std::chrono::milliseconds writeDelay = DEFAULT_DELAY;
    std::chrono::milliseconds moveDelay = DEFAULT_DELAY;
    std::chrono::milliseconds rewindDelay = DEFAULT_DELAY;

    int tapeLength;
    int maxMemory;
    std::string filename;

    friend class TapeSorter;

  public:
    TapeDevice(const std::string &filename);
    ~TapeDevice() noexcept;

    int read() override;
    void write(const int value) override;
    void move() noexcept override;
    void stop() noexcept override;
    void rewind() noexcept override;
    void loadConfig(const std::string &configFile) override;
    bool isEnd() noexcept;
    void openTape(const std::string &filename, std::ios_base::openmode mode);

    void setReadDelay(const std::chrono::milliseconds &delay) noexcept;
    void setWriteDelay(const std::chrono::milliseconds &delay) noexcept;

    [[nodiscard]] const std::string &getFilename() const noexcept;
};

#endif // TAPE_DEVICE_HPP