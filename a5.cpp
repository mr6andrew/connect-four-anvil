// ==========================================================
// CMPT 125 Assignment 5
//
// Author
// Andrew Liu
//
// Platform and Tools
// I used macOS as my operating system. I compiled my program
// using g++, and I used vs code as my editor.
//
// Extra Features
//My program does not include any additional features beyond the
// assignment requirements. I focused on correctly implementing all
// required functionality.

// Limitations and Known Bugs
// - The computer strategy is simple (no minimax)
// - The interface is text-based

//
// Help and References
// I used course materials.
//
// I also used chatgpt to review <cstdlib> and <ctime>, which I had
// learned in previous assigmnet but forgot. I ensured that I understood how these
// libraries work before using them in my code.

// Statement of Originality
// All the code and comments are my own original work.
// Any non-original work has been properly cited.
// I have not shared this work with others and have not
// copied from other students or external sources.


#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

const int ROWS = 6;
const int COLS = 7;

const char EMPTY = '.';
const char P1 = 'X';
const char P2 = 'O';
const char P1_ANVIL = 'A';
const char P2_ANVIL = 'B';




void print_title()
{
    cout << "========================================\n";
    cout << "         CONNECT FOUR: ANVIL\n";
    cout << "========================================\n";
    cout << "Rules:\n";
    cout << "1. The board is 6 rows by 7 columns.\n";
    cout << "2. Players take turns dropping pieces.\n";
    cout << "3. First player to connect 4 wins.\n";
    cout << "4. Each player may use ONE anvil per game.\n";
    cout << "5. An anvil destroys the whole column,\n";
    cout << "   then stays at the bottom as your piece.\n";
    cout << "6. You cannot drop any piece into a full column.\n";
    cout << "========================================\n\n";
}


void make_board(vector<vector<char>>& board)
{

    board.clear();
    board.resize(ROWS, vector<char>(COLS, EMPTY));
}

void print_legend()
{

    cout << "Legend:\n";
    cout << "X = Player 1 regular piece\n";
    cout << "O = Player 2 regular piece\n";
    cout << "A = Player 1 anvil\n";
    cout << "B = Player 2 anvil\n";
    cout << ". = empty\n\n";
}

void print_board(const vector<vector<char>>& board)
{

    print_legend();

    cout << "    ";
    for (int c = 0; c < COLS; c++) {
        cout << "  " << c + 1 << " ";
    }
    cout << "\n";

    for (int r = 0; r < ROWS; r++) {

        for (int sub = 0; sub < 3; sub++) {
            cout << "    ";
            for (int c = 0; c < COLS; c++) {
                char ch = board[r][c];

                if (ch == EMPTY) {

                    cout << "... ";
                } else {
                    if (sub == 1) {
                        cout << "x" << ch << "x ";
                    } else {

                        cout << "xxx ";
                    }
                }
            }

            cout << "\n";
        }
    }

    cout << "\n";
}

bool column_full(const vector<vector<char>>& board, int col)
{
    if (board[0][col] != EMPTY) {
        return true;
    }

    return false;
}

bool board_full(const vector<vector<char>>& board)
{
    for (int c = 0; c < COLS; c++) {

        if (!column_full(board, c)) {
            return false;
        }
    }
    return true;
}

int piece_owner(char ch)
{
    if (ch == P1 || ch == P1_ANVIL) {
        return 1;
    }

    if (ch == P2 || ch == P2_ANVIL) {
        return 2;
    }

    return 0;
}

char regular_piece(int player)
{
    if (player == 1) {
        return P1;
    } else {
        return P2;
    }
}

char anvil_piece(int player)
{
    if (player == 1) {
        return P1_ANVIL;
    } else {
        return P2_ANVIL;
    }
}

bool drop_regular_piece(vector<vector<char>>& board, int player, int col)
{
    if (col < 0 || col >= COLS) {
        return false;
    }

    if (column_full(board, col)) {
        return false;
    }

    for (int r = ROWS - 1; r >= 0; r--) {

        if (board[r][col] == EMPTY) {
            board[r][col] = regular_piece(player);
            return true;
        }
    }

    return false;
}

