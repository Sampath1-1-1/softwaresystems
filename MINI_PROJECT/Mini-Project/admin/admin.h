#ifndef ADMIN_FUNCTIONS
#define ADMIN_FUNCTIONS



#define MAX_PASSWORD_LENGTH 100
#include "../resources/commonfunc.h"
#include <stdio.h>
#include <stdlib.h>
#include "../customer/customer.h"
#include "../record/employee.h"
#include "../record/loanapply.h"
#include "../record/feedback.h"

// Function Prototypes =================================

// #define SESSION_FILE "admin_session.txt"
bool admin_operation_handler(int connFD);
bool add_account(int connFD);
bool deactivate_account(int connFD);
bool modify_customer_info(int connFD);
bool modify_manager_roles(int connFD);
bool add_employee(int connFD);
bool change_admin_password_in_header(const char *newPassword);
bool change_admin_password(int connFD);
bool get_employee_details(int connFD, struct Employee *employeeAccount);
bool modify_employee_info(int connFD);


// bool change_password_admin(int connFD);
// bool lock_critical_section(struct sembuf *semOp);
// bool unlock_critical_section(struct sembuf *semOp);

/////////////////////////////////////////////

bool admin_operation_handler(int connFD)
{

    // int check= acheck();//
    // if(check==0){
    if (login_handler(1, connFD, NULL,NULL))
    {
        ssize_t writeData, readData;            // Number of bytes read from / written to the client
        char readCache[1500], writeCache[1500]; // A buffer used for reading & writing to the client
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, ADMIN_LOGIN_SUCCESS);
        while (1)
        {
            strcat(writeCache, "\n");
            strcat(writeCache, ADMIN_MENU);
            writeData = write(connFD, writeCache, strlen(writeCache));
            if (writeData == -1)
            {
                perror("Error while writing ADMIN_MENU to the client!");
                return false;
            }
            bzero(writeCache, sizeof(writeCache));
            bzero(readCache, sizeof(readCache));
            readData = read(connFD, readCache, sizeof(readCache));

            if (readData == -1)
            {
                perror("Error while reading client's choice for ADMIN_MENU");
                return false;
            }

            int choice = atoi(readCache);
            switch (choice)
            {
            case 1:
                add_account(connFD);
                break;
            case 2:
                modify_customer_info(connFD);
                break;
            case 3:
                get_account_details(connFD, NULL);
                break;
            case 4:
                get_transaction_details(connFD, -1);
                break;
            
            case 5:
                deactivate_account(connFD);
                break;
            case 6:
                add_employee(connFD);   
                break;
            case 7:
                modify_employee_info(connFD); 
                break;
            case 8:
                get_employee_details(connFD,NULL);
                break;
            case 9:
                modify_manager_roles(connFD);
                break;
          
            case 10:
                change_admin_password(connFD);
                break;
            default:
                writeData = write(connFD, ADMIN_LOGOUT, strlen(ADMIN_LOGOUT));
                return false;
            }
        }
        // return true;
    }
    else
    {
        // ADMIN LOGIN FAILED
        return false;
    }
    // }// end of check if
    return true;
}

