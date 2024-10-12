#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LENGTH 50
#define SUBJECT_COUNT 5

typedef struct
{
    char name[NAME_LENGTH];
    int id;
    float grades[SUBJECT_COUNT];
    float average;
} Student;

// Coefficients for each subject
const int coefficients[SUBJECT_COUNT] = {2, 2, 1, 1, 1};
const char *subjects[SUBJECT_COUNT] = {"Math", "Science", "Programming", "English", "Algorithms"};

// Function to calculate the weighted average
void calculateAverage(Student *student)
{
    float sum = 0;
    int totalCoefficients = 0;
    for (int i = 0; i < SUBJECT_COUNT; i++)
    {
        sum += student->grades[i] * coefficients[i];
        totalCoefficients += coefficients[i];
    }
    student->average = sum / totalCoefficients;
}

// Function to validate grades
int validateGrade(float grade)
{
    return (grade >= 0 && grade <= 20);
}

// Function to add a student
void addStudent(Student students[], int *count)
{
    if (*count >= MAX_STUDENTS)
    {
        printf("Maximum student limit reached!\n");
        return;
    }
    Student newStudent;
    printf("Enter student ID: ");
    scanf("%d", &newStudent.id);
    printf("Enter student name: ");
    scanf(" %[^\n]", newStudent.name);

    // Input grades with validation
    for (int i = 0; i < SUBJECT_COUNT; i++)
    {
        float grade;
        do
        {
            printf("%s : ", subjects[i]);
            scanf("%f", &grade);
            if (!validateGrade(grade))
            {
                printf("Invalid grade! Please enter a grade between 0 and 20.\n");
            }
        } while (!validateGrade(grade));
        newStudent.grades[i] = grade;
    }

    calculateAverage(&newStudent);
    students[(*count)++] = newStudent;
    printf("Student added successfully!\n");
}

// Function to display all students
void displayStudents(Student students[], int count)
{
    printf("\n%-10s %-20s %-10s %-10s %-10s %-10s %-10s %-10s\n", "ID", "Name", "Math", "Science", "Prog", "English", "Algo", "Average");
    printf("-----------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++)
    {
        printf("%-10d %-20s ", students[i].id, students[i].name);
        for (int j = 0; j < SUBJECT_COUNT; j++)
        {
            printf("%-10.2f ", students[i].grades[j]);
        }
        printf("%-10.2f\n", students[i].average);
    }
}

// Function to save students to a file
void saveStudentsToFile(Student students[], int count)
{
    FILE *file = fopen("students.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%d %s %.2f %.2f %.2f %.2f %.2f\n",
                students[i].id, students[i].name,
                students[i].grades[0], students[i].grades[1],
                students[i].grades[2], students[i].grades[3],
                students[i].grades[4]);
    }
    fclose(file);
    printf("Students saved to file successfully!\n");
}

// Function to load students from a file
int loadStudentsFromFile(Student students[])
{
    FILE *file = fopen("students.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return 0;
    }
    int count = 0;
    while (fscanf(file, "%d %s %f %f %f %f %f",
                  &students[count].id, students[count].name,
                  &students[count].grades[0], &students[count].grades[1],
                  &students[count].grades[2], &students[count].grades[3],
                  &students[count].grades[4]) == 7)
    {
        calculateAverage(&students[count]);
        count++;
    }
    fclose(file);
    return count;
}

// Function to update a student's grades
void updateStudent(Student students[], int *count)
{
    int id;
    printf("Enter the student ID to update: ");
    scanf("%d", &id);

    for (int i = 0; i < *count; i++)
    {
        if (students[i].id == id)
        {
            printf("Updating grades for %s (ID: %d)\n", students[i].name, id);
            for (int j = 0; j < SUBJECT_COUNT; j++)
            {
                float grade;
                do
                {
                    printf("%s (0-20): ", subjects[j]);
                    scanf("%f", &grade);
                    if (!validateGrade(grade))
                    {
                        printf("Invalid grade! Please enter a grade between 0 and 20.\n");
                    }
                } while (!validateGrade(grade));
                students[i].grades[j] = grade;
            }
            calculateAverage(&students[i]);
            printf("Student updated successfully!\n");
            saveStudentsToFile(students, *count); // Save changes to file
            return;
        }
    }
    printf("Student with ID %d not found.\n", id);
}

// Function to delete a student by ID
void deleteStudent(Student students[], int *count)
{
    int id;
    printf("Enter the student ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < *count; i++)
    {
        if (students[i].id == id)
        {
            for (int j = i; j < *count - 1; j++)
            {
                students[j] = students[j + 1];
            }
            (*count)--;
            printf("Student with ID %d has been deleted successfully!\n", id);
            saveStudentsToFile(students, *count); // Save changes to file
            return;
        }
    }
    printf("Student with ID %d not found.\n", id);
}

int main()
{
    Student students[MAX_STUDENTS];
    int count = 0, choice;

    // Load existing students from file
    count = loadStudentsFromFile(students);

    do
    {
        printf("\n1. Add Student\n2. Display Students\n3. Update Student\n4. Delete Student\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            addStudent(students, &count);
            saveStudentsToFile(students, count);
            break;
        case 2:
            displayStudents(students, count);
            break;
        case 3:
            updateStudent(students, &count);
            break;
        case 4:
            deleteStudent(students, &count);
            break;
        case 5:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}
