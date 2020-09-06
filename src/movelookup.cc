#include "../inc/movelookup.hh"

#define COORD_TO_BIT_POS(x , y) (x * 8 + y)

static Pieces *piece_type;

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

  b |= m_pseudolegal_mv.bishop -> get_moves(sq, occ);
  b |= m_pseudolegal_mv.rook   -> get_moves(sq, occ);

  return b;
}

void MoveLookup::init()
{
  TRACE_INFO("Initialization bitboards...");

  m_black_pieces.king    = Bitboard(0x0800000000000000ULL);
  m_black_pieces.queens  = Bitboard(0x1000000000000000ULL);
  m_black_pieces.rooks   = Bitboard(0x8100000000000000ULL);
  m_black_pieces.knights = Bitboard(0x4200000000000000ULL);
  m_black_pieces.bishops = Bitboard(0x2400000000000000ULL);
  m_black_pieces.pawns   = Bitboard(0x00ff000000000000ULL);

  m_white_pieces.king    = Bitboard(0x0000000000000008ULL);
  m_white_pieces.queens  = Bitboard(0x0000000000000010ULL);
  m_white_pieces.rooks   = Bitboard(0x0000000000000081ULL);
  m_white_pieces.knights = Bitboard(0x0000000000000042ULL);
  m_white_pieces.bishops = Bitboard(0x0000000000000024ULL);
  m_white_pieces.pawns   = Bitboard(0x000000000000ff00ULL);

  TRACE_INFO("Initialization bitboards done");

  TRACE_INFO("Generation of pieces moves...");

  m_pseudolegal_mv.king   = new King   (Opponent::BOTH);
  m_pseudolegal_mv.rook   = new Rook   (Opponent::BOTH);
  m_pseudolegal_mv.knight = new Knight (Opponent::BOTH);
  m_pseudolegal_mv.bishop = new Bishop (Opponent::BOTH);
  m_pseudolegal_mv.b_pawn = new Pawn   (Opponent::BLACK);
  m_pseudolegal_mv.w_pawn = new Pawn   (Opponent::WHITE);

  TRACE_INFO("Generation of moves sucessful");
  TRACE_INFO("Initialization new game sucessful");

  return;
}

 bool MoveLookup::is_move_valid(uint8_t x_desc,
                                uint8_t y_desc,
                                TypesOfPieces piece_type,
                                SiteMove      site_move)
{
  bool result = true;

  result = __is_move_correct(x_desc, y_desc, piece_type, site_move);
  std::cout << "DUPA: " << result << std::endl;

  return result;
}

bool MoveLookup::__is_sq_attacked(std::size_t sq, Opponent opp)
{
  ChessBoard *opp_board = opp == Opponent::BLACK ? &m_black_pieces
                                                 : &m_white_pieces;
  Pawn       *opp_pawns = opp == Opponent::BLACK ? m_pseudolegal_mv.w_pawn
                                                 : m_pseudolegal_mv.b_pawn;

  Bitboard both_occ = __get_both_occupancies();

  if ( ((opp_pawns -> get_pawn_capture(sq)) & (opp_board -> pawns)) != 0 )
  {
    return true;
  }

  if ( (m_pseudolegal_mv.bishop -> get_moves(sq, both_occ) & (opp_board -> bishops)) != 0 )
  {
    return true;
  }

  if ( (m_pseudolegal_mv.knight -> get_moves(sq, both_occ) & (opp_board -> knights)) != 0 )
  {
    return true;
  }

  if ( (m_pseudolegal_mv.rook -> get_moves(sq, both_occ) & (opp_board -> rooks)) != 0 )
  {
    return true;
  }

  if ( ((get_queen_moves(sq, both_occ)) & (opp_board -> queens)) !=0 )
  {
    return true;
  }

  if ( (m_pseudolegal_mv.king -> get_moves(sq, both_occ) & (opp_board -> king)) != 0 )
  {
    return true;
  }

  return false;
}

