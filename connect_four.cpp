/****************************************************************************************
 ** Author: Javier Garcia Ramirez
 ** Filename: connect_four.cpp
 ** Date Created: M. Mar 8, 2021
 ** Description: 1-2 player Connect Four game using 2-dimensional arrays.
 ** Time Spent: 19h 35m
 ** Input: All integers. How many rows/columns, amount of players, play again, etc.
 ** Output: Game board after each turn and game results including winner.
****************************************************************************************/

/*        COMPILE USING C++ 11     --->      g++ -std=c++11 connect_four.cpp        */

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>  // include to allow rand() and srand() to be use
#include <cmath>
#include <stdio.h>
#include <ctime>    // included to allow time() to be used

using namespace std;

bool verify_arguements(int argc, char **argv);
bool displayError(bool valid, int i, string word1, string word2, string word3);
char **create_board(int rows, int columns);
void play_game(char **board, int rows, int columns, int active_players);
void displayGameResult(char **board, int columns, char piece, bool &game_over);
void assignBoardDimensions(bool valid_arg, bool play_again, char **argv, int &active_players, int &rows, int &columns);
int getColumn(int full_col[], int columns, int active_players, char piece);
void dropPiece(char **board, int rows, int columns, char piece, int full_col [], int active_players);
void print_board(char **board, int rows, int columns);
bool locate_piece(char **board, int rows, int columns, char piece);
int check_horiz_vert(char **board, int rows, int columns, string direction, int i, int j, char piece, int count);
int check_diagnal(char **board, int rows, int columns, string direction, int i, int j, char piece, int count);
int promptUnsignedInt(string error_msg, string prompt, int min_constraint, int max_constraint);
void addItem(int full_col[], int columns, int newValue);
void swap(char &a, char &b);
int generate_random(int min, int amount);



int main(int argc, char **argv) {
    int play_again;
    int active_players = 0, rows = 0, columns = 0;
    bool valid_arg = verify_arguements(argc, argv);
    do {
        assignBoardDimensions(valid_arg, play_again, argv, active_players, rows, columns);
        char **board = create_board(rows, columns);
        print_board(board, rows, columns);
        play_game(board, rows, columns, active_players);
        play_again = promptUnsignedInt("ERROR;", "Enter option \n1) play again \n2) quit \n> ", 1, 2);
        for (int i = 0; i < rows; i++) {
            delete [] board[i];
            board[i] = NULL;
        }
        delete [] board;
    } while (play_again == 1);
    return 0;
}


/****************************************************************************************
 ** Function: verifies the command line arguemnets by parsing through it and detecting
 any errors and the type of error
 ** Description: at the start of the program
 ** Parameters: integer argc, character double pointer argv
 ** Pre-Conditions: must know the values of the command line arguements
 ** Post-Conditions: returns true if arguements valid
****************************************************************************************/
bool verify_arguements(int argc, char **argv) {
    bool valid = true;
    if (argc != 4) {
        cout << "ERROR: too many/few arguments" << endl;
        return false;
    }
    string all[3] = { argv[1], argv[2], argv[3] };

    for (int i = 0; i < 3; i++) {
        string word = all[i];
        for (int j = 0; j < word.length(); j++) {
            if (!('0' <= word[j] && word[j] <= '9')) { // if not a number
                valid = displayError(false, i, all[0], all[1], all[2]);
                return valid; // will stay in loop bc less than min_constaint
            }
        }
    }
    valid = displayError(true, -1, all[0], all[1], all[2]);
    return valid;
}


/****************************************************************************************
 ** Function: displays/checks error from command line arguements accordingly
 ** Description: used if an error is located in the command line arguements
 ** Parameters: boolean valid, integer i, string word1, string word2, string word3
 ** Pre-Conditions: must have arguements as integers or the index of where the error is
 ** Post-Conditions: returns true if there are actually no errors
****************************************************************************************/
bool displayError(bool valid, int i, string word1, string word2, string word3) {
    int x = -1;
    if (valid == true){
        if (atoi(word1.c_str()) < 1 || 2 < atoi(word1.c_str()))
            x = 0;
        if (atoi(word2.c_str()) < 4 || 20 < atoi(word2.c_str()))
            x = 1;
        if (atoi(word3.c_str()) < 4 || 20 < atoi(word3.c_str()))
            x = 2;
    } if (valid == false || x != -1){
        if (i == 0 || x == 0)
            cout << "ERROR: number of players is not 1 or 2" << endl;
        else if (i == 1 || x == 1)
            cout << "ERROR: rows must be a number between 4-20" << endl;
        else if (i == 2 || x == 2)
            cout << "ERROR: columns must be a number between 4-20" << endl;
        return false;
    }
    return valid;
}


