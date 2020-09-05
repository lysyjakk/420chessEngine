#include "../inc/gameManager.hh"

#define QUEEN_WEIGHT      9
#define ROOK_WEIGHT       5
#define KNIGHT_WEIGHT     3
#define BISHOP_WEIGHT     3
#define PAWN_WEIGHT       1
#define KING_WEIGHT      -1

#define COORD_TO_BIT_POS(x , y) (x * 8 + y)

void GameManager::start_new_game()
{
  TRACE_INFO("Initialization new game...");
  int init_board [MAX_BOARD_COLUMNS][MAX_BOARD_ROWS] =
    {
      // A   B   C   D   E   F   G   H
      {  2,  3,  4,  6,  5,  4,  3,  2 },// 1
      {  1,  1,  1,  1,  1,  1,  1,  1 },// 2
      {  0,  0,  0,  0,  0,  0,  0,  0 },// 3
      {  0,  0,  0,  0,  0,  0,  0,  0 },// 4
      {  0,  0,  0,  0,  0,  0,  0,  0 },// 5
      {  0,  0,  0,  0,  0,  0,  0,  0 },// 6
      {  7,  7,  7,  7,  7,  7,  7,  7 },// 7
      {  8,  9, 10, 12, 11, 10,  9,  8 } // 8
    };

  for (int row = 0; row < MAX_BOARD_ROWS; ++row)
    {
      for (int col = 0; col < MAX_BOARD_COLUMNS; ++col)
      {
        Bitboard b;

        switch (init_board[row][col])
        {
        //> BLACK PIECES
        case BLACK_ROOK:
          m_pieces_pos[(row * 8 + col)] = BLACK_ROOK;
        break;

        case BLACK_KNIGHT:
          m_pieces_pos[(row * 8 + col)] = BLACK_KNIGHT;
        break;

        case BLACK_BISHOP:
          m_pieces_pos[(row * 8 + col)] = BLACK_BISHOP;
        break;

        case BLACK_QUEEN:
          m_pieces_pos[(row * 8 + col)] = BLACK_QUEEN;
        break;

        case BLACK_KING:
          m_pieces_pos[(row * 8 + col)] = BLACK_KING;
        break;

        case BLACK_PAWN:
          m_pieces_pos[(row * 8 + col)] = BLACK_PAWN;
        break;

        //> WHITE PIECES

        case WHITE_ROOK:
          m_pieces_pos[(row * 8 + col)] = WHITE_ROOK;
        break;

        case WHITE_KNIGHT:
          m_pieces_pos[(row * 8 + col)] = WHITE_KNIGHT;
        break;

        case WHITE_BISHOP:
          m_pieces_pos[(row * 8 + col)] = WHITE_BISHOP;
        break;

        case WHITE_QUEEN:
          m_pieces_pos[(row * 8 + col)] = WHITE_QUEEN;
        break;

        case WHITE_KING:
          m_pieces_pos[(row * 8 + col)] = WHITE_KING;
        break;

        case WHITE_PAWN:
          m_pieces_pos[(row * 8 + col)] = WHITE_PAWN;
        break;
        //> NONE

        case NONE:
        break;

        default:
          break;
      }
    }
  }
  TRACE_INFO("Generation of pieces moves...");

  m_pseudolegal_mv.king   = new King   (KING_WEIGHT, Opponent::BOTH);
  m_pseudolegal_mv.queen  = new Queen  (QUEEN_WEIGHT, Opponent::BOTH);
  m_pseudolegal_mv.rook   = new Rook   (ROOK_WEIGHT, Opponent::BOTH);
  m_pseudolegal_mv.knight = new Knight (KNIGHT_WEIGHT, Opponent::BOTH);
  m_pseudolegal_mv.bishop = new Bishop (BISHOP_WEIGHT, Opponent::BOTH);
  m_pseudolegal_mv.b_pawn = new Pawn   (PAWN_WEIGHT, Opponent::BLACK);
  m_pseudolegal_mv.w_pawn = new Pawn   (PAWN_WEIGHT, Opponent::WHITE);

  TRACE_INFO("Generation of moves sucessful");
  TRACE_INFO("Initialization new game sucessful");

  Bitboard b = Bitboard(0x8000004048000000);
  std::cout << std::endl;
  b.print();
  std::cout << std::endl;
  m_pseudolegal_mv.queen -> get_moves(28, b).print();
  std::cout << std::endl;

  return;
}

void GameManager::move_piece(uint8_t x_src,
                             uint8_t y_src,
                             uint8_t x_dest,
                             uint8_t y_dest)
{
  ChessBoard *target_board;

  target_board = __gen_target_board(m_pieces_pos[63 - (y_src * 8 + x_src)],
                                      x_src, y_src, x_dest, y_dest);

  target_board -> black_rooks.print();
  std::cout << std::endl << target_board -> black_rooks.board << std::endl;
  if (target_board != nullptr)
  {
    //__check_valid_of_move(*target_board,NU);
  }

  return;
}

BitBoardToGUI GameManager::get_board() const
{
  return m_pieces_pos;
}

