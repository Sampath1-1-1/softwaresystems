#ifndef EMPLOYEE_FUNCTIONS
#define EMPLOYEE_FUNCTIONS

#include "../resources/commonfunc.h"
#include "../record/employee.h"
#include "../record/account.h"
#include "../record/loanapply.h"
struct Employee loggedInEmployee;
// Function Prototypes =================================

bool employee_operation_handler(int connFD);
bool add_e_account(int connFD);
// bool delete_account(int connFD);
bool modify_e_customer_info(int connFD);
bool accept_reject_loan_applications(int connFD);
bool change_e_password(int connFD);


#define LOAN_FILE "./database/loan.bank"
#define LOAN_APPROVED "Loan Approved"
#define LOAN_REJECTED "Loan Rejected"
#define LOAN_APPLICATION_MENU "Enter Loan ID to accept/reject (or 0 to exit): "
// bool Process_Loan_Applications(int connFD)
// bool Approve_Reject_Loans(int connFD)
// bool View_Assigned_Loan_Applications(int connFD)
// bool View_Customer_Transactions(int connFD)
// bool Change_Password(int connFD)
// // =====================================================

// // Function Definition =================================

// // =====================================================

bool employee_operation_handler(int connFD)
{

    if (login_handler(3, connFD,NULL, &loggedInEmployee))
    {
        ssize_t writeBytes, readBytes;            // Number of bytes read from / written to the client
        char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, EMPLOYEE_LOGIN_SUCCESS);
        while (1)
        {
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, EMPLOYEE_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing EMPLOYEE_MENU to client!");
                return false;
            }
            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for EMPLOYEE_MENU");
                return false;
            }

            int choice = atoi(readBuffer);
            switch (choice)
            {
              case 1:
                   add_e_account(connFD);
                   break;
              
            //   case 3:
            //        get_transaction_details(connFD, -1);
            //        break;
              case 2:
                   modify_e_customer_info(connFD);
                   break;
              case 3:
                   accept_reject_loan_applications(connFD);
                   break;   
            //   case 4:
            //        get_account_details(connFD, NULL);
            //        break;       
            //   case 5:
            //        process_Loan_Applications(int connFD)
            //        break;
            //   case 6:
            //        Approve_Reject_Loans(int connFD)
            //        break;
            //   case 7:
            //        View Assigned_Loan_Applications(int connFD)
            //        break;
            //   case 8:
            //        view_Customer_Transactions(int connFD)
            //        break;
            case 9:
                   change_e_password(connFD);
                      break;
            default:
                  writeBytes = write(connFD, EMPLOYEE_LOGOUT, strlen(EMPLOYEE_LOGOUT));
                  return false;
            }
        }
    }
    else
    {
        //  EMPLOYEE LOGIN FAILED
        return false;
    }
    return true;
}

