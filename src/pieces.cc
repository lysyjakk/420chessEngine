#include "../inc/pieces.hh"
#include "../inc/traceAndError.hh"

Pieces::Pieces(uint8_t x_pos,
               uint8_t y_pos,
               uint8_t weight,
               Opponent opponent)
{
  m_x_pos    = x_pos;
  m_y_pos    = y_pos;
  m_weight   = weight;
  m_opponent = opponent;
}

void Pieces::move(uint8_t x, uint8_t y)
{
  m_x_pos = x;
  m_y_pos = y;

  return;
}

std::vector< Moves > Pieces::get_pseudo_legal_moves() const
{
  return m_pseudo_legal_moves;
}

void Pieces::destroy() const
{
  delete this;

  return;
}

Opponent Pieces::get_opponent_side() const
{
  return m_opponent;
}

void Pawn::update_pseudo_legal_moves()
{
  int8_t site_modifier = m_opponent == Opponent::BLACK ? 1 : -1;
  Moves temp_vector;

  m_pseudo_legal_moves.clear();

  temp_vector.push_back(std::make_pair(m_x_pos, m_y_pos + site_modifier));
  m_pseudo_legal_moves.push_back(temp_vector);

  return;
}

void Bishop::update_pseudo_legal_moves()
{
  int8_t x;
  int8_t y;
  Moves temp_vector;

  m_pseudo_legal_moves.clear();

  for (x = m_x_pos, y = m_y_pos; x < 8 && y < 8; ++x, ++y)
  {
    temp_vector.push_back(std::make_pair(x, y));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  for (x = m_x_pos, y = m_y_pos; x >= 0 && y >= 0; --x, --y)
  {
    temp_vector.push_back(std::make_pair(x, y));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  for (x = m_x_pos, y = m_y_pos; x < 8 && y >= 0; ++x, --y)
  {
    temp_vector.push_back(std::make_pair(x, y));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  for (x = m_x_pos, y = m_y_pos; x >= 0 && y < 8; --x, ++y)
  {
    temp_vector.push_back(std::make_pair(x, y));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  return;
}

void Knight::update_pseudo_legal_moves()
{
  m_pseudo_legal_moves.clear();
  Moves temp_vector;


  if (m_x_pos + 2 < 8 && m_y_pos + 1 < 8)
  {
    temp_vector.push_back(std::make_pair(m_x_pos + 2, m_y_pos + 1));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  if (m_x_pos + 2 < 8 && m_y_pos - 1 >= 0)
  {
    temp_vector.push_back(std::make_pair(m_x_pos + 2, m_y_pos - 1));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  if (m_x_pos - 2 >= 0 && m_y_pos + 1 < 8)
  {
    temp_vector.push_back(std::make_pair(m_x_pos - 2, m_y_pos + 1));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  if (m_x_pos - 2 >= 0 && m_y_pos - 1 >= 0)
  {
    temp_vector.push_back(std::make_pair(m_x_pos - 2, m_y_pos - 1));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  if (m_x_pos + 1 < 8 && m_y_pos + 2 < 8)
  {
    temp_vector.push_back(std::make_pair(m_x_pos + 1, m_y_pos + 2));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  if (m_x_pos - 1 >= 0 && m_y_pos + 2 < 8)
  {
    temp_vector.push_back(std::make_pair(m_x_pos - 1, m_y_pos + 2));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  if (m_x_pos + 1 < 8 && m_y_pos - 2 >= 0)
  {
    temp_vector.push_back(std::make_pair(m_x_pos + 1, m_y_pos - 2));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  if (m_x_pos - 1 >= 0 && m_y_pos -2 >= 0)
  {
    temp_vector.push_back(std::make_pair(m_x_pos - 1, m_y_pos - 2));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  return;
}

void Rook::update_pseudo_legal_moves()
{
  Moves temp_vector;


  m_pseudo_legal_moves.clear();

  for (int x = m_x_pos + 1; x < 8; ++x)
  {
    temp_vector.push_back(std::make_pair(x, m_y_pos));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  for (int x = m_x_pos - 1; x >= 0; --x)
  {
    temp_vector.push_back(std::make_pair(x, m_y_pos));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  for (int y = m_y_pos + 1; y < 8; ++y)
  {
    temp_vector.push_back(std::make_pair(m_x_pos, y));
  }
  m_pseudo_legal_moves.push_back(temp_vector);  
  temp_vector.clear();

  for (int y = m_y_pos -1 ; y >= 0; --y)
  {
    temp_vector.push_back(std::make_pair(m_x_pos, y));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  return;
}

void Queen::update_pseudo_legal_moves()
{
  int8_t x;
  int8_t y;
  Moves temp_vector;

  m_pseudo_legal_moves.clear();

  for (x = 0; x < 8; ++x)
  {
    temp_vector.push_back(std::make_pair(x, m_y_pos));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  for (y = 0; y < 8; ++y)
  {
    temp_vector.push_back(std::make_pair(m_x_pos, y));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  for (x = m_x_pos, y = m_y_pos; x < 8 && y < 8; ++x, ++y)
  {
    temp_vector.push_back(std::make_pair(x, y));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  for (x = m_x_pos, y = m_y_pos; x >= 0 && y >= 0; --x, --y)
  {
    temp_vector.push_back(std::make_pair(x, y));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  for (x = m_x_pos, y = m_y_pos; x < 8 && y >= 0; ++x, --y)
  {
    temp_vector.push_back(std::make_pair(x, y));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  for (x = m_x_pos, y = m_y_pos; x >= 0 && y < 8; --x, ++y)
  {
    temp_vector.push_back(std::make_pair(x, y));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  return;
}

void King::update_pseudo_legal_moves()
{
  Moves temp_vector;

  m_pseudo_legal_moves.clear();

  if (m_x_pos - 1 >= 0)
  {
    temp_vector.push_back(std::make_pair(m_x_pos - 1, m_y_pos));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  if (m_x_pos + 1 < 8)
  {
    temp_vector.push_back(std::make_pair(m_x_pos + 1, m_y_pos));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  if (m_y_pos - 1 >= 0)
  {
    temp_vector.push_back(std::make_pair(m_x_pos, m_y_pos - 1));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  if (m_y_pos + 1 < 8)
  {
    temp_vector.push_back(std::make_pair(m_x_pos, m_y_pos + 1));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  if (m_x_pos - 1 >= 0 && m_y_pos - 1 >= 0)
  {
    temp_vector.push_back(std::make_pair(m_x_pos - 1, m_y_pos - 1));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  if (m_x_pos + 1 < 8 && m_y_pos + 1 < 8)
  {
    temp_vector.push_back(std::make_pair(m_x_pos + 1, m_y_pos + 1));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  if (m_x_pos + 1 < 8 && m_y_pos - 1 >= 0)
  {
    temp_vector.push_back(std::make_pair(m_x_pos + 1, m_y_pos - 1));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  if (m_x_pos - 1 >= 0 && m_y_pos + 1 < 8)
  {
    temp_vector.push_back(std::make_pair(m_x_pos - 1, m_y_pos + 1));
  }
  m_pseudo_legal_moves.push_back(temp_vector);
  temp_vector.clear();

  return;
}