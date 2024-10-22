#ifndef CUSTOMER_FUNCTIONS
#define CUSTOMER_FUNCTIONS

// Semaphores are necessary joint account due the design choice I've made
#include <sys/ipc.h>
#include <sys/sem.h>

struct Account loggedInCustomer;
int semIdentifier;
struct Loan loan;

// Function Prototypes =================================

bool customer_operation_handler(int connFD);
bool deposit(int connFD);
bool withdraw(int connFD);
bool get_balance(int connFD);
bool change_password(int connFD);
bool lock_critical_section(struct sembuf *semOp);
bool unlock_critical_section(struct sembuf *sem_op);
void write_transaction_to_array(int *transactionArray, int ID);
int write_transaction_to_file(int accountNumber, long int oldBalance, long int newBalance, bool operation);
bool handle_loan_operations(int connFD) ;
bool apply_feedback(int connFD);
//bool apply_for_loan(int connFD)
bool transfer_funds(int connFD);

// =====================================================

// Function Definition =================================

bool customer_operation_handler(int connFD)
{
    if (login_handler(2, connFD, &loggedInCustomer,NULL))  // false signifies that it is not admin
    {
        ssize_t writeBytes, readBytes;            // Number of bytes read from / written to the client
        char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client

        // Get a semaphore for the user
        key_t semKey = ftok(ACCOUNT_FILE, loggedInCustomer.id); // Generate a key based on the account number hence, different customers will have different semaphores

        union semun
        {
            int val; // Value of the semaphore
        } semSet;

        int semctlStatus;
        semIdentifier = semget(semKey, 1, 0); // Get the semaphore if it exists
        if (semIdentifier == -1)
        {
            semIdentifier = semget(semKey, 1, IPC_CREAT | 0700); // Create a new semaphore
            if (semIdentifier == -1)
            {
                perror("Error while creating semaphore!");
                _exit(1);
            }

            semSet.val = 1; // Set a binary semaphore
            semctlStatus = semctl(semIdentifier, 0, SETVAL, semSet);
            if (semctlStatus == -1)
            {
                perror("Error while initializing a binary sempahore!");
                _exit(1);
            }
        }

        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, CUSTOMER_LOGIN_SUCCESS);
        while (1)
        {
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, CUSTOMER_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing CUSTOMER_MENU to client!");
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for CUSTOMER_MENU");
                return false;
            }
            
            // printf("READ BUFFER : %s\n", readBuffer);
            int choice = atoi(readBuffer);
            // printf("CHOICE : %d\n", choice);
            switch (choice)
            {
            case 1:
                // get_customer_details(connFD, loggedInCustomer.id);
                break;
            case 2:
                deposit(connFD);
                break;
            case 3:
                withdraw(connFD);
                break;
            case 4:
                get_balance(connFD);
                break;
            case 5:
                // get_transaction_details(connFD, loggedInCustomer.id);
                break;
            case 6:
                change_password(connFD);
                break;
            case 7:
                handle_loan_operations(connFD);    
            case 8:
                 apply_feedback(connFD);
            case 9:
                transfer_funds(connFD);     
            default:
                writeBytes = write(connFD, CUSTOMER_LOGOUT, strlen(CUSTOMER_LOGOUT));
                return false;
            }
        }
    }
    else
    {
        // CUSTOMER LOGIN FAILED


         write(connFD, CUSTOMER_ERROR, strlen(CUSTOMER_ERROR));

        return false;
    }
    return true;
}

