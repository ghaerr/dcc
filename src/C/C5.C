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
/* c5.C					PART 5 OF C88 COMPILER	*/
		
#include "PASS1.H"
#include "NODES.H"
#if CHECK
#include "OBJ.H"
#endif

heir23(node)				/* operater is mul and div (* and /) */
	int  node[]; {
	int  nodeo[3],node2[2];
	char lval,bv;
	lval=heir24(node);
	while (heir == 23) {
		nodeo[0]=bv=bvalue;
		nodeo[1]=node[0];
		tokit();
		heir24(node2);
		nodeo[2]=node2[0];
		if (nodeo[1] == 0 || nodeo[2] == 0) nooper();
		node[0]=tree3(nodeo);
		if (bv != MOD) node[1]=maxtype(node[1],node2[1]);
		else node[1]=nofloat(node[1],node2[1]);
		lval=0;
		}
	return lval;
	}
heir24(node)				/* operater is prefix (* & - ! ++ --) */
							/* or SIZEOF */
	int  node[]; {
	int  nodeo[3],backup;
	char lval,needrp,*bptr,savenest;
	char savestor;
	unsigned saveat,saveloc,savetype;

	if (heir == RESERVED && bvalue == RSIZEOF) {
		tokit();
		needrp=0;
		backup=cur;
		savenest=nested;
		/*	save add information in case initilizing a variable */
		savestor=addstor;
		savetype=addtype;
		saveat=addat;
		saveloc=addloc;
		if (ifch('(')) {
			if (specs(SUNSTOR)) {
				node[0]=oconst(dsize(abstract(0)));
				node[1]=int_type;
				addstor=savestor;
				addtype=savetype;
				addat=saveat;
				addloc=saveloc;
				return 0;
				}
			else {
				addstor=savestor;
				cur=backup;
				nested=savenest;
				tokit();
				needrp=1;
				}
			}
/*	if SIZEOF, do not allow a logical expression.	*/
		heir24(node);
		addstor=savestor;
		addtype=savetype;
		addat=saveat;
		addloc=saveloc;
		node[0]=oconst(dsize(node[1]));
		node[1]=int_type;
		if (needrp) notch(')');
		return 0;
		}

	switch(curch) {
		case '*':
			if (heir == 23) {
				tokit();
				heir24(node);
				bptr=node[1];
				if (*bptr == PTRTO) bptr++;
				else if (*bptr == ARRAY) bptr+=3;
				else {
					if (is_big || (*bptr != CINT && *bptr != CUNSG))
						error("illegal indirection");
					bptr=c_type;
					}
				node[1]=bptr;
				if (*bptr == ARRAY) return 0;
				nodeo[0]=IND+xtypeof(bptr);
				nodeo[1]=node[0];
				if (nodeo[1] == 0) nooper();
				node[0]=tree2(nodeo);
				return 1;
				}
			break;
		case '&':
			if (heir == 18) {
				tokit();
				lval=heir24(node);
				bptr=node[1];
				if (*bptr == ARRAY || *bptr == FUNCTION)
					return 0;
				if (lval == 0 && *bptr != CLABEL && *bptr != CSTRUCT) {
					error("illegal address");
					return 0;
					}
				nodeo[0]=TA;
				nodeo[1]=node[0];
				if (nodeo[1] == 0) nooper();
				node[0]=tree2(nodeo);
				node[1]=abstract(node[1]);
				return 0;
				}
			break;
		case '-':
			if (heir == 22) {
				tokit();
				if (heir == CONSTANT) {
					wvalue=-wvalue;
					heir24(node);
					return 0;
					}
				else if (heir == LCONSTANT) {
					dvalue=-dvalue;
					heir24(node);
					return 0;
					}
				else if (heir == UCONSTANT) {
					dvalue=0-(long)wvalue;
					heir=LCONSTANT;
					heir24(node);
					return 0;
					}
				else if (heir == ULCONSTANT) {
					dvalue=-dvalue;
					heir=LCONSTANT;
					heir24(node);
					return 0;
					}
				else if (heir == FFCONSTANT) {
					fvalue[1] ^=0x8000;
					heir24(node);
					return 0;
					}
				else if (heir == FDCONSTANT) {
					fvalue[3] ^=0x8000;
					heir24(node);
					return 0;
					}
				heir24(node);
				nodeo[0]=NEG;
				nodeo[1]=node[0];
				if (nodeo[1] == 0) nooper();
				node[0]=tree2(nodeo);
				return 0;
				}
		case '+':
			if (heir == 22) {
				tokit();
				heir24(node);
				return 0;
				}
			if (heir == 24) {
				nodeo[0]=bvalue;
				tokit();
				lval=heir24(node);
				if (lval == 0) { error("need lval"); return 0; }
				nodeo[1]=node[0];
				if (nodeo[1] == 0) nooper();
				nodeo[2]=scaled(node[1]);
				node[0]=tree3(nodeo);
				return 0;
				}
			break;
		case '!':
			if (heir == 24) {
				if (heir == CONSTANT) {
					wvalue=!wvalue;
					heir24(node);
					return 0;
					}
				nodeo[0]=NOT;
				tokit();
				lval=heir24(node);
				nodeo[1]=node[0];
				if (nodeo[1] == 0) nooper();
				node[0]=tree2(nodeo);
				node[1]=int_type;
				return 0;
				}
			break;
		case '\176':
				nodeo[0]=COMP;
				tokit();
				lval=heir24(node);
				nodeo[1]=node[0];
				if (nodeo[1] == 0) nooper();
				node[0]=tree2(nodeo);
				node[1]=nofloat(node[1],node[1]);
				return 0;
		default: ;
		}
	lval=heir25(node);
	if (heir == 24 && (curch == '+' || curch == '-')) {
		nodeo[0]=bvalue+2;
		tokit();
		if (lval == 0) { error("need lval"); return 0;};
		nodeo[1]=node[0];
		if (nodeo[1] == 0) nooper();
		nodeo[2]=scaled(node[1]);
		node[0]=tree3(nodeo);
		lval=0;
		}
	return lval;
	}

