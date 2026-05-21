#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

static const double MINIMUM_BALANCE = 500.0;

// Terminal text color utilities using ANSI escape codes
void Red()      { cout << "\033[31m"; } 
void Green()    { cout << "\033[32m"; }
void Yellow()   { cout << "\033[33m"; }
void Blue()     { cout << "\033[34m"; }
void Magenta()  { cout << "\033[35m"; }
void Cyan()     { cout << "\033[36m"; }
void White()    { cout << "\033[37m"; }
void Reset()    { cout << "\033[0m";  }   

class bankAccount {
    private:
        string bankName = "BOI";  // Generates account formats like BOI101
        int bankCode;
        string accountHolderName;
        string accountNumber;
        string accountPassword;
        string aadhaarNumber;
        string email;
        string phoneNumber;
        double balance;
        vector<string> transactionHistory;

    public:
        void savetoFile();
        void loadFromFile();
        void mainMenu();
        void createAccount();
        void loginBankAccount();
        void depositMoney();
        void withdrawMoney();
        void checkBalance();
        void accountDetails();
        void allTransactionHistory();
};

void bankAccount::savetoFile() {
    ofstream file(accountNumber + ".txt");
    if (file.is_open()) {
        file << accountHolderName << endl;
        file << accountNumber << endl;
        file << accountPassword << endl;
        file << aadhaarNumber << endl;
        file << email << endl;
        file << phoneNumber << endl;
        file << balance << endl;
        
        for (const auto& transaction : transactionHistory) {
            file << transaction << endl;
        }    
        file.close();
    } else {
        cout << "Error saving account data!" << endl;
    }
}

void bankAccount::loadFromFile() {
    ifstream file(accountNumber + ".txt");
    if (file.is_open()) {
        getline(file, accountHolderName);
        getline(file, accountNumber);
        getline(file, accountPassword);
        getline(file, aadhaarNumber);
        getline(file, email);
        getline(file, phoneNumber);
        file >> balance;
        file.ignore();
        
        string transaction;
        transactionHistory.clear(); // Clear memory before reloading to avoid duplicate history stack
        while (getline(file, transaction)) {
            if (!transaction.empty()) {
                transactionHistory.push_back(transaction);
            }
        }
        file.close();
    } else {
        cout << "Error loading account data!" << endl;
    }
}