bool deposit(int connFD)
{
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;

    struct Account account;
    account.id = loggedInCustomer.id;

    long int depositAmount = 0;

    // Lock the critical section
    struct sembuf semOp;
    lock_critical_section(&semOp);

    if (get_account_details(connFD, &account))
    {
        
        if (account.isActive)
        {

            writeBytes = write(connFD, DEPOSIT_AMOUNT, strlen(DEPOSIT_AMOUNT));
            if (writeBytes == -1)
            {
                perror("Error writing DEPOSIT_AMOUNT to client!");
                unlock_critical_section(&semOp);
                return false;
            }

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error reading deposit money from client!");
                unlock_critical_section(&semOp);
                return false;
            }

            depositAmount = atol(readBuffer);
            if (depositAmount != 0)
            {

                int newTransactionID = write_transaction_to_file(account.id, account.balance, account.balance + depositAmount, 1);
                write_transaction_to_array(account.trancsaction, newTransactionID);

                account.balance += depositAmount;

                int accountFileDescriptor = open(ACCOUNT_FILE, O_WRONLY);
                off_t offset = lseek(accountFileDescriptor, account.id * sizeof(struct Account), SEEK_SET);

                struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
                int lockingStatus = fcntl(accountFileDescriptor, F_SETLKW, &lock);
                if (lockingStatus == -1)
                {
                    perror("Error obtaining write lock on account file!");
                    unlock_critical_section(&semOp);
                    return false;
                }

                writeBytes = write(accountFileDescriptor, &account, sizeof(struct Account));
                if (writeBytes == -1)
                {
                    perror("Error storing updated deposit money in account record!");
                    unlock_critical_section(&semOp);
                    return false;
                }

                lock.l_type = F_UNLCK;
                fcntl(accountFileDescriptor, F_SETLK, &lock);

                write(connFD, DEPOSIT_AMOUNT_SUCCESS, strlen(DEPOSIT_AMOUNT_SUCCESS));
                read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

                get_balance(connFD);

                unlock_critical_section(&semOp);

                return true;
            }
            else
                writeBytes = write(connFD, DEPOSIT_AMOUNT_INVALID, strlen(DEPOSIT_AMOUNT_INVALID));
        }
        else
            write(connFD, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

        unlock_critical_section(&semOp);
    }
    else
    {
        // FAIL
        unlock_critical_section(&semOp);
        return false;
    }
}

bool withdraw(int connFD)
{
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;

    struct Account account;
    account.id = loggedInCustomer.id;

    long int withdrawAmount = 0;

    // Lock the critical section
    struct sembuf semOp;
    lock_critical_section(&semOp);

    if (get_account_details(connFD, &account))
    {
        if (account.isActive)
        {

            writeBytes = write(connFD, WITHDRAW_AMOUNT, strlen(WITHDRAW_AMOUNT));
            if (writeBytes == -1)
            {
                perror("Error writing WITHDRAW_AMOUNT message to client!");
                unlock_critical_section(&semOp);
                return false;
            }

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error reading withdraw amount from client!");
                unlock_critical_section(&semOp);
                return false;
            }

            withdrawAmount = atol(readBuffer);

            if (withdrawAmount != 0 && account.balance - withdrawAmount >= 0)
            {

                int newTransactionID = write_transaction_to_file(account.id, account.balance, account.balance - withdrawAmount, 0);
                write_transaction_to_array(account.trancsaction, newTransactionID);

                account.balance -= withdrawAmount;

                int accountFileDescriptor = open(ACCOUNT_FILE, O_WRONLY);
                off_t offset = lseek(accountFileDescriptor, account.id * sizeof(struct Account), SEEK_SET);

                struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
                int lockingStatus = fcntl(accountFileDescriptor, F_SETLKW, &lock);
                if (lockingStatus == -1)
                {
                    perror("Error obtaining write lock on account record!");
                    unlock_critical_section(&semOp);
                    return false;
                }

                writeBytes = write(accountFileDescriptor, &account, sizeof(struct Account));
                if (writeBytes == -1)
                {
                    perror("Error writing updated balance into account file!");
                    unlock_critical_section(&semOp);
                    return false;
                }

                lock.l_type = F_UNLCK;
                fcntl(accountFileDescriptor, F_SETLK, &lock);

                write(connFD, WITHDRAW_AMOUNT_SUCCESS, strlen(WITHDRAW_AMOUNT_SUCCESS));
                read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

                get_balance(connFD);

                unlock_critical_section(&semOp);

                return true;
            }
            else
                writeBytes = write(connFD, WITHDRAW_AMOUNT_INVALID, strlen(WITHDRAW_AMOUNT_INVALID));
        }
        else
            write(connFD, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

        unlock_critical_section(&semOp);
    }
    else
    {
        // FAILURE while getting account information
        unlock_critical_section(&semOp);
        return false;
    }
}

