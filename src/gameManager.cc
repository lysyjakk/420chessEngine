#include "../inc/gameManager.hh"

#define COORD_TO_BIT_POS(x , y) (x * 8 + y)

void GameManager::start_new_game()
{
  TRACE_INFO("Initialization new game...");
  TRACE_INFO("Initialization bitboards...");

  m_black_board.king    = Bitboard(0x1000000000000000ULL);
  m_black_board.queens  = Bitboard(0x0800000000000000ULL);
  m_black_board.rooks   = Bitboard(0x8100000000000000ULL);
  m_black_board.knights = Bitboard(0x4200000000000000ULL);
  m_black_board.bishops = Bitboard(0x2400000000000000ULL);
  m_black_board.pawns   = Bitboard(0x00ff000000000000ULL);

  m_white_board.king    = Bitboard(0x0000000000000010ULL);
  m_white_board.queens  = Bitboard(0x0000000000000008ULL);
  m_white_board.rooks   = Bitboard(0x0000000000000081ULL);
  m_white_board.knights = Bitboard(0x0000000000000042ULL);
  m_white_board.bishops = Bitboard(0x0000000000000024ULL);
  m_white_board.pawns   = Bitboard(0x000000000000ff00ULL);

  TRACE_INFO("Initialization bitboards done");

  TRACE_INFO("Setting necessary moves varables...");

  m_bot          = NegaMax();
  m_move_checker = MoveLookup();

  m_move_checker.init();

  m_player_turn = Site::WHITE;

  m_special_moves.en_passant = ENPASS_NO_SQ;
  m_special_moves.castle = BLACK_KING_SIDE | BLACK_QUEEN_SIDE |
                           WHITE_KING_SIDE | WHITE_QUEEN_SIDE;

  m_special_moves.w_double_mv_pawn = Bitboard(Mask["FIELD_2"]);
  m_special_moves.b_double_mv_pawn = Bitboard(Mask["FIELD_7"]);

  //Clear GUI board
  for (int itr = 0; itr < MAX_BOARD_SQ; ++itr)
  {
    m_pieces_pos[itr] = NONE;
  }

  __set_up_GUI_board(m_white_board, Site::WHITE);
  __set_up_GUI_board(m_black_board, Site::BLACK);

  TRACE_INFO("Setting necessary moves varables done");
  TRACE_INFO("Initialization new game done");

  return;
}

void GameManager::move_piece(uint8_t x_src,
                             uint8_t y_src,
                             uint8_t x_dest,
                             uint8_t y_dest)
{
  std::size_t sq_src = COORD_TO_BIT_POS(y_src, x_src);
  std::size_t sq_dest = COORD_TO_BIT_POS(y_dest, x_dest);

  if (m_player_turn == Site::WHITE)
  {
    MoveToMake move;

    move.sq_src     = sq_src;
    move.sq_dest    = sq_dest;
    move.piece_type = m_pieces_pos[sq_src];
    move.site       = Site::WHITE;
    move.special_mv = m_special_moves;

    if ( m_move_checker.is_move_valid(move, m_white_board, m_black_board) )
    {
      m_move_checker.make_move(&move, &m_white_board, &m_black_board);

      m_special_moves = move.special_mv;
      change_player_turn(m_player_turn);

      //Clear GUI board
      for (int itr = 0; itr < MAX_BOARD_SQ; ++itr)
      {
        m_pieces_pos[itr] = NONE;
      }

      __set_up_GUI_board(m_white_board, Site::WHITE);
      __set_up_GUI_board(m_black_board, Site::BLACK);
    }
  }
  else
  {
    MoveToMake move;
    BestMove best_move;

    best_move = m_bot.get_best_move(m_white_board,
                        m_black_board,
                        m_player_turn,
                        m_special_moves);

      move.sq_src     = std::get<0>(best_move);
      move.sq_dest    = std::get<1>(best_move);
      move.piece_type = m_pieces_pos[std::get<0>(best_move)];
      move.site       = Site::BLACK;
      move.special_mv = m_special_moves;

      m_move_checker.make_move(&move, &m_black_board, &m_white_board);

      m_special_moves = move.special_mv;
      change_player_turn(m_player_turn);

      //Clear GUI board
      for (int itr = 0; itr < MAX_BOARD_SQ; ++itr)
      {
        m_pieces_pos[itr] = NONE;
      }

      __set_up_GUI_board(m_white_board, Site::WHITE);
      __set_up_GUI_board(m_black_board, Site::BLACK);
  }

  return;
}

BitBoardToGUI GameManager::get_board() const
{
  return m_pieces_pos;
}

void GameManager::__set_up_GUI_board(ChessBoard board, Site site)
{
    //Set up all white pieces
    int index = 1;

    for (Bitboard* element = get_begin(&board);
         element < get_end(&board);
         ++element)
    {
      switch (index)
      {

      case 1: //Pawn
      {
        std::vector< std::size_t > sq_vector = element -> scan_for_bit_index();

        for (auto sq : sq_vector)
        {
          m_pieces_pos[sq] = site == Site::BLACK ? BLACK_PAWN
                                                 : WHITE_PAWN;
        }
      }
      break;

      case 2: //Rook
      {
        std::vector< std::size_t > sq_vector = element -> scan_for_bit_index();

        for (auto sq : sq_vector)
        {
          m_pieces_pos[sq] = site == Site::BLACK ? BLACK_ROOK
                                                 : WHITE_ROOK;
        }
      }
      break;

      case 3: //Knight
      {
        std::vector< std::size_t > sq_vector = element -> scan_for_bit_index();

        for (auto sq : sq_vector)
        {
          m_pieces_pos[sq] = site == Site::BLACK ? BLACK_KNIGHT
                                                 : WHITE_KNIGHT;
        }
      }
      break;

      case 4: //Bishop
      {
        std::vector< std::size_t > sq_vector = element -> scan_for_bit_index();

        for (auto sq : sq_vector)
        {
          m_pieces_pos[sq] = site == Site::BLACK ? BLACK_BISHOP
                                                 : WHITE_BISHOP;
        }
      }
      break;

      case 5: //Queen
      {
        std::vector< std::size_t > sq_vector = element -> scan_for_bit_index();

        for (auto sq : sq_vector)
        {
          m_pieces_pos[sq] = site == Site::BLACK ? BLACK_QUEEN
                                                 : WHITE_QUEEN;
        }
      }
      break;

      case 6: //King
      {
        std::vector< std::size_t > sq_vector = element -> scan_for_bit_index();

        for (auto sq : sq_vector)
        {
          m_pieces_pos[sq] = site == Site::BLACK ? BLACK_KING
                                                 : WHITE_KING;
        }
      }
      break;

      default:
        break;
      }

      ++index;
    }
}