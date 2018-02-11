/* 
 * File:folder.h   
 * Authors: Stephen Appiah Frimpong NIA: 206637 , Manuel Aneiros Otero NIA: 205351 , Hugo Hernández Quimbay NIA: 206662
 * DATE: 11/02/2018
 * Version : 1.0
 * 
 *  This file contains the main structure of the definition of folder.
 *  The folder is defined to store 10 emails and 3 emails messages as a default option ,
 *  this can be changed by changing the values of MAX_FOLDER_EMAILS if need to store more than 10 emails.
 * 
 */

#ifndef FOLDER_H
#define FOLDER_H

#include "common.h"
#include "email.h"

#define MAX_FOLDER_NAME 30 // The max length of each folder name 
#define MAX_FOLDER_EMAILS 10 // Total email that can a folder contain

// -------------------- Folder Structure ----------------------------------

typedef struct {
    char folder_name[MAX_FOLDER_NAME]; //name of the folder 
    Email *emails[MAX_FOLDER_EMAILS]; // contains email list
    int size; // contains the length of the folder name
    int empty; // determine if  a folder contains the basic structure and there is no messages
    int protected; // determine if a folder is protected or not
} Folder;


// -------------------- Folder Header Methods----------------------------------


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