void bankAccount::mainMenu() {
    int choice;
    do {
        cout << "----------------------------------" << endl;
        cout << " Enter 1 for Deposit Money " << endl;
        cout << " Enter 2 for Withdraw Money " << endl;
        cout << " Enter 3 for Check Balance " << endl;
        cout << " Enter 4 for Account Details " << endl;
        cout << " Enter 5 for All Transaction History " << endl;
        cout << " Enter 6 for Logout " << endl;
        cout << "----------------------------------" << endl;
        cout << " Enter your choice: ";
        cin >> choice;
        cin.ignore();
        system("clear");

        switch (choice) {
            case 1: depositMoney(); break;
            case 2: withdrawMoney(); break;
            case 3: checkBalance(); break;
            case 4: accountDetails(); break;
            case 5: allTransactionHistory(); break;
            case 6: cout << "Logging out from BOI Portal...!" << endl; break;
            default: cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6);
}

void bankAccount::createAccount() {
    Green();
    cout << "     Create BOI Account     " << endl;
    cout << "----------------------------" << endl;
    cout << "Enter Your Full Name        : ";
    getline(cin, accountHolderName);
    cout << "Enter Email ID              : ";
    getline(cin, email);
    cout << "Enter 10-Digit Mobile No    : ";
    getline(cin, phoneNumber);
    cout << "Enter 12-Digit Aadhaar No   : ";
    getline(cin, aadhaarNumber);
    cout << "Enter Account Password      : ";
    getline(cin, accountPassword);
    cout << "----------------------------" << endl;
    Reset();

    balance = MINIMUM_BALANCE; // Standard minimum initial deposit amount in INR
    
    // Read the tracking index file tailored for Bank of India registry mapping
    ifstream file("boi_registry_index.txt");
    if (file.is_open()) {
        file >> bankCode;
        file.close();
    } else {
        bankCode = 10001; // Secure base format default starting branch index 
    }

    // Increment and save the global indexing identifier
    ofstream output("boi_registry_index.txt");
    if (output.is_open()) {
        output << bankCode + 1;
        output.close();
    } else {
        cout << "System Error: Unable to access bank registry file!" << endl;
        return;
    }

    accountNumber = bankName + to_string(bankCode);
    transactionHistory.push_back("Initial Deposit :- Rs. " + to_string(balance));
    
    Yellow();
    cout << "Account Created Successfully!" << endl;
    cout << "Account Number   : " << accountNumber << endl;
    cout << "Account Password : " << accountPassword << endl;
    cout << "Minimum Balance  : Rs. 500.00" << endl;
    cout << "----------------------------" << endl;
    Reset();
    
    savetoFile();
}

void bankAccount::loginBankAccount() {
    string enteredAccountNumber;
    string enteredPassword;
    Blue();
    
    cout << "     Login to Bank Of India    " << endl;
    cout << "-------------------------------" << endl;
    cout << "Enter Account Number           : ";
    getline(cin, enteredAccountNumber);
    accountNumber = enteredAccountNumber;
    
    ifstream file(enteredAccountNumber + ".txt");
    if (file.is_open()) {
        file.close(); // Close stream immediately; loadFromFile builds its own native stream
        loadFromFile();
        
        cout << "Enter Account Password         : ";
        getline(cin, enteredPassword);
        
        if (accountPassword == enteredPassword) {
            cout << "Authentication Successful!" << endl;
            mainMenu();
        } else {
            cout << "Invalid Credentials / Wrong Password!" << endl;
        }
    } else {
        cout << "Account details not found in BOI database!" << endl;
    }
    Reset();
}

void bankAccount::depositMoney() {
    double amount;
    cout << "Enter Amount to Deposit (Rs.): ";
    cin >> amount;
    cin.ignore();
    
    if (amount > 0) {
        balance += amount;
        transactionHistory.push_back("Deposit    :- Rs. " + to_string(amount));
        savetoFile();

        cout << "Deposit Successful!" << endl;
        cout << "Updated Account Balance: Rs. " << balance << endl;
    } else {
        cout << "Invalid Entry: Transaction Amount must be positive!" << endl;
    }
}

void bankAccount::withdrawMoney() {
    double amount;
    cout << "Enter Amount to Withdraw (Rs.): ";
    cin >> amount;
    cin.ignore();
   
    if (amount <= 0) {
        cout << "Invalid Entry: Transaction Amount must be positive!" << endl;
        return;
    }

    if (amount > balance - MINIMUM_BALANCE) {
        cout << "Transaction Declined: Withdrawal would breach minimum balance of Rs. " << MINIMUM_BALANCE << "!" << endl;
        return;
    }

    balance -= amount;
    transactionHistory.push_back("Withdraw   :- Rs. " + to_string(amount));
    savetoFile();

    cout << "Withdrawal Successful!" << endl;
    cout << "Remaining Account Balance: Rs. " << balance << endl;
}

void bankAccount::checkBalance() {
    cout << "Current Account Balance : Rs. " << balance << endl;
    cout << "---------------------------------------" << endl;
}

void bankAccount::accountDetails() {
    Magenta();
    cout << "|--------------------------------------|" << endl;
    cout << "|            BANK OF INDIA             |" << endl;
    cout << "|--------------------------------------|" << endl;
    Blue();

    cout << "Account Holder Name : " << accountHolderName << endl;
    cout << "Account Number      : " << accountNumber << endl;
    cout << "Aadhaar Card Number : " << aadhaarNumber << endl;
    cout << "Registered Email ID : " << email << endl;
    cout << "Mobile Phone Number : " << phoneNumber << endl;
    cout << "Current Ledger Bal  : Rs. " << balance << endl;
    cout << "---------------------------------------" << endl;
    Reset();
}

void bankAccount::allTransactionHistory() {
    cout << " Detailed Transaction History :- " << endl;
    Yellow();
    cout << "---------------------------------------" << endl;
    cout << "Account Holder Name : " << accountHolderName << endl;
    cout << "Account Number      : " << accountNumber << endl;
    
    for (const auto& transaction : transactionHistory) {
        cout << transaction << endl;
    }
    cout << "---------------------------------------" << endl;
    Reset();
}

int main() {
    bankAccount account;
    int choice;
   
    do {
        cout << " Welcome to the Bank Of India e-Portal" << endl;
        cout << "--------------------------------------" << endl;
        cout << "1. Create New Bank Account" << endl;
        cout << "2. Secure Account Login" << endl;
        cout << "3. Exit System" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        system("clear");
        
        switch (choice) {
            case 1: account.createAccount(); break;
            case 2: account.loginBankAccount(); break;
            case 3: cout << "Thank you for banking with Bank Of India. Jai Hind!" << endl; break;
            default: cout << "Invalid choice selection. Please try again." << endl;
        }
    } while (choice != 3);
    
    return 0;
}
