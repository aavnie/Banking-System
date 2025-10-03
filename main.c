#include <stdio.h>
#include "utils/account_utils.h"
#include <stdlib.h>
#include <string.h>

#define MIN_USERNAME_LENGTH 4 // minimum username length
#define MAX_USERNAME_LENGTH 33 // max username length
#define MIN_PASSWORD_LENGTH 8 //min password length
#define MAX_PASSWORD_LENGTH 65 // max password length

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

        printf("Welcome to the BANK MANAGEMENT SYSTEM\n");
        printf("1. Create Account\n");
        printf("2. Login to your Account\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if(choice == 3){
            printf("Exiting the program. Goodbye!\n");
            break;
        }
        if(choice < 1 || choice > 3 || choice == '\n'){
            printf("Invalid choice! Please try again.\n");
            continue;
        }
        
        while(1){
            switch (choice)
            {
            case 1:
                // Create Account and take user input for the user details
                printf("Welcome to the registration page!\n\n");

                Account_Registration new_account;
                printf("Enter your First Name: ");
                getchar();
                fgets(new_account.fName, sizeof(new_account.fName), stdin);
                new_account.fName[strcspn(new_account.fName, "\n")] = '\0'; // Removes the new line character, same for all the ones below
                if(new_account.fName[0] == '\0'){
                    printf("First Name cannot be empty!\n");
                    choice = 1;
                    continue;
                }

                printf("Enter your Last Name: ");
                fgets(new_account.lName, sizeof(new_account.lName), stdin);
                new_account.lName[strcspn(new_account.lName, "\n")] = '\0'; 

                printf("Enter your Username: ");
                fgets(new_account.username, sizeof(new_account.username), stdin);
                new_account.username[strcspn(new_account.username, "\n")] = '\0'; 
                
                
                printf("Enter your Password: ");
                fgets(new_account.password, sizeof(new_account.password), stdin);
                new_account.password[strcspn(new_account.password, "\n")] = '\0'; 
                
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
                
                continue;

            case 2:
                // Login to Account with the user details
                printf("2\n");
                break;
            case 3:
                // Exit the program
                printf("3\n");
                return -1;
            default:
                printf("Invalid choice! Plase try again.\n");
                return -1;
            }
        }
    }


    return 0;
}