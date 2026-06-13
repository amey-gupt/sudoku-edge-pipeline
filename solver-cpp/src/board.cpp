#include "sudoku/board.hpp"

using namespace sudoku;

Board::Board() = default;

std::optional<std::string> Board::load_from_string(const std::string& text) {
    std::string flat;
    for (char c : text) {
        if (c == '\n' || c == '\r') continue;
        flat += c;
    }

    if (flat.size() != 81) {
        return std::string("expected 81 cells, got ") + std::to_string(flat.size());
    }

    for (size_t i = 0; i < 81; ++i) {
        char c = flat[i];
        size_t row = i / 9, col = i % 9;
        if (c == '.') {
            grid_[row][col] = 0;
        } else if (c >= '1' && c <= '9') {
            grid_[row][col] = c - '0';
        } else {
            return "invalid character '" + std::string(1, c) + "' at index " + std::to_string(i);
        }
    }

    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (grid_[r][c] != 0 && !is_valid_placement(r, c, grid_[r][c]))
                return "conflicting clue at row " + std::to_string(r) + " col " + std::to_string(c);

    return std::nullopt;  // success
}

bool Board::is_valid_placement(int row, int col, int digit) const {
    if (digit < 1 || digit > 9) return false;
    for (int c = 0; c < 9; ++c)
        if (c != col && grid_[row][c] == digit) return false;
    for (int r = 0; r < 9; ++r)
        if (r != row && grid_[r][col] == digit) return false;
    int br = (row / 3) * 3, bc = (col / 3) * 3;
    for (int r = br; r < br + 3; ++r)
        for (int c = bc; c < bc + 3; ++c)
            if ((r != row || c != col) && grid_[r][c] == digit) return false;
    return true;
}

bool Board::is_complete() const {
    for (const auto& row : grid_)
        for (int v : row)
            if (v == 0) return false;
    return true;
}

bool Board::is_solved() const {
    if (!is_complete()) return false;
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (!is_valid_placement(r, c, grid_[r][c])) return false;
    return true;
}