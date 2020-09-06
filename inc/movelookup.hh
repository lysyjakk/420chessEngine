#ifndef MOVE_LOOKUP_H_INCLUDED
#define MOVE_LOOKUP_H_INCLUDED

#include <vector>
#include "pieces.hh"
#include "traceAndError.hh"
#include "errorCodes.hh"

enum class SiteMove
{
  WHITE_MOVE,
  BLACK_MOVE
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

typedef struct
{
  Bitboard pawns;
  Bitboard rooks;
  Bitboard knights;
  Bitboard bishops;
  Bitboard queens;
  Bitboard king;
} ChessBoard;

static struct Moves
{
  King   *king;
  Rook   *rook;
  Knight *knight;
  Bishop *bishop;
  Pawn   *b_pawn;
  Pawn   *w_pawn;
} m_pseudolegal_mv;


class MoveLookup
{
public:
  MoveLookup()  = default;
  ~MoveLookup() = default;

  void init();

  bool is_move_valid(uint8_t x_desc,
                     uint8_t y_desc,
                     TypesOfPieces piece_type,
                     SiteMove      site_move);

private:
  ChessBoard m_white_pieces;
  ChessBoard m_black_pieces;
  Bitboard   m_white_occ;
  Bitboard   m_black_occ;

  //Bitboard __gen_occupancy_board(ChessBoard board);
  Bitboard __get_white_occupancies();
  Bitboard __get_black_occupancies();
  Bitboard __get_both_occupancies();

  bool __is_sq_attacked(std::size_t sq, Opponent opp);
  bool __is_move_correct(uint8_t       x_desc,
                         uint8_t       y_desc,
                         TypesOfPieces piece_type,
                         SiteMove      site_move);
};


#endif // MOVE_LOOKUP_H_INCLUDED