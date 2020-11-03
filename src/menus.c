

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "querys.h"
#include "odbc.h"
#include "menus.h"


#define MAX_CHAR 255


/*Private functions*/
void menus_changeScreen(FILE *out);
void menus_input(char *c);
void menus_inputError(FILE *out);

/*Submenus*/
void menus_generalPrint(FILE *out);
void menus_productsPrint(FILE *out);
void menus_ordersPrint(FILE *out);
void menus_customersPrint(FILE *out);


int menus_products(SQLHSTMT *stmt, FILE *out);
int menus_orders(SQLHSTMT *stmt, FILE *out);
int menus_customers(SQLHSTMT *stmt, FILE *out);
void menus_exit(FILE *out);


/*Products*/
int menus_productsStock(SQLHSTMT *stmt, FILE *out);
int menus_productsFind(SQLHSTMT *stmt, FILE *out);







void menus_input(char *c){

  if(!c) return;

  fseek(stdin,0,SEEK_END);
  system("stty -echo");
  system("/bin/stty raw");
  (*c) = getchar();
  system("/bin/stty cooked");
  system("stty echo");

  return;
}







int menus_general(SQLHSTMT *stmt, FILE *out){
  char c;
  int end=0;

  if(!out) return 1;

  while(!end){
    menus_generalPrint(out);

    menus_input(&c);

    if(c=='1'){
      menus_products(stmt, out);
    }else if(c=='2'){
      menus_orders(stmt, out);
    }else if(c=='3'){
      menus_customers(stmt, out);
    }else if(c=='4'){
      end=1;
      menus_exit(out);
    }else{
      menus_inputError(out);
    }

  }


  return 0;
}






/* General submenus */

int menus_products(SQLHSTMT *stmt, FILE *out){
  char c;
  int end=0;

  if(!out) return 1;

  while(!end){
    menus_productsPrint(out);

    menus_input(&c);

    if(c=='1'){
      menus_productsStock(stmt, out);
    }else if(c=='2'){
      menus_productsFind(stmt, out);
    }else if(c=='3'){
      end=1;
      menus_exit(out);
    }else{
      menus_inputError(out);
    }

  }

  return 0;
}




int menus_orders(SQLHSTMT *stmt, FILE *out){
  menus_ordersPrint(out);
  return 0;
}
int menus_customers(SQLHSTMT *stmt, FILE *out){
  menus_customersPrint(out);
  return 0;
}

void menus_exit(FILE *out){
  fprintf(out, "\nMENU Exit\n");
  return;
}









/* Products options */
int menus_productsStock(SQLHSTMT *stmt, FILE *out){

  printf(" > Insert product code ");
  query_productStock(stmt, out);

  return 0;
}
int menus_productsFind(SQLHSTMT *stmt, FILE *out){

  printf(" > Insert product name ");
  query_productFind(stmt, out);

  return 0;
}


/* Orders options* /

/* Customers options */













/***********************
 *                     *
 *  Menus' interfaces  *
 *                     *
 ***********************/

void menus_generalPrint(FILE *out){
  menus_changeScreen(out);

  fprintf(out, "\n\n ################################\n");
  fprintf(out, " # Choose what you want to show #\n");
  fprintf(out, " ################################\n");
  fprintf(out, "\n\t1. Products.\n\t2. Orders.\n\t3. Customers.\n\t4. Exit.\n\n");

  return;
}

void menus_productsPrint(FILE *out){
  menus_changeScreen(out);

  fprintf(out, "\n\n ##########################\n");
  fprintf(out, " # Choose products option #\n");
  fprintf(out, " ##########################\n");
  fprintf(out, "\n\t1. Stock.\n\t2. Find.\n\t3. Back.\n\n");

  return;
}

void menus_ordersPrint(FILE *out){
  menus_changeScreen(out);

  fprintf(out, "\n\n ########################\n");
  fprintf(out, " # Choose orders option #\n");
  fprintf(out, " ########################\n");
  fprintf(out, "\n\t1. Open.\n\t2. Range.\n\t3. Detail.\n\t4. Back.\n\n");

  return;
}

void menus_customersPrint(FILE *out){
  menus_changeScreen(out);

  fprintf(out, "\n\n ##########################\n");
  fprintf(out, " # Choose customer option #\n");
  fprintf(out, " ##########################\n");
  fprintf(out, "\n\t1. Find.\n\t2. List Products.\n\t3. Balance.\n\t4. Back\n\n");

  return;
}



/* General Purpose */

void menus_inputError(FILE *out){
  fprintf(out, "\n\n > Please enter a valid option < \n\n");
  return;
}

void menus_changeScreen(FILE *out){
  fprintf(out, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  return;
}
