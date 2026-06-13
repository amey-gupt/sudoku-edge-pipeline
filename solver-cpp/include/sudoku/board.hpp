#pragma once

#include <array>
#include <string>
#include <optional>
#include <fstream>
#include <iostream>

namespace sudoku {

    class Board {
        public:
            //Note: const enforces read-only immutability at runtime or compile-time while constexpr enforces it at compile-time.
            static constexpr int SIZE = 9;
            static constexpr int BOX = 3;

            Board();

            explicit Board(const std::string& flat);
            // "explicit" prevents accidental Board b = "53..7...."; // implicit conversion from string to Board

            std::optional<std::string> load_from_string(const std::string& text);

            std::optional<std::string> load_from_file(const std::string& path);

            int get(int row, int col) const;
            void set(int row, int col, int digit);

            bool is_valid_placement(int row, int col, int digit) const;

            bool is_complete() const;

            bool is_solved() const;

            std::string to_string() const;

            const std::array<std::array<int, SIZE>, SIZE>& cells() const { 
                return grid_; 
            }

        private:
            std::array<std::array<int, SIZE>, SIZE> grid_ {};
            
            static int box_index(int row, int col);

            bool row_contains(int row, int digit, int skip_col) const;
            bool col_contains(int col, int digit, int skip_row) const;
            bool box_contains(int row, int col, int digit) const;

           
    };
}