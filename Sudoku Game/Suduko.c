#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 9

// To track hints usage
int hintsRemaining;

// Timer-related variables
time_t startTime, endTime;

// Function prototypes
void printBoard(int board[SIZE][SIZE]);
int isSafe(int board[SIZE][SIZE], int row, int col, int num);
int solveSudoku(int board[SIZE][SIZE], int row, int col);
void removeDigits(int board[SIZE][SIZE], int level);
void generateSudoku(int board[SIZE][SIZE], int level);
void giveHint(int board[SIZE][SIZE], int solution[SIZE][SIZE]);
int isPuzzleSolved(int board[SIZE][SIZE], int solution[SIZE][SIZE]);
void saveScore(char* name, int timeTaken, int level);
void handleWin(int timeTaken, int level);
void displayScoreboard();
void saveGameState(int board[SIZE][SIZE], int level, int hintsRemaining, int elapsedTime);
int loadGameState(int board[SIZE][SIZE], int *level, int *hintsRemaining, int *elapsedTime);
void playSudoku();
void showMainMenu();
void showInstructions();

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

// Function to check if the Sudoku board is fully and correctly completed
int isPuzzleSolved(int board[SIZE][SIZE], int solution[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] != solution[i][j])
            {
                return 0; // Puzzle is not solved correctly
            }
        }
    }
    return 1; // Puzzle is solved correctly
}

// Save the score to a file
void saveScore(char* name, int timeTaken, int level)
{
    FILE *file = fopen("scores.txt", "a");
    if (file == NULL)
    {
        printf("Error: Unable to open file to save score.\n");
        return;
    }

    char* difficulty;
    switch (level)
    {
        case 1:
            difficulty = "Easy";
            break;
        case 2:
            difficulty = "Medium";
            break;
        case 3:
            difficulty = "Hard";
            break;
        default:
            difficulty = "Unknown";
    }

    fprintf(file, "Player: %s, Time: %d seconds, Difficulty: %s\n", name, timeTaken, difficulty);
    fclose(file);
    printf("Score saved successfully!\n");
}

// Handle winning scenario, display time and ask to save score
void handleWin(int timeTaken, int level)
{
    char name[50];
    char saveOption;

    printf("\nCongratulations! You've completed the puzzle!\n");
    printf("Time taken: %d seconds\n", timeTaken);

    printf("Do you want to save your score? (y/n): ");
    scanf(" %c", &saveOption);

    if (saveOption == 'y' || saveOption == 'Y')
    {
        printf("Enter your name: ");
        scanf("%s", name);

        saveScore(name, timeTaken, level);
    }
}

// Display the scoreboard
void displayScoreboard()
{
    FILE *file = fopen("scores.txt", "r");
    if (file == NULL)
    {
        printf("No scores available to display.\n");
        return;
    }

    char line[100];
    printf("\n--- Scoreboard ---\n");
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }
    fclose(file);
    printf("\n");
}

// Save the current game state
void saveGameState(int board[SIZE][SIZE], int level, int hintsRemaining, int elapsedTime)
{
    FILE *file = fopen("saved_game.dat", "wb");
    if (file == NULL)
    {
        printf("Error: Unable to save game.\n");
        return;
    }

    fwrite(&level, sizeof(int), 1, file);
    fwrite(&hintsRemaining, sizeof(int), 1, file);
    fwrite(&elapsedTime, sizeof(int), 1, file);
    fwrite(board, sizeof(int), SIZE * SIZE, file);

    fclose(file);
    printf("Game saved successfully!\n");
}

// Load a saved game state
int loadGameState(int board[SIZE][SIZE], int *level, int *hintsRemaining, int *elapsedTime)
{
    FILE *file = fopen("saved_game.dat", "rb");
    if (file == NULL)
    {
        return 0; // No saved game
    }

    fread(level, sizeof(int), 1, file);
    fread(hintsRemaining, sizeof(int), 1, file);
    fread(elapsedTime, sizeof(int), 1, file);
    fread(board, sizeof(int), SIZE * SIZE, file);

    fclose(file);
    return 1; // Successfully loaded
}

