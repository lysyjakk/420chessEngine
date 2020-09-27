/* > Includes definition ******************************************************/

#include "../inc/pieces.hh"
#include "../inc/traceAndError.hh"

// bishop relevant occupancy bit count for every square on board
const uint8_t bishop_relevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    6, 5, 5, 5, 5, 5, 5, 6
};

// rook relevant occupancy bit count for every square on board
const uint8_t rook_relevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    12, 11, 11, 11, 11, 11, 11, 12
};

uint64_t rook_magic_numbers[64] = {
  0x8a80104000800020ULL,
  0x140002000100040ULL,
  0x2801880a0017001ULL,
  0x100081001000420ULL,
  0x200020010080420ULL,
  0x3001c0002010008ULL,
  0x8480008002000100ULL,
  0x2080088004402900ULL,
  0x800098204000ULL,
  0x2024401000200040ULL,
  0x100802000801000ULL,
  0x120800800801000ULL,
  0x208808088000400ULL,
  0x2802200800400ULL,
  0x2200800100020080ULL,
  0x801000060821100ULL,
  0x80044006422000ULL,
  0x100808020004000ULL,
  0x12108a0010204200ULL,
  0x140848010000802ULL,
  0x481828014002800ULL,
  0x8094004002004100ULL,
  0x4010040010010802ULL,
  0x20008806104ULL,
  0x100400080208000ULL,
  0x2040002120081000ULL,
  0x21200680100081ULL,
  0x20100080080080ULL,
  0x2000a00200410ULL,
  0x20080800400ULL,
  0x80088400100102ULL,
  0x80004600042881ULL,
  0x4040008040800020ULL,
  0x440003000200801ULL,
  0x4200011004500ULL,
  0x188020010100100ULL,
  0x14800401802800ULL,
  0x2080040080800200ULL,
  0x124080204001001ULL,
  0x200046502000484ULL,
  0x480400080088020ULL,
  0x1000422010034000ULL,
  0x30200100110040ULL,
  0x100021010009ULL,
  0x2002080100110004ULL,
  0x202008004008002ULL,
  0x20020004010100ULL,
  0x2048440040820001ULL,
  0x101002200408200ULL,
  0x40802000401080ULL,
  0x4008142004410100ULL,
  0x2060820c0120200ULL,
  0x1001004080100ULL,
  0x20c020080040080ULL,
  0x2935610830022400ULL,
  0x44440041009200ULL,
  0x280001040802101ULL,
  0x2100190040002085ULL,
  0x80c0084100102001ULL,
  0x4024081001000421ULL,
  0x20030a0244872ULL,
  0x12001008414402ULL,
  0x2006104900a0804ULL,
  0x1004081002402ULL
};

uint64_t bishop_magic_numbers[64] = {
  0x40040822862081ULL,
  0x40810a4108000ULL,
  0x2008008400920040ULL,
  0x61050104000008ULL,
  0x8282021010016100ULL,
  0x41008210400a0001ULL,
  0x3004202104050c0ULL,
  0x22010108410402ULL,
  0x60400862888605ULL,
  0x6311401040228ULL,
  0x80801082000ULL,
  0x802a082080240100ULL,
  0x1860061210016800ULL,
  0x401016010a810ULL,
  0x1000060545201005ULL,
  0x21000c2098280819ULL,
  0x2020004242020200ULL,
  0x4102100490040101ULL,
  0x114012208001500ULL,
  0x108000682004460ULL,
  0x7809000490401000ULL,
  0x420b001601052912ULL,
  0x408c8206100300ULL,
  0x2231001041180110ULL,
  0x8010102008a02100ULL,
  0x204201004080084ULL,
  0x410500058008811ULL,
  0x480a040008010820ULL,
  0x2194082044002002ULL,
  0x2008a20001004200ULL,
  0x40908041041004ULL,
  0x881002200540404ULL,
  0x4001082002082101ULL,
  0x8110408880880ULL,
  0x8000404040080200ULL,
  0x200020082180080ULL,
  0x1184440400114100ULL,
  0xc220008020110412ULL,
  0x4088084040090100ULL,
  0x8822104100121080ULL,
  0x100111884008200aULL,
  0x2844040288820200ULL,
  0x90901088003010ULL,
  0x1000a218000400ULL,
  0x1102010420204ULL,
  0x8414a3483000200ULL,
  0x6410849901420400ULL,
  0x201080200901040ULL,
  0x204880808050002ULL,
  0x1001008201210000ULL,
  0x16a6300a890040aULL,
  0x8049000441108600ULL,
  0x2212002060410044ULL,
  0x100086308020020ULL,
  0x484241408020421ULL,
  0x105084028429c085ULL,
  0x4282480801080cULL,
  0x81c098488088240ULL,
  0x1400000090480820ULL,
  0x4444000030208810ULL,
  0x1020142010820200ULL,
  0x2234802004018200ULL,
  0xc2040450820a00ULL,
  0x2101021090020ULL
};

