#include <stdio.h>
#include <stdlib.h>

char board[3][3];
int player1Wins = 0, player2Wins = 0, draws = 0;

// Function to initialize the board with numbers 1 to 9
void initializeBoard() {
    int num = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = num + '0';  // Store as characters '1' to '9'
            num++;
        }
    }
}

// Function to print the current state of the board
void printBoard() {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" %c ", board[i][j]);
            if (j < 2) printf("|");
        }
        if (i < 2) printf("\n---|---|---\n");
    }
    printf("\n");
}

// Function to check if a player has won
int checkWin() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) return 1;
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) return 1;
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) return 1;
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) return 1;
    return 0;
}

// Function to check if the board is full (draw)
int checkDraw() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') return 0;
        }
    }
    return 1;
}

// Function to update the board based on player's move
int updateBoard(int choice, char mark) {
    int row = (choice - 1) / 3;
    int col = (choice - 1) % 3;
    if (board[row][col] != 'X' && board[row][col] != 'O') {
        board[row][col] = mark;
        return 1;
    } else {
        printf("Invalid move! Try again.\n");
        return 0;
    }
}

// Function to load scores from the file
void loadScores() {
    FILE *file = fopen("scores.txt", "r");
    if (file == NULL) {
        // If file doesn't exist, initialize scores to 0
        player1Wins = 0;
        player2Wins = 0;
        draws = 0;
    } else {
        fscanf(file, "%d %d %d", &player1Wins, &player2Wins, &draws);
        fclose(file);
    }
}

// Function to save scores to the file
void saveScores() {
    FILE *file = fopen("scores.txt", "w");
    if (file == NULL) {
        printf("Error saving scores!\n");
        return;
    }
    fprintf(file, "%d %d %d", player1Wins, player2Wins, draws);
    fclose(file);
}

// Function to reset scores
void resetScores() {
    player1Wins = 0;
    player2Wins = 0;
    draws = 0;
    saveScores();
    printf("Scores have been reset!\n");
}

// Function to display scores
void displayScores() {
    printf("Current Scores:\n");
    printf("Player 1 Wins: %d\n", player1Wins);
    printf("Player 2 Wins: %d\n", player2Wins);
    printf("Draws: %d\n", draws);
}

// Function to play the game
void playGame() {
    int player = 1, choice;
    char mark;
    int gameOver = 0;

    initializeBoard();
    while (!gameOver) {
        printBoard();
        player = (player % 2) ? 1 : 2;
        mark = (player == 1) ? 'X' : 'O';
        printf("Player %d, enter your move (1-9): ", player);
        scanf("%d", &choice);
        if (updateBoard(choice, mark)) {
            if (checkWin()) {
                printBoard();
                printf("Player %d wins!\n", player);
                if (player == 1) player1Wins++;
                else player2Wins++;
                gameOver = 1;
            } else if (checkDraw()) {
                printBoard();
                printf("It's a draw!\n");
                draws++;
                gameOver = 1;
            }
            player++;
        }
    }
    saveScores(); 
}

// Main menu
void displayMenu() {
    int choice;
    do {
        printf("\nTic-Tac-Toe Menu\n");
        printf("1. Play Game\n");
        printf("2. View Scores\n");
        printf("3. Reset Scores\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                displayScores();
                break;
            case 3:
                resetScores();
                break;
            case 4:
                printf("Exiting the game.\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 4);
}

int main() {
    loadScores();  
    displayMenu();  
    return 0;
}
