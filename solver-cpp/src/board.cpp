#include "sudoku/board.hpp"
#include <fstream>
#include <stdexcept>

using namespace sudoku;

std::optional<std::string> Board::load_from_string(const std::string& text) {
    std::array<std::array<int, SIZE>, SIZE> tmp {};
    std::array<std::array<bool, SIZE>, SIZE> tmp_clue {};
    std::string flat;
    for (char c : text) {
        if (c == '\n' || c == '\r') continue;
        flat += c;
    }

    if (flat.size() != SIZE * SIZE) {
        return std::string("expected 81 cells, got ") + std::to_string(flat.size());
    }

    for (size_t i = 0; i < SIZE * SIZE; ++i) {
        char c = flat[i];
        size_t row = i / SIZE, col = i % SIZE;
        if (c == '.') {
            tmp_clue[row][col] = false;
            tmp[row][col] = 0;
        } else if (c >= '1' && c <= '9') {
            tmp_clue[row][col] = true;
            tmp[row][col] = c - '0';
        } else {
            return "invalid character '" + std::string(1, c) + "' at index " + std::to_string(i);
        }
    }

    

    for (int r = 0; r < SIZE; ++r)
        for (int c = 0; c < SIZE; ++c)
            if (tmp[r][c] != 0 && !is_valid_placement(r, c, tmp[r][c], tmp))
                return "conflicting clue at row " + std::to_string(r) + " col " + std::to_string(c);

    grid_ = tmp;
    clue_ = tmp_clue;
    return std::nullopt;  // success
}

bool Board::is_valid_placement(int row, int col, int digit, const std::array<std::array<int, SIZE>, SIZE>& grid) const {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) return false;
    if (digit < 1 || digit > 9) return false;
    return (!row_contains(row, digit, col, grid) && !col_contains(col, digit, row, grid) && !box_contains(row, col, digit, grid));
}

bool Board::is_valid_placement(int row, int col, int digit) const {
    return is_valid_placement(row, col, digit, grid_);
}

bool Board::is_complete() const {
    for (const auto& row : grid_)
        for (int v : row)
            if (v == 0) return false;
    return true;
}

bool Board::is_solved() const {
    if (!is_complete()) return false;
    for (int r = 0; r < SIZE; ++r)
        for (int c = 0; c < SIZE; ++c)
            if (!is_valid_placement(r, c, grid_[r][c])) return false;
    return true;
}

int Board::get(int row, int col) const {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) return -1;
    return grid_[row][col];
}

bool Board::set(int row, int col, int digit, bool validate) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) return false;
    if (digit < 1 || digit > 9) return false;
    if (validate && !is_valid_placement(row, col, digit)) return false;
    if (is_clue(row, col)) return false;
    grid_[row][col] = digit;
    return true;
}

bool Board::clear(int row, int col) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) return false;
    if (is_clue(row, col)) return false;
    grid_[row][col] = 0;
    return true;
}

std::optional<std::string> Board::load_from_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return "failed to open file " + path;

    std::string content{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    return load_from_string(content);
}

std::string Board::to_string() const {
    std::string result;
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (grid_[r][c] == 0) result += '.';
            else result += std::to_string(grid_[r][c]);
        }
        result += '\n';
    }
    return result;
}

Board::Board(const std::string& flat) {
    auto err = load_from_string(flat);
    if (err) throw std::invalid_argument(*err); 
}

bool Board::row_contains(int row, int digit, int skip_col, const std::array<std::array<int, SIZE>, SIZE>& grid) const {
    for (int c = 0; c < SIZE; ++c)
        if (c != skip_col && grid[row][c] == digit) return true;
    return false;
}

bool Board::col_contains(int col, int digit, int skip_row, const std::array<std::array<int, SIZE>, SIZE>& grid) const {
    for (int r = 0; r < SIZE; ++r)
        if (r != skip_row && grid[r][col] == digit) return true;
    return false;
}

bool Board::box_contains(int row, int col, int digit, const std::array<std::array<int, SIZE>, SIZE>& grid) const {
    const int start_row = (row / BOX) * BOX;
    const int start_col = (col / BOX) * BOX;
    for (int r = start_row; r < start_row + BOX; ++r) {
        for (int c = start_col; c < start_col + BOX; ++c) {
            if (r == row && c == col) continue;
            if (grid[r][c] == digit) return true;
        }
    }
    return false;
}

std::optional<std::pair<int,int>> Board::find_empty() const {
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (grid_[r][c] == 0) return std::make_pair(r, c);
        }
    }
    return std::nullopt;
}
