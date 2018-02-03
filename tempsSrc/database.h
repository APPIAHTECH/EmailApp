/* 
 * File:  
 * Author: 
 *
 * Fill Header!!
 */

#ifndef DATABASE_H
#define DATABASE_H

#include "common.h"
#include "email.h"
#include "folder.h"

#define MAX_FOLDERS 4
#define MAX_EMAILS 5//3

typedef struct {
    int msg_id_seed;

    Email emails[MAX_EMAILS];
    int email_count;

    Folder folders[MAX_FOLDERS];
    int folder_count;
} Database;

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

void init_folder_emails(Folder* folder, Database *db);
int add_folder_to_database(Database* db, Folder* src_folder);
void get_new_unic_id(Database* db);
#endif /* DATABASE_H */

