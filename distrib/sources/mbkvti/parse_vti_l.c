/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*******************************************************************************
* mbk : vti parser netlist view : hns & fne formats v4r11                      *
*                                                                              *
* Based upon material provided by Alain Greiner in 1990, in lex and yacc       *
*                                                                              *
* version 0.0 written by Hichang Li in june 1991 under the supervision of      *
*             Frederic Petrot                                                  *
* version n.x is a rewitting and update of version 0.0 written by Frederic     *
*             Petrot on september 1991                                         *
*                                                                              *
* New versions are mostly bug corrections, or standardization matters and are  *
* done by Frederic Petrot, since middle 1990.                                  *
*                                                                              *
* Rc nets support done by Gregoire Avot on April 1997.                         *
*                                                                              *
*                                                                              *
* version : 4.12                                                               *
* date    : 12/06/1998                                                         *
*******************************************************************************/

#ident "@(#)vti logical views parser version 4.04"

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include MUT_H
#include MLO_H
#include MLU_H
#include RCN_H

#define LSIZE 16384 /* max line size for vfgets */

enum {SYNTAX_ERROR, NET_ERROR, OPEN_ERROR, PARSE_ERROR, CLOSE_ERROR,
		NAME_ERROR, CON_SIGDIF, BAD_JOIN};

/* rcnet support */
struct hns_X {
	struct hns_X *NEXT;
	long idx;
	long rcn;
} ;

struct hns_I {
	struct hns_I *NEXT;
	long idx;
	loins_list *ins;
	long rcn;
} ;

struct hns_P {
	struct hns_P *NEXT;
	long idx;
	losig_list *sig;
	long rcn;
} ;

/* flag :
	 1 : node1 = hns_X
	 2 : node1 = hns_I
	 4 : node1 = hns_P
	 8 : node2 = hns_X
	16 : node2 = hns_I
	32 : node2 = hns_P
*/
struct hns_W {
	struct hns_W *NEXT;
	long flag;
	void *node1;
	void *node2;
	float r;
	float c;
} ;

struct hns_X	*HNS_X=NULL;
struct hns_I	*HNS_I=NULL;
struct hns_P	*HNS_P=NULL;
struct hns_W	*HNS_W=NULL;
chain_list	*FREE_HNS=NULL;

#define HNSRCN         53432
#define HNSRCNIDX      53433
#define HNSPHCON       53434
#define HNSRCNX        53435
#define HNSRCNI        53436

static void		buildrcn __P(( lofig_list* ));
static struct hns_I*	givehnsi __P(( lofig_list*, int, loins_list* ));
static struct hns_X*	givehnsx __P(( lofig_list*, int ));
static struct hns_P*	givehnsp __P(( losig_list*, int ));
static struct hns_I*	add_hns_I __P(( void ));
static struct hns_X*	add_hns_X __P(( void ));
static struct hns_P*	add_hns_P __P(( void ));
static struct hns_W*	add_hns_W __P(( void ));
static void		del_hns_X __P(( struct hns_X* ));
/* static void		del_hns_P __P(( struct hns_P* )); */
static void		del_hns_I __P(( struct hns_I* ));
static void		del_hns_W __P(( struct hns_W* ));
/* static void		movehnsw __P(( losig_list*, losig_list* )); */
static float	stof __P(( char* ));
static losig_list* givelosigalias __P(( lofig_list*, chain_list*, int ));
static int    compare_alias_join __P(( lofig_list*, chain_list*, chain_list** ));
static int    traite_join __P(( lofig_list*, chain_list* ));
static void   msg_err __P(( lofig_list*, chain_list*  ));
/* static void   debug __P(( lofig_list* )); */


/*******************************************************************************
* hns_error : generic error routine for vtiloadlofig function                  *
*******************************************************************************/
static void hns_error(type, name, line, string, parameter)
int type;
long line, parameter;
char *name, *string;
{
	(void)fflush(stdout);
	(void)fprintf(stderr, "*** mbk error ***\n");
	switch (type) {
		case SYNTAX_ERROR :
			(void)fprintf(stderr, "syntax error");
			break;
		case NET_ERROR :
			(void)fprintf(stderr, "external connector %s on signal %ld",
								string, parameter);
			(void)fprintf(stderr, " is internally joined with an other");
			(void)fprintf(stderr, " external connector with an other name");
			break;
		case OPEN_ERROR :
			(void)fprintf(stderr, "can't open file : %s", name);
			break;
		case PARSE_ERROR :
			(void)fprintf(stderr, "abnormal parsing for : %s", name);
			break;
		case CLOSE_ERROR :
			(void)fprintf(stderr, "can't close file : %s", name);
			break;
		case NAME_ERROR :
			(void)fprintf(stderr, "name syntax error");
			break;
		case CON_SIGDIF :
			(void)fprintf(stderr, "two connectors with the same name on two different signals");
			break;
		case BAD_JOIN :
			(void)fprintf(stderr, "join of signal %ld connected on two different external connectors %s",parameter,string);
			break;
	}
	if (line)
		(void)fprintf(stderr, "\nvtiloadlofig : parsing file %s.%s line %ld\n",
						name, IN_LO, line);
	else
		(void)fprintf(stderr, "\nvtiloadlofig : parsing file %s.%s\n",
						name, IN_LO);
	EXIT(1);
}

/*******************************************************************************
* vfgets: read a line of a file to a buffer, the line beeing size max or end   *
* with a \n. In order to properly treat vti line breaks, the -\n pattern is    *
* recognized and ignored                                                       *
*******************************************************************************/
static char *vfgets(str, size, iob)
char *str;
int size;
FILE *iob;
{
register char *cs;
register int c=0;

	cs = str;
	while (--size > 0 && (c = getc(iob)) != EOF) {
		if ((*(cs++) = c) == '-') {
			if ((c = getc(iob)) == '\n') {
        /* avant : *cs-- */
				cs--;
			} else
				(void)ungetc(c ,iob);
		} else if (c == '\n')
			break;
	}
	*cs = '\0';
	return (c == EOF && cs == str) ? (char *)NULL : str;
}

/*******************************************************************************
* shift : shift a float 3 times to give a long in ascii                        *
*******************************************************************************/
#define shift(pt) \
	if (*(pt + 1) == '\0') { \
		*pt = '0'; \
		*(pt + 1) = '0'; \
		*(pt + 2) = '0'; \
		*(pt + 3) = '\0'; \
	} else if (*(pt + 2) == '\0') { \
		*pt = *(pt + 1); \
		*(pt + 1) = '0'; \
		*(pt + 2) = '0'; \
		*(pt + 3) = '\0'; \
	} else if (*(pt + 3) == '\0') { \
		*pt = *(pt + 1); \
		*(pt + 1) = *(pt + 2); \
		*(pt + 2) = '0'; \
		*(pt + 3) = '\0'; \
	} else { \
		*pt = *(pt + 1); \
		*(pt + 1) = *(pt + 2); \
		*(pt + 2) = *(pt + 3); \
		*(pt + 3) = '\0'; \
	}

/*******************************************************************************
* stol : convert a string to a long                                            *
*******************************************************************************/
static long	stol(token)
char	*token;
{
register char *pt;

	if ((pt = strchr(token, '.')) == (char *)NULL)
		return (long)(SCALE_X * atol(token));
	else {
		shift(pt);
		return (long)(SCALE_X * atol(token) / 1000);
	}
}

/*******************************************************************************
* stof : convert a string to a float                                           *
*******************************************************************************/
static float stof(token)
char	*token;
{
	char *end;
	double f;
	f = strtod( token, &end );
	if( end == token )
		hns_error((int)SYNTAX_ERROR, NULL, (long)0, (char *)NULL, 0L);
	return( (float)(f) );
}


/*******************************************************************************
* checkname : check for busses                                                 *
*******************************************************************************/
static char *checkname(name)
char *name;
{
char *s, *t;

   s = t = name;
   while (*t) {
      if (*t == '[')
         *t = ' ';
      else if (*t == ']')
      {
         if (*(++t) == '\0') /* ok, it's finished */
				goto end;
         else if (*t == '[') /* multiple array */
            continue;
      }
      *s++ = *t++;
   }
end:
	*s = '\0';
	return name;
}

