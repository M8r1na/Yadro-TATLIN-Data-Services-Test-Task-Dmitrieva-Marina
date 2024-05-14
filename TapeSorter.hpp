#ifndef SORTER_HPP
#define SORTER_HPP

#include "TapeDevice.hpp"
#include "TapeExceptions.hpp"
#include <memory>
#include <vector>

class TapeSorter {
  private:
    void createTempTapes(const int numTapes);
    void divideAndSortBlocks();
    void readBlock(std::vector<int> &block);
    void sortAndWriteBlock(std::vector<int> &block, int tapeIndex);
    void mergeSortedBlocks() noexcept;

    TapeDevice *inputTape;
    TapeDevice *outputTape;
    std::vector<std::unique_ptr<TapeDevice>> tempTapes;

  public:
    TapeSorter(TapeDevice *inputTape, TapeDevice *outputTape, int numTempTapes);
    ~TapeSorter() noexcept;
    void sort();
};

#endif // SORTER_HPP