#ifndef ACCOUNT_UTILS
#define ACCOUNT_UTILS  

#include <sqlite3.h>

sqlite3 *setupDB();

// Login and Register function declaration
int login(const char *username, const char *password);
double getBalance(const char *username);
void deposit(char *prompt, double balance, const char *user);
int transfer_money(const char *username);
void register_account(char *fName, char *lName, char *username, char *phone_number, char *email, char *password);

#endif