/*******************************************************************************
* function env()                                                               *
*******************************************************************************/
static void env(tn, tp)
char *tn, *tp;
{
char *s, *avt_gethashvar();

	s = getenv("MBK_VTI_TN");
	*tn = s ? *s : 'e';
	if (islower((int)*tn))
		*tn = toupper(*tn);
	s = getenv("MBK_VTI_TP");
	*tp = s ? *s : 'p';
	if (islower((int)*tp))
		*tp = toupper(*tp);
}


/*******************************************************************************
*        logical parser : vti2mbk                                              *
*******************************************************************************/
static int parse(fig, mode, in, fname)
lofig_list *fig;
char mode;
FILE *in;
char *fname;
{
char line[LSIZE+1];
char *t1, *t2, *t3, *t4, *t5;             /* pointers to token on a line */
char *t6, *t7, *t8, *t9, *t10, *t11;      /* of vti file */
char s[100];  /* intermediate string to process tokens */
long l1, l2, l3, l4;  /* results from strtol */
float f; /* only used for capacitances */
lofig_list *model = (lofig_list *)NULL; /* must be initialized */
num_list *ylist = (num_list *)NULL; /* must be initialized */
long i = 1L; /* line number */
static char tn, tp;
losig_list *cursig = NULL; /* current signal for wire */
ptype_list *ptl;
chain_list *head_join;
chain_list *head_sigalias;
losig_list      *sigct1,*sigct2;

	head_join = head_sigalias = NULL;

	if (!tn) /* or tp, who cares? */
		env(&tn, &tp);

	while (1) {
		i++;
		if (vfgets(line, LSIZE, in) == (char *)NULL)
			hns_error((int)SYNTAX_ERROR, fname, (long)i, (char *)NULL, 0L);
		switch (line[0]) {
			case '#' : /* comment */
			case '\{' : /* comment */
			case ' ' : /* empty line in vti file */
			case ';' : /* empty line in vti file */
			case 'A' :
			case 'B' :
				break;
			case 'C' : /* C INT NOP NOP cap INT INT ; */
				if (mode != 'P') {

					t1 = strtok(line + 2, " \n\t\"");
					l1 = atol(t1);
					t2 = strtok((char *)NULL, " \n\t\"");
					t3 = strtok((char *)NULL, " \n\t\"");
					t4 = strtok((char *)NULL, " \n\t\"");
					(void)strcpy(s, t4);
					f = atof(s);
					t5 = strtok((char *)NULL, " \n\t\"");
					l3 = atol(t5);
					t6 = strtok((char *)NULL, " \n\t\"");
					l4 = atol(t6);
					/* capacitance :
					   units are picofarads inside vti logical formats.
					   add capacitance on both signals.
						0 is a linear, wire, capacitance,
						1 is a grid capacitance,
					   2-5 are sidewall and surface diffusion capacitances
					   6 is diaphonic capacitances. */
					if (l1 != 1) {

            sigct1 = givelosigalias(fig, head_sigalias, (long)l3);
            sigct2 = givelosigalias(fig, head_sigalias, (long)l4);
            
            if( !sigct1->PRCN )
              addlorcnet( sigct1 );
            
            if( !sigct2->PRCN )
              addlorcnet( sigct2 );
            
            addloctc( sigct1, 1, sigct2, 1, f );
            
            rcn_addcapa( sigct1, f );
            rcn_addcapa( sigct2, f );
					}
				}
				break;
			case 'E': /* E ; */
				{
				locon_list	*c1;
				ptype_list	*ptl;
				num_list	*pn;
				lofig_list	*ptmodel;
				struct hns_X	*effx;
				struct hns_X	*effscanx;
				chain_list	*scan_head_join;
                                losig_list      *ptsig;
                                locon_list      *ptcon;

				lofigchain( fig );

				for (c1 = fig->LOCON; c1; c1 = c1->NEXT) {
					ptl = getptype(c1->USER,HNSRCN);
					ptl->DATA = reverse( ptl->DATA );
				}

				if( !compare_alias_join( fig, head_sigalias, &head_join ) ) {
					hns_error((int)CON_SIGDIF, fname, (long)i, (char *)NULL, 0L);
				}

				if( !traite_join( fig, head_join ) ) {
					hns_error((int)SYNTAX_ERROR, fname, (long)i, (char *)NULL, 0L);
				}

				/* Comme la fonction traite_join peut avoir deplacer des connecteurs ( passe d'un signal a un autre ), on refait un lofigchain
				   pour tout remettre a jour. Le lofigchain fait un nettoyage prealable, donc pas de problemes... */

				lofigchain(fig);

				/* On nettoie les join */
				for( scan_head_join = head_join; scan_head_join; scan_head_join = scan_head_join->NEXT ) {
					freechain( (chain_list*)(scan_head_join->DATA) );
				}
				freechain( head_join );

				/* destroy the trhu routes connectors and signals */
				for (pn = ylist; pn; pn = pn->NEXT)
                                {
                                  ptsig = getlosig( fig, pn->DATA );
                                  ptl = getptype( ptsig->USER, LOFIGCHAIN );
                                  if( !ptl->DATA )
                                    dellosig( fig, pn->DATA );
                                  else
                                  {
                                    ptcon = (locon_list*)(((chain_list*)(ptl->DATA))->DATA);
                                    if( !((chain_list*)(ptl->DATA))->NEXT && ptcon->ROOT == fig )
                                    {
			              if( (ptl = getptype( ptcon->USER, HNSRCN )) )
                                      {
				        freechain( (chain_list*)(ptl->DATA) );
				        ptcon->USER = delptype( ptcon->USER, HNSRCN );
                                      }
                                      dellocon( fig, ptcon->NAME );
                                      /*
                                      for (delete = &fig->LOCON; *delete != NULL; )
					if ((*delete)->SIG->INDEX == pn->DATA)
                                        {
					  c1 = (*delete)->NEXT;
					  if( (ptl = getptype( (*delete)->USER,HNSRCN )))
                                          {
					    freechain( (chain_list*)(ptl->DATA));
					    (*delete)->USER = delptype( (*delete)->USER, HNSRCN );
					  }
					  mbkfree((void *)*delete);
					  *delete = c1;
					}
                                        else
 					  delete = &(*delete)->NEXT;
                                      */

                                      dellosig( fig, pn->DATA );
                                    }
                                    else
                                    {
                                      fprintf( stderr, "*** mbkvti warning : Can't delete thru routes %ld.\n", ptsig->INDEX );
                                    }
                                  }
                                }

                                freenum(ylist);
                                 
				buildrcn(fig);

				for( ptmodel =  model ; ptmodel; ptmodel = ptmodel->NEXT ) {
					freechain( (chain_list*)( getptype(ptmodel->USER, HNSPHCON)->DATA ) );
					for(c1 = ptmodel->LOCON; c1; c1 = c1->NEXT ) {
						effx = NULL;
						for( effscanx = (struct hns_X*)( getptype( c1->USER,HNSRCN )->DATA );
						     effscanx; effscanx = effscanx->NEXT ) {
							if( effx )
								del_hns_X(effx);
							effx = effscanx;	
						}
						if( effx )
							del_hns_X(effx);
						c1->USER = delptype(c1->USER, HNSRCN);
					}
				}
				freelomodel(model);

				/* successfull exit */
				return 0;
				}
			case 'G' : /* G INT name ; */
				if (mode != 'P') {
				losig_list *ptsig;
					t1 = strtok(line + 2, " \n\t\"@");
					l1 = atol(t1);
					t2 = strtok((char *)NULL, " \n\t\"@;");
					for (ptsig = fig->LOSIG; ptsig; ptsig = ptsig->NEXT)
						if (ptsig->INDEX == (long)l1) {
							ptsig->NAMECHAIN = addchain(ptsig->NAMECHAIN,(void *)namealloc(checkname(t2)));
							break;
						}
					if (ptsig == (losig_list *)NULL) 
						(void)addlosig(fig, (long)l1, addchain((chain_list *)NULL,
											(void *)t2), INTERNAL);
				}
				break;
			case 'H' : /* H INT ; */
				break;
			case 'I' : /* I name name name | numlist | numlist ; */
				if (mode != 'P') {
				num_list *pnlist1 = (num_list *)NULL, *pnlist2 = (num_list *)NULL;
				char s2[100];
				char s3[100];

					if (strchr(line + 2, ';') != (char *)NULL) {
						t1 = strtok(line + 2, " \n\t\"@");
						t2 = strtok((char *)NULL, " \n\t\"@");
						(void)strcpy(s2, t2);
						t3 = strtok((char *)NULL, " \n\t\"@");
						(void)strcpy(s3, t3);
						t4 = strtok((char *)NULL, " \n\t\"@");
						t5 = strtok((char *)NULL, " \n\t\"@");
						if (*t5 == '|')
							pnlist1 = (num_list *)NULL;
						else {
							if (*t5 == '*')
								pnlist1 = addnum((num_list *)NULL, 0L);
							else
								pnlist1 = addnum((num_list *)NULL, atol(t5));
							while (*(t5 = strtok((char *)NULL, " \n\t\"@")) != '|') {
								if (*t5 == '*')
									pnlist1 = addnum(pnlist1, 0L);
								else
									pnlist1 = addnum(pnlist1, atol(t5));
							}
						}
						t6 = strtok((char *)NULL, " \n\t\"@;");
						if (t6 == (char *)NULL)
							pnlist2 = (num_list *)NULL;
						else {
							if (*t6 == '*')
								pnlist2 = addnum((num_list *)NULL, 0L);
							else
								pnlist2 = addnum((num_list *)NULL, atol(t6));
							while ((t6 = strtok((char *)NULL, " \n\t\"@;"))
									!= (char *)NULL) {
								if (*t6 == '*')
									pnlist2 = addnum(pnlist2, 0L);
								else
									pnlist2 = addnum(pnlist2, atol(t6));
							}
						}
					} else
						hns_error((int)SYNTAX_ERROR, fname, (long)i, (char *)NULL, 0L);

					{
					lofig_list	*ptmodel;
					chain_list	*phcon,*ch1,*ch2,*chainsig;
					num_list	*n1,*n2;
					losig_list	*ptsig;
					locon_list	*cm, *ci;
					loins_list	*ptins;
					struct hns_X	*scanx;
					struct hns_I	*newi;
					chain_list	*headi;

					ptmodel = getlomodel(model,s2);
					phcon = (chain_list*)( (getptype(ptmodel->USER,HNSPHCON))->DATA );
					pnlist2 = (num_list*)reverse((chain_list*)pnlist2);
					chainsig = NULL;

					for(ch1 = phcon, n1 = pnlist2; ch1 && n1; ch1 = ch1->NEXT, n1 = n1->NEXT ) {
						/* Verify all signals on locon are the same */
						for(ch2 = ch1->NEXT, n2 = n1->NEXT ; ch2 && n2 ; ch2 = ch2->NEXT, n2 = n2->NEXT ) {
							if( (ch1->DATA == ch2->DATA) && (n1->DATA != n2->DATA) )
								hns_error((int)SYNTAX_ERROR, fname, (long)i, (char*)NULL, 0L);
						}
						/* Get one signal per locon */
						for(ch2 = phcon; ch2!=ch1; ch2 = ch2->NEXT ) {
							if(ch2->DATA == ch1->DATA)
								break;
						}
						if(ch1 == ch2) {
							ptsig = givelosigalias( fig, head_sigalias, n1->DATA );
							chainsig = addchain(chainsig,(void*)ptsig);
						}
					}
					/* On recupere la liste des signaux dans l'ordre inverse ou ils apparaissent dans le
					   fichier, mais dans l'ordre des connecteurs du modele */
					ptins = addloins(fig,s3,ptmodel,chainsig);
					freenum(pnlist2);
					freenum(pnlist1); /* no more parameters */

					for( cm = ptmodel->LOCON; cm; cm = cm->NEXT) {
						for( ci = ptins->LOCON; ci; ci = ci->NEXT ) {
							if( ci->NAME == cm->NAME )
								break;
						}
						headi = NULL;
						for(scanx=(struct hns_X*)( getptype(cm->USER,HNSRCN)->DATA );
						    scanx;
						    scanx = scanx->NEXT) {
							newi  = givehnsi(fig, scanx->idx,ptins);
							headi = addchain(headi,newi);
						}
						ci->USER = addptype(ci->USER,HNSRCN,headi);
					}
					}
				}
				break;
			case 'J' : /* J INT INT ; */
				{
					losig_list	*s1, *s2;
					chain_list	*scan_head_join1;
					chain_list	*scan_head_join2;
					chain_list	*scan_join;

					t1 = strtok( line + 2, " \n\t\"@" );
					s1 = givelosig( fig, atol(t1) );

					for( scan_head_join1 = head_join; scan_head_join1 ; scan_head_join1 = scan_head_join1->NEXT ) {
						for( scan_join = (chain_list*)(scan_head_join1->DATA); scan_join; scan_join = scan_join->NEXT ) {
							if( s1 == scan_join->DATA )
								break;
						}
						if( scan_join )
							break;
					}

					if( ! scan_head_join1 ) {
						head_join = addchain( head_join, addchain( NULL, s1 ) );
						scan_head_join1 = head_join;
					}
					/* scan_head_join1 est la liste qui contient le premier signal rencontre */

					while( (t1 = strtok( NULL, " \n\t\"@" ) ) )
					{
						if( strcmp(t1,";")==0 )
							break;
						s2 = givelosig( fig, atol(t1) );

						for( scan_head_join2 = head_join; scan_head_join2 ; scan_head_join2 = scan_head_join2->NEXT ) {
							for( scan_join = (chain_list*)(scan_head_join2->DATA); scan_join; scan_join = scan_join->NEXT ) {
								if( s2 == scan_join->DATA )
									break;
							}
							if( scan_join )
								break;
						}

						if( !scan_head_join2 ) {
							scan_head_join1->DATA = addchain( (chain_list*)(scan_head_join1->DATA), s2 );
						}
						else 
						if( scan_head_join1 != scan_head_join2 ) {
							for( scan_join = (chain_list*)(scan_head_join2->DATA); scan_join; scan_join = scan_join->NEXT ) {
								scan_head_join1->DATA = addchain( (chain_list*)(scan_head_join1->DATA), scan_join->DATA );
							}
							freechain( (chain_list*)(scan_head_join2->DATA) );
							head_join = delchain( head_join, scan_head_join2 );
						}
					}
				}
				break;
			case 'M' : /* M name name | strlist | strlist | numlist ; */
				if (mode != 'P') {
				chain_list *slist2 = (chain_list *)NULL, *pt;
				num_list *llist3 = NULL, *ptn;
				chain_list *ptv;
				locon_list *ptcon;
				struct hns_X	*headX;

					t1 = strtok(line + 2, " \n\t\"@");
					t2 = strtok((char *)NULL, " \n\t\"@");
					(void)strcpy(s, t2);
					t3 = strtok((char *)NULL, " \n\t\"@");     /* the seperator | */

					if (*t3 != '|')
						hns_error((int)SYNTAX_ERROR, fname, (long)i, (char *)NULL, 0L);

					/* the first strlist is to be skipped */
					do {
						t4 = strtok((char *)NULL, " \n\t\"@");
						if (t4 == (char *)NULL)
							hns_error((int)SYNTAX_ERROR, fname, (long)i, (char *)NULL, 0L);
					} while(*t4 != '|');

					/* the second one contains the connectors name and order */
					t4 = strtok((char *)NULL, " \n\t\"@");
					while(*t4 != '|') {
						if (t4 == (char *)NULL)
							hns_error((int)SYNTAX_ERROR, fname, (long)i, (char *)NULL, 0L);
						slist2 = addchain(slist2, (void *)namealloc(checkname(t4)));
						t4 = strtok((char *)NULL, " \n\t\"@");
					}

					/* the third  one contains the connectors ident */
					t4 = strtok((char *)NULL, " \n\t\"@");
					while(*t4 != ';') {
						if (t4 == (char *)NULL)
							hns_error((int)SYNTAX_ERROR, fname, (long)i, (char *)NULL, 0L);
						llist3 = addnum(llist3,atol(t4));
						t4 = strtok((char *)NULL, " \n\t\"@");
					}

					model = addlomodel(model, s);
					llist3 = (num_list*)reverse((chain_list*)llist3);
					slist2 = reverse(slist2);

					for (pt = slist2, ptn = llist3; pt && ptn ; pt = pt->NEXT, ptn = ptn->NEXT) {

						headX       = add_hns_X();
						headX->NEXT = NULL;
						headX->idx  = ptn->DATA;
						headX->rcn  = 0;

						for(ptv = slist2; ptv != pt; ptv = ptv->NEXT) {
							if( pt->DATA == ptv->DATA )
								break; 
						}
						if(pt == ptv) {
							ptcon       = addlocon(model, (char *)pt->DATA,(losig_list *)NULL, 'X');
							ptcon->USER = addptype(ptcon->USER, HNSRCN, headX);
						}
						else {
							ptcon       = getlocon(model,(char*)(pt->DATA));
							ptl         = getptype(ptcon->USER,HNSRCN);
							headX->NEXT = (struct hns_X*)( ptl->DATA );
							ptl->DATA   = headX;
						}
					}

					if( pt || ptn )
						hns_error((int)PARSE_ERROR, fname, (long)i, (char *)NULL, 0L);

					freenum(llist3);
					model->USER = addptype(model->USER,HNSPHCON,slist2);
				}
				break;
			case 'N' : /* N INT name ; */
				if (mode != 'P') {
					losig_list *ptsig;
					t1 = strtok(line + 2, " \n\t\"@");
					l1 = atol(t1);
					t2 = strtok((char *)NULL, " \n\t\"@;");
					ptsig = givelosigalias(fig, head_sigalias, (long)l1);
					ptsig->NAMECHAIN = addchain(ptsig->NAMECHAIN,(void *)namealloc(checkname(t2)));
					cursig = givelosig( fig, (long)l1 );
				}
				break;
			case 'P' : /* P INT name num ; */
				if (mode != 'C') {
					t1 = strtok(line + 2, " \n\t\"@");
					t2 = strtok((char *)NULL, " \n\t\"@");
					t3 = strtok((char *)NULL, " \n\t\"@;");
					/* let's keep it, just in case ... */
				}
				break;

			case 'T' : /* T name name name num num cap INT INT INT INT ; */
				if (mode != 'P') {
				char type;
					t1 = strtok(line + 2, " \n\t\"@");
					t2 = strtok((char *)NULL, " \n\t\"@[");
					t3 = strtok((char *)NULL, " \n\t\"@[,");
					t4 = strtok((char *)NULL, " \n\t\"@,]");
					t5 = strtok((char *)NULL, " \n\t\"@]");
					t6 = strtok((char *)NULL, " \n\t\"@");
					t7 = strtok((char *)NULL, " \n\t\"@");
					t8 = strtok((char *)NULL, " \n\t\"@");
					t9 = strtok((char *)NULL, " \n\t\"@");
					if (islower((int)*t1))
						*t1 = toupper(*t1);
					type = (*t1 == tn) ? TRANSN : TRANSP;
					(void)strcpy(s, t3);
					l1 = stol(s);
					(void)strcpy(s, t4);
					l2 = stol(s);
					if ((t10 = strtok((char *)NULL, " \n\t\"@;")) == (char *)NULL) {
					losig_list *ptgrid;
						ptgrid = givelosigalias(fig, head_sigalias, atol(t6));
						(void)addlotrs(fig, type, 0L, 0L,
											(unsigned short)l1, (unsigned short)l2,
											0, 0, 0, 0,
											ptgrid, givelosigalias(fig, head_sigalias, atol(t7)),
											givelosigalias(fig, head_sigalias, atol(t8)), givelosigalias(fig, head_sigalias, atol(t9) ), NULL);
						/* do not add grid capacatance anymore
						(void)rcn_addcapa(ptgrid, atof(t5));
						*/
					}
					else { /* T name name [num,num] num num cap INT INT INT INT; */
					losig_list *ptgrid;
						t11 = strtok((char *)NULL, " \n\t\"@;");
						ptgrid = givelosigalias(fig, head_sigalias, atol(t8));
						(void)strcpy(s, t5);
						l3 = stol(s);
						(void)strcpy(s, t6);
						l4 = stol(s);
						(void)addlotrs(fig, type, l1, l2,
									(unsigned short)l3, (unsigned short)l4,
									0, 0, 0, 0,
									ptgrid, givelosigalias(fig, head_sigalias, atol(t9)),
									givelosigalias(fig, head_sigalias, atol(t10)), givelosigalias(fig, head_sigalias, atol(t11) ), NULL);
						/* do not add grid capacatance anymore
						(void)rcn_addcapa(ptgrid, atof(t7));
						*/
					}
				}
				break;

			case 'W' : /* W node1 node2 R C */
				if (mode != 'P') {
				char           *separ;
				struct hns_X   *pt_X;
				struct hns_P   *pt_P;
				struct hns_I   *pt_I;
				struct hns_W   *pt_W;
				struct hns_W   *head_W;

				separ=" \n\t\"@";
				ptl = getptype(cursig->USER,HNSRCN);
				if(!ptl) {
					cursig->USER = addptype(cursig->USER,HNSRCN,NULL);
					ptl          = cursig->USER;
				}
				head_W = ( struct hns_W* )( ptl->DATA );

				pt_W = add_hns_W();

				t1 = strtok(line + 2, separ);
				if( strcasecmp(t1,"P")==0 ) {
					t2             = strtok((char*)NULL,separ);
					l1             = atol(t2);
					pt_P           = givehnsp(cursig, l1);
					pt_W->flag     = pt_W->flag | 4;
					pt_W->node1    = pt_P;
				}
				else
				if( strcasecmp(t1,"I")==0 ) {
					t2             = strtok((char*)NULL,separ);
					t3             = strtok((char*)NULL,separ);
					l1             = atol(t3);

					pt_I           = givehnsi(fig, l1,getloins(fig,t2));
					pt_W->flag     = pt_W->flag | 2;
					pt_W->node1    = pt_I;
				}
				else
				if( strcasecmp(t1,"X")==0 ) {
					t2             = strtok((char*)NULL,separ);
					l1             = atol(t2);

					pt_X           = givehnsx(fig, l1);
					pt_W->flag     = pt_W->flag | 1;
					pt_W->node1    = pt_X;
				}
				else {
					hns_error((int)SYNTAX_ERROR, fname, (long)i, (char *)NULL, 0L);
				}
				t1 = strtok((char*)NULL, separ);
				if( strcasecmp(t1,"P")==0 ) {
					t2             = strtok((char*)NULL,separ);
					l1             = atol(t2);

					pt_P           = givehnsp(cursig, l1);
					pt_W->flag     = pt_W->flag | 32;
					pt_W->node2    = pt_P;
				}
				else
				if( strcasecmp(t1,"I")==0 ) {
					t2             = strtok((char*)NULL,separ);
					t3             = strtok((char*)NULL,separ);
					l1             = atol(t3);

					pt_I           = givehnsi(fig, l1,getloins(fig,t2));
					pt_W->flag     = pt_W->flag | 16;
					pt_W->node2    = pt_I;
				}
				else
				if( strcasecmp(t1,"X")==0 ) {
					t2             = strtok((char*)NULL,separ);
					l1             = atol(t2);

					pt_X           = givehnsx(fig, l1);
					pt_W->flag     = pt_W->flag | 8;
					pt_W->node2    = pt_X;
				}
				else {
					hns_error((int)SYNTAX_ERROR, fname, (long)i, (char *)NULL, 0L);
				}

				t1         = strtok( NULL, separ);
				pt_W->r    = stof(t1);
				t1         = strtok( NULL, separ);
				pt_W->c    = stof(t1);
				pt_W->NEXT = head_W;
				head_W     = pt_W;
				ptl->DATA  = head_W;
				}
				break;

			case 'X' : /* X INT bof name ; */
				{
					locon_list	*ptcon;
					losig_list	*ptsig;
					losig_list	*sig2;
					char		*conname;
					struct hns_X	*headX;
					ptype_list	*ptl;
					chain_list	*scan_head_sigalias, *scan_sigalias;

					t1 = strtok(line + 2, " \n\t\"@");
					l1 = atol(t1);
					t2 = strtok((char *)NULL, " \n\t\"@");
					t3 = strtok((char *)NULL, " \n\t\"@;");

					headX= givehnsx(fig, atol(t2));

					conname = namealloc(checkname(t3));

					/* Chaque locon possede un nom unique */

					for(ptcon = fig->LOCON; ptcon; ptcon = ptcon->NEXT) {
						if( ptcon->NAME == conname )
							break;
					}

					if( ptcon ) {
						if ( ptcon->SIG->INDEX != l1 ) {
							sig2 = givelosig(fig, l1);
                                                        sig2->TYPE = EXTERNAL;
							for( scan_head_sigalias = head_sigalias; scan_head_sigalias; scan_head_sigalias = scan_head_sigalias->NEXT ) {
								for( scan_sigalias = (chain_list*)(scan_head_sigalias->DATA); scan_sigalias; scan_sigalias = scan_sigalias->NEXT ) {
									if( (losig_list*)( scan_sigalias->DATA ) == ptcon->SIG )
										break;
								}
								if( scan_sigalias )
									break;
							}
							if( !scan_head_sigalias ) {
								/* Attention : l'ordre est important : le signal qui existait avant doit etre
								   en fin de liste */
								head_sigalias = addchain( head_sigalias, addchain( addchain(NULL,ptcon->SIG), sig2 ));
							}
							else {
								for( scan_sigalias = (chain_list*)(scan_head_sigalias->DATA); scan_sigalias; scan_sigalias = scan_sigalias->NEXT ) {
									if( (losig_list*)( scan_sigalias->DATA ) == sig2 )
										break;
								}
								if( !scan_sigalias ) {
									scan_head_sigalias->DATA = addchain( (chain_list*)(scan_head_sigalias->DATA), sig2);
								}
							}
						}
					
						/* Si on est en mode 'C', on peut se retrouver ici sans avoir de ptype */
						if( mode == 'C' )
						{
							ptl       = getptype(ptcon->USER,HNSRCN);
							if( !ptl )
							{
							  /* C'est la 1ere fois qu'on tombe sur ce locon, on lui enleve tous les connecteurs physiques 
								 * qu'il avait */
							  while(ptcon->PNODE)
								  delloconnode( ptcon, ptcon->PNODE->DATA);
								ptl = ptcon->USER = addptype( ptcon->USER, HNSRCN, NULL );
							}
						}
						else
							ptl       = getptype(ptcon->USER,HNSRCN);

						ptl->DATA = addchain( (chain_list*)(ptl->DATA),headX);
					}
					else {
						/* Si c'est il existe deja un connecteur sur ce signal, le addlosig terminera le programme. */
						ptsig       = addlosig(fig, (long)l1, addchain((chain_list *)NULL,
					  	                     (void *)conname), EXTERNAL);
						ptcon       = addlocon( fig, conname, ptsig, 'X');
						ptcon->USER = addptype( ptcon->USER, HNSRCN, addchain(NULL,headX) );
					}
				}

				break;
			case 'Y' : /* Y INT INT [ num , num ] [ num , num ] ; */
                                break;
				t1 = strtok(line + 2, " \n\t\"@");
				t2 = strtok((char *)NULL, " \n\t\"@;");
				l2 = atol(t2);
				ylist = addnum(ylist, l2); /* create a list of thru routes */
				break;
			case 'Z' : /* Z INT name INT cap INT num num ; */
				break;
			default :
				hns_error((int)SYNTAX_ERROR, fname, (long)i, (char *)NULL, 0L);
				break;
		}
	}
}

