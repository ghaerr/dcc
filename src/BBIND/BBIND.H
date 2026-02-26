/*
 *  Released under the GNU GPL.  See http://www.gnu.org/licenses/gpl.txt
 *
 *  This program is part of the DeSmet C Compiler
 *
 *  DeSmet C is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundatation; either version 2 of the License, or any
 *  later version.
 *
 *  DeSmet C is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 */
#define NUMARG	100
#define ABSIZE	2000
#define NUMLAB	2000

#define LOTHER		0
#define LPUBLIC		1
#define LRESERVE	2
#define LTYPE		3
#define LTYPERES	4

#define CINT	2
#define CLAB	7
#define CFUN	253

#define INCSEG	1
#define INESEG	2
#define INDSEG	3
#define INDSEG_ALLOC 4
#define INDSEG_RES 5
#define INESEG_ALLOC 6
#define INESEG_RES 7

#define FOPT	0
#define FOPEN	1
#define FCLOSED	2

#define CONTZ	26
	char inbuf[4234],*inin,*endin,codebuf[1162],*incode;
	char databuf[2182],extrabuf[2182],*indata,*inextra,filen;
	char inname[65],outname[65],chkname[65];
	int  infile[NUMARG],outfile,*fixat,numab,nerr,chkfile;
	char isopen[NUMARG],*inext,numopen,labis[NUMLAB];
	int  curseg,curseg_res,curseg_alloc;
	char pass,no_member;
	long lab_off[NUMLAB],fixbuf[200],byte4buf[100],*infix,*inbyte4;


	/*   offs indexes are given by INCSEG, INDSEG etc.	*/

	long offs[INESEG_RES+1];

	unsigned hash[32],stacklen,num_byte4,off_byte4,ds_at;
	int  num_public_patch;
	char *public_patch[NUMLAB/6];
	unsigned lab_ptr4[NUMLAB];
	long codeout,dataout,extraout,fixout,byte4out;
	unsigned util;
	unsigned len,num,nummod;
	char copt,aopt,hopt,under_opt,*found,islib,see_exit;
	char *pname,*fileat[NUMARG];

	char argbuf[ABSIZE];		/* -f arguments go here	*/
	long objat;
	long to_long(),to_addr();

#define CHKSIZE	512
	char chkbuf[CHKSIZE],*chkat;
#define MAXMOD 500
	char wantmod[MAXMOD];
	char *memory,*memlast;


	struct {char eseg,edefn,enlen,eused; long eaddr;unsigned eptr4_at;
			char typlen, etype[1]; } *eptr;
	union {int word; char byte; };
	struct long_addr {unsigned off_part,seg_part;} ;

	extern char libname[65];
	extern int  pfile,numfile;
	extern char initobj[52];
	extern struct header {unsigned sigture,pages_plus,pages,relo,hsize,minmem,
			hilo,regss,regsp,csum,regip,regcs,firstr,ov;}
			ihead;

	