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
/*	BB2.C	--	Part 2 of BBIND, large case binder.	*/

#include "OBJ.H"
#include "BBind.h"



pass2() {
	char clen,tlen,want;
	int fix,fixer,add,i;

	want=wantmod[nummod];

	while (1) {
		if (inin+128 > endin) refill();
		switch(clen=*inin++) {
			case OEOF:		return;
			case OPUBLIC:	if (want) find(2);
							else {
								while (*inin++) ;
								inin+=2;
								}
							break;
			case OSTATIC:	if (want) find(3);
							else {
								while (*inin++) ;
								inin+=2;
								}
							break;
			case ORESERVE:	if (want) {
								num=inin->word;
								inin+=2;
								len=inin->word;
								inin+=2;
								if (labis[num] == LOTHER) {
									lab_off[num]=offs[curseg_alloc];
									offs[curseg_alloc]+=len;
									if (curseg == INESEG) {
										roundup(INESEG_ALLOC);
										up_addr(INESEG_ALLOC);
										}
									}
								else if (labis[num] == LTYPE) {
									labis[num]=LPUBLIC;
									((char *) lab_off[num])->eaddr=offs[curseg_alloc];
									offs[curseg_alloc]+=len;
									if (curseg == INESEG) {
										roundup(INESEG_ALLOC);
										up_addr(INESEG_ALLOC);
										}
									}
								}
							else inin+=4;
							break;
			case OLOCAL:	num=inin->word;
							if (want) {
								inin+=2;
								len=inin->word;
								inin+=2;
								if (labis[num] == LOTHER || labis[num] == LTYPE) {
									labis[num]=LOTHER;
									lab_off[num]=len+offs[curseg];
									}
								}
							else inin+=4;
							break;
			case ODSEG:		curseg=INDSEG;
							curseg_res=INDSEG_RES;
							curseg_alloc=INDSEG_ALLOC;
							break;
			case OCSEG:		curseg=INCSEG;
							curseg_res=INESEG_RES;
							curseg_alloc=INESEG_ALLOC;
							break;
			case OESEG:		curseg=INESEG;
							curseg_res=INESEG_RES;
							curseg_alloc=INESEG_ALLOC;
							break;
			case ONAMEREL:	fix=inin->word;
							inin+=2;
							if (want)
							*fixat+=labis[fix] == LPUBLIC ?
								(unsigned)((char *) lab_off[fix])->eaddr:
								(unsigned)lab_off[fix];
							break;
			case OJUMPREL:	fix=inin->word;
							inin+=2;
							if (want)
							*fixat+=(labis[fix] == LPUBLIC ?
								(unsigned)((char *) lab_off[fix])->eaddr:
								(unsigned)lab_off[fix])-(unsigned)offs[curseg];
							break;
			case OPTYPE:	while (*inin++) ;
							inin+=2;
							inin+=*inin+1;
							break;

			case OMTYPE:	if (no_member) {
								while (*inin++) ;
								inin+=2;
								inin+=*inin+1;
								break;
								}
			case OLTYPE:	if (copt && want) {
								chkb(clen);
								do {
									chkb(toupper(*inin));
									}
								while (*inin++);
								chkw(inin->word);
								inin+=2;
								tlen=*inin;
								do {
									chkb(*inin++);
									}
								while (tlen--);
								outchk();
								break;
								}
							while (*inin++) ;
							inin+=2;
							inin+=*inin+1;
							break;
			case OLINE:		if (copt && want) {
								chkb(OLINE);
								chkw(inin->word);
								chkl(offs[curseg]);
								chkb(0xcc);
								outchk();
								}
							inin+=2;
							break;
			case ONAME:
			case OLNAME:	if (copt && want) {
								chkb(clen);
								do {
									chkb(toupper(*inin));
									}
								while (*inin++);
								outchk();
								break;
								}
							while (*inin++) ;
							break;

			case OBIG:		break;
			case OPTR:		num=inin->word;
							inin+=2;
							if (want == 0) break;
							if (lab_ptr4[num] == 0) {
								if (labis[num] == LPUBLIC) {
									found=(unsigned) lab_off[num];
									if (found->eptr4_at == 0) {
										found->eptr4_at=off_byte4;
										}
									else {
										lab_ptr4[num]=found->eptr4_at;
										goto do_ptr4_fix;
										}
									}
								lab_ptr4[num]=off_byte4;
								create_byte4(num);
								}
do_ptr4_fix:
							*fixat+=lab_ptr4[num];
							break;
			case OSEGPTR:	fix=inin->word;
							inin+=2;
							if (want) fixup_seg(curseg,fix);
							break;
			case OLNAMEREL:	fix=inin->word;
							inin+=2;
							if (want) fixup_ptr(curseg,fix);
							break;
			default:		clen-=128;
							if (want == 0) {
								inin+=clen;
								break;
								}
							offs[curseg]+=clen;
							if (curseg == INCSEG) {
								while (clen--)
									*incode++=*inin++;
								if (incode > &codebuf[1027]) ocode(1024,1);
								fixat=incode-2;
								}
							else if (curseg == INDSEG) {
								while (clen--)
									*indata++=*inin++;
								if (indata >&databuf[2051]) odata(2048,1);
								fixat=indata-2;
								}
							else {
								while (clen--)
									*inextra++=*inin++;
								if (inextra >&extrabuf[2051]) oextra(2048,1);
								fixat=inextra-2;
								}
			}
		}
	}

