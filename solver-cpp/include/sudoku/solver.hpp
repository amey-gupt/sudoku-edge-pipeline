#pragma once

#include "sudoku/board.hpp"
#include <string>

namespace sudoku {

    struct Solver {
        virtual ~Solver() = default;
        virtual bool solve(Board& board) = 0;
        virtual std::string name() const = 0;
    };
}