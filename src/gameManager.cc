#include "../inc/gameManager.hh"

#define QUEEN_WEIGHT      9
#define ROOK_WEIGHT       5
#define KNIGHT_WEIGHT     3
#define BISHOP_WEIGHT     3
#define PAWN_WEIGHT       1
#define KING_WEIGHT      -1

void GameManager::start_new_game2()
{
  int init_board [MAX_BOARD_COLUMNS][MAX_BOARD_ROWS] =
    {
      {  2,  3,  4,  6,  5,  4,  3,  2 },
      {  1,  1,  1,  1,  1,  1,  1,  1 },
      {  0,  0,  0,  0,  0,  0,  0,  0 },
      {  0,  0,  0,  0,  0,  0,  0,  0 },
      {  0,  0,  0,  0,  0,  0,  0,  0 },
      {  0,  0,  0,  0,  0,  0,  0,  0 },
      {  7,  7,  7,  7,  7,  7,  7,  7 },
      {  8,  9, 10, 12, 11, 10,  9,  8 }
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

          b = Bitboard().board.set( std::size_t(row * 8 + col) );
          m_board.black_rooks = m_board.black_rooks | b;
        break;

        case BLACK_KNIGHT:
          b = Bitboard().board.set( std::size_t(row * 8 + col) );
          m_board.black_knights |= b;
        break;

        case BLACK_BISHOP:
          b = Bitboard().board.set( std::size_t(row * 8 + col) ); 
          m_board.black_bishops |= b;
        break;

        case BLACK_QUEEN:
          b = Bitboard().board.set( std::size_t(row * 8 + col) );
          m_board.black_queens |= b;
        break;

        case BLACK_KING:
          b = Bitboard().board.set( std::size_t(row * 8 + col) );
          m_board.black_king |= b;
        break;

        case BLACK_PAWN:
          b = Bitboard().board.set( std::size_t(row * 8 + col) );
          m_board.black_pawns |= b;
        break;

        //> WHITE PIECES

        case WHITE_ROOK:
          b = Bitboard().board.set( std::size_t(row * 8 + col) );
          m_board.white_rooks |= b;
        break;

        case WHITE_KNIGHT:
          b = Bitboard().board.set( std::size_t(row * 8 + col) );
          m_board.white_knights |= b;
        break;

        case WHITE_BISHOP:
          b = Bitboard().board.set( std::size_t(row * 8 + col) );
          m_board.white_bishops |= b;
        break;

        case WHITE_QUEEN:
          b = Bitboard().board.set( std::size_t(row * 8 + col) );
          m_board.white_queens |= b;
        break;

        case WHITE_KING:
          b = Bitboard().board.set( std::size_t(row * 8 + col) );
          m_board.white_king |= b;
        break;

        case WHITE_PAWN:
          b = Bitboard().board.set( std::size_t(row * 8 + col) );
          m_board.white_pawns |= b;
        break;
        //> NONE

        case NONE:
        break;

        default:
          break;
      }
    }
  }

  return;
}

