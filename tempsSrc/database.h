/* 
 * File:database.h   
 * Author:Stephen Appiah
 * DATE: 09/01/2018
 * Version : 1.0
 * 
 *  This file contains the main structure of the definition of database.
 *  This database is defined to store 3 folder and 3 emails messages as a default option ,
 *  this can be changed by changing the values of MAX_FOLDERS if need to store more than 3 folders and MAX EMAILS
 *  if need to store more than 3 emails.
 * 
 *  Email arrays messages is defined as a unique set of emails , such that there is no repeated email on the array
 *  For the folder we also don't expect to have repeated folder in the array , so the folder array also are a set of unique non repeated folder array.
 * 
 *  msg_id_seed saves the last unique id generated for an email , such that for every new email created there is a new unic id to give to that email
 *  Finally folder count contains the total of folders in the array.
 * 
 */

#ifndef DATABASE_H
#define DATABASE_H

#include "common.h"
#include "email.h"
#include "folder.h"

#define MAX_FOLDERS 3 //max folder that can be store in the database
#define MAX_EMAILS 3 //max emails that can be store in the database

// -------------------- Database Structure ----------------------------------

typedef struct {
    int msg_id_seed; // saves the last id generated for a message

    Email emails[MAX_EMAILS]; // contains all list of emails 
    int email_count; // saves the amount of emails in email list

    Folder folders[MAX_FOLDERS]; // contains all list of folders 
    int folder_count; // saves the amount of emails in email list
} Database;


// -------------------- Database Headers Methods ----------------------------------


void init_database(Database *db, int initial_msg_id);


int get_database_email_count(Database* db);

int get_database_emails(Database* db, Email** emails);


Email* search_database_email_id(Database* db, char* target_id);


void get_new_message_id(Database* db, char buf[]);


Email* add_email_to_database(Database* db, Email* email);


int delete_database_email(Database* db, Email* email);

int get_database_folder_count(Database* db);

int get_database_folders(Database* db, Folder** folders);


Folder* get_database_folder(Database* db, char* target_name);


Folder* create_database_folder(Database* db, char* folder_name);


int delete_database_folder(Database* db, Folder* folder);

int add_folder_to_database(Database* db, Folder* src_folder);

void get_new_unic_id(Database* db);


#endif /* DATABASE_H */