bool add_account(int connFD)
{
    ssize_t readData, writeData;
    char readCache[1500], writeCache[1500];

    struct Account newAccount, previousAccount;

    int accountFileDescriptor = open(ACCOUNT_FILE, O_RDONLY); //| O_CREAT, 0666
    if (accountFileDescriptor == -1 && errno == ENOENT)
    {
        // Account file was never created
        newAccount.id = 0;
    }
    else if (accountFileDescriptor == -1)
    {
        perror("Error while opening  the account file");
        return false;
    }
    else
    {
        int filePtr = lseek(accountFileDescriptor, -sizeof(struct Account), SEEK_END);
        if (filePtr == -1)
        {
            perror("Error seeking to  the last Account record!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, filePtr, sizeof(struct Account), getpid()}; // why it is reading the lock if it is adding the  file
        int lockingStatus = fcntl(accountFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on the  Account record!");
            return false;
        }

        readData = read(accountFileDescriptor, &previousAccount, sizeof(struct Account));
        if (readData == -1)
        {
            perror("Error while reading the  Account record from  the file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(accountFileDescriptor, F_SETLK, &lock);

        close(accountFileDescriptor);

        newAccount.id = previousAccount.id + 1;
       
    }

    newAccount.isActive = true;
    newAccount.balance = 0;

    memset(newAccount.trancsaction, -1, MAX_TRANSACTIONS * sizeof(int));

    accountFileDescriptor = open(ACCOUNT_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (accountFileDescriptor == -1)
    {
        perror("Error while creating / opening the  account file!");
        return false;
    }
    // sprintf(writeCache, "%s", ADMIN_ADD_CUSTOMER_NAME);

    writeData = write(connFD, ADMIN_ADD_CUSTOMER_NAME, sizeof(ADMIN_ADD_CUSTOMER_NAME));
    if (writeData == -1)
    {
        perror("Error writing ADMIN_ADD_CUSTOMER_NAME message to  the client!");
        return false;
    }
    bzero(readCache,sizeof(readCache));
    readData = read(connFD, readCache, sizeof(readCache));
    if (readData == -1)
    {
        perror("Error reading customer name response from the  client!");
        return false;
    }
    write(STDOUT_FILENO,readCache,strlen(readCache));
    strcpy(newAccount.name, readCache);
    // read(connFD, readCache, sizeof(readCache));//dummy
write(STDOUT_FILENO,newAccount.name,strlen(newAccount.name));
    writeData = write(connFD, ADMIN_ADD_CUSTOMER_GENDER, strlen(ADMIN_ADD_CUSTOMER_GENDER));
    if (writeData == -1)
    {
        perror("Error writing ADMIN_ADD_CUSTOMER_GENDER message to the  client!");
        return false;
    }

    bzero(readCache, sizeof(readCache));
    readData = read(connFD, readCache, sizeof(readCache));
    if (readData == -1)
    {
        perror("Error reading customer gender response from the  client!");
        return false;
    }

    if (readCache[0] == 'M' || readCache[0] == 'F' || readCache[0] == 'O')
        newAccount.gender = readCache[0];
    else
    {
        writeData = write(connFD, ADMIN_ADD_CUSTOMER_WRONG_GENDER, strlen(ADMIN_ADD_CUSTOMER_WRONG_GENDER));
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }

    bzero(writeCache, sizeof(writeCache));
    strcpy(writeCache, ADMIN_ADD_CUSTOMER_AGE);
    writeData = write(connFD, writeCache, strlen(writeCache));
    if (writeData == -1)
    {
        perror("Error writing ADMIN_ADD_CUSTOMER_AGE message to  the client!");
        return false;
    }

    bzero(readCache, sizeof(readCache));
    readData = read(connFD, readCache, sizeof(readCache));
    if (readData == -1)
    {
        perror("Error reading customer age response from the  client!");
        return false;
    }

    int customerAge = atoi(readCache);
    if (customerAge == 0)
    {
        // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, ERRON_INPUT_FOR_NUMBER);
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing ERRON_INPUT_FOR_NUMBER message to  the client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }
    newAccount.age = customerAge;

    //  newCustomer.account = newAccountNumber;

    strcpy(newAccount.login, newAccount.name);
    strcat(newAccount.login, "-");
    sprintf(writeCache, "%d", newAccount.id);
    strcat(newAccount.login, writeCache);

    char hashedPassword[1000];
    strcpy(hashedPassword, crypt(AUTOGEN_PASSWORD, SALT_BAE));
    strcpy(newAccount.password, hashedPassword);
    write(connFD, newAccount.login, strlen(newAccount.login));
    writeData = write(accountFileDescriptor, &newAccount, sizeof(struct Account));
    if (writeData == -1)
    {
        perror("Error while writing Account record to file!");
        return false;
    }

    close(accountFileDescriptor);

    bzero(writeCache, sizeof(writeCache));
    sprintf(writeCache, "%s%d", ADMIN_ADD_ACCOUNT_NUMBER, newAccount.id);
    strcat(writeCache, "\nRedirecting you to the main menu ...^");
    writeData = write(connFD, writeCache, sizeof(writeCache));
    readData = read(connFD, readCache, sizeof(read)); // Dummy read
    bzero(readCache, sizeof(readCache));
    return true;
}



// bool add_account(int connectionFD)
// {
//     ssize_t bytesRead, bytesWritten;
//     char inputBuffer[1500], outputBuffer[1500];

//     struct Account newAcc, lastAcc;

//     int accFileDesc = open(ACCOUNT_FILE, O_RDONLY);
//     if (accFileDesc == -1 && errno == ENOENT)
//     {
//         // If the account file doesn't exist
//         newAcc.id = 0;
//     }
//     else if (accFileDesc == -1)
//     {
//         perror("Error opening account file");
//         return false;
//     }
//     else
//     {
//         int pointerPos = lseek(accFileDesc, -sizeof(struct Account), SEEK_END);
//         if (pointerPos == -1)
//         {
//             perror("Error seeking last account record!");
//             return false;
//         }

//         struct flock readLock = {F_RDLCK, SEEK_SET, pointerPos, sizeof(struct Account), getpid()}; // Read lock for safe access
//         int lockStatus = fcntl(accFileDesc, F_SETLKW, &readLock);
//         if (lockStatus == -1)
//         {
//             perror("Error obtaining read lock on account record!");
//             return false;
//         }

//         bytesRead = read(accFileDesc, &lastAcc, sizeof(struct Account));
//         if (bytesRead == -1)
//         {
//             perror("Error reading account record!");
//             return false;
//         }

//         // Unlock after reading
//         readLock.l_type = F_UNLCK;
//         fcntl(accFileDesc, F_SETLK, &readLock);

//         close(accFileDesc);

//         // Increment account ID
//         newAcc.id = lastAcc.id + 1;
//     }

//     newAcc.isActive = true;
//     newAcc.balance = 0;
//     memset(newAcc.trancsaction, -1, MAX_TRANSACTIONS * sizeof(int));

//     accFileDesc = open(ACCOUNT_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
//     if (accFileDesc == -1)
//     {
//         perror("Error creating/opening account file!");
//         return false;
//     }

//     sprintf(outputBuffer, "%s", ADMIN_ADD_CUSTOMER_NAME);
//     bytesWritten = write(connectionFD, outputBuffer, sizeof(outputBuffer));
//     if (bytesWritten == -1)
//     {
//         perror("Error writing customer name request to client!");
//         return false;
//     }

//     bytesRead = read(connectionFD, inputBuffer, sizeof(inputBuffer));
//     if (bytesRead == -1)
//     {
//         perror("Error reading customer name response from client!");
//         return false;
//     }

//     strcpy(newAcc.name, inputBuffer);
//     bytesWritten = write(connectionFD, ADMIN_ADD_CUSTOMER_GENDER, strlen(ADMIN_ADD_CUSTOMER_GENDER));
//     if (bytesWritten == -1)
//     {
//         perror("Error writing gender request to client!");
//         return false;
//     }

//     bzero(inputBuffer, sizeof(inputBuffer));
//     bytesRead = read(connectionFD, inputBuffer, sizeof(inputBuffer));
//     if (bytesRead == -1)
//     {
//         perror("Error reading gender response from client!");
//         return false;
//     }

//     // Validate gender
//     if (inputBuffer[0] == 'M' || inputBuffer[0] == 'F' || inputBuffer[0] == 'O')
//         newAcc.gender = inputBuffer[0];
//     else
//     {
//         bytesWritten = write(connectionFD, ADMIN_ADD_CUSTOMER_WRONG_GENDER, strlen(ADMIN_ADD_CUSTOMER_WRONG_GENDER));
//         read(connectionFD, inputBuffer, sizeof(inputBuffer)); // Dummy read
//         return false;
//     }

//     bzero(outputBuffer, sizeof(outputBuffer));
//     strcpy(outputBuffer, ADMIN_ADD_CUSTOMER_AGE);
//     bytesWritten = write(connectionFD, outputBuffer, strlen(outputBuffer));
//     if (bytesWritten == -1)
//     {
//         perror("Error writing age request to client!");
//         return false;
//     }

//     bzero(inputBuffer, sizeof(inputBuffer));
//     bytesRead = read(connectionFD, inputBuffer, sizeof(inputBuffer));
//     if (bytesRead == -1)
//     {
//         perror("Error reading age response from client!");
//         return false;
//     }

//     int customerAge = atoi(inputBuffer);
//     if (customerAge == 0)
//     {
//         // Invalid age or non-numeric input
//         bzero(outputBuffer, sizeof(outputBuffer));
//         strcpy(outputBuffer, ERRON_INPUT_FOR_NUMBER);
//         bytesWritten = write(connectionFD, outputBuffer, strlen(outputBuffer));
//         if (bytesWritten == -1)
//         {
//             perror("Error writing invalid input message to client!");
//             return false;
//         }
//         read(connectionFD, inputBuffer, sizeof(inputBuffer)); // Dummy read
//         return false;
//     }
//     newAcc.age = customerAge;

//     // Create login credentials
//     strcpy(newAcc.login, newAcc.name);
//     strcat(newAcc.login, "-");
//     sprintf(outputBuffer, "%d", newAcc.id);
//     strcat(newAcc.login, outputBuffer);

//     // Hashing the password
//     char hashedPwd[1000];
//     strcpy(hashedPwd, crypt(AUTOGEN_PASSWORD, SALT_BAE));
//     strcpy(newAcc.password, hashedPwd);

//     // Sending login details to the client
//     write(connectionFD, newAcc.login, strlen(newAcc.login));
//     bytesWritten = write(accFileDesc, &newAcc, sizeof(struct Account));
//     if (bytesWritten == -1)
//     {
//         perror("Error writing account record to file!");
//         return false;
//     }

//     close(accFileDesc);

//     bzero(outputBuffer, sizeof(outputBuffer));
//     sprintf(outputBuffer, "%s%d", ADMIN_ADD_ACCOUNT_NUMBER, newAcc.id);
//     strcat(outputBuffer, "\nRedirecting to main menu...");
//     write(connectionFD, outputBuffer, sizeof(outputBuffer));
//     read(connectionFD, inputBuffer, sizeof(inputBuffer)); // Dummy read
//     bzero(inputBuffer, sizeof(inputBuffer));

//     return true;
// }


bool deactivate_account(int connFD)
{
    ssize_t readData, writeData;
    char readCache[1000], writeCache[1000];

    struct Account account;

    writeData = write(connFD, ADMIN_DEL_ACCOUNT_NO, strlen(ADMIN_DEL_ACCOUNT_NO));
    if (writeData == -1)
    {
        perror("Error writing ADMIN_DEL_ACCOUNT_NO to client!");
        return false;
    }

    bzero(readCache, sizeof(readCache));
    readData = read(connFD, readCache, sizeof(readCache));
    if (readData == -1)
    {
        perror("Error reading account number response from the client!");
        return false;
    }

    int accountNumber = atoi(readCache);

    int accountFileDescriptor = open(ACCOUNT_FILE, O_RDONLY);
    if (accountFileDescriptor == -1)
    {
        // Account record doesn't exist
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeCache, "^");
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }

    int filePtr = lseek(accountFileDescriptor, accountNumber * sizeof(struct Account), SEEK_SET);
    if (errno == EINVAL)
    {
        // Customer record doesn't exist
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, ACCOUNT_ID_DOESNT_EXIT);
        strcat(writeCache, "^");
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }
    else if (filePtr == -1)
    {
        perror("Error while seeking to required account record!");
        return false;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, filePtr, sizeof(struct Account), getpid()};
    int lockingStatus = fcntl(accountFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error obtaining read lock on Account record!");
        return false;
    }

    readData = read(accountFileDescriptor, &account, sizeof(struct Account));
    if (readData == -1)
    {
        perror("Error while reading Account record from file!");
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(accountFileDescriptor, F_SETLK, &lock);

    close(accountFileDescriptor);

    bzero(writeCache, sizeof(writeCache));
    if (account.balance == 0)
    {
        // No money, hence can close account
        account.isActive = false;
        accountFileDescriptor = open(ACCOUNT_FILE, O_WRONLY);
        if (accountFileDescriptor == -1)
        {
            perror("Error opening Account file in write mode!");
            return false;
        }

        filePtr = lseek(accountFileDescriptor, accountNumber * sizeof(struct Account), SEEK_SET);
        if (filePtr == -1)
        {
            perror("Error seeking to the Account!");
            return false;
        }

        lock.l_type = F_WRLCK;
        lock.l_start = filePtr;

        int lockingStatus = fcntl(accountFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining write lock on the Account file!");
            return false;
        }

        writeData = write(accountFileDescriptor, &account, sizeof(struct Account));
        if (writeData == -1)
        {
            perror("Error deleting account record!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(accountFileDescriptor, F_SETLK, &lock);

        strcpy(writeCache, ADMIN_DEL_ACCOUNT_SUCCESS);
    }
    else
        // Account has some money ask customer to withdraw it
        strcpy(writeCache, ADMIN_DEL_ACCOUNT_FAILURE);
    writeData = write(connFD, writeCache, strlen(writeCache));
    if (writeData == -1)
    {
        perror("Error while writing final DEL message to client!");
        return false;
    }
    readData = read(connFD, readCache, sizeof(readCache)); // Dummy read

    return true;
}

bool modify_customer_info(int connFD)
{
    ssize_t readData, writeData;
    char readCache[1000], writeCache[1000];

    struct Account account;

    int customerID;

    off_t filePtr;
    int lockingStatus;

    writeData = write(connFD, ADMIN_MOD_CUSTOMER_ID, strlen(ADMIN_MOD_CUSTOMER_ID));
    if (writeData == -1)
    {
        perror("Error while writing ADMIN_MOD_CUSTOMER_ID message to client!");
        return false;
    }
    bzero(readCache, sizeof(readCache));
    readData = read(connFD, readCache, sizeof(readCache));
    if (readData == -1)
    {
        perror("Error while reading customer ID from client!");
        return false;
    }

    customerID = get_ID_INTEGER(readCache);

    int customerFileDescriptor = open(ACCOUNT_FILE, O_RDONLY);
    if (customerFileDescriptor == -1)
    {
        // Customer File doesn't exist
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, CUSTOMER_ID_DOESNT_EXIT);
        strcat(writeCache, "^");
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }

    filePtr = lseek(customerFileDescriptor, customerID * sizeof(struct Account), SEEK_SET);
    if (errno == EINVAL)
    {
        // Customer record doesn't exist
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, CUSTOMER_ID_DOESNT_EXIT);
        strcat(writeCache, "^");
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }
    else if (filePtr == -1)
    {
        perror("Error while seeking to required customer record!");
        return false;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, filePtr, sizeof(struct Account), getpid()};

    // Lock the record to be read
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Couldn't obtain lock on customer record!");
        return false;
    }

    readData = read(customerFileDescriptor, &account, sizeof(struct Account));
    if (readData == -1)
    {
        perror("Error while reading customer record from the file!");
        return false;
    }

    // Unlock the record
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);

    close(customerFileDescriptor);

    writeData = write(connFD, ADMIN_MOD_CUSTOMER_MENU, strlen(ADMIN_MOD_CUSTOMER_MENU));
    if (writeData == -1)
    {
        perror("Error while writing ADMIN_MOD_CUSTOMER_MENU message to client!");
        return false;
    }
    readData = read(connFD, readCache, sizeof(readCache));
    if (readData == -1)
    {
        perror("Error while getting customer modification menu choice from client!");
        return false;
    }

    int choice = atoi(readCache);
    if (choice == 0)
    { // A non-numeric string was passed to atoi
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, ERRON_INPUT_FOR_NUMBER);
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }

    bzero(readCache, sizeof(readCache));
    switch (choice)
    {
    case 1: /////////////////////////////////////// change name///////////////////////////////////
        writeData = write(connFD, ADMIN_MOD_CUSTOMER_NEW_NAME, strlen(ADMIN_MOD_CUSTOMER_NEW_NAME));
        if (writeData == -1)
        {
            perror("Error while writing ADMIN_MOD_CUSTOMER_NEW_NAME message to client!");
            return false;
        }
        readData = read(connFD, &readCache, sizeof(readCache));
        if (readData == -1)
        {
            perror("Error while getting response for customer's new name from client!");
            return false;
        }
        strcpy(account.name, readCache);
        break;
    case 2: ////////////////////////////////////////chavne age/////////////////////////////////
        writeData = write(connFD, ADMIN_MOD_CUSTOMER_NEW_AGE, strlen(ADMIN_MOD_CUSTOMER_NEW_AGE));
        if (writeData == -1)
        {
            perror("Error while writing ADMIN_MOD_CUSTOMER_NEW_AGE message to client!");
            return false;
        }
        readData = read(connFD, &readCache, sizeof(readCache));
        if (readData == -1)
        {
            perror("Error while getting response for customer's new age from client!");
            return false;
        }
        int updatedAge = atoi(readCache);
        if (updatedAge == 0)
        {
            // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
            bzero(writeCache, sizeof(writeCache));
            strcpy(writeCache, ERRON_INPUT_FOR_NUMBER);
            writeData = write(connFD, writeCache, strlen(writeCache));
            if (writeData == -1)
            {
                perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
                return false;
            }
            readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
            return false;
        }
        account.age = updatedAge;
        break;
    case 3://///////////////////////////////////change gender/////////////////////////////////
        writeData = write(connFD, ADMIN_MOD_CUSTOMER_NEW_GENDER, strlen(ADMIN_MOD_CUSTOMER_NEW_GENDER));
        if (writeData == -1)
        {
            perror("Error while writing ADMIN_MOD_CUSTOMER_NEW_GENDER message to client!");
            return false;
        }
        readData = read(connFD, &readCache, sizeof(readCache));
        if (readData == -1)
        {
            perror("Error while getting response for customer's new gender from client!");
            return false;
        }
        account.gender = readCache[0];
        break;
    case 4: // to update password ///////////////////////////////////////////////////////////////////////////////

                    writeData = write(connFD, "Password is being resetted", strlen("Password is being resetted"));
        if (writeData == -1)
        {
            perror("Error while writing ADMIN_MOD_CUSTOMER_NEW_Password message to client!");
            return false;
        }
                //     char hashedPassword[1000];
                //  strcpy(hashedPassword, );//
         strcpy(account.password, crypt(AUTOGEN_PASSWORD, SALT_BAE));

        break; 

    default: //////////////////////////////////////////////////////////////////////////////
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, INVALID_MENU_CHOICE);
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing INVALID_MENU_CHOICE message to client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }
        // saving the struct back to the main file
    customerFileDescriptor = open(ACCOUNT_FILE, O_WRONLY);
    if (customerFileDescriptor == -1)
    {
        perror("Error while opening customer file");
        return false;
    }
    filePtr = lseek(customerFileDescriptor, customerID * sizeof(struct Account), SEEK_SET);
    if (filePtr == -1)
    {
        perror("Error while seeking to required customer record!");
        return false;
    }

    lock.l_type = F_WRLCK;
    lock.l_start = filePtr;
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error while obtaining write lock on customer record!");
        return false;
    }

    writeData = write(customerFileDescriptor, &account, sizeof(struct Account));
    if (writeData == -1)
    {
        perror("Error while writing update customer info into file");
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLKW, &lock);

    close(customerFileDescriptor);

    writeData = write(connFD, ADMIN_MOD_CUSTOMER_SUCCESS, strlen(ADMIN_MOD_CUSTOMER_SUCCESS));
    if (writeData == -1)
    {
        perror("Error while writing ADMIN_MOD_CUSTOMER_SUCCESS message to client!");
        return false;
    }
    readData = read(connFD, readCache, sizeof(readCache)); // Dummy read

    return true;
}

