/****************************************************************************/
/*                                                                          */
/*                      Chaine de CAO & VLSI   AVERTEC                      */
/*                                                                          */
/*    Produit : AVERTEC global tools                                        */
/*    Fichier : sim_message.c                                               */
/*                                                                          */
/*    © copyright 2000 AVERTEC                                              */
/*    Tous droits reserves                                                  */
/*                                                                          */
/*    Auteur(s) : Antony PINTO                                              */
/*                                                                          */
/****************************************************************************/

#include <stdlib.h>
#include "sim_message.h"

/****************************************************************************/
/*{{{                                                                       */
/****************************************************************************/
/*{{{                    sim_war()                                          */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void sim_war(char *text, ... )
{
  va_list    ap;
  
  va_start(ap, text);
  (void) fprintf(stderr, "[SIM_WAR] : ");
  (void) vfprintf(stderr, text, ap);
  (void) fprintf(stderr, "\n");
  va_end(ap);
}

/*}}}************************************************************************/
/*{{{                    sim_err()                                          */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void sim_err(char *text, ... )
{
  va_list    ap;
  
  va_start(ap, text);
  (void) fprintf(stderr, "[SIM_ERR] : ");
  (void) vfprintf(stderr, text, ap);
  (void) fprintf(stderr, "\n");
  va_end(ap);

  EXIT(1);
}

/*}}}************************************************************************/
/*}}}************************************************************************/
