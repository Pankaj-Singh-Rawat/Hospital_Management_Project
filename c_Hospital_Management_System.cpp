#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_REPORT_LEN 500

typedef struct 
{  
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    char type[MAX_USERNAME_LEN];  
} User;

typedef struct 
{  
    char patient_username[MAX_USERNAME_LEN];
    char content[MAX_REPORT_LEN];  
} PatientReport;

void save_user_data(User user) { 
    FILE *file = fopen("user_data.txt" , "a");
    if (file != NULL){
        fprintf(file , "%s %s %s\n", user.type , user.username , user.password); 
        fclose(file); 
    }
    else{
        printf("Error: Unable to save user data.\n");
    }
}

int user_exists(char *username) {
    FILE *file = fopen("user_data.txt","r");
    char line[MAX_USERNAME_LEN + MAX_PASSWORD_LEN + MAX_USERNAME_LEN]; 
    while (fgets(line, sizeof(line) , file)){
        char *token = strtok(line, " "); //tokenising the data and " " differentiating it individually.
        if (strcmp(token, username) == 0){  // strcmp checks two string and if same results in 0.
            fclose(file);
            return 1; 
        }
    }
    fclose(file);
    return 0;
}

void sign_up() {
    User user;

    printf("Enter user type (admin/patient/doctor): ");
    scanf("%s", user.type);
    printf("Enter username: ");
    scanf("%s", user.username);
    printf("Enter password: ");
    scanf("%s", user.password);

    if (user_exists(user.username)) {
        printf("Error: User already exists.\n");
    } else {
        save_user_data(user);
        printf("Signup Successful.\n");
    }
}

int generate_room_number() {
    srand(time(NULL));
    return rand() % 900 + 100;//between 100 and 999 (inclusive)
}


void view_all_user_data(){
    FILE *file = fopen("user_data.txt" , "r");
    if(file == NULL){
        printf("Error: Unable to open user data , maybe file is empty!!!");
        return;
    }
    
    printf("All user data:\n");
    char line[MAX_USERNAME_LEN + MAX_PASSWORD_LEN + MAX_USERNAME_LEN];
    while (fgets(line, sizeof(line) , file)){
        char stored_type[MAX_USERNAME_LEN];
        char stored_username[MAX_USERNAME_LEN];
        char stored_password[MAX_PASSWORD_LEN];
        sscanf(line, "%s %s %s", stored_type, stored_username, stored_password);
        printf("User Type: %s, Username: %s, Password: %s\n", stored_type, stored_username, stored_password);
    }
    fclose(file);
}

void add_doctor(){
    User doctor;
    printf("Enter doctor's username: ");
    scanf("%s" , doctor.username);
    printf("Enter doctor's password: ");
    scanf("%s" , doctor.password);

    strcpy(doctor.type , "doctor");

    save_user_data(doctor);
    printf("Doctor added successfully. \n");

}

