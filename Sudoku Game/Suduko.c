#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 9

// To track hints usage
int hintsRemaining;

// Timer-related variables
time_t startTime, endTime;

// Function to print the Sudoku board with clearer formatting
void printBoard(int board[SIZE][SIZE])
{
    printf("    1 2 3   4 5 6   7 8 9\n");
    printf("  +-------+-------+-------+\n");
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d | ", i + 1);
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] == 0)
            {
                printf(". "); // Display dots for empty cells
            }
            else
            {
                printf("%d ", board[i][j]);
            }
            if ((j + 1) % 3 == 0 && j != SIZE - 1)
            {
                printf("| ");
            }
        }
        printf("|\n");
        if ((i + 1) % 3 == 0 && i != SIZE - 1)
        {
            printf("  +-------+-------+-------+\n");
        }
    }
    printf("  +-------+-------+-------+\n");
}

// Check if it's safe to place a number
int isSafe(int board[SIZE][SIZE], int row, int col, int num)
{
    for (int x = 0; x < SIZE; x++)
    {
        if (board[row][x] == num || board[x][col] == num)
        {
            return 0;
        }
    }
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i + startRow][j + startCol] == num)
            {
                return 0;
            }
        }
    }
    return 1;
}

// Solve the Sudoku board
int solveSudoku(int board[SIZE][SIZE], int row, int col)
{
    if (row == SIZE - 1 && col == SIZE)
    {
        return 1;
    }
    if (col == SIZE)
    {
        row++;
        col = 0;
    }
    if (board[row][col] != 0)
    {
        return solveSudoku(board, row, col + 1);
    }
    for (int num = 1; num <= SIZE; num++)
    {
        if (isSafe(board, row, col, num))
        {
            board[row][col] = num;
            if (solveSudoku(board, row, col + 1))
            {
                return 1;
            }
            board[row][col] = 0;
        }
    }
    return 0;
}

// Randomly remove digits from the board based on difficulty
void removeDigits(int board[SIZE][SIZE], int level)
{
    int attempts = level * 10;
    while (attempts > 0)
    {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        if (board[row][col] != 0)
        {
            board[row][col] = 0;
            attempts--;
        }
    }
}

// Generate a random Sudoku puzzle
void generateSudoku(int board[SIZE][SIZE], int level)
{
    int baseBoard[SIZE][SIZE] = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}};

    // Prepare the board by copying the base
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = baseBoard[i][j];
        }
    }

    // Remove digits for the difficulty level
    removeDigits(board, level);
}

// Give hints to the player (limited based on difficulty)
void giveHint(int board[SIZE][SIZE], int solution[SIZE][SIZE])
{
    if (hintsRemaining > 0)
    {
        int row, col;
        do
        {
            row = rand() % SIZE;
            col = rand() % SIZE;
        } while (board[row][col] != 0);
        board[row][col] = solution[row][col];
        hintsRemaining--;
        printf("Hint provided at row %d, column %d.\n", row + 1, col + 1);
    }
    else
    {
        printf("No hints remaining!\n");
    }
}

// Function to play the Sudoku game
void playSudoku(int board[SIZE][SIZE], int solution[SIZE][SIZE])
{
    int row, col, num;
    startTime = time(NULL); // Start the timer

    while (1)
    {
        printBoard(board);
        printf("Enter row (1-9), column (1-9), and number (1-9) (0 to exit, -1 for hint): ");
        scanf("%d %d %d", &row, &col, &num);
        if (row == 0 || col == 0 || num == 0)
            break;
        if (row == -1)
        {
            giveHint(board, solution);
            continue;
        }
        if (row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9)
        {
            printf("Invalid input! Please try again.\n");
            continue;
        }
        if (isSafe(board, row - 1, col - 1, num))
        {
            board[row - 1][col - 1] = num;
        }
        else
        {
            printf("Invalid move. Try again.\n");
        }
    }

    endTime = time(NULL);
    int timeTaken = (int)difftime(endTime, startTime);
    printf("Time taken: %d seconds\n", timeTaken);
}

// Print the instructions for the user
void printInstructions()
{
    printf("Welcome to the Sudoku Game!\n");
    printf("Instructions:\n");
    printf("1. You will choose a difficulty level: Easy, Medium, or Hard.\n");
    printf("2. A Sudoku puzzle will be generated with some cells pre-filled.\n");
    printf("3. Your goal is to fill in the empty cells to complete the puzzle.\n");
    printf("4. To play, enter the row number (1-9), column number (1-9), and the number (1-9).\n");
    printf("5. You can exit by entering '0 0 0' or get hints by entering '-1 -1 -1'.\n");
    printf("6. Good luck and have fun!\n\n");
}

// Main function
int main()
{
    int board[SIZE][SIZE];
    int solution[SIZE][SIZE];
    int level;

    srand(time(0));
    printInstructions();
    printf("Select Difficulty:\n1. Easy (5 hints)\n2. Medium (3 hints)\n3. Hard (1 hint)\nChoose (1-3): ");
    scanf("%d", &level);

    // Set the number of hints
    switch (level)
    {
    case 1:
        hintsRemaining = 5;
        generateSudoku(board, 1);
        break;
    case 2:
        hintsRemaining = 3;
        generateSudoku(board, 2);
        break;
    case 3:
        hintsRemaining = 1;
        generateSudoku(board, 3);
        break;
    default:
        printf("Invalid choice. Exiting...\n");
        return 0;
    }

    // Save the solution for hint purposes
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            solution[i][j] = board[i][j];
    solveSudoku(solution, 0, 0);

    playSudoku(board, solution);
    printf("Thanks for playing!\n");
    return 0;
}
