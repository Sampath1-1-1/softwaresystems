#ifndef FEEDBACK_RECORD
#define  FEEDBACK_RECORD
struct Feedback {
    int feedbackID;          // Unique ID for each feedback
    int customerID;         // Customer ID who provided the feedback
    char customerName[100]; // Customer name who provided the feedback
    char feedback[500];     // Feedback text
    char response[500];     // Response from the admin (if applicable)
    char date[20];          // Date and time when the feedback was given
};

#endif