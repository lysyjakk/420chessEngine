#include "../inc/movelookup.hh"

void MoveLookup::init(BitBoardToGUI board)
{
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
          b = Bitboard().set_bit_at( std::size_t( COORD_TO_BIT_POS(row, col)) );
          m_board.black_rooks |= b;
          m_pieces_pos[63 - (row * 8 + col)] = BLACK_ROOK;
        break;

        case BLACK_KNIGHT:
          b = Bitboard().set_bit_at( std::size_t( COORD_TO_BIT_POS(row, col)) );
          m_board.black_knights |= b;
          m_pieces_pos[63 - (row * 8 + col)] = BLACK_KNIGHT;
        break;

        case BLACK_BISHOP:
          b = Bitboard().set_bit_at( std::size_t( COORD_TO_BIT_POS(row, col)) ); 
          m_board.black_bishops |= b;
          m_pieces_pos[63 - (row * 8 + col)] = BLACK_BISHOP;
        break;

        case BLACK_QUEEN:
          b = Bitboard().set_bit_at( std::size_t( COORD_TO_BIT_POS(row, col)) );
          m_board.black_queens |= b;
          m_pieces_pos[63 - (row * 8 + col)] = BLACK_QUEEN;
        break;

        case BLACK_KING:
          b = Bitboard().set_bit_at( std::size_t( COORD_TO_BIT_POS(row, col)) );
          m_board.black_king |= b;
          m_pieces_pos[63 - (row * 8 + col)] = BLACK_KING;
        break;

        case BLACK_PAWN:
          b = Bitboard().set_bit_at( std::size_t( COORD_TO_BIT_POS(row, col)) );
          m_board.black_pawns |= b;
          m_pieces_pos[63 - (row * 8 + col)] = BLACK_PAWN;
        break;

        //> WHITE PIECES

        case WHITE_ROOK:
          b = Bitboard().set_bit_at( std::size_t( COORD_TO_BIT_POS(row, col)) );
          m_board.white_rooks |= b;
          m_pieces_pos[63 - (row * 8 + col)] = WHITE_ROOK;
        break;

        case WHITE_KNIGHT:
          b = Bitboard().set_bit_at( std::size_t( COORD_TO_BIT_POS(row, col)) );
          m_board.white_knights |= b;
          m_pieces_pos[63 - (row * 8 + col)] = WHITE_KNIGHT;
        break;

        case WHITE_BISHOP:
          b = Bitboard().set_bit_at( std::size_t( COORD_TO_BIT_POS(row, col)) );
          m_board.white_bishops |= b;
          m_pieces_pos[63 - (row * 8 + col)] = WHITE_BISHOP;
        break;

        case WHITE_QUEEN:
          b = Bitboard().set_bit_at( std::size_t( COORD_TO_BIT_POS(row, col)) );
          m_board.white_queens |= b;
          m_pieces_pos[63 - (row * 8 + col)] = WHITE_QUEEN;
        break;

        case WHITE_KING:
          b = Bitboard().set_bit_at( std::size_t( COORD_TO_BIT_POS(row, col)) );
          m_board.white_king |= b;
          m_pieces_pos[63 - (row * 8 + col)] = WHITE_KING;
        break;

        case WHITE_PAWN:
          b = Bitboard().set_bit_at( std::size_t( COORD_TO_BIT_POS(row, col)) );
          m_board.white_pawns |= b;
          m_pieces_pos[63 - (row * 8 + col)] = WHITE_PAWN;
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
}

bool MoveLookup::is_move_valid() const
{
  return true;
}