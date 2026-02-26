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
/*	BB3.C	--	Part 3 of BBIND, large case binder.	*/

#include "OBJ.H"
#include "BBind.h"

/*	print the symbols for 'P' option	*/

printsym(nsym,syms)
	unsigned nsym,syms[]; {

	unsigned temp,*word_ptr;
	int gap,i,j,column;
	char *nameptr,*found,nlen;

/*	sort the symbols	*/

	for (gap=nsym/2; gap > 0; gap/=2)
		for (i=gap; i < nsym; i++)
			for (j=i-gap; j >= 0 && sover(syms[j],syms[j+gap]); j-=gap) {
				temp=syms[j];
				syms[j]=syms[j+gap];
				syms[j+gap]=temp;
				}


/*	output public sysbols for checkout option	*/

	if (copt) {
		for (i=0; i < nsym; i++) {
			found=syms[i];
			if (*found == '_' && under_opt == 0) continue;
			chkb(OPTYPE);
			do {
				if (*found != '_' || *(found+1)) chkb(*found);
				}
			while (*found++);
			chkl(found->edefn == LOTHER ? 0: found->eaddr);
			if (found->etype[0] == 0) {
				if (found->eseg != INCSEG) {
					found->etype[0]=1;			/* set data to int type */
					found->etype[1]=CINT;
					}
				else { /* assume label */
					found->etype[0]=1;
					found->etype[1]=CLAB;
					}
				}
			found=found->etype;
			nlen=*found;
			do {
				chkb(*found++);
				}
			while (nlen--);
			outchk();
			}
		if (pfile == -1) return;
		}

#endif

/*	print the symbols	*/

	column=0;
	for (i=0; i < nsym; i++) {
		nameptr=found=syms[i];
		if (*found == '_' && under_opt == 0) continue;
		while (*found++) ;

		if (column >= 60) {
			fputs("\r\n",pfile);
			column=0;
			}
		else if (column >= 40) while (column < 60) {
			column++;
			putc(' ',pfile);
			}
		else if (column >= 20) while (column < 40) {
			column++;
			putc(' ',pfile);
			}
		else if (column) while (column < 20) {
			column++;
			putc(' ',pfile);
			}
		if (column+strlen(nameptr) >= 79) {
			fputs("\r\n",pfile);
			column=0;
			}
		word_ptr=&found->eaddr;
		if (found->eseg == INDSEG) fputs("  DS",pfile);
		else oh(*(word_ptr+1));
		putc(':',pfile);
		oh(*word_ptr);
		putc(' ',pfile);
		column+=10;
		while (*nameptr) {
			putc(*nameptr++,pfile);
			column++;
			}
		}
	}


/*	return true if first string is greater than second	*/

sover(stra,strb)
	char *stra,*strb; {

	do {
		if (*stra > *strb) return 1;
		if (*stra < *strb++) return 0;
		}
	while (*stra++);
	return 0;
	}






find(pass)
	char pass; {
	char hashno,ch,mlen,nlen,*target,name[80];
	int  *next,tlen,dif;

	hashno=nlen=0;
	do {
		ch=*inin++;
		if (ch >= 'a') ch-=32;
		name[nlen++]=ch;
		hashno+=ch;
		}
	while (ch);

	name[nlen]='!';

	hashno+=nlen;
	hashno&=31;
	num=0;
				/* get the variable number	*/
	num=inin->word;
	inin+=2;

	next=&hash[hashno];
	while (*next) {
		found=*next+2;
		mlen=0;
		while (*found++ == name[mlen++]) ;
		if (mlen > nlen) {
			found--;
			if (pass == 0) {		/* pass 0 is debug information */
				tlen=*inin;
				target=found->etype;
				if (*target == 0) {
					inin+=tlen+1;
					}
				else {
					dif=0;
					do {
						if (*target++ != *inin++) dif=1;
						}
					while (tlen--);
					if (dif) {
						puts("conflicting types for ");
						puts(name);
						puts(" in ");
						puts(inname);
						putchar('\n');
						}
					}
				}
			else {
				/*	pass 3 is when a static is found.	*/

				if (pass == 3) {
					if (found->edefn != LTYPE) return; /* ignore static if
															have a public.	*/
					if (labis[num] == LOTHER) {
						labis[num]=LTYPE;
						(unsigned) lab_off[num]=found;
						return;
						}
					}
				if (found->edefn >= LTYPE) {
					found->eseg=curseg;
					found->edefn=LOTHER;
					found->enlen=nlen;
					}
				if (found->eseg != curseg && pass == 1)
					error("different segments for",name);

				if (pass) {
					labis[num]=LPUBLIC;
					(unsigned) lab_off[num]=found;
					}
				}
			return;
			}
		next=*next;
		}
	*next=inext;
	inext->word=0;
	found=inext+2;
	mlen=0;
	do
		*found++=name[mlen];
	while (name[mlen++]);
	found->eseg=curseg;
	found->eused=0;
	found->eptr4_at=0;
	found->edefn=pass ? LOTHER: LTYPE;
	found->enlen=nlen;
	inext=found+sizeof(*eptr)-2;
	if (pass == 0) {
		tlen=*inin;
		*inext++=tlen+2;		/* size of type plus type length fields	*/
		do {
			*inext++=*inin++;
			}
		while (tlen--);
		}
	else if (copt) {
		*inext++=3;
		*inext++=0;
		*inext++=0;
		}
	else *inext++=1;
	if (inext >= memlast)
		ferror("too many total PUBLICs in ",inname);
	if (pass) {
		labis[num]=pass == 3 ? LTYPE: LPUBLIC;
		(unsigned) lab_off[num]=found;
		}
	}


