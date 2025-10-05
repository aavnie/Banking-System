#ifndef ACCOUNT_UTILS
#define ACCOUNT_UTILS  

// Login and Register function declaration
int login(const char *username, const char *password);
double getBalance(const char *username);
double deposit_money(const char *username);
double transfer_money(const char *username);
void register_account(char *fName, char *lName, char *username, char *phone_number, char *email, char *password);


#endif