bool add_employee(int connFD){

         ssize_t readData, writeData;
    char readCache[1000], writeCache[1000];

    struct Employee newEmployee, prevEmployee;

    int employeeFileDescriptor = open(EMPLOYEE_FILE, O_RDONLY); //| O_CREAT, 0666
    if (employeeFileDescriptor == -1 && errno == ENOENT)
    {
        // Employee file was never created
        newEmployee.id = 0;
    }
    else if (employeeFileDescriptor == -1)
    {
        perror("Error while opening employee file");
        return false;
    }
    else
    {
        int filePtr = lseek(employeeFileDescriptor, -sizeof(struct Employee), SEEK_END);
        if (filePtr == -1)
        {
            perror("Error seeking to last Employee record!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, filePtr, sizeof(struct Employee), getpid()}; // why it is reading the lock if it is adding the  file
        int lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Employee record!");
            return false;
        }

        readData = read(employeeFileDescriptor, &prevEmployee, sizeof(struct Employee));
        if (readData == -1)
        {
            perror("Error while reading Employee record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(employeeFileDescriptor, F_SETLK, &lock);

        close(employeeFileDescriptor);

        newEmployee.id = prevEmployee.id + 1;
       
    }

    newEmployee.isManager = false;
    // newEmployee.balance = 0;

    // memset(newEmployee.trancsaction, -1, MAX_TRANSACTIONS * sizeof(int));

    employeeFileDescriptor = open(EMPLOYEE_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (employeeFileDescriptor == -1)
    {
        perror("Error while creating / opening employee file!");
        return false;
    }
    sprintf(writeCache, "%s", ADMIN_ADD_EMPLOYEE_NAME);

    writeData = write(connFD, writeCache, sizeof(writeCache));
    if (writeData == -1)
    {
        perror("Error writing ADMIN_ADD_EMPLOYEE_NAME message to client!");
        return false;
    }

    readData = read(connFD, readCache, sizeof(readCache));
    if (readData == -1)
    {
        perror("Error reading employee name response from client!");
        
        return false;
    }

    strcpy(newEmployee.name, readCache);
    writeData = write(connFD, ADMIN_ADD_EMPLOYEE_GENDER, strlen(ADMIN_ADD_EMPLOYEE_GENDER));
    if (writeData == -1)
    {
        perror("Error writing ADMIN_ADD_EMPLOYEE_GENDER message to client!");
        return false;
    }

    bzero(readCache, sizeof(readCache));
    readData = read(connFD, readCache, sizeof(readCache));
    if (readData == -1)
    {
        perror("Error reading employee gender response from client!");
        return false;
    }

    if (readCache[0] == 'M' || readCache[0] == 'F' || readCache[0] == 'O')
        newEmployee.gender = readCache[0];
    else
    {
        writeData = write(connFD, ADMIN_ADD_EMPLOYEE_WRONG_GENDER, strlen(ADMIN_ADD_EMPLOYEE_WRONG_GENDER));
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }

    bzero(writeCache, sizeof(writeCache));
    strcpy(writeCache, ADMIN_ADD_EMPLOYEE_AGE);
    writeData = write(connFD, writeCache, strlen(writeCache));
    if (writeData == -1)
    {
        perror("Error writing ADMIN_ADD_EMPLOYEE_AGE message to client!");
        return false;
    }

    bzero(readCache, sizeof(readCache));
    readData = read(connFD, readCache, sizeof(readCache));
    if (readData == -1)
    {
        perror("Error reading employee age response from client!");
        return false;
    }

    int employeeAge = atoi(readCache);
    if (employeeAge == 0)
    {
        // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, ERRON_INPUT_FOR_NUMBER);
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }
    newEmployee.age = employeeAge;

    //  newemployee.employee = newEmployeeNumber;

    strcpy(newEmployee.login, newEmployee.name);
    strcat(newEmployee.login, "-");
    sprintf(writeCache, "%d", newEmployee.id);
    strcat(newEmployee.login, writeCache);

    char hashedPassword[1000];
    strcpy(hashedPassword, crypt(AUTOGEN_PASSWORD, SALT_BAE));
    strcpy(newEmployee.password, hashedPassword);
    write(connFD, newEmployee.login, strlen(newEmployee.login));
    writeData = write(employeeFileDescriptor, &newEmployee, sizeof(struct Employee));
    if (writeData == -1)
    {
        perror("Error while writing Employee record to file!");
        return false;
    }

    close(employeeFileDescriptor);

    bzero(writeCache, sizeof(writeCache));
    sprintf(writeCache, "%s%d", ADMIN_ADD_EMPLOYEE_NUMBER, newEmployee.id);
    strcat(writeCache, "\nRedirecting you to the main menu ...^");
    writeData = write(connFD, writeCache, sizeof(writeCache));
    readData = read(connFD, readCache, sizeof(read)); // Dummy read
    bzero(readCache, sizeof(readCache));
    return true;

}

bool modify_employee_info(int connFD){

    ssize_t readData, writeData;
    char readCache[1000], writeCache[1000];

    struct Employee employee;

    int employeeID;

    off_t filePtr;
    int lockingStatus;

    writeData = write(connFD, ADMIN_MOD_EMPLOYEE_ID, strlen(ADMIN_MOD_EMPLOYEE_ID));
    if (writeData == -1)
    {
        perror("Error while writing ADMIN_MOD_EMPLOYEE_ID message to client!");
        return false;
    }
    bzero(readCache, sizeof(readCache));
    readData = read(connFD, readCache, sizeof(readCache));
    if (readData == -1)
    {
        perror("Error while reading employee ID from client!");
        return false;
    }

    employeeID = atoi(readCache);

    int employeeFileDescriptor = open(EMPLOYEE_FILE, O_RDONLY);
    if (employeeFileDescriptor == -1)
    {
        // Customer File doesn't exist
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, EMPLOYEE_ID_DOESNT_EXIT);
        strcat(writeCache, "^");
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing EMPLOYEE_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }

    filePtr = lseek(employeeFileDescriptor, employeeID * sizeof(struct Employee), SEEK_SET);
    if (errno == EINVAL)
    {
        // Customer record doesn't exist
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, EMPLOYEE_ID_DOESNT_EXIT);
        strcat(writeCache, "^");
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing EMPLOYEE_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }
    else if (filePtr == -1)
    {
        perror("Error while seeking to required employee record!");
        return false;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, filePtr, sizeof(struct Employee), getpid()};

    // Lock the record to be read
    lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Couldn't obtain lock on employee record!");
        return false;
    }

    readData = read(employeeFileDescriptor, &employee, sizeof(struct Employee));
    if (readData == -1)
    {
        perror("Error while reading employee record from the file!");
        return false;
    }

    // Unlock the record
    lock.l_type = F_UNLCK;
    fcntl(employeeFileDescriptor, F_SETLK, &lock);

    close(employeeFileDescriptor);

    writeData = write(connFD, ADMIN_MOD_EMPLOYEE_MENU, strlen(ADMIN_MOD_EMPLOYEE_MENU));
    if (writeData == -1)
    {
        perror("Error while writing ADMIN_MOD_EMPLOYEE_MENU message to client!");
        return false;
    }
    readData = read(connFD, readCache, sizeof(readCache));
    if (readData == -1)
    {
        perror("Error while getting employee modification menu choice from client!");
        return false;
    }

    int choice = atoi(readCache);
    if (choice == 0)
    { // A non-numeric string was passed to atoi
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, ERRON_INPUT_FOR_NUMBER);
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }

    bzero(readCache, sizeof(readCache));
    switch (choice)
    {
    case 1: /////////////////////////////////////// change name///////////////////////////////////
        writeData = write(connFD, ADMIN_MOD_EMPLOYEE_NEW_NAME, strlen(ADMIN_MOD_EMPLOYEE_NEW_NAME));
        if (writeData == -1)
        {
            perror("Error while writing ADMIN_MOD_EMPLOYEE_NEW_NAME message to client!");
            return false;
        }
        readData = read(connFD, &readCache, sizeof(readCache));
        if (readData == -1)
        {
            perror("Error while getting response for employee's new name from client!");
            return false;
        }
        strcpy(employee.name, readCache);
        break;
    case 2: ////////////////////////////////////////chavne age/////////////////////////////////
        writeData = write(connFD, ADMIN_MOD_EMPLOYEE_NEW_AGE, strlen(ADMIN_MOD_EMPLOYEE_NEW_AGE));
        if (writeData == -1)
        {
            perror("Error while writing ADMIN_MOD_EMPLOYEE_NEW_AGE message to client!");
            return false;
        }
        readData = read(connFD, &readCache, sizeof(readCache));
        if (readData == -1)
        {
            perror("Error while getting response for employee's new age from client!");
            return false;
        }
        int updatedAge = atoi(readCache);
        if (updatedAge == 0)
        {
            // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
            bzero(writeCache, sizeof(writeCache));
            strcpy(writeCache, ERRON_INPUT_FOR_NUMBER);
            writeData = write(connFD, writeCache, strlen(writeCache));
            if (writeData == -1)
            {
                perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
                return false;
            }
            readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
            return false;
        }
        employee.age = updatedAge;
        break;
    case 3://///////////////////////////////////change gender/////////////////////////////////
        writeData = write(connFD, ADMIN_MOD_EMPLOYEE_NEW_GENDER, strlen(ADMIN_MOD_EMPLOYEE_NEW_GENDER));
        if (writeData == -1)
        {
            perror("Error while writing ADMIN_MOD_EMPLOYEE_NEW_GENDER message to client!");
            return false;
        }
        readData = read(connFD, &readCache, sizeof(readCache));
        if (readData == -1)
        {
            perror("Error while getting response for employee's new gender from client!");
            return false;
        }
        employee.gender = readCache[0];
        break;
    case 4: // to update password ///////////////////////////////////////////////////////////////////////////////

                    writeData = write(connFD, "Password is being resetted", strlen("Password is being resetted"));
        if (writeData == -1)
        {
            perror("Error while writing ADMIN_MOD_EMPLOYEE_NEW_Password message to client!");
            return false;
        }
                //     char hashedPassword[1000];
                //  strcpy(hashedPassword, );//
         strcpy(employee.password, crypt(AUTOGEN_PASSWORD, SALT_BAE));

        break; //////////////////////////////////////////////////////////////////////////////////////////////////////////

    default: //////////////////////////////////////////////////////////////////////////////
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, INVALID_MENU_CHOICE);
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing INVALID_MENU_CHOICE message to client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }
        // saving the struct back to the main file
    employeeFileDescriptor = open(EMPLOYEE_FILE, O_WRONLY);
    if (employeeFileDescriptor == -1)
    {
        perror("Error while opening employee file");
        return false;
    }
    filePtr = lseek(employeeFileDescriptor, employeeID * sizeof(struct Employee), SEEK_SET);
    if (filePtr == -1)
    {
        perror("Error while seeking to required employee record!");
        return false;
    }

    lock.l_type = F_WRLCK;
    lock.l_start = filePtr;
    lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error while obtaining write lock on employee record!");
        return false;
    }

    writeData = write(employeeFileDescriptor, &employee, sizeof(struct Employee));
    if (writeData == -1)
    {
        perror("Error while writing update employee info into file");
    }

    lock.l_type = F_UNLCK;
    fcntl(employeeFileDescriptor, F_SETLKW, &lock);

    close(employeeFileDescriptor);

    writeData = write(connFD, ADMIN_MOD_EMPLOYEE_SUCCESS, strlen(ADMIN_MOD_EMPLOYEE_SUCCESS));
    if (writeData == -1)
    {
        perror("Error while writing ADMIN_MOD_EMPLOYEE_SUCCESS message to client!");
        return false;
    }
    readData = read(connFD, readCache, sizeof(readCache)); // Dummy read

    return true;






} 

