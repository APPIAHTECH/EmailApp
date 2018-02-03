/* 
 * Grup de Seminari: S401
 * Professor seminari: Roberto Juan Carlini
 * Nom Fitxer: FP-S401-Sy-Ez-NIA-Cognom-Nom.c
 * Incidències:
 * 
 * Autor (Nom i NIA):
 * Data/hora:
 * Collaboradors (si cal):
 * 
 * Número problema:
 * Enunciat Problema:
 * Ús del programa:
 * Estat de l'entrega:
 * Altres comentaris o incidències:
 */

/**NOTE : EL PROJECTO ESTA 90%*/

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
