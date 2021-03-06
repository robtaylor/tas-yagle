/******************************************/
/* lib_global.h                           */
/******************************************/
/******************************************************************************/
/* INCLUDE                                                                    */
/******************************************************************************/
#ifndef LIB_GLOBAL_H
#define LIB_GLOBAL_H


#include "lib_util.h"
#include "lib_func.h"
#include "lib_timing.h"


/******************************************************************************/
/* DEFINE                                                                     */
/******************************************************************************/

/******************************************************************************/
/* STRUCTURES                                                                 */
/******************************************************************************/

/******************************************************************************/
/* GLOBALS                                                                    */
/******************************************************************************/

/******************************************************************************/
/* EXTERNS                                                                    */
/******************************************************************************/

/******************************************************************************/
/* FONCTIONS                                                                  */
/******************************************************************************/
extern  void    lib_treat_group_head(char *ident, chain_list *param);
extern  void    lib_treat_group_end(void);
extern  void    lib_treat_s_attr(char *ident, char *attr);
extern  void    lib_treat_c_attr(char *ident, chain_list *attr);

#endif

