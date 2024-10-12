#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_ATTEMPTS 6
#define WORD_LENGTH 100
#define MAX_WORDS 100

// Function to display the current state of the hangman
void displayHangman(int attemptsLeft)
{
    printf("\n");
    printf("_________\n");
    printf("|       |\n");
    printf("|       %c\n", (attemptsLeft < 6) ? 'O' : ' ');
    printf("|      %c%c%c\n", (attemptsLeft < 4) ? '/' : ' ', (attemptsLeft < 5) ? '|' : ' ', (attemptsLeft < 3) ? '\\' : ' ');
    printf("|      %c %c\n", (attemptsLeft < 2) ? '/' : ' ', (attemptsLeft < 1) ? '\\' : ' ');
    printf("|_________\n\n");
}

// Function to check if the word has been guessed
int isWordGuessed(char word[], int wordLen, int guessed[])
{
    for (int i = 0; i < wordLen; i++)
    {
        if (!guessed[i])
            return 0;
    }
    return 1;
}

// Function to process the player's guess
int processGuess(char word[], int wordLen, char guess, int guessed[])
{
    int correctGuess = 0;
    for (int i = 0; i < wordLen; i++)
    {
        if (tolower(word[i]) == tolower(guess))
        {
            guessed[i] = 1; // Mark the letter as guessed
            correctGuess = 1;
        }
    }
    return correctGuess;
}

// Function to play the hangman game
void playGame(char word[])
{
    int wordLen = strlen(word);
    int guessed[WORD_LENGTH] = {0};
    int attemptsLeft = MAX_ATTEMPTS;
    char guess;
    int correctGuesses = 0;

    printf("Welcome to Hangman! Try to guess the word.\n");

    // Main game loop
    while (attemptsLeft > 0)
    {
        displayHangman(attemptsLeft);
        printf("Word: ");

        // Display the current word status (guessed letters and underscores)
        for (int i = 0; i < wordLen; i++)
        {
            if (guessed[i])
            {
                printf("%c ", word[i]);
            }
            else
            {
                printf("_ ");
            }
        }
        printf("\n");

        // Ask the player for a guess
        printf("Enter a letter: ");
        scanf(" %c", &guess);

        // Process the guess
        if (processGuess(word, wordLen, guess, guessed))
        {
            printf("Correct guess!\n");
        }
        else
        {
            printf("Wrong guess! Attempts left: %d\n", --attemptsLeft);
        }

        // Check if the word is completely guessed
        if (isWordGuessed(word, wordLen, guessed))
        {
            printf("\nCongratulations! You've guessed the word '%s'!\n", word);
            return;
        }
    }

    // If out of attempts, player loses
    displayHangman(0);
    printf("\nGame Over! The word was '%s'.\n", word);
}

// Function to read words from a file
int readWordsFromFile(char *filename, char wordList[][WORD_LENGTH], int maxWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error: Could not open file %s.\n", filename);
        return 0;
    }

    int count = 0;
    while (count < maxWords && fscanf(file, "%s", wordList[count]) == 1)
    {
        count++;
    }

    fclose(file);
    return count;
}

// Main function with a simple menu and a word list from file
int main()
{
    char wordList[MAX_WORDS][WORD_LENGTH];
    int numWords;
    int choice;

    // Read words from file
    numWords = readWordsFromFile("words.txt", wordList, MAX_WORDS);
    if (numWords == 0)
    {
        printf("No words found. Exiting...\n");
        return 1;
    }

    // Seed the random number generator
    srand(time(0));

    // Game loop
    do
    {
        // Randomly select a word from the list
        char *word = wordList[rand() % numWords];

        // Play the game
        playGame(word);

        // Ask if the player wants to play again
        printf("\nDo you want to play again? (1 for Yes, 0 for No): ");
        scanf("%d", &choice);
    } while (choice == 1);

    printf("Thanks for playing Hangman!\n");
    return 0;
}
