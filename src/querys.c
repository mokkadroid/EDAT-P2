#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "querys.h"



void stop();

void query_productStockInterface(SQLHSTMT *stmt, SQLINTEGER *result, SQLCHAR *productcode);
void query_productFindInterface(SQLHSTMT *stmt, SQLCHAR *pcode, SQLCHAR *pname, SQLCHAR *string);




/*
 *   PRODUCT  STOCK
 */

int query_productStock(SQLHSTMT *stmt, FILE *out){
  SQLRETURN ret; /* ODBC API return status */
  SQLCHAR productcode[MY_CHAR_LEN];
  SQLINTEGER y;

  if(!stmt || !out) return 1;


  fprintf(out," > ");
  fflush(out);
  scanf("%s", productcode);

  ret=SQLPrepare((*stmt), (SQLCHAR*) "select p.quantityinstock from products as p where p.productcode = ?", SQL_NTS);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLPREPARE %d\n", ret);


  ret=SQLBindParameter((*stmt), 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, productcode, 0, NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDPARAMETER\n");

  /* Ejecuta la query */
  ret=SQLExecute(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLEXECUTE\n");

  /* Asigna la columna resultado a la variable y */
  ret=SQLBindCol(*stmt, 1, SQL_INTEGER, &y, sizeof(y), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL\n");

  /* Interfaz */
  query_productStockInterface(stmt, &y, productcode);

  SQLCloseCursor(*stmt);

  fflush(out);

  return 0;
}





void query_productStockInterface(SQLHSTMT *stmt, SQLINTEGER *result, SQLCHAR *productcode){
  SQLRETURN ret;
  int a=0;

  while(SQL_SUCCEEDED(ret = SQLFetch(*stmt))) {
      printf("\n\n  Hay %d productos con el codigo \'%s\'.\n\n", *result, productcode);
      a++;
  }
  if(!a) printf("\n\n  No hay ningun producto con el codigo \'%s\'.\n\n",productcode);

  stop();

  return;
}





/*
 *  PRODUCT FIND
 */

/*select p.productcode, p.productname from products p where p.productname like '%cadena%';*/
int query_productFind(SQLHSTMT *stmt, FILE *out){
  SQLRETURN ret; /* ODBC API return status */
  SQLCHAR productcode[MY_CHAR_LEN], productname[MY_CHAR_LEN], string[MY_CHAR_LEN];
  char query[MY_CHAR_LEN]="select p.productcode, p.productname from products p where p.productname like '%";





  if(!stmt || !out) return 1;


  fprintf(out," > ");
  fflush(out);
  scanf("%s", string);

  strcat(query, string);
  strcat(query, "%' order by p.productcode");

  ret=SQLPrepare((*stmt), (SQLCHAR*) query, SQL_NTS);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLPREPARE %d\n", ret);


  /*ret=SQLBindParameter((*stmt), 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, string, 0, NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDPARAMETER\n");

  /* Ejecuta la query */
  ret=SQLExecute(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLEXECUTE\n");

  /* Asigna la columna resultado a la variable y */
  ret=SQLBindCol(*stmt, 1, SQL_CHAR, productcode, sizeof(productcode), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL\n");
  /* Asigna la columna resultado a la variable y */
  ret=SQLBindCol(*stmt, 2, SQL_CHAR, productname, sizeof(productname), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL\n");

  /* Interfaz */
  query_productFindInterface(stmt, productcode, productname, string);

  SQLCloseCursor(*stmt);

  fflush(out);

  return 0;
}


void query_productFindInterface(SQLHSTMT *stmt, SQLCHAR *pcode, SQLCHAR *pname, SQLCHAR *string){
  SQLRETURN ret;
  int a=1;

  printf("\n\t| Product code\t| Product name\n");
  printf(  "--------+-----------------+--------------\n");


  while(SQL_SUCCEEDED(ret = SQLFetch(*stmt))) {
      printf("   %d\t| %s\t| %s\t\n", a, pcode, pname);
      a++;
      if(!(a%10)){

        stop();
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t| Product code\t| Product name\n");
        printf(  "--------+-----------------+--------------\n");
      }
  }
  printf("\n");
  if(!(a-1)) printf("\n < No product named \'%s\' >\n\n",string);

  printf(" < Press ENTER to exit >\n");


  stop();

  return;
}


















/**/



void stop(){

  while(getchar()!='\n');
  getchar();

  return;
}
