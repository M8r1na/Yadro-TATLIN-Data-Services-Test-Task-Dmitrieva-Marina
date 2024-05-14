#include "TapeDevice.hpp"
#include "TapeExceptions.hpp"
#include "TapeSorter.hpp"
#include <fstream>
#include <gtest/gtest.h>

TEST(TapeSorterTest, SortTest) {
    std::ofstream input_tape_file("input_tape");
    input_tape_file.close();

    std::ofstream output_tape_file("output_tape");
    output_tape_file.close();

    TapeDevice inputTape("input_tape");
    for (int i = 9; i >= 0; --i) {
        inputTape.write(i);
    }
    inputTape.rewind();

    TapeDevice outputTape("output_tape");
    TapeSorter sorter(&inputTape, &outputTape, 10);
    sorter.sort();

    // Check the output tape
    outputTape.rewind();
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(i, outputTape.read());
    }

    std::remove("input_tape");
    std::remove("output_tape");
}

TEST(TapeDeviceTest, ReadWriteTest) {
    std::ofstream test_tape("test_tape");
    test_tape.close();

    TapeDevice tape("test_tape");

    // Write values to the tape
    for (int i = 0; i < 10; ++i) {
        tape.write(i);
    }

    // Rewind the tape and read the values
    tape.rewind();
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(i, tape.read());
    }

    std::remove("test_tape");
}

TEST(TapeDeviceTest, MoveTest) {
    std::ofstream test_tape("test_tape");
    test_tape.close();

    TapeDevice tape("test_tape");

    // Write values to the tape
    for (int i = 0; i < 10; ++i) {
        tape.write(i);
    }

    // Rewind the tape and move the head
    tape.rewind();
    tape.move();

    // The first value should be skipped
    for (int i = 1; i < 10; ++i) {
        EXPECT_EQ(i, tape.read());
    }

    std::remove("test_tape");
}

TEST(TapeDeviceTest, IsEndTest) {
    std::ofstream test_tape("test_tape");
    test_tape.close();

    TapeDevice tape("test_tape");

    // Write values to the tape
    for (int i = 0; i < 10; ++i) {
        tape.write(i);
    }

    // Rewind the tape and read all the values
    tape.rewind();
    for (int i = 0; i < 10; ++i) {
        tape.read();
    }

    // The tape should be at the end
    EXPECT_TRUE(tape.isEnd());

    std::remove("test_tape");
}

TEST(TapeSorterTest, EmptyTapeTest) {
    std::ofstream input_tape_file("input_tape");
    input_tape_file.close();

    std::ofstream output_tape_file("output_tape");
    output_tape_file.close();

    TapeDevice inputTape("input_tape");
    TapeDevice outputTape("output_tape");
    TapeSorter sorter(&inputTape, &outputTape, 2);
    sorter.sort();

    // The output tape should be empty
    EXPECT_TRUE(outputTape.isEnd());

    std::remove("input_tape");
    std::remove("output_tape");
}

TEST(TapeDeviceTest, ReadFromEmptyTapeTest) {
    std::ofstream test_tape("test_tape");
    test_tape.close();

    TapeDevice tape("test_tape");
    EXPECT_THROW(tape.read(), ReadFromEmptyTapeException);

    std::remove("test_tape");
}

TEST(TapeDeviceTest, WriteDelayTest) {
    std::ofstream test_tape("test_tape");
    test_tape.close();

    TapeDevice tape("test_tape");
    tape.setWriteDelay(std::chrono::milliseconds(1000));

    auto start = std::chrono::high_resolution_clock::now();
    tape.write(1);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    EXPECT_TRUE(elapsed.count() >= 1000);

    std::remove("test_tape");
}

TEST(TapeSorterTest, SortEmptyTapeTest) {
    std::ofstream input_tape_file("input_tape");
    input_tape_file.close();
    std::ofstream output_tape_file("output_tape");
    output_tape_file.close();

    TapeDevice inputTape("input_tape");
    TapeDevice outputTape("output_tape");
    TapeSorter sorter(&inputTape, &outputTape, 2);
    sorter.sort();

    outputTape.rewind();
    EXPECT_THROW(outputTape.read(), ReadFromEmptyTapeException);

    std::remove("input_tape");
    std::remove("output_tape");
}

TEST(TapeSorterTest, SortSingleElementTapeTest) {
    std::ofstream input_tape_file("input_tape");
    input_tape_file.close();
    std::ofstream output_tape_file("output_tape");
    output_tape_file.close();

    TapeDevice inputTape("input_tape");
    inputTape.write(1);
    inputTape.rewind();

    TapeDevice outputTape("output_tape");
    TapeSorter sorter(&inputTape, &outputTape, 2);
    sorter.sort();

    outputTape.rewind();
    EXPECT_EQ(1, outputTape.read());
    EXPECT_THROW(outputTape.read(), ReadFromEmptyTapeException);

    std::remove("input_tape");
    std::remove("output_tape");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}