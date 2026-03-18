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
/*	SQUISH.C		Object Squisher for C88	*/

/* --- ELKS changes --- */
#if __ia16__
#define char        unsigned char
#define _setmem(addr,count,byte)    memset(addr,byte,count)
#define _move(num,from,to)          memmove((char *)(to), (char *)(from), num)
#endif

#define O_RDONLY    0
/* ------------------- */

#include "OBJ.H"

#define NUMLAB	2000

#define LOTHER		0
#define LPUBLIC		1
#define LRESERVE	2

#define CINT	2
#define CFUN	253

#define INDSEG	1
#define INCSEG	2
#define INUSEG	3
#define INESEG	4
#define INPTR4	5

#define CONTZ	26

	char inbuf[1024*10+200],*inin,*endin,*inin_from,*outat,*contents;
	char outbuf[1024*10+200];
	char inname[65],outname[65];
	int  infile,outfile;
	char curseg,name[33],labis[NUMLAB],labseg[NUMLAB];
	unsigned offs[6],hash[32],labat[NUMLAB];
	char is_large;

	struct sym {int eseg; char edefn,enlen,eisext; int elen,eptr4_at; } *stable;
	typedef struct sym *sp;

	union un_word {int word; char byte;};
	#define WORD(v)		((union un_word *)(v))


	char *memory,*memlast;



main(argc,argv)
	int  argc;
	char *argv[]; {

	init(argc,argv);
	nextpass(1);
	endup();
	}

init(argc,argv)
	int  argc;
	char *argv[]; {
	char *argat,gotdot;
	int  nin,i,endn,ffile;
	char renname[65];

	xputs("Squisher for C88 and ASM88     V1.0    (c) Mark DeSmet, 1986\n", 1);
	if (argc < 2) ferror("missing filename","");
	strcpy(inname,argv[1]);
	i=0;
	while (inname[i] && inname[i] != '.') i++;
	if (inname[i] == 0) strcat(inname,".O");
	strcpy(outname,inname);
	if (rename(inname,"ctemp7") == -1) ferror("cannot rename ",inname);
	i=endn=0;
	while (inname[i]) {
		if (inname[i] == ':' || inname[i] == '\\') endn=i+1;
		i++;
		}
	strcpy(&inname[endn],"ctemp7");

	if ((infile=open(inname,O_RDONLY)) == -1)
		ferror("cannot open ",inname);
	inin=endin=&inbuf[1024*10];
	if ((outfile=creat(outname)) == -1)
		ferror("cannot create",outname);
	outat=outbuf;
	}



nextpass(pass)
	char pass; {
	char *argat;
	int  i;

	do {
		_setmem(labis,NUMLAB,LOTHER);
		_setmem(labseg,NUMLAB,0);
		pass1();
		if (inin >= endin) refill();
		if (*inin == CONTZ) inin=endin;
		}
	while (inin < endin);
	}

pass1() {
	char clen;
	unsigned i,*next,num,len;

	contents=0;
	offs[INCSEG]=0;
	offs[INDSEG]=0;

	while (1) {
		if (inin+128 > endin) refill();
		inin_from=inin;
		switch (clen=*inin++) {
			case OEOF:		copyit(0);
							return;
			case OBIG:		copyit(0);
							is_large=1;
							break;
			case OPUBLIC:	while(*inin++) ;
							num=WORD(inin)->word;
							inin+=2;
							labis[num]=LPUBLIC;
							copyit(0);
							break;
			case OSTATIC:	while(*inin++) ;
							inin+=2;
							break;
			case ORESERVE:	num=WORD(inin)->word;
							inin+=2;
							labis[num]=LPUBLIC;
							inin+=2;
							copyit(0);
							break;
			case OLOCAL:	num=WORD(inin)->word;
							inin+=2;
							len=WORD(inin)->word;
							inin+=2;
							if (curseg != INESEG) {
								labat[num]=len;
								labseg[num]=curseg == INCSEG ? OCFIX: ODFIX;
								}
							if (labis[num] != LOTHER || curseg == INESEG || is_large)
								copyit(0);
							break;
			case ODSEG:		curseg=INDSEG;
							if (*(outat-1) == OCSEG) *(outat-1)=ODSEG;
							else copyit(0);
							break;
			case OCSEG:		curseg=INCSEG;
							if (*(outat-1) == ODSEG) *(outat-1)=OCSEG;
							else copyit(0);
							break;
			case OESEG:		curseg=INESEG;
							copyit(0);
							break;
			case ONAMEREL:	num=WORD(inin)->word;
							inin+=2;
							if (labseg[num]) {
								WORD(outat-2)->word+=labat[num];
								*outat++=labseg[num];
								contents=0;
								}
							else copyit(0);
							break;

			case OJUMPREL:	num=WORD(inin)->word;
							inin+=2;
							if (labseg[num]) {
								WORD(outat-2)->word+=labat[num]-offs[curseg];
								}
							else copyit(0);
							break;

			case OPTYPE:
			case OMTYPE:
			case OLTYPE:	while (*inin++) ;
							inin+=2;
							inin+=*inin+1;
							break;
			case OLINE:		inin+=2;
							break;
			case ONAME:
			case OLNAME:	while (*inin++) ;
							break;

			case OPTR:		inin+=2;
							copyit(0);
							break;
			case OSEGPTR:
			case OLNAMEREL:	inin+=2;
							copyit(0);
							break;
			default:		if (clen <= 128)
								ferror("bad object file ",inname);
							clen-=128;
							offs[curseg]+=clen;
							if (contents && (*contents-128)+((int)clen) < 100) {
								*contents+=clen;
								_move(clen,inin,outat);
								outat+=clen;
								inin+=clen;
								}
							else {
								inin+=clen;
								contents=outat;
								copyit(1);
								}

			}
		}
	}


copyit(zero)
	char zero; {

	if (zero == 0) contents=0;
	_move(inin-inin_from,inin_from,outat);
	outat+=inin-inin_from;
	if (outat-outbuf > 1024*10+2) {
		if (write(outfile,outbuf,outat-outbuf-2) == -1)
			ferror("cannot write",outname);
		outbuf[0]=outbuf[outat-outbuf-2];
		outbuf[1]=outbuf[outat-outbuf-1];
		outat=&outbuf[2];
		contents=0;
		}
	}



refill() {
	char *temp;
	int  numbytes;

	if (endin < &inbuf[1024*10]) return;
	temp=inbuf;
	while (inin < endin) *temp++=*inin++;
	if ((numbytes=read(infile,temp,1024*10)) == -1)
		ferror("cannot read ",inname);
	endin=temp+numbytes;
	inin=inbuf;
	}

endup() {

	if (write(outfile,outbuf,outat-outbuf) == -1)
		ferror("cannot write",outname);
	close(infile);
	if (unlink(inname) == -1) ferror("cannot delete ",inname);
	close(outfile);
	xputs("end of SQUISH    \n", 1);
	}
	

ferror(str1,str2)
	char *str1,*str2; {

	xputs("\n", 1);
	xputs(str1, 1);
	xputs(str2, 1);
	xputs("     SQUISH abandoned\n", 1);
	exit(2);
	}

/* --------- ELKS ---------- */

xputs(str, fd)
    char *str; int fd; {

    return write(fd, str, strlen(str));
}
