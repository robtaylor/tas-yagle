/******************************************************************************/
/*                                                                            */
/*                      Chaine de CAO & VLSI   AVERTEC                        */
/*                                                                            */
/*    Produit : LIB Version 1.00                                              */
/*    Fichier : lib.y                                                         */
/*                                                                            */
/*    (c) copyright 2000 AVERTEC                                              */
/*    Tous droits reserves                                                    */
/*                                                                            */
/*    Auteur(s) : Gilles Augustins                                            */
/*                                                                            */
/******************************************************************************/

%{
#include "tlfarea_parse.h" 

/******************************************************************************/
/* private                                                                    */
/******************************************************************************/

/******************************************************************************/
/* function declarations                                                      */
/******************************************************************************/

int yyerror();
int yylex();

%}

%union {
  char           *text ;
  float           real ;
} ;


%start tlf_file 

%%

tlf_file     : empty 
             ;

empty        : /* empty */ 
             ;

%%

int yyerror ()
{
	EXIT (EXIT_FAILURE) ;
}

