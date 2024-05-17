#include "TapeSorter.hpp"
#include "TapeUtils.hpp"
#include <algorithm>
#include <iostream>
#include <queue>

TapeSorter::TapeSorter(TapeDevice *inputTape, TapeDevice *outputTape, int numTempTapes)
    : inputTape(inputTape), outputTape(outputTape)
{
    createTempTapes(numTempTapes);
}

TapeSorter::~TapeSorter() noexcept
{
    for (auto &tape : tempTapes) {
        tape->stop();
        std::remove(tape->filename.c_str());
    }
    tempTapes.clear();
}

void TapeSorter::createTempTapes(const int numTapes)
{
    std::filesystem::path tmpDirectory = TapeUtils::createTmpDirectory();
    std::vector<std::unique_ptr<TapeDevice>> newTapes;

    try {
        TapeUtils::createTapeFiles(numTapes, tmpDirectory, newTapes);
    } catch (...) {
        for (auto &tape : newTapes) {
            std::remove(tape->filename.c_str());
        }
        throw;
    }
    for (auto &tape : newTapes) {
        tempTapes.push_back(std::move(tape));
    }
}

// Merge sort algorithm
void TapeSorter::sort()
{
    divideAndSortBlocks();
    mergeSortedBlocks();
}

void TapeSorter::divideAndSortBlocks()
{
    std::vector<int> block;
    int numTapes = 0;
    while (!inputTape->isEnd()) {
        readBlock(block);
        if (block.size() * sizeof(int) >= inputTape->maxMemory) {
            sortAndWriteBlock(block, numTapes);
            block.clear();
            numTapes++;
        }
    }
    // Handle the last block if it's not empty
    if (!block.empty()) {
        sortAndWriteBlock(block, numTapes);
        numTapes++;
    }
}

void TapeSorter::readBlock(std::vector<int> &block) { block.push_back(inputTape->read()); }

void TapeSorter::sortAndWriteBlock(std::vector<int> &block, int tapeIndex)
{
    try {
        if (tapeIndex >= tempTapes.size()) {
            throw NotEnoughTempTapesException();
        }
        std::sort(block.begin(), block.end());
        for (int value : block) {
            tempTapes[tapeIndex]->write(value);
        }
        tempTapes[tapeIndex]->rewind();
    } catch (...) {
        block.clear();
        throw;
    }
}

void TapeSorter::mergeSortedBlocks() noexcept
{
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>>
        minHeap;
    for (int i = 0; i < tempTapes.size(); ++i) {
        if (!tempTapes[i]->isEnd()) {
            minHeap.push({tempTapes[i]->read(), i});
        }
    }
    while (!minHeap.empty()) {
        auto [value, tapeIndex] = minHeap.top();
        minHeap.pop();
        outputTape->write(value);
        if (!tempTapes[tapeIndex]->isEnd()) {
            minHeap.push({tempTapes[tapeIndex]->read(), tapeIndex});
        }
    }
}