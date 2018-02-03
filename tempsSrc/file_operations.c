/* 
 * File:  
 * Author: 
 *
 * Fill Header!!
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

    configuraionFile = fopen(EM_CONFIG_PATH, FILE_READ_MODE); //Getting the EMconfig.txt in  read mode

    if (configuraionFile == NULL) { //If any error log error msg
        display_error(ERROR_MSG_FILE_NOT_FOUDN);
        return FALSE;
    } else {
        last_message_id = get_message_id(configuraionFile);
        if (last_message_id != NOT_FOUNDED) { //if get_message_id found an valid ID
            db->msg_id_seed = last_message_id; //setting msg_id_seed to ID geted
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
    int i, result, j, k;
    ;
    char format[MAX_BUF], str_num[MAX_BUF];

    config_file = fopen(EM_CONFIG_PATH, FILE_WRITE_MODE); //Getting the EMconfig.txt in  write mode

    if (config_file == NULL) { //If any error log error msg
        display_error(ERROR_MSG_FILE_NOT_FOUDN);
        return FALSE;
    } else {

        //Section Message-ID: n  Format 
        strcpy(format, "Message-ID: ");
        sprintf(str_num, "%d", db->msg_id_seed); //cast int value to string
        strcat(format, str_num);
        strcat(format, "\r\n");
        strcat(format, "\r\n");

        result = fputs(format, config_file);

        //Section Folders:
        strcpy(format, "Folders:\r\n");
        result = fputs(format, config_file);
        get_database_folders(db, folder);
        for (i = 0; i < MAX_FOLDERS; i++) {
            strcpy(format, folder[i]->folder_name);
            strcat(format, "\r\n");
            result = fputs(format, config_file);
        }
        strcpy(format, "\r\n");
        result = fputs(format, config_file);

        //Section Folders Messages
        for (i = 0; i < MAX_FOLDERS; i++) {
            strcpy(format, folder[i]->folder_name);
            strcat(format, " Messages:\r\n");
            result = fputs(format, config_file);


            for (j = 0; j < MAX_FOLDER_EMAILS; j++) {
                if (is_email_empty(folder[i]->emails[j]) != TRUE) { //Getting only position that as an email
                    strcpy(format, folder[i]->emails[j]->id);
                    strcat(format, "\r\n");
                    result = fputs(format, config_file);
                }
            }
            strcpy(format, "\r\n");
            result = fputs(format, config_file);
        }

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

    //Creating path format
    strcpy(path, EM_STORE_PATH);
    strcat(path, email->id);
    strcat(path, FILE_FORMAT);

    //Opennig file on write mode
    email_file = fopen(path, FILE_WRITE_MODE);

    //if any error return false , if not writes the email content to a file and store it at path specify
    if (email_file == NULL) {
        return FAIL;
    } else {
        result = write_email_to_file(email_file, email); //writing content
        if (result == FAIL) {
            fclose(email_file); //end uup email_file proces
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
    int id, matched, i, pos, result;
    Folder folder;

    i = 0;
    init_folder(&folder);
    init_folder_emails(&folder, db);

    //Reading file line by line , getting folder names
    fgets(buff, MAX_BUF, config_file);
    while (!feof(config_file)) {
        fgets(buff, MAX_BUF, config_file);

        if (strcmp(buff, NEW_LINE) == 0)
            break;
        else {
            str_remove_trash(buff); //return str with a \0 where \r is

            //Getting the pos of :, if it return pos that means the line reading is a type something:
            pos = index_of(buff, FORMAT_FIELD_HEADERS);

            if (pos > 0) //if he gets the first occurrence , continue the next iteration to get netx line
                continue;
            matched = sscanf(buff, FORMAT_SPECIAL_BEAM_CANON, folder_name);
            if (matched > 0) { // if a match according to format

                //Setting up and Storing folder to DB
                strcpy(folder.folder_name, folder_name);
                if (strcmp(folder_name, PROTECT_OUTBOX) == 0 || strcmp(folder_name, PROTECT_INBOX) == 0) // set to protected to true , folder inbox,outbox
                    folder.protected = TRUE;

                folder.empty = FALSE;
                folder.size = strlen(folder_name);
                result = add_folder_to_database(db, &folder);
                if (result == FAIL)
                    return FAIL;
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
    int id, matched, i = 0, pos = 0, j = 0;
    Email email, *temp_email;

    init_email(&email);
    i = 0; // i represent curent folder
    j = 0;
    //Reading file line by line 
    fgets(buff, MAX_BUF, config_file);
    while (!feof(config_file)) {

        fgets(buff, MAX_BUF, config_file);
        if (strcmp(buff, NEW_LINE) == 0) //if a new line increment i , meaning there is a new eamil secction to read
        {
            i++; //to store next email secction
            j = 0; //represents emails to be read and store
        }

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
                email.referenced = UNDEFINED;
                email.empty = FALSE;
                temp_email = add_email_to_database(db, &email);
                if (temp_email == NULL)
                    return FAIL;
                else {
                    db->folders[i].emails[j] = temp_email; //adding email to folder
                    db->folders[i].empty = FALSE;
                    j++;
                }

            }
        }
    }

    return SUCCESS;
}