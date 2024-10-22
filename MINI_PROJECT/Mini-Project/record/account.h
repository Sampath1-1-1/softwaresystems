#ifndef ACCOUNT_RECORD
#define ACCOUNT_RECORD

#define MAX_TRANSACTIONS 100
#include <stdbool.h>
struct Account
{
    int id; // 0, 1, 2 ....
    char name[25];
    char gender; // M -> Male, F -> Female, O -> Other
    int age;
    // Login Credentials
    char login[30]; // Format : name-id (name will the first word in the structure member `name`)
    char password[30];
    bool isActive;
    int balance;
    int trancsaction[MAX_TRANSACTIONS];
    // Bank data
    int accountNumber; // Account number of the account the customer owns
};

#endif