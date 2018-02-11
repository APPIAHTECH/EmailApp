/* 
 * Grup de Seminari: S401
 * Professor seminari: Roberto Juan Carlini
 * Incidències:
 * 
 * Authors: Stephen Appiah Frimpong NIA: 206637 , Manuel Aneiros Otero NIA: 205351 , Hugo Hernández Quimbay NIA: 206662
 * DATE: 11/02/2018
 * 
 * Número problema:
 * Enunciat Problema:
 * Ús del programa: 
 * Estat de l'entrega: Compila y funciona
 * Altres comentaris o incidències:
 */

#include "imports.h"
#include "file_operations.h"
#include "menu.h"
#include "utils.h"

int main(int argc, char**argv) {

    //Variables declarations
    Database db;

    //init database all fileds
    init_database(&db, UNDEFINED);

    //reading config file and storing it on db
    int result = read_config(&db);

    if (result) { //if all goes well it show the menu for user to decide what to do
        menu(&db);
        result = write_config(&db); //when the end of the app , it stors/update all the db info to the config file
        if (result == FAIL)
            log_error(stdin, ERROR_SAVING_CONIFG_FILE);
    } else {
        log_error(stdin, ERROR_SPECIAL_EMConfig);
    }
    return 0;
}
