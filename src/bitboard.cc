/* > Includes *****************************************************************/

#include "../inc/bitboard.hh"

/* > Defines ******************************************************************/

/* > Public Methods ***********************************************************/

Bitboard Bitboard::move_north()
{
  return *this << 8;
}

Bitboard Bitboard::move_east()
{
  return (*this >> 1) & ~Mask["RANK_A"];
}

Bitboard Bitboard::move_south()
{
  return *this >> 8;
}

Bitboard Bitboard::move_west()
{
  return (*this << 1) & ~Mask["RANK_H"];
}

Bitboard Bitboard::move_no_ea()
{
  return (*this << 7) & ~Mask["RANK_A"];
}

Bitboard Bitboard::move_so_ea()
{
  return (*this >> 9) & ~Mask["RANK_A"];
}

Bitboard Bitboard::move_no_we()
{
  return (*this << 9) & ~Mask["RANK_H"];
}

Bitboard Bitboard::move_so_we()
{
  return (*this >> 7) & ~Mask["RANK_H"];
}