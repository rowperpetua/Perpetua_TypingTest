#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_USERS 10
#define MAX_NAME_LENGTH 50
#define MAX_TEXT_LENGTH 1000
#define USER_FILE "users.txt"

typedef struct {
    char name[MAX_NAME_LENGTH];
    int typingSpeed;
    float accuracy;
    char studentNumber[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
} User;

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void createNewUser(User users[], int *userCount) {
    if (*userCount >= MAX_USERS) {
        printf("Maximum number of users reached.\n");
        return;
    }

    User newUser;
    printf("\n\n\n\n\t\tEnter the name of the new user and course(ex. Chloe_BSIS): ");
    fgets(newUser.name, MAX_NAME_LENGTH, stdin);
    newUser.name[strcspn(newUser.name, "\n")] = '\0'; // Remove trailing newline

    printf("\n\t\tEnter the student number: ");
    fgets(newUser.studentNumber, MAX_NAME_LENGTH, stdin);
    newUser.studentNumber[strcspn(newUser.studentNumber, "\n")] = '\0'; // Remove trailing newline

    printf("\n\t\tCreate a password: ");
    fgets(newUser.password, MAX_NAME_LENGTH, stdin);
    newUser.password[strcspn(newUser.password, "\n")] = '\0'; // Remove trailing newline

    users[*userCount] = newUser;
    (*userCount)++;
    printf("\n\t\tUser '%s' created successfully.\n", newUser.name);
    saveUsersToFile(users, *userCount);
}

void login(User users[], int userCount, User **currentUser) {
    char studentNumber[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];

    printf("Enter student number: ");
    fgets(studentNumber, MAX_NAME_LENGTH, stdin);
    studentNumber[strcspn(studentNumber, "\n")] = '\0'; // Remove trailing newline

    printf("Enter password: ");
    fgets(password, MAX_NAME_LENGTH, stdin);
    password[strcspn(password, "\n")] = '\0'; // Remove trailing newline

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].studentNumber, studentNumber) == 0 && strcmp(users[i].password, password) == 0) {
            *currentUser = &users[i];
            printf("Login successful. Welcome, %s!\n", (*currentUser)->name);
            return;
        }
    }

    printf("Invalid student number or password. Login failed.\n");
}

void displayStatistics(User users[], int userCount) {
    if (userCount == 0) {
        printf("No users found.\n");
        return;
    }

    int maxAccuracyIndex = 0;
    int maxTypingSpeedIndex = 0;
    for (int i = 1; i < userCount; i++) {
        if (users[i].accuracy > users[maxAccuracyIndex].accuracy) {
            maxAccuracyIndex = i;
        }
        if (users[i].typingSpeed > users[maxTypingSpeedIndex].typingSpeed) {
            maxTypingSpeedIndex = i;
        }
    }

    printf("=== User Statistics ===\n");
    for (int i = 0; i < userCount; i++) {
        printf("User: %s\n", users[i].name);
        printf("Typing Speed: %d WPM\n", users[i].typingSpeed);
        printf("Accuracy: %.2f%%\n", users[i].accuracy);
        printf("-----------------------\n");
    }

    printf("Top User (Accuracy): %s\n", users[maxAccuracyIndex].name);
    printf("Top Accuracy: %.2f%%\n", users[maxAccuracyIndex].accuracy);
    printf("Top User (Typing Speed): %s\n", users[maxTypingSpeedIndex].name);
    printf("Top Typing Speed: %d WPM\n", users[maxTypingSpeedIndex].typingSpeed);
}