bool GameManager::__check_valid_of_move(ChessBoard target_board,
                                        TypesOfPieces piece) const
{
  Bitboard *moved_piece;

  {
    // Find bitboard from struct ChessBoard where
    // piece was moved.

    Bitboard* target = get_begin(&target_board);

    for (Bitboard* origin = get_begin(&m_board);
         origin < get_end(&m_board);
         ++origin)
    {
      if (*origin != *target)
      {
        moved_piece = target;
        break;
      }

      try
      {
        ++target;
      }
      catch(const std::exception& e)
      {
        TRACE_ERROR("Increase index failed! More: %s", e.what());
        break;
      }

    }
  }

  __check_piece_move(*moved_piece);

  return true;
}

bool GameManager::__check_piece_move(Bitboard target_piece) const
{
  std::vector<std::size_t> bit_index;
  bit_index = target_piece.scan_for_bit_index();

  for (auto it : bit_index)
  {
    std::cout << it << std::endl;
  }

  return true;
}

ChessBoard* GameManager::__gen_target_board(TypesOfPieces piece,
                                            uint8_t       x_src,
                                            uint8_t       y_src,
                                            uint8_t       x_dest,
                                            uint8_t       y_dest) const
{
  ChessBoard* target_board = new ChessBoard;
  *target_board = m_board;
  Bitboard b;
  switch(piece)
  {
    //> BLACK PIECES

    case BLACK_ROOK:
      b = m_board.black_rooks.set_bit_at(
                  std::size_t( COORD_TO_BIT_POS(y_dest, x_dest)));
      b = b.set_bit_at(std::size_t( COORD_TO_BIT_POS(y_src, x_src)), false);

      target_board -> black_rooks = b;
    break;

    case BLACK_KNIGHT:
      b = m_board.black_knights.set_bit_at(
                  std::size_t( COORD_TO_BIT_POS(y_dest, x_dest)));
      b = b.set_bit_at(std::size_t( COORD_TO_BIT_POS(y_src, x_src)), false)  ;

      target_board -> black_knights = b;
    break;

    case BLACK_BISHOP:
      b = m_board.black_bishops.set_bit_at(
                  std::size_t( COORD_TO_BIT_POS(y_dest, x_dest)));
      b = b.set_bit_at(std::size_t( COORD_TO_BIT_POS(y_src, x_src)), false);

      target_board -> black_bishops = b;
    break;

    case BLACK_QUEEN:
      b = m_board.black_queens.set_bit_at(
                  std::size_t( COORD_TO_BIT_POS(y_dest, x_dest)));
      b = b.set_bit_at(std::size_t( COORD_TO_BIT_POS(y_src, x_src)), false);

      target_board -> black_queens = b;
    break;

    case BLACK_KING:
      b = m_board.black_king.set_bit_at(
                  std::size_t( COORD_TO_BIT_POS(y_dest, x_dest)));
      b = b.set_bit_at(std::size_t( COORD_TO_BIT_POS(y_src, x_src)), false);

      target_board -> black_king = b;
    break;

    case BLACK_PAWN:
      b = m_board.black_pawns.set_bit_at(
                  std::size_t( COORD_TO_BIT_POS(y_dest, x_dest)));
      b = b.set_bit_at(std::size_t( COORD_TO_BIT_POS(y_src, x_src)), false);

      target_board -> black_pawns = b;
    break;

    //> WHITE PIECES
    case WHITE_ROOK:
      b = m_board.white_rooks.set_bit_at(
                  std::size_t( COORD_TO_BIT_POS(y_dest, x_dest)));
      b = b.set_bit_at(std::size_t( COORD_TO_BIT_POS(y_src, x_src)), false);

      target_board -> white_rooks = b;
    break;

    case WHITE_KNIGHT:
      b = m_board.white_knights.set_bit_at(
                  std::size_t( COORD_TO_BIT_POS(y_dest, x_dest)));
      b = b.set_bit_at(std::size_t( COORD_TO_BIT_POS(y_src, x_src)), false);

      target_board -> white_knights = b;
    break;

    case WHITE_BISHOP:
      b = m_board.white_bishops.set_bit_at(
                  std::size_t( COORD_TO_BIT_POS(y_dest, x_dest)));
      b = b.set_bit_at(std::size_t( COORD_TO_BIT_POS(y_src, x_src)), false);

      target_board -> white_bishops = b;
    break;

    case WHITE_QUEEN:
      b = m_board.white_queens.set_bit_at(
                  std::size_t( COORD_TO_BIT_POS(y_dest, x_dest)));
      b = b.set_bit_at(std::size_t( COORD_TO_BIT_POS(y_src, x_src)), false);

      target_board -> white_queens = b;
    break;

    case WHITE_KING:
      b = m_board.white_king.set_bit_at(
                  std::size_t( COORD_TO_BIT_POS(y_dest, x_dest)));
      b = b.set_bit_at(std::size_t( COORD_TO_BIT_POS(y_src, x_src)), false);

      target_board -> white_king = b;
    break;

    case WHITE_PAWN:
      b = m_board.white_pawns.set_bit_at(
                  std::size_t( COORD_TO_BIT_POS(y_dest, x_dest)));
      b = b.set_bit_at(std::size_t( COORD_TO_BIT_POS(y_src, x_src)), false);

      target_board -> white_pawns = b;
    break;

    //> NONE
    case NONE:
      // If its no piece there just ignore request.
      target_board = nullptr;
    break;

    default:
      FATAL_ERROR(ERROR_INVALID_VARIABLE,
          "Invalid variable (%i) in chess board array!",
          m_pieces_pos[(y_src * 8 + x_src)]);
    break;
  }

  return target_board;
}