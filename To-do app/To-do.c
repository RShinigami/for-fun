#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100
#define TASK_LENGTH 100
#define FILENAME "tasks.txt"

typedef struct {
    char description[TASK_LENGTH];
    int isComplete;
} Task;

Task tasks[MAX_TASKS];
int taskCount = 0;

// Function prototypes
void loadTasks();
void saveTasks();
void addTask();
void viewTasks();
void markTask();
void deleteTask();

int main() {
    int choice;
    
    // Load tasks from file at startup
    loadTasks();

    do {
        printf("\nTo-Do List Application\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Mark Task as Complete/Incomplete\n");
        printf("4. Delete Task\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addTask();
                break;
            case 2:
                viewTasks();
                break;
            case 3:
                markTask();
                break;
            case 4:
                deleteTask();
                break;
            case 0:
                saveTasks();
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}

// Function to load tasks from file
void loadTasks() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        return; // No file to load
    }

    while (fscanf(file, "%[^\n]\n%d\n", tasks[taskCount].description, &tasks[taskCount].isComplete) != EOF) {
        taskCount++;
    }

    fclose(file);
}

// Function to save tasks to file
void saveTasks() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error: Could not save tasks.\n");
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "%s\n%d\n", tasks[i].description, tasks[i].isComplete);
    }

    fclose(file);
}

// Function to add a new task
void addTask() {
    if (taskCount >= MAX_TASKS) {
        printf("Task list is full.\n");
        return;
    }

    printf("Enter task description: ");
    getchar(); // To consume leftover newline
    fgets(tasks[taskCount].description, TASK_LENGTH, stdin);
    tasks[taskCount].description[strcspn(tasks[taskCount].description, "\n")] = 0; // Remove newline
    tasks[taskCount].isComplete = 0;
    taskCount++;
    printf("Task added successfully.\n");
}

// Function to view all tasks
void viewTasks() {
    if (taskCount == 0) {
        printf("No tasks to display.\n");
        return;
    }

    for (int i = 0; i < taskCount; i++) {
        printf("%d. %s [%s]\n", i + 1, tasks[i].description, tasks[i].isComplete ? "Complete" : "Incomplete");
    }
}

// Function to mark a task as complete or incomplete
void markTask() {
    int taskNum;
    viewTasks();

    if (taskCount == 0) return;

    printf("Enter task number to mark complete/incomplete: ");
    scanf("%d", &taskNum);

    if (taskNum < 1 || taskNum > taskCount) {
        printf("Invalid task number.\n");
        return;
    }

    tasks[taskNum - 1].isComplete = !tasks[taskNum - 1].isComplete;
    printf("Task updated.\n");
}

// Function to delete a task
void deleteTask() {
    int taskNum;
    viewTasks();

    if (taskCount == 0) return;

    printf("Enter task number to delete: ");
    scanf("%d", &taskNum);

    if (taskNum < 1 || taskNum > taskCount) {
        printf("Invalid task number.\n");
        return;
    }

    for (int i = taskNum - 1; i < taskCount - 1; i++) {
        tasks[i] = tasks[i + 1];
    }

    taskCount--;
    printf("Task deleted.\n");
}