bool get_employee_details(int connFD, struct Employee *employeeAccount)  // Null  is passed by admin
{
    ssize_t readData, writeData;            // Number of bytes read from / written to the socket
    char readCache[1000], writeCache[1000]; // A buffer for reading from / writing to the socket
    char tempBuffer[1000];

    int employeeNumber;
    struct Employee employee;
    int employeeFileDescriptor;

    if (employeeAccount == NULL)
    {

        writeData = write(connFD, GET_EMPLOYEE_NUMBER, strlen(GET_EMPLOYEE_NUMBER));
        if (writeData == -1)
        {
            perror("Error writing GET_ACCOUNT_NUMBER message to client!");
            return false;
        }

        bzero(readCache, sizeof(readCache));
        readData = read(connFD, readCache, sizeof(readCache));
        if (readData == -1)
        {
            perror("Error reading employee number response from client!");
            return false;
        }
    
        employeeNumber = get_ID_INTEGER(readCache);
    }
    else
        employeeNumber = employeeAccount->id;

    employeeFileDescriptor = open(EMPLOYEE_FILE, O_RDONLY);
    if (employeeFileDescriptor == -1)
    {
        // Employee record doesn't exist
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, EMPLOYEE_ID_DOESNT_EXIT);
        strcat(writeCache, "^");
        perror("Error opening employee file in get_employee_details!");
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }

    int filePtr = lseek(employeeFileDescriptor, employeeNumber * sizeof(struct Employee), SEEK_SET);
    if (filePtr == -1 && errno == EINVAL)
    {
        // Employee record doesn't exist
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, EMPLOYEE_ID_DOESNT_EXIT);
        strcat(writeCache, "^");
        perror("Error seeking to employee record in get_employee_details!");
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }
    else if (filePtr == -1)
    {
        perror("Error while seeking to required employee record!");
        return false;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, filePtr, sizeof(struct Employee), getpid()};

    int lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error obtaining read lock on employee record!");
        return false;
    }

    readData = read(employeeFileDescriptor, &employee, sizeof(struct Employee));
    if (readData == -1)
    {
        perror("Error reading employee record from file!");
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(employeeFileDescriptor, F_SETLK, &lock);

    if (employeeAccount != NULL)
    {
        *employeeAccount = employee;
        return true;
    }

    bzero(writeCache, sizeof(writeCache));
    sprintf(writeCache, "Employee Details - \n\tEmployee Number : %d\n\tName : %s\n\tRole Status : %s\n\tGender : %c\n\tLoginID : %s\n", employee.id,employee.name,(employee.isManager?"Manager":"Employee"),employee.gender,employee.login);
    // if (employee.isActive)
    // {
    //     sprintf(tempBuffer, "\n\tAccount Balance:₹ %d", employee.balance);
    //     strcat(writeCache, tempBuffer);
    // }

    // sprintf(tempBuffer, "\n\tPrimary Owner ID: %d", employee.owners[0]);
    // strcat(writeCache, tempBuffer);
   

    strcat(writeCache, "\n^");

    writeData = write(connFD, writeCache, strlen(writeCache));
    readData = read(connFD, readCache, sizeof(readCache)); // Dummy read

    return true;
}

