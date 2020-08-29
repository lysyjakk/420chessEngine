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

void Pawn::__gen_piece_moves()
{
  Bitboard (Bitboard::*func)(void);
  func = m_opponent == Opponent::BLACK ?
          &Bitboard::move_south : &Bitboard::move_north;

  for (int sq = 0; sq < 64; ++sq)
  {
    Bitboard b = Bitboard().board.set(sq);
    m_all_moves[sq] |= (b.*func)();
  }

  return;
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

void Bishop::__gen_piece_moves()
{
  for (int sq = 0; sq < 64; ++sq)
  {
    Bitboard moves;

    Bitboard b = Bitboard().board.set(sq);
    Bitboard ne = b.move_no_ea(), nw = b.move_no_we(),
             se = b.move_so_ea(), sw = b.move_so_we();

    while(ne != 0x0)
    {
      moves |= ne;
      ne = ne.move_no_ea();
    }

    while(nw != 0x0)
    {
      moves |= nw;
      nw = nw.move_no_we();
    }

    while(se != 0x0)
    {
      moves |= se;
      se = se.move_so_ea();
    }

    while(sw != 0x0)
    {
      moves |= sw;
      sw = sw.move_so_we();
    }

    m_all_moves[sq] |= moves;
  }

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

void Knight::__gen_piece_moves()
{
  for (int sq = 0; sq < 64; ++sq)
  {
    Bitboard b = Bitboard().board.set(sq);

    Bitboard e = b.move_east() , n = b.move_north(),
             s = b.move_south(), w = b.move_west();

    Bitboard moves;

    moves = n.move_no_ea() | n.move_no_we() | s.move_so_ea() | s.move_so_we() |
             e.move_no_ea() | e.move_so_ea() | w.move_no_we() | w.move_so_we();

    m_all_moves[sq] |= moves;
  }

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

void Rook::__gen_piece_moves()
{
    for (int sq = 0; sq < 64; ++sq)
  {
    Bitboard moves;

    Bitboard b = Bitboard().board.set(sq);
    Bitboard  n = b.move_north(),  w = b.move_west(),
              s = b.move_south(),  e = b.move_east(),
             ne = b.move_no_ea(), nw = b.move_no_we(),
             se = b.move_so_ea(), sw = b.move_so_we();

    while(n != 0x0)
    {
      moves |= n;
      n = n.move_north();
    }

    while(w != 0x0)
    {
      moves |= w;
      w = w.move_west();
    }

    while(s != 0x0)
    {
      moves |= s;
      s = s.move_south();
    }

    while(e != 0x0)
    {
      moves |= e;
      e = e.move_east();
    }

    while(ne != 0x0)
    {
      moves |= ne;
      ne = ne.move_no_ea();
    }

    while(nw != 0x0)
    {
      moves |= nw;
      nw = nw.move_no_we();
    }

    while(se != 0x0)
    {
      moves |= se;
      se = se.move_so_ea();
    }

    while(sw != 0x0)
    {
      moves |= sw;
      sw = sw.move_so_we();
    }

    m_all_moves[sq] |= moves;
    m_all_moves[sq].print();
  }

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

void Queen::__gen_piece_moves()
{
  for (int sq = 0; sq < 64; ++sq)
  {
    Bitboard moves;

    Bitboard b = Bitboard().board.set(sq);
    Bitboard n = b.move_north(), w = b.move_west(),
             s = b.move_south(), e = b.move_east();

    while(n != 0x0)
    {
      moves |= n;
      n = n.move_north();
    }

    while(w != 0x0)
    {
      moves |= w;
      w = w.move_west();
    }

    while(s != 0x0)
    {
      moves |= s;
      s = s.move_south();
    }

    while(e != 0x0)
    {
      moves |= e;
      e = e.move_east();
    }

    m_all_moves[sq] |= moves;
  }

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

void King::__gen_piece_moves()
{
  for (int sq = 0; sq < 64; ++sq)
  {
    Bitboard b = Bitboard().board.set(sq);
    b = b.move_east() | b.move_north() | b.move_south() | b.move_west() |
          b.move_no_ea() | b.move_no_we() | b.move_so_ea() | b.move_so_we();

    m_all_moves[sq] |= b;
  }

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