bool MoveLookup::__is_move_correct(uint8_t       x_desc,
                                   uint8_t       y_desc,
                                   TypesOfPieces piece_type,
                                   SiteMove      site_move)
{
  Bitboard b = Bitboard(0x0);
  Bitboard both_occ = __get_both_occupancies();

  ChessBoard *opp_board = site_move == SiteMove::BLACK_MOVE ? &m_black_pieces
                                                            : &m_white_pieces;
  bool result = true;

  switch (piece_type)
  {
    case WHITE_ROOK:
    case BLACK_ROOK:
      b = m_pseudolegal_mv.rook ->
                     get_moves(COORD_TO_BIT_POS(y_desc, x_desc), both_occ);
      b.print();
      if ( ((opp_board -> rooks) & b) == 0)
      {
        result = false;
      }
    break;

    case WHITE_KNIGHT:
    case BLACK_KNIGHT:
      b = m_pseudolegal_mv.knight ->
                     get_moves(COORD_TO_BIT_POS(y_desc, x_desc));
      b.print();
      if ( ((opp_board -> knights) & b) == 0)
      {
        result = false;
      }
    break;

    case WHITE_BISHOP:
    case BLACK_BISHOP:
      b = m_pseudolegal_mv.bishop ->
                     get_moves(COORD_TO_BIT_POS(y_desc, x_desc), both_occ);
      b.print();
      if ( ((opp_board -> bishops) & b) == 0)
      {
        result = false;
      }
    break;

    case WHITE_QUEEN:
    case BLACK_QUEEN:
      b = get_queen_moves(COORD_TO_BIT_POS(y_desc, x_desc), both_occ);
      if ( ((opp_board -> queens) & b) == 0)
      {
        result = false;
      }
    break;

    case WHITE_KING:
    case BLACK_KING:
      b = m_pseudolegal_mv.king ->
                     get_moves(COORD_TO_BIT_POS(y_desc, x_desc));
      if ( ((opp_board -> king) & b) == 0)
      {
        result = false;
      }
    break;

    case BLACK_PAWN:
      b = m_pseudolegal_mv.b_pawn ->
                     get_moves(COORD_TO_BIT_POS(y_desc, x_desc));
      if ( ((opp_board -> pawns) & b) == 0)
      {
        // If pawn move is incorrect check if it can capture
        b = m_pseudolegal_mv.w_pawn ->
             get_pawn_capture(COORD_TO_BIT_POS(y_desc, x_desc));

        if ( ((opp_board -> pawns) & b) == 0)
        {
          result = false;
        }
      }
    break;

    case WHITE_PAWN:
      b = m_pseudolegal_mv.w_pawn ->
                     get_moves(COORD_TO_BIT_POS(y_desc, x_desc));
      if ( ((opp_board -> pawns) & b) == 0)
      {
        // If pawn move is incorrect check if it can capture
        b = m_pseudolegal_mv.b_pawn ->
             get_pawn_capture(COORD_TO_BIT_POS(y_desc, x_desc));

        if ( ((opp_board -> pawns) & b) == 0)
        {
          result = false;
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

  return result;
}

 /*Bitboard MoveLookup::__gen_occupancy_board(ChessBoard board)
{
  board.king.scan_for_bit_index();
}*/


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

Bitboard MoveLookup::__get_white_occupancies()
{
  Bitboard b = Bitboard(0x0);

  b |= m_white_pieces.king;
  b |= m_white_pieces.queens;
  b |= m_white_pieces.rooks;
  b |= m_white_pieces.knights;
  b |= m_white_pieces.bishops;
  b |= m_white_pieces.pawns;

  return b;
}

Bitboard MoveLookup::__get_black_occupancies()
{
  Bitboard b = Bitboard(0x0);

  b |= m_black_pieces.king;
  b |= m_black_pieces.queens;
  b |= m_black_pieces.rooks;
  b |= m_black_pieces.knights;
  b |= m_black_pieces.bishops;
  b |= m_black_pieces.pawns;

  return b;
}

Bitboard MoveLookup::__get_both_occupancies()
{
  Bitboard b = Bitboard(0x0);

  b |= m_white_pieces.king;
  b |= m_white_pieces.queens;
  b |= m_white_pieces.rooks;
  b |= m_white_pieces.knights;
  b |= m_white_pieces.bishops;
  b |= m_white_pieces.pawns;

  b |= m_black_pieces.king;
  b |= m_black_pieces.queens;
  b |= m_black_pieces.rooks;
  b |= m_black_pieces.knights;
  b |= m_black_pieces.bishops;
  b |= m_black_pieces.pawns;

  return b;
}