heir25(node)					/* look for a primary */
	int  node[]; {
	char lval,*bptr,really_stru;
	int  node2[2],nodeo[4],scal,struct_len;

	lval=heir26(node);
	if (heir == 25 && FIX(node[1]) == CSTRUCT) {	/* need offset of struct */
		node2[1]=node[0];
		node2[0]=TA+(is_big ? (PTRTO << 8) : (CUNSG << 8));
		node[0]=tree2(node2);
		}

	really_stru=0;
	while (1) {
		if (ifch('[')) {
			heir11(node2);
			if (node2[0] == 0) nooper();
			if (is_big && FIX(node2[1]) == PTRTO) error("illegal index");
			if (notch(']')) return 0;
			bptr=node[1];
			if ((scal=scaled(bptr)) != 1) scale(node2,scal);
			nodeo[0]=ADD;
			nodeo[1]=node[0];
			nodeo[2]=node2[0];
			node[0]=tree3(nodeo);
			if (*bptr == ARRAY) bptr+=3;
			else if (*bptr == PTRTO) bptr++;
			else {
				if (is_big)	error("illegal indirection");
				bptr=c_type;
				}
			node[1]=bptr;
			lval=0;
			really_stru=0;
			if (*bptr != ARRAY && *bptr != CSTRUCT) {
				nodeo[0]=IND+xtypeof(bptr);
				nodeo[1]=node[0];
				node[0]=tree2(nodeo);
				lval=1;
				}
			else if (*bptr == CSTRUCT) really_stru=1;
			}
		else if (heir == 25) {
			tokit();
			if (is_big && FIX(node[1]) != PTRTO && FIX(node[1]) != CSTRUCT)
				error("illegal indirection");
			if(!find_member(node[1]))
				return 0;
			if (heir != OPERAND || m_operand(nameat)->nstor != SMEMBER)
				{ error("need member"); return 0;}
			if (m_operand(nameat)->noff) {
				nodeo[1]=oconst(m_operand(nameat)->noff);
				nodeo[0]=ADD;
				nodeo[2]=node[0];
				node[0]=tree3(nodeo);
				}
			lval=0;
			really_stru=0;
			if (m_operand(nameat)->ntype[0] != ARRAY && m_operand(nameat)->ntype[0] != CSTRUCT) {
				nodeo[0]=IND+xtypeof(&m_operand(nameat)->ntype[0]);
				nodeo[1]=node[0];
				node[0]=tree2(nodeo);
				lval=1;
				}
			else if (m_operand(nameat)->ntype[0] == CSTRUCT) really_stru=1;
			node[1]=&m_operand(nameat)->ntype[0];
			tokit();
			}
		else if (curch == '(') {
			bptr=node[1];
			if (*bptr == FUNCTION) bptr++;
			else if (*bptr == PTRTO && *(bptr+1) == FUNCTION) bptr+=2;
			else {
				if (is_big) error("illegal indirection");
				else warning("indirect call");
				}
			nodeo[0]=CALL+xtypeof(bptr);
			nodeo[1]=node[0];
			for(wp=protohash[(int)nameat&31]; wp; wp=*wp)
				if(*(wp+1)==nameat) {
					nodeo[2]=args(*(wp+2));
					goto pfmt;
					}
			nodeo[2]=args(-1);
pfmt:		struct_type=bptr;
			nodeo[3]=struct_type->sttype == CSTRUCT ?
				struct_type->ststagat->staglen : 0;
			node[0]=tree4(nodeo);
			if (bptr != node[1]) node[1]=bptr; else node[1]=int_type;
			really_stru=0;
			lval=0;
			}
		else break;
		}

	/* if found a structure as an element of another structure or array,
		need to take contents of.	*/
	if (really_stru) {
		nodeo[1]=node[0];
		nodeo[0]=IND+(CSTRUCT << 8);
		node[0]=tree2(nodeo);
		}
	return lval;
	}

