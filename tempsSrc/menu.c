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
    int i = 0;

    log_info(stdout, "You have selected: List emails option\n");

    puts(ALL_EMAIL);
    puts(LINE);
    for (i = 0; i < MAX_EMAILS; i++) {

        if (is_email_empty(&db->emails[i]) != TRUE) { // if positon reading is empty we dont print else that means there is a email we can print
            printf(_EMAIL_TO_PRINT, i);
            print_email(&db->emails[i]);
            puts(LINE);
        }
    }
}

/**
 * 
 * @param db
 */
void show_email_option(Database* db) {

    //Variables declarations
    char mail_to_show[MAX_EMAIL_ID];
    Email *email;

    log_info(stdout, "You have selected: Show email option\n");

    //User input value
    puts(MESSAGE_EMAIL_ENTER_NAME);
    scanf("%s", mail_to_show);

    //Searching for user email in db
    email = search_database_email_id(db, mail_to_show);

    if (email != NULL)
        print_email(email); //printing the result
    else
        log_info(stdout, EMAIL_INFO_NOT_IN_DB);



}

/**
 * 
 * @param db
 */
void create_email_option(Database* db) {

    //Variables declarations
    Email email, *temp;
    Folder *folder = NULL;
    char new_id[MAX_BUF], valid, succes;

    log_info(stdout, "You have selected: Create email option\n");

    init_email(&email); //setting up a default satement to email
    get_new_message_id(db, new_id); //generating a new id for the message
    get_new_unic_id(db); //generaing a unic id for the next email to creat
    strcpy(email.id, new_id); // setting the new id to email
    read_email_interactive(&email); //getting user information to setup email structure

    valid = validate_email(&email);

    if (valid) {
        folder = get_database_folder(db, PROTECT_OUTBOX); //getting Outbox folder such that we can add the email to this folder
        temp = add_email_to_database(db, &email);

        if (folder != NULL) { //if it founds  Outbox folder 
            if (temp != NULL) {
                if (add_email_to_folder(folder, temp) != FAIL) {
                    succes = store_email(temp); //Finaly writing the email to the emailDB
                    if (succes)
                        log_info(stdout, MESSAGE_EMAIL_CREATED);
                    else
                        log_error(stdout, ERROR_SAVING_FILE);
                } else {
                    log_error(stdout, ERROR_EMAIL_CANT_ADD_FOLDER);
                    log_warn(stdout, ERROR_EMAIL_CANT_ADD_FOLDER_FIX);
                }
            } else {
                log_error(stdout, ERROR_EMAIL_CANT_ADD_DB);
                log_warn(stdout, ERROR_EMAIL_CANT_ADD_DB_FIX);
            }
        } else {
            log_error(stdout, ERROR_CAN_FIND_OUTBOX);
            log_warn(stdout, ERROR_SPECIAL_EMConfig);
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

    //Print sections
    puts(ALL_FOLDER);
    total_folder = get_database_folder_count(db);
    total_email = get_database_email_count(db);

    printf(_FOLDER_TOTAL_PRINT, total_folder);
    printf(_EMAIL_TOTAL_PRINT, total_email);
    puts(LINE);

    //For each folder and for each folder email we print they values
    for (i = 0; i < MAX_FOLDERS; i++) {

        if (strcmp(db->folders[i].folder_name, FOLDER_INIT_NAME) != 0) { //if the folder contains a default value , it means is an empty folder in memory so we don't print it
            printf(_FOLDER_TO_PRINT, db->folders[i].folder_name, i);

            for (j = 0; j < MAX_FOLDER_EMAILS; j++) {
                if (db->folders[i].emails[j] != NULL) { //if email is empty we dont print it
                    print_email(db->folders[i].emails[j]);
                    printf(LINE);
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

    init_folder(&folder);
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
    puts(MESSAGE_EMAIL_TO_ADD_FOLDER);
    scanf("%s", mail_to_add);

    puts(MESSAGE_FOLDER_EMAIL_TO_ADD);
    scanf("%s", folder_to_store);

    //Finding the email on the forder
    email = search_database_email_id(db, mail_to_add);
    if (email == NULL) //if can't find email specify
        log_info(stdout, EMAIL_INFO_NOT_IN_DB);
    else {
        folder = get_database_folder(db, folder_to_store);
        if (folder == NULL)//if can't find folder specify
            log_error(stdout, FOLDER_INFO_NOT_IN_DB);
        else {
            if (add_email_to_folder(folder, email) == SUCCESS) {
                log_info(stdout, MESSAGE_EMAIL_ADDED_FOLDER);
            } else {
                log_error(stdout, ERROR_EMAIL_CANT_ADD_FOLDER);
                log_warn(stdout, ERROR_EMAIL_CANT_ADD_FOLDER_FIX);
            }
        }

    }




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
    //Variable declaration
    int i = 0, founded = 0;
    char caracter[MAX_BUF];

    log_info(stdout, "You have selected: Search string option\n");

    //Getting caracter info
    printf(_SEARCH_MSG);
    scanf("%s", caracter);

    puts(ALL_MATCHED_EMAIL);
    for (i = 0; i < MAX_EMAILS; i++) {
        if (sub_email(&db->emails[i], caracter)) {
            printf(_SEARCH_MSG_RESULT, &db->emails[i].id);
            founded++;
        }

    }
    if (founded == 0)
        puts(_SEARCH_MSG_NO_RESULT);
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