/*******************************************************************************
* unique function accesible from this file                                     *
*******************************************************************************/
void vtiloadlofig(ptfig, figname, mode)
lofig_list *ptfig;
char *figname;
char mode;
{
FILE *in;

	/* opening file */
	if ((in = mbkfopen(figname, IN_LO, READ_TEXT)) == (FILE *)NULL)
		hns_error((int)OPEN_ERROR, figname, 0L, (char *)NULL, 0L);

	if (TRACE_MODE == 'Y')
		(void)printf("\n--- mbk --- parsing file : %s.%s in mode %c\n",
		     		figname, IN_LO, mode);

	/* parsing */
	if (parse(ptfig, mode, in, figname))
		hns_error((int)PARSE_ERROR, figname, 0L, (char *)NULL, 0L);

	if (fclose(in))
		hns_error((int)CLOSE_ERROR, figname, 0L, (char *)NULL, 0L);

	(void)strcpy(PARSER_INFO, "@(#)vti logical views parser version 4.04");
}

static struct hns_X* add_hns_X()
{
	struct hns_X *head;
	int i;

	if(!HNS_X)
	{
		HNS_X = (struct hns_X*)mbkalloc(sizeof(struct hns_X)*64);
		FREE_HNS = addchain(FREE_HNS,HNS_X);

		for(i=0;i<=62;i++)
			HNS_X[i].NEXT=&(HNS_X[i+1]);
		HNS_X[63].NEXT=NULL;
	}
	head       = HNS_X;
	HNS_X      = HNS_X->NEXT;

