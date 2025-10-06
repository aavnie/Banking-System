#include "utils/account_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <crypt.h>
#include <unistd.h>

int login(const char *username, const char *password){

    sqlite3 *db = setupDB();
    sqlite3_stmt *stmt;

    const char *sql = "SELECT password FROM Accounts WHERE username = ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if(rc != SQLITE_OK){
        fprintf(stderr, "Failed to prepare statement: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const char *stored_hash = (const char *)sqlite3_column_text(stmt, 0);

        // Hash input password using stored hash (which contains salt & cost)
        char *rehash = crypt(password, stored_hash);

        if (rehash && strcmp(rehash, stored_hash) == 0) {
            printf("Login successful!\n");
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            system("clear");
            return 1;
        } else {
            printf("Invalid password.\n");
        }
    } else {
        printf("No such user found.\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}


double getBalance(const char *username){
    sqlite3 *db = setupDB();
    sqlite3_stmt *stmt;
    double balance = -1;

    const char *sql = "SELECT balance FROM Accounts WHERE username = ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if(rc != SQLITE_OK){
        fprintf(stderr, "Failed to prepare statement: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW){
        balance = sqlite3_column_double(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return balance;
}

void deposit(char *prompt, double balance, const char *user){
    double amount;
    printf("%s", prompt);
    if(scanf("%lf", &amount) != 1 || amount <= 0){
        printf("Invalid amount!\n");
        while(getchar() != '\n');
        return;
    }

    double updatedBalance = balance + amount;
    
    sqlite3 *db = setupDB();
    sqlite3_stmt *stmt;

    const char *sql = "UPDATE Accounts SET balance = ? WHERE username = ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if(rc != SQLITE_OK){
        fprintf(stderr, "Failed to prepare statement: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_double(stmt, 1, updatedBalance);
    sqlite3_bind_text(stmt, 2, user, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE){
        fprintf(stderr, "Failed to update balance!\nError: %s", sqlite3_errmsg(db));
    } else{
        printf("Deposit successful!\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

double getPositiveDouble(const char *prompt) {
    double value;
    int rc;

    while (1) {
        printf("%s", prompt);
        rc = scanf("%lf", &value);
        while(getchar() != '\n'); // flush input

        if (rc == 1 && value > 0) {
            return value;
        }
        printf("Invalid input. Please enter a positive number.\n");
    }
}

// Helper function check if user exists in the database and get their balance
int getUserBalance(const char *username, double *balance) {
    double b = getBalance(username);
    if (b < 0) return 0; // user does not exist
    *balance = b;
    return 1;
}

// Helper function update balance in DB
int updateBalance(sqlite3 *db, const char *username, double newBalance) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE Accounts SET balance = ? WHERE username = ?";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_double(stmt, 1, newBalance);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to update balance for %s: %s\n", username, sqlite3_errmsg(db));
        return 0;
    }

    return 1;
}

int transfer_money(const char *sender) {
    char receiver[33];
    double sender_balance, receiver_balance, amount;

    printf("Enter receiver username: ");
    fgets(receiver, sizeof(receiver), stdin);
    receiver[strcspn(receiver, "\n")] = '\0';

    // Validate users
    if (!getUserBalance(sender, &sender_balance) || !getUserBalance(receiver, &receiver_balance)) {
        printf("Invalid user(s).\n");
        return -1;
    }

    // Get transfer amount
    amount = getPositiveDouble("Enter amount to transfer: ");
    if (amount > sender_balance) {
        printf("Insufficient funds.\n");
        return -1;
    }

    // Open DB and begin transaction
    sqlite3 *db = setupDB();
    if (!db) return -1;

    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

    // Update balances
    if (!updateBalance(db, sender, sender_balance - amount) ||
        !updateBalance(db, receiver, receiver_balance + amount)) {
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    sqlite3_close(db);

    printf("Transfer successful!\n");
    return 0;
}