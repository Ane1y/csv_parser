#include <cstring>
#include "csv_parser.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        std::cout << "Usage: " << argv[0] << " file.csv" << std::endl;
        return 1;
    }
    bool suppress_errors = false;
    std::string filename;
    if (strcmp(argv[1],"--suppress-errors") == 0) {
        suppress_errors = true;
        filename = argv[2];
    } else {
        filename = argv[1];
    }
    try {
        CSVParser parser(filename, suppress_errors);
        parser.print_csv(std::cout);
    } catch (const std::exception& e) {
        std::cerr << "An exception occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
