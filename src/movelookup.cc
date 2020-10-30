#include "../inc/movelookup.hh"

#define NO_DOUBLE_MV_SQ     0
#define BLACK_KING_INIT_SQ 60
#define WHITE_KING_INIT_SQ  4
#define B_CASTLE_K_SIDE_SQ 62
#define B_CASTLE_Q_SIDE_SQ 58
#define W_CASTLE_K_SIDE_SQ  6
#define W_CASTLE_Q_SIDE_SQ  2

static std::size_t en_pass_candidate;
static std::size_t double_mv_pawn_sq;


/*
                           castling   move     in      in
                              right update     binary  decimal
 king & rooks didn't move:     1111 & 1111  =  1111    15
        white king  moved:     1111 & 1100  =  1100    12
  white king's rook moved:     1111 & 1110  =  1110    14
 white queen's rook moved:     1111 & 1101  =  1101    13

         black king moved:     1111 & 0011  =  1011    3
  black king's rook moved:     1111 & 1011  =  1011    11
 black queen's rook moved:     1111 & 0111  =  0111    7
*/

const uint8_t castling_rights[64] = {
    13, 15, 15, 15, 12, 15, 15, 14,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
     7, 15, 15, 15,  3, 15, 15, 11
};


/*
                             OCCUPANCIES
     White occupancy       Black occupancy       All occupancies
  8  0 0 0 0 0 0 0 0    8  1 1 1 1 1 1 1 1    8  1 1 1 1 1 1 1 1
  7  0 0 0 0 0 0 0 0    7  1 1 1 1 1 1 1 1    7  1 1 1 1 1 1 1 1
  6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0    6  0 0 0 0 0 0 0 0
  5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0    5  0 0 0 0 0 0 0 0
  4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0    4  0 0 0 0 0 0 0 0
  3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0    3  0 0 0 0 0 0 0 0
  2  1 1 1 1 1 1 1 1    2  0 0 0 0 0 0 0 0    2  1 1 1 1 1 1 1 1
  1  1 1 1 1 1 1 1 1    1  0 0 0 0 0 0 0 0    1  1 1 1 1 1 1 1 1


                            ALL TOGETHER
                        8  ♖ ♘ ♗ ♕ ♔ ♗ ♘ ♖
                        7  ♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙
                        6  . . . . . . . .
                        5  . . . . . . . .
                        4  . . . . . . . .
                        3  . . . . . . . .
                        2  ♟︎ ♟︎ ♟︎ ♟︎ ♟︎ ♟︎ ♟︎ ♟︎
                        1  ♜ ♞ ♝ ♛ ♚ ♝ ♞ ♜
                           a b c d e f g h
*/

static Bitboard __get_occupancies(ChessBoard board)
{
  Bitboard b = Bitboard(0x0);

  b |= board.king;
  b |= board.queens;
  b |= board.rooks;
  b |= board.knights;
  b |= board.bishops;
  b |= board.pawns;

  return b;
}

static Bitboard __get_both_occupancies(ChessBoard board_1,
                                       ChessBoard board_2)
{
  Bitboard b = Bitboard(0x0);

  b |= board_1.king;
  b |= board_1.queens;
  b |= board_1.rooks;
  b |= board_1.knights;
  b |= board_1.bishops;
  b |= board_1.pawns;

  b |= board_2.king;
  b |= board_2.queens;
  b |= board_2.rooks;
  b |= board_2.knights;
  b |= board_2.bishops;
  b |= board_2.pawns;

  return b;
}

// Here's a little explanation.
//
// Why the queen move generating function is in this
// file instead of on file pieces.hh?

// The answer is simple, the movements of queen are
// a combination of bishop and rook moves, and all movements
// of these figures are generated at the start and
// take up some memory, so in order to save it we
// will combine the movements of both figures here
// to generate queen moves

static Bitboard get_queen_moves(std::size_t sq, Bitboard occ)
{
  Bitboard b = Bitboard(0x0);

  b |= pseudolegal_mv.bishop -> get_moves(sq, occ);
  b |= pseudolegal_mv.rook   -> get_moves(sq, occ);

  return b;
}