bool add_e_account(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Account newAccount, prevAccount;

    int accountFileDescriptor = open(ACCOUNT_FILE, O_RDONLY); //| O_CREAT, 0666
    if (accountFileDescriptor == -1 && errno == ENOENT)
    {
        // Account file was never created
        newAccount.id = 0;
    }
    else if (accountFileDescriptor == -1)
    {
        perror("Error while opening account file");
        return false;
    }
    else
    {
        int offset = lseek(accountFileDescriptor, -sizeof(struct Account), SEEK_END);
        if (offset == -1)
        {
            perror("Error seeking to last Account record!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Account), getpid()}; // why it is reading the lock if it is adding the  file
        int lockingStatus = fcntl(accountFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Account record!");
            return false;
        }

        readBytes = read(accountFileDescriptor, &prevAccount, sizeof(struct Account));
        if (readBytes == -1)
        {
            perror("Error while reading Account record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(accountFileDescriptor, F_SETLK, &lock);

        close(accountFileDescriptor);

        newAccount.id = prevAccount.id + 1;
       
    }

    newAccount.isActive = true;
    newAccount.balance = 0;

    memset(newAccount.trancsaction, -1, MAX_TRANSACTIONS * sizeof(int));

    accountFileDescriptor = open(ACCOUNT_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (accountFileDescriptor == -1)
    {
        perror("Error while creating / opening account file!");
        return false;
    }
    sprintf(writeBuffer, "%s", EMPLOYEE_ADD_CUSTOMER_NAME);

    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing EMPLOYEE_ADD_CUSTOMER_NAME message to client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer name response from client!");
        ;
        return false;
    }

    strcpy(newAccount.name, readBuffer);
    writeBytes = write(connFD, EMPLOYEE_ADD_CUSTOMER_GENDER, strlen(EMPLOYEE_ADD_CUSTOMER_GENDER));
    if (writeBytes == -1)
    {
        perror("Error writing EMPLOYEE_ADD_CUSTOMER_GENDER message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer gender response from client!");
        return false;
    }

    if (readBuffer[0] == 'M' || readBuffer[0] == 'F' || readBuffer[0] == 'O')
        newAccount.gender = readBuffer[0];
    else
    {
        writeBytes = write(connFD,EMPLOYEE_ADD_CUSTOMER_WRONG_GENDER, strlen(EMPLOYEE_ADD_CUSTOMER_WRONG_GENDER));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, EMPLOYEE_ADD_CUSTOMER_AGE);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing EMPLOYEE_ADD_CUSTOMER_AGE message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading customer age response from client!");
        return false;
    }

    int customerAge = atoi(readBuffer);
    if (customerAge == 0)
    {
        // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    newAccount.age = customerAge;

    //  newCustomer.account = newAccountNumber;

    strcpy(newAccount.login, newAccount.name);
    strcat(newAccount.login, "-");
    sprintf(writeBuffer, "%d", newAccount.id);
    strcat(newAccount.login, writeBuffer);

    char hashedPassword[1000];
    strcpy(hashedPassword, crypt(AUTOGEN_PASSWORD, SALT_BAE));
    strcpy(newAccount.password, hashedPassword);
    write(connFD, newAccount.login, strlen(newAccount.login));
    writeBytes = write(accountFileDescriptor, &newAccount, sizeof(struct Account));
    if (writeBytes == -1)
    {
        perror("Error while writing Account record to file!");
        return false;
    }

    close(accountFileDescriptor);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "%s%d", EMPLOYEE_ADD_ACCOUNT_NUMBER, newAccount.id);
    strcat(writeBuffer, "\nRedirecting you to the main menu ...^");
    writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
    readBytes = read(connFD, readBuffer, sizeof(read)); // Dummy read
    bzero(readBuffer, sizeof(readBuffer));
    return true;
}



bool modify_e_customer_info(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct Account account;

    int customerID;

    off_t offset;
    int lockingStatus;

    writeBytes = write(connFD, EMPLOYEE_MOD_CUSTOMER_ID, strlen(EMPLOYEE_MOD_CUSTOMER_ID));
    if (writeBytes == -1)
    {
        perror("Error while writing EMPLOYEE_MOD_CUSTOMER_ID message to client!");
        return false;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error while reading customer ID from client!");
        return false;
    }

    customerID = atoi(readBuffer);

    int customerFileDescriptor = open(ACCOUNT_FILE, O_RDONLY);
    if (customerFileDescriptor == -1)
    {
        // Customer File doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, CUSTOMER_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    offset = lseek(customerFileDescriptor, customerID * sizeof(struct Account), SEEK_SET);
    if (errno == EINVAL)
    {
        // Customer record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, CUSTOMER_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required customer record!");
        return false;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};

    // Lock the record to be read
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Couldn't obtain lock on customer record!");
        return false;
    }

    readBytes = read(customerFileDescriptor, &account, sizeof(struct Account));
    if (readBytes == -1)
    {
        perror("Error while reading customer record from the file!");
        return false;
    }

    // Unlock the record
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);

    close(customerFileDescriptor);

    writeBytes = write(connFD, EMPLOYEE_MOD_CUSTOMER_MENU, strlen(EMPLOYEE_MOD_CUSTOMER_MENU));
    if (writeBytes == -1)
    {
        perror("Error while writing EMPLOYEE_MOD_CUSTOMER_MENU message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error while getting customer modification menu choice from client!");
        return false;
    }

    int choice = atoi(readBuffer);
    if (choice == 0)
    { // A non-numeric string was passed to atoi
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    switch (choice)
    {
    case 1: /////////////////////////////////////// change name///////////////////////////////////
        writeBytes = write(connFD, EMPLOYEE_MOD_CUSTOMER_NEW_NAME, strlen(EMPLOYEE_MOD_CUSTOMER_NEW_NAME));
        if (writeBytes == -1)
        {
            perror("Error while writing EMPLOYEE_MOD_CUSTOMER_NEW_NAME message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for customer's new name from client!");
            return false;
        }
        strcpy(account.name, readBuffer);
        break;
    case 2: ////////////////////////////////////////chavne age/////////////////////////////////
        writeBytes = write(connFD, EMPLOYEE_MOD_CUSTOMER_NEW_AGE, strlen(EMPLOYEE_MOD_CUSTOMER_NEW_AGE));
        if (writeBytes == -1)
        {
            perror("Error while writing EMPLOYEE_MOD_CUSTOMER_NEW_AGE message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for customer's new age from client!");
            return false;
        }
        int updatedAge = atoi(readBuffer);
        if (updatedAge == 0)
        {
            // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
            bzero(writeBuffer, sizeof(writeBuffer));
            strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
                return false;
            }
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
            return false;
        }
        account.age = updatedAge;
        break;
    case 3://///////////////////////////////////change gender/////////////////////////////////
        writeBytes = write(connFD, EMPLOYEE_MOD_CUSTOMER_NEW_GENDER, strlen(EMPLOYEE_MOD_CUSTOMER_NEW_GENDER));
        if (writeBytes == -1)
        {
            perror("Error while writing EMPLOYEE_MOD_CUSTOMER_NEW_GENDER message to client!");
            return false;
        }
        readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error while getting response for customer's new gender from client!");
            return false;
        }
        account.gender = readBuffer[0];
        break;
    case 4: // to update password ///////////////////////////////////////////////////////////////////////////////

                    writeBytes = write(connFD, "Password is being resetted", strlen("Password is being resetted"));
        if (writeBytes == -1)
        {
            perror("Error while writing EMPLOYEE_MOD_CUSTOMER_NEW_Password message to client!");
            return false;
        }
                //     char hashedPassword[1000];
                //  strcpy(hashedPassword, );//
         strcpy(account.password, crypt(AUTOGEN_PASSWORD, SALT_BAE));

        break; //////////////////////////////////////////////////////////////////////////////////////////////////////////

    default: //////////////////////////////////////////////////////////////////////////////
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, INVALID_MENU_CHOICE);
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing INVALID_MENU_CHOICE message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
        // saving the struct back to the main file
    customerFileDescriptor = open(ACCOUNT_FILE, O_WRONLY);
    if (customerFileDescriptor == -1)
    {
        perror("Error while opening customer file");
        return false;
    }
    offset = lseek(customerFileDescriptor, customerID * sizeof(struct Account), SEEK_SET);
    if (offset == -1)
    {
        perror("Error while seeking to required customer record!");
        return false;
    }

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error while obtaining write lock on customer record!");
        return false;
    }

    writeBytes = write(customerFileDescriptor, &account, sizeof(struct Account));
    if (writeBytes == -1)
    {
        perror("Error while writing update customer info into file");
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLKW, &lock);

    close(customerFileDescriptor);

    writeBytes = write(connFD, EMPLOYEE_MOD_CUSTOMER_SUCCESS, strlen(EMPLOYEE_MOD_CUSTOMER_SUCCESS));
    if (writeBytes == -1)
    {
        perror("Error while writing EMPLOYEE_MOD_CUSTOMER_SUCCESS message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return true;
}
 


 
bool accept_reject_loan_applications(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    int loanFileDescriptor = open(LOAN_FILE, O_RDWR); // Open for read and write
    if (loanFileDescriptor == -1) {
        perror("Error opening loan file");
        return false;
    }

    struct Loan loan;
    while (true) {
        // Read the loan applications
        readBytes = read(loanFileDescriptor, &loan, sizeof(struct Loan));
        if (readBytes == 0) {
            // End of file reached
            break;
        } else if (readBytes == -1) {
            perror("Error reading loan record from file");
            close(loanFileDescriptor);
            return false;
        }

        // Display loan application details
        sprintf(writeBuffer, "Loan ID: %d\nCustomer ID: %d\nAmount: %.2f\nStatus: %s\n", 
                loan.loanID, loan.customerID, loan.loanAmount, loan.isApproved ? "Approved" : "Pending");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error writing loan details to client");
            close(loanFileDescriptor);
            return false;
        }

        // Ask the employee for decision
        writeBytes = write(connFD, "Enter 1 to Approve, 2 to Reject, or 0 to Skip: ", 
                           strlen("Enter 1 to Approve, 2 to Reject, or 0 to Skip: "));
        if (writeBytes == -1) {
            perror("Error writing prompt to client");
            close(loanFileDescriptor);
            return false;
        }

        // Read employee decision
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error reading decision from client");
            close(loanFileDescriptor);
            return false;
        }

        int decision = atoi(readBuffer);
        if (decision == 1) { // Approve
            loan.isApproved = LOAN_APPROVED;
        } else if (decision == 2) { // Reject
            loan.isApproved = LOAN_REJECTED;
        } else if (decision == 0) { // Skip
            continue;
        } else {
            write(connFD, ERRON_INPUT_FOR_NUMBER, strlen(ERRON_INPUT_FOR_NUMBER)); // Invalid input
            continue;
        }

        // Update the loan status in the file
        off_t offset = (loan.loanID - 1) * sizeof(struct Loan); // Assuming IDs are continuous and start from 1
        if (lseek(loanFileDescriptor, offset, SEEK_SET) == -1) {
            perror("Error seeking to the loan record");
            close(loanFileDescriptor);
            return false;
        }

        writeBytes = write(loanFileDescriptor, &loan, sizeof(struct Loan));
        if (writeBytes == -1) {
            perror("Error writing updated loan record to file");
            close(loanFileDescriptor);
            return false;
        }
    }

    close(loanFileDescriptor);
    return true;
}