args(struct pargs *ap) {
	int  n;
	int  list[7],node[2],nodeo[4],val;
	char *ntype;

	tokit();
	n=7;
	if (ifch(')')) {
		if(ap!=0 && ap!=-1)
			error("not enough arguments");
		return 0;
		}
	do {
		if(ap==0) {
			error("too many arguments");
			return 0;
			}
		val=heir12(node);
		if (node[0] == 0) {
			error("missing argument");
			return 0;
			}
		if(ap != -1) {
			if(xtypeof(ap->ptype) != xtypeof(node[1])
			&& ap->ptype[0] < ARRAY && *((char*)node[1]) < ARRAY) {
				if(wopt) warning("argument type conversion");
				ntype=node[1];
				node[1]=node[0];
				node[0]=CAST+xtypeof(ap->ptype);
				if (is_big && val) {
				
					if (node[0] == CAST+(PTRTO << 8) && *ntype != PTRTO &&
					*ntype != CSTRUCT && *ntype != ARRAY && *ntype != FUNCTION)
						error("illegal indirection");
					}
				node[0]=tree2(node);
				node[1]=ap->ptype;
				}
			ap=ap->plink;
			}
		ntype=node[1];
		if (*ntype == CSTRUCT) {
			if (wopt) warning("structure argument");
			 /* need address of struct */
			nodeo[0]=TA+(is_big ? (PTRTO << 8) : (CUNSG << 8));
			nodeo[1]=node[0];
			nodeo[1]=tree2(nodeo);
			nodeo[0]=MOVE;
			nodeo[2]=0;
			struct_type=ntype;
			nodeo[3]=struct_type->ststagat->staglen;
			node[0]=tree4(nodeo);
			}
		list[--n]=node[0];
		if (n == 1) {
			list[0]=LST+6;
			list[6]=treev(7,list);
			n=6;
			}
		}
	while (ifch(','));
	if (notch(')')) return 0;
	if(ap!=0 && ap!=-1) {
		error("not enough arguments");
		return 0;
		}
	if (n == 6) val=list[6];
	else {
		list[0]=LST+7-n;
		val=tree1(list);
		treev(7-n,&list[n]);
		}
	return val;
	}

/*	type of a string. needed by odd casts.	*/
	struct {char ary; int length; char tp;} string_type={ARRAY,0,CCHAR};


