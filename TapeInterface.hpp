#ifndef TAPE_INTERFACE_HPP
#define TAPE_INTERFACE_HPP

#include <string>

class TapeInterface
{
  public:
    virtual ~TapeInterface() {}

    virtual int read() = 0;
    virtual void write(const int value) = 0;
    virtual void move() = 0;
    virtual void stop() = 0;
    virtual void rewind() = 0;
    virtual void loadConfig(const std::string &configFile) = 0;
};

#endif  // TAPE_INTERFACE_HPP