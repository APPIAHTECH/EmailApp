/* 
 * File:Database.c   
 * Author:Stephen Appiah
 * DATE: 09/01/2018
 * Version : 1.0
 *
 * 
 * This file describes the file operations methods related to the file operations  header
 * The general purpose of the file if to serve the different functionality of the file operations.
 * 
 */

#include "imports.h"

#include "file_operations.h"
#include "common.h"
#include "utils.h"

/**
 * Opens the file located in config_path, reads it and loads the database accordingly.
 * @param db
 * @return 
 */
int read_config(Database *db) {

    //Variables declarations
    FILE *configuraionFile = NULL;
    int last_message_id = 0, result_get_folder, result_get_message;

    if (db == NULL)
        return FAIL;

    configuraionFile = fopen(EM_CONFIG_PATH, FILE_READ_MODE); //Getting the EMconfig.txt in  read mode

    if (configuraionFile == NULL) { //If any error log error msg
        display_error(ERROR_MSG_FILE_NOT_FOUDN);
        return FALSE;
    } else {

        last_message_id = get_message_id(configuraionFile);

        if (last_message_id != NOT_FOUNDED) { //if get_message_id found an valid ID

            db->msg_id_seed = last_message_id; //Setting msg_id_seed to ID geted
            result_get_folder = get_folders(configuraionFile, db); //Getting folders from config file and storing to db
            result_get_message = get_message(configuraionFile, db); //Getting messages from config file and storing to db

            if (result_get_folder == SUCCESS && result_get_message == SUCCESS) //if could read and store file info
                return TRUE;
        }

        fclose(configuraionFile); //End up the file proces
    }

    return FALSE;
}

/**
 * Writes the current state of the database into the configuration file located in config_path.
 * @param db
 * @return 
 */
int write_config(Database *db) {

    //Variables declarations
    FILE *config_file = NULL;
    Folder * folder[MAX_FOLDERS];
    Email email;
    int result;
    char format[MAX_BUF];

    if (db == NULL)
        return FAIL;

    config_file = fopen(EM_CONFIG_PATH, FILE_WRITE_MODE); //Getting the EMconfig.txt in  write mode

    if (config_file == NULL) { //If any error log error msg
        display_error(ERROR_MSG_FILE_NOT_FOUDN);
        return FALSE;
    } else {

        //Section Message-ID: n  Format 
        set_config_section_messageID(db, format);
        result = fputs(format, config_file);

        //Section Folders:
        strcpy(format, FILE_FORMAT_CONFIG_STRUC_FOLDERS);
        result = fputs(format, config_file);
        get_database_folders(db, folder);
        result = set_config_section_Folder(db, folder, config_file, format);
        strcpy(format, NEW_LINE);
        result = fputs(format, config_file);

        //Section Folders Messages
        set_config_section_Folder_MESSAGES(folder, config_file, format);

        //section end
        strcpy(format, FILE_END);

        result = fputs(format, config_file);
        if (result == EOF) //EOF f there was an error while writing to the file
        {
            fclose(config_file);
            return FAIL;
        }

        fclose(config_file); //End up the file proces
    }

    return SUCCESS;
}

/**
 * Writes the email content into the email database directory.
 * @param email
 * @return 
 */
int store_email(Email* email) {

    //Variables declarations
    FILE *email_file = NULL;
    char path[MAX_PATH];
    char id;
    int result = 0;

    if (email == NULL)
        return FAIL;

    //Creating path format
    get_email_store_path(path, email->id);
    //Opennig file on write mode
    email_file = fopen(path, FILE_WRITE_MODE);

    //if any error return false , if not writes the email content to a file and store it at path specify
    if (email_file == NULL) {
        return FAIL;
    } else {
        result = write_email_to_file(email_file, email); //writing content
        if (result == FAIL) {
            fclose(email_file); //end up email_file proces
            return FAIL;
        }
    }
    fclose(email_file); //end up email_file proces
    return SUCCESS;
}

/**
 *  Gets and sets folders to de DB
 * @param config_file
 * @param db
 */
int get_folders(FILE *config_file, Database *db) {

    //Variable declarations
    char buff[MAX_BUF]; //Use to store file string line (folder name)
    char folder_name[MAX_BUF]; //use to store folder value
    int matched, i, pos;

    if (config_file == NULL || db == NULL)
        return FAIL;

    i = 0;
    //Reading file line by line , getting folder names
    fgets(buff, MAX_BUF, config_file);
    while (!feof(config_file)) {
        fgets(buff, MAX_BUF, config_file);

        if (strcmp(buff, NEW_LINE) == 0)
            break;
        else {
            str_remove_trash(buff); //return str with a \0 where \r is

            //Getting the pos of :, if it return pos that means the line reading is a type (HEADER_NAME:)
            pos = index_of(buff, FORMAT_FIELD_HEADERS);
            if (pos > 0) //if he gets the first occurrence with a header_name: format , continue the next iteration to get netx line
                continue;

            matched = sscanf(buff, FORMAT_SPECIAL_BEAM_CANON, folder_name);
            if (matched > 0) { // if a match according to format

                //Setting up and Storing folder to DB
                strcpy(db->folders[i].folder_name, folder_name);
                if (strcmp(folder_name, PROTECT_OUTBOX) == 0 || strcmp(folder_name, PROTECT_INBOX) == 0) // set to protected to true , folder inbox,outbox
                    db->folders[i].protected = TRUE;

                db->folders[i].size = strlen(folder_name);
                db->folders[i] = db->folders[i];
                db->folder_count++;
                i++;
            }
        }
    }

    return SUCCESS;
}

/**
 *  Get the field Message-ID: id
 * @param config_file
 * @return 
 */
