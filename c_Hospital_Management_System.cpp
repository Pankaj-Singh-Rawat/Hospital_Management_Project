#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50

typedef struct 
{  
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    char type[MAX_USERNAME_LEN];  
} User;

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

void sign_up(){
    User user; 
    printf("Enter user type (admin/patient/doctor): ");
    scanf("%s" , user.type);
    printf("Enter username: ");
    scanf("%s" , user.username);
    printf("Enter password: ");
    scanf("%s" , user.password);

    if(user_exists(user.username)){
        printf("Error: User already exists.\n");
    }
    else{
        save_user_data(user);
        printf("Signup Successful.\n");
    }
    exit(0);
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
    printf("Doctor added successfully.");

}

void delete_doctor(){
    //need to add code
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

void delete_patient(){
    //need to add code
}

void admin_menu(){
    printf("Admin Menu:\n");
    printf("1. View All user data.\n");
    printf("2. Add Doctor\n");
    printf("3. Delete doctor\n");
    printf("4. Add Patient\n");
    printf("5. Delete Patient\n");

    int choice;
    scanf("%d" , &choice);

    switch(choice){
        case 1:
            view_all_user_data();
            break;
        case 2:
            add_doctor();
            break;

        default:
            printf("Invalid choice.\n");
    }
}

void patient_menu(){
    printf("Patient Menu:\n");

}

void doctor_menu(){
    printf("Doctor Menu:\n");

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

int main(){
    int choice;
    printf("Welcome to Hospital Management System!\n");
    while(1){
        printf("1. Sign Up\n2. Login\n3. Exit\nEnter your choice: "); 
        scanf("%d" , &choice); 
        switch(choice){
            case 1: sign_up();
            break;
            case 2: login();
            break;
            case 3: printf("Exiting Hospital Management System!...\n");
                    exit(0);
            default:
            printf("Invalid choice please try again.\n");
            exit(0);
        }
    }
}