heir26(node)					/* operand or constant or string */
	int  node[]; {
	int  nodeo[5],save,dim;
	char lval,*typad,slen,savenest,oldstor,toked,tp,*oldtype;

	if(ifch('(')) {
		oldstor=addstor;
		oldtype=addtype;
		if (specs(SUNSTOR)) {	/* check for a cast	*/
			save=abstract(0);
/*	dont allow a cast of a logical expression	*/
			lval=heir24(node);
			if (node[0] == 0) nooper();
			tp=FIX(node[1]);
			node[1]=node[0];
			node[0]=CAST+xtypeof(save);
			if (is_big && lval) {
				
				if (node[0] == CAST+(PTRTO << 8) && tp != PTRTO &&
				tp != CSTRUCT && tp != ARRAY && tp != FUNCTION)
					error("illegal indirection");
				}
			node[0]=tree2(node);
			node[1]=save;
			}
		else {
			addtype=oldtype;
			lval=heir11(node);
			notch(')');
			}
		addstor=oldstor;
		return lval;
		}

	toked=0;
	if (heir == UNDEF) {
		newname();
		toked=1;
		tokit();
		if (ininit) {
			toked=0;
			wp=mfree;		/*	remove the hash pointer for this guy	*/
			hash[hashno]=*wp;
			error("undefined variable");
			}
		else if (curch == '(') {
			addstor=m_operand(nameat)->nstor=SEXTERN;
			addat=nameat;
			m_operand(addat)->ntype[0]=FUNCTION;
			addtype=CINT;
			addloc=&m_operand(nameat)->ntype[1];
			atype(addtype);
#if	CHECK
			if (copt) objtype(OPTYPE,m_operand(addat)->noff);
#endif
			}
		else {			/* must be a local integer */
			error("undefined variable");        // FIXME was warning
			m_operand(nameat)->nstor=SAUTO;
			locoff-=2;
			m_operand(nameat)->noff=locoff;
			m_operand(nameat)->ntype[0]=CINT;
			mfree=&m_operand(nameat)->ntype[1];
			}
		goto nowop;
		}
	if (heir == OPERAND && m_operand(nameat)->nstor != STYPEDEF) {
nowop:	nodeo[1]=nodeo[2]=nodeo[3]=0;
		if (m_operand(nameat)->nstor < SAUTO) nodeo[1]=m_operand(nameat)->noff;
		else nodeo[2]=m_operand(nameat)->noff;
		typad=&m_operand(nameat)->ntype[0];
		if (toked == 0) tokit();
		while (curch == '.' || curch == '[') {
			if (curch == '.') {
				if (*typad != CSTRUCT) {
					if (*typad != PTRTO) {error("need structure");return 0;}
					break;
					}
				tokit();
				find_member(typad);
				if (heir != OPERAND || m_operand(nameat)->nstor != SMEMBER) {
					error("need member");
					return 0;
					}
				/* if a zero offset, set flag for could be member of union */
				if (m_operand(nameat)->noff)
					nodeo[2]+=m_operand(nameat)->noff;
				else nodeo[3]=1;
				typad=&m_operand(nameat)->ntype[0];
				tokit();
				}
			else {
				if (*typad != ARRAY) break;
				while (*cur == LF && cur > savEnd) {
					cur=nestfrom[--nested];
					whitesp();
					}
				save=cur;
				savenest=nested;
				tokit();
				if (heir == CONSTANT) {
					dim=wvalue;
					tokit();
					if (ifch(']')) {
						typad+=3;
						dim=wvalue;
						nodeo[2]+=dsize(typad)*dim;
						continue;
						}
					}

				cur=save;
				nested=savenest;
				curch='[';
				break;
				}
			}
		nodeo[0]=OPND+xtypeof(typad);
		node[0]=tree4(nodeo);
		node[1]=typad;
		if (*typad < BITS) {
			if (*typad < CLABEL) return 1;
			}
		else if (*typad < FUNCTION || *typad > ARRAY) return 1;
		return 0;
		}
	if (heir == CONSTANT) {
		node[0]=oconst2(wvalue, CINT);
		node[1]=int_type;
		tokit();
		return 0;
		}
	if (heir == UCONSTANT) {
		node[0]=oconst2(wvalue, CUNSG);
		node[1]=u_type;
		tokit();
		return 0;
		}
	if (heir == LCONSTANT) {
		nodeo[0]=CONST+(CLONG<<8);
		nodeo[1]=dvalue;
		nodeo[2]=LONG(&dvalue)->hiword;
		node[0]=tree3(nodeo);
		node[1]=l_type;
		tokit();
		return 0;
		}
	if (heir == ULCONSTANT) {
		nodeo[0]=CONST+(CULONG<<8);
		nodeo[1]=dvalue;
		nodeo[2]=LONG(&dvalue)->hiword;
		node[0]=tree3(nodeo);
		node[1]=ul_type;
		tokit();
		return 0;
		}
	if (heir == FFCONSTANT) {
		nodeo[0]=CONST+(CFLOAT<<8);
		nodeo[1]=fvalue[0];
		nodeo[2]=fvalue[1];
		node[0]=tree3(nodeo);
		node[1]=ff_type;
		tokit();
		return 0;
		}
	if (heir == FDCONSTANT) {
		nodeo[0]=CONST+(CDOUBLE<<8);
		nodeo[1]=fvalue[0];
		nodeo[2]=fvalue[1];
		nodeo[3]=fvalue[2];
		nodeo[4]=fvalue[3];
		node[0]=tree5(nodeo);
		node[1]=fd_type;
		tokit();
		return 0;
		}
	if (heir == STRNG) {
		nodeo[0]=OPND+(ARRAY<<8);
		nodeo[1]=++ordinal;
		nodeo[2]=nodeo[3]=0;
		node[0]=tree4(nodeo);
		node[1]=&string_type;
		ctlb(1);
		ctlb(SSTATIC);
		ctlw(ordinal);
		ctlb(0);			/* name is null */
concatstring:
		ctlb(INITSTR);
		slen=0;
		while (string[slen] || string[slen+1] != 0xff) slen++;
		string_type.length=slen;
		ctlb(slen);
		i=0;
		while (i < slen) ctlb(string[i++]);
		tokit();
		if(heir==STRNG) goto concatstring;
		ctlb(0xFF);
		ctlb(INITEND);
		return 0;
		}
	node[0]=0;
	return 0;
	}