bool change_e_password(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000], hashedPassword[1000];

    char newPassword[1000];

    // Lock the critical section
    struct sembuf semOp = {0, -1, SEM_UNDO};
    int semopStatus = semop(semIdentifier, &semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while locking critical section");
        return false;
    }

    writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS, strlen(PASSWORD_CHANGE_OLD_PASS));
    if (writeBytes == -1)
    {
        perror("Error writing PASSWORD_CHANGE_OLD_PASS message to client!");
        unlock_critical_section(&semOp);
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading old password response from client");
        unlock_critical_section(&semOp);
        return false;
    }

    if (strcmp(crypt(readBuffer, SALT_BAE), loggedInEmployee.password) == 0)
    {
        // Password matches with old password
        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS, strlen(PASSWORD_CHANGE_NEW_PASS));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS message to client!");
            unlock_critical_section(&semOp);
            return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading new password response from client");
            unlock_critical_section(&semOp);
            return false;
        }

        strcpy(newPassword, crypt(readBuffer, SALT_BAE));

        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_RE, strlen(PASSWORD_CHANGE_NEW_PASS_RE));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS_RE message to client!");
            unlock_critical_section(&semOp);
            return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading new password reenter response from client");
            unlock_critical_section(&semOp);
            return false;
        }

        if (strcmp(crypt(readBuffer, SALT_BAE), newPassword) == 0)
        {
            // New & reentered passwords match

            strcpy(loggedInEmployee.password, newPassword);

            int employeeFileDescriptor = open(ACCOUNT_FILE, O_WRONLY);
            if (employeeFileDescriptor == -1)
            {
                perror("Error opening customer file!");
                unlock_critical_section(&semOp);
                return false;
            }

            off_t offset = lseek(employeeFileDescriptor, loggedInEmployee.id * sizeof(struct Employee), SEEK_SET);
            if (offset == -1)
            {
                perror("Error seeking to the customer record!");
                unlock_critical_section(&semOp);
                return false;
            }

            struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Employee), getpid()};
            int lockingStatus = fcntl(employeeFileDescriptor, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining write lock on customer record!");
                unlock_critical_section(&semOp);
                return false;
            }

            writeBytes = write(employeeFileDescriptor, &loggedInEmployee, sizeof(struct Employee));
            if (writeBytes == -1)
            {
                perror("Error storing updated employee password into employee record!");
                unlock_critical_section(&semOp);
                return false;
            }

            lock.l_type = F_UNLCK;
            lockingStatus = fcntl(employeeFileDescriptor, F_SETLK, &lock);

            close(employeeFileDescriptor);

            writeBytes = write(connFD, PASSWORD_CHANGE_SUCCESS, strlen(PASSWORD_CHANGE_SUCCESS));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

            unlock_critical_section(&semOp);

            return true;
        }
        else
        {
            // New & reentered passwords don't match
            writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_INVALID, strlen(PASSWORD_CHANGE_NEW_PASS_INVALID));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        }
    }
    else
    {
        // Password doesn't match with old password
        writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS_INVALID, strlen(PASSWORD_CHANGE_OLD_PASS_INVALID));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    }

    unlock_critical_section(&semOp);

    return false;
}

