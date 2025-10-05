#include "utils/account_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <crypt.h>
#include <unistd.h>

int login(const char *username, const char *password){
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("db/accounts.db", &db);
    if(rc != SQLITE_OK){
        fprintf(stderr, "Error openning the database: %s", sqlite3_errmsg(db));
        return -1;
    } 

    const char *sql = "SELECT password FROM Accounts WHERE username = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
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
    sqlite3 *db;
    sqlite3_stmt *stmt;
    double balance = -1;
    int rc = sqlite3_open("db/accounts.db", &db);
    if(rc != SQLITE_OK){
        fprintf(stderr, "Error opening the database: %s", sqlite3_errmsg(db));
        return -1;
    }

    const char *sql = "SELECT balance FROM Accounts WHERE username = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
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

double deposit_money(const char *username){

    double current_balance = getBalance(username);
    double deposit;
    printf("Enter the amount to deposit: ");
    scanf("%lf", &deposit);

    double new_balance = current_balance + deposit;
    
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("db/accounts.db", &db);
    if(rc != SQLITE_OK){
        fprintf(stderr, "Error opening the database: %s", sqlite3_errmsg(db));
        return -1;
    }

    const char *sql = "UPDATE Accounts SET balance = ? WHERE username = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if(rc != SQLITE_OK){
        fprintf(stderr, "Failed to prepare statement: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_double(stmt, 1, new_balance);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE){
        fprintf(stderr, "Failed to update balance!\nError: %s", sqlite3_errmsg(db));
    } else{
        printf("Deposit successful!\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

double transfer_money(const char *username){
    char reciever_user[33];
    printf("Enter the reciever username: ");
    fgets(reciever_user, sizeof(reciever_user), stdin);
    reciever_user[strcspn(reciever_user, "\n")] = '\0';

    double current_balance = getBalance(username); // The sender's current balance
    double reciever_balance = getBalance(reciever_user); // The receiver's current balance
    double transfer;
    printf("Enter the amount you want to transfer: ");
    scanf("%lf", &transfer);
    getchar(); // Clear newline

    double sender_new_balance = current_balance - transfer;
    double reciever_new_balance = reciever_balance + transfer;

    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc = sqlite3_open("db/accounts.db", &db);
    if(rc != SQLITE_OK){
        fprintf(stderr, "Error opening the database: %s", sqlite3_errmsg(db));
        return -1;
    }

    // Update sender's balance
    const char *sql_sender = "UPDATE Accounts SET balance = ? WHERE username = ?";
    rc = sqlite3_prepare_v2(db, sql_sender, -1, &stmt, NULL);
    if(rc != SQLITE_OK){
        fprintf(stderr, "Failed to prepare sender statement: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }
    sqlite3_bind_double(stmt, 1, sender_new_balance);
    sqlite3_bind_text(stmt, 2, username, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE){
        fprintf(stderr, "Failed to update sender balance!\nError: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }
    sqlite3_finalize(stmt);

    // Update receiver's balance
    rc = sqlite3_prepare_v2(db, sql_sender, -1, &stmt, NULL);
    if(rc != SQLITE_OK){
        fprintf(stderr, "Failed to prepare receiver statement: %s", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }
    sqlite3_bind_double(stmt, 1, reciever_new_balance);
    sqlite3_bind_text(stmt, 2, reciever_user, -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE){
        fprintf(stderr, "Failed to update receiver balance!\nError: %s", sqlite3_errmsg(db));
    } else{
        printf("Transfer successful!\n");
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}