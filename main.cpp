#include "TapeDevice.hpp"
#include "TapeExceptions.hpp"
#include "TapeSorter.hpp"
#include <fstream>
#include <iostream>

const int EXPECTED_ARG_COUNT = 3;
const char *CONFIG_FILE_NAME = "config.json";
const int NUM_TEMP_TAPES = 100;

bool isValidString(const char *line)
{
    while (*line) {
        if (!std::isprint(*line)) {
            return false;
        }
        line++;
    }
    return true;
}

std::pair<const char *, const char *> parseArguments(int argc, char *argv[])
{
    if (argc != EXPECTED_ARG_COUNT) {
        throw InvalidNumberOfArgumentsException(EXPECTED_ARG_COUNT - 1, argc - 1);
    }

    if (!isValidString(argv[1]) || !isValidString(argv[2])) {
        throw InvalidFileNameException();
    }

    return {argv[1], argv[2]};
}

void deleteTapeDevices(TapeDevice *inputTape, TapeDevice *outputTape)
{
    delete inputTape;
    delete outputTape;
}

std::pair<TapeDevice *, TapeDevice *> createTapeDevices(const std::string &inputFileName,
                                                        const std::string &outputFileName)
{
    TapeDevice *inputTape = nullptr;
    TapeDevice *outputTape = nullptr;

    try {
        inputTape = new TapeDevice(inputFileName);
        outputTape = new TapeDevice(outputFileName);

        // Load configuration from a predefined JSON file
        inputTape->loadConfig(CONFIG_FILE_NAME);
        outputTape->loadConfig(CONFIG_FILE_NAME);
    } catch (const TapeReadException &e) {
        deleteTapeDevices(inputTape, outputTape);
        throw;
    }

    return {inputTape, outputTape};
}

int main(int argc, char *argv[])
{
    TapeDevice *inputTape = nullptr;
    TapeDevice *outputTape = nullptr;

    try {
        auto [inputFileName, outputFileName] = parseArguments(argc, argv);
        auto [input, output] = createTapeDevices(inputFileName, outputFileName);
        inputTape = input;
        outputTape = output;

        TapeSorter sorter(inputTape, outputTape, NUM_TEMP_TAPES);
        sorter.sort();

    } catch (const std::ios_base::failure &e) {
        deleteTapeDevices(inputTape, outputTape);
        throw FileOpenException();
    } catch (const std::runtime_error &e) {
        deleteTapeDevices(inputTape, outputTape);
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return 1;
    }

    deleteTapeDevices(inputTape, outputTape);
    return 0;
}