	head->NEXT = NULL;
	head->idx  = 0;
	head->rcn  = 0;

	return(head);  
}

static void del_hns_X( ptx )
struct hns_X *ptx;
{
	ptx->NEXT = HNS_X;
	HNS_X = ptx;
}

static struct hns_I* add_hns_I()
{
	struct hns_I *head;
	int i;

	if(!HNS_I)
	{
		HNS_I = (struct hns_I*)mbkalloc(sizeof(struct hns_I)*64);
		FREE_HNS = addchain(FREE_HNS,HNS_I);

		for(i=0;i<=62;i++)
			HNS_I[i].NEXT=&(HNS_I[i+1]);
		HNS_I[63].NEXT=NULL;
	}
	head       = HNS_I;
	HNS_I      = HNS_I->NEXT;

	head->NEXT = NULL;
	head->idx  = 0;
	head->rcn  = 0;
	head->ins  = NULL;

	return(head);  
}

static void del_hns_I( pti )
struct hns_I *pti;
{
	pti->NEXT = HNS_I;
	HNS_I = pti;
}

static struct hns_P* add_hns_P()
{
	struct hns_P *head;
	int i;

	if(!HNS_P)
	{
		HNS_P = (struct hns_P*)mbkalloc(sizeof(struct hns_P)*64);
		FREE_HNS = addchain(FREE_HNS,HNS_P);

		for(i=0;i<=62;i++)
			HNS_P[i].NEXT=&(HNS_P[i+1]);
		HNS_P[63].NEXT=NULL;
	}
	head       = HNS_P;
	HNS_P      = HNS_P->NEXT;

