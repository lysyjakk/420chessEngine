#include "../inc/pieces.hh"

Pieces::Pieces(uint32_t x_pos,
               uint32_t y_pos,
               uint8_t weight,
               Opponent opponent)
{
  this -> m_x_pos    = x_pos;
  this -> m_y_pos    = y_pos;
  this -> m_weight   = weight;
  this -> m_opponent = opponent;
}