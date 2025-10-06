#include "utils/account_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>

#define MIN_USERNAME_LENGTH 4 // minimum username length
#define MAX_USERNAME_LENGTH 33 // max username length
#define MIN_PASSWORD_LENGTH 8 //min password length
#define MAX_PASSWORD_LENGTH 65 // max password length

int main_menu();
void client_portal_menu(const char* username);

typedef struct
{
    char username[64];
    char password[64];

}accountLogin;

typedef struct{
    char name[32];
    char lastname[32];
    char username[33];
    char phone_number[15];
    char email[64];
    char password[65];
}accountRegistration;

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
    accountRegistration new_account = {0}; // Initialize new_account to zero
    accountLogin login_account = {0}; // Initialize login_account to zero

    while(1){

        choice = main_menu();

        if(choice == 3){
            printf("Exiting the program. Goodbye!\n");
            break;
        }
        if(choice < 1 || choice > 3 || choice == '\n'){
            printf("Invalid choice! Please try again.\n");
            continue;
        }
        
        switch (choice)
        {
        case 1:
            do
            {
                
                // Create Account and take user input for the user details
                printf("Welcome to the registration page!\n\n");

                getchar( );
                do
                {
                    printf("Enter your First Name: ");
                    fgets(new_account.name, sizeof(new_account.name), stdin);
                    new_account.name[strcspn(new_account.name, "\n")] = '\0'; // Removes the new line character, same for all the ones below
                } while (strlen(new_account.name) <= 2);
                
                
                do{
                    printf("Enter your Last Name: ");                        
                    fgets(new_account.lastname, sizeof(new_account.lastname), stdin);
                    new_account.lastname[strcspn(new_account.lastname, "\n")] = '\0'; 
                }while (strlen(new_account.lastname) <= 2);

                do
                {
                    printf("Enter your Username: ");
                    fgets(new_account.username, sizeof(new_account.username), stdin);
                    new_account.username[strcspn(new_account.username, "\n")] = '\0'; 
                } while (strlen(new_account.username) == 0 || strlen(new_account.username) < MIN_USERNAME_LENGTH || strlen(new_account.username) > MAX_USERNAME_LENGTH);
                
                do
                {
                    printf("Enter your Password: ");
                    fgets(new_account.password, sizeof(new_account.password), stdin);
                    new_account.password[strcspn(new_account.password, "\n")] = '\0'; 
                } while (strlen(new_account.password) == 0 || strlen(new_account.password) < MIN_PASSWORD_LENGTH || strlen(new_account.password) > MAX_PASSWORD_LENGTH);
                

                do
                {                        
                    printf("Enter your Phone Number: ");
                    fgets(new_account.phone_number, sizeof(new_account.phone_number), stdin);
                    new_account.phone_number[strcspn(new_account.phone_number, "\n")] = '\0'; 
                } while (strlen(new_account.phone_number) == 0);
                
                do
                {                        
                    printf("Enter your Email: ");
                    fgets(new_account.email, sizeof(new_account.email), stdin);
                    new_account.email[strcspn(new_account.email, "\n")] = '\0'; 
                } while (strlen(new_account.email) == 0);
                

                register_account(
                                new_account.name, 
                                new_account.lastname, 
                                new_account.username, 
                                new_account.phone_number, 
                                new_account.email, 
                                new_account.password);
                choice = 2; // This will make sure to send the user to the login page where they will login to their account!
            } while (choice == 1);
            break;
        case 2:
        //Login to the Account with the user credentials
            do
            {
                getchar();
                printf("Enter your Username: ");
                fgets(login_account.username, sizeof(login_account.username), stdin);
                login_account.username[strcspn(login_account.username, "\n")] = '\0';
            } while (strlen(login_account.username) == 0 || strlen(login_account.username) < MIN_USERNAME_LENGTH || strlen(login_account.username) > MAX_USERNAME_LENGTH);

            do
            {
                printf("Enter your Password: ");
                fgets(login_account.password, sizeof(login_account.password), stdin);
                login_account.password[strcspn(login_account.password, "\n")] = '\0'; 
            } while (strlen(login_account.password) == 0 || strlen(login_account.password) < MIN_PASSWORD_LENGTH || strlen(login_account.password) > MAX_PASSWORD_LENGTH);
            
            int authentication = login(login_account.username, login_account.password);

            if(authentication == 1){
                client_portal_menu(login_account.username);
            }
            break;
        case 3:
            //Exiting program
            printf("4\n");
            return -1;
        default:
            printf("Invalid choice! Plase try again.\n");
            return -1;
        }
    }


    return 0;
}



int main_menu(){
        int choice = 0;
        printf("Welcome to the BANK MANAGEMENT SYSTEM\n");
        printf("1) Create Account\n");
        printf("2) Login to your Account\n");
        printf("3) Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        return choice;
}

void client_portal_menu(const char* username) {
    int choice;
    do {
        
        double balance = getBalance(username);
        printf("\nClient Portal\n");
        printf("-----------------------------\n");
        printf("Account: %s\n", username);
        printf("Current Balance: $%.2f\n", balance);
        printf("-----------------------------\n");
        printf("1. Deposit Money\n");
        printf("2. Transfer Money\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        getchar();

        switch(choice) {
            case 1:
                deposit("Deposit:$", balance, username);
                sleep(1.5);
                system("clear");
                break;
            case 2:
                transfer_money(username);
                sleep(1.5);
                system("clear");
                break;
            case 3:
                printf("Logging out...\n");
                sleep(1);
                system("clear");
                return;
            default:
                printf("Invalid choice. Try again.\n");
                sleep(1);
        }
    } while(1);
}