	head->NEXT = NULL;
	head->idx  = 0;
    head->sig  = NULL;
	head->rcn  = 0;

	return(head);  
}

/*
static void del_hns_P( ptp )
struct hns_P *ptp;
{
	ptp->NEXT = HNS_P;
	HNS_P = ptp;
}
*/

static struct hns_W* add_hns_W()
{
	struct hns_W *head;
	int i;

	if(!HNS_W)
	{
		HNS_W = (struct hns_W*)mbkalloc(sizeof(struct hns_W)*64);
		FREE_HNS = addchain(FREE_HNS,HNS_W);

		for(i=0;i<=62;i++)
			HNS_W[i].NEXT=&(HNS_W[i+1]);
		HNS_W[63].NEXT=NULL;
	}
	head        = HNS_W;
	HNS_W       = HNS_W->NEXT;

	head->NEXT  = NULL;
	head->flag  = 0;
	head->node1 = NULL;
	head->node2 = NULL;
	head->r     = 0.0;
	head->c     = 0.0;

	return(head);  
}

static void del_hns_W( ptw )
struct hns_W *ptw;
{
	ptw->NEXT = HNS_W;
	HNS_W = ptw;
}

static struct hns_P* givehnsp( ptsig, index )
losig_list	*ptsig;
int		 index;
{
	ptype_list	*ptl;
	struct hns_P	*ptp;
	struct hns_W	*scanw;

	ptl = getptype(ptsig->USER, HNSRCN);
	if( !ptl ) {
		ptp      = add_hns_P();
		ptp->idx = index;
		ptp->sig = ptsig;
		ptp->rcn = 0;
		return(ptp);
	}

	for(scanw = (struct hns_W*)( getptype( ptsig->USER, HNSRCN )->DATA ); scanw; scanw = scanw->NEXT ) {
		if(scanw->flag & 4) {
			ptp = (struct hns_P*)(scanw->node1);
			if( ptp->idx == index && ptp->sig == ptsig )
				return(ptp);
		}
		if(scanw->flag & 32) {
			ptp = (struct hns_P*)(scanw->node2);
			if( ptp->idx == index  && ptp->sig == ptsig )
				return(ptp);
		}
	}
	ptp      = add_hns_P();
	ptp->idx = index;
	ptp->sig = ptsig;
	ptp->rcn = 0;
	return(ptp);
}

