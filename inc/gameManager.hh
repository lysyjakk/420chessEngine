#ifndef GAMEMANAGER_H_INCLUDED
#define GAMEMANAGER_H_INCLUDED

#include <array>
#include <algorithm>
#include "traceAndError.hh"
#include "pieces.hh"
#include "bitboard.hh"

#define MAX_BOARD_COLUMNS 8
#define MAX_BOARD_ROWS    8

enum TypesOfPieces
{
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
  BLACK_KING,

  NONE
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
  void       move_piece(uint8_t x_src,
                        uint8_t y_src,
                        uint8_t x_dest,
                        uint8_t y_dest);
  ChessBoard get_chess_board() const;

private:
  ChessBoard m_chess_board;

  bool check_valid_of_move(Pieces *piece, uint8_t x_dest, uint8_t y_dest) const;
  Moves get_all_valid_moves(Pieces *piece) const;
};

#endif // GAMEMANAGER_H_INCLUDED