void MoveLookup::init()
{
  TRACE_INFO("Generation of pieces moves...");

  pseudolegal_mv.king   = new King   (Opponent::BOTH);
  pseudolegal_mv.rook   = new Rook   (Opponent::BOTH);
  pseudolegal_mv.knight = new Knight (Opponent::BOTH);
  pseudolegal_mv.bishop = new Bishop (Opponent::BOTH);
  pseudolegal_mv.b_pawn = new Pawn   (Opponent::BLACK);
  pseudolegal_mv.w_pawn = new Pawn   (Opponent::WHITE);

  TRACE_INFO("Generation of moves sucessful");
  TRACE_INFO("Initialization new game sucessful");
}

bool MoveLookup::is_move_valid(MoveToMake move,
                               ChessBoard ally_board,
                               ChessBoard enemy_board)
{
  bool result = true;

  m_ally_board    = ally_board;
  m_enemy_board   = enemy_board;
  m_white_board   = move.site == Site::BLACK ? &enemy_board : &ally_board;
  m_black_board   = move.site == Site::BLACK ? &ally_board : &enemy_board;
  m_special_moves = move.special_mv;

  if (__is_sq_occupate_by(move.sq_dest, move.site))
  {
    //TRACE_INFO("DUPA1");
    result = false;
  }
  else if ( !__is_move_correct(move.sq_src,
                               move.sq_dest,
                               move.piece_type,
                               move.site) )
  {
   // TRACE_INFO("DUPA2");
    result = false;
  }
  //else if(__is_king_checked(site))
  //{
    //TRACE_INFO("DUPA3");
    //result = false;
  //}

  return result;
}


