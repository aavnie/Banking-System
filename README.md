# Banking System in C 

A console-based banking system implemented in **C**, allowing users to register, log in, and perform basic banking operations.  
Data is stored persistently using **SQLite**, making transactions and account management reliable.

---

## 📌 Features

- **User Registration & Login**  
  Secure account creation and authentication.

- **Account Management**  
  View and update personal details.

- **Transactions**  
  Deposit and withdraw funds.

- **Persistent Storage with SQLite**  
  All account data is stored in a local SQLite database.

---

## ⚙️ Installation & Compilation

1. **Clone the repository:**

   ```bash
   git clone https://github.com/aavnie/Banking-System.git
   cd Banking-System
   ```
2. **Compile the program:**
   ```bash
   gcc main.c register.c login.c -o banking-system -lsqlite3 -lcrypt
   ```
3. **Run the program:**
   ```bash
   ./banking-system
   ```
---
Tech Stack

* Language: **C**

* Database: **SQLite**

* Compiler: **GCC** (or any standard C compiler)