bool get_balance(int connFD)
{
    char buffer[1000];
    struct Account account;
    account.id = loggedInCustomer.id;
    if (get_account_details(connFD, &account))
    {
        bzero(buffer, sizeof(buffer));
        if (account.isActive)
        {
            sprintf(buffer, "You have ₹ %d imaginary money in our bank!^", account.balance);
            write(connFD, buffer, strlen(buffer));
        }
        else
            write(connFD, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        read(connFD, buffer, sizeof(buffer)); // Dummy read
    }
    else
    {
        // ERROR while getting balance
        return false;
    }
}

bool change_password(int connFD)
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

    if (strcmp(crypt(readBuffer, SALT_BAE), loggedInCustomer.password) == 0)
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

            strcpy(loggedInCustomer.password, newPassword);

            int customerFileDescriptor = open(ACCOUNT_FILE, O_WRONLY);
            if (customerFileDescriptor == -1)
            {
                perror("Error opening customer file!");
                unlock_critical_section(&semOp);
                return false;
            }

            off_t offset = lseek(customerFileDescriptor, loggedInCustomer.id * sizeof(struct Account), SEEK_SET);
            if (offset == -1)
            {
                perror("Error seeking to the customer record!");
                unlock_critical_section(&semOp);
                return false;
            }

            struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct Account), getpid()};
            int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining write lock on customer record!");
                unlock_critical_section(&semOp);
                return false;
            }

            writeBytes = write(customerFileDescriptor, &loggedInCustomer, sizeof(struct Account));
            if (writeBytes == -1)
            {
                perror("Error storing updated customer password into customer record!");
                unlock_critical_section(&semOp);
                return false;
            }

            lock.l_type = F_UNLCK;
            lockingStatus = fcntl(customerFileDescriptor, F_SETLK, &lock);

            close(customerFileDescriptor);

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

bool lock_critical_section(struct sembuf *semOp)
{
    semOp->sem_flg = SEM_UNDO;
    semOp->sem_op = -1;
    semOp->sem_num = 0;
    int semopStatus = semop(semIdentifier, semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while locking critical section");
        return false;
    }
    return true;
}

bool unlock_critical_section(struct sembuf *semOp)
{
    semOp->sem_op = 1;
    int semopStatus = semop(semIdentifier, semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error while operating on semaphore!");
        _exit(1);
    }
    return true;
}

void write_transaction_to_array(int *transactionArray, int ID)
{
    // Check if there's any free space in the array to write the new transaction ID
    int iter = 0;
    while (transactionArray[iter] != -1)
        iter++;

    if (iter >= MAX_TRANSACTIONS)
    {
        // No space
        for (iter = 1; iter < MAX_TRANSACTIONS; iter++)
            // Shift elements one step back discarding the oldest transaction
            transactionArray[iter - 1] = transactionArray[iter];
        transactionArray[iter - 1] = ID;
    }
    else
    {
        // Space available
        transactionArray[iter] = ID;
    }
}

int write_transaction_to_file(int accountNumber, long int oldBalance, long int newBalance, bool operation)
{
    time_t currentTime;
    struct tm *timeInfo;
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    struct Transaction newTransaction;
    newTransaction.accountNumber = accountNumber;
    newTransaction.oldBalance = oldBalance;
    newTransaction.newBalance = newBalance;
    newTransaction.operation = operation;
    strftime(newTransaction.transactionTime, sizeof(newTransaction.transactionTime), "%Y-%m-%d %H:%M:%S", timeInfo);

    ssize_t readBytes, writeBytes;

    int transactionFileDescriptor = open(TRANSACTION_FILE, O_CREAT | O_APPEND | O_RDWR, S_IRWXU);

    // Get most recent transaction number
    off_t offset = lseek(transactionFileDescriptor, -sizeof(struct Transaction), SEEK_END);
    if (offset >= 0)
    {
        // There exists at least one transaction record
        struct Transaction prevTransaction;
        readBytes = read(transactionFileDescriptor, &prevTransaction, sizeof(struct Transaction));

        newTransaction.transactionID = prevTransaction.transactionID + 1;
    }
    else
        // No transaction records exist
        newTransaction.transactionID = 0;

    writeBytes = write(transactionFileDescriptor, &newTransaction, sizeof(struct Transaction));

    return newTransaction.transactionID;
}

