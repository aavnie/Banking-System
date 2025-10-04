#include <stdio.h>
#include "utils/account_utils.h"
#include <stdlib.h>
#include <string.h>

#define MIN_USERNAME_LENGTH 4 // minimum username length
#define MAX_USERNAME_LENGTH 33 // max username length
#define MIN_PASSWORD_LENGTH 8 //min password length
#define MAX_PASSWORD_LENGTH 65 // max password length

char *validate_username(char *username, size_t size);
char *validate_password(char *password, size_t size);
int user_choice(){
        int choice = 0;
        printf("Welcome to the BANK MANAGEMENT SYSTEM\n");
        printf("1. Create Account\n");
        printf("2. Login to your Account\n");
        printf("3. Back to Menu!\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        return choice;
}

typedef struct
{
    char account[64];
    char password[64];

}Account_Login;

typedef struct{
    char fName[32];
    char lName[32];
    char username[33];
    char phone_number[15];
    char email[64];
    char password[65];
}Account_Registration;

int main(){
    /*
    
    A Bank Account System is essential for managing personal
    finances, allowing users to securely store money, check balances,
    and transfer funds. Using C language, we can create an application
    that can manage the data of the Bank, we use basic knowledge of
    C like string, array, structures, etc.
    The functionality of the Bank Management System Project is
    mentioned below:
                    1. Transfer Money to the Account
                    2. Creation of Account
                    3. Check Amount
                    4. Login Functionality
    
                    */
    int choice;
    Account_Registration new_account = {0}; // Initialize new_account to zero
    Account_Login login_account = {0}; // Initialize login_account to zero

    while(1){

        choice = user_choice();

        if(choice == 4){
            printf("Exiting the program. Goodbye!\n");
            break;
        }
        if(choice < 1 || choice > 4 || choice == '\n'){
            printf("Invalid choice! Please try again.\n");
            continue;
        }
        
        while(1){
            switch (choice)
            {
            case 1:
                do
                {
                    
                    // Create Account and take user input for the user details
                    printf("Welcome to the registration page!\n\n");

                    Account_Registration new_account;
                    printf("Enter your First Name: ");
                    getchar();
                    fgets(new_account.fName, sizeof(new_account.fName), stdin);
                    new_account.fName[strcspn(new_account.fName, "\n")] = '\0'; // Removes the new line character, same for all the ones below
                    if(new_account.fName == 0){
                        printf("Your ");
                    }

                    printf("Enter your Last Name: ");
                    fgets(new_account.lName, sizeof(new_account.lName), stdin);
                    new_account.lName[strcspn(new_account.lName, "\n")] = '\0'; 


                    printf("Enter your Username: ");
                    fgets(new_account.username, sizeof(new_account.username), stdin);
                    new_account.username[strcspn(new_account.username, "\n")] = '\0'; 
                    validate_username(new_account.username, sizeof(new_account.username));
                    
                    
                    printf("Enter your Password: ");
                    fgets(new_account.password, sizeof(new_account.password), stdin);
                    new_account.password[strcspn(new_account.password, "\n")] = '\0'; 
                    validate_password(new_account.password, sizeof(new_account.password));
                    
                    printf("Enter your Phone Number: ");
                    fgets(new_account.phone_number, sizeof(new_account.phone_number), stdin);
                    new_account.phone_number[strcspn(new_account.phone_number, "\n")] = '\0'; 
                    
                    printf("Enter your Email: ");
                    fgets(new_account.email, sizeof(new_account.email), stdin);
                    new_account.email[strcspn(new_account.email, "\n")] = '\0'; 

                    register_account(
                                    new_account.fName, 
                                    new_account.lName, 
                                    new_account.username, 
                                    new_account.phone_number, 
                                    new_account.email, 
                                    new_account.password);
                    choice = user_choice();
                    continue; 
                } while (choice == 1);

            case 2:
                // Login to Account with the user details
                printf("2\n");
                break;
            case 3:
                // Back to menu
                printf("3\n");
                break;
            case 4:
                //Exiting program
                printf("4\n");
                return -1;
            default:
                printf("Invalid choice! Plase try again.\n");
                return -1;
            }
        }
    }


    return 0;
}

char *validate_username(char *username, size_t size){
    int length = strlen(username);
    while (length < MIN_USERNAME_LENGTH || length > MAX_USERNAME_LENGTH) {
        fprintf(stderr, "Invalid username, it must be %d–%d characters long!\n", MIN_USERNAME_LENGTH, MAX_USERNAME_LENGTH - 1);

        printf("Enter your Username: ");
        fgets(username, size, stdin);
        username[strcspn(username, "\n")] = '\0';
        length = strlen(username);
    }
    return username; // pointer to valid username
}

char *validate_password(char *password, size_t size){

    int length = strlen(password);

    while(length < MIN_PASSWORD_LENGTH || length > MAX_PASSWORD_LENGTH) {
        fprintf(stderr, "Invalid password! Please input a password %d-%d characters long\n", MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH - 1);
        
        printf("Enter your Password: ");
        fgets(password, size, stdin);
        password[strcspn(password, "\n")] = '\0';
        length = strlen(password);
    }
    return password;
}