/* > Local functions **********************************************************/ 

static Bitboard set_occupancy(int index, int bits_in_mask, Bitboard attack_mask)
{
  Bitboard occ = Bitboard(0x0);

  for (int itr = 0; itr < bits_in_mask; ++itr)
  {
    int sq = get_ls1b_index(attack_mask);

    attack_mask = attack_mask.pop_bit_at(sq);

    if (index & (1 << itr))
    {
      occ |= (1ULL << sq);
    }
  }

  return occ;
}

/* > Methods definitions ******************************************************/

Pieces::Pieces(Opponent opponent)
{
  m_opponent = opponent;
}

Opponent Pieces::get_opponent_side() const
{
  return m_opponent;
}

/* > --------------------------------------------------         >>        KING*/

Bitboard King::get_moves(std::size_t sq, Bitboard occ) const
{
  return m_pseudo_mask[sq];
}

void King::__gen_pseudo_mask()
{
  for (int sq = 0; sq < MAX_BOARD_SQ; ++sq)
  {
    Bitboard b = Bitboard().board.set(sq);
    b = b.move_east() | b.move_north() | b.move_south() | b.move_west() |
          b.move_no_ea() | b.move_no_we() | b.move_so_ea() | b.move_so_we();

    m_pseudo_mask[sq] |= b;
  }

  return;
}

/* > --------------------------------------------------         >>        ROOK*/

Bitboard Rook::get_moves(std::size_t sq, Bitboard occ) const
{
  occ &= m_blocker_mask[sq];
  occ *= rook_magic_numbers[sq];
  occ >>= 64 - rook_relevant_bits[sq];

  return m_attack_mask[sq][occ.board.to_ullong()];
}

void Rook::__gen_attacks_mask()
{
  for (int sq = 0; sq < MAX_BOARD_SQ; ++sq)
  {
    uint8_t  relevant_bits_count = m_blocker_mask[sq].board.count();
    uint32_t occupancy_indicies  = (1 << relevant_bits_count);

    for (int index = 0; index < occupancy_indicies; ++index)
    {
      Bitboard occ = set_occupancy(index, relevant_bits_count, m_blocker_mask[sq]);

      int magic_index = ((occ * rook_magic_numbers[sq])
                          >> (64 - rook_relevant_bits[sq])).board.to_ulong();
      m_attack_mask[sq][magic_index] = __moves_with_occ_at(sq, occ);
    }
  }
  return;
}

