/* > Includes *****************************************************************/

#include "../inc/bitboard.hh"

/* > Defines ******************************************************************/

#define NOT_A_MASK Mask["RANK_A"].flip()
#define NOT_H_MASK Mask["RANK_H"].flip()

/* > Public Methods ***********************************************************/

inline uint32_t Bitboard::board_to_ulong() const
{
  return m_board.to_ulong();
}

inline bit_sqs Bitboard::get_board() const
{
  return m_board;
}

inline Bitboard Bitboard::move_north() {
  return *this << 8;
}

inline Bitboard Bitboard::move_east() {
  return (*this << 1) & NOT_A_MASK;
}

inline Bitboard Bitboard::move_south() {
  return *this >> 8;
}

inline Bitboard Bitboard::move_west() {
  return (*this >> 1) & NOT_H_MASK;
}

inline Bitboard Bitboard::move_no_ea() {
  return (*this << 9) & NOT_A_MASK;
}

inline Bitboard Bitboard::move_so_ea() {
  return (*this >> 7) & NOT_A_MASK;
}

inline Bitboard Bitboard::move_no_we() {
  return (*this << 7) & NOT_H_MASK;
}

inline Bitboard Bitboard::move_so_we() {
  return (*this >> 9) & NOT_H_MASK;
}