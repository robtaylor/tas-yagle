/****************************************************************************/
/*                                                                          */
/*                      Chaine de CAO & VLSI   AVERTEC                      */
/*                                                                          */
/*    Produit : STM Version 1.00                                            */
/*    Fichier : stm_noise_scr_print.c                                       */
/*                                                                          */
/*    (c) copyright 2000 AVERTEC                                            */
/*    Tous droits reserves                                                  */
/*                                                                          */
/*    Auteur(s) : Gr�goire Avot                                             */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*     includes                                                             */
/****************************************************************************/

#include "stm.h" 

/****************************************************************************/
/*     functions                                                            */
/****************************************************************************/

void stm_noise_scr_print( FILE *f, noise_scr *model )
{
  fprintf( f, "\t\tnoise_scr (%.5g %.5g)\n", model->R, model->INVTH );
}
