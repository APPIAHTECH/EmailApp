/* 
 * File:Database.c   
 * Authors: Stephen Appiah Frimpong NIA: 206637 , Manuel Aneiros Otero NIA: 205351 , Hugo Hernández Quimbay NIA: 206662
 * DATE: 11/02/2018
 * Version : 1.0
 *
 * 
 * This file describes the database methods related to the database header
 * The general purpose of the file if to serve the different functionality of the database, 
 * such as init the database structure ,add folder , etc.
 * 
 */


#include "imports.h"
#include "database.h"
#include "utils.h"
#include "folder.h"
#include "file_operations.h"
/**
 * Initializes an email database.
 * @param db
 * @param initial_msg_id
 */
void init_database(Database *db, int initial_msg_id) {

    //Variable declarations
    int i = 0;

    //initialization of fields of database by default values
    db->email_count = UNDEFINED;
    db->folder_count = UNDEFINED;
    db->msg_id_seed = initial_msg_id;

    //initialization every email 
    for (i = 0; i < MAX_EMAILS; i++)
        init_email(&db->emails[i]);

    //folder initialization and emails
    for (i = 0; i < MAX_FOLDERS; i++)
        init_folder(&db->folders[i]);

}

/**
 * Returns the total number of unique emails stored into the database.
 * @param db
 * @return 
 */
int get_database_email_count(Database* db) {
    return db->email_count;
}

/**
 * Fills the parameter emails with the pointers to the emails contained into
 * the database and returns the email count.
 * @param db
 * @param emails
 * @return 
 */
int get_database_emails(Database* db, Email* emails[]) {

    //Variables declarations
    int email_quantity = 0, i;

    if (db != NULL && emails != NULL) {

        //For each email in db we fills the parameter emails with the pointers to the emails
        for (i = 0; i < MAX_FOLDERS; i++)
            emails[i] = &db->emails[i];

        //Gets the email counts of the db 
        email_quantity = get_database_email_count(db);
    }

    return email_quantity;
}

/**
 * Returns the reference to the email matching the target_id into the database 
 * or NULL, if not found.
 * @param db
 * @param target_id
 * @return 
 */
Email* search_database_email_id(Database* db, char* target_id) {

    //Variable declarations
    int i = 0;

    //For each email in db , if the target email is found we return a referenced to it 
    if (target_id != NULL && db != NULL) {
        for (i = 0; i < MAX_EMAILS; i++) {
            if (strcmp(db->emails[i].id, target_id) == 0) //if db email is equals to target_id
                return &db->emails[i];
        }
    }
    return NULL;
}

/**
 * creates a new unique message id and writes it to buf parameter.
 * @param db
 * @param buf
 */
void get_new_message_id(Database* db, char buf[]) {

    //Variable declarations
    int last_id_gen;
    char temp[MAX_BUF], str_num[MAX_BUF];

    if (buf != NULL) {
        //Getting current id , last generated
        last_id_gen = db->msg_id_seed;
        last_id_gen++;

        sprintf(str_num, NUMBER, last_id_gen); //cast int value to string
        strcpy(temp, str_num); // concatenating id 
        strcat(temp, EMAIL_INIT_ID); // concatenating _EDA_email
        strcpy(buf, temp); //storing result to buff
    }
}

/**
 * It COPIES the email into the database. This means that the email pointer 
 * parameter is not pointing to the actual email stored into the database.
 * It returns the pointer to the database stored email.
 * @param db
 * @param src_email
 * @return 
 */
Email* add_email_to_database(Database* db, Email* src_email) {

    //Variable declarations
    int i;

    if (db != NULL && src_email != NULL) {

        //Searching for position to store email data
        for (i = 0; i < MAX_EMAILS; i++) {

            //if position of i is a empty email space then we add the email to that position of the db emails
            if (is_email_empty(&db->emails[i])) {
                db->email_count++;
                src_email->empty = FALSE;
                copy_email(&db->emails[i], src_email);
                return &db->emails[i];
            }
        }

    }

    return NULL;
}