int get_message_id(FILE *config_file) {

    //Variable declarations
    char buff[MAX_BUF]; //Use to store file string line
    char message_text[MAX_BUF]; //user to store Message-ID: n
    int id, matched;

    if (config_file == NULL)
        return FAIL;

    fgets(buff, MAX_BUF, config_file); //Getting first line of the config file
    str_remove_trash(buff); //return str with a \0 where \r is

    matched = sscanf(buff, FORMAT_MESSAGE_ID, message_text, &id); //getting value of Message-ID field
    if (matched > 0) //Cheking if any match according to the format Message-ID: n , return the unic id else -1 meaning no valid id found
        return id;
    else
        return NOT_FOUNDED;
}

/**
 *  Gets all emails and store it
 * @param config_file
 * @param db
 */
int get_message(FILE *config_file, Database *db) {

    //Variable declarations
    char buff[MAX_BUF];
    char email_name[MAX_BUF]; //use to store folder value
    int matched, i = 0, pos = 0, j = 0;
    Email email, *temp_email;

    if (config_file == NULL || db == NULL)
        return FAIL;

    init_email(&email);
    i = 0; // i represent curent folder
    j = 0; // j represent curent folder emails

    //Reading file line by line 
    fgets(buff, MAX_BUF, config_file);
    while (!feof(config_file)) {

        fgets(buff, MAX_BUF, config_file);
        if (strcmp(buff, NEW_LINE) == 0) //if a new line increment i , meaning there is a new eamil section to read
        {
            i++; //to store next email sectoion
            j = 0; //setting j = 0 coz there is a new email section coming
        } else {
            str_remove_trash(buff); //return str with a \0 where \r is
            if (strcmp(buff, FILE_END) == 0)
                break;

            //Getting the pos of :, if it return pos that means the line reading is a type something:
            pos = index_of(buff, FORMAT_FIELD_HEADERS);
            if (pos > 0) //if he gets the first occurrence , continue the next iteration to get netx line
                continue;
            else {
                matched = sscanf(buff, FORMAT_SPECIAL_BEAM_CANON, email_name);
                if (matched > 0) {

                    //Setting email info
                    strcpy(email.id, email_name);

                    //Searching if allready there is a message id the db
                    temp_email = search_database_email_id(db, email.id);
                    if (temp_email == NULL) { //if the current message was not found in db we add it to the db

                        if (get_messages_info(&email) != FAIL) {
                            temp_email = add_email_to_database(db, &email); //adding email to the db and returning a refrence to it
                            if (temp_email != NULL) {
                                db->folders[i].emails[j] = temp_email; //adding email to folder
                                temp_email->referenced++;
                                db->folders[i].empty = FALSE;
                                j++;
                            } else
                                return FAIL;
                        } else
                            return FAIL;

                    } else {
                        //if cant add to db it means the email is already in db , so we get the reference from the db
                        temp_email = search_database_email_id(db, email.id);
                        db->folders[i].emails[j] = temp_email;
                        temp_email->referenced++;
                        db->folders[i].empty = FALSE;
                        temp_email = NULL;
                    }

                }
            }
        }
    }

    return SUCCESS;
}

/**
 * 
 * @param email
 * @return 
 */
int get_messages_info(Email *email) {

    //Variables declarations
    FILE *file = NULL;
    char path[MAX_BUF];
    char *message_id;


    if (email == NULL)
        return FAIL;

    //setting the file path
    message_id = get_email_id(email);
    get_email_store_path(path, message_id);
    file = fopen(path, FILE_READ_MODE);

    if (file == NULL) { //If any error log error msg
        log_error(stdout, ERROR_MSG_FILE_NOT_FOUDN);
    } else {

        if (load_email_from_file(file, email) != FAIL) //reading email file and saveing it on email structure
        {
            fclose(file);
            return SUCCESS;
        }

    }
    fclose(file);
    return FAIL;
}

void set_config_section_messageID(Database *db, char *format) {

    //Variable declarations
    char str_num[MAX_BUF];

    strcpy(format, FILE_FORMAT_CONFIG_STRUC_MESSAGES);
    sprintf(str_num, "%d", db->msg_id_seed); //cast int value to string
    strcat(format, str_num);
    strcat(format, NEW_LINE);
    strcat(format, NEW_LINE);
}

int set_config_section_Folder(Database *db, Folder *folder[MAX_FOLDERS], FILE *config_file, char *format) {
    //Variable declarations
    int i = 0, result;

    for (i = 0; i < MAX_FOLDERS; i++) {
        if (strcmp(folder[i]->folder_name, FOLDER_INIT_NAME) != 0) {
            strcpy(format, folder[i]->folder_name);
            strcat(format, NEW_LINE);
            result = fputs(format, config_file);
        }

    }

    return result;
}

int set_config_section_Folder_MESSAGES(Folder *folder[MAX_FOLDERS], FILE *config_file, char *format) {

    //Variable declarations
    int i = 0, j = 0, result;

    for (i = 0; i < MAX_FOLDERS; i++) {
        if (strcmp(folder[i]->folder_name, FOLDER_INIT_NAME) != 0) {
            strcpy(format, folder[i]->folder_name);
            strcat(format, FILE_FORMAT_CONFIG_STRUC_MESSAGES_FOLDER_SECTION);
            result = fputs(format, config_file);

            for (j = 0; j < MAX_FOLDER_EMAILS; j++) {
                if (folder[i]->emails[j] != NULL) { //Getting only position that as an email
                    strcpy(format, folder[i]->emails[j]->id);
                    strcat(format, NEW_LINE);
                    result = fputs(format, config_file);
                }
            }
            strcpy(format, NEW_LINE);
            result = fputs(format, config_file);
        }
    }

    return result;
}