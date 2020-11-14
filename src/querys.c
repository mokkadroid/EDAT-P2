/**
 * @brief Implementa las funciones que realizan las querys a la base de datos
 *
 * @file querys.c
 * @author Erik Yuste & Lucia Martinez-Valero
 * @version 1.0
 * @date 29-10-2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "querys.h"



/*Privadas*/

/**
* stop Función que detiene la ejecución del programa hasta que el usuario pulsa una tecla
*
* @date 02-11-2020
* @author: Erik Yuste
*/
static void stop();


/**
* query_productStockInterface imprime el resultado de la query 'Stock'
*
* @date 29-10-2020
* @author: Erik Yuste
*
* @param stmt Puntero a SQLHSTMT
* @param result Puntero a SQLINTEGER donde se recibirá el resultado de la query
* @param productcode Puntero a SQLCHAR donde se encuentra el codigo de producto introducido por el usuario
*/
static void query_productStockInterface(SQLHSTMT *stmt, SQLINTEGER *result, SQLCHAR *productcode);
/**
* query_productFindInterface imprime el resultado de la query 'Find'
*
* @date 01-11-2020
* @author: Lucia Martinez-Valero
*
* @param stmt Puntero a SQLHSTMT
* @param pcode Puntero a SQLCHAR donde se recibirá el nombre de los productos resultado de la query
* @param pname Puntero a SQLCHAR donde se recibirá el codigo de los productos resultado de la query
* @param string Puntero a char donde se almacena la cadena de caracters introducida por el usuario
*/
static void query_productFindInterface(SQLHSTMT *stmt, SQLCHAR *pcode, SQLCHAR *pname, char *string);
/**
* query_orderOpenInterface imprime el resultado de la query 'Stock'
*
* @date 06-11-2020
* @author: Lucia Martinez-Valero
*
* @param stmt Puntero a SQLHSTMT
* @param result Puntero a SQLCHAR donde se recibirá el resultado de la query
*/
static void query_orderOpenInterface(SQLHSTMT *stmt, SQLCHAR *onum);
/**
* query_customersFindInterface imprime el resultado de la query 'Find'
*
* @date 13-11-2020
* @author: Erik Yuste
*
* @param stmt Puntero a SQLHSTMT
* @param cnum Puntero a SQLCHAR donde se recibirá el numero de los clientes resultado de la query
* @param cname Puntero a SQLCHAR donde se recibirá el nombre de los clientes resultado de la query
* @param cfn Puntero a SQLCHAR donde se recibirá el primer nombre de los clientes resultado de la query
* @param csn Puntero a SQLCHAR donde se recibirá el segundo nombre de los clientes resultado de la query
* @param string Puntero a char donde se almacena la cadena de caracters introducida por el usuario
*/
void query_customersFindInterface(SQLHSTMT *stmt, SQLCHAR *cnum, SQLCHAR *cname, SQLCHAR *cfn, SQLCHAR *csn,char *string);




/*
 *   PRODUCT  STOCK
 */

