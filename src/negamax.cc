#include  "../inc/negamax.hh"

#define PAWN_INDEX           0
#define KNIGHT_INDEX         1
#define BISHOP_INDEX         2
#define ROOK_INDEX           3
#define QUEEN_INDEX          4
#define KING_INDEX           5
#define BLACK_INDEX_MODIFIER 6
#define WHITE_INDEX_MODIFIER 0

enum {
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8, no_sq
};

static ChessBoard *ally_board;
static ChessBoard *enemy_board;

static int half_move;

static BestMove best_move;

// material scrore

/*
    ♙ =   100   = ♙
    ♘ =   300   = ♙ * 3
    ♗ =   350   = ♙ * 3 + ♙ * 0.5
    ♖ =   500   = ♙ * 5
    ♕ =   1000  = ♙ * 10
    ♔ =   10000 = ♙ * 100
*/

static const int material_score[12] = {
    100,      // white pawn score
    300,      // white knight scrore
    350,      // white bishop score
    500,      // white rook score
   1000,      // white queen score
  10000,      // white king score
   -100,      // black pawn score
   -300,      // black knight scrore
   -350,      // black bishop score
   -500,      // black rook score
  -1000,      // black queen score
 -10000,      // black king score
};

// pawn positional score
const int pawn_score[64] = 
{
  90,  90,  90,  90,  90,  90,  90,  90,
  30,  30,  30,  40,  40,  30,  30,  30,
  20,  20,  20,  30,  30,  30,  20,  20,
  10,  10,  10,  20,  20,  10,  10,  10,
   5,   5,  10,  20,  20,   5,   5,   5,
   0,   0,   0,   5,   5,   0,   0,   0,
   0,   0,   0, -10, -10,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0
};

// knight positional score
const int knight_score[64] = 
{
  -5,   0,   0,   0,   0,   0,   0,  -5,
  -5,   0,   0,  10,  10,   0,   0,  -5,
  -5,   5,  20,  20,  20,  20,   5,  -5,
  -5,  10,  20,  30,  30,  20,  10,  -5,
  -5,  10,  20,  30,  30,  20,  10,  -5,
  -5,   5,  20,  10,  10,  20,   5,  -5,
  -5,   0,   0,   0,   0,   0,   0,  -5,
  -5, -10,   0,   0,   0,   0, -10,  -5
};

// bishop positional score
const int bishop_score[64] = 
{
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,  10,  10,   0,   0,   0,
  0,   0,  10,  20,  20,  10,   0,   0,
  0,   0,  10,  20,  20,  10,   0,   0,
  0,  10,   0,   0,   0,   0,  10,   0,
  0,  30,   0,   0,   0,   0,  30,   0,
  0,   0, -10,   0,   0, -10,   0,   0
};

// rook positional score
const int rook_score[64] =
{
  50,  50,  50,  50,  50,  50,  50,  50,
  50,  50,  50,  50,  50,  50,  50,  50,
   0,   0,  10,  20,  20,  10,   0,   0,
   0,   0,  10,  20,  20,  10,   0,   0,
   0,   0,  10,  20,  20,  10,   0,   0,
   0,   0,  10,  20,  20,  10,   0,   0,
   0,   0,  10,  20,  20,  10,   0,   0,
   0,   0,   0,  20,  20,   0,   0,   0
};

// king positional score
const int king_score[64] = 
{
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   5,   5,   5,   5,   0,   0,
  0,   5,   5,  10,  10,   5,   5,   0,
  0,   5,  10,  20,  20,  10,   5,   0,
  0,   5,  10,  20,  20,  10,   5,   0,
  0,   0,   5,  10,  10,   5,   0,   0,
  0,   5,   5,  -5,  -5,   0,   5,   0,
  0,   0,   5,   0, -15,   0,  10,   0
};

// mirror positional score tables for opposite side
const int mirror_score[64] =
{
  a8, b8, c8, d8, e8, f8, g8, h8,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a1, b1, c1, d1, e1, f1, g1, h1
};

BestMove NegaMax::get_best_move(ChessBoard   white_board,
                                ChessBoard   black_board,
                                Site         site,
                                SpecialMoves special_mv)
{
  m_lookup.init();
  m_white_board = white_board;
  m_black_board = black_board;
  m_special_mv  = special_mv;
  m_site        = site;
  std::cout << "\n\nscore: " << __evaluate() << "\n\n";
  __negamax(-50000, 50000, 4);

/*  std::cout << "-------------------- src --------------------" << std::endl;
  Bitboard().set_bit_at(std::get<0>(best_move)).print();
  std::cout << "-------------------- desc -------------------" << std::endl;
  Bitboard().set_bit_at(std::get<1>(best_move)).print();
  std::cout << "\n---------------------------------------------" << std::endl;
*/
  return best_move;
}
int node;


