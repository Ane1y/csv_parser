#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stack>
#include <regex>
#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

namespace utils {
    std::string trim(const std::string& str);
}
class CSVParser {
public:
    CSVParser(const std::string& filename);
    void print_csv(std::ostream& out) const;
private:
    // for storing correspondence (column name) -> (column idx) to get address in table in constant time
    std::unordered_map<std::string, size_t> header_map;
    // to store order of columns for printing
    std::vector<std::string> header_order;

    // for storing correspondencce (row number) -> (row idx)
    std::unordered_map<int, size_t> row_map;
    std::vector<int> row_order;

    std::vector<std::vector<std::string>> table;
    std::vector<std::vector<int>> evaluated_table;

    // to find cyclic dependencies
    std::unordered_set<std::string> visited;

    size_t n_col = 0;
    void parse_header(const std::string& line);
    void parse_row(const std::string& line, size_t row_idx);

    void evaluate();

    int evaluate_expression(std::string expression);
    int evaluate_operation(int left_operand, int right_operand, char op);
    int evaluate_cell_value(size_t col_idx, size_t row_idx, const std::string& cell_name);

    // returns col_idx, row_idx for given address in table
    std::pair<size_t, size_t> get_table_inds(const std::string& col_name, int row_idx);

};
