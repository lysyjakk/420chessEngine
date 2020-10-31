#ifndef MOVE_LOOKUP_H_INCLUDED
#define MOVE_LOOKUP_H_INCLUDED

#include <vector>
#include <bitset>
#include <tuple>
#include "pieces.hh"
#include "traceAndError.hh"
#include "errorCodes.hh"
#include "promotion.hh"

#define ENPASS_NO_SQ 0

enum class Site
{
  WHITE,
  BLACK
};

enum CastleRights
{
  WHITE_KING_SIDE  = 1,
  WHITE_QUEEN_SIDE = 2,
  BLACK_KING_SIDE  = 4,
  BLACK_QUEEN_SIDE = 8
};

enum TypesOfPieces
{
  NONE,

// White pieces
  WHITE_PAWN,
  WHITE_ROOK,
  WHITE_KNIGHT,
  WHITE_BISHOP,
  WHITE_QUEEN,
  WHITE_KING,

// Black pieces
  BLACK_PAWN,
  BLACK_ROOK,
  BLACK_KNIGHT,
  BLACK_BISHOP,
  BLACK_QUEEN,
  BLACK_KING
};

typedef struct
{
  std::size_t src;
  std::size_t desc;

  TypesOfPieces piece_type;

} SingleMoveGen;

typedef struct
{
  Bitboard pawns;
  Bitboard rooks;
  Bitboard knights;
  Bitboard bishops;
  Bitboard queens;
  Bitboard king;
} ChessBoard;

typedef struct
{
  std::bitset<64> en_passant;
  std::bitset<4>  castle;

  Bitboard w_double_mv_pawn;
  Bitboard b_double_mv_pawn;

} SpecialMoves;

typedef struct
{
  std::size_t   sq_src;
  std::size_t   sq_dest;
  TypesOfPieces piece_type;
  Site          site;
  SpecialMoves  special_mv;

} MoveToMake;

typedef std::vector< SingleMoveGen > GenMoves;

static struct PseudoLegalMoves
{
  King   *king;
  Rook   *rook;
  Knight *knight;
  Bishop *bishop;
  Pawn   *b_pawn;
  Pawn   *w_pawn;
} pseudolegal_mv;

static inline void change_player_turn(Site &site)
{
  int turn_index = (int)site;

  turn_index ^= 1;
  site = Site(turn_index);

  return;
}

class MoveLookup
{
public:
  MoveLookup()  = default;
  ~MoveLookup() = default;

  void init();

  void make_move(MoveToMake *move,
                 ChessBoard *ally_board,
                 ChessBoard *enemy_board);

  GenMoves gen_moves_current_state(ChessBoard   ally_board,
                                   ChessBoard   enemy_board,
                                   Site         site,
                                   SpecialMoves special_mv);

  bool is_move_valid(MoveToMake move,
                     ChessBoard ally_board,
                     ChessBoard enemy_board);

  bool is_king_checkmate(Site site);

  bool is_king_check(ChessBoard ally_board,
                     ChessBoard enemy_board,
                     Site site);


private:
  ChessBoard   m_ally_board;
  ChessBoard   m_enemy_board;
  ChessBoard  *m_white_board;
  ChessBoard  *m_black_board;

  SpecialMoves m_special_moves;

  bool __is_sq_attacked(std::size_t sq, Site site);

  bool __is_move_correct(std::size_t   sq_src,
                         std::size_t   sq_dest,
                         TypesOfPieces piece_type,
                         Site          site);

  bool __is_sq_occupate_by(std::size_t sq,
                           Site        site);

  bool __is_king_checked(Site site);

  bool __is_special_move(std::size_t   sq_src,
                         std::size_t   sq_dest,
                         TypesOfPieces piece_type,
                         Site          site);

  void __remove_piece_at_sq(std::size_t sq,
                            ChessBoard  *board);
};


#endif // MOVE_LOOKUP_H_INCLUDED