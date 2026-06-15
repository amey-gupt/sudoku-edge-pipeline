#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "sudoku/backtracking_solver.hpp"
#include "sudoku/board.hpp"


namespace fs = std::filesystem;

static std::vector<std::string> load_puzzle_strings(const fs::path& dir) {
    std::vector<std::string> puzzles;
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.path().extension() != ".txt") continue;

        std::ifstream file(entry.path());
        std::string content {
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()
        };
        puzzles.push_back(content);
    }
    return puzzles;
}

int main() {
    const fs::path data_dir = PROJECT_ROOT "/data/puzzles";
    auto puzzles = load_puzzle_strings(data_dir);

    if (puzzles.empty()) {
        std::cerr << "No puzzles found in " << data_dir << std::endl;
        return 1;
    }

    sudoku::BacktrackingSolver solver;

    constexpr int kRuns = 10000;

    auto start = std::chrono::steady_clock::now();

    int solved_count = 0;
    for (int i = 0; i < kRuns; ++i) {
        const std::string& puzzle = puzzles[i % puzzles.size()];

        sudoku::Board board;
        auto err = board.load_from_string(puzzle);
        if (err) continue;

        if (solver.solve(board) && board.is_solved()) {
            ++solved_count;
        }
    }

    auto end = std::chrono::steady_clock::now();
    auto total_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double avg_us = static_cast<double>(total_us) / kRuns;

    std::cout << "solver: " << solver.name() << "\n";
    std::cout << "puzzles loaded: " << puzzles.size() << "\n";
    std::cout << "runs: " << kRuns << "\n";
    std::cout << "solved: " << solved_count << "\n";
    std::cout << "total_ms: " << (total_us / 1000.0) << "\n";
    std::cout << "avg_us_per_solve: " << avg_us << "\n";
    std::cout << "avg_ms_per_solve: " << (avg_us / 1000.0) << "\n";
    return 0;
}