void MoveLookup::make_move(MoveToMake *move,
                           ChessBoard *ally_board,
                           ChessBoard *enemy_board)
{
  Bitboard *piece;
  Bitboard src  = Bitboard().set_bit_at(move -> sq_src);
  Bitboard desc = Bitboard().set_bit_at(move -> sq_dest);

  SpecialMoves *special_moves = &move -> special_mv;

  switch (move -> piece_type)
  {
    case WHITE_ROOK:
    case BLACK_ROOK:
      piece = &ally_board -> rooks;

      special_moves -> castle &= castling_rights[move -> sq_src];
      special_moves -> en_passant = ENPASS_NO_SQ;
    break;

    case WHITE_KNIGHT:
    case BLACK_KNIGHT:
      piece = &ally_board -> knights;
      special_moves -> en_passant = ENPASS_NO_SQ;
    break;

    case WHITE_BISHOP:
    case BLACK_BISHOP:
      piece = &ally_board -> bishops;
      special_moves -> en_passant = ENPASS_NO_SQ;
    break;

    case WHITE_QUEEN:
    case BLACK_QUEEN:
      piece = &ally_board -> queens;
      special_moves -> en_passant = ENPASS_NO_SQ;
    break;

    case WHITE_KING:
    {
      if (special_moves -> castle.any())
      {
        Bitboard king_side = Bitboard(0x40);
        Bitboard queen_side = Bitboard(0x04);
        Bitboard src_mask = Bitboard(0x10);

        if ( ((src & src_mask) != 0) &&
             ((desc & king_side) != 0) &&
             ((special_moves -> castle.to_ullong() & WHITE_KING_SIDE) != 0))
        {
           ally_board -> rooks |= Bitboard(0x20);
           ally_board -> rooks &= ~Bitboard(0x80);
        }
        else if ( ((src & src_mask) != 0) &&
                ((desc & queen_side) != 0) &&
                ((special_moves -> castle.to_ullong() & WHITE_QUEEN_SIDE) != 0))
        {
          ally_board -> rooks |= Bitboard(0x08);
          ally_board -> rooks &= ~Bitboard(0x01);
        }
      }

      piece = &ally_board -> king;
      special_moves -> castle &= castling_rights[move -> sq_src];
      special_moves -> en_passant = ENPASS_NO_SQ;
    }
    break;
    case BLACK_KING:
    {
      if (special_moves -> castle.any())
      {
        Bitboard king_side = Bitboard(0x4000000000000000);
        Bitboard queen_side = Bitboard(0x400000000000000);
        Bitboard src_mask = Bitboard(0x1000000000000000);

        if ( ((src & src_mask) != 0) &&
             ((desc & king_side) != 0) &&
             ((special_moves -> castle.to_ullong() & BLACK_KING_SIDE) != 0))
        {
          ally_board -> rooks |= Bitboard(0x2000000000000000);
          ally_board -> rooks &= ~Bitboard(0x8000000000000000);
        }
        else if ( ((src & src_mask) != 0) &&
                ((desc & queen_side) != 0) &&
                ((special_moves -> castle.to_ullong() & BLACK_QUEEN_SIDE) != 0))
        {
          ally_board -> rooks |= Bitboard(0x800000000000000);
          ally_board -> rooks &= ~Bitboard(0x8000000000000000);
          ally_board -> rooks.print();
        }
      }

      piece = &ally_board -> king;
      special_moves -> castle &= castling_rights[move -> sq_src];
      special_moves -> en_passant = ENPASS_NO_SQ;
    }
    break;

    case BLACK_PAWN:
    case WHITE_PAWN:
    {
      piece = &ally_board -> pawns;
      Bitboard *double_mv_right =
                move -> site == Site::BLACK ? &move -> special_mv.b_double_mv_pawn
                                            : &move -> special_mv.w_double_mv_pawn;

      //Double move pawn
      if ( ((src & ally_board -> pawns) != 0) &&
           ((src & *double_mv_right) != 0) )
      {
        Pawn *pawn = move -> site == Site::BLACK ? pseudolegal_mv.b_pawn
                                                 : pseudolegal_mv.w_pawn;

        Bitboard mv = pawn -> get_pawn_double(move -> sq_src);

        Bitboard mask = move -> site == Site::BLACK ? Mask["FIELD_6"]
                                                    : Mask["FIELD_3"];

        *double_mv_right &= ~Bitboard().set_bit_at(move -> sq_src);
        special_moves -> en_passant.set((std::size_t)get_ls1b_index((mv & mask)));
      }
      //En passant
      else if (Bitboard().set_bit_at(move -> sq_dest) == special_moves -> en_passant)
      {
        Bitboard (Bitboard::*func)(void) const;
        func = move -> site == Site::BLACK ? &Bitboard::move_north
                                           : &Bitboard::move_south;
        Bitboard pawn_to_rm = Bitboard().set_bit_at(move -> sq_dest);

        pawn_to_rm = (pawn_to_rm.*func)();

        enemy_board -> pawns &= ~pawn_to_rm;
        special_moves -> en_passant = ENPASS_NO_SQ;
      }

      else
      {
        special_moves -> en_passant = ENPASS_NO_SQ;

        Bitboard mask = move -> site == Site::BLACK ? Mask["FIELD_1"]
                                                    : Mask["FIELD_8"];

        //Promotion
        if ((desc & mask) != 0)
        {
          int promotion = promotion_pawn(move -> site);

          *piece &= piece -> set_bit_at(move -> sq_src, 0);

          if (promotion == PROMOTION_TO_QUEEN)
            piece = &ally_board -> queens;
          else if (promotion == PROMOTION_TO_ROOK)
            piece = &ally_board -> rooks;
          else if (promotion == PROMOTION_TO_BISHOP)
            piece = &ally_board -> bishops;
          else if (promotion == PROMOTION_TO_KNIGHT)
            piece = &ally_board -> knights;
        }
      }
    }
    break;

    //> NONE
    case NONE:
      TRACE_WARNING("Received NONE type piece in movelookup.");
    break;

    default:
      FATAL_ERROR(ERROR_INVALID_VARIABLE,
          "Invalid variable (%i) in chess board array!",
          move -> piece_type);
    break;
  }

  *piece &= piece -> set_bit_at(move -> sq_src, 0);
  *piece |= piece -> set_bit_at(move -> sq_dest);

  __remove_piece_at_sq(move -> sq_dest, enemy_board);

  return;
}

