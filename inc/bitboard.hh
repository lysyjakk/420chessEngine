#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED

#include <iostream>
#include <bitset>
#include <map>
#include "traceAndError.hh"

typedef std::bitset<64> bit_sqs;

static std::map< std::string, bit_sqs > Mask {
  { "FIELD_1", bit_sqs(0xffffffffffffff00) },
  { "FIELD_2", bit_sqs(0xffffffffffff00ff) },
  { "FIELD_3", bit_sqs(0xffffffffff00ffff) },
  { "FIELD_4", bit_sqs(0xffffffff00ffffff) },
  { "FIELD_5", bit_sqs(0xffffff00ffffffff) },
  { "FIELD_6", bit_sqs(0xffff00ffffffffff) },
  { "FIELD_7", bit_sqs(0xff00ffffffffffff) },
  { "FIELD_8", bit_sqs(0x00ffffffffffffff) },

  { "RANK_A", bit_sqs(0x101010101010101) },
  { "RANK_B", bit_sqs(0x202020202020202) },
  { "RANK_C", bit_sqs(0x303030303030303) },
  { "RANK_D", bit_sqs(0x404040404040404) },
  { "RANK_E", bit_sqs(0x505050505050505) },
  { "RANK_F", bit_sqs(0x707070707070707) },
  { "RANK_G", bit_sqs(0x808080808080808) },
  { "RANK_H", bit_sqs(0x909090909090909) },

  };

class Bitboard
{
public:
  Bitboard() = default;

  Bitboard( bit_sqs new_bs):
    m_board(new_bs) {
  };

  Bitboard( long long unsigned int initval ) :
    m_board(initval) {
  }

  ~Bitboard() = default;

  Bitboard move_north();
  Bitboard move_east();
  Bitboard move_south();
  Bitboard move_west();
  Bitboard move_no_ea();
  Bitboard move_so_ea();
  Bitboard move_no_we();
  Bitboard move_so_we();

  inline uint32_t board_to_ulong() const;
  inline bit_sqs  get_board()      const;

  void print()
  {
    TRACE_INFO("%d", m_board.to_ulong());
  }

private:
  bit_sqs m_board;
};

inline Bitboard operator>>(Bitboard b,uint64_t amount) {
  return Bitboard(b.board_to_ulong() >> amount);
}

inline Bitboard operator<<(Bitboard b,uint64_t amount) {
  return Bitboard(b.board_to_ulong() << amount);
}

inline Bitboard operator&(Bitboard a, bit_sqs b) {
  return Bitboard(a.get_board() & b);
}

#endif // BITBOARD_H_INCLUDED