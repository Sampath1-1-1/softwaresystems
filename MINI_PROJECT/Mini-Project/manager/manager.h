
#ifndef MANAGER_FUNCTIONS
#define EMANAGER_FUNCTIONS

#include "../resources/commonfunc.h"
#include "../record/employee.h"
#include "../record/account.h"
#include "../record/loanapply.h"
#include "../record/feedback.h"
#include "../customer/customer.h"

bool manager_operation_handler(int connFD);
bool activate_deactivate_customer_accounts(int connFD);
// bool delete_account(int connFD);
bool assign_loan_application_processes_to_employees(int connFD);
bool review_customer_feedback(int connFD);
bool change_m_password(int connFD);
struct Employee loggedInManager;
// Function Prototypes =================================bool employee_operation_handler(int connFD)
bool manager_operation_handler(int connFD)
{

    if (login_handler(4, connFD, NULL, &loggedInManager))
    {
        ssize_t writeBytes, readBytes;            // Number of bytes read from / written to the client
        char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, MANAGER_LOGIN_SUCCESS);
        if (loggedInManager.isManager == true)
        {
            while (1)
            {
                strcat(writeBuffer, "\n");
                strcat(writeBuffer, MANAGER_MENU);
                writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
                if (writeBytes == -1)
                {
                    perror("Error while writing MANAGER_MENU to client!");
                    return false;
                }
                bzero(readBuffer, sizeof(readBuffer));
                readBytes = read(connFD, readBuffer, sizeof(readBuffer));
                if (readBytes == -1)
                {
                    perror("Error while reading client's choice for MANAGER_MENU");
                    return false;
                }

                int choice = atoi(readBuffer);
                switch (choice)
                {
                // case 1:
                //     activate_deactivate_customer_accounts(connFD);
                //     break;
                case 2:
                    assign_loan_application_processes_to_employees(connFD);
                    break;
                case 3:
                    review_customer_feedback(connFD);
                    break;
                case 4:
                    change_m_password(connFD);
                    break;

                default:
                    writeBytes = write(connFD, MANAGER_LOGOUT, strlen(MANAGER_LOGOUT));
                    return false;
                }
            }
        }
        else
        {
            bzero(writeBuffer, strlen(writeBuffer));
            strcat(writeBuffer, "enter employee is not manager");
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error writing login prompt to client!");
                return false;
            }
        }
    }
    else
    {
        //  MANAGER LOGIN FAILED
        return false;
    }
    return true;
}
bool change_m_password(int connFD)
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

    if (strcmp(crypt(readBuffer, SALT_BAE), loggedInManager.password) == 0)
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

            strcpy(loggedInManager.password, newPassword);

            int employeeFileDescriptor = open(ACCOUNT_FILE, O_WRONLY);
            if (employeeFileDescriptor == -1)
            {
                perror("Error opening customer file!");
                unlock_critical_section(&semOp);
                return false;
            }

            off_t offset = lseek(employeeFileDescriptor, loggedInManager.id * sizeof(struct Employee), SEEK_SET);
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

            writeBytes = write(employeeFileDescriptor, &loggedInManager, sizeof(struct Employee));
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

bool view_all_employees(int connFD) {
    ssize_t writeBytes;
    char writeBuffer[1000];

    // Open the employee file for reading
    int employeeFileDescriptor = open(EMPLOYEE_FILE, O_RDONLY);
    if (employeeFileDescriptor == -1) {
        perror("Error opening employee file!");
        return false;
    }

    struct Employee employee;
    bzero(writeBuffer, sizeof(writeBuffer));

    strcat(writeBuffer, "Employee Details:\n");
    strcat(writeBuffer, "ID\tName\tPosition\n");

    // Read all employee records from the file
    while (read(employeeFileDescriptor, &employee, sizeof(struct Employee)) > 0) {
        char employeeDetails[200];
        sprintf(employeeDetails, "%d\t%s\t%s\n", employee.id, employee.name, employee.isManager ? "Manager" : "Employee");
        strcat(writeBuffer, employeeDetails);
    }

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing employee details to client!");
        close(employeeFileDescriptor);
        return false;
    }

    close(employeeFileDescriptor);
    return true;
}


bool assign_loan_application_processes_to_employees(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    // First, display all the employees
    if (!view_all_employees(connFD)) {
        return false;
    }

    // Ask the manager to enter the employee ID to assign the loan
    writeBytes = write(connFD, "Enter the Employee ID to assign the loan process: ", strlen("Enter the Employee ID to assign the loan process: "));
    if (writeBytes == -1) {
        perror("Error writing employee ID request to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading employee ID from client!");
        return false;
    }

    int employeeId = atoi(readBuffer);

    // Now, find a loan application to assign
    writeBytes = write(connFD, "Viewing all pending loan applications:\n", strlen("Viewing all pending loan applications:\n"));
    if (writeBytes == -1) {
        perror("Error writing loan view message to client!");
        return false;
    }

    // Open the loan application file
    int loanFileDescriptor = open(LOAN_FILE, O_RDWR);
    if (loanFileDescriptor == -1) {
        perror("Error opening loan file!");
        return false;
    }

    struct Loan loanApp;
    bool loanAssigned = false;

    while (read(loanFileDescriptor, &loanApp, sizeof(struct Loan)) > 0) {
        // Display each loan application
        if (loanApp.assignedTo == -1) {  // Check if loan is unassigned
            char loanDetails[500];
          sprintf(loanDetails, "Loan ID: %d\nCustomer ID: %d\nAmount: %.2f\nStatus: %s\n", loanApp.loanID, loanApp.customerID, loanApp.loanAmount, 
        loanApp.isApproved ? "Approved" : "Not Approved");

write(connFD, loanDetails, strlen(loanDetails));


            // Assign loan to the employee
            loanApp.assignedTo = employeeId;

            // Seek to the position of this loan record in the file
            lseek(loanFileDescriptor, -sizeof(struct Loan), SEEK_CUR);

            // Write the updated loan record back to the file
            if (write(loanFileDescriptor, &loanApp, sizeof(struct Loan)) == -1) {
                perror("Error assigning loan to employee!");
                close(loanFileDescriptor);
                return false;
            }

            loanAssigned = true;
            break;
        }
    }

    if (!loanAssigned) {
        write(connFD, "No pending loan applications to assign.\n", strlen("No pending loan applications to assign.\n"));
    } else {
        write(connFD, "Loan successfully assigned to the employee!\n", strlen("Loan successfully assigned to the employee!\n"));
    }

    close(loanFileDescriptor);
    return true;
}


