/* 
 * File:  
 * Author: 
 *
 * Fill Header!!
 */

#ifndef UTILS_H
#include "common.h"
#include "folder.h"
#include "database.h"
#define UTILS_H

#define ERROR_LEVEL 0
#define WARN_LEVEL 1
#define INFO_LEVEL 2

#define LOG_LEVEL 2

void flush_input();

void log_warn(FILE* fd, char* msg);
void log_error(FILE* fd, char* msg);
void log_info(FILE* fd, char* msg);

void display_error(char *msg);
void print_email(Email *email);
char *get_curent_date();
void str_remove_trash(char *buff);
int index_of(char * str, char * searched);
void print_folder(Folder *folder);
void print_database(Database *db);
#endif /* UTILS_H */