#endif
// int add_customer(int connFD, bool isPrimary, int newAccountNumber)
// {
//     ssize_t readBytes, writeBytes;
//     char readBuffer[1000], writeBuffer[1000];

//     struct Customer newCustomer, previousCustomer;

//     int customerFileDescriptor = open(CUSTOMER_FILE, O_RDONLY);
//     if (customerFileDescriptor == -1 && errno == ENOENT)
//     {
//         // Customer file was never created
//         newCustomer.id = 0;
//     }
//     else if (customerFileDescriptor == -1)
//     {
//         perror("Error while opening customer file");
//         return -1;
//     }
//     else
//     {
//         int offset = lseek(customerFileDescriptor, -sizeof(struct Customer), SEEK_END);
//         if (offset == -1)
//         {
//             perror("Error seeking to last Customer record!");
//             return false;
//         }

//         struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
//         int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
//         if (lockingStatus == -1)
//         {
//             perror("Error obtaining read lock on Customer record!");
//             return false;
//         }

//         readBytes = read(customerFileDescriptor, &previousCustomer, sizeof(struct Customer));
//         if (readBytes == -1)
//         {
//             perror("Error while reading Customer record from file!");
//             return false;
//         }

//         lock.l_type = F_UNLCK;
//         fcntl(customerFileDescriptor, F_SETLK, &lock);