bool modify_manager_roles(int connFD)
{
    ssize_t readData, writeData;
    char readCache[1000], writeCache[1000];

    struct Employee employee;

    writeData = write(connFD, ADMIN_MOD_EMPLOYEE_NO, strlen(ADMIN_MOD_EMPLOYEE_NO));
    if (writeData == -1)
    {
        perror("Error writing ADMIN_MOD_EMPLOYEE_NO to client!");
        return false;
    }

    bzero(readCache, sizeof(readCache));
    readData = read(connFD, readCache, sizeof(readCache));
    if (readData == -1)
    {
        perror("Error reading employee number response from the client!");
        return false;
    }
    char t[1000];
    strcpy(t,readCache);
    int employeeNumber= get_ID_INTEGER(t);
    // int employeeNumber = atoi(readCache);

    int employeeFileDescriptor = open(EMPLOYEE_FILE, O_RDONLY);
    if (employeeFileDescriptor == -1)
    {
        // Employee record doesn't exist
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, EMPLOYEE_ID_DOESNT_EXIT);
        strcat(writeCache, "^");
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing EMPLOYEE_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }

    int filePtr = lseek(employeeFileDescriptor, employeeNumber * sizeof(struct Employee), SEEK_SET);
    if (errno == EINVAL)
    {
        // Customer record doesn't exist
        bzero(writeCache, sizeof(writeCache));
        strcpy(writeCache, EMPLOYEE_ID_DOESNT_EXIT);
        strcat(writeCache, "^");
        writeData = write(connFD, writeCache, strlen(writeCache));
        if (writeData == -1)
        {
            perror("Error while writing EMPLOYEE_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readData = read(connFD, readCache, sizeof(readCache)); // Dummy read
        return false;
    }
    else if (filePtr == -1)
    {
        perror("Error while seeking to required employee record!");
        return false;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, filePtr, sizeof(struct Employee), getpid()};
    int lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error obtaining read lock on Employee record!");
        return false;
    }

    readData = read(employeeFileDescriptor, &employee, sizeof(struct Employee));
    if (readData == -1)
    {
        perror("Error while reading Employee record from file!");
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(employeeFileDescriptor, F_SETLK, &lock);

    close(employeeFileDescriptor);

    bzero(writeCache, sizeof(writeCache));
    
        if(employee.isManager){ //////////////////////////////////////////////
              employee.isManager=false;
        } 
        else{
              employee.isManager=true;
        
        }///////////////////////////////////////////////////////////
        employeeFileDescriptor = open(EMPLOYEE_FILE, O_WRONLY);
        if (employeeFileDescriptor == -1)
        {
            perror("Error opening Employee file in write mode!");
            return false;
        }

        filePtr = lseek(employeeFileDescriptor, employeeNumber * sizeof(struct Employee), SEEK_SET);
        if (filePtr == -1)
        {
            perror("Error seeking to the Employee!");
            return false;
        }

        lock.l_type = F_WRLCK;
        lock.l_start = filePtr;

     

        writeData = write(employeeFileDescriptor, &employee, sizeof(struct Employee));
        if (writeData == -1)
        {
            perror("Error deleting employee record!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(employeeFileDescriptor, F_SETLK, &lock);

        strcpy(writeCache, ADMIN_MOD_EMPLOYEE_SUCCESS);
  
    writeData = write(connFD, writeCache, strlen(writeCache));
    if (writeData == -1)
    {
        perror("Error while writing final DEL message to client!");
        return false;
    }
    readData = read(connFD, readCache, sizeof(readCache)); // Dummy read

    return true;
}


bool change_admin_password(int connFD) {
    ssize_t readData, writeData;
    char readCache[1000], writeCache[1000], newPassword[1000];
    
    // Ask the client to enter the new password
    writeData = write(connFD, PASSWORD_CHANGE_NEW_PASS, strlen(PASSWORD_CHANGE_NEW_PASS));
    if (writeData == -1) {
        perror("Error writing PASSWORD_CHANGE_NEW_PASS message to client!");
        return false;
    }

    // Read the new password from the client
    bzero(readCache, sizeof(readCache));
    readData = read(connFD, readCache, sizeof(readCache));
    if (readData == -1) {
        perror("Error reading new password response from client");
        return false;
    }
    
    // Store the new password entered by the user
    //strcpy(newPassword, readCache);
    strcpy(newPassword, crypt(readCache, SALT_BAE));

     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Open the admincred.h file and read its contents
    FILE *file = fopen(ADMIN_CREDENTIALS_FILE, "r");
    if (file == NULL) {
        perror("Error opening admin credential  file!");
        return false;
    }

    // Read the file contents into a buffer
    char fileBuffer[5000];
    bzero(fileBuffer, sizeof(fileBuffer));
    fread(fileBuffer, sizeof(char), sizeof(fileBuffer) - 1, file);
    fclose(file);

    // Replace the old password with the new one
    char *passwordPosition = strstr(fileBuffer, "#define ADMIN_PASSWORD \"");
    if (passwordPosition == NULL) {
        perror("Error finding ADMIN_PASSWORD in admincred.h!");
        return false;
    }

    // Move pointer to the beginning of the actual password value
    passwordPosition += strlen("#define ADMIN_PASSWORD \"");

    // Find the closing quote of the current password
    char *passwordEnd = strchr(passwordPosition, '"');
    if (passwordEnd == NULL) {
        perror("Error finding the end of ADMIN_PASSWORD in admincred.h!");
        return false;
    }

    // Create a temporary buffer to hold the new file contents
    char newFileBuffer[5000];
    bzero(newFileBuffer, sizeof(newFileBuffer));

    // Copy up to the password part
    strncpy(newFileBuffer, fileBuffer, passwordPosition - fileBuffer);

    // Append the new password and close the quotes
    strcat(newFileBuffer, newPassword);
    strcat(newFileBuffer, "\"");

    // Append the rest of the file content after the password
    strcat(newFileBuffer, passwordEnd + 1);

    // Write the new content back to admincred.h
    file = fopen(ADMIN_CREDENTIALS_FILE, "w");
    if (file == NULL) {
        perror("Error opening admin credentialfile for writing!");
        return false;
    }

    fwrite(newFileBuffer, sizeof(char), strlen(newFileBuffer), file);
    fclose(file);

    // Notify the client that the password change was successful
    writeData = write(connFD, ADMIN_PASSWORD_CHANGE_SUCCESS, strlen(ADMIN_PASSWORD_CHANGE_SUCCESS));
    if (writeData == -1) {
        perror("Error writing PASSWORD_CHANGE_SUCCESS message to client!");
        return false;
    }
    
     writeData = write(connFD, "\n Recompile the server as admin credential are changed^", strlen("\n Recompile the server as admin credential are changed^"));
    if (writeData == -1) {
        perror("Error writing PASSWORD_CHANGE_SUCCESS message to client!");
        return false;
    }

    return true;
}



#endif