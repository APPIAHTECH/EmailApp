/* 
 * File:  
 * Author: 
 *
 * Fill Header!!
 */


#include "imports.h"

#include "email.h"
#include "database.h"
#include"utils.h"

/**
 * Initializes an email just after memory is reserved.
 * @param email
 */
void init_email(Email* email) {

    //Defautl email structure
    strcpy(email->id, EMAIL_INIT_ID);
    strcpy(email->date, get_curent_date());
    strcpy(email->from, EMAIL_INIT_FROM);
    strcpy(email->to, EMAIL_INIT_TO);
    strcpy(email->subject, EMAIL_INIT_SUBJECT);
    strcpy(email->body, EMAIL_INIT_BODY);

    email->empty = TRUE;
    email->referenced = UNDEFINED;
}

/**
 * Copies the information from email to other.
 * Follows strcpy parameter setting.
 * @param dest_email
 * @param src_email
 */
void copy_email(Email* dest_email, Email* src_email) {

    strcpy(dest_email->id, src_email->id);
    strcpy(dest_email->body, src_email->body);
    strcpy(dest_email->date, src_email->date);
    strcpy(dest_email->from, src_email->from);
    strcpy(dest_email->to, src_email->to);
    strcpy(dest_email->subject, src_email->subject);
    dest_email->empty = src_email->empty;
    dest_email->referenced = src_email->referenced;
}

/**
 * Returns the email id.
 * @param email
 * @return 
 */
char* get_email_id(Email* email) {
    return email->id;
}

/** emailDB/3_EDA_email.txt

 * Returns the subject of the email.
 * @param email
 * @return 
 */
char* get_email_subject(Email* email) {
    return email->subject;
}

/**
 * Returns 1 if email is empty, 0 otherwise.
 * @param email
 * @return 
 */
int is_email_empty(Email* email) {
    return email->empty;
}

/**
 * TDB
 * @param email
 * @param buf
 * @param buf_size
 * @return 
 */
char* load_body(Email* email, char buf[], int buf_size) {

}

/**
 * Set the now date into the date email field.
 * @param email
 */
void set_email_date(Email* email) {
    strcpy(email->date, get_curent_date());
}

/**
 * Ask the user the required information in order to create an email and 
 * stores it into the referenced email structure.
 * @param email
 */
void read_email_interactive(Email* email) {

    char temp[DEFAULT_MAX_FIELD];
    char body[MAX_BODY];

    //Getting email info
    set_email_date(email);
    printf(EMAIL_INTERACTIVE_FROM);
    fgets(temp, DEFAULT_MAX_FIELD, stdin);

    //removing left and right white spaces and \n such that we only read text without first and last withe space
    str_remove_new_line(temp);
    str_trim(email->from, temp);

    printf(EMAIL_INTERACTIVE_TO);
    fgets(temp, DEFAULT_MAX_FIELD, stdin);

    str_remove_new_line(temp);
    str_trim(email->to, temp);

    printf(EMAIL_INTERACTIVE_SUBJECT);
    fgets(email->subject, DEFAULT_MAX_FIELD, stdin);

    str_remove_new_line(temp);
    str_trim(email->subject, temp);

    printf(EMAIL_INTERACTIVE_MESSAGE);
    fgets(body, MAX_BODY, stdin);

    str_remove_new_line(body);
    str_trim(email->body, body);

}

/**
 * Loads an email stored into a given file.
 * @param fd
 * @param email
 * @return 
 */