//         close(customerFileDescriptor);

//         newCustomer.id = previousCustomer.id + 1;
//     }

//     if (isPrimary)
//         sprintf(writeBuffer, "%s%s", EMPLOYEE_ADD_CUSTOMER_PRIMARY, EMPLOYEE_ADD_CUSTOMER_NAME);
//     else
//         sprintf(writeBuffer, "%s%s", EMPLOYEE_ADD_CUSTOMER_SECONDARY, EMPLOYEE_ADD_CUSTOMER_NAME);

//     writeBytes = write(connFD, writeBuffer, sizeof(writeBuffer));
//     if (writeBytes == -1)
//     {
//         perror("Error writing EMPLOYEE_ADD_CUSTOMER_NAME message to client!");
//         return false;
//     }

//     readBytes = read(connFD, readBuffer, sizeof(readBuffer));
//     if (readBytes == -1)
//     {
//         perror("Error reading customer name response from client!");
//         ;
//         return false;
//     }

//     strcpy(newCustomer.name, readBuffer);

//     writeBytes = write(connFD, EMPLOYEE_ADD_CUSTOMER_GENDER, strlen(EMPLOYEE_ADD_CUSTOMER_GENDER));
//     if (writeBytes == -1)
//     {
//         perror("Error writing EMPLOYEE_ADD_CUSTOMER_GENDER message to client!");
//         return false;
//     }

//     bzero(readBuffer, sizeof(readBuffer));
//     readBytes = read(connFD, readBuffer, sizeof(readBuffer));
//     if (readBytes == -1)
//     {
//         perror("Error reading customer gender response from client!");
//         return false;
//     }

//     if (readBuffer[0] == 'M' || readBuffer[0] == 'F' || readBuffer[0] == 'O')
//         newCustomer.gender = readBuffer[0];
//     else
//     {
//         writeBytes = write(connFD, EMPLOYEE_ADD_CUSTOMER_WRONG_GENDER, strlen(EMPLOYEE_ADD_CUSTOMER_WRONG_GENDER));
//         readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
//         return false;
//     }

//     bzero(writeBuffer, sizeof(writeBuffer));
//     strcpy(writeBuffer, EMPLOYEE_ADD_CUSTOMER_AGE);
//     writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
//     if (writeBytes == -1)
//     {
//         perror("Error writing EMPLOYEE_ADD_CUSTOMER_AGE message to client!");
//         return false;
//     }

//     bzero(readBuffer, sizeof(readBuffer));
//     readBytes = read(connFD, readBuffer, sizeof(readBuffer));
//     if (readBytes == -1)
//     {
//         perror("Error reading customer age response from client!");
//         return false;
//     }

//     int customerAge = atoi(readBuffer);
//     if (customerAge == 0)
//     {
//         // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
//         bzero(writeBuffer, sizeof(writeBuffer));
//         strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
//         writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
//         if (writeBytes == -1)
//         {
//             perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
//             return false;
//         }
//         readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
//         return false;
//     }
//     newCustomer.age = customerAge;

//     newCustomer.account = newAccountNumber;

