# cheess_sfml

A simple chess engine project with an even simpler bot.

This project was built as a personal challenge to design a chess engine from scratch without referring to external resources (e.g., chessprogramming wiki). It also serves as a learning tool for using the SFML graphics library.

## Board Representation

The game state is stored in a simple character array. Each piece is represented by a character:
- Uppercase for white pieces (e.g., 'K', 'Q', 'P')
- Lowercase for black pieces (e.g., 'k', 'q', 'p')
- Empty squares are represented by '-'

## Bot Implementation

The bot uses a basic minimax algorithm:
- No alpha-beta pruning
- No heuristic optimizations
- Evaluates positions using simple piece-square value tables (predefined weight maps for each piece type depending on its position)

Because of the above limitations, even shallow search depths result in very long computation times. This is mostly due to:
- Lack of algorithm optimizations (no pruning or move ordering)
- Probably inefficient move generation and legality checking (e.g., verifying that a move doesn't expose the king)
- Inefficient data structures for storing the board state

The current version of the code is quite messy and definitely needs refactoring. There’s a lot of room for improvements in structure, efficiency, and readability.

## Bitboards - Concept for a Future Project

After submitting the initial version of the project, I explored chess engine internals and discovered bitboards—a much more efficient way to represent the game state.

Bitboards use 64-bit integers where each bit represents a square on the board.

### 8-Bitboard Representation:
- 2 boards for color
- 6 boards for piece types

```
int64_t white, black;
int64_t king, rook, bishop, knight, queen, pawn;
```

### 12-Bitboard Representation:
One board for each piece type and color combination

```
int64_t w_king, w_rook, w_bishop, w_knight, w_queen, w_pawn;
int64_t b_king, b_rook, b_bishop, b_knight, b_queen, b_pawn;
```
This representation allows very fast computations using bitwise operations. For example, getting all squares attacked by pawns can be done with a bit shift and masking out edge columns.

The simplicity of this technique made me decide to write a bitboard-based engine in the future.
