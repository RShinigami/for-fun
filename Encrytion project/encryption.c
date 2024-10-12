#include <stdio.h>
#include <string.h>

// Function to encrypt the message
void encryptMessage(char *message, int shift)
{
    for (int i = 0; message[i] != '\0'; i++)
    {
        char ch = message[i];
        if (ch >= 'a' && ch <= 'z')
        {
            ch = ((ch - 'a' + shift) % 26) + 'a';
        }
        else if (ch >= 'A' && ch <= 'Z')
        {
            ch = ((ch - 'A' + shift) % 26) + 'A';
        }
        message[i] = ch;
    }
}

// Function to decrypt the message
void decryptMessage(char *message, int shift)
{
    for (int i = 0; message[i] != '\0'; i++)
    {
        char ch = message[i];
        if (ch >= 'a' && ch <= 'z')
        {
            ch = ((ch - 'a' - shift + 26) % 26) + 'a';
        }
        else if (ch >= 'A' && ch <= 'Z')
        {
            ch = ((ch - 'A' - shift + 26) % 26) + 'A';
        }
        message[i] = ch;
    }
}

// Function to read from input file and write to output file
void processFile(char *inputFile, char *outputFile, int shift, int mode)
{
    FILE *inFile = fopen(inputFile, "r");
    FILE *outFile = fopen(outputFile, "w");
    char line[256];

    if (inFile == NULL || outFile == NULL)
    {
        printf("Error: Could not open input or output file.\n");
        return;
    }

    while (fgets(line, sizeof(line), inFile))
    {
        // Remove newline character from each line
        line[strcspn(line, "\n")] = 0;

        if (mode == 1)
        {
            encryptMessage(line, shift);
        }
        else if (mode == 2)
        {
            decryptMessage(line, shift);
        }

        fprintf(outFile, "%s\n", line);
    }

    fclose(inFile);
    fclose(outFile);
    printf("File processed successfully!\n");
}

int main()
{
    int choice, shift = -1;
    char inputFile[100], outputFile[100];

    while (1)
    {
        printf("\n1. Encrypt a File\n");
        printf("2. Decrypt a File\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            printf("Enter the input file name: ");
            scanf("%s", inputFile);
            printf("Enter the output file name: ");
            scanf("%s", outputFile);
            printf("Enter a shift value (1-25): ");
            scanf("%d", &shift);

            processFile(inputFile, outputFile, shift, 1);
        }
        else if (choice == 2)
        {
            if (shift == -1)
            {
                printf("You must encrypt a file first before decrypting.\n");
            }
            else
            {
                printf("Enter the input file name: ");
                scanf("%s", inputFile);
                printf("Enter the output file name: ");
                scanf("%s", outputFile);

                processFile(inputFile, outputFile, shift, 2);
            }
        }
        else if (choice == 3)
        {
            printf("Exiting...\n");
            break;
        }
        else
        {
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
