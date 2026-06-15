# Benchmarks

## Environment
- Date: 2026-06-14
- CPU: 12th Gen Intel(R) Core(TM) i7-1255U
- Compiler: 13.3.0

## backtracking_naive

| Metric | Value |
|--------|-------|
| Puzzle set | 20 files in `data/puzzles/` |
| Runs | 10000 |
| Solved | 10000 |
| Total time | 684.553 ms |
| Avg per solve | 68.4553 µs |

### Notes
- Timing includes board load + solve (not solve-only yet).
- Baseline for comparison with bitmask (Step 1.3) and DLX (Step 1.4).