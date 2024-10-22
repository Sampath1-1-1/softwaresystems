#ifndef LOAN_RECORD
#define LOAN_RECORD
struct Loan {
    int loanID;
    int customerID;
    int accountNumber;
    double loanAmount;
    bool isApproved;
     int  assignedTo;
    char applicationDate[20];
    char approvalDate[20];
};
#endif 