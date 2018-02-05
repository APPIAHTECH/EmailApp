/* 
 * File:  
 * Author: 
 *
 * Fill Header!!
 */


#include "imports.h"
#include "database.h"
#include "utils.h"
#include "folder.h"

/**
 * Initializes an email database.
 * @param db
 * @param initial_msg_id
 */
void init_database(Database *db, int initial_msg_id) {

    //Variable declarations
    int i;

    //Initialization of fields of Database by default values
    db->email_count = UNDEFINED;
    db->folder_count = UNDEFINED;
    db->msg_id_seed = initial_msg_id;

    //Initialization every email 
    for (i = 0; i < MAX_EMAILS; i++)
        init_email(&db->emails[i]);

    //folder Initialization and emails
    for (i = 0; i < MAX_FOLDERS; i++) {
        init_folder(&db->folders[i]);
        init_folder_emails(&db->folders[i], db);
    }


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

    //For each email in db 
    for (i = 0; i < MAX_FOLDERS; i++) {
        emails[i] = &db->emails[i];
        email_quantity = i;
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

    //For each email in db
    for (i = 0; i < MAX_EMAILS; i++) {
        if (strcmp(db->emails[i].id, target_id) == 0) //if db email is equals to target_id
            return &db->emails[i];
    }
    return NULL;
}

/**
 * Produces a new unique message id and writes it to buf parameter.
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

        sprintf(str_num, "%d", last_id_gen); //cast int value to string
        strcpy(temp, str_num); // adding id 
        strcat(temp, EMAIL_INIT_ID); // adding _EDA_email
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

    //Searching for position to store email data
    for (i = 0; i < MAX_EMAILS; i++) {

        if (is_email_empty(&db->emails[i])) {
            db->email_count++;
            src_email->referenced++;
            copy_email(&db->emails[i], src_email);
            return &db->emails[i];
        }
    }

    return NULL;
}

int delete_database_email(Database* db, Email* email) {

    //TODO!!!
    //Variable declarations
    int i;

    //Searching for position to be delete email data
    for (i = 0; i < MAX_EMAILS; i++) {


    }

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
    if (target_name != NULL) { //if the target_name as a value on it 
        //comparing each folder in db with target name , to found a match 
        for (i = 0; i < MAX_FOLDERS; i++) {
            folder_name = get_folder_name(&db->folders[i]);
            result = strcmp(folder_name, target_name);
            if (result == 0)
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

    //For each folder in db 
    //filing the parameter folders with the pointers to the fodlers contained into
    for (i = 0; i < MAX_FOLDERS; i++) {
        folders[i] = &db->folders[i];
        folder_quantity = i;
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

    //Setting up folder information
    init_folder(&folder);
    init_folder_emails(&folder, db);
    strcpy(folder.folder_name, folder_name);

    //adding to db
    add_folder_to_database(db, &folder);

}

/**
 * 
 * @param db
 * @param src_folder
 * @return 
 */
int add_folder_to_database(Database* db, Folder* src_folder) {

    //Variable declarations
    int i;

    //Searching for position to store email data
    for (i = 0; i < MAX_FOLDERS; i++) {

        if (is_folder_empty(&db->folders[i])) {
            copy_folder(&db->folders[i], src_folder);
            db->folder_count++;
            return SUCCESS;
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

}

/**
 *  Setts db id to a new an unic id 
 * @param db
 */
void get_new_unic_id(Database* db) {
    db->msg_id_seed++;
}