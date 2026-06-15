#pragma once

#include "sudoku/solver.hpp"
#include <string>

namespace sudoku {
    class BacktrackingSolver : public Solver {
        public:
            bool solve(Board& board) override;
            std::string name() const override;


    };
}