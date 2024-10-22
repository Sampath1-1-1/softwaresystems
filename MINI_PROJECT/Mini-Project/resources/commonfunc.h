#ifndef COMMON_FUNCTIONS
#define COMMON_FUNCTIONS

#include <stdio.h>     // Import for `printf` & `perror`
#include <unistd.h>    // Import for `read`, `write & `lseek`
#include <string.h>    // Import for string functions
#include <stdbool.h>   // Import for `bool` data type
#include <sys/types.h> // Import for `open`, `lseek`
#include <sys/stat.h>  // Import for `open`
#include <fcntl.h>     // Import for `open`
#include <stdlib.h>    // Import for `atoi`
#include <errno.h>     // Import for `errno`

#include "../record/account.h"
// #include "../record/customer.h"
#include "../record/transaction.h"
#include "../admin/admincred.h"
#include "./constantterm.h"
#include "../record/employee.h"
#include "../record/loanapply.h"
#include "../record/feedback.h"
// Function Prototypes =================================

bool login_handler(int userType, int connFD, struct Account *ptrToCustomerID,struct Employee *ptrToUserID);
bool get_account_details(int connFD, struct Account *customerAccount);
// bool get_customer_details(int connFD, int customerID);
int get_ID_INTEGER(char *readBuffer);
bool get_transaction_details(int connFD, int accountNumber);
// =====================================================