/*	CREATE_byte4  --	create a four byte pointer.	*/

create_byte4(fix)
	int  fix; {
	unsigned *temp_fixat;

	temp_fixat=fixat;
	*inbyte4=0;					/* zero 4 byte pointer	*/
	fixat=((char *)inbyte4)+2;
	fixup_ptr(-1,fix);
	fixat=temp_fixat;
	off_byte4+=4;
	inbyte4++;
	if (inbyte4 == &byte4buf[100]) {
		obyte4(sizeof(byte4buf));
		inbyte4=byte4buf;
		}
	}
	


/*	FIXUP_PTR  --	add a four byte fixup at fixat.	*/

fixup_ptr(segin,fix)
	int  segin,fix; {

	*(fixat-1)+=labis[fix] == LPUBLIC ?
		(unsigned)((char *) lab_off[fix])->eaddr:
		(unsigned)lab_off[fix];
	fixup_seg(segin,fix);
	}


/*	FIXUP_SEG  --	do a segment fixup at fixat + 1.	*/

fixup_seg(segin,fix)
	int  segin,fix; {
	unsigned *wptr;

	wptr=labis[fix] == LPUBLIC ?
		&((char *) lab_off[fix])->eaddr:
		&lab_off[fix];
	*fixat+=*(wptr+1);
	if (segin == -1) { 	/*	do the fixup for a 4 byte pointer */
		*((unsigned *) infix)++=off_byte4+2;
		*((unsigned *) infix)++=ds_at;
		}
	else *infix++=offs[segin]-2;
	if (infix == &fixbuf[200]) {
		ofix(sizeof(fixbuf));
		infix=fixbuf;
		}
	}



/*	UP_ADDR  --	make the segment part of an address as large as possable */

up_addr(segis)
	int  segis; {
	/*char *addrp;*/
	struct long_addr *addrp;

	while (((unsigned) offs[segis]) & 15) add_zero(segis);
	addrp=&offs[segis];
	addrp->seg_part+=(addrp->off_part >> 4);
	addrp->off_part=0;
	}


/*	ROUNDUP  --	force segment to an even boundry */

roundup(segis)
	int  segis; {

	if (((int)offs[segis]) & 1) add_zero(segis);
	}


/*	ADD_ZERO  -- add a zero contents to a segment.	*/

add_zero(segis)
	int  segis; {

	if ((++(int)offs[segis]) == 0) offs[segis]+=0x10000000;
	if (pass == 2) {
		if (segis == INDSEG) *indata++=0;
		else if (segis == INCSEG) *incode++=0;
		else *inextra++=0;
		}
	}


/*	TO_ADDR  --	convert a long to an address.	*/

long to_addr(addr)
	long addr; {

	return ((addr & 0xffff0) << 4) +(((int)addr) & 15);
	}


long to_long(off,seg)
	unsigned off,seg; {

	return (((long)seg) << 4)+off;
	}



refill() {
	char *temp;
	int  numbytes;

	if (endin < &inbuf[4096]) return;
	temp=inbuf;
	while (inin < endin) *temp++=*inin++;
	if ((numbytes=read(infile[filen],temp,4096)) == -1)
		ferror("cannot read ",inname);
	endin=temp+numbytes;
	inin=inbuf;
	}

/*	output data or code	*/

odata(bytes,more)
	int  bytes;
	char more; {
	int  nout;
	int  i,num;

	if (objat != dataout) {
		if (lseek(outfile,dataout,0) == -1)
			ferror("cannot seek ",outname);
		objat=dataout;
		}
	oany(databuf,bytes);
	objat+=bytes;
	dataout+=bytes;
	if (more) {
		num=indata-&databuf[bytes];
		indata=&databuf[num];
		_move(num,&databuf[bytes],databuf);
		}
	}

