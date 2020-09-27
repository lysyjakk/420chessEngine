#ifndef PIECES_H_INCLUDED
#define PIECES_H_INCLUDED

/* > Defines ******************************************************************/

#define MAX_BOARD_SQ 64
#define MAX_DOUBLE_PAWN_MV 8

/* > Includes *****************************************************************/

#include <iostream>
#include <vector>
#include "traceAndError.hh"
#include "bitboard.hh"

/* > Structs definition *******************************************************/

enum class Opponent
{
  WHITE,
  BLACK,
  BOTH
};

/* > Classes definition *******************************************************/

class Pieces
{
public:
  Pieces(Opponent opponent);
  ~Pieces() = default;

  Opponent get_opponent_side() const;

  virtual Bitboard get_moves(std::size_t sq, Bitboard occ = NULL) const = 0;

protected:
  // basic information about piece
  Opponent m_opponent;
  Bitboard m_pseudo_mask[MAX_BOARD_SQ];

  virtual void __gen_pseudo_mask() = 0;
};

/* > --------------------------------------------------         >>        KING*/

class King : public Pieces
{
public:
  King(Opponent opponent) : 
        Pieces(opponent)
  {
    __gen_pseudo_mask();
  };

  ~King() = default;

  virtual Bitboard get_moves(std::size_t sq, Bitboard occ = NULL) const;

private:
  virtual void __gen_pseudo_mask();
};

/* > --------------------------------------------------         >>        ROOK*/

class Rook : public Pieces
{
public:
  Rook(Opponent opponent) : 
        Pieces(opponent)
  {
    __gen_pseudo_mask();
    __gen_blocker_mask();
    __gen_attacks_mask();
  };

  ~Rook() = default;
  uint64_t find_magic_number(int square, int relevant_bits);
  virtual Bitboard get_moves(std::size_t sq, Bitboard occ = NULL) const;

private:
  Bitboard m_blocker_mask[MAX_BOARD_SQ];
  Bitboard m_attack_mask[MAX_BOARD_SQ][4096];

          void __gen_blocker_mask();
          void __gen_attacks_mask();
  virtual void __gen_pseudo_mask();

  Bitboard __moves_with_occ_at(uint8_t sq, Bitboard occ) const;
};

/* > --------------------------------------------------         >>      BISHOP*/
class Bishop : public Pieces
{
public:
  Bishop(Opponent opponent) : 
          Pieces(opponent)
  {
    __gen_pseudo_mask();
    __gen_blocker_mask();
    __gen_attacks_mask();
  };

  ~Bishop() = default;

  virtual Bitboard get_moves(std::size_t sq, Bitboard occ = NULL) const;

private:
  Bitboard m_blocker_mask[MAX_BOARD_SQ];
  Bitboard m_attack_mask[MAX_BOARD_SQ][512];

          void __gen_blocker_mask();
          void __gen_attacks_mask();
  virtual void __gen_pseudo_mask();

  Bitboard __moves_with_occ_at(uint8_t sq, Bitboard occ) const;
};

/* > --------------------------------------------------         >>      KNIGHT*/

class Knight : public Pieces
{
public:
  Knight(Opponent opponent) : 
          Pieces(opponent)
  {
    __gen_pseudo_mask();
  };

  ~Knight() = default;

  virtual Bitboard get_moves(std::size_t sq, Bitboard occ = NULL) const;

private:
  virtual void __gen_pseudo_mask();
};

/* > --------------------------------------------------         >>        PAWN*/

class Pawn : public Pieces
{
public:
  Pawn(Opponent opponent) : 
        Pieces(opponent)
  {
    __gen_pseudo_mask();
    __gen_capture_mask();
    __gen_double_mv_mask();
  };

  ~Pawn() = default;

          Bitboard get_pawn_capture(std::size_t sq) const;
          Bitboard get_pawn_double(std::size_t sq) const;
  virtual Bitboard get_moves(std::size_t sq, Bitboard occ = NULL) const;

private:
  Bitboard m_capture_mask[MAX_BOARD_SQ];
  Bitboard m_double_moves[MAX_DOUBLE_PAWN_MV];

          void __gen_capture_mask();
          void __gen_double_mv_mask();
  virtual void __gen_pseudo_mask();
};

#endif // PIECES_H_INCLUDED