/*	create an abstract data type. if argument is not zero result type
	must go in table and argument points to thing address taken of.	*/

abstract(oldtype) 
	char *oldtype; {
	char oldstor,*oldloc,*oldt,*oldat,*absat;

	oldstor=addstor;
	oldloc=addloc;
	oldt=addtype;
	oldat=addat;
	WORD(mfree)->word=0;
	WORD(mfree+2)->byte=0;			/* null name */
	addat=mfree+3;
	m_operand(addat)->opcl=OPERAND;
	m_operand(addat)->nchain=0;
	m_operand(addat)->nlen=1;
	m_operand(addat)->nstor=addstor=SUNSTOR;
	m_operand(addat)->noff=0;
	addloc=&m_operand(addat)->ntype[0];
	if (oldtype) {
		*addloc++=PTRTO;
		addtype=oldtype;
		}
	else {
		getab(0);
		notch(')');
		}
	atype(addtype);
	addstor=oldstor;
	addloc=oldloc;
	addtype=oldt;
	absat=addat;
	addat=oldat;
	return (&m_operand(absat)->ntype[0]);
	}

getab(lvl)
	int lvl; {
	char *otype;

	while (1) {
		if (ifch('*')) {
			getab(1);
			*addloc++=PTRTO;
			return;
			}
		otype=addtype;
		if(lvl) while((heir==RESERVED && bvalue >= RCDECL && bvalue <= RPASCAL)
					|| (heir == UNDEF) || (heir == OPERAND))
			tokit();
		if(lvl) while(specs(0)) ifch(',');
		addtype=otype;
		if(heir == RELIPSE) tokit();
		if (ifch('(')) {
			if (curch != ')') {
				getab(1);
				notch(')');
				continue;
				}
			tokit();
			*addloc++=FUNCTION;
			}
		else if (ifch('[')) {
			if (ifch(']')) {
				*addloc++=PTRTO;
				return;
				}
			if (heir != CONSTANT) {
				error("missing dimension");
				return;
				}
			tokit();
			notch(']');
			*addloc++=ARRAY;
			WORD(addloc)->word=wvalue;
			addloc+=2;
			}
		else return;
		}
	}

oconst(con)
	char *con; {
	int  nodeo[2];
	if (con > 255) nodeo[0]=CONST+(CINT<<8);
	else nodeo[0]=CONST+(CCHAR<<8);
	nodeo[1]=con;
	return tree2(nodeo);
	}

oconst2(con, typ)
	char *con;
	int typ; {
	int  nodeo[2];
	nodeo[0]=CONST+(typ<<8);
	nodeo[1]=con;
	return tree2(nodeo);
	}


lookup_member(char * mptr) {

	while (mptr) {
		char *nptr = mptr - m_operand(mptr)->nlen;
		if (strcmp(nptr, string) == 0)
			return heir=*(nameat=mptr);
		mptr=m_stag(mptr)->schain;
		}
	return 0;
	}

/*	FIND_MEMBER	--	find a member name. use the stag's chain list if possible,
					otherwise look for any member name.	*/

find_member(typeat)
	char *typeat; {
	char *name_at,*string_at,*typeis,*memat;

	typeis=typeat;
	if (heir == OPERAND || heir == UNDEF) {
		while (*typeis == PTRTO) typeis++;
		if (*typeis == CSTRUCT) {
			struct_type=typeis;
			memat=struct_type->ststagat;		  /* address of stag */
			memat=m_stag(memat)->schain;		  /* address of first member */
			if(lookup_member(memat))
				return 1;
			}
		else if (*typeis == CCHAR) {
			wp = anon;
			while(wp) {
				struct_type = wp+1;
				memat=struct_type->ststagat;		  /* address of stag */
				memat=m_stag(memat)->schain;		  /* address of first member */
				if(lookup_member(memat))
					return 1;
				wp = *wp;
				}
			}
		else {
			error("Need structure");
			return 0;
			}
		}
	error("member not in structure");
	return 0;
	}
