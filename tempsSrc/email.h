/* 
 * File:email.h   
 * Author:Stephen Appiah
 * DATE: 09/01/2018
 * Version : 1.0
 * 
 *  This file contains the main structure of the definition of email.
 * 
 */

#ifndef EMAIL_H
#define EMAIL_H


#include "common.h"

#define MAX_EMAIL_ID 50 //max length of characters for the id field
#define MAX_SUBJECT 100 //max length of characters for the subject field
#define MAX_BODY    150 //max length of characters for the body field
#define DEFAULT_MAX_FIELD 50 //max length of characters for date field

// -------------------- Email Structure ----------------------------------

typedef struct {
    char id[MAX_EMAIL_ID]; //stores a generated unique id such taht id is [ID NUMBER]_EDA_email
    char subject[MAX_SUBJECT]; //stores the subject for the message
    char from[DEFAULT_MAX_FIELD]; //saves the from 
    char to[DEFAULT_MAX_FIELD]; //saves the to 
    char date[DEFAULT_MAX_FIELD]; //saves the current date and time  
    char body[MAX_BODY]; ////saves the body of the message 
    int empty; // this field is empty when the Email structure is defined with the default values  , 
    //determinants whether the message contains valid information or not
    int referenced; //determinants how many folders the email is being referenced
} Email;


// -------------------- Email Header Methods ----------------------------------

void init_email(Email* email);

char* get_email_id(Email* email);
char* get_email_subject(Email* email);
int is_email_empty(Email* email);

void copy_email(Email* dest_email, Email* src_email);

void read_email_interactive(Email* email);
int load_email_from_file(FILE* fd, Email *email);
int write_email_to_file(FILE* fd, Email* email);

int email_to_file_section_date(Email *email, FILE *fd, char *format, int *result);
int email_to_file_section_from(Email *email, FILE *fd, char *format, int *result);
int email_to_file_section_to(Email *email, FILE *fd, char *format, int *result);
int email_to_file_section_messageid(Email *email, FILE *fd, char *format, int *result);
int email_to_file_section_subject(Email *email, FILE *fd, char *format, int *result);
int email_to_file_section_body(Email *email, FILE *fd, char *format, int *result);

int validate_email(Email *email);

int is_valid_email(char *email);

int is_valid_message(char *message);
#endif /* EMAIL_H */