GenMoves MoveLookup::gen_moves_current_state(ChessBoard   ally_board,
                                             ChessBoard   enemy_board,
                                             Site         site,
                                             SpecialMoves special_mv)
{
  Bitboard both_occ = __get_both_occupancies(ally_board, enemy_board);

  GenMoves moves_v;
  MoveToMake move;
  TypesOfPieces type;

  std::vector< std::size_t > piece_src_sq;
  std::vector< std::size_t > piece_src_desc;

  move.site = site;
  move.special_mv = special_mv;
  piece_src_sq = ally_board.pawns.scan_for_bit_index();

//------------------------------------------------------------------->      Pawn

  type = site == Site::BLACK ? BLACK_PAWN
                             : WHITE_PAWN;

  Bitboard mask = site == Site::BLACK ? Mask["FIELD_7"]
                                      : Mask["FIELD_2"];

  move.piece_type = type;

  for (std::size_t src = 0; src < piece_src_sq.size(); ++src)
  {
    Bitboard pseudo_mv =
                site == Site::BLACK ? pseudolegal_mv.b_pawn -> get_moves(piece_src_sq[src])
                                    : pseudolegal_mv.w_pawn -> get_moves(piece_src_sq[src]);
    Bitboard pseudo_double =
                site == Site::BLACK ? pseudolegal_mv.b_pawn -> get_pawn_double(piece_src_sq[src])
                                    : pseudolegal_mv.w_pawn -> get_pawn_double(piece_src_sq[src]);
    Bitboard pseudo_capture = 
                site == Site::BLACK ? pseudolegal_mv.b_pawn -> get_pawn_capture(piece_src_sq[src])
                                    : pseudolegal_mv.w_pawn -> get_pawn_capture(piece_src_sq[src]);

    piece_src_desc = pseudo_mv.scan_for_bit_index();
    move.sq_src     = piece_src_sq[src];

    for (std::size_t desc = 0; desc < piece_src_desc.size(); ++desc)
    {
      move.sq_dest = piece_src_desc[desc];

      if (is_move_valid(move, ally_board, enemy_board) == true)
      {
        SingleMoveGen gen_mv;

        gen_mv.src        = piece_src_sq[src];
        gen_mv.desc       = piece_src_desc[desc];
        gen_mv.piece_type = type;

        moves_v.push_back(gen_mv);
      }
    }

    piece_src_desc = pseudo_capture.scan_for_bit_index();

    for (std::size_t desc = 0; desc < piece_src_desc.size(); ++desc)
    {
      move.sq_dest = piece_src_desc[desc];

      if (is_move_valid(move, ally_board, enemy_board) == true)
      {
        SingleMoveGen gen_mv;

        gen_mv.src        = piece_src_sq[src];
        gen_mv.desc       = piece_src_desc[desc];
        gen_mv.piece_type = type;

        moves_v.push_back(gen_mv);
      }
    }

    //Double forward
    if ( (Bitboard().set_bit_at(piece_src_sq[src]) & mask) != 0 )
    {
      
      piece_src_desc = pseudo_double.scan_for_bit_index();

      for (std::size_t desc = 0; desc < piece_src_desc.size(); ++desc)
      {
        move.sq_dest = piece_src_desc[desc];

        if (is_move_valid(move, ally_board, enemy_board) == true)
        {
          SingleMoveGen gen_mv;

          gen_mv.src        = piece_src_sq[src];
          gen_mv.desc       = piece_src_desc[desc];
          gen_mv.piece_type = type;

          moves_v.push_back(gen_mv);
        }
      }
    }
  }

//------------------------------------------------------------------->    Bishop
  type = site == Site::BLACK ? BLACK_BISHOP
                             : WHITE_BISHOP;

  piece_src_sq = ally_board.bishops.scan_for_bit_index();
  move.piece_type = type;

  for (std::size_t src = 0; src < piece_src_sq.size(); ++src)
  {
    Bitboard pseudo_mv = pseudolegal_mv.bishop -> get_moves(piece_src_sq[src], both_occ);

    piece_src_desc  = pseudo_mv.scan_for_bit_index();
    move.sq_src     = piece_src_sq[src];

    for (std::size_t desc = 0; desc < piece_src_desc.size(); ++desc)
    {
      move.sq_dest = piece_src_desc[desc];
  
      if (is_move_valid(move, ally_board, enemy_board) == true)
      {
        SingleMoveGen gen_mv;

        gen_mv.src        = piece_src_sq[src];
        gen_mv.desc       = piece_src_desc[desc];
        gen_mv.piece_type = type;
  
        moves_v.push_back(gen_mv);
      }
    }
  }

//------------------------------------------------------------------->    Knight
  type = site == Site::BLACK ? BLACK_KNIGHT
                             : WHITE_KNIGHT;

  piece_src_sq = ally_board.knights.scan_for_bit_index();
  move.piece_type = type;

  for (std::size_t src = 0; src < piece_src_sq.size(); ++src)
  {
    Bitboard pseudo_mv = pseudolegal_mv.knight -> get_moves(piece_src_sq[src]);

    piece_src_desc = pseudo_mv.scan_for_bit_index();
    move.sq_src    = piece_src_sq[src];

    for (std::size_t desc = 0; desc < piece_src_desc.size(); ++desc)
    {
      move.sq_dest = piece_src_desc[desc];

      if (is_move_valid(move, ally_board, enemy_board) == true)
      {
        SingleMoveGen gen_mv;

        gen_mv.src        = piece_src_sq[src];
        gen_mv.desc       = piece_src_desc[desc];
        gen_mv.piece_type = type;
  
        moves_v.push_back(gen_mv);
      }
    }
  }

//------------------------------------------------------------------->      Rook
  type = site == Site::BLACK ? BLACK_ROOK
                             : WHITE_ROOK;

  piece_src_sq = ally_board.rooks.scan_for_bit_index();
  move.piece_type = type;

  for (std::size_t src = 0; src < piece_src_sq.size(); ++src)
  {
    Bitboard pseudo_mv = pseudolegal_mv.rook -> get_moves(piece_src_sq[src], both_occ);

    piece_src_desc = pseudo_mv.scan_for_bit_index();
    move.sq_src    = piece_src_sq[src];

    for (std::size_t desc = 0; desc < piece_src_desc.size(); ++desc)
    {
      move.sq_dest = piece_src_desc[desc];

      if (is_move_valid(move, ally_board, enemy_board) == true)
      {
        SingleMoveGen gen_mv;

        gen_mv.src        = piece_src_sq[src];
        gen_mv.desc       = piece_src_desc[desc];
        gen_mv.piece_type = type;

        moves_v.push_back(gen_mv);
      }
    }
  }

//------------------------------------------------------------------->     Queen
  type = site == Site::BLACK ? BLACK_QUEEN
                             : WHITE_QUEEN;

  piece_src_sq = ally_board.queens.scan_for_bit_index();
  move.piece_type = type;

  for (std::size_t src = 0; src < piece_src_sq.size(); ++src)
  {
    Bitboard pseudo_mv = get_queen_moves(piece_src_sq[src], both_occ);

    piece_src_desc = pseudo_mv.scan_for_bit_index();
    move.sq_src    = piece_src_sq[src];

    for (std::size_t desc = 0; desc < piece_src_desc.size(); ++desc)
    {
      move.sq_dest = piece_src_desc[desc];

      if (is_move_valid(move, ally_board, enemy_board) == true)
      {
        SingleMoveGen gen_mv;

        gen_mv.src        = piece_src_sq[src];
        gen_mv.desc       = piece_src_desc[desc];
        gen_mv.piece_type = type;

        moves_v.push_back(gen_mv);
      }
    }
  }

//------------------------------------------------------------------->      King
  type = site == Site::BLACK ? BLACK_KING
                             : WHITE_KING;

  piece_src_sq = ally_board.king.scan_for_bit_index();

  if ( piece_src_sq.size() != 0 )
  {
    Bitboard pseudo_mv = pseudolegal_mv.king -> get_moves(piece_src_sq[0]);

    piece_src_desc  = pseudo_mv.scan_for_bit_index();
    move.piece_type = type;
    move.sq_src     = piece_src_sq[0];

    for (std::size_t desc = 0; desc < piece_src_desc.size(); ++desc)
    {
      move.sq_dest = piece_src_desc[desc];

      if (is_move_valid(move, ally_board, enemy_board) == true)
      {
        SingleMoveGen gen_mv;

        gen_mv.src        = piece_src_sq[0];
        gen_mv.desc       = piece_src_desc[desc];
        gen_mv.piece_type = type;

        moves_v.push_back(gen_mv);
      }
    }


    //Castle
    if (site == Site::BLACK || piece_src_sq[0] == BLACK_KING_INIT_SQ)
    {
      move.sq_dest = B_CASTLE_K_SIDE_SQ;
      if (is_move_valid(move, ally_board, enemy_board) == true)
      {
        SingleMoveGen gen_mv;

        gen_mv.src        = piece_src_sq[0];
        gen_mv.desc       = B_CASTLE_K_SIDE_SQ;
        gen_mv.piece_type = type;

        moves_v.push_back(gen_mv);
      }

      move.sq_dest = B_CASTLE_Q_SIDE_SQ;

      if (is_move_valid(move, ally_board, enemy_board) == true)
      {
        SingleMoveGen gen_mv;

        gen_mv.src        = piece_src_sq[0];
        gen_mv.desc       = B_CASTLE_Q_SIDE_SQ;
        gen_mv.piece_type = type;

        moves_v.push_back(gen_mv);
      }
    }
    else if (site == Site::WHITE || piece_src_sq[0] == WHITE_KING_INIT_SQ)
    {
      move.sq_dest = W_CASTLE_K_SIDE_SQ;
      if (is_move_valid(move, ally_board, enemy_board) == true)
      {
        SingleMoveGen gen_mv;

        gen_mv.src        = piece_src_sq[0];
        gen_mv.desc       = W_CASTLE_K_SIDE_SQ;
        gen_mv.piece_type = type;

        moves_v.push_back(gen_mv);
      }

      move.sq_dest = W_CASTLE_Q_SIDE_SQ;

      if (is_move_valid(move, ally_board, enemy_board) == true)
      {
        SingleMoveGen gen_mv;

        gen_mv.src        = piece_src_sq[0];
        gen_mv.desc       = W_CASTLE_Q_SIDE_SQ;
        gen_mv.piece_type = type;

        moves_v.push_back(gen_mv);
      }
    }
  }

  return moves_v;
}

