#include <gtest/gtest.h>
#include "csv_parser.hpp"

TEST(CSVParserTest, SimpleTest) {
    CSVParser parser("../data/file.csv");
    std::stringstream out;

    std::ifstream file("../data/expected.csv");
    std::stringstream expected;
    expected << file.rdbuf();

    parser.print_csv(out);
    ASSERT_EQ(out.str(), expected.str());
}

TEST(CSVParserTest, CyclicDependency) {
    std::string expected_msg = "Cyclic dependency detected for cell: A8";
    try {
        CSVParser parser("../data/cycle.csv");
        FAIL() << "Expected: " << expected_msg;
    }
    catch(const std::runtime_error& e) {
        EXPECT_EQ(e.what(), expected_msg);
    }
    catch(...) {
        FAIL() << "Expected: " << expected_msg;
    }

}

TEST(CSVParserTest, WrongAddress) {
    std::string expected_msg = "Tried to access to non-existent cell A80";
    try {
        CSVParser parser("../data/wrong_address.csv");
        FAIL() << "Expected: " << expected_msg;
    }
    catch(const std::runtime_error& e) {
        EXPECT_EQ(e.what(), expected_msg);
    }
    catch(...) {
        FAIL() << "Expected: " << expected_msg;
    }
}

TEST(CSVParserTest, WrongShape) {
    std::string expected_msg = "Corrupted CSV:0 Row size does not match header size";
    try {
        CSVParser parser("../data/wrong_shape.csv");
        FAIL() << "Expected: " << expected_msg;
    }
    catch(const std::runtime_error& e) {
        EXPECT_EQ(e.what(), expected_msg);
    }
    catch(...) {
        FAIL() << "Expected: " << expected_msg;
    }
}


TEST(CSVParserTest, IncorrectValues) {
    std::string expected_msg = "one of the cell contains neither number nor address";
    try {
        CSVParser parser("../data/incorrect_values.csv");
        FAIL() << "Expected: " << expected_msg;
    }
    catch(const std::runtime_error& e) {
        EXPECT_EQ(e.what(), expected_msg);
    }
    catch(...) {
        FAIL() << "Expected: " << expected_msg;
    }
}

TEST(CSVParserTest, RandomSpaces) {
    CSVParser parser("../data/random_spaces.csv");
    std::stringstream out;

    std::ifstream file("../data/expected.csv");
    std::stringstream expected;
    expected << file.rdbuf();

    parser.print_csv(out);
    ASSERT_EQ(out.str(), expected.str());
}

TEST(CSVParserTest, SameNames) {
    std::string expected_msg = "csv file contains dublicated column names: A";
    try {
        CSVParser parser("../data/same_names.csv");
        FAIL() << "Expected: " << expected_msg;
    }
    catch(const std::runtime_error& e) {
        EXPECT_EQ(e.what(), expected_msg);
    }
    catch(...) {
        FAIL() << "Expected: " << expected_msg;
    }
}


