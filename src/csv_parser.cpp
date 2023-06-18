#include "csv_parser.hpp"

CSVParser::CSVParser(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::string line;

    if (std::getline(file, line)) {
        parse_header(line);
    }

    size_t row_cnt = 0;
    while (std::getline(file, line)) {
        parse_row(line, row_cnt);
        ++row_cnt;
    }

    file.close();

    evaluated_table.resize(row_cnt);
    for (size_t i = 0; i < row_cnt; ++i) {
        evaluated_table[i].resize(n_col, std::numeric_limits<int>::max());
    }
    evaluate();
}

void CSVParser::print_csv(std::ostream& out) const {
        for (const auto& header : header_order) {
            out << "," << header;
        }
    out << std::endl;

        auto it = row_order.begin();
        for (size_t i = 0; i < table.size(); ++i) {
            for (size_t j = 0; j < table[i].size(); ++j) {
                if (j == 0) {
                    out << *it;
                    it++;
                }
                out << "," << evaluated_table[i][j];
            }
            out << std::endl;
        }
}

void CSVParser::parse_header(const std::string& line) {
    std::stringstream ss(line);
    std::string cell;
    size_t col = 0;

    // pass the first because it is row num
    std::getline(ss, cell, ',');
    while (std::getline(ss, cell, ',')) {
        cell = utils::trim(cell);
        if (header_map.find(cell) != header_map.end()) {
            throw std::runtime_error("csv file contains duplicated column names: " +  cell);
        }

        header_map.insert({cell, col});
        header_order.push_back(cell);
        col++;
    }
    n_col = header_map.size();
}

void CSVParser::parse_row(const std::string& line, size_t row_idx) {
    std::stringstream ss(line);
    std::string cell;
    std::vector<std::string> row;
    row.reserve(n_col + 1);

    if (std::getline(ss, cell, ',')) {
        cell = utils::trim(cell);
        int row_num = std::stoi(cell);

        if (row_map.find(row_num) != row_map.end()) {
            throw std::runtime_error("csv file contains duplicated row numbers: " +  cell);
        }
        row_map.insert({row_num, row_idx});
        row_order.push_back(row_num);
    }

    while (std::getline(ss, cell, ',')) {
        cell = utils::trim(cell);
        if (cell[0] != '=') {
            try {
                 [[maybe_unused]] auto value = std::stoi(cell);
            } catch (const std::invalid_argument& e) {
                throw std::runtime_error("one of the cell contains neither number nor address");
            }
        }
        row.push_back(cell);
    }

    if (row.size()  != header_map.size()) {
        throw std::runtime_error("Corrupted CSV:" + std::to_string(row_idx) + " Row size does not match header size");
    }
    table.push_back(row);
}

// TODO problems with cyclic dependencies
void CSVParser::evaluate() {
    for (size_t row_idx = 0; row_idx < table.size(); row_idx++) {
        for (size_t col_idx = 0; col_idx < table[row_idx].size(); col_idx++) {
            const std::string& value = table[row_idx][col_idx];
            evaluated_table[row_idx][col_idx] = evaluate_expression(value);
            visited.clear();
        }
    }
}

int CSVParser::evaluate_operation(int left_operand, int right_operand, char op) {
    switch (op) {
        case '+':
            return left_operand + right_operand;
        case '-':
            return left_operand - right_operand;
        case '*':
            return left_operand * right_operand;
        case '/':
            if (right_operand == 0) {
                throw std::runtime_error("Try to divide by zero");
            }
            return left_operand / right_operand;
        default:
            throw std::runtime_error("Invalid operator: " + std::string(1, op));
    }
}

int CSVParser::evaluate_cell_value(size_t row_idx, size_t col_idx, const std::string& cell_name) {
    if (evaluated_table[row_idx][col_idx] != std::numeric_limits<int>::max()) {
        return evaluated_table[row_idx][col_idx];
    }
    if (visited.find(cell_name) != visited.end()) {
        throw std::runtime_error("Cyclic dependency detected for cell: " + cell_name);
    }

    visited.insert(cell_name);
    std::string expression = table[row_idx][col_idx];
    return evaluate_expression(expression);
}



int CSVParser::evaluate_expression(std::string expression) {
    if (expression[0] != '=') {
        return std::stoi(expression);
    }

    std::regex cell_reference_regex("([A-Za-z]+)(\\d+)");
    std::smatch cell_match;

    while (std::regex_search(expression, cell_match, cell_reference_regex)) {
        auto [col_idx, row_idx] = get_table_inds(cell_match[1], std::stoi(cell_match[2]));
        std::string cell_name = cell_match[0].str();
        int cell_value = evaluate_cell_value(row_idx, col_idx, cell_name);
        expression = std::regex_replace(expression, std::regex(cell_name), ' ' + std::to_string(cell_value) + ' ');
    }

    std::stack<int> values;
    std::stack<char> ops;

    std::istringstream iss(expression.substr(1));
    std::string token;

    while (iss >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            ops.push(token[0]);
        } else {
            int value = std::stoi(token);
            values.push(value);
        }
    }

    while (!ops.empty()) {
        char op = ops.top();
        ops.pop();

        int right_operand = values.top();
        values.pop();

        int left_operand = values.top();
        values.pop();

        int result = evaluate_operation(left_operand, right_operand, op);
        values.push(result);
    }

    return values.top();
}

std::pair<size_t, size_t> CSVParser::get_table_inds(const std::string& col_name, int row_idx) {
    if (header_map.find(col_name) == header_map.end() || row_map.find(row_idx) == row_map.end()) {
        throw std::runtime_error("Tried to access to non-existent cell " + col_name + std::to_string(row_idx));
    }
    return {header_map[col_name], row_map[row_idx]};
}

std::string utils::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    return (first != std::string::npos && last != std::string::npos)
           ? str.substr(first, last - first + 1)
           : std::string();
}

