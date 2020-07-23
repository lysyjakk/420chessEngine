#ifndef PIECES_H_INCLUDED
#define PIECES_H_INCLUDED

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glu.h>
#include <iostream>

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

  void move(uint8_t x, uint8_t y);

protected:
  // basic information about piece
  Opponent m_opponent;

  uint8_t m_weight;

  uint8_t m_x_pos;
  uint8_t m_y_pos;
};

class King : public Pieces
{
public:
  King(uint8_t x_pos, uint8_t y_pos, uint8_t weight, Opponent opponent) : 
        Pieces(x_pos, y_pos, weight, opponent) {};
  ~King() = default;
};

class Queen : public Pieces
{
public:
  Queen(uint8_t x_pos, uint8_t y_pos, uint8_t weight, Opponent opponent) : 
         Pieces(x_pos, y_pos, weight, opponent) {};
  ~Queen() = default;
};

class Rook : public Pieces
{
public:
  Rook(uint8_t x_pos, uint8_t y_pos, uint8_t weight, Opponent opponent) : 
        Pieces(x_pos, y_pos, weight, opponent) {};
  ~Rook() = default;
};

class Bishop : public Pieces
{
public:
  Bishop(uint8_t x_pos, uint8_t y_pos, uint8_t weight, Opponent opponent) : 
          Pieces(x_pos, y_pos, weight, opponent) {};
  ~Bishop() = default;
};

class Knight : public Pieces
{
public:
  Knight(uint8_t x_pos, uint8_t y_pos, uint8_t weight, Opponent opponent) : 
          Pieces(x_pos, y_pos, weight, opponent) {};
  ~Knight() = default;
};

class Pawn : public Pieces
{
public:
  Pawn(uint8_t x_pos, uint8_t y_pos, uint8_t weight, Opponent opponent) : 
        Pieces(x_pos, y_pos, weight, opponent) {};
  ~Pawn() = default;
};


#endif // PIECES_H_INCLUDED