bool drop_anvil(vector<vector<char>>& board, int player, int col)
{
    if (col < 0 || col >= COLS) {
        return false;
    }

    if (column_full(board, col)) {
        return false;
    }

    for (int r = 0; r < ROWS; r++) {

        board[r][col] = EMPTY;
    }

    board[ROWS - 1][col] = anvil_piece(player);
    return true;
}

bool check_direction(const vector<vector<char>>& board, int player,
                     int row, int col, int dr, int dc)
{
    for (int i = 0; i < 4; i++) {

        int nr = row + i * dr;
        int nc = col + i * dc;

        if (nr < 0 || nr >= ROWS || nc < 0 || nc >= COLS) {
            return false;
        }

        if (piece_owner(board[nr][nc]) != player) {
            return false;
        }
    }

    return true;
}

bool player_wins(const vector<vector<char>>& board, int player)
{
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {

            if (piece_owner(board[r][c]) == player) {
                if (check_direction(board, player, r, c, 0, 1)) {
                    return true;
                }

                if (check_direction(board, player, r, c, 1, 0)) {
                    return true;
                }

                if (check_direction(board, player, r, c, 1, 1)) {
                    return true;
                }

                if (check_direction(board, player, r, c, 1, -1)) {
                    return true;
                }
            }
        }
    }

    return false;
}

int ask_game_mode()
{
    int choice;

    while (true) {

        cout << "Choose game mode:\n";
        cout << "1. Player vs Computer\n";
        cout << "2. Player vs Player\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n\n";
            continue;
        }

        cin.ignore(10000, '\n');

        if (choice == 1 || choice == 2) {
            return choice;
        }

        cout << "Please enter 1 or 2.\n\n";
    }
}

int ask_first_choice()
{
    int choice;

    while (true) {

        cout << "Who goes first?\n";
        cout << "1. First player\n";
        cout << "2. Second player / Computer\n";
        cout << "3. Random\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n\n";
            continue;
        }

        cin.ignore(10000, '\n');

        if (choice >= 1 && choice <= 3) {
            return choice;
        }

        cout << "Please enter 1, 2, or 3.\n\n";
    }
}

string ask_name(const string& prompt)
{
    string name;
    cout << prompt;
    getline(cin, name);
    return name;
}

bool ask_use_anvil(const string& name, bool anvil_used)
{
    if (anvil_used) {
        return false;
    }

    string answer;

    while (true) {


        cout << name << ", do you want to use your anvil? (y/n): ";
        getline(cin, answer);

        if (answer == "y" || answer == "Y") {
            return true;
        }


        if (answer == "n" || answer == "N") {
            return false;
        }

        cout << "Please enter y or n.\n";
    }
}

int ask_column(const vector<vector<char>>& board, const string& name)
{
    int col;

    while (true) {

        cout << name << ", choose a column (1-7): ";
        cin >> col;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        cin.ignore(10000, '\n');

        if (col < 1 || col > 7) {

            cout << "Please choose a number from 1 to 7.\n";
            continue;
        }

        if (column_full(board, col - 1)) {
            cout << "That column is full. Choose another one.\n";
            continue;
        }

        return col - 1;
    }
}

bool ask_play_again()
{
    string answer;

    while (true) {

        cout << "Do you want to play again? (y/n): ";
        getline(cin, answer);

        if (answer == "y" || answer == "Y") {
            return true;
        }

        if (answer == "n" || answer == "N") {
            return false;
        }

        cout << "Please enter y or n.\n";
    }
}

bool try_move(vector<vector<char>> board, int player, int col, bool use_anvil)
{
    bool success;

    if (use_anvil) {
        success = drop_anvil(board, player, col);
    } else {
        success = drop_regular_piece(board, player, col);
    }

    if (!success) {
        return false;
    }

    return player_wins(board, player);
}

