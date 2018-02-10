/* 
 * File:utils.h
 * Author: Stephen Appiah
 * DATE: 09/01/2018
 * This file contains all utilities stuff
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

// -------------------- Utilies Header Methods ----------------------------------
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
void str_sub_string(char *dest, char *src, int from, int to);
void get_email_store_path(char *dest, char *email);
void str_trim(char *dest, char *str);
void str_remove_new_line(char *str);
int sub_email(Email *email, char *caracter);

#endif /* UTILS_H */