// Function Definition =================================
bool login_handler(int userType, int connFD, struct Account *ptrToCustomerID,struct Employee *ptrToUserID) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1500];
    char tempBuffer[1500];
    struct Account account;
    struct Employee employee;
    int ID;

    // File descriptors for customer and employee files
    int customerFileFD = -1, employeeFileFD = -1,managerFileFD=-1;

    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));

    // Send appropriate login welcome message based on userType
    switch (userType) {
        case 1: // Admin
            strcpy(writeBuffer, ADMIN_LOGIN_WELCOME);
            break;
        case 2: // Customer
            strcpy(writeBuffer, CUSTOMER_LOGIN_WELCOME);
            break;
        case 3: // Employee
            strcpy(writeBuffer, EMPLOYEE_LOGIN_WELCOME);
            break;
        case 4:
            strcpy(writeBuffer,MANAGER_LOGIN_WELCOME);
            break;
        default:
            perror("Invalid user type!");
            return false;
    }

    strcat(writeBuffer, "\n");
    strcat(writeBuffer, LOGIN_ID);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing login prompt to client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading login ID from client!");
        return false;
    }

    bool userFound = false;

    // Switch-case for handling login based on user type
    switch (userType) {
        case 1: // Admin login
            if (strcmp(readBuffer, ADMIN_LOGIN_ID) == 0) {
                userFound = true;
            }
            break;

        case 2: // Customer login
            // Handle customer file and login validation
            bzero(tempBuffer, sizeof(tempBuffer));
            strcpy(tempBuffer, readBuffer);

            ID = get_ID_INTEGER(tempBuffer); // Get ID from the input
            // bzero(writeBuffer,sizeof(writeBuffer));
            // sprintf(writeBuffer,"%d --- %s",ID,readBuffer);
            // write(STDOUT_FILENO,writeBuffer,strlen(writeBuffer));
            customerFileFD = open(ACCOUNT_FILE, O_RDONLY); // Open customer file
            if (customerFileFD == -1) {
                perror("Error opening customer file in read mode!");
                return false;
            }

            off_t offset = lseek(customerFileFD, ID * sizeof(struct Account), SEEK_SET);
            if (offset >= 0) {
                // Lock the file before reading
                struct flock lock = {F_RDLCK, SEEK_SET, ID * sizeof(struct Account), sizeof(struct Account), getpid()};
                int lockingStatus = fcntl(customerFileFD, F_SETLKW, &lock);
                if (lockingStatus == -1) {
                    perror("Error obtaining read lock on customer record!");
                    close(customerFileFD);
                    return false;
                }

                // Read the customer account from the file
                readBytes = read(customerFileFD, &account, sizeof(struct Account));
                if (readBytes == -1) {
                    perror("Error reading customer record from file!");
                    lock.l_type = F_UNLCK;
                    fcntl(customerFileFD, F_SETLK, &lock); // Unlock before returning
                    close(customerFileFD);
                    return false;
                }

                // Unlock the record after reading
                lock.l_type = F_UNLCK;
                fcntl(customerFileFD, F_SETLK, &lock);

                // Compare login ID
                            write(STDOUT_FILENO,account.login,strlen(account.login));
                if (strcmp(account.login, readBuffer) == 0) {
                    userFound = true;
                } else {
                    writeBytes = write(connFD, INVALID_LOGIN, strlen(INVALID_LOGIN));
                    close(customerFileFD);
                    return false;
                }
                close(customerFileFD); // Close customer file descriptor
            } else {
                writeBytes = write(connFD, CUSTOMER_LOGIN_ID_DOESNT_EXIT, strlen(CUSTOMER_LOGIN_ID_DOESNT_EXIT));
                return false;
            }
            break;

        case 3: // Employee login
            // Handle employee file and login validation
            bzero(tempBuffer, sizeof(tempBuffer));
            strcpy(tempBuffer, readBuffer);

            ID = get_ID_INTEGER(tempBuffer); // Get ID from the input

            employeeFileFD = open(EMPLOYEE_FILE, O_RDONLY); // Open employee file
            if (employeeFileFD == -1) {
                perror("Error opening employee file in read mode!");
                return false;
            }

            off_t empOffset = lseek(employeeFileFD, ID * sizeof(struct Employee), SEEK_SET);
            if (empOffset >= 0) {
                // Lock the employee file before reading
                struct flock empLock = {F_RDLCK, SEEK_SET, ID * sizeof(struct Employee), sizeof(struct Employee), getpid()};
                int empLockingStatus = fcntl(employeeFileFD, F_SETLKW, &empLock);
                if (empLockingStatus == -1) {
                    perror("Error obtaining read lock on employee record!");
                    close(employeeFileFD);
                    return false;
                }

                // Read the  Employee from the file
                readBytes = read(employeeFileFD, &employee, sizeof(struct Employee));
                if (readBytes == -1) {
                    perror("Error reading employee record from file!");
                    empLock.l_type = F_UNLCK;
                    fcntl(employeeFileFD, F_SETLK, &empLock); // Unlock before returning
                    close(employeeFileFD);
                    return false;
                }

                // Unlock the record after reading
                empLock.l_type = F_UNLCK;
                fcntl(employeeFileFD, F_SETLK, &empLock);

                // Compare login ID
                if (strcmp(employee.login, readBuffer) == 0) {
                    userFound = true;
                } else {
                    writeBytes = write(connFD, INVALID_LOGIN, strlen(INVALID_LOGIN));
                    close(employeeFileFD);
                    return false;
                }
                close(employeeFileFD); // Close employee file descriptor
            } else {
                writeBytes = write(connFD, EMPLOYEE_LOGIN_ID_DOESNT_EXIT, strlen(EMPLOYEE_LOGIN_ID_DOESNT_EXIT));
                return false;
            }
            break;

        case 4: // manager login
            // Handle manager file and login validation
            bzero(tempBuffer, sizeof(tempBuffer));
            strcpy(tempBuffer, readBuffer);

            ID = get_ID_INTEGER(tempBuffer); // Get ID from the input

            managerFileFD = open(EMPLOYEE_FILE, O_RDONLY); // Open employee file
            if (managerFileFD == -1) {
                perror("Error opening manager file in read mode!");
                return false;
            }

            empOffset = lseek(managerFileFD, ID * sizeof(struct Employee), SEEK_SET);
            if (empOffset >= 0) {
                // Lock the employee file before reading
                struct flock empLock = {F_RDLCK, SEEK_SET, ID * sizeof(struct Employee), sizeof(struct Employee), getpid()};
                int empLockingStatus = fcntl(managerFileFD, F_SETLKW, &empLock);
                if (empLockingStatus == -1) {
                    perror("Error obtaining read lock on manager record!");
                    close(managerFileFD);
                    return false;
                }

                // Read the  Employee from the file
                readBytes = read(managerFileFD, &employee, sizeof(struct Employee));
                if (readBytes == -1) {
                    perror("Error reading manager record from file!");
                    empLock.l_type = F_UNLCK;
                    fcntl(managerFileFD, F_SETLK, &empLock); // Unlock before returning
                    close(managerFileFD);
                    return false;
                }

                // Unlock the record after reading
                empLock.l_type = F_UNLCK;
                fcntl(managerFileFD, F_SETLK, &empLock);

                // Compare login ID
                if (strcmp(employee.login, readBuffer) == 0) {
                    userFound = true;
                } else {
                    writeBytes = write(connFD, INVALID_LOGIN, strlen(INVALID_LOGIN));
                    close(managerFileFD);
                    return false;
                }
                close(managerFileFD); // Close employee file descriptor
            } else {
                writeBytes = write(connFD, "MANAGER_LOGIN_ID_DOESNT_EXIT", strlen("MANAGER_LOGIN_ID_DOESNT_EXIT"));
                return false;
            }
            break;

    

        default:
            perror("Invalid user type!");
            return false;
    }

    if (userFound) {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, PASSWORD, strlen(PASSWORD));
        if (writeBytes == -1) {
            perror("Error writing password prompt to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error reading password from client!");
            return false;
        }

        char hashedPassword[1000];
        strcpy(hashedPassword, crypt(readBuffer, SALT_BAE)); // Hash the entered password

        // Check hashed password for admin, customer, or employee
        switch (userType) {
            case 1: // Admin password check
                if (strcmp(hashedPassword, ADMIN_PASSWORD) == 0) {
                    return true;
                }
                break;

            case 2: // Customer password check
               {
             if (strcmp(hashedPassword, account.password) == 0)
             {
                 *ptrToCustomerID = account;
                 return true;
             }
         }

            case 3: // Employee password check
                if (strcmp(hashedPassword, employee.password) == 0) {
                    *ptrToUserID = employee;
                    return true;
                }
                break;
             case 4: // manager password check
                if (strcmp(hashedPassword, employee.password) == 0) {
                    *ptrToUserID = employee;
                    return true;
                }
                break;    

            default:
                perror("Invalid user type!");
                return false;
        }

        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, INVALID_PASSWORD, strlen(INVALID_PASSWORD));
    }

    return false;
}