/****************************************************************************************
 ** Function: assigns the dimensions of the board depending on if there were any errors
 in the command line arguements
 ** Description: at the start of each game
 ** Parameters: boolean valid_arg, boolean play_again, character double pointer argv,
 integer addresss active_players, integer address rows, integer address columns
 integer row, integer columns, character piece
 ** Pre-Conditions: must know if the command line arguements were valid
 ** Post-Conditions: assigns values to rows and columns
****************************************************************************************/
void assignBoardDimensions(bool valid_arg, bool play_again, char **argv, int &active_players, int &rows, int &columns) {
    if (valid_arg == false || play_again == true) {
        active_players = promptUnsignedInt("ERROR: number of players is not 1 or 2;", "Enter how many players: ", 1, 2);
        rows = promptUnsignedInt ("ERROR: rows must be a number between 4-20;", "Enter rows: ", 4, 20);
        columns = promptUnsignedInt ("ERROR: columns must be a number between 4-20;", "Enter columns: ", 4, 20);
    } else if (valid_arg == true) {
        active_players = atoi(argv[1]);
        rows = atoi(argv[2]);
        columns = atoi(argv[3]);
    }
}


/****************************************************************************************
 ** Function: creates the board for the game
 ** Description: used at the start of the game
 ** Parameters: integer row, integer columns
 ** Pre-Conditions: must have dimensions of board
 ** Post-Conditions: returns the 2-dimensional board
****************************************************************************************/
char **create_board(int rows, int columns) { // delete board function too
    char **board = new char*[rows];

    for (int i = 0; i < rows; i++)
        board[i] = new char[columns];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j] = ' ';
        }
    }
    return board;
}


/****************************************************************************************
 ** Function: operates game functions such as switching players after a turn, and calling
 the display game result function at the end of each turn
 ** Description: called once but used throughout the whole game
 ** Parameters: character double pointer board,
 integer row, integer columns, integer active_piece
 ** Pre-Conditions: must have dimensions of board and how many players there are
 ** Post-Conditions: the game must end through a tie or because there is a winner
****************************************************************************************/
void play_game(char **board, int rows, int columns, int active_players){
    int full_col[columns];
    bool game_over = false;
    for (int i = 0; i < columns; i++)
        full_col[i] = -1;
    char player1_piece = 'X', player2_piece = 'O';
    if (active_players == 1){
        int decision = promptUnsignedInt("ERROR;", "Enter option \n1) Go First \n2) Go second \n> ", 1, 2);
        if (decision == 1)
            swap(player2_piece, player1_piece);
    }
    do {
        cout << "* Player '" << player1_piece << "' turn *" << endl; // do they chose their pieces?
        dropPiece(board, rows, columns, player1_piece, full_col, active_players);
        game_over = locate_piece(board, rows, columns, player1_piece);

        displayGameResult(board, columns, player1_piece, game_over);
        swap(player2_piece, player1_piece);
    } while (game_over == false);
}


/****************************************************************************************
 ** Function: displays if there is a winner or if it is a tie
 ** Description: used at the end of each round
 ** Parameters: character double pointer board, integer columns, character piece, bool
 address game_over
 ** Pre-Conditions: must be at the end of the current player's turn
 ** Post-Conditions: if there is a winner or tie, prints the winner or tie
****************************************************************************************/
void displayGameResult(char **board, int columns, char piece, bool &game_over) {
    if (game_over == true)
        cout << "GAME RESULT: Player '" << piece << "' wins" << endl;
    else {
        int count = 0;
        for (int j = 0; j < columns; j++) {
            if (board[0][j] == ' ')
                break;
            count++;
            if (count == columns) {
                game_over = true; // tie
                cout << "GAME RESULT: Tie " << endl;
            }
        }
    }
}