ocode(bytes,more)
	int  bytes;
	char more; {
	int  nout;
	int  i,num;

	if (objat != codeout) {
		if (lseek(outfile,codeout,0) == -1)
			ferror("cannot seek ",outname);
		objat=codeout;
		}
	oany(codebuf,bytes);
	objat+=bytes;
	codeout+=bytes;
	if (more) {
		num=incode-&codebuf[bytes];
		incode=&codebuf[num];
		_move(num,&codebuf[bytes],codebuf);
		}
	}

oextra(bytes,more)
	int  bytes;
	char more; {
	int  nout;
	int  i,num;

	if (objat != extraout) {
		if (lseek(outfile,extraout,0) == -1)
			ferror("cannot seek ",outname);
		objat=extraout;
		}
	oany(extrabuf,bytes);
	objat+=bytes;
	extraout+=bytes;
	if (more) {
		num=inextra-&extrabuf[bytes];
		inextra=&extrabuf[num];
		_move(num,&extrabuf[bytes],extrabuf);
		}
	}


ofix(bytes)
	int  bytes; {
	int  num;

	if (objat != fixout) {
		if (lseek(outfile,fixout,0) == -1)
			ferror("cannot seek ",outname);
		objat=fixout;
		}
	oany(fixbuf,bytes);
	objat+=bytes;
	fixout+=bytes;
	}

obyte4(bytes)
	int  bytes; {
	int  num;

	if (objat != byte4out) {
		if (lseek(outfile,byte4out,0) == -1)
			ferror("cannot seek ",outname);
		objat=byte4out;
		}
	oany(byte4buf,bytes);
	objat+=bytes;
	byte4out+=bytes;
	}

oany(from,num)
	char *from;
	int  num; {

	if (write(outfile,from,num) == -1)
		ferror("cannot write ",outname);
	}

endup() {
	unsigned max,fbytes;

	fbytes=infix;
	fbytes-=fixbuf;
	if (fbytes) ofix(fbytes);
	if (incode-codebuf) ocode(incode-codebuf,0);
	if (inextra-extrabuf) oextra(inextra-extrabuf,0);
	if (indata-databuf) odata(indata-databuf,0);
	fbytes=inbyte4;
	fbytes-=byte4buf;
	if (fbytes) obyte4(fbytes);
	if (close(outfile) == -1)
		ferror("cannot close ",outname);

	if (pfile != -1) {
		if (close(pfile) == -1)
			ferror("cannot close ",pname);
		}
	if (copt) {
		chkb(OEOF);
		outchk();
		if (chkat-chkbuf && write(chkfile,chkbuf,chkat-chkbuf) == -1)
			ferror("cannot write",chkname);
		if (close(chkfile) == -1) ferror("cannot close",chkname);
		}

	max=(inext-memory)/((memlast-memory)/100);
	if (max > util) util=max;
	nummod/=3;
	if (see_exit == 0) {
		puts("end of BBIND        ");
		onum(util > nummod ? util: nummod);
		puts("% utilization    ");
		}
	if (nerr) {
		onum(nerr);
		puts(" errors");
		}
	}

/*	output to .CHK file	*/

char chkline[80];
int  cindex;

chkb(ch)
	char ch; {
	chkline[cindex++]=ch;
	}

outchk() {
	int  i;

	chkbyte(chkline[0]);
	chkbyte(cindex+1);
	i=1;
	while (i < cindex)
		chkbyte(chkline[i++]);
	cindex=0;
	}


	
chkbyte(ch)
	char ch; {

	if (chkat == &chkbuf[CHKSIZE]) {
		if (copt && write(chkfile,chkbuf,CHKSIZE) == -1)
			ferror("cannot write",chkname);
		chkat=chkbuf;
		}
	*chkat++=ch;
	}

chkw(wrd)
	unsigned wrd; {

	chkb(wrd);
	chkb(wrd>>8);
	}

chkl(worda,wordb)
	unsigned worda,wordb; {

	chkw(worda);
	chkw(wordb);
	}




ferror(str1,str2)
	char *str1,*str2; {

	ocrlf();
	puts(str1);
	puts(str2);
	puts("     BBIND abandoned\n");
	exit(2);
	}

error(str1,str2)
	char *str1,*str2; {

	puts(inname);
	puts(" - ");
	puts(str1);
	putchar(' ');
	puts(str2);
	ocrlf();
	nerr++;
	}

ocrlf() {

	putchar(10);
	}

ohn(ch)
	char ch; {

	ch=(ch&15)+'0';
	putc(ch > '9' ? ch+7: ch,pfile);
	}

oh(num)
	int  num; {

	ohn(num>>12);
	ohn(num>>8);
	ohn(num>>4);
	ohn(num);
	}

ohl(low,hi)
	unsigned low,hi; {

	ohn(hi);
	oh(low);
	}


onum(num)
	int  num; {
	if (num > 9) onum(num/10);
	putchar(num%10+'0');
	}