bool get_account_details(int connFD, struct Account *customerAccount)  // Null  is passed by admin
{
    ssize_t readBytes, writeBytes;            // Number of bytes read from / written to the socket
    char readBuffer[1000], writeBuffer[1000]; // A buffer for reading from / writing to the socket
    char tempBuffer[1000];

    int accountNumber;
    struct Account account;
    int accountFileDescriptor;

    if (customerAccount == NULL)
    {

        writeBytes = write(connFD, GET_ACCOUNT_NUMBER, strlen(GET_ACCOUNT_NUMBER));
        if (writeBytes == -1)
        {
            perror("Error writing GET_ACCOUNT_NUMBER message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading account number response from client!");
            return false;
        }
    
        accountNumber = get_ID_INTEGER(readBuffer);
    }
    else
        accountNumber = customerAccount->id;

    accountFileDescriptor = open(ACCOUNT_FILE, O_RDONLY);
    if (accountFileDescriptor == -1)
    {
        // Account record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        perror("Error opening account file in get_account_details!");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    int offset = lseek(accountFileDescriptor, accountNumber * sizeof(struct Account), SEEK_SET);
    if (offset == -1 && errno == EINVAL)
    {
        // Account record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        perror("Error seeking to account record in get_account_details!");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required account record!");
        return false;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};

    int lockingStatus = fcntl(accountFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error obtaining read lock on account record!");
        return false;
    }

    readBytes = read(accountFileDescriptor, &account, sizeof(struct Account));
    if (readBytes == -1)
    {
        perror("Error reading account record from file!");
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(accountFileDescriptor, F_SETLK, &lock);

    if (customerAccount != NULL)
    {
        *customerAccount = account;
        return true;
    }

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "Account Details - \n\tAccount Number : %d\n\tName : %s\n\tAccount Status : %s\n\tGender : %c\n\tLoginID : %s\n", account.id,account.name,(account.isActive?"Active":"Deactivated"),account.gender,account.login);
    if (account.isActive)
    {
        sprintf(tempBuffer, "\n\tAccount Balance:₹ %d", account.balance);
        strcat(writeBuffer, tempBuffer);
    }

    // sprintf(tempBuffer, "\n\tPrimary Owner ID: %d", account.owners[0]);
    //strcat(writeBuffer, tempBuffer);
   

    strcat(writeBuffer, "\n^");

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return true;
}


bool get_transaction_details(int connFD, int accountNumber)
{

    ssize_t readBytes, writeBytes;                               // Number of bytes read from / written to the socket
    char readBuffer[1000], writeBuffer[10000], tempBuffer[1000]; // A buffer for reading from / writing to the socket

    struct Account account;

    if (accountNumber == -1)
    {
        // Get the accountNumber
        writeBytes = write(connFD, GET_ACCOUNT_NUMBER, strlen(GET_ACCOUNT_NUMBER));
        if (writeBytes == -1)
        {
            perror("Error writing GET_ACCOUNT_NUMBER message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading account number response from client!");
            return false;
        }

        account.id = atoi(readBuffer);
    }
    else
        account.id = accountNumber;

    if (get_account_details(connFD, &account))
    {
        int iter;

        struct Transaction transaction;
        struct tm transactionTime;

        bzero(writeBuffer, sizeof(readBuffer));

        int transactionFileDescriptor = open(TRANSACTION_FILE, O_RDONLY);
        if (transactionFileDescriptor == -1)
        {
            perror("Error while opening transaction file!");
            write(connFD, TRANSACTIONS_NOT_FOUND, strlen(TRANSACTIONS_NOT_FOUND));
            read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
            return false;
        }

        for (iter = 0; iter < MAX_TRANSACTIONS && account.trancsaction[iter] != -1; iter++)
        {

            int offset = lseek(transactionFileDescriptor, account.trancsaction[iter] * sizeof(struct Transaction), SEEK_SET);
            if (offset == -1)
            {
                perror("Error while seeking to required transaction record!");
                return false;
            }

            struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Transaction), getpid()};

            int lockingStatus = fcntl(transactionFileDescriptor, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining read lock on transaction record!");
                return false;
            }

            readBytes = read(transactionFileDescriptor, &transaction, sizeof(struct Transaction));
            if (readBytes == -1)
            {
                perror("Error reading transaction record from file!");
                return false;
            }

            lock.l_type = F_UNLCK;
            fcntl(transactionFileDescriptor, F_SETLK, &lock);

            // transactionTime = *localtime(&(transaction.transactionTime));

            bzero(tempBuffer, sizeof(tempBuffer));
            sprintf(tempBuffer, "Details of transaction %d - \n\t Date : %s \n\t Operation : %s \n\t Balance - \n\t\t Before : %ld \n\t\t After : %ld \n\t\t Difference : %ld\n", (iter + 1), transaction.transactionTime, (transaction.operation ? "Deposit" : "Withdraw"), transaction.oldBalance, transaction.newBalance, (transaction.newBalance - transaction.oldBalance));

            if (strlen(writeBuffer) == 0)
                strcpy(writeBuffer, tempBuffer);
            else
                strcat(writeBuffer, tempBuffer);
        }

        close(transactionFileDescriptor);

        if (strlen(writeBuffer) == 0)
        {
            write(connFD, TRANSACTIONS_NOT_FOUND, strlen(TRANSACTIONS_NOT_FOUND));
            read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
            return false;
        }
        else
        {
            strcat(writeBuffer, "^");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        }
    }
}
// =====================================================
int get_ID_INTEGER(char *input){
    char *token;
    token = strtok(input, "-");
    token = strtok(NULL, "-");
    return atoi(token);
}
#endif