// =====================================================




bool handle_loan_operations(int connFD) {
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;
    int choice;

    // Send loan operation menu to the client
    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "Loan Menu:\n1. Apply for Loan\n2. Check Loan Status\nEnter your choice: ");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing loan menu to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading choice from client!");
        return false;
    }

    choice = atoi(readBuffer);

    switch (choice) {
        case 1: {
            // Apply for Loan
            struct Loan newLoan;
            newLoan.customerID = loggedInCustomer.id;
            newLoan.accountNumber = loggedInCustomer.accountNumber;
            newLoan.isApproved = false; // Initially, the loan is not approved

            time_t currentTime;
            struct tm *timeInfo;
            time(&currentTime);
            timeInfo = localtime(&currentTime);
            strftime(newLoan.applicationDate, sizeof(newLoan.applicationDate), "%Y-%m-%d", timeInfo);
            strcpy(newLoan.approvalDate, "Pending");

            // Ask the user for loan amount
            writeBytes = write(connFD, "Enter the loan amount: ", strlen("Enter the loan amount: "));
            if (writeBytes == -1) {
                perror("Error writing loan amount prompt to client!");
                return false;
            }

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1) {
                perror("Error reading loan amount from client!");
                return false;
            }

            newLoan.loanAmount = atol(readBuffer);

            // Save loan information in the file
            int loanFileDescriptor = open(LOAN_FILE, O_CREAT | O_APPEND | O_RDWR, S_IRWXU);
            if (loanFileDescriptor == -1) {
                perror("Error opening loan file!");
                return false;
            }

            // Set loanID
            off_t offset = lseek(loanFileDescriptor, -sizeof(struct Loan), SEEK_END);
            if (offset >= 0) {
                struct Loan lastLoan;
                readBytes = read(loanFileDescriptor, &lastLoan, sizeof(struct Loan));
                newLoan.loanID = lastLoan.loanID + 1;
            } else {
                newLoan.loanID = 0; // First loan
            }

            writeBytes = write(loanFileDescriptor, &newLoan, sizeof(struct Loan));
            if (writeBytes == -1) {
                perror("Error writing loan details to file!");
                close(loanFileDescriptor);
                return false;
            }

            close(loanFileDescriptor);

            write(connFD, "Loan application submitted successfully.^", strlen("Loan application submitted successfully.^"));
            read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

            break;
        }

        case 2: {
            // Check Loan Status
            struct Loan loan;
            int loanFileDescriptor = open(LOAN_FILE, O_RDONLY);
            if (loanFileDescriptor == -1) {
                perror("Error opening loan file!");
                return false;
            }

            bool foundLoan = false;
            while (read(loanFileDescriptor, &loan, sizeof(struct Loan)) > 0) {
                if (loan.customerID == loggedInCustomer.id) {
                    foundLoan = true;
                    bzero(writeBuffer, sizeof(writeBuffer));
                    sprintf(writeBuffer, "Loan ID: %d, Amount: ₹ %.2f, Status: %s^", loan.loanID, loan.loanAmount, loan.isApproved ? "Approved" : "Pending");
                    write(connFD, writeBuffer, strlen(writeBuffer));
                    read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
                }
            }

            close(loanFileDescriptor);

            if (!foundLoan) {
                write(connFD, "No loan applications found.^", strlen("No loan applications found.^"));
                read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
            }

            break;
        }

        default:
            write(connFD, "Invalid choice!^", strlen("Invalid choice!^"));
            read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
            return false;
    }

    return true;
}




// bool apply_feedback(int connFD) {
//     int feedbackFileDescriptor, feedbackID;
//     struct Feedback feedback;
//     off_t offset;
//     time_t currentTime;
//     char readBuffer[2000], writeBuffer[2000];
//     ssize_t readBytes, writeBytes;
//     int choice;

