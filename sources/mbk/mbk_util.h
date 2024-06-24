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

/* 
 * Purpose : header file for mbk_util.c used to declare functions
 * Date    : 20/06/92
 * Author  : Frederic Petrot <Frederic.Petrot@lip6.fr>
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1997,98
 * $Id: mbk_util.h,v 1.4 2002/12/20 08:32:12 olivier Exp $
 */

#ifndef _MBK_UTIL_H_
#define _MBK_UTIL_H_

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/* hash tables maximun depth */
#define HMAX_CALLS  20


  extern    chain_list * addchain (chain_list *pthead, const void *ptdata);
  extern      num_list * addnum (num_list *ptnum, long data);
  extern    ptype_list * addptype (ptype_list *pthead, long type, void *ptdata);
  extern    chain_list * append (chain_list *pt1, chain_list *pt2);
  extern          char * concatname (char *name1, char *name2);
  extern    chain_list * delchaindata (chain_list *pthead, void *ptdeldata);
  extern    chain_list * delchain (chain_list *pthead, chain_list *ptdel);
  extern    ptype_list * delptype (ptype_list *pthead, long type);
  extern           void  downstr (const char *s, char *t);
  extern           void  freechain (chain_list *pt);
  extern           void  freenum (num_list *pt);
  extern           void  freeptype (ptype_list *pt);
  extern    ptype_list * getptype (ptype_list *pthead, long type);
  extern            int  inlibcatalog (char *figname);
  extern            int  incatalog (char *figname);
  extern            void addcatalog (char *figname);
  extern            int  incataloggds (char *figname);
  extern            int  incatalogfeed (char *figname);
  extern            int  incatalogdelete (char *figname);
  extern            void loadcatalog (char ***table, int *size, int type);
  extern           void  mbkenv ();
  extern           void  readlibfile (char *type,void (*fonc)(char *),int reload);
  extern          char * namealloc (const char *inputname);
  extern          char * nameindex (char *name, long index);
static void read_lib();
  extern    chain_list * reverse (chain_list *head);
  extern           void  upstr (char *s, char *t);
  extern            ht * addht (unsigned long len);
int gethtelm() ;
int addhtelm() ;
int sethtelm() ;
int delhtelm();
  extern           void  viewht (ht *pTable, char *(*pout)());
  extern           void  delht (ht *pTable);
  static void reallocht(ht *pTable);
  static void banner (char *s, char *police[][62], int nl);
  static void cartouche (char *tool, char *tv, char *comment, char *date, char *av, char *authors, char *contrib);
  static int indx (int c);

/* Font definition for the banner

   Rights on font material :

   Donated by H. Kagotani <kagotani@cs.titech.ac.jp>; public domain
   font from Japan
   -Misc-Fixed-Medium-R-Normal--14-130-75-75-C-140-JISX0208.1983-0
   "Public domain font.  Share and enjoy."

	This font has been extracted from X11r5 bdf file by myself, to
   be used for the standard Aliiance CAD system banners. */

