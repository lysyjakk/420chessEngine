#include "../inc/gameManager.hh"

#define QUEEN_WEIGHT      9
#define ROOK_WEIGHT       5
#define KNIGHT_WEIGHT     3
#define BISHOP_WEIGHT     3
#define PAWN_WEIGHT       1
#define KING_WEIGHT      -1

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

  TRACE_INFO("Initialization move generator...");

  m_move_checker = new MoveLookup();
  m_move_checker -> init();

  TRACE_INFO("Move generator was created.");

  TRACE_INFO("Initialization new game done");

  return;
}

void GameManager::move_piece(uint8_t x_src,
                             uint8_t y_src,
                             uint8_t x_dest,
                             uint8_t y_dest)
{
  m_move_checker -> is_move_valid(x_dest, y_dest,
                                  m_pieces_pos[(y_src * 8 + x_src)],
                                  SiteMove::BLACK_MOVE);
  return;
}

BitBoardToGUI GameManager::get_board() const
{
  return m_pieces_pos;
}