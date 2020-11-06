/**
 * @brief It defines the odbc functions to connect to the database
 *
 * @file odbc.h
 * @author Profesores de EDAT
 */


#ifndef AUX_H
#define AUX_H




/* CONNECTION PARAMETERS, ADAPT TO YOUR SCENARIO */
#define CONNECTION_PARS "DRIVER=PostgreSQL ANSI;DATABASE=classicmodels;SERVER=localhost;PORT=5432;UID=alumnodb;PWD=alumnodb;"



/* STANDARD CONNECTION PROCEDURE */
SQLRETURN odbc_connect(SQLHENV* env, SQLHDBC* dbc);

/* STANDARD DISCONNECTION PROCEDURE */
SQLRETURN odbc_disconnect(SQLHENV env, SQLHDBC dbc);

#endif
