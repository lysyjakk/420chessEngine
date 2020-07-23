#ifndef GAMEMANAGER_H_INCLUDED
#define GAMEMANAGER_H_INCLUDED

#include <array>
#include "../inc/traceAndError.hh"
#include "../inc/pieces.hh"

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

  bool check_valid_of_move(Pieces *piece) const;
};

#endif // GAMEMANAGER_H_INCLUDED