between() {
	char *hptr,*nameptr;
	int  i,*next,nout;
	unsigned *syms,nsym,add,*resptr,es_at,*word_ptr,init_from;
	long pad,allcode,alldata,allres;

	objat=0;

	/*	allocate reserved in order so public reserved's are in order */
	/*	data reserved comes first.	*/

	es_at=(to_long(offs[INCSEG])+15) >> 4;
	ds_at=es_at+((to_long(offs[INESEG])+15) >> 4);
	word_ptr=&offs[INDSEG_ALLOC];
	*(word_ptr+1)=ds_at;
	off_byte4=offs[INDSEG];
	offs[INDSEG]+=num_byte4*4;		/* space for 4 vyte pointers */
	offs[INDSEG_ALLOC]+=offs[INDSEG];
	
	found=memory;
	syms=inext;			/* put array of symbol name pointers at end
						 of symbols	*/
	nsym=0;
 	while (found < inext) {
		found+=2;
		nameptr=found;
		while (*found++) ;
		found->eptr4_at=0;			/* dont care if have 4 byte for this guy */

		if (found->eseg == INDSEG) {
			if (found->edefn == LRESERVE) {
				add=found->eaddr;
				found->eaddr=offs[INDSEG_ALLOC];
				offs[INDSEG_ALLOC]+=add;
				}
			else if (found->edefn != LTYPERES) {
				word_ptr=&found->eaddr;
				*(word_ptr+1)=ds_at;
				}
			}
		if (found->edefn == LOTHER && found->eused) {
			puts("undefined PUBLIC - ");
			puts(nameptr);
			ocrlf();
			nerr++;
			}
		if (&syms[nsym] < memlast && (found->eused || found->typlen >1))
			syms[nsym++]=nameptr;
		found+=sizeof(*eptr)-2;
		found+=*found;		/* area of type info	*/
		}

	roundup(INDSEG_RES);
	roundup(INDSEG_ALLOC);
	if (to_long(offs[INDSEG_ALLOC])+to_long(offs[INDSEG_RES])
		-(((long)ds_at) << 4) > 65535) ferror("over 64K data","");

	/* must find address of static reserved vars	*/

	found=memory;
 	while (found < inext) {
		found+=2;
		nameptr=found;
		while (*found++) ;
		if (found->edefn == LTYPERES && found->eseg == INDSEG)
			found->eaddr+=offs[INDSEG_ALLOC];
		found+=sizeof(*eptr)-2;
		found+=*found;		/* area of type info	*/
		}

	/*  now allocate the reserved that are in eseg. they come after dseg
		reserved.	*/

	offs[INESEG_ALLOC]=offs[INDSEG_ALLOC]+offs[INDSEG_RES];
	roundup(INESEG_ALLOC);
	up_addr(INESEG_ALLOC);
	
	found=memory;
 	while (found < inext) {
		found+=2;
		while (*found++) ;

		if (found->eseg == INESEG) {
			if (found->edefn == LRESERVE) {
				add=found->eaddr;
				if (((long)(unsigned) offs[INESEG_ALLOC]) + add > 65535)
					up_addr(INESEG_ALLOC);
				found->eaddr=offs[INESEG_ALLOC];
				offs[INESEG_ALLOC]+=add;
				}
			else if (found->edefn != LTYPERES) {
				word_ptr=&found->eaddr;
				*(word_ptr+1)+=es_at;
				}
			}
		found+=sizeof(*eptr)-2;
		found+=*found;		/* area of type info	*/
		}

	/* must find address of static reserved vars	*/

	found=memory;
 	while (found < inext) {
		found+=2;
		nameptr=found;
		while (*found++) ;
		if (found->edefn == LTYPERES && found->eseg == INESEG)
			found->eaddr+=offs[INESEG_ALLOC];
		found+=sizeof(*eptr)-2;
		found+=*found;		/* area of type info	*/
		}


	if (pfile != -1 || copt) printsym(nsym,syms);

/*	calculate totals for public list.	*/

	allcode=to_long(offs[INCSEG]);
	init_from=offs[INDSEG];
	allres=to_long(offs[INESEG_ALLOC])+to_long(offs[INESEG_RES])
		-(((long)ds_at) << 4)-init_from;
	alldata=to_long(offs[INESEG])+init_from+allres;

/*	calculate start of each section of module.	*/

	fixout=0;
	codeout=(0x1c+((long) ihead.relo) *4 +15) & 0xfffffff0;
	extraout=codeout+(((long)es_at) << 4);
	dataout=codeout+(((long)ds_at) << 4);
	byte4out=dataout+off_byte4;

/*	generate and output the header sector	*/

	/*	calculate offsets and totals for code, data etc. */

	pad=(((long)ds_at) << 4)+init_from;
	ihead.pages_plus=pad & 511;
	ihead.pages=(pad+codeout+511) >> 9;
	ihead.hsize=codeout >> 4;
	ihead.minmem=(allres >> 4)+100;
	ihead.regss=ds_at;
	ihead.regsp=init_from+1024;
	
	incode=codebuf;
	indata=databuf;
	inextra=extrabuf;
	infix=fixbuf;
	inbyte4=byte4buf;
	_move(sizeof(ihead),&ihead,fixbuf);
	((char *)infix)+=sizeof(ihead);

	next=&initobj[sizeof(initobj)-7];
	*next++=init_from;
	*next++=(allres + 15) >> 4;
	*next++=stacklen ? stacklen : 0x2000;
	if ((outfile=creat(outname)) == -1)
		ferror("cannot create",outname);

/*	set offsets to real addresses.	*/

	offs[INCSEG]=0;
	offs[INDSEG]=((long)ds_at) << 16;	
	offs[INESEG]=((long)es_at) << 16;	

/*	print the CODE= message	*/
	if (pfile != -1) {
		fputs("\ncode=",pfile);
		ohl(allcode);
		fputs("H  data=",pfile);
		ohl(alldata);
		fputs("H  DS=",pfile);
		oh(ds_at);
		fputs("H  stack and locals=",pfile);
		if (stacklen) oh(stacklen);
		else oh(0x2000);
		fputs("H\n",pfile);
		}
	}
