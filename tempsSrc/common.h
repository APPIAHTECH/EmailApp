/* 
 * File:  
 * Author: 
 *
 * Fill Header!!
 */


#ifndef COMMON_H
#define COMMON_H

#define TRUE 1
#define FALSE 0

#define SUCCESS 1
#define FAIL 0
#define ERROR -1
#define NOT_FOUNDED -1
#define UNDEFINED 0

#define MAX_PATH 150
#define MAX_BUF 100
#define MIN_FOLDERS 2

/**FORMATS*/
#define FORMAT_MESSAGE_ID "%s %d"
#define FORMAT_SPECIAL_BEAM_CANON "%s\n"
#define FORMAT_FIELD_HEADERS ":"
#define NEW_LINE "\r\n"
#define FORMAT_EMAIL_FILE_STRUC "%s"


/**Email Basic Structure*/
#define EMAIL_INIT_ID "_EDA_email"
#define EMAIL_INIT_FROM "stephen.appiah@estudiant.upf.edu"
#define EMAIL_INIT_TO "manuell@estudiant.upf.edu"
#define EMAIL_INIT_SUBJECT "hugo k tal"
#define EMAIL_INIT_BODY "Esto es una estructura!!"

/**EMAIL FILE FOR READING STRUCTURE*/
#define EMAIL_FILE_STRUC_DATE "Date:"
#define EMAIL_FILE_STRUC_FROM "From:"
#define EMAIL_FILE_STRUC_TO "To:"
#define EMAIL_FILE_STRUC_MESSAGEID "Message-ID:"
#define EMAIL_FILE_STRUC_SUBJECT "Subject:"

/**EMAIL INTERACTIVE**/
#define EMAIL_INTERACTIVE_FROM "From:\n"
#define EMAIL_INTERACTIVE_TO "To:\n"
#define EMAIL_INTERACTIVE_SUBJECT "Subject:\n"
#define EMAIL_INTERACTIVE_MESSAGE "Message:\n"

/**Folder Basic Structure*/
#define FOLDER_INIT_NAME "_EDA_folder"

/**Folders Protectes*/
#define PROTECT_INBOX "Inbox"
#define PROTECT_OUTBOX "OutBox"

/**PATHS*/
#define EM_CONFIG_PATH "emailDB/EMconfig.txt"
#define EM_STORE_PATH "emailDB/"

/**FILE*/
#define FILE_READ_MODE "r"
#define FILE_WRITE_MODE "w"
#define FILE_APPEND_MODE "a"
#define FILE_END "End"
#define FILE_FORMAT ".txt"

/**INFOS*/

#define MESSAGE_EMAIL_CREATED "Mail has been created and saved at Outbox Folder!"
#define MESSAGE_FOLDER_CREATED "Folder created"
#define EMAIL_INFO_NOT_IN_DB "Cant find the email you are asking"
#define FOLDER_INFO_NOT_IN_DB "Cant find the folder you are asking"
#define FOLDER_INFO_ALREADY_IN "This folder already exist"
#define MESSAGE_EMAIL_ADDED_FOLDER "You have succefuly added the email to the folder"

/**ERRORS*/
#define ERROR_MSG_FILE_NOT_FOUDN "File not found Unlucky :(  "
#define ERROR_MSG_FILE_CANT_CREAT "Cant creat file Unlucky :(  "
#define ERROR_SPECIAL_EMConfig "Problem found: Please revise your EMConfig file format\n"
#define ERROR_MAIL_INVALID "Invalid email , please check it and try again :D "
#define ERROR_FOLDER_CANT_ADD "Ups can't add folder"
#define ERROR_EMAIL_CANT_ADD_DB "Ups can't add email to db"
#define ERROR_EMAIL_CANT_ADD_FOLDER "Ups can't add email to folder"
#define ERROR_SAVING_FILE "Someting went wrong while saving email file"
#define ERROR_SAVING_CONIFG_FILE "WTF! , CANT SAVE CONFIG FILE :("
#endif /* COMMON_H */

/**WARNING*/
#define ERROR_FOLDER_CANT_ADD_FIX "The db folder is full , try to delete some folder and try it again :D"
#define ERROR_EMAIL_CANT_ADD_DB_FIX "The db email is full , try to delete some emails and try it again :D"
#define ERROR_EMAIL_CANT_ADD_FOLDER_FIX "This folder is full , try to delete some emails from the folder, and try it again :D"
