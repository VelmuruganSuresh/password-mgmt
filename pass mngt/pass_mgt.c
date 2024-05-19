#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> // Include errno.h for errno and ENOENT

#define MAX_PASSWORD_LENGTH 50
#define FILENAME "passwords.txt"

struct PasswordEntry {
    char website[MAX_PASSWORD_LENGTH];
    char username[MAX_PASSWORD_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    struct PasswordEntry* next;
};

struct PasswordEntry* head = NULL;

void savePasswordsToFile() {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    struct PasswordEntry* current = head;
    while (current != NULL) {
        fprintf(file, "%s %s %s\n", current->website, current->username, current->password);
        current = current->next;
    }

    fclose(file);
}

void loadPasswordsFromFile() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        if (errno != ENOENT) { // ENOENT means the file does not exist, which is not an error
            perror("Error opening file for reading");
        }
        return;
    }

    struct PasswordEntry* newEntry;
    char website[MAX_PASSWORD_LENGTH], username[MAX_PASSWORD_LENGTH], password[MAX_PASSWORD_LENGTH];
    
    while (fscanf(file, "%s %s %s", website, username, password) == 3) {
        newEntry = (struct PasswordEntry*)malloc(sizeof(struct PasswordEntry));
        if (newEntry == NULL) {
            printf("Memory allocation failed.\n");
            fclose(file);
            return;
        }
        strcpy(newEntry->website, website);
        strcpy(newEntry->username, username);
        strcpy(newEntry->password, password);
        newEntry->next = head;
        head = newEntry;
    }

    fclose(file);
}

void addPassword() {
    struct PasswordEntry* newEntry = (struct PasswordEntry*)malloc(sizeof(struct PasswordEntry));
    if (newEntry == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter website: ");
    scanf("%s", newEntry->website);
    printf("Enter username: ");
    scanf("%s", newEntry->username);
    printf("Enter password: ");
    scanf("%s", newEntry->password);
    
    newEntry->next = head;
    head = newEntry;
    
    savePasswordsToFile();
    printf("Password added successfully.\n");
}

void displayPasswords() {
    if (head == NULL) {
        printf("No passwords stored.\n");
        return;
    }

    printf("Stored Passwords:\n");
    struct PasswordEntry* current = head;
    while (current != NULL) {
        printf("Website: %s, Username: %s, Password: %s\n",
               current->website, current->username, current->password);
        current = current->next;
    }
}

void deletePassword() {
    if (head == NULL) {
        printf("No passwords stored.\n");
        return;
    }

    char website[MAX_PASSWORD_LENGTH];
    printf("Enter website to delete password: ");
    scanf("%s", website);

    struct PasswordEntry* current = head;
    struct PasswordEntry* previous = NULL;
    int found = 0;

    while (current != NULL) {
        if (strcmp(current->website, website) == 0) {
            found = 1;
            if (previous == NULL) {
                head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            savePasswordsToFile();
            break;
        }
        previous = current;
        current = current->next;
    }

    if (found) {
        printf("Password for %s deleted successfully.\n", website);
    } else {
        printf("Password not found for %s.\n", website);
    }
}

int main() {
    loadPasswordsFromFile();
    
    int choice;

    do {
        printf("\nPassword Management System\n");
        printf("1. Add Password\n");
        printf("2. Display Passwords\n");
        printf("3. Delete Password\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPassword();
                break;
            case 2:
                displayPasswords();
                break;
            case 3:
                deletePassword();
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    // Free all remaining allocated memory
    struct PasswordEntry* current = head;
    while (current != NULL) {
        struct PasswordEntry* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
