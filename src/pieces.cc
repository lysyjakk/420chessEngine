#include "../inc/pieces.hh"

Pieces::Pieces(uint8_t x_pos,
               uint8_t y_pos,
               uint8_t weight,
               Opponent opponent)
{
  this -> m_x_pos    = x_pos;
  this -> m_y_pos    = y_pos;
  this -> m_weight   = weight;
  this -> m_opponent = opponent;
}

void Pieces::move(uint8_t x, uint8_t y)
{
  this -> m_x_pos = x;
  this -> m_y_pos = y;

  return;
}