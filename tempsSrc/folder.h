/* 
 * File:   
 * Author: 
 *
 * Fill Header!!
 */

#ifndef FOLDER_H
#define FOLDER_H

#include "common.h"
#include "email.h"

#define MAX_FOLDER_NAME 30
#define MAX_FOLDER_EMAILS 2 //10

typedef struct {
    char folder_name[MAX_FOLDER_NAME];
    Email *emails[MAX_FOLDER_EMAILS];
    int size;
    int empty;
    int protected;
} Folder;

void init_folder(Folder* folder);
int delete_folder(Folder* folder);
int create_folder(Folder* folder, char* name);

char* get_folder_name(Folder* folder);
int get_folder_size(Folder* folder);
int is_folder_empty(Folder* folder);
int is_folder_protected(Folder* folder);

int get_folder_emails(Folder* folder, Email** emails);

int add_email_to_folder(Folder* folder, Email* email);
int delete_folder_email(Folder* folder, Email* email);
void read_folder_interactive(Folder* folder);
void copy_folder(Folder* dest_folder, Folder* src_folder);
#endif /* FOLDER_H */

