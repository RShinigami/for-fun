#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX_SIZE 9
#define MAX_MINES 10

char board[MAX_SIZE][MAX_SIZE];
char display[MAX_SIZE][MAX_SIZE];
int size, mines;

// Struct to hold game settings
typedef struct
{
    int size;
    int mines;
    int timer;
} GameSettings;

GameSettings currentSettings;

// Function prototypes
void setupBoard();
void placeMines();
void calculateAdjacentMines();
void displayBoard();
int openCell(int row, int col);
int isGameWon();
void revealAllMines();
void saveScore(int timeTaken);
void showScoreboard();

void setupBoard()
{
    for (int i = 0; i < currentSettings.size; i++)
    {
        for (int j = 0; j < currentSettings.size; j++)
        {
            board[i][j] = '-';
            display[i][j] = '-';
        }
    }
    placeMines();
    calculateAdjacentMines();
}

void placeMines()
{
    int placedMines = 0;
    while (placedMines < currentSettings.mines)
    {
        int row = rand() % currentSettings.size;
        int col = rand() % currentSettings.size;
        if (board[row][col] != '*')
        {
            board[row][col] = '*';
            placedMines++;
        }
    }
}

void calculateAdjacentMines()
{
    for (int i = 0; i < currentSettings.size; i++)
    {
        for (int j = 0; j < currentSettings.size; j++)
        {
            if (board[i][j] == '*')
                continue;
            int mineCount = 0;
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    int newRow = i + x;
                    int newCol = j + y;
                    if (newRow >= 0 && newRow < currentSettings.size && newCol >= 0 && newCol < currentSettings.size)
                    {
                        if (board[newRow][newCol] == '*')
                            mineCount++;
                    }
                }
            }
            board[i][j] = mineCount + '0';
        }
    }
}

void displayBoard()
{
    printf("\n   ");
    for (int i = 0; i < currentSettings.size; i++)
    {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < currentSettings.size; i++)
    {
        printf("%d  ", i);
        for (int j = 0; j < currentSettings.size; j++)
        {
            printf("%c ", display[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int openCell(int row, int col)
{
    if (row < 0 || row >= currentSettings.size || col < 0 || col >= currentSettings.size || display[row][col] != '-')
    {
        return 0;
    }

    if (board[row][col] == '*')
    {
        revealAllMines();
        return -1;
    }

    display[row][col] = board[row][col];
    if (board[row][col] == '0')
    {
        for (int x = -1; x <= 1; x++)
        {
            for (int y = -1; y <= 1; y++)
            {
                openCell(row + x, col + y);
            }
        }
    }
    return 0;
}

int isGameWon()
{
    for (int i = 0; i < currentSettings.size; i++)
    {
        for (int j = 0; j < currentSettings.size; j++)
        {
            if (display[i][j] == '-' && board[i][j] != '*')
            {
                return 0;
            }
        }
    }
    return 1;
}

void revealAllMines()
{
    for (int i = 0; i < currentSettings.size; i++)
    {
        for (int j = 0; j < currentSettings.size; j++)
        {
            if (board[i][j] == '*')
            {
                display[i][j] = '*';
            }
        }
    }
}

void saveScore(int timeTaken)
{
    char playerName[30];
    printf("Enter your name: ");
    scanf("%s", playerName);

    FILE *file = fopen("scoreboard.txt", "a");
    if (file == NULL)
    {
        printf("Error opening scoreboard file.\n");
        return;
    }

    fprintf(file, "Player: %s, Time: %d seconds\n", playerName, timeTaken);
    fclose(file);
}

void showScoreboard()
{
    FILE *file = fopen("scoreboard.txt", "r");
    if (file == NULL)
    {
        printf("No scores recorded yet.\n");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }
    fclose(file);
}

int main()
{
    int row, col, gameStatus, levelChoice;
    char choice;

    srand(time(0));

    printf("Welcome to Minesweeper!\n");
    printf("Select difficulty level:\n1. Easy (5x5, 3 mines)\n2. Medium (7x7, 8 mines)\n3. Hard (9x9, 10 mines)\n");
    scanf("%d", &levelChoice);

    switch (levelChoice)
    {
    case 1:
        currentSettings.size = 5;
        currentSettings.mines = 3;
        break;
    case 2:
        currentSettings.size = 7;
        currentSettings.mines = 8;
        break;
    case 3:
        currentSettings.size = 9;
        currentSettings.mines = 10;
        break;
    default:
        printf("Invalid choice, defaulting to Easy.\n");
        currentSettings.size = 5;
        currentSettings.mines = 3;
        break;
    }

    setupBoard();

    clock_t startTime = clock();

    while (1)
    {
        displayBoard();
        printf("Enter row and column (e.g., 2 3): ");
        scanf("%d %d", &row, &col);

        gameStatus = openCell(row, col);
        if (gameStatus == -1)
        {
            displayBoard();
            printf("Game Over! You hit a mine!\n");
            break;
        }

        if (isGameWon())
        {
            displayBoard();
            printf("Congratulations! You've won the game!\n");
            clock_t endTime = clock();
            int timeTaken = (int)(endTime - startTime) / CLOCKS_PER_SEC;

            printf("You completed the game in %d seconds.\n", timeTaken);
            printf("Would you like to save your score? (y/n): ");
            scanf(" %c", &choice);

            if (tolower(choice) == 'y')
            {
                saveScore(timeTaken);
            }

            break;
        }
    }

    printf("\nWould you like to see the scoreboard? (y/n): ");
    scanf(" %c", &choice);
    if (tolower(choice) == 'y')
    {
        showScoreboard();
    }

    return 0;
}
