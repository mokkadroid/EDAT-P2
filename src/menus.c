

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
static void menus_changeScreen(FILE *out);
static void menus_input(char *c);
static void menus_inputError(FILE *out);

/*Submenus*/
static void menus_generalPrint(FILE *out);
static void menus_productsPrint(FILE *out);
static void menus_ordersPrint(FILE *out);
static void menus_customersPrint(FILE *out);


static int menus_products(SQLHSTMT *stmt, FILE *out);
static int menus_orders(SQLHSTMT *stmt, FILE *out);
static int menus_customers(SQLHSTMT *stmt, FILE *out);
static void menus_exit(FILE *out);


/*Products*/
static int menus_productsStock(SQLHSTMT *stmt, FILE *out);
static int menus_productsFind(SQLHSTMT *stmt, FILE *out);







void menus_input(char *c){
  int er=0;
  if(!c) return;

  er=fseek(stdin,0,SEEK_END); 
  er+=system("stty -echo"); 
  er+=system("/bin/stty raw");
  (*c) = getchar();
  er+=system("/bin/stty cooked"); 
  er+=system("stty echo");

  return;
}







int menus_general(SQLHSTMT *stmt, FILE *out){
  char c=' ';
  int end=0;


  if(!out || !stmt) return 1;

  while(end==0){
    menus_generalPrint(out);

    menus_input(&c);

    if(c=='1'){
      if(menus_products(stmt, out)!=0){
         printf("\n > An error occurred while loading the products menu. < \n");
      }
    }else if(c=='2'){
      if(menus_orders(stmt, out)!=0){
         printf("\n > An error occurred while loading the orders menu. < \n");
      }
    }else if(c=='3'){
      if(menus_customers(stmt, out)!=0){
         printf("\n > An error occurred while loading the customers menu. < \n");
      }
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
  char c=' ';
  int end=0;

  if(!out || !stmt) return 1;

  while(end==0){
    menus_productsPrint(out);

    menus_input(&c);

    if(c=='1'){
      if(menus_productsStock(stmt, out)!=0){
         printf("\n > An error occurred while loading the products stock. < \n");
      }
    }else if(c=='2'){
      if(menus_productsFind(stmt, out)!=0){
         printf("\n > An error occurred while loading the products menu. < \n");
      }
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
  if(!out||!stmt) return 1;
  menus_ordersPrint(out);
  return 0;
}
int menus_customers(SQLHSTMT *stmt, FILE *out){
  if(!out||!stmt) return 1;
  menus_customersPrint(out);
  return 0;
}

void menus_exit(FILE *out){
  if(!out) return;
  fprintf(out, "\nMENU Exit\n");
  return;
}









/* Products options */
int menus_productsStock(SQLHSTMT *stmt, FILE *out){

  printf("\nEnter productcode > ");
  if(query_productStock(stmt, out)!=0) return 1;

  return 0;
}
int menus_productsFind(SQLHSTMT *stmt, FILE *out){

  printf("\nEnter productname > ");
  if(query_productFind(stmt, out)!=0) return 1;

  return 0;
}


/* Orders options */

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
  fprintf(out, "Enter a number that corresponds to your choice > ");

  return;
}

void menus_productsPrint(FILE *out){
  menus_changeScreen(out);

  fprintf(out, "\n\n ##########################\n");
  fprintf(out, " # Choose products option #\n");
  fprintf(out, " ##########################\n");
  fprintf(out, "\n\t1. Stock.\n\t2. Find.\n\t3. Back.\n\n");
  fprintf(out, "Enter a number that corresponds to your choice > ");

  return;
}

void menus_ordersPrint(FILE *out){
  menus_changeScreen(out);

  fprintf(out, "\n\n ########################\n");
  fprintf(out, " # Choose orders option #\n");
  fprintf(out, " ########################\n");
  fprintf(out, "\n\t1. Open.\n\t2. Range.\n\t3. Detail.\n\t4. Back.\n\n");
  fprintf(out, "Enter a number that corresponds to your choice > ");

  return;
}

void menus_customersPrint(FILE *out){
  menus_changeScreen(out);

  fprintf(out, "\n\n ##########################\n");
  fprintf(out, " # Choose customer option #\n");
  fprintf(out, " ##########################\n");
  fprintf(out, "\n\t1. Find.\n\t2. List Products.\n\t3. Balance.\n\t4. Back\n\n");
  fprintf(out, "Enter a number that corresponds to your choice > ");

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