void startTypingTest(User *currentUser) {

    system("cls");
    printf("Choose the typing test option:\n");
    printf("1. Custom Phrase\n");
    printf("2. Randomly Generated Phrase\n");
    printf("Enter your choice: ");

    int choice;
    scanf("%d", &choice);
    clearInputBuffer();

    char text[MAX_TEXT_LENGTH];

    switch (choice) {
        case 1:
            printf("Enter the custom phrase for typing: ");
            fgets(text, MAX_TEXT_LENGTH, stdin);
            break;
        case 2:
            generateRandomPhrase(text, MAX_TEXT_LENGTH);
            printf("Randomly Generated Phrase: %s\n", text);
            break;
        default:
            printf("Invalid choice. Exiting typing test.\n");
            return;
    }

    printf("Type the following text:\n%s\n", text);

    time_t startTime = time(NULL);

    int typedChars = 0;
    int incorrectChars = 0;

    for (int i = 0; i < strlen(text); i++) {
        char inputChar = getchar();
        if (inputChar == '\n') {
            break;
        }

        if (inputChar != text[i]) {
            incorrectChars++;
        }
        typedChars++;
    }

    time_t endTime = time(NULL);
    double elapsedTime = difftime(endTime, startTime);

    float accuracy = 100.0 - (float)(incorrectChars * 100) / typedChars;
    int typingSpeed = (int)((typedChars / 5) / (elapsedTime / 60.0));

    currentUser->accuracy = accuracy;
    currentUser->typingSpeed = typingSpeed;

    printf("\nTest completed.\n");
    printf("Typed Characters: %d\n", typedChars);
    printf("Incorrect Characters: %d\n", incorrectChars);
    printf("Elapsed Time: %.2f seconds\n", elapsedTime);
    printf("Typing Speed: %d WPM\n", typingSpeed);
    printf("Accuracy: %.2f%%\n", accuracy);
    saveUsersToFile(currentUser, 1);
}

void loadUsersFromFile(User users[], int *userCount) {
    FILE *file = fopen(USER_FILE, "r");
    if (file == NULL) {
        printf("Error opening user file.\n");
        return;
    }

    *userCount = 0;
    while (*userCount < MAX_USERS && fscanf(file, "%s %d %f %s %s",
            users[*userCount].name,
            &users[*userCount].typingSpeed,
            &users[*userCount].accuracy,
            users[*userCount].studentNumber,
            users[*userCount].password) == 5) {
        (*userCount)++;
    }

    fclose(file);
}

void saveUsersToFile(User users[], int userCount) {
    FILE *file = fopen(USER_FILE, "w");
    if (file == NULL) {
        printf("Error opening user file.\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %d %.2f %s %s\n",
                users[i].name,
                users[i].typingSpeed,
                users[i].accuracy,
                users[i].studentNumber,
                users[i].password);
    }

    fclose(file);
}

void generateRandomPhrase(char *text, int maxLength) {
    const char *characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int numCharacters = strlen(characters);

    int length = maxLength;
    if (length > 25) {
        length = 25; // Limit the length to 25 characters
    }

    for (int i = 0; i < length; i++) {
        text[i] = characters[rand() % numCharacters];
    }
    text[length] = '\0';
}



int menu(User users[], int userCount, User *currentUser) {

    int choice;
    do {
        printf("\n\n\n\t\t\t1. Login\n");
        printf("\n\t\t\t2. Display Statistics\n");
        printf("\n\t\t\t3. Start Typing Test\n");
        printf("\n\t\t\t4. Exit\n");
        printf("\n\t\t\t5. Go back to MAIN");
        printf("\n\t\t\tEnter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                login(users, userCount, &currentUser);
                break;
            case 2:
                displayStatistics(users, userCount);
                break;
            case 3:
                if (currentUser != NULL) {
                    startTypingTest(currentUser);
                } else {
                    printf("Please login before starting the typing test.\n");
                }
                break;
            case 4:
                printf("Exiting program.\n");
                break;
            case 5:
                main();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}

int main(){

    system("cls");
    system("color 1F");
    char res;

    User users[MAX_USERS];
    int userCount = 0;
    User *currentUser = NULL;

    loadUsersFromFile(users, &userCount);

    printf("\n\n\n\n\n\n\t\t\t\t\tWELCOME TO TYPING SKILL ENHANCER !!");
    printf("\n\n\t\t\t\t\tAre you new here?");
    printf("\n\t\t\t\t\tPress [Y] if yes and [N] if no");
    printf("\n\t\t\t\t\tEnter your response: ");
    scanf("%c", &res);
    clearInputBuffer();

    switch (res){
        case 'y': case 'Y':
            createNewUser(users, &userCount);
            break;
        case 'n': case 'N':
            login(users, userCount, &currentUser);
            break;
        default:
            printf("Invalid choice. Please try again.\n");
    }

    menu(users, userCount, currentUser); // Added the menu function call here

    return 0;
}
