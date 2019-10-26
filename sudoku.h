void load_board(const char* filename, char board[9][9]);
void display_board(const char board[9][9]);
bool is_complete(const char board[9][9]);
bool make_move(const char move_str[], char digit, char board[9][9]); 
bool save_board(const char* filename, char board[9][9]);
bool solve_board(char board[9][9], int row = 0, int col = 0);
bool solve_next(char board[9][9], int row, int col);
bool is_valid(const char board[9][9], int row, int col, int num);