/**
 * this function deletes the email from the database
 * also delets the email  file
 * @param db
 * @param email
 * @return 
 */
int delete_database_email(Database* db, Email* email) {

    if (db == NULL && email == NULL) return FAIL;

    if (delete_email_file(email) == SUCCESS) { //if the function deletes the email file
        //We set the email to a null state 
        //this  means that the position where that email was is now avilable for a new email
        init_email(email);
        //Decrementing the email_count cos we delete one email
        db->email_count--;
    } else
        return FAIL;

    return SUCCESS;
}

/**
 * Returns the database folder matching the target name or NULL, if not found.
 * @param db
 * @param target_name
 * @return 
 */
Folder* get_database_folder(Database* db, char* target_name) {

    //Variables declarations
    int i = 0, result;
    char *folder_name;

    //Searching for target name on db
    if (target_name != NULL && db != NULL) { //if the target_name as a value on it 
        //comparing each folder in db with target name , to found a match 
        for (i = 0; i < MAX_FOLDERS; i++) {
            folder_name = get_folder_name(&db->folders[i]);
            result = strcmp(folder_name, target_name);
            if (result == 0) //if there is a match it returns the reference to the target folder
                return &db->folders[i];
        }
    }

    return NULL;
}

/**
 * Returns the database folder count.
 * @param db
 * @return 
 */
int get_database_folder_count(Database* db) {
    return db->folder_count;
}

/**
 * Fills the parameter folders with the pointers to the fodlers contained into
 * the database and returns the folder count.
 * @param db
 * @param folders
 * @return 
 */
int get_database_folders(Database* db, Folder* folders[]) {

    //Variables declarations
    int folder_quantity = 0, i;

    if (db != NULL && folders != NULL) {
        //For each folder in db 
        //filing the parameter folders with the pointers to the folders contained into
        for (i = 0; i < MAX_FOLDERS; i++)
            folders[i] = &db->folders[i];


        //Getting the folders counts
        folder_quantity = get_database_folder_count(db);
    }

    return folder_quantity;
}

/**
 * Creates a folder into the database with the provided folder name.
 * @param db
 * @param folder_name
 * @return 
 */
Folder* create_database_folder(Database* db, char* folder_name) {

    //Variables declarations
    Folder folder;

    if (db != NULL, folder_name != NULL) {
        //Setting up folder information
        init_folder(&folder);
        //init_folder_emails(&folder, db);
        strcpy(folder.folder_name, folder_name);
        //adding to db
        add_folder_to_database(db, &folder);
    }

    return NULL;

}

/**
 * Adds the given folder to the database and returns success if added else fail 
 * @param db
 * @param src_folder
 * @return 
 */
int add_folder_to_database(Database* db, Folder* src_folder) {

    //Variable declarations
    int i;

    if (db != NULL && src_folder != NULL) {
        //Searching for position to store email data
        for (i = 0; i < MAX_FOLDERS; i++) {

            //if a empty position is found then we the folder at that position 
            if (is_folder_empty(&db->folders[i]) == TRUE) {
                copy_folder(&db->folders[i], src_folder);
                db->folder_count++;
                db->folders[i].empty = FALSE;
                return SUCCESS;
            }
        }
    }


    return FAIL;
}

/**
 * Deletes a folder from the database.
 * @param db
 * @param folder
 * @return 
 */
int delete_database_folder(Database* db, Folder* folder) {

    if (db == NULL && folder == NULL) return FAIL;

    //We set the folder to a null state 
    //this  means that the position where that folder was is now avilable for a new folder
    init_folder(folder);
    return SUCCESS;
}

/**
 *  Sets db id to a new an unique id 
 * @param db
 */
void get_new_unic_id(Database* db) {
    db->msg_id_seed++;
}