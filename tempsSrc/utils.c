/* 
 * File:utils.c   
 * Author:Stephen Appiah
 * DATE: 09/01/2018
 * Version : 1.0
 *
 * 
 * This file describes the utilities methods related to the utilities header
 * The general purpose of the file if to serve basic functions to all section of the program
 * 
 */


#include "imports.h"

#include "utils.h"
#include "database.h"

void flush_input() {
    //Variable declarations
    char c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/**
 * 
 * @param fd
 * @param msg
 */
void log_info(FILE* fd, char* msg) {
    if (LOG_LEVEL >= INFO_LEVEL) {
        fprintf(fd, "INFO: %s\n", msg);
        fflush(fd);
    }
}

/**
 * 
 * @param fd
 * @param msg
 */
void log_warn(FILE* fd, char* msg) {
    if (LOG_LEVEL >= WARN_LEVEL) {
        fprintf(fd, "WARNING: %s\n", msg);
        fflush(fd);
    }
}

/**
 * 
 * @param fd
 * @param msg
 */
void log_error(FILE* fd, char* msg) {
    if (LOG_LEVEL >= ERROR_LEVEL) {
        fprintf(fd, "ERROR: %s\n", msg);
        fflush(fd);
    }
}

/**
 * 
 * @param msg
 */
void display_error(char *msg) {
    perror(msg);
}

/**
 * prints all emails fields
 * @param email
 */
void print_email(Email *email) {

    printf(EMAIL_STRUCTURE_DATE, email->date);
    printf(EMAIL_STRUCTURE_ID, email->id);
    printf(EMAIL_STRUCTURE_FROM, email->from);
    printf(EMAIL_STRUCTURE_TO, email->to);
    printf(EMAIL_STRUCTURE_SUBJECT, email->subject);
    printf(EMAIL_STRUCTURE_BODY, email->body);
    /*
    printf("Empty %d \n", email->empty);
    printf("Referenced %d \n", email->referenced);*/
}

/**
 * prints all folder fields
 * @param folder
 */
void print_folder(Folder *folder) {

    //variable declaration
    int i;

    printf(FOLDER_STRUCTURE_NAME, folder->folder_name);
    printf(FOLDER_STRUCTURE_EMPTY, folder->empty);
    printf(FOLDER_STRUCTURE_SIZE, folder->size);
    printf(FOLDER_STRUCTURE_PROTECTED, folder->protected);
    puts(LINE);
    printf(FOLDER_STRUCTURE_FOLDER_MAILS_LIST);

    //Printing all emails that the folder contains
    for (i = 0; i < MAX_FOLDER_EMAILS; i++) {
        if (folder->emails[i] != NULL) {
            print_email(folder->emails[i]);
            puts(LINE);
        }
    }

}

/**
 * Prints all db information
 * Build for checking if db information are correct
 * @param db
 */
void print_database(Database *db) {

    //variable declaration
    int i = 0;

    puts(DATABASE_STRUCTURE);
    puts(LINE);

    printf(DATABASE_STRUCTURE_MSG_ID, db->msg_id_seed);
    printf(DATABASE_STRUCTURE_EMAIL_COUNT, db->email_count);
    printf(DATABASE_STRUCTURE_FOLDER_COUNT, db->folder_count);

    puts(LINE);
    puts(DATABASE_STRUCTURE_EMAIL_LIST);

    //for each email in db printing it 
    for (i = 0; i < MAX_EMAILS; i++)
        print_email(&db->emails[i]);

    puts(LINE);
    puts(DATABASE_STRUCTURE_FOLDER_LIST);
    //for each folder in db printing folder and folder emails
    for (i = 0; i < MAX_FOLDERS; i++) {
        printf(_FOLDER_TO_PRINT, &db->folders[i].folder_name, i);
        print_folder(&db->folders[i]);
    }

}

/**
 * This functions gets the currect date and tikmem an it returns a string of the result
 * @return 
 */
char *get_curent_date() {
    time_t orig_format;
    time(&orig_format);
    return asctime(gmtime(&orig_format));
}

/**
 * This function sets , where a \r is, to a \0.
 *Used to determinats end of str line
 * @param buff
 */
void str_remove_trash(char *buff) {

    buff = strchr(buff, '\r');
    if (buff != NULL)
        *buff = EMPTY; //to determinate where the str ends
}

/**
 * returns the postion of the first occurrence with the second str
 * @param str
 * @param searched
 * @return 
 */
int index_of(char * str, char * searched) {

    //Variable declarations
    int position = -1;
    char * result = NULL;

    //checking if str and searched are defined
    if (str && searched) {
        result = strstr(str, searched); //finds the first occurrence of the substring
        if (result) // if there was any occurrence
            position = result - str; //getting the pos of the occurrence on the substring
    }

    return position;
}

/**
 *  Given a str it returns the substring 
 * @param dest
 * @param src
 * @param from
 * @param to
 */
void str_sub_string(char *dest, char *src, int from, int to) {

    //Variable declarations
    int i = 0, j = 0;

    //Copies the src string from a specific to a specific value, if character reading is not a \n or \r
    for (i = from; i < to; i++) {
        if (src[i] != '\n' || src[i] != '\r') {
            dest[j] = src[i];
            j++;
        }
    }
}

/**
 * returns emailDB/name of email if . txt
 * @param dest
 * @param email
 */
void get_email_store_path(char *dest, char *email) {

    //Variable declarations
    char path[MAX_PATH];

    //Concatenating the str
    strcpy(path, EM_STORE_PATH);
    strcat(path, email);
    strcat(path, FILE_FORMAT);

    strcpy(dest, path);
}

/**
 * returns a str dest with no white space at first and last position
 * @param src_dest
 * @param src
 */
void str_trim(char *dest, char *str) {

    char *end;
    int white_space = ' ';

    //detecting white space , and moving forward
    while (*str == white_space)
        str++;

    end = str + strlen(str) - 1;
    while (end > str && *end == white_space)
        end--;

    // Write new null terminator
    *(end + 1) = EMPTY;

    strcpy(dest, str);
}

/**
 * This function sets , where a \n is, to a \0.
 * @param str
 */
void str_remove_new_line(char *str) {

    str = strchr(str, '\n');
    if (str != NULL)
        *str = EMPTY;
}

/**
 * return true if a email contains caracter else fale
 * @param email
 * @param caracter
 * @return 
 */
int sub_email(Email *email, char *caracter) {

    //Variable declaration
    int pos_id = -1, pos_body = -1, pos_date = -1;
    int pos_from = -1, pos_to = -1, pos_sub = -1;

    if (email == NULL && caracter == NULL)
        return FAIL;

    //Finding a match in email fields
    pos_id = index_of(email->id, caracter);
    pos_body = index_of(email->body, caracter);
    pos_date = index_of(email->date, caracter);
    pos_from = index_of(email->from, caracter);
    pos_to = index_of(email->to, caracter);
    pos_sub = index_of(email->subject, caracter);

    //returning succes if matched in any field of email
    if (pos_id > 0 || pos_body > 0 || pos_date > 0 || pos_from > 0 || pos_to > 0 || pos_sub > 0)
        return TRUE;

    return FALSE;

}