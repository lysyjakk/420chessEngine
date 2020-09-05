#ifndef MOVE_LOOKUP_H_INCLUDED
#define MOVE_LOOKUP_H_INCLUDED

#include "pieces.hh"
#include "gameManager.hh"
#include "traceAndError.hh"

typedef struct
{
  King   *king;
  Rook   *rook;
  Knight *knight;
  Bishop *bishop;
  Pawn   *b_pawn;
  Pawn   *w_pawn;
} Moves;


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
} ChessBoard;


class MoveLookup
{
public:
  MoveLookup()  = default;
  ~MoveLookup() = default;

  void init(BitBoardToGUI board);

  bool is_move_valid() const;
};


#endif // MOVE_LOOKUP_H_INCLUDED