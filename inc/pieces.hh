#ifndef PIECES_H_INCLUDED
#define PIECES_H_INCLUDED

/* > Defines ******************************************************************/

#define MAX_BOARD_SQ 64

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
  Pieces(uint8_t weight, Opponent opponent);
  ~Pieces() = default;

  Opponent get_opponent_side() const;

  virtual Bitboard get_moves(std::size_t sq, Bitboard occ = NULL) const = 0;

protected:
  // basic information about piece
  Opponent m_opponent;
  Bitboard m_pseudo_mask[MAX_BOARD_SQ];

  uint8_t m_weight;

  virtual void __gen_pseudo_mask() = 0;
};

/* > --------------------------------------------------         >>        KING*/

class King : public Pieces
{
public:
  King(uint8_t weight, Opponent opponent) : 
        Pieces(weight, opponent)
  {
    __gen_pseudo_mask();
  };

  ~King() = default;

  virtual Bitboard get_moves(std::size_t sq, Bitboard occ = NULL) const;

private:
  virtual void __gen_pseudo_mask();
};

/* > --------------------------------------------------         >>       QUEEN*/

class Queen : public Pieces
{
public:
  Queen(uint8_t weight, Opponent opponent) : 
         Pieces(weight, opponent)
  {
    __gen_pseudo_mask();
    __gen_blocker_mask();
  };

  ~Queen() = default;

  virtual Bitboard get_moves(std::size_t sq, Bitboard occ = NULL) const;

private:
  Bitboard m_blocker_mask[MAX_BOARD_SQ];

          void __gen_blocker_mask();
  virtual void __gen_pseudo_mask();
};

/* > --------------------------------------------------         >>        ROOK*/

class Rook : public Pieces
{
public:
  Rook(uint8_t weight, Opponent opponent) : 
        Pieces(weight, opponent)
  {
    __gen_pseudo_mask();
    __gen_blocker_mask();
  };

  ~Rook() = default;

  virtual Bitboard get_moves(std::size_t sq, Bitboard occ = NULL) const;

private:
  Bitboard m_blocker_mask[MAX_BOARD_SQ];
  Bitboard m_attack_mask[MAX_BOARD_SQ][4096];

          void __gen_blocker_mask ();
          void __init_attacks_mask();
  virtual void __gen_pseudo_mask  ();
};

/* > --------------------------------------------------         >>      BISHOP*/
class Bishop : public Pieces
{
public:
  Bishop(uint8_t weight, Opponent opponent) : 
          Pieces(weight, opponent)
  {
    __gen_pseudo_mask();
    __gen_blocker_mask();
    __gen_attacks_mask();
  };

  ~Bishop() = default;
  uint64_t find_magic_number(int square, int relevant_bits);
  virtual Bitboard get_moves(std::size_t sq, Bitboard occ = NULL) const;

private:
  Bitboard m_blocker_mask[MAX_BOARD_SQ];
  Bitboard m_attack_mask[MAX_BOARD_SQ][512];

          void __gen_blocker_mask();
          void __gen_attacks_mask();
  virtual void __gen_pseudo_mask();

  Bitboard __moves_with_occ_at(uint8_t sq, Bitboard occ);
};

/* > --------------------------------------------------         >>      KNIGHT*/

class Knight : public Pieces
{
public:
  Knight(uint8_t weight, Opponent opponent) : 
          Pieces(weight, opponent)
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
  Pawn(uint8_t weight, Opponent opponent) : 
        Pieces(weight, opponent)
  {
    __gen_pseudo_mask();
  };

  ~Pawn() = default;

  virtual Bitboard get_moves(std::size_t sq, Bitboard occ = NULL) const;

private:
  virtual void __gen_pseudo_mask();
};


#endif // PIECES_H_INCLUDED