//     // Loop to handle customer operations
//     do {
//         // Send customer menu to client
//         bzero(writeBuffer, sizeof(writeBuffer));
//         sprintf(writeBuffer, "\nCustomer Menu:\n1. Give Feedback\n2. View Feedback Response\n3. Exit\nEnter your choice: ");
//         writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
//         if (writeBytes == -1) {
//             perror("Error writing menu to client!");
//             return false;
//         }

//         // Receive the customer's choice
//         bzero(readBuffer, sizeof(readBuffer));
//         readBytes = read(connFD, readBuffer, sizeof(readBuffer));
//         if (readBytes == -1) {
//             perror("Error reading choice from client!");
//             return false;
//         }

//         choice = atoi(readBuffer);

//         switch (choice) {
//             case 1: {  // Giving feedback
//                 feedback.customerID = loggedInCustomer.id;

//                 // Ask for feedback input
//                 writeBytes = write(connFD, "Enter your feedback: ", strlen("Enter your feedback: "));
//                 if (writeBytes == -1) {
//                     perror("Error writing feedback prompt to client!");
//                     return false;
//                 }

//                 // Receive feedback from the client
//                 bzero(feedback.feedback, sizeof(feedback.feedback));
//                 readBytes = read(connFD, feedback.feedback, sizeof(feedback.feedback));
//                 if (readBytes == -1) {
//                     perror("Error receiving feedback!");
//                     return false;
//                 }

//                 // No response yet
//                 strcpy(feedback.response, "");
//                 currentTime = time(NULL);
//                 strftime(feedback.date, sizeof(feedback.date), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));

//                 // Open the feedback file
//                 feedbackFileDescriptor = open(FEEDBACK_FILE, O_CREAT | O_APPEND | O_RDWR, S_IRWXU);
//                 if (feedbackFileDescriptor == -1) {
//                     perror("Error opening feedback file!");
//                     return false;
//                 }

//                 // Determine feedback ID
//                 offset = lseek(feedbackFileDescriptor, -sizeof(struct Feedback), SEEK_END);
//                 if (offset >= 0) {
//                     struct Feedback prevFeedback;
//                     read(feedbackFileDescriptor, &prevFeedback, sizeof(struct Feedback));
//                     feedback.feedbackID = prevFeedback.feedbackID + 1;
//                 } else {
//                     feedback.feedbackID = 1;  // First feedback
//                 }

//                 // Write feedback to the file
//                 writeBytes = write(feedbackFileDescriptor, &feedback, sizeof(struct Feedback));
//                 if (writeBytes == -1) {
//                     perror("Error writing feedback to file!");
//                     close(feedbackFileDescriptor);
//                     return false;
//                 }

//                 close(feedbackFileDescriptor);

//                 // Inform the client of successful submission and feedback ID
//                 bzero(writeBuffer, sizeof(writeBuffer));
//                 sprintf(writeBuffer, "Feedback submitted successfully! Your feedback ID is %d.\n", feedback.feedbackID);
//                 write(connFD, writeBuffer, strlen(writeBuffer));

//                 break;
//             }

//             case 2: {  // Viewing feedback response
//                 // Ask for feedback ID
//                 writeBytes = write(connFD, "Enter your feedback ID to view the response: ", strlen("Enter your feedback ID to view the response: "));
//                 if (writeBytes == -1) {
//                     perror("Error writing feedback ID prompt to client!");
//                     return false;
//                 }

//                 // Receive feedback ID from client
//                 bzero(readBuffer, sizeof(readBuffer));
//                 readBytes = read(connFD, readBuffer, sizeof(readBuffer));
//                 if (readBytes == -1) {
//                     perror("Error reading feedback ID from client!");
//                     return false;
//                 }

//                 feedbackID = atoi(readBuffer);

//                 feedbackFileDescriptor = open(FEEDBACK_FILE, O_RDONLY);
//                 if (feedbackFileDescriptor == -1) {
//                     perror("Error opening feedback file!");
//                     return false;
//                 }

//                 int found = 0;
//                 while (read(feedbackFileDescriptor, &feedback, sizeof(struct Feedback)) > 0) {
//                     if (feedback.feedbackID == feedbackID && feedback.customerID == loggedInCustomer.id) {
//                         bzero(writeBuffer, sizeof(writeBuffer));
//                         sprintf(writeBuffer, "Feedback: %s\nResponse: %s\n", feedback.feedback, feedback.response);
//                         write(connFD, writeBuffer, strlen(writeBuffer));
//                         found = 1;
//                         break;
//                     }
//                 }

