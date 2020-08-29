#ifndef GAMEMANAGER_H_INCLUDED
#define GAMEMANAGER_H_INCLUDED

#include <array>
#include <algorithm>
#include "traceAndError.hh"
#include "pieces.hh"
#include "bitboard.hh"

#define MAX_BOARD_COLUMNS 8
#define MAX_BOARD_ROWS    8

enum
{
  H1, G1, F1, E1, D1, C1, B1, A1,
  H2, G2, F2, E2, D2, C2, B2, A2,
  H3, G3, F3, E3, D3, C3, B3, A3,
  H4, G4, F4, E4, D4, C4, B4, A4,
  H5, G5, F5, E5, D5, C5, B5, A5,
  H6, G6, F6, E6, D6, C6, B6, A6,
  H7, G7, F7, E7, D7, C7, B7, A7,
  H8, G8, F8, E8, D8, C8, B8, A8
};

enum TypesOfPieces
{
  NONE,

// White pieces
  WHITE_PAWN,
  WHITE_ROOK,
  WHITE_KNIGHT,
  WHITE_BISHOP,
  WHITE_QUEEN,
  WHITE_KING,

// Black pieces
  BLACK_PAWN,
  BLACK_ROOK,
  BLACK_KNIGHT,
  BLACK_BISHOP,
  BLACK_QUEEN,
  BLACK_KING
};

typedef std::array< std::array< std::pair< Pieces*, TypesOfPieces >,
          MAX_BOARD_COLUMNS >, MAX_BOARD_ROWS > ChessBoard;

typedef struct
{
  /* The white piece positions */
  Bitboard white_pawns;
  Bitboard white_rooks;
  Bitboard white_knights;
  Bitboard white_bishops;
  Bitboard white_queens;
  Bitboard white_king;

  /* The black piece positions */
  Bitboard black_pawns;
  Bitboard black_rooks;
  Bitboard black_knights;
  Bitboard black_bishops;
  Bitboard black_queens;
  Bitboard black_king;
} chessBoard;



class GameManager
{
public:
  GameManager() = default;
  ~GameManager() = default;

  void       start_new_game();
  void       start_new_game2();
  void       move_piece(uint8_t x_src,
                        uint8_t y_src,
                        uint8_t x_dest,
                        uint8_t y_dest);
  ChessBoard get_chess_board() const;

private:
  ChessBoard m_chess_board;
  chessBoard m_board;

  bool check_valid_of_move(Pieces *piece, uint8_t x_dest, uint8_t y_dest) const;
  Moves get_all_valid_moves(Pieces *piece) const;
};

#endif // GAMEMANAGER_H_INCLUDED