int load_email_from_file(FILE* fd, Email *email) {

    //Variables declarations
    char buff[MAX_BUF], temp[MAX_BUF], header[MAX_BUF], str_body[MAX_BUF];
    int pos, matched, str_body_len, str_to_entry;

    if (fd == NULL || email == NULL)
        return FAIL;

    memset(str_body, EMPTY, MAX_BUF); //int str all pos with \0
    str_body_len = 0;
    str_to_entry = 0;

    fgets(buff, MAX_BUF, fd);
    while (!feof(fd)) {
        str_remove_trash(buff);
        matched = sscanf(buff, FORMAT_EMAIL_FILE_STRUC, header);
        if (matched > 0) {
            pos = index_of(buff, FORMAT_FIELD_HEADERS);

            if (pos > 0) { //if it could find a : that means there was a header format

                str_remove_new_line(buff); //for getting the line without the first 2 white spaces 
                str_trim(temp, buff);

                //PREGUNTAR SI FROM Y TO TENEMOS QUE SACAR LOS MAILS QUE ESTAN DENTRO DE <MAIL>???
                //Setting up email information acording to each field
                if (strcmp(header, EMAIL_FILE_STRUC_DATE) == 0)
                    strcpy(email->date, temp);

                else if (strcmp(header, EMAIL_FILE_STRUC_FROM) == 0)
                    strcpy(email->from, temp);

                else if (strcmp(header, EMAIL_FILE_STRUC_TO) == 0)
                    strcpy(email->to, temp);

                else if (strcmp(header, EMAIL_FILE_STRUC_SUBJECT) == 0)
                    strcpy(email->subject, temp);

            } else { // if there were not a header format that means it contains emails body
                if (strcmp(buff, NEW_LINE) != 0) //if linea we are reading is not a line we add it to the str_body , such that str_body contains all the email body info
                {
                    //This secction dosen't allow a body of more than MAX_BODY can be store to the body of the email
                    //Getting the str_body that holds the result of all the body concatenated
                    str_body_len = strlen(str_body);
                    str_to_entry = strlen(buff); // the lenght of the line to be store

                    if (str_body_len + str_to_entry < MAX_BODY) //if the sum of the lenght of the str_body_len and str_to_entry , is greather than MAX_BODY , it means  the total amount is overpased the MAX_BODY so we dont add it to the str_body
                        strcat(str_body, buff);
                }

            }
        }
        fgets(buff, MAX_BUF, fd); //getting next line
    }

    //Storing body data to email body
    strcpy(email->body, str_body);
    return SUCCESS;
}

/**
 * Writes an email to a given file.
 * @param fd
 * @param email
 * @return 
 */
int write_email_to_file(FILE* fd, Email* email) {

    //Variables declarations
    char format[MAX_BUF];
    int result;

    if (fd != NULL && email != NULL) {
        //Creating email file format

        //Date section
        email_to_file_section_date(email, fd, format, &result);
        //From secttion
        email_to_file_section_from(email, fd, format, &result);
        //Message-ID section
        email_to_file_section_messageid(email, fd, format, &result);
        //Subject Section
        email_to_file_section_subject(email, fd, format, &result);
        //Body Section
        email_to_file_section_body(email, fd, format, &result);

        if (result != EOF) //EOF determinats if there was an error while writing to the file
            return SUCCESS;
        else
            return FAIL;

    } else
        return FAIL;

}

/**
 *  if from , to , is empty , set email.empty to true
 * @param email
 * @return 
 */
int validate_email(Email *email) {

    if (is_valid_email(email->from) != 0)
        email->empty = TRUE;
    else if (is_valid_email(email->to) != 0)
        email->empty = TRUE;
    else if (is_valid_message(email->body) != 0)
        email->empty = TRUE;
    else
        email->empty = FALSE;
}

//NOTE : TODO!!

/**
 * 
 * @param email
 * @return 
 */
int is_valid_email(char *email) {
    return 0;
}

/**
 * 
 * @param message
 * @return 
 */
int is_valid_message(char *message) {
    return 0;
}

int email_to_file_section_date(Email *email, FILE *fd, char *format, int *result) {

    strcpy(format, EMAIL_FILE_STRUC_DATE_STORE);
    strcat(format, email->date);
    strcat(format, NEW_LINE);
    *result = fputs(format, fd);
}

int email_to_file_section_from(Email *email, FILE *fd, char *format, int *result) {

    strcpy(format, EMAIL_FILE_STRUC_FROM_STORE);
    strcat(format, ARROW_LESS);
    strcat(format, email->from);
    strcat(format, ARROW_BID);
    strcat(format, NEW_LINE);
    *result = fputs(format, fd);
}

int email_to_file_section_to(Email *email, FILE *fd, char *format, int *result) {

    strcpy(format, EMAIL_FILE_STRUC_TO_STORE);
    strcat(format, ARROW_LESS);
    strcat(format, email->to);
    strcat(format, ARROW_BID);
    strcat(format, NEW_LINE);
    *result = fputs(format, fd);
}

int email_to_file_section_messageid(Email *email, FILE *fd, char *format, int *result) {
    strcpy(format, EMAIL_FILE_STRUC_MESSAGEID_STORE);
    strcat(format, email->id);
    strcat(format, NEW_LINE);
    *result = fputs(format, fd);
}

int email_to_file_section_subject(Email *email, FILE *fd, char *format, int *result) {
    strcpy(format, EMAIL_FILE_STRUC_SUBJECT_STORE);
    strcat(format, email->subject);
    strcat(format, NEW_LINE);
    *result = fputs(format, fd);
}

int email_to_file_section_body(Email *email, FILE *fd, char *format, int *result) {

    *result = fputs(NEW_LINE, fd);
    strcpy(format, email->body);
    strcat(format, NEW_LINE);
    *result = fputs(format, fd);
}