bool MoveLookup:: is_king_checkmate(Site site)
{
  bool result = true;

  if (__is_king_checked(site))
  {
    ;
  }

  return result;
}

bool MoveLookup::__is_sq_attacked(std::size_t sq, Site site)
{
  ChessBoard  board = site == Site::BLACK ? *m_black_board
                                          : *m_white_board;

  Pawn *opp_pawns = site == Site::BLACK ? pseudolegal_mv.w_pawn
                                        : pseudolegal_mv.b_pawn;

  Bitboard both_occ = __get_both_occupancies(m_ally_board, m_enemy_board);

  if ( ((opp_pawns -> get_pawn_capture(sq)) & (board.pawns)) != 0 )
  {
    return true;
  }

  if ( (pseudolegal_mv.bishop -> get_moves(sq, both_occ) & (board.bishops)) != 0 )
  {
    return true;
  }

  if ( (pseudolegal_mv.knight -> get_moves(sq, both_occ) & (board.knights)) != 0 )
  {
    return true;
  }

  if ( (pseudolegal_mv.rook -> get_moves(sq, both_occ) & (board.rooks)) != 0 )
  {
    return true;
  }

  if ( ((get_queen_moves(sq, both_occ)) & (board.queens)) !=0 )
  {
    return true;
  }

  if ( (pseudolegal_mv.king -> get_moves(sq, both_occ) & (board.king)) != 0 )
  {
    return true;
  }

  return false;
}

