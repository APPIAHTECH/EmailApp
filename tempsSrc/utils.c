/* 
 * File:  
 * Author: 
 *
 * Fill Header!!
 */


#include "imports.h"

#include "utils.h"
#include "database.h"

void flush_input() {
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
 * prints emails fields
 * @param email
 */
void print_email(Email *email) {

    printf("Date %s \n", email->date);
    printf("ID %s \n", email->id);
    printf("De %s \n", email->from);
    printf("Para %s \n", email->to);
    printf("Asunto %s \n", email->subject);
    printf("Menssage %s \n", email->body);
    printf("Empty %d \n", email->empty);
    printf("Referenced %d \n", email->referenced);
}

/**
 * prints folder fields
 * @param folder
 */
void print_folder(Folder *folder) {

    int i;
    printf("Folder name : %s \n", folder->folder_name);
    printf("Empty %d \n", folder->empty);
    printf("Size %d \n", folder->size);
    printf("Protected %d \n", folder->protected);
    printf("------------------------------\n");
    printf("Folder Emails Llist\n\n");

    for (i = 0; i < MAX_FOLDER_EMAILS; i++) {
        print_email(folder->emails[i]);
        printf("------------------------------\n");
    }

}

/**
 * Prints all db information
 * @param db
 */
void print_database(Database *db) {

    int i = 0, j = 0;

    puts("Database");
    puts("------------------------------");
    printf("Message ID %d \n", db->msg_id_seed);
    printf("Email count %d \n", db->email_count);
    printf("Folder count %d \n", db->folder_count);
    puts("------------------------------");
    puts("Emails Llist");

    for (i = 0; i < MAX_EMAILS; i++)
        print_email(&db->emails[i]);

    puts("------------------------------");
    puts("Folders Llist");
    for (i = 0; i < MAX_FOLDERS; i++) {
        printf("Folder %d \n", i);
        print_folder(&db->folders[i]);
    }

}

/**
 * 
 * @return 
 */
char *get_curent_date() {
    time_t orig_format;
    time(&orig_format);
    return asctime(gmtime(&orig_format));
}

/**
 * 
 * @param buff
 */
void str_remove_trash(char *buff) {

    buff = strchr(buff, '\r');
    if (buff != NULL)
        *buff = '\0';
}

/**
 * returns the postion of the first occurrence with the second str
 * @param str
 * @param searched
 * @return 
 */
int index_of(char * str, char * searched) {
    int position = -1;
    char * result = NULL;

    if (str && searched) {
        result = strstr(str, searched);
        if (result) {
            position = result - str;
        }
    }

    return position;
}