// Main game play function
void playSudoku(int board[SIZE][SIZE], int solution[SIZE][SIZE], int level, int loaded, int hintsRemaining, time_t startTime)
{
    int row, col, num;
    char action;

    if (!loaded) {
        hintsRemaining = 3; // Initial number of hints based on difficulty level
        time(&startTime);
        generateSudoku(board, level);
        
        // Create the solution board by solving it
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                solution[i][j] = board[i][j];
            }
        }
        solveSudoku(solution, 0, 0);
    }

    printf("Here is your Sudoku puzzle:\n");
    printBoard(board);

    while (1)
    {
        printf("\nChoose action (e.g. E 3 5 9 to Enter 9 at (3,5); H for Hint; S to Save, Q to Quit): ");
        scanf(" %c", &action);

        if (action == 'H' || action == 'h')
        {
            giveHint(board, solution);
        }
        else if (action == 'S' || action == 's')
        {
            time(&endTime);
            saveGameState(board, level, hintsRemaining, difftime(endTime, startTime));
        }
        else if (action == 'Q' || action == 'q')
        {
            return; // Exit the game
        }
        else if (action == 'E' || action == 'e')
        {
            scanf("%d %d %d", &row, &col, &num);
            if (row < 1 || row > SIZE || col < 1 || col > SIZE || num < 1 || num > SIZE)
            {
                printf("Invalid input. Please try again.\n");
            }
            else if (board[row - 1][col - 1] != 0)
            {
                printf("This cell is not empty. Please choose an empty cell.\n");
            }
            else if (isSafe(board, row - 1, col - 1, num))
            {
                board[row - 1][col - 1] = num;
                printBoard(board);

                if (isPuzzleSolved(board, solution))
                {
                    time(&endTime);
                    int timeTaken = difftime(endTime, startTime);
                    handleWin(timeTaken, level);

                    // Ask if the player wants to play again
                    char replayOption;
                    printf("Do you want to play again? (y/n): ");
                    scanf(" %c", &replayOption);

                    if (replayOption == 'y' || replayOption == 'Y')
                    {
                        int newBoard[SIZE][SIZE];
                        int newSolution[SIZE][SIZE];
                        playSudoku(newBoard, newSolution, level, 0, hintsRemaining, startTime); // Start a new game
                    }
                    else
                    {
                        return; // End the game
                    }
                }
            }
            else
            {
                printf("Incorrect entry! This number cannot be placed here.\n");
            }
        }
        else
        {
            printf("Invalid action. Please try again.\n");
        }
    }
}

// Main menu function
void showMainMenu()
{
    int choice;

    do
    {
        printf("\n--- Sudoku Game ---\n");
        printf("1. Play Sudoku\n");
        printf("2. View Scoreboard\n");
        printf("3. Load Previous Game\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        int board[SIZE][SIZE];
        int solution[SIZE][SIZE];
        int level, hintsRemaining, elapsedTime = 0;
        time_t startTime;

        switch (choice)
        {
            case 1:
                printf("Choose difficulty level (1-Easy, 2-Medium, 3-Hard): ");
                scanf("%d", &level);
                playSudoku(board, solution, level, 0, hintsRemaining, startTime);
                break;
            case 2:
                displayScoreboard();
                break;
            case 3:
                if (loadGameState(board, &level, &hintsRemaining, &elapsedTime))
                {
                    printf("Loaded saved game found. Resuming...\n");
                    time(&startTime);
                    startTime -= elapsedTime; // Adjust timer
                    playSudoku(board, solution, level, 1, hintsRemaining, startTime); // Resume play
                }
                else
                {
                    printf("No saved game found.\n");
                }
                break;
            case 4:
                printf("Exiting game. Thank you for playing!\n");
                break;
            default:
                printf("Invalid choice. Please choose again.\n");
        }
    } while (choice != 4);
}

// Show instructions to the player
void showInstructions()
{
    printf("\nWelcome to the Sudoku game!\n");
    printf("1. You can choose a difficulty level: Easy, Medium, or Hard.\n");
    printf("2. You will be given a board with some cells filled in.\n");
    printf("3. Your goal is to fill the board so that each row, column, and 3x3 box contains the numbers 1 to 9.\n");
    printf("4. Use the commands shown on the game screen to enter your moves or take actions.\n");
    printf("5. You can save your progress or use hints if you get stuck.\n");
    printf("6. Complete the puzzle to win.\n");
    printf("Good luck!\n");
}

// Main function
int main()
{
    srand(time(0));
    showInstructions();
    showMainMenu();

    return 0;
}