void computer_choose_move(const vector<vector<char>>& board,
                          int computer_player,
                          bool computer_anvil_used,
                          int human_player,
                          bool human_anvil_used,
                          int& chosen_col,
                          bool& use_anvil)
{
    for (int c = 0; c < COLS; c++) {

        if (!column_full(board, c)) {

            if (try_move(board, computer_player, c, false)) {
                chosen_col = c;
                use_anvil = false;
                return;
            }
        }
    }

    if (!computer_anvil_used) {

        for (int c = 0; c < COLS; c++) {

            if (!column_full(board, c)) {
                if (try_move(board, computer_player, c, true)) {
                    chosen_col = c;
                    use_anvil = true;
                    return;
                }
            }
        }
    }

    for (int c = 0; c < COLS; c++) {

        if (!column_full(board, c)) {
            if (try_move(board, human_player, c, false)) {
                chosen_col = c;
                use_anvil = false;
                return;
            }
        }
    }

    if (!human_anvil_used && !computer_anvil_used) {

        for (int c = 0; c < COLS; c++) {

            if (!column_full(board, c)) {
                if (try_move(board, human_player, c, true)) {
                    chosen_col = c;
                    use_anvil = true;

                    return;
                }
            }
        }
    }

    int order[COLS] = {3, 2, 4, 1, 5, 0, 6};

    for (int i = 0; i < COLS; i++) {

        int c = order[i];
        if (!column_full(board, c)) {
            chosen_col = c;
            use_anvil = false;

            return;
        }
    }

    chosen_col = 0;
    use_anvil = false;
}

void play_one_game()
{
    vector<vector<char>> board;
    make_board(board);

    int mode = ask_game_mode();

    string name1;
    string name2;

    if (mode == 1) {
        name1 = ask_name("Enter your name: ");
        name2 = "Computer";
    } else {
        name1 = ask_name("Enter Player 1 name: ");
        name2 = ask_name("Enter Player 2 name: ");
    }

    int first_choice = ask_first_choice();

    string player1_name = name1;
    string player2_name = name2;

    if (first_choice == 2) {
        swap(player1_name, player2_name);
    } else if (first_choice == 3) {

        if (rand() % 2 == 1) {
            swap(player1_name, player2_name);
        }
    }

    bool first_is_computer = false;
    bool second_is_computer = false;

    if (mode == 1) {

        if (player1_name == "Computer") {
            first_is_computer = true;
            second_is_computer = false;
        } else {
            first_is_computer = false;
            second_is_computer = true;
        }
    }

    bool p1_anvil_used = false;
    bool p2_anvil_used = false;

    int current_player = 1;

    cout << "\n" << player1_name << " goes first.\n";
    cout << player2_name << " goes second.\n\n";

    while (true) {
        print_board(board);

        string current_name;
        bool current_is_computer;
        bool use_anvil = false;
        int col = 0;

        if (current_player == 1) {
            current_name = player1_name;
            current_is_computer = first_is_computer;
        } else {
            current_name = player2_name;
            current_is_computer = second_is_computer;
        }

        if (!current_is_computer) {

            if (current_player == 1) {
                use_anvil = ask_use_anvil(current_name, p1_anvil_used);
            } else {
                use_anvil = ask_use_anvil(current_name, p2_anvil_used);
            }

            col = ask_column(board, current_name);

            if (use_anvil) {
                if (current_player == 1) {
                    drop_anvil(board, 1, col);
                    p1_anvil_used = true;
                } else {
                    drop_anvil(board, 2, col);
                    p2_anvil_used = true;
                }
            } else {
                drop_regular_piece(board, current_player, col);
            }
        } else {

            cout << "Computer is thinking...\n";

            if (current_player == 1) {
                computer_choose_move(board, 1, p1_anvil_used, 2, p2_anvil_used, col, use_anvil);
            } else {
                computer_choose_move(board, 2, p2_anvil_used, 1, p1_anvil_used, col, use_anvil);
            }

            if (use_anvil) {
                cout << "Computer uses an anvil in column " << col + 1 << ".\n";
                drop_anvil(board, current_player, col);

                if (current_player == 1) {
                    p1_anvil_used = true;
                } else {
                    p2_anvil_used = true;
                }
            } else {
                
                cout << "Computer chooses column " << col + 1 << ".\n";
                drop_regular_piece(board, current_player, col);
            }
        }

        if (player_wins(board, current_player)) {
            print_board(board);
            cout << current_name << " wins!\n\n";
            break;
        }

        if (board_full(board)) {
            print_board(board);
            cout << "The game is a tie.\n\n";
            break;
        }

        if (current_player == 1) {
            current_player = 2;
        } else {
            current_player = 1;
        }
    }
}

int main()
{
    srand((unsigned int)time(0));

    print_title();

    do {
        play_one_game();
    } while (ask_play_again());

    cout << "Thanks for playing!\n";
    return 0;
}
