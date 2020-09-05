#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED

#include <iostream>
#include <bitset>
#include <map>
#include <vector>
#include "traceAndError.hh"

typedef std::bitset<64> bit_sqs;

static std::map< std::string, bit_sqs > Mask {
  { "FIELD_1", bit_sqs(0x00000000000000ff) },
  { "FIELD_2", bit_sqs(0x000000000000ff00) },
  { "FIELD_3", bit_sqs(0x0000000000ff0000) },
  { "FIELD_4", bit_sqs(0x00000000ff000000) },
  { "FIELD_5", bit_sqs(0x000000ff00000000) },
  { "FIELD_6", bit_sqs(0x0000ff0000000000) },
  { "FIELD_7", bit_sqs(0x00ff000000000000) },
  { "FIELD_8", bit_sqs(0xff00000000000000) },

  { "RANK_A", bit_sqs(0x8080808080808080) },
  { "RANK_B", bit_sqs(0x4040404040404040) },
  { "RANK_C", bit_sqs(0x2020202020202020) },
  { "RANK_D", bit_sqs(0x1010101010101010) },
  { "RANK_E", bit_sqs(0x808080808080808) },
  { "RANK_F", bit_sqs(0x404040404040404) },
  { "RANK_G", bit_sqs(0x202020202020202) },
  { "RANK_H", bit_sqs(0x101010101010101) },

  };

class Bitboard
{
public:
  Bitboard() = default;

  Bitboard( bit_sqs new_bs ):
    board(new_bs) {};

  Bitboard( long long unsigned int initval ) :
    board(initval) {};

  ~Bitboard() = default;

  Bitboard move_north() const;
  Bitboard move_east () const;
  Bitboard move_south() const;
  Bitboard move_west () const;
  Bitboard move_no_ea() const;
  Bitboard move_so_ea() const;
  Bitboard move_no_we() const;
  Bitboard move_so_we() const;

  Bitboard set_bit_at(std::size_t index, bool val = true) const;
  Bitboard pop_bit_at(std::size_t index)                  const;

  std::vector<std::size_t> scan_for_bit_index() const;

  void print();

  bit_sqs board;
};

/* > Bit shifts operators *****************************************************/

inline Bitboard operator>>(Bitboard b, uint8_t amount)
{
  return Bitboard(b.board.to_ullong() >> amount);
}

inline Bitboard operator<<(Bitboard b, uint8_t amount)
{
  return Bitboard(b.board.to_ullong() << amount);
}

inline Bitboard operator>>=(Bitboard &b, uint8_t amount)
{
  b.board = b.board >> amount;
  return b;
}

/* > Bit logical operators ****************************************************/

inline Bitboard operator&(Bitboard a, Bitboard b)
{
  return Bitboard(a.board & b.board);
}

inline Bitboard operator&(Bitboard a, bit_sqs b)
{
  return Bitboard(a.board & b);
}

inline Bitboard operator&(Bitboard a, uint64_t amount)
{
  return Bitboard(a.board.to_ullong() & amount);
}

inline Bitboard operator&=(Bitboard &a, Bitboard b)
{
  a = a & b;
  return a;
}

inline Bitboard operator|(Bitboard b, uint64_t amount)
{
  return Bitboard(b.board | amount);
}

inline Bitboard operator|(Bitboard a,Bitboard b)
{
  return Bitboard(a.board | b.board);
}

inline Bitboard operator|=(Bitboard &a, Bitboard b)
{
  a = a | b;
  return a;
}

inline Bitboard operator~(Bitboard a)
{
  return Bitboard(~a.board);
}

inline bool operator==(Bitboard a,Bitboard b)
{
  return a.board == b.board;
}

inline bool operator==(Bitboard a,uint64_t b)
{
  return a.board.to_ullong() == b;
}

inline bool operator!=(Bitboard a,Bitboard b)
{
  return a.board != b.board;
}


inline bool operator!=(Bitboard a,uint64_t b)
{
  return a.board.to_ullong() != b;
}

/* > Bit arithmetic operators *************************************************/

inline Bitboard operator-(Bitboard a, uint64_t amount)
{
  return Bitboard( (a.board.to_ullong() - amount) );
}

inline Bitboard operator-(Bitboard a)
{
  return Bitboard( -(a.board.to_ullong()) );
}

inline Bitboard operator*(Bitboard a, uint64_t amount)
{
  return Bitboard( (a.board.to_ullong() * amount) );
}

inline Bitboard operator*(Bitboard a, Bitboard b)
{
  return Bitboard( (a.board.to_ullong() * b.board.to_ullong()) );
}

inline Bitboard operator*=(Bitboard &a, uint64_t amount)
{
  a.board = a.board.to_ullong() * amount;
  return a;
}

#endif // BITBOARD_H_INCLUDED