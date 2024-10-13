#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ROWS 9
#define COLS 9
#define MINES_EASY 10
#define MINES_MEDIUM 20
#define MINES_HARD 30
#define MAX_SCORES 100

typedef struct {
    char name[50];
    int time;
    char difficulty[10];
} Score;

Score scores[MAX_SCORES];
int scoreCount = 0;

void displayBoard(char board[ROWS][COLS], int revealMines) {
    printf("   ");
    for (int i = 0; i < COLS; i++) {
        printf("%2d ", i);
    }
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        printf("%2d ", i);
        for (int j = 0; j < COLS; j++) {
            if (revealMines) {
                printf("%2c ", board[i][j]);
            } else {
                printf("%2c ", (board[i][j] == '*') ? '-' : board[i][j]);
            }
        }
        printf("\n");
    }
}

void initializeBoard(char board[ROWS][COLS], int mineCount) {
    srand(time(NULL));
    memset(board, '-', sizeof(char) * ROWS * COLS);

    for (int i = 0; i < mineCount; i++) {
        int row, col;
        do {
            row = rand() % ROWS;
            col = rand() % COLS;
        } while (board[row][col] == '*');
        board[row][col] = '*';
    }
}

int isMine(char board[ROWS][COLS], int row, int col) {
    return board[row][col] == '*';
}

void revealCell(char board[ROWS][COLS], char display[ROWS][COLS], int row, int col) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS || display[row][col] != '-') return;

    int mineCount = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int newRow = row + i, newCol = col + j;
            if (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < COLS && isMine(board, newRow, newCol)) {
                mineCount++;
            }
        }
    }

    if (mineCount > 0) {
        display[row][col] = '0' + mineCount;
    } else {
        display[row][col] = ' ';
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i != 0 || j != 0) {
                    revealCell(board, display, row + i, col + j);
                }
            }
        }
    }
}

int checkWin(char display[ROWS][COLS], int totalMines) {
    int unrevealed = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (display[i][j] == '-') unrevealed++;
        }
    }
    return unrevealed == totalMines;
}

void loadScores() {
    FILE *file = fopen("scores.txt", "r");
    if (file == NULL) return;

    scoreCount = 0;
    while (fscanf(file, "Player: %[^,], Time: %d seconds , Difficulty : %s\n", scores[scoreCount].name, &scores[scoreCount].time, scores[scoreCount].difficulty) == 3) {
        scoreCount++;
    }
    fclose(file);
}

void saveScore(int time, const char* difficulty) {
    char name[50];
    printf("Enter your name: ");
    scanf("%s", name);

    FILE *file = fopen("scores.txt", "a");
    if (file == NULL) {
        printf("Unable to save score.\n");
        return;
    }
    fprintf(file, "Player: %s, Time: %d seconds , Difficulty : %s\n", name, time, difficulty);
    fclose(file);
}

void showScoreboard() {
    loadScores();
    if (scoreCount == 0) {
        printf("No scores to display.\n");
        return;
    }

    printf("\n--- Scoreboard ---\n");
    for (int i = 0; i < scoreCount; i++) {
        printf("Player: %s, Time: %d seconds , Difficulty : %s\n", scores[i].name, scores[i].time, scores[i].difficulty);
    }
    printf("------------------\n");
}

void playGame(int mineCount, const char* difficulty) {
    char board[ROWS][COLS];
    char display[ROWS][COLS];
    int gameOver = 0;

    initializeBoard(board, mineCount);
    memset(display, '-', sizeof(char) * ROWS * COLS);

    clock_t startTime = clock();

    while (!gameOver) {
        displayBoard(display, 0);
        int row, col;
        printf("Enter row and column: ");
        scanf("%d %d", &row, &col);

        if (isMine(board, row, col)) {
            printf("Game Over! You hit a mine.\n");
            displayBoard(board, 1);
            break;
        }

        revealCell(board, display, row, col);

        if (checkWin(display, mineCount)) {
            clock_t endTime = clock();
            int timeTaken = (endTime - startTime) / CLOCKS_PER_SEC;
            printf("Congratulations! You won in %d seconds.\n", timeTaken);
            displayBoard(board, 1);
            char choice;
            printf("Do you want to save your score? (y/n): ");
            scanf(" %c", &choice);

            if (choice == 'y' || choice == 'Y') {
                saveScore(timeTaken, difficulty);
            }

            showScoreboard();
            break;
        }
    }
}

int main() {
    int choice;
    do {
        printf("\n--- Minesweeper Game ---\n");
        printf("1. Play Game\n");
        printf("2. Show Scoreboard\n");
        printf("3. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int difficultyChoice;
                printf("Select Difficulty: \n1. Easy\n2. Medium\n3. Hard\n");
                scanf("%d", &difficultyChoice);

                int mines = 0;
                char difficulty[10];
                if (difficultyChoice == 1) {
                    mines = MINES_EASY;
                    strcpy(difficulty, "Easy");
                } else if (difficultyChoice == 2) {
                    mines = MINES_MEDIUM;
                    strcpy(difficulty, "Medium");
                } else if (difficultyChoice == 3) {
                    mines = MINES_HARD;
                    strcpy(difficulty, "Hard");
                } else {
                    printf("Invalid difficulty choice.\n");
                    continue;
                }

                playGame(mines, difficulty);
                break;
            }
            case 2:
                showScoreboard();
                break;
            case 3:
                printf("Exiting the game. Thanks for playing!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);

    return 0;
}