/****************************************************************************************
 ** Function: gets valid column from computer/user
 ** Description: used whenever a piece is located
 ** Parameters: character double pointer board, integer columns, integer active_players,
 character piece
 ** Pre-Conditions: must know how many players there are and what columns are full
 ** Post-Conditions: returns where to drop piece
****************************************************************************************/
int getColumn(int full_col[], int columns, int active_players, char piece) {
    bool error = false;
    int test2 = 0;

    do {
        error = false;
        if (active_players == 1 && piece == 'X') // X is the piece for the computer
            test2 = generate_random(1, columns); // min, how many
        else
            test2 = promptUnsignedInt("ERROR: select column (1-" + to_string(columns+1) + ") that is not full;", "Enter column number: ", 1, columns);

        test2--;
        for (int i = 0; i < columns; i++) {
            if (full_col[i] == test2)
                error = true;
        }
    } while (error == true);

    return test2;
}


/****************************************************************************************
 ** Function: drops the piece in the appropriate spot in board
 ** Description: used whenever a column is generated/entered by user
 ** Parameters: character double pointer board, integer row, integer columns, character
 piece, integer array full_col[], integer active_players
 ** Pre-Conditions: must know where (column) to drop the piece
 ** Post-Conditions: board is updated
****************************************************************************************/
void dropPiece(char **board, int rows, int columns, char piece, int full_col [], int active_players) {
    int test2 = getColumn(full_col, columns, active_players, piece); // get where to drop piece

    // drop piece
    for (int i = 0; i < rows; i++){
        if (board[i][test2] != ' ') { // must be an X or O
            board[i-1][test2] = piece;
            if (i-1 == 0) // checks if top collumn is full
                addItem(full_col, columns, test2); // adds full collumn to full_coll array to keep track
            break;
        } else if (i == rows-1 && board[i][test2] == ' ') { // place piece on bottom row
            board[i][test2] = piece; // and add this test2 to full_col array
        }
    }
    print_board(board, rows, columns);
}


/****************************************************************************************
 ** Function: prints the board
 ** Description: used at the start of the game and at the end of each player's turn
 ** Parameters: character double pointer board, integer row, integer columns
 ** Pre-Conditions: must have dimensions of board
 ** Post-Conditions: board must be printed
****************************************************************************************/
void print_board(char **board, int rows, int columns){
   for (int i=0; i<rows; i++) {
        cout << char(i+65) << " ";
        for (int j=0; j<columns; j++) {
            if (i % 2 == 0 && j % 2 == 0)
                cout << "|\033[30;47m " << board[i][j] << " ";
            else if (i % 2 == 1 && j % 2 == 1)
                cout << "|\033[30;47m " << board[i][j] << " ";
            else
                cout << "|\033[0m " << board[i][j] << " ";
            cout << "\033[0m";
        }
        cout << endl;
    }
    cout << "    ";
    for (int i = 0; i < columns; i++)
        (i < 9) ? cout << i+1 << "   " : cout << i+1 << "  ";
    cout << endl;
}

/****************************************************************************************
 ** Function: checks if there is a diagnal connect 4
 ** Description: used whenever a piece is located
 ** Parameters: character double pointer board,
 integer row, integer columns, character piece
 ** Pre-Conditions: must have dimensions of board
 ** Post-Conditions: returns true if there is a connect 4, false if not
****************************************************************************************/
bool locate_piece(char **board, int rows, int columns, char piece) {
    int count = 0;
    int p = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (board[i][j] == piece) { // piece is found
                // find if there is a connect 4
                (count != 4) ? (count = check_horiz_vert(board, rows, columns, "horizontal", i, j, piece, 0)) : p++;
                (count != 4) ? (count = check_horiz_vert(board, rows, columns, "verticle", i, j, piece, 0)) : p++;
                (count != 4) ? (count = check_diagnal(board, rows, columns, "right", i, j, piece, 0)) : p++;
                (count != 4) ? (count = check_diagnal(board, rows, columns, "left", i, j, piece, 0)) : p++;
            }
            if ( 4 <= count)
                return true;
        }
    }
    return false;
}