//     strcpy(newCustomer.login, newCustomer.name);
//     strcat(newCustomer.login, "-");
//     sprintf(writeBuffer, "%d", newCustomer.id);
//     strcat(newCustomer.login, writeBuffer);

//     char hashedPassword[1000];
//     strcpy(hashedPassword, crypt(AUTOGEN_PASSWORD, SALT_BAE));
//     strcpy(newCustomer.password, hashedPassword);

//     customerFileDescriptor = open(CUSTOMER_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
//     if (customerFileDescriptor == -1)
//     {
//         perror("Error while creating / opening customer file!");
//         return false;
//     }
//     writeBytes = write(customerFileDescriptor, &newCustomer, sizeof(newCustomer));
//     if (writeBytes == -1)
//     {
//         perror("Error while writing Customer record to file!");
//         return false;
//     }

//     close(customerFileDescriptor);

//     bzero(writeBuffer, sizeof(writeBuffer));
//     sprintf(writeBuffer, "%s%s-%d\n%s%s", EMPLOYEE_ADD_CUSTOMER_AUTOGEN_LOGIN, newCustomer.name, newCustomer.id, EMPLOYEE_ADD_CUSTOMER_AUTOGEN_PASSWORD, AUTOGEN_PASSWORD);
//     strcat(writeBuffer, "^");
//     writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
//     if (writeBytes == -1)
//     {
//         perror("Error sending customer loginID and password to the client!");
//         return false;
//     }

//     readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

//     return newCustomer.id;
// }

// bool delete_account(int connFD)
// {
//     ssize_t readBytes, writeBytes;
//     char readBuffer[1000], writeBuffer[1000];

//     struct Account account;

//     writeBytes = write(connFD, EMPLOYEE_DEL_ACCOUNT_NO, strlen(EMPLOYEE_DEL_ACCOUNT_NO));
//     if (writeBytes == -1)
//     {
//         perror("Error writing EMPLOYEE_DEL_ACCOUNT_NO to client!");
//         return false;
//     }

//     bzero(readBuffer, sizeof(readBuffer));
//     readBytes = read(connFD, readBuffer, sizeof(readBuffer));
//     if (readBytes == -1)
//     {
//         perror("Error reading account number response from the client!");
//         return false;
//     }

//     int accountNumber = atoi(readBuffer);

//     int accountFileDescriptor = open(ACCOUNT_FILE, O_RDONLY);
//     if (accountFileDescriptor == -1)
//     {
//         // Account record doesn't exist
//         bzero(writeBuffer, sizeof(writeBuffer));
//         strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
//         strcat(writeBuffer, "^");
//         writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
//         if (writeBytes == -1)
//         {
//             perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
//             return false;
//         }
//         readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
//         return false;
//     }


//     int offset = lseek(accountFileDescriptor, accountNumber * sizeof(struct Account), SEEK_SET);
//     if (errno == EINVAL)
//     {
//         // Customer record doesn't exist
//         bzero(writeBuffer, sizeof(writeBuffer));
//         strcpy(writeBuffer, ACCOUNT_ID_DOESNT_EXIT);
//         strcat(writeBuffer, "^");
//         writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
//         if (writeBytes == -1)
//         {
//             perror("Error while writing ACCOUNT_ID_DOESNT_EXIT message to client!");
//             return false;
//         }
//         readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
//         return false;
//     }
//     else if (offset == -1)
//     {
//         perror("Error while seeking to required account record!");
//         return false;
//     }

//     struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
//     int lockingStatus = fcntl(accountFileDescriptor, F_SETLKW, &lock);
//     if (lockingStatus == -1)
//     {
//         perror("Error obtaining read lock on Account record!");
//         return false;
//     }

//     readBytes = read(accountFileDescriptor, &account, sizeof(struct Account));
//     if (readBytes == -1)
//     {
//         perror("Error while reading Account record from file!");
//         return false;
//     }

//     lock.l_type = F_UNLCK;
//     fcntl(accountFileDescriptor, F_SETLK, &lock);

//     close(accountFileDescriptor);

//     bzero(writeBuffer, sizeof(writeBuffer));
//     if (account.balance == 0)
//     {
//         // No money, hence can close account
//         account.active = false;
//         accountFileDescriptor = open(ACCOUNT_FILE, O_WRONLY);
//         if (accountFileDescriptor == -1)
//         {
//             perror("Error opening Account file in write mode!");
//             return false;
//         }

