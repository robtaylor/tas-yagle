
/* ###--------------------------------------------------------------### */
/* file		: beh_makquad.c						*/
/* date		: Mar  23 1995						*/
/* version	: v109							*/
/* authors	: Laurent VUILLEMIN					*/
/* description	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "bhl_lib.h"

/* ###--------------------------------------------------------------### */
/* function     : beh_makquad						*/
/* description  : add a BEQUAD structure for each expression		*/
/* called func. : beh_addbequad	 					*/
/* ###--------------------------------------------------------------### */

void beh_makquad(befig_list *pt_befig)
  {
  struct beaux  *pt_beaux;
  struct beout  *pt_beout;
  struct bebus  *pt_bebus;
  struct bebux  *pt_bebux;
  struct bereg  *pt_bereg;
  struct bemsg  *pt_bemsg;
  struct binode *pt_binod;

	/* ###------------------------------------------------------### */
	/*   simple output ports					*/
	/* ###------------------------------------------------------### */

  pt_beout = pt_befig->BEOUT;
  while (pt_beout != NULL)
     {
     pt_beout->NODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);
     pt_beout       = pt_beout->NEXT;
     }

	/* ###------------------------------------------------------### */
	/*   bussed ouput ports						*/
	/* ###------------------------------------------------------### */

  pt_bebus = pt_befig->BEBUS;
  while (pt_bebus != NULL)
     {
     pt_binod = pt_bebus->BINODE;
     while (pt_binod != NULL)
       {
       pt_binod->CNDNODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);
       pt_binod->VALNODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);

       pt_binod          = pt_binod->NEXT;
       }
     pt_bebus = pt_bebus->NEXT;
     }

	/* ###------------------------------------------------------### */
	/*   internal bussed signals					*/
	/* ###------------------------------------------------------### */

  pt_bebux = pt_befig->BEBUX;
  while (pt_bebux != NULL)
     {
     pt_binod = pt_bebux->BINODE;
     while (pt_binod != NULL)
       {
       pt_binod->CNDNODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);
       pt_binod->VALNODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);

       pt_binod          = pt_binod->NEXT;
       }
     pt_bebux = pt_bebux->NEXT;
     }

	/* ###------------------------------------------------------### */
	/*   internal registers						*/
	/* ###------------------------------------------------------### */

  pt_bereg = pt_befig->BEREG;
  while (pt_bereg != NULL)
     {
     pt_binod = pt_bereg->BINODE;
     while (pt_binod != NULL)
       {
       pt_binod->CNDNODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);
       pt_binod->VALNODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);

       pt_binod          = pt_binod->NEXT;
       }
     pt_bereg = pt_bereg->NEXT;
     }

	/* ###------------------------------------------------------### */
	/*   internal simple signals					*/
	/* ###------------------------------------------------------### */
    
  pt_beaux = pt_befig->BEAUX;
  while (pt_beaux != NULL)
     {
     pt_beaux->NODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);
     pt_beaux       = pt_beaux->NEXT;
     }

	/* ###------------------------------------------------------### */
	/*   internal delayed signals					*/
	/* ###------------------------------------------------------### */

  pt_beaux = pt_befig->BEDLY;
  while (pt_beaux != NULL)
     {
     pt_beaux->NODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);
     pt_beaux       = pt_beaux->NEXT;
     }

	/* ###------------------------------------------------------### */
	/*   assertions							*/
	/* ###------------------------------------------------------### */

  pt_bemsg = pt_befig->BEMSG;
  while (pt_bemsg != NULL)
     {
     pt_bemsg->NODE = (pNode) beh_addbequad (NULL,NULL,NULL,NULL);
     pt_bemsg       = pt_bemsg->NEXT;
     }

  pt_befig->TYPE = pt_befig->TYPE | BEH_NODE_QUAD;
  }
