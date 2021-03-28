/****************************************************************************************
 ** Author: Javier Garcia Ramirez
 ** Filename: connect_four.cpp
 ** Date Created: M. Mar 8, 2021
 ** Description: 1-2 player Connect Four game using 2-dimensional arrays.
 ** Time Spent: 19h 35m
 ** Input: All integers. How many rows/columns, amount of players, play again, etc.
 ** Output: Game board after each turn and game results including winner.
****************************************************************************************/


#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>  // include to allow rand() and srand() to be use
#include <cmath>
#include <stdio.h>
#include <ctime>    // included to allow time() to be used


bool verify_arguements(int argc, char **argv);
bool displayError(bool valid, int i, std::string word1, std::string word2, std::string word3);
char **create_board(int rows, int columns);
void play_game(char **board, int rows, int columns, int active_players);
void displayGameResult(char **board, int columns, char piece, bool &game_over);
void assignBoardDimensions(bool valid_arg, bool play_again, char **argv, int &active_players, int &rows, int &columns);
int getColumn(int full_col[], int columns, int active_players, char piece);
void dropPiece(char **board, int rows, int columns, char piece, int full_col [], int active_players);
void print_board(char **board, int rows, int columns);
bool locate_piece(char **board, int rows, int columns, char piece);
int check_horiz_vert(char **board, int rows, int columns, std::string direction, int i, int j, char piece, int count);
int check_diagnal(char **board, int rows, int columns, std::string direction, int i, int j, char piece, int count);
int promptUnsignedInt(std::string error_msg, std::string prompt, int min_constraint, int max_constraint);
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


/*verifies the command line arguemnets by parsing through 
it and detecting any errors and the type of error */
bool verify_arguements(int argc, char **argv) {
    bool valid = true;
    if (argc != 4) {
        std::cout << "ERROR: too many/few arguments" << std::endl;
        return false;
    }
    std::string all[3] = { argv[1], argv[2], argv[3] };

    for (int i = 0; i < 3; i++) {
        std::string word = all[i];
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


/* displays/checks error from command line arguements accordingly 
used if an error is located in the command line arguements */
bool displayError(bool valid, int i, std::string word1, std::string word2, std::string word3) {
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
            std::cout << "ERROR: number of players is not 1 or 2" << std::endl;
        else if (i == 1 || x == 1)
            std::cout << "ERROR: rows must be a number between 4-20" << std::endl;
        else if (i == 2 || x == 2)
            std::cout << "ERROR: columns must be a number between 4-20" << std::endl;
        return false;
    }
    return valid;
}


// assigns the dimensions of the board, even if there were no errors in the command line arguements
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


/* operates game functions such as switching players after a turn, and calling the display game result 
function at the end of each turn */
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
        std::cout << "* Player '" << player1_piece << "' turn *" << std::endl; // do they chose their pieces?
        dropPiece(board, rows, columns, player1_piece, full_col, active_players);
        game_over = locate_piece(board, rows, columns, player1_piece);

        displayGameResult(board, columns, player1_piece, game_over);
        swap(player2_piece, player1_piece);
    } while (game_over == false);
}


// displays if there is a winner or if it is a tie, used at the end of each round
void displayGameResult(char **board, int columns, char piece, bool &game_over) {
    if (game_over == true)
        std::cout << "GAME RESULT: Player '" << piece << "' wins" << std::endl;
    else {
        int count = 0;
        for (int j = 0; j < columns; j++) {
            if (board[0][j] == ' ')
                break;
            count++;
            if (count == columns) {
                game_over = true; // tie
                std::cout << "GAME RESULT: Tie " << std::endl;
            }
        }
    }
}


// gets valid column from computer/user
int getColumn(int full_col[], int columns, int active_players, char piece) {
    bool error = false;
    int test2 = 0;

    do {
        error = false;
        if (active_players == 1 && piece == 'X') // X is the piece for the computer
            test2 = generate_random(1, columns); // min, how many
        else
            test2 = promptUnsignedInt("ERROR: select column (1-" + std::to_string(columns+1) + ") that is not full;", "Enter column number: ", 1, columns);

        test2--;
        for (int i = 0; i < columns; i++) {
            if (full_col[i] == test2)
                error = true;
        }
    } while (error == true);

    return test2;
}


// drops the piece in the appropriate spot in board
void dropPiece(char **board, int rows, int columns, char piece, int full_col [], int active_players) {
    int test2 = getColumn(full_col, columns, active_players, piece); // get where to drop piece

    // drop piece
    for (int i = 0; i < rows; i++) {
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



void print_board(char **board, int rows, int columns) {
   std:: cout << std::endl << std::endl; 
   for (int i = 0; i < rows; i++) {
        std::cout << char(i+65) << " ";
        for (int j = 0; j < columns; j++) {
            if (i % 2 == 0 && j % 2 == 0)
                std::cout << "|\033[30;47m " << board[i][j] << " ";
            else if (i % 2 == 1 && j % 2 == 1)
                std::cout << "|\033[30;47m " << board[i][j] << " ";
            else
                std::cout << "|\033[0m " << board[i][j] << " ";
            std::cout << "\033[0m";
        }
        std::cout << std::endl;
    }
    std::cout << "    ";
    for (int i = 0; i < columns; i++)
        (i < 9) ? std::cout << i+1 << "   " : std::cout << i+1 << "  ";
    std::cout << std::endl;
}


// checks if there is a downward right OR left diagnal connect 4
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


// checks if there is a horizontal OR vericle connect 4
int check_horiz_vert(char **board, int rows, int columns, std::string direction, int i, int j, char piece, int count) {
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


// checks if there is a diagnal connect 4
int check_diagnal(char **board, int rows, int columns, std::string direction, int i, int j, char piece, int count) {
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


// prompt for an integar and check if it is a positive number
int promptUnsignedInt(std::string error_msg, std::string prompt, int min_constraint, int max_constraint) {
    int test_int = -2;
    std::string test_str = "";

    do {
        test_int == -2 ? std::cout << prompt : std::cout << error_msg << " " << prompt; // only -2 once, for the first prompt
        getline(std::cin, test_str);

        for (int i = 0; i < test_str.length(); i++) {
            if (!('0' <= test_str[i] && test_str[i] <= '9')) // if not a number
                test_str = "-1"; // will stay in loop bc less than min_constaint
        }
        test_int = atoi(test_str.c_str()); // convert string to integer
    } while (test_int < min_constraint || max_constraint < test_int);

    return test_int;
}


// appends an item to the array, used when a column gets full to keep track of it
void addItem(int full_col[], int columns, int newValue) {
    for (int i = 0; i < columns; i++) {
        if (full_col[i] == -1){
            full_col[i] = newValue;
            break;
        }
    }
}


// swaps values of arguements
void swap(char &a, char &b){
    int copy_a = a;
    a = b;
    b = copy_a;
}


// used to generate computer's column selection
int generate_random(int min, int amount) {
    int random_number;
    random_number = rand() % amount + min; // % how many numbers + min
	return random_number;
}