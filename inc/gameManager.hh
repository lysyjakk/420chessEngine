#ifndef GAMEMANAGER_H_INCLUDED
#define GAMEMANAGER_H_INCLUDED

#include <array>
#include <algorithm>
#include <vector>
#include "traceAndError.hh"
#include "errorCodes.hh"
#include "pieces.hh"
#include "bitboard.hh"

#define MAX_BOARD_COLUMNS 8
#define MAX_BOARD_ROWS    8

enum CoordDecoder
{
  A1, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8
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

typedef std::array< TypesOfPieces, 64 > BitBoardToGUI;

typedef struct
{
  King   *king;
  Queen  *queen;
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

template <typename S> Bitboard* get_begin(S *s)
{
    return (Bitboard*)s;
}

template <typename S> Bitboard* get_end(S *s)
{
    return (Bitboard*)((uint8_t*)s+sizeof(*s));
}

class GameManager
{
public:
  GameManager()  = default;
  ~GameManager() = default;

  void start_new_game();
  void move_piece(uint8_t x_src,
                  uint8_t y_src,
                  uint8_t x_dest,
                  uint8_t y_dest);

  BitBoardToGUI get_board() const;

private:
  BitBoardToGUI m_pieces_pos;
  ChessBoard    m_board;
  Moves         m_pseudolegal_mv;

  bool        __check_valid_of_move(ChessBoard    target_board,
                                    TypesOfPieces piece)        const;
  bool        __check_piece_move   (Bitboard      target_piece) const;


  ChessBoard* __gen_target_board   (TypesOfPieces piece,
                                    uint8_t       x_src,
                                    uint8_t       y_src,
                                    uint8_t       x_dest,
                                    uint8_t       y_dest) const;

};

#endif // GAMEMANAGER_H_INCLUDED