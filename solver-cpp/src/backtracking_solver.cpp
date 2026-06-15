#include "sudoku/backtracking_solver.hpp"

using namespace sudoku;


namespace {
    bool backtrack(Board& board) {
        auto empty = board.find_empty();
        if (!empty) {
            return board.is_solved();
        }

        const int row = empty->first;
        const int col = empty->second;

        for (int digit = 1; digit <= Board::SIZE; ++digit) {
            if (!board.is_valid_placement(row, col, digit)) {
                continue;  // prune: skip invalid digits early
            }
            if (!board.set(row, col, digit)) {
                continue;  // should not happen on empty cells, but safe
            }
            if (backtrack(board)) {
                return true;
            }
            board.clear(row, col);  // undo choice (backtrack)
        }

        return false;
    }
}

bool BacktrackingSolver::solve(Board& board) {
    return backtrack(board);
}
std::string BacktrackingSolver::name() const {
    return "backtracking_naive";
}