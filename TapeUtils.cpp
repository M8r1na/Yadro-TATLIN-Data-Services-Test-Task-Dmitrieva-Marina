#include "TapeUtils.hpp"
#include "TapeExceptions.hpp"
#include <fstream>

namespace TapeUtils {
std::filesystem::path createTmpDirectory() {
    std::filesystem::path path("tmp");
    if (!std::filesystem::exists(path)) {
        std::error_code errorCode;
        if (!std::filesystem::create_directory(path, errorCode)) {
            throw CreateTmpDirectoryException();
        }
    }
    return path;
}

void createTapeFiles(int numTapes, const std::filesystem::path &tmpDirectory,
                     std::vector<std::unique_ptr<TapeDevice>> &newTapes) {
    for (int number = 0; number < numTapes; ++number) {
        std::string filename =
            tmpDirectory.string() + "/tmp" + std::to_string(number);

        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            throw CreateFileException(filename);
        }
        file.close();
        newTapes.push_back(std::make_unique<TapeDevice>(filename));
    }
}
} // namespace TapeUtils