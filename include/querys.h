/**
 * @brief It defines the functions for making querys
 *
 * @file querys.h
 * @author Erik Yuste & Lucía Martinez-Valero
 * @date 29-10-2020
 */


#ifndef QUERYS_H
#define QUERYS_H



#define MY_CHAR_LEN 255


/**
 * query_productsStock Implemeta la query que informa de la cantidad de productos que hay con el codigo especificado
 *
 * @date 29-10-2020
 * @author: Erik Yuste
 *
 * @param stmt Puntero a SQLHSTMT
 * @param out Puntero a FILE donde se imprimirá la información
 * @return Devuelve 0 si no ha habido ningún problema y otro entero si lo ha habido
 */
int query_productStock(SQLHSTMT *stmt, FILE *out);

/**
 * query_productsStock Implemeta la query que devuelve todos los productos que contienen el nombre especificado
 *
 * @date 1-11-2020
 * @author: Lucía Martinez-Valero
 *
 * @param stmt Puntero a SQLHSTMT
 * @param out Puntero a FILE donde se imprimirá la información
 * @return Devuelve 0 si no ha habido ningún problema y otro entero si lo ha habido
 */
int query_productFind(SQLHSTMT *stmt, FILE *out);

int query_orderOpen(SQLHSTMT *stmt, FILE *out);

int query_orderRange(SQLHSTMT *stmt, FILE *out);

int query_orderDetails(SQLHSTMT *stmt, FILE *out);

int query_customersFind(SQLHSTMT *stmt, FILE *out);

int query_customersListProducts(SQLHSTMT *stmt, FILE *out);

int query_customersBalance(SQLHSTMT *stmt, FILE *out);




#endif
