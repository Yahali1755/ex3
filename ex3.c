#include <stdio.h>

#ifndef ROWS
#define ROWS 6
#endif

#ifndef COLS
#define COLS 7
#endif

#define CONNECT_N 4

/* Tokens */
#define EMPTY '.'
#define TOKEN_P1 'X'
#define TOKEN_P2 'O'
#define COLUMN_FULL_INDEX -1
#define PLAYER1_TURN 1
#define PLAYER2_TURN 0
#define PLAYER1_NUMBER 1
#define PLAYER2_NUMBER 2

#define HUMAN 1
#define COMPUTER 2

int isColumnFull(char[][COLS], int, int, int);

int isBoardFull(char[][COLS], int, int);

int isInBounds(int, int, int, int);

/* Return index of row where token will land, or -1 if column full */
int getFreeRow(char[][COLS], int, int, int);

/* Place token in column (0-based). Return row index or -1 if illegal */
int makeMove(char[][COLS], int, int, int, char);

int checkVictory(char[][COLS], int, int, int, int, char);

/* Human player: asks repeatedly until a valid non-full column is chosen (0-based) */
int humanChoose(char[][COLS], int, int);

/* Computer*/
int computerChoose(char[][COLS], int, int, char, char);

void runConnectFour(char[][COLS], int, int, int, int);

void initBoard(char[][COLS], int, int);

void printBoard(char[][COLS], int, int);

int getPlayerType(int);

int main() {
    char board[ROWS][COLS];
    printf("Connect Four (%d rows x %d cols)\n\n", ROWS, COLS);
    int p1Type = getPlayerType(1);
    int p2Type = getPlayerType(2);
    initBoard(board, ROWS, COLS);
    printBoard(board, ROWS, COLS);
    runConnectFour(board, ROWS, COLS, p1Type, p2Type);
    return 0;
}

void printBoard(char board[][COLS], int rows, int cols) {
    printf("\n");
    for (int r = 0; r < rows; r++) {
        printf("|");
        for (int c = 0; c < cols; c++) {
            putchar(board[r][c]);
            printf("|");
        }
        printf("\n");
    }
    for (int c = 1; c <= cols; c++) {
        printf(" %d", c % 10);
    }
    printf("\n\n");
}

int getPlayerType(int playerNumber) {
    char ch;
    while (1) {
        printf("Choose type for player %d: h - human, c - computer: ", playerNumber);
        int n = scanf(" %c", &ch);
        if (n != 1) {
            printf("Input error. Try again.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        if (ch == 'h' || ch == 'H') return HUMAN;
        if (ch == 'c' || ch == 'C') return COMPUTER;

        printf("Invalid selection. Enter h or c.\n");
        while (getchar() != '\n'); // clear rest of input
    }
}

void initBoard(char board[][COLS], int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            board[r][c] = EMPTY;
        }
    }
}

int isBoardFull(char board[][COLS], int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (board[r][c] == EMPTY) {
                return 0;
            };
        }
    }

    return 1;
}

int isColumnFull(char board[][COLS], int rows, int columns, int column) {
    for (int row = 0; row < rows; row++) {
        if (board[row][column] == EMPTY) {
            return 0;
        }
    }

    return 1;
}

int isInBounds(int rows, int columns, int row, int column) {
    if (row >= 0 && row < rows && column >= 0 && column < columns) {
        return 1; 
    }

    return 0;
}   

int getFreeRow(char board[][COLS], int rows, int columns, int column) {
    for (int row = rows - 1; row >= 0; row--) {
        if (board[row][column] == EMPTY) {
            return row;
        }
    }

    return COLUMN_FULL_INDEX;
}

int makeMove(char board[][COLS], int rows, int columns, int column, char token) {
    int freeRow = getFreeRow(board, rows, columns, column);

    if (freeRow == COLUMN_FULL_INDEX) {
        return COLUMN_FULL_INDEX;
    }

    board[freeRow][column] = token;

    return freeRow;
}

int checkVictory(char board[][COLS], int rows, int columns, int row, int column, char token) {
    // Check if player with given token won by a column of 4 tokens
    if (isInBounds(rows, columns, row + 3, column) &&
        token == board[row][column] && 
        token == board[row + 1][column] && 
        token == board[row + 2][column] && 
        token == board[row + 3][column] ) {
        return 1;
    }

    // Check if player with given token won by a row of 4 tokens
    for (int columnIndex = column - 3; columnIndex <= column; columnIndex++) {
        if (isInBounds(rows, columns, row, columnIndex) && isInBounds(rows, columns, row, columnIndex + 3) &&
            token == board[row][columnIndex] && 
            token == board[row][columnIndex + 1] && 
            token == board[row][columnIndex + 2] && 
            token == board[row][columnIndex + 3] ) {
            return 1;
        }
    }

    // Check if player with given token won by right diagonal of 4 tokens
    for (int index = 0; index <= 3; index++) {
        if (isInBounds(rows, columns, row - index, column - index) && 
            isInBounds(rows, columns, (row + 3) - index, (column + 3) - index) &&
            token == board[row - index][column - index] && 
            token == board[row + 1 - index][column + 1 - index] && 
            token == board[row + 2 - index][column + 2 - index] && 
            token == board[row + 3 - index][column + 3 - index] ) {
            return 1;
        }
    }

    // Check if player with given token won by left diagonal of 4 tokens
    for (int index = 0; index <= 3; index++) {
        if (isInBounds(rows, columns, row - index, column + index) && 
            isInBounds(rows, columns, (row + 3) - index, (column - 3) + index) &&
            token == board[row - index][column + index] && 
            token == board[row + 1 - index][column - 1 + index] && 
            token == board[row + 2 - index][column - 2 + index] && 
            token == board[row + 3 - index][column - 3 + index] ) {
            return 1;
        }
    }

    return 0;
}

int humanChoose(char board[][COLS], int rows, int columns) {
    int inputColumn;
    int scanResult;
    int chosenColumn;

    printf("Enter column (1-%d): ", columns);

    do {
        scanResult = scanf("%d", &inputColumn);

        if (scanResult != 1) {
            printf("Invalid input. Enter a number.\n");
            
           // clear entered char from the buffer
            while (getchar() != '\n'); 

            continue;
        }

        chosenColumn = inputColumn - 1;

        if (chosenColumn < 0 || chosenColumn >= columns) {
            printf("Invalid column. Choose between 1 and %d.\n", columns);

            continue;
        }

        if (isColumnFull(board, rows, columns, chosenColumn)) {
            printf("Column %d is full. Choose another column.\n", inputColumn);

            continue;
        }

        return chosenColumn;
    } while (1);
}

void runConnectFour(char board[][COLS], int rows, int columns, int player1Type, int player2Type) {
    int turn = PLAYER1_TURN;
    int column;
    int row;
    int token;
    int playerNumber;

    if (player1Type == HUMAN && player2Type == HUMAN) {
        do {
            int playerNumber = turn ? PLAYER1_NUMBER : PLAYER2_NUMBER;
            int token = turn ? TOKEN_P1 : TOKEN_P2;
            printf("Player %d (%c) turn.\n", playerNumber, token);

            column = humanChoose(board, rows, columns);
            row = makeMove(board, rows, columns, column, token);

            printBoard(board, rows, columns);

            if (checkVictory(board, rows, columns, row, column, token)) {
                printf("Player %d (%c) wins!\n", playerNumber, token);

                break;
            }

            turn = turn == PLAYER1_TURN ? PLAYER2_TURN : PLAYER1_TURN;
        } while (1);
    }
}