//         offset = lseek(accountFileDescriptor, accountNumber * sizeof(struct Account), SEEK_SET);
//         if (offset == -1)
//         {
//             perror("Error seeking to the Account!");
//             return false;
//         }

//         lock.l_type = F_WRLCK;
//         lock.l_start = offset;

//         int lockingStatus = fcntl(accountFileDescriptor, F_SETLKW, &lock);
//         if (lockingStatus == -1)
//         {
//             perror("Error obtaining write lock on the Account file!");
//             return false;
//         }

//         writeBytes = write(accountFileDescriptor, &account, sizeof(struct Account));
//         if (writeBytes == -1)
//         {
//             perror("Error deleting account record!");
//             return false;
//         }

//         lock.l_type = F_UNLCK;
//         fcntl(accountFileDescriptor, F_SETLK, &lock);

//         strcpy(writeBuffer, EMPLOYEE_DEL_ACCOUNT_SUCCESS);
//     }
//     else
//         // Account has some money ask customer to withdraw it
//         strcpy(writeBuffer, EMPLOYEE_DEL_ACCOUNT_FAILURE);
//     writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
//     if (writeBytes == -1)
//     {
//         perror("Error while writing final DEL message to client!");
//         return false;
//     }
//     readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

//     return true;
// }

// bool modify_customer_info(int connFD)
// {
//     ssize_t readBytes, writeBytes;
//     char readBuffer[1000], writeBuffer[1000];

//     struct Customer customer;

//     int customerID;

//     off_t offset;
//     int lockingStatus;

//     writeBytes = write(connFD, EMPLOYEE_MOD_CUSTOMER_ID, strlen(EMPLOYEE_MOD_CUSTOMER_ID));
//     if (writeBytes == -1)
//     {
//         perror("Error while writing EMPLOYEE_MOD_CUSTOMER_ID message to client!");
//         return false;
//     }
//     bzero(readBuffer, sizeof(readBuffer));
//     readBytes = read(connFD, readBuffer, sizeof(readBuffer));
//     if (readBytes == -1)
//     {
//         perror("Error while reading customer ID from client!");
//         return false;
//     }

//     customerID = atoi(readBuffer);

//     int customerFileDescriptor = open(CUSTOMER_FILE, O_RDONLY);
//     if (customerFileDescriptor == -1)
//     {
//         // Customer File doesn't exist
//         bzero(writeBuffer, sizeof(writeBuffer));
//         strcpy(writeBuffer, CUSTOMER_ID_DOESNT_EXIT);
//         strcat(writeBuffer, "^");
//         writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
//         if (writeBytes == -1)
//         {
//             perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
//             return false;
//         }
//         readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
//         return false;
//     }
    
//     offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);
//     if (errno == EINVAL)
//     {
//         // Customer record doesn't exist
//         bzero(writeBuffer, sizeof(writeBuffer));
//         strcpy(writeBuffer, CUSTOMER_ID_DOESNT_EXIT);
//         strcat(writeBuffer, "^");
//         writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
//         if (writeBytes == -1)
//         {
//             perror("Error while writing CUSTOMER_ID_DOESNT_EXIT message to client!");
//             return false;
//         }
//         readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
//         return false;
//     }
//     else if (offset == -1)
//     {
//         perror("Error while seeking to required customer record!");
//         return false;
//     }

//     struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};

//     // Lock the record to be read
//     lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
//     if (lockingStatus == -1)
//     {
//         perror("Couldn't obtain lock on customer record!");
//         return false;
//     }

//     readBytes = read(customerFileDescriptor, &customer, sizeof(struct Customer));
//     if (readBytes == -1)
//     {
//         perror("Error while reading customer record from the file!");
//         return false;
//     }

//     // Unlock the record
//     lock.l_type = F_UNLCK;
//     fcntl(customerFileDescriptor, F_SETLK, &lock);

//     close(customerFileDescriptor);

//     writeBytes = write(connFD, EMPLOYEE_MOD_CUSTOMER_MENU, strlen(EMPLOYEE_MOD_CUSTOMER_MENU));
//     if (writeBytes == -1)
//     {
//         perror("Error while writing EMPLOYEE_MOD_CUSTOMER_MENU message to client!");
//         return false;
//     }
//     readBytes = read(connFD, readBuffer, sizeof(readBuffer));
//     if (readBytes == -1)
//     {
//         perror("Error while getting customer modification menu choice from client!");
//         return false;
//     }