static struct hns_X* givehnsx(ptfig, index)
lofig_list		*ptfig;
int	index;
{
	ptype_list	*ptl;
	struct hns_X	*headx, *scanx;

	ptl = getptype(	ptfig->USER, HNSRCNX );
	if( !ptl ) {
		ptfig->USER = addptype( ptfig->USER, HNSRCNX, NULL );
		ptl = ptfig->USER;
	}
	headx = (struct hns_X*)(ptl->DATA);

	for( scanx = headx; scanx; scanx = scanx->NEXT ) {
		if (scanx->idx == index )
			break;
	}

	if(!scanx) {
		scanx       = add_hns_X();
		scanx->idx  = index;
		scanx->NEXT = headx;
		scanx->rcn  = 0;
		headx       = scanx;
		ptl->DATA = headx;
	}

	return(scanx);
}

static struct hns_I*  givehnsi(ptfig, index, ptins)
lofig_list		*ptfig;
int			index;
loins_list		*ptins;
{
	ptype_list	*ptl;
	struct hns_I	*headi, *scani;

	ptl = getptype(	ptfig->USER, HNSRCNI );
	if( !ptl ) {
		ptfig->USER = addptype( ptfig->USER, HNSRCNI, NULL );
		ptl = ptfig->USER;
	}
	headi = (struct hns_I*)(ptl->DATA);

	for( scani = headi; scani; scani = scani->NEXT ) {
		if (scani->idx == index && scani->ins == ptins)
			break;
	}

	if(!scani) {
		scani       = add_hns_I();
		scani->idx  = index;
		scani->NEXT = headi;
		scani->ins  = ptins;
		scani->rcn  = 0;
		headi       = scani;
		ptl->DATA = headi;
	}

	return(scani);
	
}

static void buildrcn( ptfig )
lofig_list	*ptfig;
{
	int		 index;
	losig_list	*scanlosig;
	ptype_list	*ptl;
	lorcnet_list	*ptrcnet;
	struct hns_W	*headw;
	struct hns_W	*scanw;
	struct hns_W	*elem_w;
	struct hns_X	*elem_x;
	struct hns_X	*headx;
	struct hns_X	*scanx;
	struct hns_I	*elem_i;
	struct hns_I	*scani;
	struct hns_I	*headi;
	struct hns_P	*elem_p;
	int		 n1=0,n2=0;
	locon_list	*ptcon;
	chain_list	*scan;
	chain_list	*head;
	chain_list	*scanchain;
	locon_list	*scanlocon;
	loins_list	*scanloins;
  ptype_list  *ptl_head;


	/* On commence par remplir le champ RCN des structures */
	for( scanlosig = ptfig->LOSIG; scanlosig; scanlosig = scanlosig->NEXT ) {

		index = 1;
		ptl = getptype( scanlosig->USER, HNSRCN );

		/* Si on est en mode 'C', il se peut que certains signaux aient herites d'une vue
		 * RC dans les signaux cree pour les connecteurs */

    headw = NULL;

		if( ptl ) {

			headw = (struct hns_W*)( ptl->DATA );

      /* On ne cree les vues RC que si on a des resistances. Tant pis pour les noeuds sur les locons */
      if( !scanlosig->PRCN && headw )
  			ptrcnet = addlorcnet(scanlosig);

			for( scanw = headw; scanw; scanw = scanw->NEXT ) {

				if( scanw->flag & 1 ) {
					elem_x = (struct hns_X*)( scanw->node1 );
					if( elem_x->rcn == 0 )
						elem_x->rcn = index++;
					n1 = elem_x->rcn;
				}
				else if( scanw->flag & 2 ) {
					elem_i = (struct hns_I*)( scanw->node1 );
					if( elem_i->rcn == 0 )
						elem_i->rcn = index++;
					n1 = elem_i->rcn;
				}
				else if( scanw->flag & 4 ) {
					elem_p = (struct hns_P*)( scanw->node1 );
					if( elem_p->rcn == 0 )
						elem_p->rcn = index++;
					n1 = elem_p->rcn;
				}

				if( scanw->flag & 8 ) {
					elem_x = (struct hns_X*)( scanw->node2 );
					if( elem_x->rcn == 0 )
						elem_x->rcn = index++;
					n2 = elem_x->rcn;
				}
				else if( scanw->flag & 16 ) {
					elem_i = (struct hns_I*)( scanw->node2 );
					if( elem_i->rcn == 0 )
						elem_i->rcn = index++;
					n2 = elem_i->rcn;
				}
				else if( scanw->flag & 32 ) {
					elem_p = (struct hns_P*)( scanw->node2 );
					if( elem_p->rcn == 0 )
						elem_p->rcn = index++;
					n2 = elem_p->rcn;
				}

				addlowire( scanlosig, 0, scanw->r, scanw->c, n1, n2 );
                                rcn_addcapa( scanlosig, scanw->c );
			}
		}

		ptl = getptype( scanlosig->USER, LOFIGCHAIN );

		if( ptl && headw ) {

			for( scanchain = (chain_list*)( ptl->DATA ); scanchain; scanchain = scanchain->NEXT ) {

				ptcon = (locon_list*)( scanchain->DATA );
        ptl_head = getptype( ptcon->USER, HNSRCN );
        if( !ptl_head )
          continue;

				head = (chain_list*)( ptl_head->DATA );
				head = reverse(head);

				if( ptcon->ROOT == ptfig ) {
					for(scan = head; scan; scan = scan->NEXT ) {
						elem_x = (struct hns_X*)(scan->DATA);
						if( elem_x->rcn == 0 )
							elem_x->rcn = index++;
						setloconnode( ptcon, elem_x->rcn );
					}
				}
				else {
					for(scan = head; scan; scan = scan->NEXT ) {
						elem_i = (struct hns_I*)(scan->DATA);
						if( elem_i->rcn == 0 )
							elem_i->rcn = index++;
						setloconnode( ptcon, elem_i->rcn );
					}
				}
				head = reverse(head);
			}
		}
	}

	/* Technicien de surface */

	for(scanlosig = ptfig->LOSIG; scanlosig; scanlosig = scanlosig->NEXT ) {
		ptl = getptype( scanlosig->USER, HNSRCN );
		if( ptl ) {
			headw = (struct hns_W*)( ptl->DATA );
			elem_w = NULL;
			for( scanw = headw; scanw; scanw = scanw->NEXT ) {
				if( elem_w )
					del_hns_W( elem_w );
			}
			if( elem_w )
				del_hns_W( elem_w );
			scanlosig->USER = delptype( scanlosig->USER, HNSRCN );
		}
	}

	for(scanlocon = ptfig->LOCON; scanlocon; scanlocon = scanlocon->NEXT ) {
		freechain( (chain_list*)( getptype(scanlocon->USER,HNSRCN)->DATA ) );
		scanlocon->USER = delptype( scanlocon->USER, HNSRCN );
	}

	for(scanloins = ptfig->LOINS; scanloins; scanloins = scanloins->NEXT ) {
		for(scanlocon = scanloins->LOCON; scanlocon; scanlocon = scanlocon->NEXT ) {
			freechain( (chain_list*)( getptype(scanlocon->USER,HNSRCN)->DATA ) );
			scanlocon->USER = delptype( scanlocon->USER, HNSRCN );
		}
	}

	ptl = getptype( ptfig->USER, HNSRCNX );
	if(ptl) {
		headx = (struct hns_X*)( ptl->DATA );
		elem_x = NULL;
		for( scanx = headx; scanx; scanx = scanx->NEXT ) {
			if( elem_x )
				del_hns_X( elem_x );
			elem_x = scanx;
		}
		if( elem_x )
			del_hns_X( elem_x );

		ptfig->USER = delptype( ptfig->USER, HNSRCNX );
	}

	ptl = getptype( ptfig->USER, HNSRCNI );
	if(ptl) {
		headi = (struct hns_I*)( ptl->DATA );
		elem_i = NULL;
		for( scani = headi; scani; scani = scani->NEXT ) {
			if( elem_i )
				del_hns_I( elem_i );
			elem_i = scani;
		}
		if( elem_i )
			del_hns_I( elem_i );

		ptfig->USER = delptype( ptfig->USER, HNSRCNI );
	}
}

