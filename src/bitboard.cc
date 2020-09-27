/* > Includes *****************************************************************/

#include "../inc/bitboard.hh"

/* > Defines ******************************************************************/

/* > Public Methods ***********************************************************/

void Bitboard::print()
 {
  std::cout << std::endl;
  for (int i = 8; i > 0; i--)
  {
    std::bitset<8> temp(board.to_ulong() >> 8 * (i-1));
    std::cout << temp << std::endl;
  }
}

Bitboard Bitboard::set_bit_at(std::size_t index, bool val) const
{
  Bitboard b = Bitboard(board);
  b.board.set(index, val);
  return b;
}

Bitboard Bitboard::pop_bit_at(std::size_t index) const
{
  Bitboard b = Bitboard(board);
  b.board ^= (1ULL << index);
  return b;
}

std::vector<std::size_t> Bitboard::scan_for_bit_index() const
{
  std::vector<std::size_t> result;

  for (std::size_t itr = 0; itr < board.size(); ++itr)
  {
    if (board[itr] == true)
    {
      result.push_back(itr);
    }
  }

  return result;
}

Bitboard Bitboard::move_north() const
{
  return *this << 8;
}

Bitboard Bitboard::move_east() const
{
  return (*this >> 1) & ~Mask["RANK_A"];
}

Bitboard Bitboard::move_south() const
{
  return *this >> 8;
}

Bitboard Bitboard::move_west() const
{
  return (*this << 1) & ~Mask["RANK_H"];
}

Bitboard Bitboard::move_no_ea() const
{
  return (*this << 7) & ~Mask["RANK_A"];
}

Bitboard Bitboard::move_so_ea() const
{
  return (*this >> 9) & ~Mask["RANK_A"];
}

Bitboard Bitboard::move_no_we() const
{
  return (*this << 9) & ~Mask["RANK_H"];
}

Bitboard Bitboard::move_so_we() const
{
  return (*this >> 7) & ~Mask["RANK_H"];
}