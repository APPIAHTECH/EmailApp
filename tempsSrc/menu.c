/* 
 * File:  
 * Author: 
 *
 * Fill Header!!
 */

#include "imports.h"

#include "menu.h"
#include "database.h"
#include "utils.h"
#include "email.h"
#include "folder.h"
#include "file_operations.h"

/**
 * 
 * @param msg
 * @return 
 */
int read_option(const char* msg) {
    int option = OPTION_INVALID;
    printf("%s", msg);
    scanf("%d", &option);

    flush_input();

    return option;
}

/**
 * 
 * @return 
 */
int query_option() {
    printf("\n");
    printf(MSG_LIST_EMAILS, OPTION_LIST_EMAILS);
    printf(MSG_SHOW_EMAIL, OPTION_SHOW_EMAIL);
    printf(MSG_CREATE_EMAIL, OPTION_CREATE_EMAIL);
    printf(MSG_DELETE_EMAIL, OPTION_DELETE_EMAIL);
    printf(MSG_SHOW_FOLDER, OPTION_SHOW_FOLDER);
    printf(MSG_CREATE_FOLDER, OPTION_CREATE_FOLDER);
    printf(MSG_DELETE_FOLDER, OPTION_DELETE_FOLDER);
    printf(MSG_ADD_EMAIL, OPTION_ADD_EMAIL);
    printf(MSG_REMOVE_EMAIL, OPTION_REMOVE_EMAIL);
    printf(MSG_SEARCH, OPTION_SEARCH);
    printf(MSG_QUIT, OPTION_QUIT);
    printf("\n");

    return read_option("Choose an option: ");
}

/**
 * This function prints all existing emails from the db
 * @param db
 */
void list_emails_option(Database* db) {

    //Variables declarations
    int i = 0, j = 0;

    log_info(stdout, "You have selected: List emails option\n");

    //TODO : EMAIL BODY IF LENGHT IS LES THAN 200 CARACTERS
    //Printing all existing emails
    puts("All Emails :");
    puts("------------------------------");
    for (i = 0; i < MAX_EMAILS; i++) {
        if (is_email_empty(&db->emails[i]) != TRUE) { // if positon reading is empty we dont print else that means there is a email we can print
            printf("Email : %d \n", i + 1);
            print_email(&db->emails[i]);
            puts("------------------------------");
        }
    }
}

/**
 * 
 * @param db
 */
void show_email_option(Database* db) {

    //Variables declarations
    int i = 0;
    char mail_to_show[MAX_BUF], path[MAX_PATH];
    FILE *email_file = NULL;
    Email *email;

    log_info(stdout, "You have selected: Show email option\n");

    //User input value
    puts("Enter the name of the email you wants : ");
    scanf("%s", mail_to_show);

    //Searching for user email
    email = search_database_email_id(db, mail_to_show);
    if (email != NULL) { //if user email geved is on db then we show it

        //creating a valida path to get the file
        strcpy(path, EM_STORE_PATH);
        strcat(path, mail_to_show);
        strcat(path, FILE_FORMAT);

        email_file = fopen(path, FILE_READ_MODE); //Getting the email_file in  read mode

        if (email_file == NULL) { //If any error log error msg
            log_error(stdout, ERROR_MSG_FILE_NOT_FOUDN);
        } else {
            load_email_from_file(email_file, email); //reading email frile and print
            fclose(email_file); //End up the file proces
        }

    } else
        log_info(stdout, EMAIL_INFO_NOT_IN_DB);

}

/**
 * 
 * @param db
 */
void create_email_option(Database* db) {

    //Variables declarations
    Email email;
    Folder *folder = NULL;
    char new_id[MAX_BUF], valid, succes;

    log_info(stdout, "You have selected: Create email option\n");

    init_email(&email); //setting up a default setment to email
    get_new_message_id(db, new_id); //generating a new id for the message
    get_new_unic_id(db); //generaing a unic id for the next email to creat
    strcpy(email.id, new_id); // setting the new id to email
    read_email_interactive(&email); //getting user information to setup email structure

    valid = validate_email(&email);

    //TODO , ARREGLAR NO PETA ,PERO AÑADE EL EMAIL EN TEST TAMBIEN WTF!!!!!!!!
    if (valid) {
        folder = get_database_folder(db, PROTECT_OUTBOX); //getting Outbox folder such that we can add the email to this folder

        if (add_email_to_database(db, &email) == FALSE) {//adding email to db
            log_error(stdout, ERROR_EMAIL_CANT_ADD_DB);
            log_warn(stdout, ERROR_FOLDER_CANT_ADD_FIX);
        } else {

            if (add_email_to_folder(folder, &email)) {
                log_error(stdout, ERROR_EMAIL_CANT_ADD_FOLDER);
                log_warn(stdout, ERROR_EMAIL_CANT_ADD_FOLDER_FIX);
            } else {
                succes = store_email(&email); //Finaly writing the email to the emailDB
                if (succes)
                    log_info(stdout, MESSAGE_EMAIL_CREATED);
                else
                    log_error(stdout, ERROR_SAVING_FILE);
            }
        }

    } else
        log_error(stdout, ERROR_MAIL_INVALID);

}