/*
static void movehnsw( sig1, sig2)
losig_list	*sig1;
losig_list	*sig2;
{
	ptype_list	*ptl;
	struct hns_W	*headw1;
	struct hns_W	*headw2;

	ptl = getptype( sig1->USER,HNSRCN );
	if (!ptl )
		headw1 = NULL;
	else
		headw1 = ( struct hns_W* )( ptl->DATA );

	ptl = getptype( sig2->USER,HNSRCN );
	if (!ptl )
		headw2 = NULL;
	else
		headw2 = ( struct hns_W* )( ptl->DATA );

	if( !headw2 )
		return;

	if( !headw1 ) {
		sig1->USER = addptype( sig1->USER, HNSRCN, headw2 );
	}
	else {
		for( ; headw1->NEXT; headw1 = headw1->NEXT ) ;
		headw1->NEXT = headw2;
	}
	sig2->USER = delptype( sig2->USER, HNSRCN );
}
*/
/*
void debug(ptfig)
lofig_list	*ptfig;
{
	locon_list	*scanlocon;
	ptype_list	*ptl;
	chain_list	*scanchain;
	struct hns_X	*elem_x;

	printf("##############################################################################################\n");
	for( scanlocon = ptfig->LOCON; scanlocon; scanlocon = scanlocon->NEXT ) {
		printf("Connecteur %s :",scanlocon->NAME);
		ptl = getptype(scanlocon->USER,HNSRCN);
		if( ptl )
		{
			for(scanchain = (chain_list*)(ptl->DATA); scanchain; scanchain = scanchain->NEXT ) {
				elem_x = (struct hns_X*)(scanchain->DATA);
				printf("%ld ", elem_x->idx);
			}
		}
		printf("\n");
	}
	printf("##############################################################################################\n");
}
*/

static losig_list* givelosigalias( ptfig, head_sigalias, n )
lofig_list	*ptfig;
chain_list	*head_sigalias;
int			 n;
{
	chain_list	*scan_head_sigalias;
	chain_list	*scan_sigalias;
	losig_list	*ptsig;
	losig_list	*grpsig;

	for( scan_head_sigalias = head_sigalias; scan_head_sigalias; scan_head_sigalias = scan_head_sigalias->NEXT ) {

		for( scan_sigalias = (chain_list*)(scan_head_sigalias->DATA); scan_sigalias; scan_sigalias = scan_sigalias->NEXT ) {

			ptsig = (losig_list*)( scan_sigalias->DATA );

			if( ptsig->INDEX == n ) {
				for( ; scan_sigalias->NEXT ; scan_sigalias = scan_sigalias->NEXT );
				grpsig = (losig_list*)( scan_sigalias->DATA );
				return( grpsig );
			}
		}
	}

	return( givelosig( ptfig, n ) );
}

static int compare_alias_join( fig, head_sigalias, ext_join )
lofig_list	*fig;
chain_list	*head_sigalias;
chain_list	**ext_join;
{
	chain_list	*scan_head_sigalias;
	chain_list	*scan_sigalias;
	chain_list	*scan_head_join;
	chain_list	*scan_join;
	chain_list	*scan_chain;
	chain_list	*head_new_join;
	chain_list	*add_to_head_join;
	chain_list	*head_join;
	int		 manque;
	int	me;

	head_join = *ext_join;

	add_to_head_join = NULL;

	for( scan_head_sigalias = head_sigalias; scan_head_sigalias; scan_head_sigalias=scan_head_sigalias->NEXT) {

		manque = 0;
		me=0;

		scan_sigalias = (chain_list*)( scan_head_sigalias ->DATA );

		for( scan_head_join = head_join; scan_head_join ; scan_head_join = scan_head_join->NEXT ) {
			for( scan_join = (chain_list*)(scan_head_join->DATA); scan_join; scan_join = scan_join->NEXT ) {
				if( scan_join->DATA == scan_sigalias->DATA ) {
					break;
				}
			}
			if( scan_join )
				break;
		}

		if( !scan_head_join ) {
			msg_err( fig, scan_head_sigalias->DATA );
			manque = 1;
			me = 1;
			/*
			return(0);
			*/
		}
		else {
		
			for( scan_sigalias = scan_sigalias->NEXT; scan_sigalias; scan_sigalias = scan_sigalias->NEXT ) {
				for( scan_join = (chain_list*)(scan_head_join->DATA); scan_join; scan_join = scan_join->NEXT ) {
					if ( scan_join->DATA == scan_sigalias->DATA )
						break;
				}
				if( !scan_join )
				{
					if( me )
						msg_err( fig, scan_head_sigalias->DATA );
					manque = 1;
					me = 1;
					/*
					return(0);
					*/
				}
			}
		}

		if( manque ) {
			/* Il manque un join pour regrouper des signaux */

			head_new_join = NULL;

			for( scan_sigalias = ((chain_list*)(scan_head_sigalias->DATA));
			                     scan_sigalias;
								 scan_sigalias = scan_sigalias->NEXT        ) {
				head_new_join = addchain( head_new_join, scan_sigalias->DATA );
			}

			for( scan_head_join = head_join; scan_head_join ; scan_head_join = scan_head_join->NEXT ) {
				for( scan_join = (chain_list*)(scan_head_join->DATA); scan_join; scan_join = scan_join->NEXT ) {
					for( scan_sigalias = ((chain_list*)(scan_head_sigalias->DATA)); scan_sigalias;
					     scan_sigalias = scan_sigalias->NEXT) {
						if( scan_join->DATA == scan_sigalias->DATA ) {
							break;
						}
					}
					if( scan_sigalias )
						break;
				}

				/* Si on a trouve au moins un element dans la liste join, on transfert toute la liste join vers la
				   nouvelle et on supprime l'originale */

				if( scan_join ) {
					for( scan_join = (chain_list*)(scan_head_join->DATA); scan_join; scan_join = scan_join->NEXT ) {
						for( scan_chain = head_new_join; scan_chain; scan_chain = scan_chain->NEXT ) {
							if( scan_chain->DATA == scan_join->DATA )
								break;
						}
						if(!scan_chain)
							head_new_join = addchain( head_new_join, scan_join->DATA );
					}

					freechain( (chain_list*)(scan_head_join->DATA) );
					scan_head_join->DATA = NULL;
				}
			}

			/* En creant le head_new_join, on a utilise addchain, ce qui inverse l'ordre des signaux */
			
			add_to_head_join = addchain( add_to_head_join, head_new_join );
		}
	}

	if( head_join ) {
		for( scan_head_join = head_join; scan_head_join->NEXT ; scan_head_join = scan_head_join->NEXT );
		scan_head_join->NEXT = add_to_head_join;
	}
	else {
		head_join = add_to_head_join;
	}
	
	*ext_join = head_join;
	return( 1 );
}