//                 if (!found) {
//                     write(connFD, "Feedback not found or no response yet.\n", strlen("Feedback not found or no response yet.\n"));
//                 }

//                 close(feedbackFileDescriptor);
//                 break;
//             }

//             case 3:
//                 write(connFD, "Exiting...\n", strlen("Exiting...\n"));
//                 break;

//             default:
//                 write(connFD, "Invalid choice! Please try again.\n", strlen("Invalid choice! Please try again.\n"));
//         }
//     } while (choice != 3);

//     return true;
// }



bool apply_feedback(int connFD) {
    int feedbackFileDescriptor;
    struct Feedback feedback;
    time_t currentTime;
    char writeBuffer[2000];
    ssize_t writeBytes;

    // Ask for feedback input
    writeBytes = write(connFD, "Enter your feedback: ", strlen("Enter your feedback: "));
    if (writeBytes == -1) {
        perror("Error writing feedback prompt to client!");
        return false;
    }

    // Receive feedback from the client
    bzero(feedback.feedback, sizeof(feedback.feedback));
    ssize_t readBytes = read(connFD, feedback.feedback, sizeof(feedback.feedback) - 1);
    if (readBytes == -1) {
        perror("Error receiving feedback!");
        return false;
    }
    feedback.feedback[readBytes] = '\0'; // Null-terminate the feedback string

    // Initialize response and date fields
    strcpy(feedback.response, ""); // No response yet
    currentTime = time(NULL);
    strftime(feedback.date, sizeof(feedback.date), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));

    // Open the feedback file
    feedbackFileDescriptor = open(FEEDBACK_FILE, O_CREAT | O_APPEND | O_RDWR, S_IRUSR | S_IWUSR);
    if (feedbackFileDescriptor == -1) {
        perror("Error opening feedback file!");
        return false;
    }

    // Determine feedback ID
    feedback.feedbackID = 1;  // Default to 1 for the first feedback
    off_t offset = lseek(feedbackFileDescriptor, -sizeof(struct Feedback), SEEK_END);
    if (offset >= 0) {
        struct Feedback prevFeedback;
        // Read the last feedback if it exists
        if (read(feedbackFileDescriptor, &prevFeedback, sizeof(struct Feedback)) == sizeof(struct Feedback)) {
            feedback.feedbackID = prevFeedback.feedbackID + 1; // Increment feedback ID
        }
    }

    // Set customer ID and name (assuming `loggedInCustomer` is defined)
    feedback.customerID = loggedInCustomer.id; // Get the logged-in customer's ID
    strncpy(feedback.customerName, loggedInCustomer.name, sizeof(feedback.customerName) - 1); // Copy customer name
    feedback.customerName[sizeof(feedback.customerName) - 1] = '\0'; // Ensure null termination

    // Write feedback to the file
    writeBytes = write(feedbackFileDescriptor, &feedback, sizeof(struct Feedback));
    if (writeBytes == -1) {
        perror("Error writing feedback to file!");
        close(feedbackFileDescriptor);
        return false;
    }

    close(feedbackFileDescriptor);

    // Inform the client of successful submission and feedback ID
    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "Feedback submitted successfully! Your feedback ID is %d.\n", feedback.feedbackID);
    write(connFD, writeBuffer, strlen(writeBuffer));

    return true;
}