static char *Unknown_Bold_Normal_14[][62] = {
{
"         ",
"   @@@   ",
" @@   @@ ",
"@@     @@",
"@@     @@",
"@@     @@",
"@@     @@",
"@@     @@",
"@@     @@",
"@@     @@",
" @@   @@ ",
"   @@@   ",
"         ",
"         ",
"         "},
/* lines 15 */
/* 2331 */
{
"      ",
"   @  ",
"  @@  ",
"@@@@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"@@@@@@",
"      ",
"      ",
"      "},
/* lines 15 */
/* 2332 */
{
"        ",
"  @@@@  ",
" @   @@ ",
"@@    @@",
"@@@   @@",
" @   @@ ",
"     @  ",
"    @   ",
"   @    ",
"  @    @",
" @@@@@@ ",
"@@@@@@@ ",
"        ",
"        ",
"        "},
/* lines 15 */
/* 2333 */
{
"        ",
"  @@@@  ",
" @   @@ ",
"@@    @@",
"@@    @@",
"     @@ ",
"   @@@  ",
"      @ ",
"@@    @@",
"@@    @@",
" @   @@ ",
"  @@@@  ",
"        ",
"        ",
"        "},
/* lines 15 */
/* 2334 */
{
"         ",
"     @@  ",
"    @@@  ",
"   @ @@  ",
"   @ @@  ",
"  @  @@  ",
" @   @@  ",
"@@   @@  ",
"@@@@@@@@@",
"     @@  ",
"     @@  ",
"    @@@@ ",
"         ",
"         ",
"         "},
/* lines 15 */
/* 2335 */
{
"        ",
" @@@@@@@",
" @@@@@@ ",
" @      ",
" @      ",
" @@@@@  ",
"     @@ ",
"      @@",
"      @@",
"@@    @@",
"@@   @@ ",
" @@@@   ",
"        ",
"        ",
"        "},
/* lines 15 */
/* 2336 */
{
"         ",
"   @@@@  ",
"  @   @@ ",
" @@   @@ ",
" @       ",
"@@ @@@@  ",
"@@@   @@ ",
"@@     @@",
"@@     @@",
"@@     @@",
" @@   @@ ",
"   @@@   ",
"         ",
"         ",
"         "},
/* lines 15 */
/* 2337 */
{
"         ",
" @@@@@@@@",
" @@@@@@@ ",
"@     @  ",
"     @   ",
"     @   ",
"    @    ",
"    @    ",
"   @@    ",
"   @@    ",
"   @@    ",
"   @@    ",
"         ",
"         ",
"         "},
/* lines 15 */
/* 2338 */
{
"        ",
"  @@@@  ",
" @    @ ",
"@@    @@",
"@@@   @@",
" @@@ @@ ",
"  @@@@  ",
" @@ @@@ ",
"@@   @@@",
"@@    @@",
" @    @ ",
"  @@@@  ",
"        ",
"        ",
"        "},
/* lines 15 */
/* 2339 */
{
"        ",
"   @@   ",
" @@  @@ ",
"@@    @@",
"@@    @@",
"@@    @@",
" @@  @@@",
"  @@@ @@",
"      @@",
"@@    @ ",
"@@   @  ",
" @@@@   ",
"        ",
"        ",
"        "},
/* lines 15 */
/* 2341 */
{
"            ",
"      @     ",
"      @     ",
"     @@@    ",
"     @@@    ",
"    @  @@   ",
"    @  @@   ",
"   @    @@  ",
"   @@@@@@@  ",
"  @      @@ ",
"  @      @@ ",
"@@@@    @@@@",
"            ",
"            ",
"            "},
/* lines 15 */
/* 2342 */
{
"           ",
"@@@@@@@    ",
"  @@   @@  ",
"  @@    @@ ",
"  @@    @@ ",
"  @@   @@  ",
"  @@@@@@   ",
"  @@    @@ ",
"  @@     @@",
"  @@     @@",
"  @@    @@ ",
"@@@@@@@@   ",
"           ",
"           ",
"           "},
/* lines 15 */
/* 2343 */
{
"          ",
"    @@@@ @",
"  @@    @@",
" @@      @",
"@@       @",
"@@        ",
"@@        ",
"@@        ",
"@@        ",
" @@      @",
"  @@    @@",
"    @@@@  ",
"          ",
"          ",
"          "},
/* lines 15 */
/* 2344 */
{
"           ",
"@@@@@@@    ",
"  @@   @@  ",
"  @@    @@ ",
"  @@     @@",
"  @@     @@",
"  @@     @@",
"  @@     @@",
"  @@     @@",
"  @@    @@ ",
"  @@   @@  ",
"@@@@@@@    ",
"           ",
"           ",
"           "},
/* lines 15 */
/* 2345 */
{
"           ",
"@@@@@@@@@  ",
"  @@    @  ",
"  @@     @ ",
"  @@       ",
"  @@   @   ",
"  @@@@@@   ",
"  @@   @   ",
"  @@       ",
"  @@      @",
"  @@     @ ",
"@@@@@@@@@@ ",
"           ",
"           ",
"           "},
/* lines 15 */
/* 2346 */
{
"          ",
"@@@@@@@@@ ",
"  @@    @ ",
"  @@     @",
"  @@      ",
"  @@   @  ",
"  @@@@@@  ",
"  @@   @  ",
"  @@      ",
"  @@      ",
"  @@      ",
"@@@@@@    ",
"          ",
"          ",
"          "},
/* lines 15 */
/* 2347 */
{
"            ",
"    @@@@ @  ",
"  @@    @@  ",
" @@      @  ",
"@@          ",
"@@          ",
"@@     @@@@@",
"@@     @ @@ ",
"@@    @  @@ ",
" @@      @@ ",
"  @@    @@  ",
"    @@@@    ",
"            ",
"            ",
"            "},
/* lines 15 */
/* 2348 */
{
"           ",
"@@@@   @@@@",
" @@     @@ ",
" @@     @@ ",
" @@     @@ ",
" @@     @@ ",
" @@@@@@@@@ ",
" @@     @@ ",
" @@     @@ ",
" @@     @@ ",
" @@     @@ ",
"@@@@   @@@@",
"           ",
"           ",
"           "},
/* lines 15 */
/* 2349 */
{
"      ",
"@@@@@@",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"@@@@@@",
"      ",
"      ",
"      "},
/* lines 15 */
/* 234a */
{
"           ",
"     @@@@@@",
"       @@  ",
"       @@  ",
"       @@  ",
"       @@  ",
"       @@  ",
" @     @@  ",
"@@@    @@  ",
"@@     @@  ",
" @@   @@   ",
"   @@@     ",
"           ",
"           ",
"           "},
/* lines 15 */
/* 234b */
{
"           ",
"@@@@   @@@@",
" @@     @  ",
" @@    @   ",
" @@   @    ",
" @@  @@    ",
" @@ @ @@   ",
" @@@  @@   ",
" @@    @@  ",
" @@    @@  ",
" @@     @@ ",
"@@@@   @@@@",
"           ",
"           ",
"           "},
/* lines 15 */
/* 234c */
{
"           ",
"@@@@@@     ",
"  @@       ",
"  @@       ",
"  @@       ",
"  @@       ",
"  @@       ",
"  @@       ",
"  @@       ",
"  @@      @",
"  @@     @ ",
"@@@@@@@@@@ ",
"           ",
"           ",
"           "},
/* lines 15 */
/* 234d */
{
"           ",
"@@@     @@@",
" @@     @@ ",
" @@@   @@@ ",
" @@@   @@@ ",
" @ @@ @ @@ ",
" @ @@ @ @@ ",
" @ @@@  @@ ",
" @  @@  @@ ",
" @  @   @@ ",
" @      @@ ",
"@@@    @@@@",
"           ",
"           ",
"           "},
/* lines 15 */
/* 234e */
{
"          ",
"@@@    @@@",
" @@     @ ",
" @@@    @ ",
" @ @@   @ ",
" @ @@   @ ",
" @  @@  @ ",
" @   @@ @ ",
" @   @@ @ ",
" @    @@@ ",
" @     @@ ",
"@@@    @@ ",
"          ",
"          ",
"          "},
/* lines 15 */
/* 234f */
{
"           ",
"    @@@    ",
"  @@   @@  ",
" @@     @@ ",
"@@       @@",
"@@       @@",
"@@       @@",
"@@       @@",
"@@       @@",
" @@     @@ ",
"  @@   @@  ",
"    @@@    ",
"           ",
"           ",
"           "},
/* lines 15 */
/* 2350 */
{
"          ",
"@@@@@@@   ",
"  @@   @@ ",
"  @@    @@",
"  @@    @@",
"  @@   @@ ",
"  @@@@@   ",
"  @@      ",
"  @@      ",
"  @@      ",
"  @@      ",
"@@@@@@    ",
"          ",
"          ",
"          "},
/* lines 15 */
/* 2351 */
{
"           ",
"    @@@    ",
"  @@   @@  ",
" @@     @@ ",
"@@       @@",
"@@       @@",
"@@       @@",
"@@       @@",
"@@  @@   @@",
" @@@  @ @@ ",
"  @@   @@  ",
"    @@@@@  ",
"         @@",
"           ",
"           "},
/* lines 15 */
/* 2352 */
{
"           ",
"@@@@@@@    ",
"  @@   @@  ",
"  @@    @@ ",
"  @@    @@ ",
"  @@   @@  ",
"  @@@@@    ",
"  @@  @@   ",
"  @@   @@  ",
"  @@   @@  ",
"  @@    @@ ",
"@@@@@   @@@",
"           ",
"           ",
"           "},
/* lines 15 */
/* 2353 */
{
"         ",
"  @@@@ @ ",
" @    @@ ",
"@@     @ ",
"@@@      ",
" @@@@    ",
"   @@@@  ",
"     @@@ ",
"@      @@",
"@@     @@",
"@@@    @ ",
"@  @@@@  ",
"         ",
"         ",
"         "},
/* lines 15 */
/* 2354 */
{
"            ",
" @@@@@@@@@@ ",
" @   @@   @ ",
"@    @@    @",
"     @@     ",
"     @@     ",
"     @@     ",
"     @@     ",
"     @@     ",
"     @@     ",
"     @@     ",
"   @@@@@@   ",
"            ",
"            ",
"            "},
/* lines 15 */
/* 2355 */
{
"           ",
"@@@@    @@@",
" @@      @ ",
" @@      @ ",
" @@      @ ",
" @@      @ ",
" @@      @ ",
" @@      @ ",
" @@      @ ",
" @@      @ ",
"  @@    @  ",
"    @@@@   ",
"           ",
"           ",
"           "},
/* lines 15 */
/* 2356 */
{
"           ",
"@@@@    @@@",
" @@      @ ",
" @@      @ ",
"  @@    @  ",
"  @@    @  ",
"   @@  @   ",
"   @@  @   ",
"    @@@    ",
"    @@@    ",
"     @     ",
"     @     ",
"           ",
"           ",
"           "},
/* lines 15 */
/* 2357 */
{
"           ",
"@@@ @@@ @@@",
" @@  @@  @ ",
" @@  @@  @ ",
" @@  @@  @ ",
"  @@ @@@ @ ",
"  @@@ @@@  ",
"  @@@ @@@  ",
"   @@  @@  ",
"   @@  @@  ",
"   @@  @@  ",
"   @@  @@  ",
"           ",
"           ",
"           "},
/* lines 15 */
/* 2358 */
{
"           ",
"@@@@   @@@@",
" @@     @  ",
"  @@   @   ",
"   @@ @    ",
"    @@     ",
"    @@     ",
"    @@@    ",
"   @  @@   ",
"  @    @@  ",
" @      @@ ",
"@@@    @@@@",
"           ",
"           ",
"           "},
/* lines 15 */
/* 2359 */
{
"           ",
"@@@@   @@@@",
" @@     @  ",
"  @@   @   ",
"   @@ @    ",
"    @@     ",
"    @@     ",
"    @@     ",
"    @@     ",
"    @@     ",
"    @@     ",
"  @@@@@@   ",
"           ",
"           ",
"           "},
/* lines 15 */
/* 235a */
{
"           ",
" @@@@@@@@@ ",
" @     @@  ",
"@     @@   ",
"     @@    ",
"     @@    ",
"    @@     ",
"   @@      ",
"   @@      ",
"  @@      @",
" @@      @ ",
"@@@@@@@@@@ ",
"           ",
"           ",
"           "},
/* lines 15 */
/* 2361 */
{
"         ",
"         ",
"         ",
"         ",
"  @@@@   ",
" @@   @  ",
" @@   @@ ",
"   @@@@@ ",
" @@   @@ ",
"@@    @@ ",
"@@   @@@ ",
" @@@@  @@",
"         ",
"         ",
"         "},
/* lines 15 */
/* 2362 */
{
"         ",
"@@@      ",
" @@      ",
" @@      ",
" @@ @@   ",
" @@@  @@ ",
" @@    @@",
" @@    @@",
" @@    @@",
" @@    @@",
" @@@  @@ ",
"@@@ @@   ",
"         ",
"         ",
"         "},
/* lines 15 */
/* 2363 */
{
"        ",
"        ",
"        ",
"        ",
"   @@@  ",
" @@   @@",
"@@     @",
"@@      ",
"@@      ",
"@@     @",
" @@   @@",
"   @@@  ",
"        ",
"        ",
"        "},
/* lines 15 */
/* 2364 */
{
"         ",
"     @@@ ",
"      @@ ",
"      @@ ",
"   @@ @@ ",
" @@  @@@ ",
"@@    @@ ",
"@@    @@ ",
"@@    @@ ",
"@@    @@ ",
" @@  @@@ ",
"   @@ @@@",
"         ",
"         ",
"         "},
/* lines 15 */
/* 2365 */
{
"         ",
"         ",
"         ",
"         ",
"  @@@@@  ",
" @     @ ",
"@@     @@",
"@@@@@@@@@",
"@@       ",
"@@      @",
" @@    @@",
"   @@@@  ",
"         ",
"         ",
"         "},
/* lines 15 */
/* 2366 */
{
"        ",
"    @@@ ",
"   @  @@",
"  @@  @@",
"  @@    ",
"@@@@@@@@",
"  @@    ",
"  @@    ",
"  @@    ",
"  @@    ",
"  @@    ",
"@@@@@@  ",
"        ",
"        ",
"        "},
/* lines 15 */
/* 2367 */
{
"        ",
"        ",
"        ",
"        ",
"  @@@@@@",
" @@  @@ ",
" @   @@ ",
" @   @  ",
"  @@@   ",
" @@     ",
" @@@@@@ ",
"@@   @@@",
" @     @",
"  @@@@@ ",
"        "},
/* lines 15 */
/* 2368 */
{
"          ",
"@@@       ",
" @@       ",
" @@       ",
" @@ @@@   ",
" @@@  @@  ",
" @@    @@ ",
" @@    @@ ",
" @@    @@ ",
" @@    @@ ",
" @@    @@ ",
"@@@@  @@@@",
"          ",
"          ",
"          "},
/* lines 15 */
/* 2369 */
{
"      ",
"   @  ",
"  @@@ ",
"   @  ",
"      ",
"@@@@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"@@@@@@",
"      ",
"      ",
"      "},
/* lines 15 */
/* 236a */
{
"      ",
"    @ ",
"   @@@",
"    @ ",
"      ",
" @@@@@",
"    @@",
"    @@",
"    @@",
"    @@",
"    @@",
"@@  @@",
"@@  @ ",
" @@@  ",
"      "},
/* lines 15 */
/* 236b */
{
"          ",
"@@@       ",
" @@       ",
" @@   @@@ ",
" @@   @   ",
" @@  @    ",
" @@ @@@   ",
" @@@ @@   ",
" @@   @@  ",
" @@   @@  ",
" @@    @@ ",
"@@@@   @@@",
"          ",
"          ",
"          "},
/* lines 15 */
/* 236c */
{
"      ",
"@@@@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"  @@  ",
"@@@@@@",
"      ",
"      ",
"      "},
/* lines 15 */
/* 236d */
{
"            ",
"            ",
"            ",
"            ",
"@@@ @@ @@@  ",
" @@@ @@  @@ ",
" @@  @@  @@ ",
" @@  @@  @@ ",
" @@  @@  @@ ",
" @@  @@  @@ ",
" @@  @@  @@ ",
"@@@@ @@@ @@@",
"            ",
"            ",
"            "},
/* lines 15 */
/* 236e */
{
"          ",
"          ",
"          ",
"          ",
"@@@ @@@   ",
" @@@   @  ",
" @@    @@ ",
" @@    @@ ",
" @@    @@ ",
" @@    @@ ",
" @@    @@ ",
"@@@@  @@@@",
"          ",
"          ",
"          "},
/* lines 15 */
/* 236f */
{
"         ",
"         ",
"         ",
"         ",
"   @@@   ",
" @@   @@ ",
"@@     @@",
"@@     @@",
"@@     @@",
"@@     @@",
" @@   @@ ",
"   @@@   ",
"         ",
"         ",
"         "},
/* lines 15 */
/* 2370 */
{
"         ",
"         ",
"         ",
"         ",
"@@@ @@@  ",
" @@@  @@ ",
" @@    @@",
" @@    @@",
" @@    @@",
" @@    @@",
" @@@  @@ ",
" @@ @@@  ",
" @@      ",
"@@@@     ",
"         "},
/* lines 15 */
/* 2371 */
{
"         ",
"         ",
"         ",
"         ",
"  @@@ @@@",
" @@  @@@ ",
"@@    @@ ",
"@@    @@ ",
"@@    @@ ",
"@@    @@ ",
" @@  @@@ ",
"  @@@ @@ ",
"      @@ ",
"     @@@@",
"         "},
/* lines 15 */
/* 2372 */
{
"        ",
"        ",
"        ",
"        ",
"@@@ @@@ ",
" @@@  @@",
" @@   @@",
" @@     ",
" @@     ",
" @@     ",
" @@     ",
"@@@@    ",
"        ",
"        ",
"        "},
/* lines 15 */
/* 2373 */
{
"        ",
"        ",
"        ",
"        ",
" @@@@@@ ",
"@@    @ ",
"@@@     ",
" @@@@   ",
"   @@@@ ",
"@    @@@",
"@@    @@",
"@ @@@@@ ",
"        ",
"        ",
"        "},
/* lines 15 */
/* 2374 */
{
"        ",
"        ",
"   @    ",
"  @@    ",
"  @@    ",
"@@@@@@@@",
"  @@    ",
"  @@    ",
"  @@    ",
"  @@    ",
"  @@   @",
"   @@@@ ",
"        ",
"        ",
"        "},
/* lines 15 */
/* 2375 */
{
"          ",
"          ",
"          ",
"          ",
"@@@  @@@@ ",
" @@    @@ ",
" @@    @@ ",
" @@    @@ ",
" @@    @@ ",
" @@    @@ ",
" @@   @@@ ",
"  @@@@  @@",
"          ",
"          ",
"          "},
/* lines 15 */
/* 2376 */
{
"          ",
"          ",
"          ",
"          ",
"@@@@@  @@@",
" @@     @ ",
"  @@   @  ",
"  @@   @  ",
"   @@ @   ",
"   @@ @   ",
"    @@    ",
"    @@    ",
"          ",
"          ",
"          "},
/* lines 15 */
/* 2377 */
{
"           ",
"           ",
"           ",
"           ",
"@@@@ @@ @@@",
" @@  @@  @ ",
" @@  @@  @ ",
"  @@@ @@ @ ",
"  @@@ @@@  ",
"   @@  @@  ",
"   @@  @@  ",
"   @@  @@  ",
"           ",
"           ",
"           "},
/* lines 15 */
/* 2378 */
{
"         ",
"         ",
"         ",
"         ",
"@@@@  @@@",
" @@    @ ",
"  @@  @  ",
"   @@@   ",
"   @@@   ",
"  @  @@  ",
" @    @@ ",
"@@@  @@@@",
"         ",
"         ",
"         "},
/* lines 15 */
/* 2379 */
{
"         ",
"         ",
"         ",
"         ",
"@@@@@ @@@",
" @@    @ ",
"  @@   @ ",
"  @@  @  ",
"   @@ @  ",
"   @@@   ",
"    @@   ",
"@@  @    ",
"@@  @    ",
" @@@     ",
"         "},
/* lines 15 */
/* 237a */
{
"        ",
"        ",
"        ",
"        ",
"@@@@@@@@",
"@    @@ ",
"    @@  ",
"   @@   ",
"  @@    ",
" @@     ",
"@@     @",
"@@@@@@@@",
"        ",
"        ",
"        "}
};

#endif

