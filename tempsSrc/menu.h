/* 
 * File:  
 * Author: 
 *
 * Fill Header!!
 */

#ifndef MENU_H
#define MENU_H

#define MSG_QUIT "%d. Quit\n"
#define OPTION_QUIT 0
#define MSG_LIST_EMAILS "%d. List emails\n"
#define OPTION_LIST_EMAILS 1
#define MSG_SHOW_EMAIL "%d. Show email\n"
#define OPTION_SHOW_EMAIL 2
#define MSG_CREATE_EMAIL "%d. Create email\n"
#define OPTION_CREATE_EMAIL 3
#define MSG_DELETE_EMAIL "%d. Delete email\n"
#define OPTION_DELETE_EMAIL 4
#define MSG_SHOW_FOLDER "%d. Show folders\n"
#define OPTION_SHOW_FOLDER 5
#define MSG_CREATE_FOLDER "%d. Create folder\n"
#define OPTION_CREATE_FOLDER 6
#define MSG_DELETE_FOLDER "%d. Delete folder\n"
#define OPTION_DELETE_FOLDER 7
#define MSG_ADD_EMAIL "%d. Add email to folder\n"
#define OPTION_ADD_EMAIL 8
#define MSG_REMOVE_EMAIL "%d. Remove email from folder\n"
#define OPTION_REMOVE_EMAIL 9
#define MSG_SEARCH "%d. Search string\n"
#define OPTION_SEARCH 10
#define OPTION_INVALID -1

#define MAX_SUBSTR 100

void menu();

#endif /* MENU_H */