bool transfer_funds(int connFD)
{
    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;

    struct Account account1, account2; // account1 = sender, account2 = receiver
    account1.id = loggedInCustomer.id; // Customer 1 (logged in customer)
    long int transferAmount = 0;
    int receiverID = 0;

    // Lock the critical section
    struct sembuf semOp;
    lock_critical_section(&semOp);

    // Get sender (Customer 1) account details
    if (!get_account_details(connFD, &account1))
    {
        unlock_critical_section(&semOp);
        return false;
    }

    if (!account1.isActive)
    {
        write(connFD, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        unlock_critical_section(&semOp);
        return false;
    }

    // Ask for Customer 2's ID (receiver)
    writeBytes = write(connFD, "Enter Customer ID to transfer funds: ", strlen("Enter Customer ID to transfer funds: "));
    if (writeBytes == -1)
    {
        perror("Error writing request for receiver ID to client!");
        unlock_critical_section(&semOp);
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading receiver ID from client!");
        unlock_critical_section(&semOp);
        return false;
    }

    receiverID = atoi(readBuffer);

    // Get receiver (Customer 2) account details
    account2.id = receiverID;
    if (!get_account_details(connFD,&account2)) // Use a function to get details by ID (Customer 2)
    {
        write(connFD, "Invalid Receiver ID.", strlen("Invalid Receiver ID."));
        unlock_critical_section(&semOp);
        return false;
    }

    if (!account2.isActive)
    {
        write(connFD, ACCOUNT_DEACTIVATED, strlen(ACCOUNT_DEACTIVATED));
        unlock_critical_section(&semOp);
        return false;
    }

    // Ask for transfer amount
    writeBytes = write(connFD, "Enter the amount to transfer: ", strlen("Enter the amount to transfer: "));
    if (writeBytes == -1)
    {
        perror("Error writing transfer amount request to client!");
        unlock_critical_section(&semOp);
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading transfer amount from client!");
        unlock_critical_section(&semOp);
        return false;
    }

    transferAmount = atol(readBuffer);

    if (transferAmount > 0 && account1.balance >= transferAmount)
    {
        // Deduct amount from Customer 1's balance
        int transactionID1 = write_transaction_to_file(account1.id, account1.balance, account1.balance - transferAmount, 0); // 0 for withdrawal
        write_transaction_to_array(account1.trancsaction, transactionID1);
        account1.balance -= transferAmount;

        // Add amount to Customer 2's balance
        int transactionID2 = write_transaction_to_file(account2.id, account2.balance, account2.balance + transferAmount, 1); // 1 for deposit
        write_transaction_to_array(account2.trancsaction, transactionID2);
        account2.balance += transferAmount;

        // Update both accounts in the file
        int accountFileDescriptor = open(ACCOUNT_FILE, O_WRONLY);

        // Lock and update Customer 1 (sender)
        off_t offset1 = lseek(accountFileDescriptor, account1.id * sizeof(struct Account), SEEK_SET);
        struct flock lock1 = {F_WRLCK, SEEK_SET, offset1, sizeof(struct Account), getpid()};
        if (fcntl(accountFileDescriptor, F_SETLKW, &lock1) == -1)
        {
            perror("Error locking sender account for writing!");
            unlock_critical_section(&semOp);
            return false;
        }
        writeBytes = write(accountFileDescriptor, &account1, sizeof(struct Account));
        lock1.l_type = F_UNLCK;
        fcntl(accountFileDescriptor, F_SETLK, &lock1); // Unlock Customer 1

        // Lock and update Customer 2 (receiver)
        off_t offset2 = lseek(accountFileDescriptor, account2.id * sizeof(struct Account), SEEK_SET);
        struct flock lock2 = {F_WRLCK, SEEK_SET, offset2, sizeof(struct Account), getpid()};
        if (fcntl(accountFileDescriptor, F_SETLKW, &lock2) == -1)
        {
            perror("Error locking receiver account for writing!");
            unlock_critical_section(&semOp);
            return false;
        }
        writeBytes = write(accountFileDescriptor, &account2, sizeof(struct Account));
        lock2.l_type = F_UNLCK;
        fcntl(accountFileDescriptor, F_SETLK, &lock2); // Unlock Customer 2

        close(accountFileDescriptor);

        write(connFD, "Transfer successful!", strlen("Transfer successful!"));
        read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

        get_balance(connFD); // Show balance of Customer 1

        unlock_critical_section(&semOp);

        return true;
    }
    else
    {
        write(connFD, "Insufficient balance or invalid amount!", strlen("Insufficient balance or invalid amount!"));
    }

    read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    unlock_critical_section(&semOp);
    return false;
}




// // Function to add a transaction to the customer's record
// void add_transaction(Customer* customer, const char* type, float amount) {
//     if (customer->transaction_count < MAX_TRANSACTIONS) {
//         Transaction* transaction = &customer->transactions[customer->transaction_count];
//         strncpy(transaction->type, type, sizeof(transaction->type) - 1);
//         transaction->amount = amount;

//         // Get the current date
//         time_t t = time(NULL);
//         struct tm tm = *localtime(&t);
//         snprintf(transaction->date, sizeof(transaction->date), "%02d-%02d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

//         customer->transaction_count++;
//     } else {
//         printf("Transaction history is full for account number %d.\n", customer->account_number);
//     }
// }

// // Function to view transaction history for a given account number
// void view_transaction_history(Customer* customers, int customer_count, int account_number) {
//     for (int i = 0; i < customer_count; i++) {
//         if (customers[i].account_number == account_number) {
//             printf("Transaction history for account number %d:\n", account_number);
//             for (int j = 0; j < customers[i].transaction_count; j++) {
//                 printf("%s of %.2f on %s\n", customers[i].transactions[j].type,
//                        customers[i].transactions[j].amount,
//                        customers[i].transactions[j].date);
//             }
//             return;
//         }
//     }
//     printf("Account number %d not found.\n", account_number);
// }





#endif




// #include <stdio.h>
// #include <fcntl.h>   // For open(), O_RDWR, file locking (flock)
// #include <unistd.h>  // For close(), read(), write()
// #include <string.h>  // For memset(), strcmp()
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <errno.h>

// #define MAX_LOANS 100
// #define LOAN_FILE "loan_applications.dat"

// struct LoanApplication {
//     int loan_id;
//     char customer_name[100];
//     double loan_amount;
//     char status[20];  // "Pending", "Approved", "Rejected"
// };

// // Function to apply for a loan
// bool apply_for_loan(int connFD) {
//     int fd;
//     struct LoanApplication loan;
//     struct LoanApplication loans[MAX_LOANS];
//     ssize_t bytesRead;
//     int nextLoanID = 1;

//     // Open the loan applications file (create it if it doesn't exist)
//     LoanFD = open(LOAN_FILE, O_RDWR | O_CREAT, 0666);
//     if (LoanFD == -1) {
//         perror("Error opening loan file");
//         return false;
//     }

//     // Lock the file to prevent race conditions
//     if (flock(, LOCK_EX) == -1) {
//         perror("Error locking loan file");
//         close(fd);
//         return false;
//     }

//     // Read the loan applications to find the next available loan ID
//     bytesRead = read(fd, loans, sizeof(loans));
//     if (bytesRead == -1) {
//         perror("Error reading loan file");
//         flock(fd, LOCK_UN);  // Unlock file before exiting
//         close(fd);
//         return false;
//     }

//     // Find the next available loan ID
//     for (int i = 0; i < MAX_LOANS; i++) {
//         if (loans[i].loan_id > 0) {
//             nextLoanID = loans[i].loan_id + 1;
//         }
//     }

//     // Prompt the customer for loan details
//     char buffer[1024];
//     write(connFD, "Enter Loan Amount: ", 19);
//     read(connFD, &loan.loan_amount, sizeof(loan.loan_amount));

//     write(connFD, "Enter Your Name: ", 17);
//     read(connFD, loan.customer_name, sizeof(loan.customer_name));

//     // Assign the next loan ID and set the status to "Pending"
//     loan.loan_id = nextLoanID;
//     strcpy(loan.status, "Pending");

//     // Write the new loan application to the file
//     lseek(fd, 0, SEEK_END);  // Move to the end of the file for appending
//     if (write(fd, &loan, sizeof(struct LoanApplication)) == -1) {
//         perror("Error writing to loan file");
//         flock(fd, LOCK_UN);  // Unlock file before exiting
//         close(fd);
//         return false;
//     }

//     // Notify the customer about the successful loan application
//     sprintf(buffer, "Loan application submitted successfully. Loan ID: %d\n", loan.loan_id);
//     write(connFD, buffer, strlen(buffer));

//     // Unlock the file after writing
//     if (flock(fd, LOCK_UN) == -1) {
//         perror("Error unlocking loan file");
//         close(fd);
//         return false;
//     }

//     close(fd);
//     return true;
// }
