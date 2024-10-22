#ifndef EMPLOYEE_RECORD
#define EMPLOYEE_RECORD

#define MAX_TRANSACTIONS 100
#include <stdbool.h>
struct Employee
{
    int id; // 0, 1, 2 ....
    char name[25];
    char gender; // M -> Male, F -> Female, O -> Other
    int age;
    bool isManager;
    // Login Credentials
    char login[30]; // Format : name-id (name will the first word in the structure member `name`)
    char password[30];
    
    // Bank data
    // int account; // Account number of the account the customer owns
};

#endif 