/**
 * 
 * @param db
 */
void delete_email_option(Database* db) {
    log_info(stdout, "You have selected: Delete email option\n");
}

/**
 * 
 * @param db
 */
void show_folder_option(Database* db) {
    //Variables declarations
    int i, j, total_email, total_folder;

    log_info(stdout, "You have selected: Show folder option\n");
    puts("Folder Emails :");

    total_folder = get_database_folder_count(db);
    total_email = get_database_email_count(db);

    printf("Total Folders :%d\n", total_folder);
    printf("Total Emails :%d \n", total_email);
    puts("------------------------------");

    for (i = 0; i < MAX_FOLDERS; i++) {

        if (is_folder_empty(&db->folders[i]) != TRUE) { //if folder is empty we dont print it
            printf("Folder [%s] : %d \n", db->folders[i].folder_name, i + 1);
            for (j = 0; j < MAX_FOLDER_EMAILS; j++) {

                if (is_email_empty(db->folders[i].emails[j]) != TRUE) { //if email is empty we dont print it
                    print_email(db->folders[i].emails[j]);
                    printf("------------------------------\n");
                }

            }
        }

    }
}

/**
 * 
 * @param db
 */
void create_folder_option(Database* db) {

    //Variables declarations
    Folder folder, *tmep = NULL;
    int result;

    log_info(stdout, "You have selected: Create folder option\n");
    read_folder_interactive(&folder); //getting user input for folder setting

    //searching for folder that user specify, if match it means there is already a foldername with that name
    tmep = get_database_folder(db, folder.folder_name);
    if (tmep != NULL) //if allready is a folder with the same name has the user enter
    {
        log_info(stdout, FOLDER_INFO_ALREADY_IN);
    } else {
        result = add_folder_to_database(db, &folder); //adding folder to db
        //cheking if could o no to add folder to db 
        if (result == FAIL) {
            log_error(stdout, ERROR_FOLDER_CANT_ADD);
            log_warn(stdout, ERROR_FOLDER_CANT_ADD_FIX);
        } else {
            log_info(stdout, MESSAGE_FOLDER_CREATED);
        }


    }

}

/**
 * 
 * @param db
 */
void delete_folder_option(Database* db) {
    log_info(stdout, "You have selected: Delete folder option\n");
}

/**
 * 
 * @param db
 */
void add_email_to_folder_option(Database* db) {

    //Variables declarations
    char mail_to_add[MAX_BUF], folder_to_store[MAX_BUF];
    Email *email;
    Folder *folder;

    log_info(stdout, "You have selected: Add email to folder option\n");

    //User input value
    puts("Enter the name of the email you wants add to the folder: ");
    scanf("%s", mail_to_add);

    puts("Enter the name of the folder you wants the email to be add: ");
    scanf("%s", folder_to_store);

    //Searching for user email
    email = search_database_email_id(db, mail_to_add);
    folder = get_database_folder(db, folder_to_store);
    if (email != NULL) { //if there exist the email 

        if (folder != NULL) {//if there exist the folder 

            //Finding the email on the forder , if is already in that folder
            //NOTE preguntar si los emails puden ser dublicados , en un mismo folder
            add_email_to_database(db, email);
            add_email_to_folder(folder, email);
            log_info(stdin, MESSAGE_EMAIL_ADDED_FOLDER);
        } else
            log_info(stdin, FOLDER_INFO_NOT_IN_DB);
    } else
        log_info(stdin, EMAIL_INFO_NOT_IN_DB);

}

/**
 * 
 * @param db
 */
void remove_email_from_folder_option(Database *db) {
    log_info(stdout, "You have selected: Remove email from folder option\n");
}

/**
 * 
 * @param db
 */
void search_string_option(Database* db) {
    log_info(stdout, "You have selected: Search string option\n");
}

/**
 * 
 * @param db
 */
void menu(Database* db) {

    int option = query_option();
    while (option != OPTION_QUIT) {

        switch (option) {
            case OPTION_LIST_EMAILS:
                list_emails_option(db);
                break;

            case OPTION_SHOW_EMAIL:
                show_email_option(db);
                break;

            case OPTION_CREATE_EMAIL:
                create_email_option(db);
                break;

            case OPTION_DELETE_EMAIL:
                delete_email_option(db);
                break;

            case OPTION_SHOW_FOLDER:
                show_folder_option(db);
                break;

            case OPTION_CREATE_FOLDER:
                create_folder_option(db);
                break;

            case OPTION_DELETE_FOLDER:
                delete_folder_option(db);
                break;

            case OPTION_ADD_EMAIL:
                add_email_to_folder_option(db);
                break;

            case OPTION_REMOVE_EMAIL:
                remove_email_from_folder_option(db);
                break;

            case OPTION_SEARCH:
                search_string_option(db);
                break;

            default:
                printf("Invalid option. Try again...\n\n");
                break;
        }

        printf("Press enter to continue...");
        flush_input();

        option = query_option();
    }
}