void delete_doctor() {
    char username[MAX_USERNAME_LEN];
    printf("Enter doctor's username to delete: ");
    scanf("%s", username);

    FILE *tempFile = fopen("temp.txt", "w");
    FILE *file = fopen("user_data.txt", "r");

    if (file == NULL) {
        printf("Error: Unable to open user data.\n");
        return;
    }

    char line[MAX_USERNAME_LEN + MAX_PASSWORD_LEN + MAX_USERNAME_LEN];
    int deleted = 0;

    while (fgets(line, sizeof(line), file)) {
        char stored_username[MAX_USERNAME_LEN];
        char stored_password[MAX_PASSWORD_LEN];
        char stored_type[MAX_USERNAME_LEN];
        sscanf(line, "%s %s %s", stored_type, stored_username, stored_password);
        if (strcmp(stored_username, username) == 0 && strcmp(stored_type, "doctor") == 0) {
            deleted = 1;
        } else {
            fprintf(tempFile, "%s %s %s\n", stored_type, stored_username, stored_password);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("user_data.txt");
    rename("temp.txt", "user_data.txt");

    if (deleted) {
        printf("Doctor '%s' deleted successfully.\n", username);
    } else {
        printf("Doctor '%s' not found.\n", username);
    }
}


void add_patient(){
    User patient;
    printf("Enter Patient's username: ");
    scanf("%s" , patient.username);
    printf("Enter Patient's password: ");
    scanf("%s" , patient.password);

    strcpy(patient.type , "patient");

    save_user_data(patient);
    printf("patient added successfully.");
}

void delete_patient() {
    char username[MAX_USERNAME_LEN];
    printf("Enter patient's username to delete: ");
    scanf("%s", username);

    FILE *tempFile = fopen("temp.txt", "w");
    FILE *file = fopen("user_data.txt", "r");

    if (file == NULL) {
        printf("Error: Unable to open user data.\n");
        return;
    }

    char line[MAX_USERNAME_LEN + MAX_PASSWORD_LEN + MAX_USERNAME_LEN];
    int deleted = 0;

    while (fgets(line, sizeof(line), file)) {
        char stored_username[MAX_USERNAME_LEN];
        char stored_password[MAX_PASSWORD_LEN];
        char stored_type[MAX_USERNAME_LEN];
        sscanf(line, "%s %s %s", stored_type, stored_username, stored_password);
        if (strcmp(stored_username, username) == 0 && strcmp(stored_type, "patient") == 0) {
            deleted = 1;
        } else {
            fprintf(tempFile, "%s %s %s\n", stored_type, stored_username, stored_password);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("user_data.txt");
    rename("temp.txt", "user_data.txt");

    if (deleted) {
        printf("Patient '%s' deleted successfully.\n", username);
    } else {
        printf("Patient '%s' not found.\n", username);
    }
}

void write_patient_report() {
    PatientReport report;

    printf("Enter patient's username: ");
    scanf("%s", report.patient_username);
    printf("Enter report content: ");
    scanf(" %[^\n]", report.content);

    FILE *file = fopen("patient_reports.txt", "a");
    if (file == NULL) {
        printf("Error: Unable to open patient report file.\n");
        return;
    }

    fprintf(file, "%s:%s\n", report.patient_username, report.content);
    fclose(file);

    printf("Report written successfully.\n");
}

void add_patient_medicine() {
    PatientReport medicine;

    printf("Enter patient's username: ");
    scanf("%s", medicine.patient_username);
    printf("Enter medicine details: ");
    scanf(" %[^\n]", medicine.content); // This reads a full line including spaces

    FILE *file = fopen("patient_medicine.txt", "a");
    if (file == NULL) {
        printf("Error: Unable to open patient medicine file.\n");
        return;
    }

    fprintf(file, "%s:%s\n", medicine.patient_username, medicine.content);
    fclose(file);

    printf("Medicine details added successfully.\n");
}


void view_patient_report() {
    char patient_username[MAX_USERNAME_LEN];

    printf("Enter patient's username to view report: ");
    scanf("%s", patient_username);

    FILE *file = fopen("patient_reports.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open patient report file.\n");
        return;
    }

    char line[MAX_USERNAME_LEN + MAX_REPORT_LEN];
    int found = 0;

    printf("Patient Report:\n");

    while (fgets(line, sizeof(line), file)) {
        char stored_username[MAX_USERNAME_LEN];
        char content[MAX_REPORT_LEN];
        sscanf(line, "%[^:]:%[^\n]", stored_username, content);
        if (strcmp(stored_username, patient_username) == 0) {
            found = 1;
            printf("Report for %s:\n%s\n", stored_username, content);
        }
    }

    fclose(file);

    if (!found) {
        printf("No report found for user %s.\n", patient_username);
    }
}


void view_OT_number(){
    int OT_number = generate_room_number();
    printf("You have to perform Operation in room: %d\n", OT_number);
}

void view_room_number() {
    int room_number = generate_room_number();
    printf("Your assigned room number is: %d\n", room_number);
}

void view_my_medicine() {
    char username[MAX_USERNAME_LEN];

    printf("Enter your username: ");
    scanf("%s", username);

    FILE *file = fopen("patient_medicine.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open patient medicine file.\n");
        return;
    }

    char line[MAX_USERNAME_LEN + MAX_REPORT_LEN];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        char stored_username[MAX_USERNAME_LEN];
        char content[MAX_REPORT_LEN];
        sscanf(line, "%[^:]:%[^\n]", stored_username, content);
        if (strcmp(stored_username, username) == 0) {
            found = 1;
            printf("Medicine for %s:\n%s\n", stored_username, content);
        }
    }

    fclose(file);

    if (!found) {
        printf("No medicine details found for user %s.\n", username);
    }
}


void view_my_report() {
    char username[MAX_USERNAME_LEN];

    printf("Enter your username: ");
    scanf("%s", username);

    FILE *file = fopen("patient_reports.txt", "r");
    if (file == NULL) {
        printf("Error: Unable to open patient report file.\n");
        return;
    }

    char line[MAX_USERNAME_LEN + MAX_REPORT_LEN];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        char stored_username[MAX_USERNAME_LEN];
        char content[MAX_REPORT_LEN];
        sscanf(line, "%[^:]:%[^\n]", stored_username, content);
        if (strcmp(stored_username, username) == 0) {
            found = 1;
            printf("Report for %s:\n%s\n", stored_username, content);
        }
    }

    fclose(file);

    if (!found) {
        printf("No report found for user %s.\n", username);
    }
}

