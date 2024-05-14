#ifndef TAPE_UTILS_HPP
#define TAPE_UTILS_HPP

#include "TapeDevice.hpp"
#include <filesystem>
#include <vector>

namespace TapeUtils {

std::filesystem::path createTmpDirectory();
void createTapeFiles(int numTapes, const std::filesystem::path &tmpDirectory,
                     std::vector<std::unique_ptr<TapeDevice>> &newTapes);

} // namespace TapeUtils

#endif // TAPE_UTILS_HPP