void Rook::__gen_blocker_mask()
{
  for (int sq = 0; sq < MAX_BOARD_SQ; ++sq)
  {
    Bitboard moves;

    Bitboard b = Bitboard().board.set(sq);
    Bitboard n = b.move_north(), w = b.move_west(),
             s = b.move_south(), e = b.move_east();
    Bitboard last_bit;

    while(n != 0x0)
    {
      moves |= n;
      last_bit.board = n.board;
      n = n.move_north();
    }
    moves &= ~last_bit;

    while(w != 0x0)
    {
      moves |= w;
      last_bit.board = w.board;
      w = w.move_west();
    }
    moves &= ~last_bit;

    while(s != 0x0)
    {
      moves |= s;
      last_bit.board = s.board;
      s = s.move_south();
    }
    moves &= ~last_bit;

    while(e != 0x0)
    {
      moves |= e;
      last_bit.board = e.board;
      e = e.move_east();
    }
    moves &= ~last_bit;

    m_blocker_mask[sq] |= moves;
  }

  return;
}

void Rook::__gen_pseudo_mask()
{
  for (int sq = 0; sq < MAX_BOARD_SQ; ++sq)
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

    m_pseudo_mask[sq] |= moves;
  }

  return;
}

Bitboard Rook::__moves_with_occ_at(uint8_t sq, Bitboard occ) const
{
  Bitboard moves;

  Bitboard b = Bitboard().board.set(sq);
  Bitboard n = b.move_north(), w = b.move_west(),
           s = b.move_south(), e = b.move_east();

  while(n != 0x0)
  {
    moves |= n;

    if ((n & occ) != 0x0)
    {
      break;
    }

    n = n.move_north();
  }

  while(w != 0x0)
  {
    moves |= w;

    if ((w & occ) != 0x0)
    {
      break;
    }

    w = w.move_west();
  }

  while(e != 0x0)
  {
    moves |= e;

    if ((e & occ) != 0x0)
    {
      break;
    }

    e = e.move_east();
  }

  while(s != 0x0)
  {
    moves |= s;

    if ((s & occ) != 0x0)
    {
      break;
    }

    s = s.move_south();
  }

  return moves;
}

/* > --------------------------------------------------         >>      BISHOP*/

Bitboard Bishop::get_moves(std::size_t sq, Bitboard occ) const
{
  occ &= m_blocker_mask[sq];
  occ *= bishop_magic_numbers[sq];
  occ >>= 64 - bishop_relevant_bits[sq];

  return m_attack_mask[sq][occ.board.to_ullong()];
}

void Bishop::__gen_attacks_mask()
{
  for (int sq = 0; sq < MAX_BOARD_SQ; ++sq)
  {
    uint8_t  relevant_bits_count = m_blocker_mask[sq].board.count();
    uint32_t occupancy_indicies  = (1 << relevant_bits_count);

    for (int index = 0; index < occupancy_indicies; ++index)
    {
      Bitboard occ = set_occupancy(index, relevant_bits_count, m_blocker_mask[sq]);

      int magic_index = ((occ * bishop_magic_numbers[sq])
                          >> (64 - bishop_relevant_bits[sq])).board.to_ulong();
      m_attack_mask[sq][magic_index] = __moves_with_occ_at(sq, occ);
    }
  }
  return;
}

Bitboard Bishop::__moves_with_occ_at(uint8_t sq, Bitboard occ) const
{
  Bitboard moves;

  Bitboard b = Bitboard().board.set(sq);
  Bitboard ne = b.move_no_ea(), nw = b.move_no_we(),
           se = b.move_so_ea(), sw = b.move_so_we();

  while(ne != 0x0)
  {
    moves |= ne;

    if ((ne & occ) != 0x0)
    {
      break;
    }

    ne = ne.move_no_ea();
  }

  while(nw != 0x0)
  {
    moves |= nw;

    if ((nw & occ) != 0x0)
    {
      break;
    }

    nw = nw.move_no_we();
  }

  while(se != 0x0)
  {
    moves |= se;

    if ((se & occ) != 0x0)
    {
      break;
    }

    se = se.move_so_ea();
  }

  while(sw != 0x0)
  {
    moves |= sw;

    if ((sw & occ) != 0x0)
    {
      break;
    }

    sw = sw.move_so_we();
  }

  return moves;
}

