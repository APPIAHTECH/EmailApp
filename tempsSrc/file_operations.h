/* 
 * File:   
 * Author: 
 *
 * Fill Header!!
 */

#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "common.h"
#include "database.h"


int read_config(Database *db);

int write_config(Database *db);

int store_email(Email* email);

int get_message(FILE *config_file , Database *db);

int get_message_id(FILE *config_file);

int get_folders(FILE *config_file , Database *db);

#endif /* FILE_OPERATIONS_H */

