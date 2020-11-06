/**
 * @brief It defines the menus functions
 *
 * @file menus.h
 * @author Erik Yuste & Lucía Martinez-Valero
 * @date 28-10-2020
 */


#ifndef MENUS_H
#define MENUS_H


#include <sql.h>
#include <sqlext.h>


/**
 * menus_general Implemeta la lógica del menu del que dependen los demás
 *
 * @date 28-10-2020
 * @author: Erik Yuste
 *
 * @param stmt Puntero a SQLHSTMT
 * @param out Puntero a FILE donde se imprimirá la información
 * @return Devuelve 0 si no ha habido ningún problema y otro entero si lo ha habido
 */
int menus_general(SQLHSTMT *stmt, FILE *out);




#endif