void admin_menu() {
    int choice;

    while (1) {
        printf("Admin Menu:\n");
        printf("1. View All user data.\n");
        printf("2. Add Doctor\n");
        printf("3. Delete Doctor\n");
        printf("4. Add Patient\n");
        printf("5. Delete Patient\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                view_all_user_data();
                break;
            case 2:
                add_doctor();
                break;
            case 3:
                delete_doctor();
                break;
            case 4:
                add_patient();
                break;
            case 5:
                delete_patient();
                break;
            case 6:
                printf("Exiting Admin Menu...\n");
                return; 
            default:
                printf("Invalid choice please try again.\n");
                while(getchar() != '\n');
        }
    }
}

void doctor_menu(){
        int choice;

    while (1) {
        printf("Doctor Menu:\n");
        printf("1. ADD Patient Report\n");        
        printf("2. ADD Patient Medicine\n");
        printf("3. View Patient Reports.\n");
        printf("4. View Operation Room Number.\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                write_patient_report();
                break;
            case 2:
                add_patient_medicine();
                break;
            case 3:
                view_patient_report();
                break;
            case 4:
                view_OT_number();
                break;
            case 5:
                printf("Exiting Doctor Menu...\n");
                return; 
            default:
                printf("Invalid choice please try again.\n");
                while(getchar() != '\n');
        }
    }
}


void patient_menu(){
        int choice;

    while (1) {
        printf("Patient Menu:\n");
        printf("1. View Room number\n");        
        printf("2. View my Medicine\n");
        printf("3. View my report\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                view_room_number();
                break;
            case 2:
                view_my_medicine();
                break;
            case 3:
                view_my_report();
                break;
            case 4:
                printf("Exiting Patient Menu...\n");
                return; 
            default:
                printf("Invalid choice please try again.\n");
                while(getchar() != '\n');
        }
    }
}

void login(){
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN]; 
    printf("Enter Username: ");
    scanf("%s" , username);
    printf("Enter Password: "); 
    scanf("%s" , password);

    FILE *file = fopen("user_data.txt" , "r");
    char line[MAX_USERNAME_LEN + MAX_PASSWORD_LEN + MAX_USERNAME_LEN]; 
    while(fgets(line, sizeof(line) , file)){
        char stored_username[MAX_USERNAME_LEN];
        char stored_password[MAX_PASSWORD_LEN];
        char stored_type[MAX_USERNAME_LEN]; 
        sscanf(line, "%s %s %s", stored_type, stored_username, stored_password);
        if(strcmp(stored_username , username) == 0 && strcmp(stored_password , password) == 0){ 
            printf("Login successful. Welcome, %s!\n", stored_type);
            fclose(file);
            if (strcmp(stored_type, "admin") == 0) {
                admin_menu();
            } else if (strcmp(stored_type, "patient") == 0) {
                patient_menu();
            } else if (strcmp(stored_type, "doctor") == 0) {
                doctor_menu();
            } else {
                printf("Unknown user type.\n");
            }            
            exit(0);
        }
    }
    fclose(file);
    printf("Invalid username or password.\n");
    exit(0);
}

int main() {
    int choice;

    printf("Welcome to Hospital Management System!\n");

    while (1) {
        printf("1. Sign Up\n2. Login\n3. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                sign_up();
                break;
            case 2:
                login();
                break;
            case 3:
                printf("Exiting Hospital Management System!...\n");
                return 0; 
            default:
                printf("Invalid choice please try again.\n");
                while(getchar() != '\n');
        }
    }
}
