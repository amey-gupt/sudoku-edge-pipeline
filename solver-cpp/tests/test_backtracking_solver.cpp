#include <iostream>
#include <cassert>
#include <string>
#include "sudoku/board.hpp"
#include "sudoku/backtracking_solver.hpp"

#define TEST(name) std::cout << "[TEST] " << name << std::endl;

int main() {

    TEST("solver name");
    sudoku::BacktrackingSolver solver;
    assert(solver.name() == "backtracking_naive");

    TEST("solves easy_01");
    std::string easy01 =
        "5..3..7.."
        "6..195..."
        ".98....6."
        "8...6...3"
        "4..8.3..1"
        "7...2...6"
        ".6....28."
        "...419..5"
        "....8..79";
    sudoku::Board board;
    assert(!board.load_from_string(easy01));
    assert(solver.solve(board));
    assert(board.is_solved());
    assert(board.get(0, 1) == 4);   // spot-check one solved cell

    TEST("already solved board returns true");
    std::string solution =
        "534678912"
        "672195348"
        "198342567"
        "859761423"
        "426853791"
        "713924856"
        "961537284"
        "287419635"
        "345286179";
    sudoku::Board board2;
    assert(!board2.load_from_string(solution));
    assert(solver.solve(board2));
    assert(board2.is_solved());

    TEST("unsolvable puzzle returns false");
    // Known puzzle with no valid completion
    std::string impossible =
        "8........"
        "..36....."
        ".7..9...."
        "....5.7.."
        "........."
        "....1...."
        ".4......."
        "..52....."
        ".......1.";
    sudoku::Board board3;
    assert(!board3.load_from_string(impossible));
    assert(!solver.solve(board3));

    TEST("failed solve leaves clues unchanged");
    sudoku::Board board_fail;
    assert(!board_fail.load_from_string(impossible));  // reuse your impossible string
    assert(board_fail.get(0, 0) == 8);                 // snapshot a clue before solve
    assert(!solver.solve(board_fail));
    assert(board_fail.get(0, 0) == 8);                 // clue still there
    assert(board_fail.is_clue(0, 0));                  // still marked as clue
    // No spurious digits left behind from partial search
    assert(board_fail.get(0, 1) == 0);                   // (0,1) was empty in impossible puzzle

    TEST("original clues unchanged after solve");
    sudoku::Board board4;
    assert(!board4.load_from_string(easy01));
    assert(board4.is_clue(0, 0));   // '5' is a given
    assert(solver.solve(board4));
    assert(board4.is_clue(0, 0));
    assert(board4.get(0, 0) == 5); // clue value unchanged

    TEST("solves puzzle loaded from file");
    sudoku::Board board5;
    auto err = board5.load_from_file(PROJECT_ROOT "/data/puzzles/medium_01.txt");
    assert(!err);
    assert(solver.solve(board5));
    assert(board5.is_solved());

    
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}