#include "utils/account_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <crypt.h>

sqlite3 *setupDB();
char *password_hashing(const char *password);


void register_account(char *fName, char *lName, char *username, char *phone_number, char *email, char *password){

    // Hash the password using SHA-256
    char *hashed_password = password_hashing(password);

    
    sqlite3 *db = setupDB();

    char *sql = "CREATE TABLE IF NOT EXISTS Accounts("
                "ID INTEGER PRIMARY KEY NOT NULL, "
                "fName TEXT   NOT NULL, " 
                "lName TEXT   NOT NULL, " 
                "username TEXT   NOT NULL, " 
                "phone_number TEXT   NOT NULL, " 
                "email TEXT   NOT NULL, " 
                "password TEXT   NOT NULL,"
                "balance REAL DEFAULT 0.0   NOT NULL);";

    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if(rc != SQLITE_OK){
        
        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        free(hashed_password);
        return;

    }

    const char *sql_insert = "INSERT INTO Accounts(fName, lName, username, phone_number, email, password) VALUES(?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, 0);
    if(rc != SQLITE_OK){
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        free(hashed_password);
        return;
    }

    // Bind the parameters to the statement
    sqlite3_bind_text(stmt, 1, fName, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, lName, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, phone_number, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, hashed_password, -1, SQLITE_STATIC);

    // Execute the insert and insert the data into the database
    rc = sqlite3_step(stmt);
    if(rc != SQLITE_DONE){
        fprintf(stderr, "Failed to insert user data: %s\n", sqlite3_errmsg(db));
    }
    else{
        printf("Account created successfully!\n");
        printf("Go to login page to login into your account!\n");
    }

    // Cleanup and close database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    free(hashed_password);
    
}

// Function to setup the database

sqlite3 *setupDB(){
    
    sqlite3 *db;    
    int rc = sqlite3_open("db/accounts.db", &db);

    if(rc != SQLITE_OK){
        fprintf(stderr, "Cannot open database, %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        return NULL;
    }

    return db;
}

// Function to hash the password

char *password_hashing(const char *password){
    char salt[64];
    
    // Generate a random salt
    if (!crypt_gensalt_rn("$2b$", 12, NULL, 0, salt, sizeof(salt))){
        perror("crypt_gensalt_rn");
        return NULL;
    }

    // Hashing the password with the salt we generated
    char *hashed_password = crypt(password, salt);
    if(!hashed_password){
        fprintf(stderr, "Error hashing the password\n");
        return NULL;
    }

    return strdup(hashed_password);
}