#include <iostream>
#include <cassert>
#include "sudoku/board.hpp"

#define TEST(name) std::cout << "[TEST] " << name << std::endl;

int main() {
    
    TEST("empty board loads");
    sudoku::Board b1;
    auto err = b1.load_from_string(std::string(81, '.'));
    assert(!err);                         // nullopt = success
    assert(b1.get(0, 0) == 0);           // dot maps to 0
    assert(b1.get(8, 8) == 0);           // corners
    assert(!b1.is_complete());            // no cells filled
    assert(!b1.is_solved());              // unsolved

    TEST("valid clue puzzle loads");
    // easy_01.txt row 0: 5..3..7..
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
    sudoku::Board b2;
    auto err2 = b2.load_from_string(easy01);

    assert(b2.is_clue(0, 0));    // top-left is a given clue
    assert(!b2.is_clue(1, 1));   // dot cell is not a clue
    assert(!err2);
    assert(b2.get(0, 0) == 5);   // top-left clue
    assert(b2.get(0, 3) == 3);   // fourth cell of row 0
    assert(b2.get(0, 2) == 0);   // dot at (0,2) → 0
    assert(!b2.is_complete());    // still has empty cells

    TEST("wrong length rejected");
    sudoku::Board b3;
    auto err3 = b3.load_from_string("12345");
    assert(err3.has_value());    // must return an error

    TEST("invalid character rejected");
    sudoku::Board b4;
    std::string bad = std::string(80, '.') + "X";   // 81 chars, last is invalid
    auto err4 = b4.load_from_string(bad);
    assert(err4.has_value());

    TEST("conflicting clues rejected");
    sudoku::Board b5;
    // Two 5s in row 0
    std::string conflict =
        "55.3..7.."
        "6..195..."
        ".98....6."
        "8...6...3"
        "4..8.3..1"
        "7...2...6"
        ".6....28."
        "...419..5"
        "....8..79";
    auto err5 = b5.load_from_string(conflict);
    assert(err5.has_value());

    TEST("is_valid_placement");
    sudoku::Board b6;
    b6.load_from_string(easy01);     // reuse the string from group 2
    // Row 0 has 5 at col 0: placing 5 elsewhere in row 0 must be invalid
    assert(!b6.is_valid_placement(0, 2, 5));   // row conflict
    // Col 0 has 5 (row 0) and 6 (row 1): placing 6 in col 0 elsewhere is invalid
    assert(!b6.is_valid_placement(2, 0, 6));   // col conflict
    // Check a valid placement: cell (0,2) is empty; 4 is not in row 0, col 2, or box 0
    assert(b6.is_valid_placement(0, 2, 4));

    TEST("set and clear");
    sudoku::Board b7;
    b7.load_from_string(std::string(81, '.'));
    b7.set(4, 4, 7);
    assert(b7.get(4, 4) == 7);
    b7.clear(4, 4);
    assert(b7.get(4, 4) == 0);
    // Out-of-bounds get returns -1
    assert(b7.get(-1, 0) == -1);
    assert(b7.get(9, 9) == -1);
    // set out-of-bounds does nothing (no crash)
    b7.set(10, 10, 5);    // should silently return

    TEST("is_solved on known solution");
    // The canonical solution to easy_01
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
    sudoku::Board b8;
    auto err8 = b8.load_from_string(solution);
    assert(!err8);
    assert(b8.is_complete());
    assert(b8.is_solved());

    TEST("conflicting full board is rejected by load");
    sudoku::Board b9a;
    auto err9a = b9a.load_from_string(conflict);
    assert(err9a.has_value());   // load must fail

    TEST("complete but invalid board is not solved");
    // Build an invalid complete board via set() so load validation is bypassed
    sudoku::Board b9b;
    b9b.load_from_string(std::string(81, '.'));
    // Fill row-by-row with a pattern that completes but has row 0 duplicate:
    // "113456789" -- two 1s in row 0
    int invalid_grid[9][9] = {
        {1,1,3,4,5,6,7,8,9},
        {4,5,6,7,8,9,1,2,3},
        {7,8,9,1,2,3,4,5,6},
        {2,1,4,3,6,5,8,9,7},
        {3,6,5,8,9,7,2,1,4},
        {8,9,7,2,1,4,3,6,5},
        {5,3,1,6,4,2,9,7,8},
        {6,4,2,9,7,8,5,3,1},
        {9,7,8,5,3,1,6,4,2}
    };
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            b9b.set(r, c, invalid_grid[r][c]);
    assert(b9b.is_complete());   // all 81 filled
    assert(!b9b.is_solved());    // but invalid — THIS is the real test

    TEST("load_from_file");
    sudoku::Board b10;
    auto err10 = b10.load_from_file(PROJECT_ROOT "/data/puzzles/easy_01.txt");
    assert(!err10);
    assert(b10.get(0, 0) == 5);
    assert(b10.get(1, 3) == 1);

    TEST("set cannot overwrite a clue");
    sudoku::Board b;
    b.load_from_string(easy01);
    bool result = b.set(0, 0, 9);  // (0,0) is clue '5'
    assert(!result);                // must return false
    assert(b.get(0, 0) == 5);      // clue unchanged

    TEST("find_empty returns first empty cell");
    b.load_from_string(easy01);
    auto cell = b.find_empty();
    assert(cell.has_value());
    // Row 0 is "5..3..7.." — first empty is (0,1)
    assert(cell->first == 0 && cell->second == 1);

    TEST("find_empty returns nullopt when board is full");
    b.load_from_string(solution);   // reuse your known solution string
    cell = b.find_empty();
    assert(!cell.has_value());

    TEST("to_string round-trips load_from_string");
    b.load_from_string(easy01);
    std::string out = b.to_string();
    // to_string adds '\n' after each row — strip them and compare
    std::string stripped;
    for (char c : out) if (c != '\n') stripped += c;
    assert(stripped == easy01);

    TEST("set with validate=true rejects conflict");
    b.load_from_string(std::string(81, '.'));
    b.set(0, 0, 5);
    result = b.set(0, 1, 5, /*validate=*/true);  // 5 already in row 0
    assert(!result);
    assert(b.get(0, 1) == 0);  // unchanged



    std::cout << "All tests passed!" << std::endl;
    return 0;
}