bool review_customer_feedback(int connFD) {
    ssize_t writeBytes;
    char writeBuffer[1000];
    
    // Open the feedback file for reading
    int feedbackFileDescriptor = open(FEEDBACK_FILE, O_RDONLY);
    if (feedbackFileDescriptor == -1) {
        perror("Error opening feedback file!");
        return false;
    }

    struct Feedback feedback;
    bzero(writeBuffer, sizeof(writeBuffer));

    strcat(writeBuffer, "Customer Feedback Details:\n");
    strcat(writeBuffer, "Feedback ID\tCustomer ID\tFeedback Message\n");

    // Read all feedback records from the file
    while (read(feedbackFileDescriptor, &feedback, sizeof(struct Feedback)) > 0) {
        char feedbackDetails[1200];
        
        // Prepare feedback details for display
        sprintf(feedbackDetails, "%d\t%d\t%s\n",
                feedback.feedbackID,
                feedback.customerID,
                feedback.feedback);

        strcat(writeBuffer, feedbackDetails);

        // If buffer is almost full, write and reset the buffer
        if (strlen(writeBuffer) > 800) {
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1) {
                perror("Error writing feedback details to client!");
                close(feedbackFileDescriptor);
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));
        }
    }

    // Send remaining feedback details in the buffer
    if (strlen(writeBuffer) > 0) {
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error writing final feedback details to client!");
            close(feedbackFileDescriptor);
            return false;
        }
    }

    close(feedbackFileDescriptor);
    return true;
}


bool activate_deactivate_customer_account(int connFD) {
    ssize_t writeBytes, readBytes;
    char readBuffer[1000], writeBuffer[1000];
    int accountID;

    // Request the account ID from the manager
    bzero(writeBuffer, sizeof(writeBuffer));
    strcat(writeBuffer, "Enter the Account ID to activate/deactivate the account: ");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing request to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading account ID from client!");
        return false;
    }
    accountID = atoi(readBuffer);

    // Open the account file for reading and writing
    int accountFileDescriptor = open(ACCOUNT_FILE, O_RDWR);
    if (accountFileDescriptor == -1) {
        perror("Error opening account file!");
        return false;
    }

    struct Account account;
    bool accountFound = false;

    // Search for the account by account ID in the file
    while (read(accountFileDescriptor, &account, sizeof(struct Account)) > 0) {
        if (account.id == accountID) {
            accountFound = true;
            break;
        }
    }

    if (!accountFound) {
        // Account not found, notify manager
        bzero(writeBuffer, sizeof(writeBuffer));
        strcat(writeBuffer, "Account ID not found in the system.\n");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        close(accountFileDescriptor);
        return false;
    }

    // Notify the manager about the current account status
    bzero(writeBuffer, sizeof(writeBuffer));
    if (account.isActive) {
        strcat(writeBuffer, "The account is currently active.\n");
    } else {
        strcat(writeBuffer, "The account is currently deactivated.\n");
    }
    strcat(writeBuffer, "Enter 1 to activate or 0 to deactivate the account: ");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing account status to client!");
        close(accountFileDescriptor);
        return false;
    }

    // Read the manager's choice
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading manager's choice from client!");
        close(accountFileDescriptor);
        return false;
    }

    int choice = atoi(readBuffer);
    if (choice == 1) {
        account.isActive = true;  // Activate the account
    } else if (choice == 0) {
        account.isActive = false; // Deactivate the account
    } else {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcat(writeBuffer, "Invalid choice. Operation canceled.\n");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        close(accountFileDescriptor);
        return false;
    }

    // Seek back to the correct position in the file and update the account status
    off_t offset = lseek(accountFileDescriptor, -sizeof(struct Account), SEEK_CUR);
    if (offset == -1) {
        perror("Error seeking in account file!");
        close(accountFileDescriptor);
        return false;
    }

    writeBytes = write(accountFileDescriptor, &account, sizeof(struct Account));
    if (writeBytes == -1) {
        perror("Error updating account status!");
        close(accountFileDescriptor);
        return false;
    }

    // Notify the manager of success
    bzero(writeBuffer, sizeof(writeBuffer));
    if (account.isActive) {
        strcat(writeBuffer, "The account has been successfully activated.\n");
    } else {
        strcat(writeBuffer, "The account has been successfully deactivated.\n");
    }
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error sending success message to client!");
        close(accountFileDescriptor);
        return false;
    }

    close(accountFileDescriptor);
    return true;
}

 
#endif