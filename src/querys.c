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
static void query_orderOpenInterface(SQLHSTMT *stmt, SQLINTEGER *onum);
static void  query_orderRangeInterface(SQLHSTMT *stmt, SQLINTEGER *ordernumber, SQLCHAR *orderdate, SQLCHAR *shippeddate);
static void query_orderDetailsInterface(SQLHSTMT *stmt, SQLINTEGER *odnum, SQLDATE *oddate, SQLCHAR *st, SQLCHAR *pc, SQLINTEGER *q, SQLDOUBLE *price);
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
void query_customersListProductsInterface(SQLHSTMT *stmt, SQLINTEGER cnum, SQLCHAR *pname, SQLINTEGER *qordered);
void query_customersBalanceInterface(SQLHSTMT *stmt, SQLINTEGER cnum, SQLDOUBLE *saldo);


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
        printf("--------+-----------------+--------------\n");
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
  SQLINTEGER ordernumber;
  char query[MY_CHAR_LEN]="select o.ordernumber from orders o where o.shippeddate isnull order by o.ordernumber";


  if(!stmt || !out) return -1;


  if(fflush(out)!=0) printf("ERROR FFLUSH");

  ret=SQLPrepare((*stmt), (SQLCHAR*) query, SQL_NTS);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLPREPARE %d\n", ret);

  /* Ejecuta la query */
  ret=SQLExecute(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLEXECUTE\n");

  /* Asigna la columna resultado a la variable */
  ret=SQLBindCol(*stmt, 1, SQL_INTEGER, &ordernumber, (SQLLEN) sizeof(ordernumber), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL\n");


  /* Interfaz */
  query_orderOpenInterface(stmt, &ordernumber);

  ret=SQLCloseCursor(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLCLOSECURSOR\n");

  if(fflush(out)!=0) printf("ERROR FFLUSH");

  return 0;
}

/* Interfaz */
static void query_orderOpenInterface(SQLHSTMT *stmt, SQLINTEGER *onum){
  int a=1;
  SQLRETURN ret;

  while(SQL_SUCCEEDED(ret = SQLFetch(*stmt))) {

      if(a==1){
        printf("| Order number\n");
        printf("+---------------\n");
      }
      printf(" %d\n", *((int*) onum));
      a++;
      /*if((a%10)==0){

        stop();
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t| Order number\t|\n");
        printf(  "--------+-----------------\n");
        printf("   %d\t| %d\n", a, *((int*) onum));
      }*/
  }
  printf("\n");
  if(a==1) printf("\n < All the orders have been shipped >\n\n");

  return;

}

/*
 *  ORDER RANGE
 */

/*SELECT o.ordernumber, o.orderdate, o.shippeddate
FROM  orders o
WHERE o.orderdate >= '2003-01-01' AND o.orderdate <= '2004-01-01'
ORDER BY o.ordernumber */
int query_orderRange(SQLHSTMT *stmt, FILE *out){
  SQLRETURN ret; /* ODBC API return status */
  SQLINTEGER ordernumber;
  int i=0, k=0;
  char odd[MY_CHAR_LEN], odd2[MY_CHAR_LEN], in[MY_CHAR_LEN], orderdate[MY_CHAR_LEN], shippeddate[MY_CHAR_LEN];
  char query[]="select o.ordernumber, o.orderdate, o.shippeddate from orders o where o.orderdate >= ? and o.orderdate <= ? order by o.ordernumber";

  if(!stmt || !out) return 1;


  if(fflush(out)!=0) printf("ERROR FFLUSH");
  printf("Enter dates (YYYY-MM-DD - YYYY-MM-DD) > ");
  fgets(in, MY_CHAR_LEN, stdin);

/*premparamos las fechas para el bind parameter*/
  while(in[i]!='\0'){
    if(in[i]!=' '){
      odd[i]=in[i];
    }
    if(in[i]==' '){
      odd[i]='\0';
      i+=3;
    }
    if(i>=13){
      odd2[k]=in[i];
      k++;
    }
    i++;
  }
  odd2[k]='\0';

  ret=SQLPrepare((*stmt), (SQLCHAR*) query, SQL_NTS);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLPREPARE %d\n", ret);


  ret=SQLBindParameter((*stmt), 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 10, 0, odd, 0, NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDPARAMETER\n");
  ret=SQLBindParameter((*stmt), 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 10, 0, odd2, 0, NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDPARAMETER\n");

  ret=SQLExecute(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLEXECUTE\n");

  /* Asigna la columna resultado a las variables  */
  ret=SQLBindCol(*stmt, 1, SQL_INTEGER, &ordernumber, (SQLLEN) sizeof(ordernumber), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 1\n");
  ret=SQLBindCol(*stmt, 2, SQL_CHAR, orderdate, (SQLLEN) sizeof(orderdate), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 2\n");
  ret=SQLBindCol(*stmt, 3, SQL_C_CHAR, shippeddate, (SQLLEN) sizeof(shippeddate), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 3\n");


  /* Interfaz */
  query_orderRangeInterface(stmt, &ordernumber, orderdate, shippeddate);

  ret=SQLCloseCursor(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLCLOSECURSOR\n");

  if(fflush(out)!=0) printf("ERROR FFLUSH");

  return 0;
}

static void  query_orderRangeInterface(SQLHSTMT *stmt, SQLINTEGER *ordernumber, SQLCHAR *orderdate, SQLCHAR *shippeddate){
  int a=1;
  SQLRETURN ret;
  char t[]="    | Order\t| Date  \t| Shipped\n";

  while(SQL_SUCCEEDED(ret = SQLFetch(*stmt))) {
    if(a==1){
      printf("\n%s", t);
      printf(  "----+-----------+---------------+-----------------------\n");
    }

    if(a<10)
      printf(" 0%d | %d\t| %s\t| %s\t\n", a, *((int*) ordernumber), (char*) orderdate, (char*)shippeddate);
    else
      printf(" %d | %d\t| %s\t| %s\t\n", a, *((int*) ordernumber), (char*) orderdate, (char*)shippeddate);

      a++;
      if((a%10)==0){

        stop();
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n%s", t);
        printf(  "\n----+-----------+---------------+-----------------------\n");
      }
  }
}
/*
 *  ORDER DETAILS
 */
/*select
	o.ordernumber, o.orderdate, o.status, od.productcode, od.quantityordered, od.priceeach, od.quantityordered * od.priceeach as subtotal
from
	orders o join orderdetails od on o.ordernumber = od.ordernumber
where
	o.ordernumber = ?
group by
	o.ordernumber, od.productcode, od.quantityordered, od.priceeach, od.orderlinenumber
order by
	od.orderlinenumber */
int query_orderDetails(SQLHSTMT *stmt, FILE *out){
  SQLRETURN ret; /* ODBC API return status */
  SQLINTEGER ordernumber= (SQLINTEGER) 0, qord= (SQLINTEGER) 0;
  SQLDOUBLE price= (SQLDOUBLE) 0.0;
  SQLCHAR pcode[MY_CHAR_LEN], stat[MY_CHAR_LEN], orderdate[MY_CHAR_LEN];
  int odn;
  char query[]="select o.ordernumber, o.orderdate, o.status, od.productcode, od.quantityordered, od.priceeach from orders o join orderdetails od on o.ordernumber = od.ordernumber where o.ordernumber = ? group by o.ordernumber, od.productcode, od.quantityordered, od.priceeach, od.orderlinenumber order by od.orderlinenumber";

  if(!stmt || !out) return 1;


  if(fflush(out)!=0) printf("ERROR FFLUSH");
  if(scanf("%d", &odn)==EOF) printf("ERROR SCANF");
  printf("%d\n", odn);
  if(odn<1){
    printf("Order number given is not valid\n");
    return 0;
  }

  ret=SQLPrepare((*stmt), (SQLCHAR*) query, SQL_NTS);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLPREPARE %d\n", ret);

  ret=SQLBindParameter((*stmt), 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &odn, 0, NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDPARAMETER\n");

  ret=SQLExecute(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLEXECUTE\n");

  /* Asignamos a cada columna de resultados una variable */
  ret=SQLBindCol(*stmt, 1, SQL_INTEGER, &ordernumber, (SQLLEN) sizeof(ordernumber), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 1\n");
  ret=SQLBindCol(*stmt, 2, SQL_CHAR, orderdate, (SQLLEN) sizeof(orderdate), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 2\n");
  ret=SQLBindCol(*stmt, 3, SQL_CHAR, stat, (SQLLEN) sizeof(stat), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 3\n");
  ret=SQLBindCol(*stmt, 4, SQL_CHAR, pcode, (SQLLEN) sizeof(pcode), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 3\n");
  ret=SQLBindCol(*stmt, 5, SQL_INTEGER, &qord, (SQLLEN) sizeof(qord), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 1\n");
  ret=SQLBindCol(*stmt, 6, SQL_DOUBLE, &price, (SQLLEN) sizeof(price), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 1\n");


  query_orderDetailsInterface(stmt, &ordernumber, orderdate, stat, pcode, &qord, &price);

  ret=SQLCloseCursor(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLCLOSECURSOR\n");

  if(fflush(out)!=0) printf("ERROR FFLUSH");
}

static void query_orderDetailsInterface(SQLHSTMT *stmt, SQLINTEGER *odnum, SQLCHAR *oddate, SQLCHAR *st, SQLCHAR *pc, SQLINTEGER *q, SQLDOUBLE *price){
  int a=1;
  double total=0;
  SQLRETURN ret;
  char t[]="    | Product Code\t| Quantity \t| Price per Unit\n";

  /*Primero calculamos el total, sumando la columna subtotal*/
  while(SQL_SUCCEEDED(ret = SQLFetch(*stmt))){
    total += (((double) *q)) * ((double) *price);
    a++;
  }

  if(a==1){
    printf("\n < No order with number %d >\n\n", (int) *odnum);
    return;
  }

  a=1;

  while(SQL_SUCCEEDED(ret = SQLFetch(*stmt))) {
    if(a==1){
      printf("\nOrder Date: %s  --- Status: %s\n", (char *)oddate, (char *)st);
      printf("Total sum = %lf\n", total);
      printf("\n%s", t);
      printf(  "----+-----------------------+-----------------------+-----------------------\n");
    }
    if(a<10){
      printf(" 0%d | %s\t| %lf\t| %lf\t\n", a,(char*) pc , *((double*) q), *((double *) price));
    }else{
      printf(" 0%d | %s\t| %lf\t| %lf\t\n", a,(char*) pc , *((double*) q), *((double *) price));
    }
    a++;
    if((a%10)==0){
      stop();
      printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n%s", t);
      printf(  "----+-----------+-----------------------+-----------------------+---------------------\n");
    }
  }

  printf("\n");
  if(a==1) printf("\n < No order with number %d >\n\n", *((int *) odnum));
}
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

void query_customersFindInterface(SQLHSTMT *stmt, SQLCHAR *cnum, SQLCHAR *cname, SQLCHAR *cfn, SQLCHAR *csn, char *string){
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
  if(a==1) printf("\n < No customer named \'%s\' >\n\n",string);

  /*stop();*/

  return;
}
/*
 * CUSTOMERS LISTPRODUCTS
 */

/*select p.productname, sum(od.quantityordered) from customers c join orders o on c.customernumber=o.customernumber join orderdetails od on o.ordernumber=od.ordernumber join products p on od.productcode=p.productcode where c.customernumber=? group by p.productcode, p.productname order by p.productcode;*/
int query_customersListProducts(SQLHSTMT *stmt, FILE *out){
  SQLRETURN ret; /* ODBC API return status */
  char pname[MY_CHAR_LEN]= "\n";
  char query[]="select p.productname, sum(od.quantityordered) from customers c join orders o on c.customernumber=o.customernumber join orderdetails od on o.ordernumber=od.ordernumber join products p on od.productcode=p.productcode where c.customernumber=? group by p.productcode, p.productname order by p.productcode";
  SQLINTEGER cnumber= (SQLINTEGER) 0, qordered= (SQLINTEGER) 0;


  if(!stmt || !out) return 1;


  if(fflush(out)!=0) printf("ERROR FFLUSH");
  if(scanf("%d", &cnumber)==EOF) printf("ERROR SCANF");


  ret=SQLPrepare((*stmt), (SQLCHAR*) query, SQL_NTS);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLPREPARE %d\n", ret);


  ret=SQLBindParameter((*stmt), 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &cnumber, 0, NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDPARAMETER\n");

  /* Ejecuta la query */
  ret=SQLExecute(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLEXECUTE\n");

  /* Asigna la columna resultado a la variable y */
  ret=SQLBindCol(*stmt, 1, SQL_CHAR, pname, (SQLLEN) sizeof(pname), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 1\n");
  /* Asigna la columna resultado a la variable y */
  ret=SQLBindCol(*stmt, 2, SQL_INTEGER, &qordered, (SQLLEN) sizeof(qordered), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 2\n");


  /* Interfaz */
  query_customersListProductsInterface(stmt, cnumber, (SQLCHAR*)pname, &qordered);

  ret=SQLCloseCursor(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLCLOSECURSOR\n");

  if(fflush(out)!=0) printf("ERROR FFLUSH");

  return 0;
}

void query_customersListProductsInterface(SQLHSTMT *stmt, SQLINTEGER cnum, SQLCHAR *pname, SQLINTEGER *qordered){
  SQLRETURN ret;
  int a=1;
  char titulo[]="    | Product Name \t| Quantity Ordered\n";

  if(!stmt||!pname||!qordered){
    printf("INTERFACE FAILURE\n");
    return;
  }

  while(SQL_SUCCEEDED(ret = SQLFetch(*stmt))) {
    if(a==1){
      printf("\n%s", titulo);
      printf(  "----+-----------+-----------------------+-----------------------+---------------------\n");
    }

    if(a<10)
      printf(" 0%d | %s %d\n", a, (char*) pname, (int) *qordered);
    else
      printf(" %d | %s %d\n", a, (char*) pname, (int) *qordered);

      a++;
      /*if((a%10)==0){
        stop();
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n%s", titulo);
        printf(  "----+-----------+-----------------------+-----------------------+---------------------\n");
      }*/
  }
  printf("\n");
  if(a==1) printf("\n < No product ordered by the customer with the code \'%d\' >\n\n",(int) cnum);

  /*stop();*/

  return;
}


/*
 * CUSTOMERS BALANCE
 */

/*
select pay.suma-s.saldo as balance
from (select sum(pm.amount) as suma
  from payments pm
  where pm.customernumber=141
  group by pm.customernumber) as pay,
  (select sum(od.quantityordered*od.priceeach) as saldo
  from customers c
  join orders o on c.customernumber=o.customernumber
  join orderdetails od on o.ordernumber=od.ordernumber
  where c.customernumber=141
  group by c.customernumber) as s;
  */
int query_customersBalance(SQLHSTMT *stmt, FILE *out){
  SQLRETURN ret; /* ODBC API return status */
  char query[]="select pay.suma-s.saldo as balance from (select sum(pm.amount) as suma from payments pm where pm.customernumber=? group by pm.customernumber) as pay, (select sum(od.quantityordered*od.priceeach) as saldo from customers c join orders o on c.customernumber=o.customernumber join orderdetails od on o.ordernumber=od.ordernumber where c.customernumber=? group by c.customernumber) as s";
  SQLINTEGER cnumber= (SQLINTEGER) 0;
  SQLDOUBLE saldo= (SQLDOUBLE) 0.0;


  if(!stmt || !out) return 1;


  if(fflush(out)!=0) printf("ERROR FFLUSH");
  if(scanf("%d", &cnumber)==EOF) printf("ERROR SCANF");


  ret=SQLPrepare((*stmt), (SQLCHAR*) query, SQL_NTS);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLPREPARE %d\n", ret);


  ret=SQLBindParameter((*stmt), 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &cnumber, 0, NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDPARAMETER\n");
  ret=SQLBindParameter((*stmt), 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &cnumber, 0, NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDPARAMETER\n");

  /* Ejecuta la query */
  ret=SQLExecute(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLEXECUTE\n");

  /* Asigna la columna resultado a la variable y */
  ret=SQLBindCol(*stmt, 1, SQL_DOUBLE, &saldo, (SQLLEN) sizeof(saldo), NULL);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLBINDCOL 2\n");


  /* Interfaz */
  query_customersBalanceInterface(stmt, cnumber, &saldo);

  ret=SQLCloseCursor(*stmt);
  if(!SQL_SUCCEEDED(ret)) printf("ERROR SQLCLOSECURSOR\n");

  if(fflush(out)!=0) printf("ERROR FFLUSH");

  return 0;
}

void query_customersBalanceInterface(SQLHSTMT *stmt, SQLINTEGER cnum, SQLDOUBLE *saldo){
  SQLRETURN ret;

  if(!stmt||!saldo){
    printf("INTERFACE FAILURE\n");
    return;
  }

  if(SQL_SUCCEEDED(ret = SQLFetch(*stmt))) {

    printf("\n\n   Balance = %.2lf\n\n", (double) *saldo);

  }else{
    printf("\n\n < No customer with code \'%d\' >\n\n",(int) cnum);
  }

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
