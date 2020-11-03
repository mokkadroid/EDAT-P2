


#include <stdio.h>
#include <stdlib.h>
#include "menus.h"
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"






















int main(){
  SQLHENV env;
  SQLHDBC dbc;
  SQLHSTMT stmt;
  SQLRETURN ret; /* ODBC API return status */

  ret = odbc_connect(&env, &dbc);
  if (!SQL_SUCCEEDED(ret)) {
      return EXIT_FAILURE;
  }

  /* Allocate a statement handle */
  ret=SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

  printf("%d\n%d\n%d\n%d\n%d\n%d\n%d\n", SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_ERROR, SQL_INVALID_HANDLE, SQL_NO_DATA, SQL_NEED_DATA, SQL_STILL_EXECUTING);



  menus_general(&stmt, stdout);



  /* free up statement handle */
  SQLFreeHandle(SQL_HANDLE_STMT, stmt);

  /* DISCONNECT */
  ret = odbc_disconnect(env, dbc);
  if (!SQL_SUCCEEDED(ret)) {
      return EXIT_FAILURE;
  }

  return 0;
}














/**/
