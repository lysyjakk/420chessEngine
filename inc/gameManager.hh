#ifndef GAMEMANAGER_H_INCLUDED
#define GAMEMANAGER_H_INCLUDED

#include <array>
#include <algorithm>
#include <vector>
#include "traceAndError.hh"
#include "errorCodes.hh"
#include "pieces.hh"
#include "bitboard.hh"
#include "movelookup.hh"

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

typedef std::array< TypesOfPieces, 64 > BitBoardToGUI;

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
  BitBoardToGUI  m_pieces_pos;
  MoveLookup    *m_move_checker;
};

#endif // GAMEMANAGER_H_INCLUDED