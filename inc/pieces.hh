#ifndef PIECES_H_INCLUDED
#define PIECES_H_INCLUDED

#include <iostream>
#include <vector>
#include "traceAndError.hh"
#include "bitboard.hh"

typedef std::vector< std::pair< uint8_t, uint8_t > > Moves;

enum class Opponent
{
  WHITE,
  BLACK
};

class Pieces
{
public:
  Pieces(uint8_t x_pos, uint8_t y_pos, uint8_t weight, Opponent opponent);
  ~Pieces() = default;

  void  move(uint8_t x, uint8_t y);

  void  destroy() const;

  std::vector< Moves > get_pseudo_legal_moves() const;
  Opponent             get_opponent_side()      const;


protected:
  // basic information about piece
  Opponent m_opponent;
  Bitboard m_all_moves[64];

  uint8_t m_weight;
  uint8_t m_x_pos;
  uint8_t m_y_pos;

  std::vector< Moves > m_pseudo_legal_moves;

  virtual void update_pseudo_legal_moves() = 0;
  virtual void __gen_piece_moves()         = 0;
};

class King : public Pieces
{
public:
  King(uint8_t x_pos, uint8_t y_pos, uint8_t weight, Opponent opponent) : 
        Pieces(x_pos, y_pos, weight, opponent)
  {
    __gen_piece_moves();
    update_pseudo_legal_moves();
    m_is_first_move = true;
  };

  ~King() = default;

private:
  bool m_is_first_move;

  virtual void update_pseudo_legal_moves();
  virtual void __gen_piece_moves();
};

class Queen : public Pieces
{
public:
  Queen(uint8_t x_pos, uint8_t y_pos, uint8_t weight, Opponent opponent) : 
         Pieces(x_pos, y_pos, weight, opponent)
  {
    __gen_piece_moves();
    update_pseudo_legal_moves();
  };

  ~Queen() = default;

private:
  virtual void update_pseudo_legal_moves();
  virtual void __gen_piece_moves();
};

class Rook : public Pieces
{
public:
  Rook(uint8_t x_pos, uint8_t y_pos, uint8_t weight, Opponent opponent) : 
        Pieces(x_pos, y_pos, weight, opponent)
  {
    __gen_piece_moves();
    update_pseudo_legal_moves();
    m_is_first_move = true;
  };

  ~Rook() = default;

private:
  bool m_is_first_move;

  virtual void update_pseudo_legal_moves();
  virtual void __gen_piece_moves();
};

class Bishop : public Pieces
{
public:
  Bishop(uint8_t x_pos, uint8_t y_pos, uint8_t weight, Opponent opponent) : 
          Pieces(x_pos, y_pos, weight, opponent)
  {
    __gen_piece_moves();
    update_pseudo_legal_moves();
  };

  ~Bishop() = default;

private:
  virtual void update_pseudo_legal_moves();
  virtual void __gen_piece_moves();
};

class Knight : public Pieces
{
public:
  Knight(uint8_t x_pos, uint8_t y_pos, uint8_t weight, Opponent opponent) : 
          Pieces(x_pos, y_pos, weight, opponent)
  {
    __gen_piece_moves();
    update_pseudo_legal_moves();
  };

  ~Knight() = default;

private:
  virtual void update_pseudo_legal_moves();
  virtual void __gen_piece_moves();
};

class Pawn : public Pieces
{
public:
  Pawn(uint8_t x_pos, uint8_t y_pos, uint8_t weight, Opponent opponent) : 
        Pieces(x_pos, y_pos, weight, opponent)
  {
    int8_t site_modifier = m_opponent == Opponent::BLACK ? 1 : -1;
    m_is_first_move = true;

    update_pseudo_legal_moves();

    m_pseudo_legal_moves.back().push_back(
      std::make_pair(m_x_pos, m_y_pos + 2 * site_modifier));
    __gen_piece_moves();
  };

  ~Pawn() = default;

private:
  bool m_is_first_move;

  virtual void update_pseudo_legal_moves();
  virtual void __gen_piece_moves();
};


#endif // PIECES_H_INCLUDED