/****************************************************************************************
 ** Function: checks if there is a horizontal or vericle connect 4
 ** Description: used whenever a piece is located
 ** Parameters: character double pointer board, integer row, integer columns, string
 direction, integer i, integer j, character piece, integer count
 ** Pre-Conditions: must know i and j to know the location of the current piece
 ** Post-Conditions: return the amount of pieces found horizontally or vertically
****************************************************************************************/
int check_horiz_vert(char **board, int rows, int columns, string direction, int i, int j, char piece, int count) {
    while (board[i][j] == piece) {
        count++;
        if (count == 4)
            break;
        if (direction == "verticle" && i+1 < rows) // stay within bounds
            i += 1;
        else if (direction == "horizontal" && j + 1 < columns) // stay within bounds
            j += 1;
        else
            break;
    }
    return count;
}


/****************************************************************************************
 ** Function: checks if there is a diagnal connect 4
 ** Description: used whenever a piece is located
 ** Parameters: character double pointer board, integer row, integer columns, string
 direction, integer i, integer j, character piece, integer count
 ** Pre-Conditions: must know i and j to know the location of the current piece
 ** Post-Conditions: return the amount of pieces found diagonally
****************************************************************************************/
int check_diagnal(char **board, int rows, int columns, string direction, int i, int j, char piece, int count) {
    int TEST = 1;

    if (direction == "left")
        TEST = -1;
    while (board[i][j] == piece) {
        count++;
        if ((i + 1 < rows && j + TEST < columns) && count != 4){ // stay within bounds
            i++;
            j = j + TEST; // TEST = 1 if direction is diagnal downward right, TEST = -1 if direction is downward left

        } else
            break;
    }
    return count;
}

/****************************************************************************************
 ** Function: prompt for an integar and check if it is a positive number
 ** Description: used throughtout the whole game
 ** Parameters: string error_message, string prompt, integer min_contraint, integer max_constraint
 ** Pre-Conditions: must know the max and min numbers possible
 ** Post-Conditions: returns the integer if it is equal to or greater than 0
****************************************************************************************/
int promptUnsignedInt(string error_msg, string prompt, int min_constraint, int max_constraint) {
    int test_int = -2;
    string test_str = "";

    do {
        test_int == -2 ? cout << prompt : cout << error_msg << " " << prompt; // only -2 once, for the first prompt
        getline(cin, test_str);

        for (int i = 0; i < test_str.length(); i++) {
            if (!('0' <= test_str[i] && test_str[i] <= '9')) // if not a number
                test_str = "-1"; // will stay in loop bc less than min_constaint
        }
        test_int = atoi(test_str.c_str()); // convert string to integer
    } while (test_int < min_constraint || max_constraint < test_int);

    return test_int;
}


/****************************************************************************************
 ** Function: appends an item to the array
 ** Description: used when a column gets full to keep track of it
 ** Parameters: integer array full_col[], integer columns, interger newValue
 ** Pre-Conditions: must know array and it must be an integer
 ** Post-Conditions: array is updated with new value
****************************************************************************************/
void addItem(int full_col[], int columns, int newValue) {
    for (int i = 0; i < columns; i++) {
        if (full_col[i] == -1){
            full_col[i] = newValue;
            break;
        }
    }
}


/****************************************************************************************
 ** Function: swaps values of arguements
 ** Description: used at the end of each player's turn to change pieces
 ** Parameters: character address a, character address b
 ** Pre-Conditions: a and b must be characters
 ** Post-Conditions: a and b and flipped
****************************************************************************************/
void swap(char &a, char &b){
    int copy_a = a;
    a = b;
    b = copy_a;
}


/****************************************************************************************
 ** Function: generate a random number for player(s) and dealer
 ** Description: used to generate computer's column selection
 ** Parameters: integer min, integer amoubt
 ** Pre-Conditions: must know the amount of columns in grid
 ** Post-Conditions: return a random number appropriate for the grid
****************************************************************************************/
int generate_random(int min, int amount) {
    int random_number;
    random_number = rand() % amount + min; // % how many numbers + min
	return random_number;
}