static void msg_err(fig, head)
lofig_list	*fig;
chain_list	*head;
{
	locon_list	*ptcon = NULL;
	losig_list	*ptsig;
	chain_list	*scan,*slf;
	ptype_list	*ptl;

	for( scan = head; scan; scan = scan->NEXT ) {

		ptcon = NULL;

		ptsig = (losig_list*)(scan->DATA);
		ptl = getptype( ptsig->USER, LOFIGCHAIN );

		for( slf = (chain_list*)(ptl->DATA) ; slf; slf = slf->NEXT ) {
			ptcon = (locon_list*)( slf->DATA );
			if(ptcon->ROOT == fig ) {
				break;
			}
		}

		if(ptcon)
			break;
	}

        (void)fprintf(stderr,"\n*** mbk warning ***\n");
	(void)fprintf(stderr,"no join for signals connected on the connector %s\n\n",ptcon->NAME);
}


static int traite_join( fig, head_join )
lofig_list	*fig;
chain_list	*head_join;
{
	chain_list	*scan_head_join, *scan_join;
	int		 trouve;
	losig_list	*ptsig;
	losig_list	*grpsig = NULL;
	loins_list	*scanloins;
	lotrs_list	*scanlotrs;
	locon_list	*scanlocon;
	locon_list	*oldlocon=NULL;
	chain_list	*scanchain;
	ptype_list	*ptlsrc;
	ptype_list	*ptldst;
	ptype_list	*ptl;
	struct hns_W	*scanw;
	char		 errmes[256] ;
	int			 externe;
	chain_list	*nom,*scnom;
        loctc_list      *ptctc;
        losig_list      *s1, *s2;
        float            f;

	for( scan_head_join = head_join; scan_head_join; scan_head_join = scan_head_join->NEXT ) {
      if( !scan_head_join->DATA )
           continue;

		/* On commence par verifier qu'il n'y a seulement qu'un signal qui possede des connecteurs.
		   On en profite egalement pour retrouver le signal qui regroupera les autres ( le dernier
		   dans la liste des join ) */ 

		trouve  = 0;
		externe = 0;
		nom     = NULL;

		for( scan_join = (chain_list*)(scan_head_join->DATA); scan_join; scan_join = scan_join->NEXT ) {

			ptsig = (losig_list*)(scan_join->DATA);
			ptl   = getptype( ptsig->USER, LOFIGCHAIN );

			if( ptsig->TYPE == EXTERNAL )
				externe = 1;

			for( scanchain = (chain_list*)(ptl->DATA); scanchain; scanchain = scanchain->NEXT ) {

				scanlocon = (locon_list*)( scanchain->DATA );

				if( scanlocon->ROOT==fig ) {
					if( trouve ) {
                                                sprintf(errmes,"%s and %s",oldlocon->NAME,scanlocon->NAME) ;
                                                hns_error((int)BAD_JOIN, fig->NAME, (long)0,errmes,ptsig->INDEX);
					}
					trouve = 1;
                                        oldlocon = scanlocon ;
				}
			}

			if( !scan_join->NEXT )
				grpsig = ptsig;
		}

		/* On fait remplace par ptsig tous les signaux de la figure qui apparaissent dans le join */

		/* Les connecteurs sur les instances */

		for( scanloins = fig->LOINS ; scanloins; scanloins = scanloins->NEXT ) {

			for( scanlocon = scanloins->LOCON; scanlocon; scanlocon = scanlocon->NEXT ) {

				for( scan_join = (chain_list*)(scan_head_join->DATA); scan_join; scan_join = scan_join->NEXT ) {

					if( scan_join->DATA == scanlocon->SIG ) {
						scanlocon->SIG = grpsig;
						break;
					}
				}
			}
		}

		/* Les connecteurs de la figure */

		for( scanlocon = fig->LOCON; scanlocon; scanlocon = scanlocon->NEXT ) {

			for( scan_join = (chain_list*)(scan_head_join->DATA); scan_join; scan_join = scan_join->NEXT ) {

				if( scan_join->DATA == scanlocon->SIG ) {

					scanlocon->SIG = grpsig;
					break;
				}
			}
		}

		/* Les connecteurs sur les transistors */

		for( scanlotrs = fig->LOTRS; scanlotrs; scanlotrs = scanlotrs->NEXT ) {

			for( scan_join = (chain_list*)(scan_head_join->DATA); scan_join; scan_join = scan_join->NEXT ) {

				if( scan_join->DATA == scanlotrs->GRID->SIG )
					scanlotrs->GRID->SIG   = grpsig;

				if( scan_join->DATA == scanlotrs->SOURCE->SIG )
					scanlotrs->SOURCE->SIG = grpsig;

				if( scan_join->DATA == scanlotrs->DRAIN->SIG )
					scanlotrs->DRAIN->SIG   = grpsig;

				if( scan_join->DATA == scanlotrs->BULK->SIG )
					scanlotrs->BULK->SIG   = grpsig;
			}
			
		}

		/* On efface les signaux devenus inutiles en transferant les vues RC */

		for( scan_join = ((chain_list*)(scan_head_join->DATA)); scan_join; scan_join = scan_join->NEXT ) {
			ptsig = (losig_list*)(scan_join->DATA);
			if( ptsig != grpsig ) {
                                if ( ptsig->PRCN )
                                {
                                        if( !grpsig->PRCN )
                                                addlorcnet( grpsig );
        				rcn_addcapa( grpsig, rcn_getcapa(fig,ptsig) );
                                }

				if( nom ) {
					for( scnom = nom ; scnom->NEXT; scnom = scnom->NEXT ) ;
					scnom->NEXT      = ptsig->NAMECHAIN;
					ptsig->NAMECHAIN = NULL;
				}
				else {
					nom = ptsig->NAMECHAIN;
					ptsig->NAMECHAIN = NULL;
				}

				ptlsrc = getptype( ptsig->USER, HNSRCN );
				if( ptlsrc ) {
					ptldst = getptype( grpsig->USER, HNSRCN );
					if( !ptldst ) {
						grpsig->USER = addptype( grpsig->USER, HNSRCN, ptlsrc->DATA );
					}
					else {
						for( scanw= (struct hns_W*)( ptlsrc->DATA ); scanw->NEXT; scanw = scanw->NEXT );
						scanw->NEXT = (struct hns_W*)( ptldst->DATA );
						ptldst->DATA = ptlsrc->DATA;
					}
					ptsig->USER  = delptype( ptsig->USER, HNSRCN );
				}

                                if( ptsig->PRCN )
                                        while( ptsig->PRCN->PCTC )
                                        {
                                                ptctc = (loctc_list*)(ptsig->PRCN->PCTC->DATA);
                                                s1 = ptctc->SIG1 ;
                                                s2 = ptctc->SIG2 ;
                                                f  = ptctc->CAPA ;
                                                delloctc( ptctc );
                                                if( s1 == ptsig )
                                                  s1 = grpsig;
                                                if( s2 == ptsig )
                                                  s2 = grpsig;
        
                                                if( s1 != s2 )
                                                {
                                                          addloctc( s1, 1, s2, 1, f );
                                                }
                                        }
                                dellosig( fig, ptsig->INDEX );
			}
		}

		if( externe )
			grpsig->TYPE = EXTERNAL;
                else
			grpsig->TYPE = INTERNAL;

		if( grpsig->NAMECHAIN ) {
			for( scnom = grpsig->NAMECHAIN ; scnom->NEXT; scnom = scnom->NEXT ) ;
			scnom->NEXT = nom;
		}
		else {
			grpsig->NAMECHAIN = nom;
		}
	}

	return( 1 );
}