int query_productStock(SQLHSTMT *stmt, FILE *out){
  SQLRETURN ret; /* ODBC API return status */
  SQLCHAR productcode[MY_CHAR_LEN];
  SQLINTEGER y=(SQLINTEGER) 0;

  if(!stmt || !out) return 1;


  fprintf(out," > ");
  if(fflush(out)!=0) printf("ERROR FFLUSH");
  if(scanf("%s", (char*)productcode)==EOF) printf("ERROR SCANF");

  ret=SQLPrepare((*stmt), (SQLCHAR*) "select p.quantityinstock from products as p where p.productcode = ?", SQL_NTS);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLPREPARE %d\n", ret);


  ret=SQLBindParameter((*stmt), 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, productcode, 0, NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDPARAMETER\n");

  /* Ejecuta la query */
  ret=SQLExecute(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLEXECUTE\n");

  /* Asigna la columna resultado a la variable y */
  ret=SQLBindCol(*stmt, 1, SQL_INTEGER, &y, (SQLLEN) sizeof(y), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL\n");

  /* Interfaz */
  query_productStockInterface(stmt, &y, productcode);

  ret=SQLCloseCursor(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLCLOSECURSOR\n");

  if(fflush(out)!=0) printf("ERROR FFLUSH");

  return 0;
}


void query_productStockInterface(SQLHSTMT *stmt, SQLINTEGER *result, SQLCHAR *productcode){
  SQLRETURN ret;

  ret=SQLFetch(*stmt);
  if(SQL_SUCCEEDED(ret)) printf("\n\n < There are %d products with the code \'%s\' >\n\n", *result, (char*) productcode);

  else printf("\n\n < No products with the code \'%s\' found >\n\n",(char*) productcode);


  /*stop();*/

  return;
}


/*
 *  PRODUCT FIND
 */

/*select p.productcode, p.productname from products p where p.productname like '%cadena%';*/
int query_productFind(SQLHSTMT *stmt, FILE *out){
  SQLRETURN ret; /* ODBC API return status */
  SQLCHAR productcode[MY_CHAR_LEN], productname[MY_CHAR_LEN];
  SQLCHAR string[MY_CHAR_LEN], query[MY_CHAR_LEN]="select p.productcode, p.productname from products p where position(UPPER(?) in UPPER(p.productname))>0 order by p.productcode";




   if(!stmt || !out) return 1;


   (void) fflush(out);
   if(scanf("%s", (char*) string)==EOF) printf("ERROR SCANF");


  ret=SQLPrepare((*stmt), (SQLCHAR*) query, SQL_NTS);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLPREPARE %d\n", ret);


  ret=SQLBindParameter((*stmt), 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, string, 0, NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDPARAMETER\n");

  /* Ejecuta la query */
  ret=SQLExecute(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLEXECUTE\n");

  /* Asigna la columna resultado a la variable y */
  ret=SQLBindCol(*stmt, 1, SQL_CHAR, productcode, (SQLLEN) sizeof(productcode), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL\n");
  /* Asigna la columna resultado a la variable y */
  ret=SQLBindCol(*stmt, 2, SQL_CHAR, productname, (SQLLEN) sizeof(productname), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL\n");

  /* Interfaz */
  query_productFindInterface(stmt, productcode, productname, string);

  ret=SQLCloseCursor(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLCLOSECURSOR\n");

  if(fflush(out)!=0) printf("ERROR FFLUSH");

  return 0;
}


void query_productFindInterface(SQLHSTMT *stmt, SQLCHAR *pcode, SQLCHAR *pname, char *string){
  SQLRETURN ret;
  int a=1;

  if(!stmt || !pcode || !pname || !string){
    printf("INTERFACE FAILURE\n");
    return;
  }

  while(SQL_SUCCEEDED(ret = SQLFetch(*stmt))) {
      if(a==1){
        printf("\n\t| Product code\t| Product name\n");
        printf(  "--------+-----------------+--------------\n");
      }
      printf("   %d\t| %s\t| %s\t\n", a, (char*) pcode, (char*) pname);
      a++;
      if((a%10)==0){

        stop();
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t| Product code\t| Product name\n");
        printf(  "--------+-----------------+--------------\n");
      }
  }
  printf("\n");
  if(a==1) printf("\n < No product named \'%s\' >\n\n",string);

  printf(" < Press ENTER to exit >\n");


  /*stop();*/

  return;
}

/*
 *  ORDER OPEN
 */

int query_orderOpen(SQLHSTMT *stmt, FILE *out){
  SQLRETURN ret; /* ODBC API return status */
  SQLCHAR ordernumber[MY_CHAR_LEN];
  char query[MY_CHAR_LEN]="select o.ordernumber from orders o where o.shippeddate isnull order by o.ordernumber";


  if(!stmt || !out) return -1;


  if(fflush(out)!=0) printf("ERROR FFLUSH");

  ret=SQLPrepare((*stmt), (SQLCHAR*) query, SQL_NTS);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLPREPARE %d\n", ret);

  /* Ejecuta la query */
  ret=SQLExecute(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLEXECUTE\n");

  /* Asigna la columna resultado a la variable y */
  ret=SQLBindCol(*stmt, 1, SQL_INT, ordernumber, (SQLLEN) sizeof(ordernumber), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL\n");


  /* Interfaz */
  query_orderOpenInterface(stmt, ordernumber);

  ret=SQLCloseCursor(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLCLOSECURSOR\n");

  if(fflush(out)!=0) printf("ERROR FFLUSH");

  return 0;
}

/* Interfaz */
static void query_orderOpenInterface(SQLHSTMT *stmt, SQLCHAR *onum){
  int a=1;

  printf("\n\t| Order number\t|\n");
  printf(  "--------+-----------------+\n");

  while(SQL_SUCCEEDED(ret = SQLFetch(*stmt))) {
      printf("   %d\t| %s\t|\n", a, (char*) onum);
      a++;
      if((a%10)==0){

        stop();
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t| Order number\t|\n");
        printf(  "--------+-----------------+\n");
      }
  }
  printf("\n");
  if(a==1) printf("\n < All the orders have been shipped >\n\n");

  return;

}

/*SELECT o.ordernumber, o.orderdate, o.shippeddate
FROM  orders o
WHERE o.orderdate >= '2003-01-01' AND o.orderdate <= '2004-01-01'
ORDER BY o.ordernumber DESC*/
int query_orderRange(SQLHSTMT *stmt, FILE *out, SQL_DATE orderdate, SQL_DATE shippeddate){
  SQLRETURN ret; /* ODBC API return status */
  char odd[MY_CHAR_LEN], odd2[MY_CHAR_LEN], query[MY_CHAR_LEN]="select o.ordernumber, o.orderdate, o.shippeddate from orders o where o.shippeddate >= ? and o.orderdate <= ? order by o.ordernumber desc";

  if(!stmt || !out) return 1;


  if(fflush(out)!=0) printf("ERROR FFLUSH");
  printf("Initial date: ");
  if(scanf("%s", odd)==EOF) printf("ERROR SCANF");
  if(fflush(out)!=0) printf("ERROR FFLUSH");
  printf("\nEnd date: ");
  if(scanf("%s", odd2)==EOF) printf("ERROR SCANF");

  /*Sabemos que las fechas tienen el siguiente formato: YYYY-MM-DD, vamos a comprobar que sea así*/
  if((odd[4]=='-' && odd[7]=='-') && (odd2[4]=='-' && odd2[7]=='-') && odd[10]=='\0' && odd2[10]=='\0'){ /* Con la comprobacion de la posicion de los guiones y \0, vemos si el formato es correcto */
    if(odd[5]>'1' || odd2[5]>'1'){ /* vemos que el mes sea correcto*/
      printf("Error in MM, value over 12.\n");
      return 0;
    }
    if((odd[5]=='1' && odd[6]>'2') || (odd2[5]=='1' && odd2[6]>'2')){
      printf("Error in MM, value over 12.\n");
      return 0;
    }
    if(odd[8]>'3' || odd2[8]>'3' || (odd[8]=='3' && odd[9]>'1') || (odd2[8]=='3' && odd2[9]>'1')){
      printf("Error in DD, value over 31.\n");
      return 0;
    }
  } else {
    printf("Wrong date format\n");
    return 0;
  }


  ret=SQLPrepare((*stmt), (SQLCHAR*) query, SQL_NTS);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLPREPARE %d\n", ret);


  ret=SQLBindParameter((*stmt), 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_TYPE_DATE, 10, 0, odd, 0, NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDPARAMETER\n");
  ret=SQLBindParameter((*stmt), 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_TYPE_DATE, 10, 0, odd2, 0, NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDPARAMETER\n");

  /* Interfaz */
  query_orderOpenInterface(stmt, ordernumber);

  ret=SQLCloseCursor(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLCLOSECURSOR\n");

  if(fflush(out)!=0) printf("ERROR FFLUSH");

  return 0;
}

int query_orderDetails(SQLHSTMT *stmt, FILE *out){}





/*
 *  CUSTOMERS FIND
 */
/*
select c.customernumber, c.customername, c.contactfirstname, c.contactlastname from customers c where (c.contactfirstname like '%Mary%') or (c.contactlastname like '%Mary%') order by c.customernumber;*/
int query_customersFind(SQLHSTMT *stmt, FILE *out){
  SQLRETURN ret; /* ODBC API return status */
  SQLCHAR cnumber[MY_CHAR_LEN], cname[MY_CHAR_LEN], cfirstn[MY_CHAR_LEN], csecondn[MY_CHAR_LEN];
  char string[MY_CHAR_LEN], query[MY_CHAR_LEN]="select c.customernumber, c.customername, c.contactfirstname, c.contactlastname from customers c where position(UPPER(?) in UPPER(c.contactfirstname))>0 or position(UPPER(?) in UPPER(c.contactlastname))>0 order by c.customernumber";



  if(!stmt || !out) return 1;


  if(fflush(out)!=0) printf("ERROR FFLUSH");
  if(scanf("%s", string)==EOF) printf("ERROR SCANF");


  ret=SQLPrepare((*stmt), (SQLCHAR*) query, SQL_NTS);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLPREPARE %d\n", ret);


  ret=SQLBindParameter((*stmt), 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, string, 0, NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDPARAMETER\n");
  ret=SQLBindParameter((*stmt), 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 0, 0, string, 0, NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDPARAMETER\n");

  /* Ejecuta la query */
  ret=SQLExecute(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLEXECUTE\n");

  /* Asigna la columna resultado a la variable y */
  ret=SQLBindCol(*stmt, 1, SQL_CHAR, cnumber, (SQLLEN) sizeof(cnumber), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 1\n");
  /* Asigna la columna resultado a la variable y */
  ret=SQLBindCol(*stmt, 2, SQL_CHAR, cname, (SQLLEN) sizeof(cname), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 2\n");

  ret=SQLBindCol(*stmt, 3, SQL_CHAR, cfirstn, (SQLLEN) sizeof(cfirstn), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 3\n");

  ret=SQLBindCol(*stmt, 4, SQL_CHAR, csecondn, (SQLLEN) sizeof(csecondn), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 4\n");

  /* Interfaz */
  query_customersFindInterface(stmt, cnumber, cname, cfirstn, csecondn, string);

  ret=SQLCloseCursor(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLCLOSECURSOR\n");

  if(fflush(out)!=0) printf("ERROR FFLUSH");

  return 0;
}


void query_customersFindInterface(SQLHSTMT *stmt, SQLCHAR *cnum, SQLCHAR *cname, SQLCHAR *cfn, SQLCHAR *csn,char *string){
  SQLRETURN ret;
  int a=1;
  char titulo[]="    | Number\t| Customer Name \t| Customer First Name\t| Customer Second Name\n";

  while(SQL_SUCCEEDED(ret = SQLFetch(*stmt))) {
    if(a==1){
      printf("\n%s", titulo);
      printf(  "----+-----------+-----------------------+-----------------------+---------------------\n");
    }

    if(a<10)
      printf(" 0%d | %s\t| %s\t| %s\t| %s\t\n", a, (char*) cnum, (char*) cname, (char*)cfn, (char*)csn);
    else
      printf(" %d | %s\t| %s\t| %s\t| %s\t\n", a, (char*) cnum, (char*) cname, (char*)cfn, (char*)csn);

      a++;
      if((a%10)==0){

        stop();
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n%s", titulo);
        printf(  "----+-----------+-----------------------+-----------------------+---------------------\n");
      }
  }
  printf("\n");
  if(a==1) printf("\n < No product named \'%s\' >\n\n",string);

  /*stop();*/

  return;
}









/**/



void stop(){
  int a;
  while(getchar()!= (int)'\n');
  a = getchar();

  return;
}