bool MoveLookup::__is_move_correct(std::size_t   sq_src,
                                   std::size_t   sq_dest,
                                   TypesOfPieces piece_type,
                                   Site          site)
{
  Bitboard temp_moves = Bitboard(0x0);
  Bitboard src        = Bitboard(0x0);
  Bitboard desc       = Bitboard(0x0);
  Bitboard both_occ   = __get_both_occupancies(m_ally_board, m_enemy_board);

  bool result = true;

  src  = src.set_bit_at(sq_src);
  desc = desc.set_bit_at(sq_dest);
 
  switch (piece_type)
  {
    case WHITE_ROOK:
    case BLACK_ROOK:
      if ( (m_ally_board.rooks & src) == 0 )
      {
        result = false;
      }
      else
      {
        temp_moves = pseudolegal_mv.rook -> get_moves(sq_src, both_occ);

        if ( (temp_moves & desc) == 0 )
        {
          result = false;
        }
      }
    break;

    case WHITE_KNIGHT:
    case BLACK_KNIGHT:
      if ( (m_ally_board.knights & src) == 0 )
      {
        result = false;
      }
      else
      {
        temp_moves = pseudolegal_mv.knight -> get_moves(sq_src);

        if ( (temp_moves & desc) == 0 )
        {
          result = false;
        }
      }
    break;

    case WHITE_BISHOP:
    case BLACK_BISHOP:
      if ( (m_ally_board.bishops & src) == 0 )
      {
        result = false;
      }
      else
      {
        temp_moves = pseudolegal_mv.bishop -> get_moves(sq_src, both_occ);

        if ( (temp_moves & desc) == 0 )
        {
          result = false;
        }
      }
    break;

    case WHITE_QUEEN:
    case BLACK_QUEEN:
      if ( (m_ally_board.queens & src) == 0 )
      {
        result = false;
      }
      else
      {
        temp_moves = get_queen_moves(sq_src, both_occ);

        if ( (temp_moves & desc) == 0 )
        {
          result = false;
        }
      }
    break;

    case WHITE_KING:
    case BLACK_KING:
      if ( (m_ally_board.king & src) == 0 )
      {
        result = false;
      }
      else
      {
        temp_moves = pseudolegal_mv.king -> get_moves(sq_src);

        if ( (temp_moves & desc) == 0 )
        {
          result = false;
        }
      }
    break;

    case BLACK_PAWN:
      if ( (m_ally_board.pawns & src) == 0 )
      {
        result = false;
      }
      else
      {
        temp_moves = pseudolegal_mv.b_pawn -> get_moves(sq_src);

        if ( (temp_moves & desc) == 0 )
        {
          temp_moves = pseudolegal_mv.b_pawn -> get_pawn_capture(sq_src);
          if ( ((temp_moves & desc) == 0) ||
               !__is_sq_occupate_by(sq_dest, Site::WHITE))
          {
            result = false;
          }
        }
        else
        {
          if(__is_sq_occupate_by(sq_dest, Site::WHITE))
          {
            result = false;
          }
        }
      }
    break;

    case WHITE_PAWN:
      if ( (m_ally_board.pawns & src) == 0 )
      {
        result = false;
      }
      else
      {
        temp_moves = pseudolegal_mv.w_pawn -> get_moves(sq_src);

        if ( (temp_moves & desc) == 0 )
        {
          temp_moves = pseudolegal_mv.w_pawn -> get_pawn_capture(sq_src);
          if ( (temp_moves & desc) == 0 ||
               !__is_sq_occupate_by(sq_dest, Site::BLACK))
          {
            result = false;
          }
        }
        else
        {
          if(__is_sq_occupate_by(sq_dest, Site::BLACK))
          {
            result = false;
          }
        }
      }
    break;

    //> NONE
    case NONE:
      TRACE_WARNING("Received NONE type piece in movelookup.");
      result = false;
    break;

    default:
      FATAL_ERROR(ERROR_INVALID_VARIABLE,
          "Invalid variable (%i) in chess board array!",
          piece_type);
    break;
  }

  if (result == false && 
      __is_special_move(sq_src, sq_dest, piece_type, site))
  {
    result = true;
  }

  return result;
}