void Bishop::__gen_blocker_mask()
{
  Bitboard bounds_mask = Bitboard(0x7e7e7e7e7e7e00);

  for (int sq = 0; sq < MAX_BOARD_SQ; ++sq)
  {
    m_blocker_mask[sq] = m_pseudo_mask[sq] & bounds_mask;
  }

  return;
}

void Bishop::__gen_pseudo_mask()
{
  for (int sq = 0; sq < MAX_BOARD_SQ; ++sq)
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

    m_pseudo_mask[sq] |= moves;
  }

  return;
}

/* > --------------------------------------------------         >>      KNIGHT*/

Bitboard Knight::get_moves(std::size_t sq, Bitboard occ) const
{
  return m_pseudo_mask[sq];
}

void Knight::__gen_pseudo_mask()
{
  for (int sq = 0; sq < MAX_BOARD_SQ; ++sq)
  {
    Bitboard b = Bitboard().board.set(sq);

    Bitboard e = b.move_east() , n = b.move_north(),
             s = b.move_south(), w = b.move_west();

    Bitboard moves;

    moves = n.move_no_ea() | n.move_no_we() | s.move_so_ea() | s.move_so_we() |
             e.move_no_ea() | e.move_so_ea() | w.move_no_we() | w.move_so_we();

    m_pseudo_mask[sq] |= moves;
  }

  return;
}

/* > --------------------------------------------------         >>        PWAN*/

Bitboard Pawn::get_moves(std::size_t sq, Bitboard occ) const
{
  return m_pseudo_mask[sq];
}

Bitboard Pawn::get_pawn_capture(std::size_t sq) const
{
  return m_capture_mask[sq];
}

Bitboard Pawn::get_pawn_double(std::size_t sq) const
{
  Bitboard mask = m_opponent == Opponent::BLACK ? Mask["FIELD_7"]
                                                : Mask["FIELD_2"];
  if ((Bitboard().set_bit_at(sq) & mask) != 0)
  {
    return m_double_moves[((int)sq) % MAX_DOUBLE_PAWN_MV];
  }

  return Bitboard(0x0);
}

void Pawn::__gen_pseudo_mask()
{
  Bitboard (Bitboard::*func)(void) const;
  func = m_opponent == Opponent::BLACK ?
          &Bitboard::move_south : &Bitboard::move_north;

  for (int sq = 0; sq < MAX_BOARD_SQ; ++sq)
  {
    Bitboard b = Bitboard().board.set(sq);
    m_pseudo_mask[sq] |= (b.*func)();
  }

  return;
}

void Pawn::__gen_capture_mask()
{
  Bitboard (Bitboard::*west)(void) const;
  Bitboard (Bitboard::*east)(void) const;

  west = m_opponent == Opponent::BLACK ?
          &Bitboard::move_so_we : &Bitboard::move_no_we;

  east = m_opponent == Opponent::BLACK ?
          &Bitboard::move_so_ea : &Bitboard::move_no_ea;

  for (int sq = 0; sq < MAX_BOARD_SQ; ++sq)
  {
    Bitboard b = Bitboard().board.set(sq);
    m_capture_mask[sq] |= (b.*west)();
    m_capture_mask[sq] |= (b.*east)();
  }

  return;
}

void Pawn::__gen_double_mv_mask()
{
  Bitboard (Bitboard::*func)(void) const;
  func = m_opponent == Opponent::BLACK ?
          &Bitboard::move_south : &Bitboard::move_north;

  int site_modify = m_opponent == Opponent::BLACK ? 48 : 8;

  for (int sq = 0; sq < MAX_DOUBLE_PAWN_MV; ++sq)
  {
    Bitboard b = Bitboard(0x0);

    b.board.set(std::size_t(sq + site_modify));

    m_double_moves[sq] |= (b.*func)();
    b |= (b.*func)();
    m_double_moves[sq] |= (b.*func)();
  }
}