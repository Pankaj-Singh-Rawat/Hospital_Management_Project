#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/sha.h>

#define MAX_USERS 100
#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 64
#define MAX_NAME_LEN 100
#define MAX_EMAIL_LEN 100

typedef struct {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    char name[MAX_NAME_LEN];
    char email[MAX_EMAIL_LEN];
} User;

typedef struct {
    User users[MAX_USERS];
    int count;
} UserDatabase;

void save_user_data(UserDatabase *db) {
    FILE *file = fopen("user_data.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < db->count; i++) {
            fprintf(file, "%s;%s;%s;%s\n", db->users[i].username, db->users[i].password, db->users[i].name, db->users[i].email);
        }
        fclose(file);
    } else {
        printf("Error: Unable to open file for writing.\n");
    }
}

void load_user_data(UserDatabase *db) {
    FILE *file = fopen("user_data.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%[^;];%[^;];%[^;];%[^\n]\n", db->users[db->count].username, db->users[db->count].password,
                      db->users[db->count].name, db->users[db->count].email) != EOF) {
            db->count++;
        }
        fclose(file);
    } else {
        printf("User data file not found. Starting with empty database.\n");
    }
}

void create_id(UserDatabase *db, const char *username, const char *password, const char *name, const char *email) {
    if (db->count < MAX_USERS) {
        strncpy(db->users[db->count].username, username, MAX_USERNAME_LEN - 1);
        strncpy(db->users[db->count].name, name, MAX_NAME_LEN - 1);
        strncpy(db->users[db->count].email, email, MAX_EMAIL_LEN - 1);
        
        // Hashing the password using SHA-256
        unsigned char hashed_password[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, password, strlen(password));
        SHA256_Final(hashed_password, &sha256);
        
        // Convert hashed password to hexadecimal string
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            sprintf(db->users[db->count].password + (i * 2), "%02x", hashed_password[i]);
        }
        db->count++;
        save_user_data(db);
    } else {
        printf("Error: Maximum number of users reached.\n");
    }
}

User *login(UserDatabase *db, const char *username, const char *password) {
    for (int i = 0; i < db->count; i++) {
        if (strcmp(db->users[i].username, username) == 0) {
            // Hashing the provided password
            unsigned char hashed_password[SHA256_DIGEST_LENGTH];
            SHA256_CTX sha256;
            SHA256_Init(&sha256);
            SHA256_Update(&sha256, password, strlen(password));
            SHA256_Final(hashed_password, &sha256);
            
            // Converting hashed password to hexadecimal string
            char hashed_password_str[MAX_PASSWORD_LEN];
            for (int j = 0; j < SHA256_DIGEST_LENGTH; j++) {
                sprintf(hashed_password_str + (j * 2), "%02x", hashed_password[j]);
            }
            
            if (strcmp(db->users[i].password, hashed_password_str) == 0) {
                return &db->users[i];
            } else {
                printf("Incorrect password.\n");
                return NULL;
            }
        }
    }
    printf("User not found.\n");
    return NULL;
}

int main() {
    UserDatabase db = {.count = 0};
    load_user_data(&db);

    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    char name[MAX_NAME_LEN];
    char email[MAX_EMAIL_LEN];
    
    printf("Enter user type (admin/patient/doctor): ");
    char user_type[MAX_USERNAME_LEN];
    scanf("%s", user_type);

    printf("Enter username: ");
    scanf("%s", username);

    User *user = login(&db, username, password);
    if (user == NULL) {
        char choice;
        printf("User not found. Do you want to sign up? (yes/no): ");
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            printf("Enter your name: ");
            scanf("%s", name);
            printf("Enter your email: ");
            scanf("%s", email);
            printf("Enter your password: ");
            scanf("%s", password);
            create_id(&db, username, password, name, email);
            printf("Sign up successful. You can now login.\n");
        } else {
            printf("Sign up canceled.\n");
        }
    } else {
        printf("Login successful.\n");
        printf("User details:\n");
        printf("Name: %s\n", user->name);
        printf("Email: %s\n", user->email);
    }

    return 0;
}
