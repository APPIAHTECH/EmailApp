/* 
 * File: fileOperations.h   
 * Author:Stephen Appiah
 * DATE: 09/01/2018
 * Version : 1.0
 * 
 *  This file contains the main structure of the definition of file operations.
 * 
 */

#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "common.h"
#include "database.h"


// -------------------- File Operation Header Methods ----------------------------------

int read_config(Database *db);

int write_config(Database *db);

int store_email(Email* email);

int get_message(FILE *config_file, Database *db);

int get_message_id(FILE *config_file);

int get_folders(FILE *config_file, Database *db);

int get_messages_info(Email *email);

void set_config_section_messageID(Database *db, char *format);

int set_config_section_Folder(Database *db, Folder *folder[MAX_FOLDERS], FILE *config_file, char *format);

int set_config_section_Folder_MESSAGES(Folder *folder[MAX_FOLDERS], FILE *config_file, char *format);
#endif /* FILE_OPERATIONS_H */