void GameManager::start_new_game()
{
  this -> start_new_game2();

  int start_board_pos [MAX_BOARD_COLUMNS][MAX_BOARD_ROWS] = 
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

  for (int y = 0; y < MAX_BOARD_ROWS; ++y)
  {
    for (int x = 0; x < MAX_BOARD_COLUMNS; ++x)
    {
      switch (start_board_pos[y][x])
      {
      //> BLACK PIECES
      case -1:
        new_piece = new Rook(x, y, ROOK_WEIGHT, Opponent::BLACK);
        m_chess_board[y][x] = std::make_pair(new_piece, BLACK_ROOK);
      break;

      case -2:
        new_piece = new Knight(x, y, KNIGHT_WEIGHT, Opponent::BLACK);
        m_chess_board[y][x] = std::make_pair(new_piece, BLACK_KNIGHT);
      break;

      case -3:
        new_piece = new Bishop(x, y, BISHOP_WEIGHT, Opponent::BLACK);
        m_chess_board[y][x] = std::make_pair(new_piece, BLACK_BISHOP);
      break;

      case -4:
        new_piece = new Queen(x, y, QUEEN_WEIGHT, Opponent::BLACK);
        m_chess_board[y][x] = std::make_pair(new_piece, BLACK_QUEEN);
      break;

      case -5:
        new_piece = new King(x, y, KING_WEIGHT, Opponent::BLACK);
        m_chess_board[y][x] = std::make_pair(new_piece, BLACK_KING);
      break;

      case -6:
        new_piece = new Pawn(x, y, PAWN_WEIGHT, Opponent::BLACK);
        m_chess_board[y][x] = std::make_pair(new_piece, BLACK_PAWN);
      break;

      //> WHITE PIECES

      case 1:
        new_piece = new Rook(x, y, ROOK_WEIGHT, Opponent::WHITE);
        m_chess_board[y][x] = std::make_pair(new_piece, WHITE_ROOK);
      break;

      case 2:
        new_piece = new Knight(x, y, KNIGHT_WEIGHT, Opponent::WHITE);
        m_chess_board[y][x] = std::make_pair(new_piece, WHITE_KNIGHT);
      break;

      case 3:
        new_piece = new Bishop(x, y, BISHOP_WEIGHT, Opponent::WHITE);
        m_chess_board[y][x] = std::make_pair(new_piece, WHITE_BISHOP);
      break;

      case 4:
        new_piece = new Queen(x, y, QUEEN_WEIGHT, Opponent::WHITE);
        m_chess_board[y][x] = std::make_pair(new_piece, WHITE_QUEEN);
      break;

      case 5:
        new_piece = new King(x, y, KING_WEIGHT, Opponent::WHITE);
        m_chess_board[y][x] = std::make_pair(new_piece, WHITE_KING);
      break;

      case 6:
        new_piece = new Pawn(x, y, PAWN_WEIGHT, Opponent::WHITE);
        m_chess_board[y][x] = std::make_pair(new_piece, WHITE_PAWN);
      break;
      //> NONE

      case 0:
        m_chess_board[y][x] = std::make_pair(nullptr, NONE);
      break;

      default:
        break;
      }
    }
  }

  return;
}

void GameManager::move_piece(uint8_t x_src,
                             uint8_t y_src,
                             uint8_t x_dest,
                             uint8_t y_dest)
{
  if (m_chess_board[y_src][x_src].first != nullptr &&
      !( x_src == x_dest && y_src == y_dest))
  {
    if (check_valid_of_move(m_chess_board[y_src][x_src].first, x_dest, y_dest))
    {
      m_chess_board[y_src][x_src].first -> move(x_dest, y_dest);

      m_chess_board[y_dest][x_dest] = m_chess_board[y_src][x_src];
      m_chess_board[y_src][x_src]  = std::make_pair(nullptr, NONE);
    }
  }

  return;
}

bool GameManager::check_valid_of_move(Pieces *piece,
                                      uint8_t x_dest,
                                      uint8_t y_dest) const
{
  Moves temp_moves = get_all_valid_moves(piece);
  auto it = std::find(temp_moves.begin(),
                      temp_moves.end(),
                      std::make_pair(x_dest, y_dest));

  if (it == temp_moves.end())
  {
    return false;
  }

  return true;
}

Moves GameManager::get_all_valid_moves(Pieces *piece) const
{
  Moves valid_moves;
  std::vector< Moves > temp_moves = piece -> get_pseudo_legal_moves();

  for (auto it : temp_moves)
  {
    for (auto coord : it)
    {
      TRACE_INFO("%d, %d", coord.first, coord.second);
      if (m_chess_board[coord.second][coord.first].first != nullptr)
      {
        if (m_chess_board[coord.second][coord.first].first -> get_opponent_side()
           != piece -> get_opponent_side())
        {
          valid_moves.push_back(coord);
        }

        break;
      }

      valid_moves.push_back(coord);
    }
  }

  return valid_moves;
}

ChessBoard GameManager::get_chess_board() const
{
  return m_chess_board;
}