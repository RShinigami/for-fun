#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 9
#define MINES 10

char board[SIZE][SIZE];        
char hiddenBoard[SIZE][SIZE];  
int revealedCells = 0;


void initializeBoards();
void placeMines();
void calculateHints();
void displayBoard();
int isValid(int row, int col);
void revealCell(int row, int col);
int playGame();

int main() {
    int result;
    initializeBoards();
    placeMines();
    calculateHints();

    printf("Welcome to Minesweeper!\n");

    
    result = playGame();

    if (result == 1) {
        printf("Congratulations! You've cleared the minefield!\n");
    } else {
        printf("You hit a mine! Game Over!\n");
        
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (hiddenBoard[i][j] == '*') {
                    board[i][j] = '*';
                }
            }
        }
        displayBoard();
    }

    return 0;
}


void initializeBoards() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = '-';         
            hiddenBoard[i][j] = '-';   
        }
    }
}


void placeMines() {
    int count = 0;
    int row, col;
    srand(time(0));

    while (count < MINES) {
        row = rand() % SIZE;
        col = rand() % SIZE;

        if (hiddenBoard[row][col] != '*') {
            hiddenBoard[row][col] = '*';
            count++;
        }
    }
}

void calculateHints() {
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (hiddenBoard[i][j] == '*') continue;

            int minesCount = 0;
            for (int d = 0; d < 8; d++) {
                int newRow = i + dx[d];
                int newCol = j + dy[d];

                if (isValid(newRow, newCol) && hiddenBoard[newRow][newCol] == '*') {
                    minesCount++;
                }
            }
            hiddenBoard[i][j] = minesCount + '0'; 
        }
    }
}


int isValid(int row, int col) {
    return (row >= 0 && row < SIZE && col >= 0 && col < SIZE);
}


void displayBoard() {
    printf("  ");
    for (int i = 0; i < SIZE; i++) printf("%d ", i);
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}


void revealCell(int row, int col) {
    if (!isValid(row, col) || board[row][col] != '-') return;

    board[row][col] = hiddenBoard[row][col];
    revealedCells++;

    if (hiddenBoard[row][col] == '0') {
        int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
        for (int d = 0; d < 8; d++) {
            int newRow = row + dx[d];
            int newCol = col + dy[d];
            revealCell(newRow, newCol);
        }
    }
}


int playGame() {
    int row, col;

    while (revealedCells < SIZE * SIZE - MINES) {
        displayBoard();
        printf("Enter row and column to reveal (e.g., 2 3): ");
        scanf("%d %d", &row, &col);

        if (!isValid(row, col)) {
            printf("Invalid input. Try again.\n");
            continue;
        }

        if (hiddenBoard[row][col] == '*') {
            return 0; 
        }

        revealCell(row, col);
    }

    return 1; 
}
