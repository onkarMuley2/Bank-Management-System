# Console-Based Banking Management System (TCBOI)

A lightweight, console-based **Banking Management System** developed in C++. This application simulates a core banking interface, enabling users to register accounts, authenticate securely, process financial transactions, and maintain a persistent transaction log using file-based storage.

## 🚀 Features

*   **Account Generation:** Automatically increments and issues unique account numbers using a tracking index file.
*   **Persistent Storage:** Saves user profiles, credentials, live balances, and history to standalone text files, allowing data to persist across sessions.
*   **Secure Authentication:** Password-protected user login matching account credentials prior to granting main menu access.
*   **Transaction Processing:** Robust logic handling real-time deposits and withdrawals with bounds validation (preventing negative amounts or overdrafts).
*   **Transaction History:** Keeps a sequential running log of financial activities within the session and updates the data flat-file.
*   **Ansi Colored UI:** Built-in terminal text formatting (Red, Green, Blue, Yellow, etc.) for high-contrast visibility and a cleaner user experience.

## 🛠️ Tech Stack & Concepts Used

*   **Language:** C++11 or higher
*   **Object-Oriented Programming (OOP):** Deep encapsulation utilizing classes, access modifiers (`private`/`public`), and discrete member functions.
*   **File I/O:** Extensive use of standard streams (`ifstream` and `ofstream`) for text-based flat-file database handling.
*   **Data Structures:** Standard template library vectors (`std::vector<string>`) to hold dynamically sized transaction histories in memory.
*   **Terminal Utilities:** ANSI escape codes and POSIX system calls (`system("clear")`) to manage the UI visual state.

## 📋 How It Works (Data Structure Workflow)

1. **Account Registration:** The app reads the last known global bank code tracking index from a central file, increments it, builds the dynamic account number (e.g., `TCBOI101`), and opens an empty account text file.
2. **Data Serialization:** Account records are stored sequentially in an exact text structure. On user login, the application reads line-by-line using `getline()` to map memory attributes and stream data securely.
3. **Transaction Append:** Live balance modification automatically commits changes back to disk, preserving structural integrity across program loops.
