#include "../inc/gameManager.hh"

#define QUEEN_WEIGHT      9
#define ROOK_WEIGHT       5
#define KNIGHT_WEIGHT     3
#define BISHOP_WEIGHT     3
#define PAWN_WEIGHT       1
#define KING_WEIGHT      -1

void GameManager::start_new_game()
{
  int8_t start_board_pos [MAX_BOARD_COLUMNS][MAX_BOARD_ROWS] = 
    {
      { -1, -2, -3, -4, -5, -3, -2, -1 },
      { -6, -6, -6, -6, -6, -6, -6, -6 },
      {  0,  0,  0,  0,  0,  0,  0,  0 },
      {  0,  0,  0,  0,  0,  0,  0,  0 },
      {  0,  0,  0,  0,  0,  0,  0,  0 },
      {  0,  0,  0,  0,  0,  0,  0,  0 },
      {  6,  6,  6,  6,  6,  6,  6,  6 },
      {  1,  2,  3,  4,  5,  3,  2,  1 }
    };

  Pieces *new_piece;

  for (int row = 0; row < MAX_BOARD_ROWS; ++row)
  {
    for (int column = 0; column < MAX_BOARD_COLUMNS; ++column)
    {
      switch (start_board_pos[row][column])
      {
      //> BLACK PIECES
      case -1:
        new_piece = new Rook(row, column, ROOK_WEIGHT, Opponent::BLACK);
        m_chess_board[row][column] = std::make_pair(new_piece, BLACK_ROOK);
      break;

      case -2:
        new_piece = new Knight(row, column, KNIGHT_WEIGHT, Opponent::BLACK);
        m_chess_board[row][column] = std::make_pair(new_piece, BLACK_KNIGHT);
      break;

      case -3:
        new_piece = new Bishop(row, column, BISHOP_WEIGHT, Opponent::BLACK);
        m_chess_board[row][column] = std::make_pair(new_piece, BLACK_BISHOP);
      break;

      case -4:
        new_piece = new Queen(row, column, QUEEN_WEIGHT, Opponent::BLACK);
        m_chess_board[row][column] = std::make_pair(new_piece, BLACK_QUEEN);
      break;

      case -5:
        new_piece = new King(row, column, KING_WEIGHT, Opponent::BLACK);
        m_chess_board[row][column] = std::make_pair(new_piece, BLACK_KING);
      break;

      case -6:
        new_piece = new King(row, column, PAWN_WEIGHT, Opponent::BLACK);
        m_chess_board[row][column] = std::make_pair(new_piece, BLACK_PAWN);
      break;

      //> WHITE PIECES

      case 1:
        new_piece = new Rook(row, column, ROOK_WEIGHT, Opponent::WHITE);
        m_chess_board[row][column] = std::make_pair(new_piece, WHITE_ROOK);
      break;

      case 2:
        new_piece = new Knight(row, column, KNIGHT_WEIGHT, Opponent::WHITE);
        m_chess_board[row][column] = std::make_pair(new_piece, WHITE_KNIGHT);
      break;

      case 3:
        new_piece = new Bishop(row, column, BISHOP_WEIGHT, Opponent::WHITE);
        m_chess_board[row][column] = std::make_pair(new_piece, WHITE_BISHOP);
      break;

      case 4:
        new_piece = new Queen(row, column, QUEEN_WEIGHT, Opponent::WHITE);
        m_chess_board[row][column] = std::make_pair(new_piece, WHITE_QUEEN);
      break;

      case 5:
        new_piece = new King(row, column, KING_WEIGHT, Opponent::WHITE);
        m_chess_board[row][column] = std::make_pair(new_piece, WHITE_KING);
      break;

      case 6:
        new_piece = new King(row, column, PAWN_WEIGHT, Opponent::WHITE);
        m_chess_board[row][column] = std::make_pair(new_piece, WHITE_PAWN);
      break;
      //> NONE

      case 0:
        m_chess_board[row][column] = std::make_pair(nullptr, NONE);
      break;

      default:
        break;
      }
    }
  }

  return;
}

ChessBoard GameManager::get_chess_board()
{
  return m_chess_board;
}