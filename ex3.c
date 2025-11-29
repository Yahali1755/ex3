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

int ensureValidColumn(int, int);

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

void createPriorityOrderArray(int[], int);

int checkSequenceOfN(char[][COLS], int, int, int, int, char, int);

int canMakeSequenceOfNMinus1(char[][COLS], int, int, int, char);

int isSequenceOfNByColumn(char[][COLS], int, int, int, int, char, int);

int isSequenceOfNByRow(char[][COLS], int, int, int, int, char, int);

int isSequenceOfNByRightDiagonal(char[][COLS], int, int, int, int, char, int);

int isSequenceOfNByLeftDiagonal(char[][COLS], int, int, int, int, char, int);

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

void initBoard(char board[][COLS], int rows, int columns) {
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            board[row][column] = EMPTY;
        }
    }
}

int isBoardFull(char board[][COLS], int rows, int columns) {
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            if (board[row][column] == EMPTY) {
                return 0;
            };
        }
    }

    return 1;
}

int ensureValidColumn(int columns, int column) {
    if (column < 0 || column >= columns) {
        return 0;
    }

    return 1;
}

int isColumnFull(char board[][COLS], int rows, int columns, int column) {
    if (!ensureValidColumn(columns, column)) 
        return COLUMN_FULL_INDEX;

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
    if (!ensureValidColumn(columns, column)) 
        return COLUMN_FULL_INDEX;

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

int isSequenceOfNByColumn(char board[][COLS], int rows, int columns, int row, int column, char token, int n) {
    if (!isInBounds(rows, columns, row + n - 1, column)) {
        return 0;
    }

    for (int index = 0; index < n; index++) {
        if (token != board[row + index][column]) {
            return 0;
        }
    }

    return 1;
}

int isSequenceOfNByRow(char board[][COLS], int rows, int columns, int row, int column, char token, int n) {
    if (!isInBounds(rows, columns, row, column) || !isInBounds(rows, columns, row, column + (n - 1))) {
        return 0;
    }

    for (int index = 0; index < n; index++) {
        if (token != board[row][column + index]) {
            return 0;
        }
    }

    return 1;
}

int isSequenceOfNByRightDiagonal(char board[][COLS], int rows, int columns, int row, int column, char token, int n) {
    if (!isInBounds(rows, columns, row + (n - 1), column + (n - 1)) || !isInBounds(rows, columns, row, column)) {
        return 0;
    }

    for (int index = 0; index < n; index++) {
        if (token != board[row + index][column + index]) {
            return 0;
        }
    }

    return 1;
}

int isSequenceOfNByLeftDiagonal(char board[][COLS], int rows, int columns, int row, int column, char token, int n) {
    if (!isInBounds(rows, columns, row + (n - 1), column - (n - 1)) || !isInBounds(rows, columns, row, column)) {
        return 0;
    }

    for (int index = 0; index < n; index++) {
        if (token != board[row + index][column - index]) {
            return 0;
        }
    }

    return 1;
}

int checkSequenceOfN(char board[][COLS], int rows, int columns, int row, int column, char token, int n) {
    // Check if sequence of n was made by column
    if (isSequenceOfNByColumn(board, rows, columns, row, column, token, n)) {
        return 1;
    }

    // Check if sequence of n was made by row
    for (int i = 0; i < n; i++) {
        if (isSequenceOfNByRow(board, rows, columns, row, column - i, token, n)) {
            return 1;
        };
    }

    // Check if sequence of n was made by right diagonal
    for (int index = 0; index < n; index++) {
        if (isSequenceOfNByRightDiagonal(board, rows, columns, row - index, column - index, token, n)) {
            return 1;
        }
    }

    // Check if sequence of n was made by left diagonal
    for (int index = 0; index < n; index++) {
        if (isSequenceOfNByLeftDiagonal(board, rows, columns, row - index, column + index, token, n)) {
            return 1;
        }
    }

    return 0;
}

// would be canMakeSequenceOfThree in a regular connect four, its generic for N minus 1
int canMakeSequenceOfNMinus1(char board[][COLS], int rows, int columns, int column, char token) {
    int freeRow;

    freeRow = getFreeRow(board, rows, columns, column);

    if (freeRow == COLUMN_FULL_INDEX) {
        return 0;
    }

    // Make a move then check if it created a sequence of three and track it
    makeMove(board, rows, columns, column, token);

    if (checkSequenceOfN(board, rows, columns, freeRow, column, token, CONNECT_N - 1)) {
        // Reset the move so it doesn't actually happen
        board[freeRow][column] = EMPTY;

        return 1;
    }

    // Reset the move so it doesn't actually happen
    board[freeRow][column] = EMPTY;

    return 0;
}

void createPriorityOrderArray(int orderArray[], int columns) {
    int left = (columns + 1) / 2;
    int right = (columns / 2) + 1;
    int count = 0;

    while (left >= 1 && right <= columns) {
        orderArray[count++] = left;

        // Avoid adding the center twice
        if (right != left) {
            orderArray[count++] = right;
        }

        left--;
        right++;
    }
}

int checkVictory(char board[][COLS], int rows, int columns, int row, int column, char token) {
    return checkSequenceOfN(board, rows, columns, row, column, token, CONNECT_N);
}

int humanChoose(char board[][COLS], int rows, int columns) {
    int inputColumn;
    int scanResult;
    int chosenColumn;

    do {
        printf("Enter column (1-%d): ", columns);

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

int computerChoose(char board[][COLS], int rows, int columns, char computerToken, char enemyToken) {
    int freeRow;
    int priorityOrder[COLS];
    int column;

    createPriorityOrderArray(priorityOrder, columns);

    for (int columnIndex = 0; columnIndex < columns; columnIndex++) {
        column = priorityOrder[columnIndex] - 1;
        freeRow = getFreeRow(board, rows, columns, column);

        if (freeRow == COLUMN_FULL_INDEX) {
            continue;
        }

        // Make a move then check if it created a sequence of three (or n - 1) and track it
        makeMove(board, rows, columns, column, computerToken);

        if (checkVictory(board, rows, columns, freeRow, column, computerToken)) {
            board[freeRow][column] = EMPTY;

            return column;
        }

        // Make the move doesn't actually happen in this function
        board[freeRow][column] = EMPTY;
    }

    for (int columnIndex = 0; columnIndex < columns; columnIndex++) {
        column = priorityOrder[columnIndex] - 1;
        freeRow = getFreeRow(board, rows, columns, column);

        if (freeRow == COLUMN_FULL_INDEX) {
            continue;
        }

        // Make a move then check if it created a sequence of three and track it
        makeMove(board, rows, columns, column, enemyToken);

        if (checkVictory(board, rows, columns, freeRow, column, enemyToken)) {
            board[freeRow][column] = EMPTY;

            return column;
        }

        // Make the move doesnt actually happen in this function
        board[freeRow][column] = EMPTY;
    }

    for (int columnIndex = 0; columnIndex < columns; columnIndex++) {
        column = priorityOrder[columnIndex] - 1;
        freeRow = getFreeRow(board, rows, columns, column);

        if (freeRow == COLUMN_FULL_INDEX) {
            continue;
        }

        if (canMakeSequenceOfNMinus1(board, rows, columns, column, computerToken)) {
            return column;
        }
    }

    for (int columnIndex = 0; columnIndex < columns; columnIndex++) {
        column = priorityOrder[columnIndex] - 1;
        freeRow = getFreeRow(board, rows, columns, column);

        if (freeRow == COLUMN_FULL_INDEX) {
            continue;
        }

        if (canMakeSequenceOfNMinus1(board, rows, columns, column, enemyToken)) {
            return column;
        }
    }

    // if none of the winning, blocking or n - 1 sequences are not an option choose column by priority order
    for (int columnIndex = 0; columnIndex < columns; columnIndex++) {
        column = priorityOrder[columnIndex] - 1;
        freeRow = getFreeRow(board, rows, columns, column);

        if (freeRow == COLUMN_FULL_INDEX) {
            continue;
        }

        return column;
    }

    return -1;
} 

void runConnectFour(char board[][COLS], int rows, int columns, int player1Type, int player2Type) {
    int turn = PLAYER1_TURN;
    int column;
    int row;
    int token;
    int playerNumber;

    do {
        playerNumber = turn ? PLAYER1_NUMBER : PLAYER2_NUMBER;
        token = turn ? TOKEN_P1 : TOKEN_P2;
        printf("Player %d (%c) turn.\n", playerNumber, token);

        if (turn) {
            if (player1Type == HUMAN) {
                column = humanChoose(board, rows, columns);
            } else {
                column = computerChoose(board, rows, columns, TOKEN_P1, TOKEN_P2);

                printf("Computer chose column %d\n", column + 1);
            } 
        } else {
           if (player2Type == HUMAN) {
                column = humanChoose(board, rows, columns);
            } else {
                column = computerChoose(board, rows, columns, TOKEN_P2, TOKEN_P1);

                printf("Computer chose column %d\n", column + 1);
            } 
        }

        row = makeMove(board, rows, columns, column, token);

        printBoard(board, rows, columns);

        if (checkVictory(board, rows, columns, row, column, token)) {
            printf("Player %d (%c) wins!\n", playerNumber, token);

            break;
        }

        if (isBoardFull(board, rows, columns)) {
            printf("Board full and no winner. It's a tie!\n");

            break;
        }

        turn = turn == PLAYER1_TURN ? PLAYER2_TURN : PLAYER1_TURN;
    } while (1);
}