int NegaMax::__negamax(int alpha, int beta, int depth)
{
  if (depth == 0)
  {
    return __evaluate();
  }

  GenMoves legal_mv;
  BestMove best_mv_sofar;

  int old_alpha = alpha;

  if (m_site == Site::WHITE)
  {
    ally_board  = &m_white_board;
    enemy_board = &m_black_board;
  }
  else
  {
    ally_board  = &m_black_board;
    enemy_board = &m_white_board;
  }

  legal_mv = m_lookup.gen_moves_current_state(*ally_board,
                                              *enemy_board,
                                               m_site,
                                               m_special_mv);

  for (auto itr : legal_mv)
  {
    //Copy necessary information
    ChessBoard   white_board_cp = m_white_board;
    ChessBoard   black_board_cp = m_black_board;
    Site         site_cp        = m_site;
    SpecialMoves special_mv_cp  = m_special_mv;

    ++half_move;

    //Make move
    MoveToMake move;

    move.sq_src     = itr.src;
    move.sq_dest    = itr.desc;
    move.piece_type = itr.piece_type;
    move.site       = m_site;
    move.special_mv = m_special_mv;

    m_lookup.make_move(&move, ally_board, enemy_board);

    m_special_mv = move.special_mv;


    int dupa = __evaluate();

              std::cout << "-------------------- info --------------------" << std::endl;
  std::cout << "SITE: (" << (int)m_site << ")\n";
  std::cout << "NODE: (" << node << ")\n";
  std::cout << "SCORE: (" << dupa << ")\n";
  std::cout << "-------------------- src --------------------" << std::endl;
  Bitboard().set_bit_at(itr.src).print();
  std::cout << "-------------------- desc -------------------" << std::endl;
  Bitboard().set_bit_at(itr.desc).print();
  std::cout << "\n---------------------------------------------" << std::endl;


    change_player_turn(m_site);
    node++;
    int score = -__negamax(-beta, -alpha, depth - 1);

    --half_move;

    //Restore previous information
    m_white_board = white_board_cp;
    m_black_board = black_board_cp;
    m_site        = site_cp;
    m_special_mv  = special_mv_cp;

    if (score >= beta)
    {
      return beta;
    }

    if (score > alpha)
    {
      alpha = score;
      
      if (half_move == 0)
      {
        best_mv_sofar = std::make_tuple(itr.src, itr.desc);
      }
    }
  }
  
  if (old_alpha != alpha)
  {
    best_move = best_mv_sofar;
  }

  return alpha;
}

int NegaMax::__evaluate()
{
  int score = 0;

  // Calculate score for white
  score += __calculate_score(m_white_board, Site::WHITE);

  // Calculate score for black
  score += __calculate_score(m_black_board, Site::BLACK);


  return m_site == Site::WHITE ? score : -score;
}

int NegaMax::__calculate_score(ChessBoard board,
                               Site site)
{
  int score = 0;
  int index = 1;
  int modifier = site == Site::BLACK ? BLACK_INDEX_MODIFIER
                                     : WHITE_INDEX_MODIFIER;
  const int *score_array;

  // Calculate score for white
  for (Bitboard* itr = get_begin(&board);
       itr < get_end(&board);
       ++itr)
  {
    std::vector<std::size_t> v = itr -> scan_for_bit_index();

    switch (index)
    {
    case 1: //Pawns
      score_array = pawn_score;
      score += material_score[PAWN_INDEX + modifier];
    break;

    case 2: //Rook
      score_array = rook_score;
      score += material_score[ROOK_INDEX + modifier];
    break;

    case 3: //Knights
      score_array = knight_score;
      score += material_score[KNIGHT_INDEX + modifier];
    break;

    case 4: //Bishop
      score_array = bishop_score;
      score += material_score[BISHOP_INDEX + modifier];
    break;

    case 5: //Queen
      score += material_score[QUEEN_INDEX + modifier];
      continue;
    break;

    case 6: //King
      score_array = king_score;
      score += material_score[KING_INDEX + modifier];
    break;

    default:
    break;
    }

    if (site == Site::WHITE)
    {
      for (int i = 0; i < v.size(); ++i)
      {
        score += score_array[ v[i] ];
      }
    }
    else
    {
      for (int i = 0; i < v.size(); ++i)
      {
        score -= score_array[ mirror_score[v[i]] ];
      }
    }

    ++index;
  }

  return score;
}