bool MoveLookup::__is_special_move(std::size_t   sq_src,
                                   std::size_t   sq_dest,
                                   TypesOfPieces piece_type,
                                   Site          site)
{
  Bitboard src  = Bitboard(0x0);
  Bitboard desc = Bitboard(0x0);

  bool result = false;

  src  = src.set_bit_at(sq_src);
  desc = desc.set_bit_at(sq_dest);

  switch (piece_type)
  {
    case BLACK_PAWN:
    case WHITE_PAWN:
    {
      Bitboard double_forward = site == Site::BLACK ? m_special_moves.b_double_mv_pawn
                                                    : m_special_moves.w_double_mv_pawn;

      Bitboard mask = site == Site::BLACK ? Mask["FIELD_6"]
                                          : Mask["FIELD_3"];

      Pawn *pawn = site == Site::BLACK ? pseudolegal_mv.b_pawn
                                       : pseudolegal_mv.w_pawn;

      //Double pawn move
      if ( ((src & m_ally_board.pawns) != 0) &&
          ((src & double_forward) != 0) )
      {
        Bitboard mv = pawn -> get_pawn_double(sq_src);
        Bitboard all_pieces = __get_both_occupancies(m_ally_board, m_enemy_board);

        if ( ((desc & mv) != 0) &&
             ((mv & all_pieces) == 0) )
        {
          en_pass_candidate = get_ls1b_index((mv & mask));
          double_mv_pawn_sq = sq_dest;
          result = true;
        }
      }
      //En Passaant
      else
      {
        if ( ((src & m_ally_board.pawns) != 0 &&
              m_special_moves.en_passant != std::bitset<64>(0)) && 
             (std::bitset<64>().set(sq_dest) == m_special_moves.en_passant))
        {
          Bitboard mv = pawn -> get_pawn_capture(sq_src);

          if ( (mv & m_special_moves.en_passant) != 0 )
          {
            result = true;
          }
        }
      }
    }
    break;

    case BLACK_KING:
    {
      //Castling
      Bitboard king_side  = Bitboard(0x4000000000000000);
      Bitboard queen_side = Bitboard(0x0400000000000000);

      if ( ((desc & king_side) != 0) &&
           ((m_special_moves.castle & std::bitset<4>(BLACK_KING_SIDE)).any()) )
      {
        Bitboard sqs_to_be_free = Bitboard(0x6000000000000000);
        std::size_t G8 = 62;
        std::size_t F8 = 61;

        if ( (sqs_to_be_free & __get_both_occupancies(m_ally_board, m_enemy_board)) == 0 &&
              !__is_sq_attacked(G8, Site::WHITE) &&
              !__is_sq_attacked(F8, Site::WHITE))
        {
          result = true;
        }
      }
      else if ( ((desc & queen_side) != 0) &&
                ((m_special_moves.castle & std::bitset<4>(BLACK_QUEEN_SIDE))).any() )
      {
        Bitboard sqs_to_be_free = Bitboard(0xE00000000000000);
        std::size_t D8 = 59;
        std::size_t C8 = 58;

        if ( (sqs_to_be_free & __get_both_occupancies(m_ally_board, m_enemy_board)) == 0 &&
              !__is_sq_attacked(D8, Site::WHITE) &&
              !__is_sq_attacked(C8, Site::WHITE))
        {
          result = true;
        }
      }
    }
    break;

    case WHITE_KING:
    {
      //Castling
      Bitboard king_side  = Bitboard(0x000000000000040);
      Bitboard queen_side = Bitboard(0x000000000000004);

      if ( ((desc & king_side) != 0) &&
           ((m_special_moves.castle & std::bitset<4>(WHITE_KING_SIDE))).any() )
      {
        Bitboard sqs_to_be_free = Bitboard(0x000000000000060);
        std::size_t G1 = 6;
        std::size_t F1 = 5;

        if ( (sqs_to_be_free & __get_both_occupancies(m_ally_board, m_enemy_board)) == 0 &&
              !__is_sq_attacked(G1, Site::BLACK) &&
              !__is_sq_attacked(F1, Site::BLACK))
        {
          result = true;
        }
      }
      else if ( ((desc & queen_side) != 0) &&
                ((m_special_moves.castle & std::bitset<4>(WHITE_QUEEN_SIDE))).any() )
      {
        Bitboard sqs_to_be_free = Bitboard(0x0E);
        std::size_t D1 = 3;
        std::size_t C1 = 2;

        if ( (sqs_to_be_free & __get_both_occupancies(m_ally_board, m_enemy_board)) == 0 &&
              !__is_sq_attacked(D1, Site::BLACK) &&
              !__is_sq_attacked(C1, Site::BLACK))
        {
          result = true;
        }
      }
    }
    break;

    default:
    break;
  }

  return result;
}

bool MoveLookup::__is_sq_occupate_by(std::size_t sq,
                                     Site        site)
{
  Bitboard b = Bitboard(0x0);
  Bitboard opponent_occ = site == Site::BLACK ? __get_occupancies(*m_black_board)
                                              : __get_occupancies(*m_white_board);
  bool result = true;

  b = b.set_bit_at(sq);

  if ( (b & opponent_occ) == 0 )
  {
    result = false;
  }

  return result;
}

bool MoveLookup::__is_king_checked(Site site)
{
    bool result = false;
    Bitboard king = site == Site::BLACK ? m_black_board -> king
                                        : m_white_board -> king;
    Site opponent = site == Site::BLACK ? Site::WHITE
                                        : Site::BLACK;

    int sq = get_ls1b_index(king);

    if (__is_sq_attacked(sq, opponent))
    {
      result = true;
    }

  return result;
}

void MoveLookup::__remove_piece_at_sq(std::size_t sq,
                                      ChessBoard  *board)
{
  Bitboard square = Bitboard().set_bit_at(sq);

  for (Bitboard* origin = get_begin(board);
       origin < get_end(board);
       ++origin)
  {
    if ((*origin & square) != 0)
    {
      *origin &= ~square;
      break;
    }
  }

  return;
}