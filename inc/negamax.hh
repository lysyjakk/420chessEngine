#ifndef NEGA_MAX_H_INCLUDED
#define NEGA_MAX_H_INCLUDED

#include <vector>
#include "movelookup.hh"
#include "bitboard.hh"

typedef std::tuple< std::size_t, std::size_t > BestMove;

class NegaMax
{
public:
  NegaMax()  = default;
  ~NegaMax() = default;

  void init();

  BestMove get_best_move(ChessBoard   white_board,
                         ChessBoard   black_board,
                         Site         site,
                         SpecialMoves special_mv);

private:
  ChessBoard   m_black_board;
  ChessBoard   m_white_board;
  Site         m_site; 
  SpecialMoves m_special_mv;

  MoveLookup m_lookup;

  int __negamax(int alpha, int beta, int depth);
  int __evaluate();
  int __calculate_score(ChessBoard board,
                        Site site);
};

#endif //NEGA_MAX_H_INCLUDED