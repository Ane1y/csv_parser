#include "csv_parser.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " file.csv" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    try {
        CSVParser parser(filename);
        parser.print_csv(std::cout);
    } catch (const std::exception& e) {
        std::cerr << "An exception occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