//     int choice = atoi(readBuffer);
//     if (choice == 0)
//     { // A non-numeric string was passed to atoi
//         bzero(writeBuffer, sizeof(writeBuffer));
//         strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
//         writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
//         if (writeBytes == -1)
//         {
//             perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
//             return false;
//         }
//         readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
//         return false;
//     }

//     bzero(readBuffer, sizeof(readBuffer));
//     switch (choice)
//     {
//     case 1:
//         writeBytes = write(connFD, EMPLOYEE_MOD_CUSTOMER_NEW_NAME, strlen(EMPLOYEE_MOD_CUSTOMER_NEW_NAME));
//         if (writeBytes == -1)
//         {
//             perror("Error while writing EMPLOYEE_MOD_CUSTOMER_NEW_NAME message to client!");
//             return false;
//         }
//         readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
//         if (readBytes == -1)
//         {
//             perror("Error while getting response for customer's new name from client!");
//             return false;
//         }
//         strcpy(customer.name, readBuffer);
//         break;
//     case 2:
//         writeBytes = write(connFD, EMPLOYEE_MOD_CUSTOMER_NEW_AGE, strlen(EMPLOYEE_MOD_CUSTOMER_NEW_AGE));
//         if (writeBytes == -1)
//         {
//             perror("Error while writing EMPLOYEE_MOD_CUSTOMER_NEW_AGE message to client!");
//             return false;
//         }
//         readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
//         if (readBytes == -1)
//         {
//             perror("Error while getting response for customer's new age from client!");
//             return false;
//         }
//         int updatedAge = atoi(readBuffer);
//         if (updatedAge == 0)
//         {
//             // Either client has sent age as 0 (which is invalid) or has entered a non-numeric string
//             bzero(writeBuffer, sizeof(writeBuffer));
//             strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
//             writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
//             if (writeBytes == -1)
//             {
//                 perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
//                 return false;
//             }
//             readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
//             return false;
//         }
//         customer.age = updatedAge;
//         break;
//     case 3:
//         writeBytes = write(connFD, EMPLOYEE_MOD_CUSTOMER_NEW_GENDER, strlen(EMPLOYEE_MOD_CUSTOMER_NEW_GENDER));
//         if (writeBytes == -1)
//         {
//             perror("Error while writing EMPLOYEE_MOD_CUSTOMER_NEW_GENDER message to client!");
//             return false;
//         }
//         readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
//         if (readBytes == -1)
//         {
//             perror("Error while getting response for customer's new gender from client!");
//             return false;
//         }
//         customer.gender = readBuffer[0];
//         break;
//     default:
//         bzero(writeBuffer, sizeof(writeBuffer));
//         strcpy(writeBuffer, INVALID_MENU_CHOICE);
//         writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
//         if (writeBytes == -1)
//         {
//             perror("Error while writing INVALID_MENU_CHOICE message to client!");
//             return false;
//         }
//         readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
//         return false;
//     }

//     customerFileDescriptor = open(CUSTOMER_FILE, O_WRONLY);
//     if (customerFileDescriptor == -1)
//     {
//         perror("Error while opening customer file");
//         return false;
//     }
//     offset = lseek(customerFileDescriptor, customerID * sizeof(struct Customer), SEEK_SET);
//     if (offset == -1)
//     {
//         perror("Error while seeking to required customer record!");
//         return false;
//     }

//     lock.l_type = F_WRLCK;
//     lock.l_start = offset;
//     lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
//     if (lockingStatus == -1)
//     {
//         perror("Error while obtaining write lock on customer record!");
//         return false;
//     }

//     writeBytes = write(customerFileDescriptor, &customer, sizeof(struct Customer));
//     if (writeBytes == -1)
//     {
//         perror("Error while writing update customer info into file");
//     }

//     lock.l_type = F_UNLCK;
//     fcntl(customerFileDescriptor, F_SETLKW, &lock);

//     close(customerFileDescriptor);

//     writeBytes = write(connFD, EMPLOYEE_MOD_CUSTOMER_SUCCESS, strlen(EMPLOYEE_MOD_CUSTOMER_SUCCESS));
//     if (writeBytes == -1)
//     {
//         perror("Error while writing EMPLOYEE_MOD_CUSTOMER_SUCCESS message to client!");
//         return false;
//     }
//     readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

//     return true;
// }
