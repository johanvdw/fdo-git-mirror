#!/usr/bin/env perl

# ====================================================================
# Written by Andy Polyakov <appro@fy.chalmers.se> for the OpenSSL
# project. The module is, however, dual licensed under OpenSSL and
# CRYPTOGAMS licenses depending on where you obtain it. For further
# details see http://www.openssl.org/~appro/cryptogams/.
# ====================================================================

# On PA-7100LC this module performs ~90-50% better, less for longer
# keys, than code generated by gcc 3.2 for PA-RISC 1.1. Latter means
# that compiler utilized xmpyu instruction to perform 32x32=64-bit
# multiplication, which in turn means that "baseline" performance was
# optimal in respect to instruction set capabilities. Fair comparison
# with vendor compiler is problematic, because OpenSSL doesn't define
# BN_LLONG [presumably] for historical reasons, which drives compiler
# toward 4 times 16x16=32-bit multiplicatons [plus complementary
# shifts and additions] instead. This means that you should observe
# several times improvement over code generated by vendor compiler
# for PA-RISC 1.1, but the "baseline" is far from optimal. The actual
# improvement coefficient was never collected on PA-7100LC, or any
# other 1.1 CPU, because I don't have access to such machine with
# vendor compiler. But to give you a taste, PA-RISC 1.1 code path
# reportedly outperformed code generated by cc +DA1.1 +O3 by factor
# of ~5x on PA-8600.
#
# On PA-RISC 2.0 it has to compete with pa-risc2[W].s, which is
# reportedly ~2x faster than vendor compiler generated code [according
# to comment in pa-risc2[W].s]. Here comes a catch. Execution core of
# this implementation is actually 32-bit one, in the sense that it
# operates on 32-bit values. But pa-risc2[W].s operates on arrays of
# 64-bit BN_LONGs... How do they interoperate then? No problem. This
# module picks halves of 64-bit values in reverse order and pretends
# they were 32-bit BN_LONGs. But can 32-bit core compete with "pure"
# 64-bit code such as pa-risc2[W].s then? Well, the thing is that
# 32x32=64-bit multiplication is the best even PA-RISC 2.0 can do,
# i.e. there is no "wider" multiplication like on most other 64-bit
# platforms. This means that even being effectively 32-bit, this
# implementation performs "64-bit" computational task in same amount
# of arithmetic operations, most notably multiplications. It requires
# more memory references, most notably to tp[num], but this doesn't
# seem to exhaust memory port capacity. And indeed, dedicated PA-RISC
# 2.0 code path provides virtually same performance as pa-risc2[W].s:
# it's ~10% better for shortest key length and ~10% worse for longest
# one.
#
# In case it wasn't clear. The module has two distinct code paths:
# PA-RISC 1.1 and PA-RISC 2.0 ones. Latter features carry-free 64-bit
# additions and 64-bit integer loads, not to mention specific
# instruction scheduling. In 64-bit build naturally only 2.0 code path
# is assembled. In 32-bit application context both code paths are
# assembled, PA-RISC 2.0 CPU is detected at run-time and proper path
# is taken automatically. Also, in 32-bit build the module imposes
# couple of limitations: vector lengths has to be even and vector
# addresses has to be 64-bit aligned. Normally neither is a problem:
# most common key lengths are even and vectors are commonly malloc-ed,
# which ensures alignment.
#
# Special thanks to polarhome.com for providing HP-UX account on
# PA-RISC 1.1 machine, and to correspondent who chose to remain
# anonymous for testing the code on PA-RISC 2.0 machine.


$0 =~ m/(.*[\/\\])[^\/\\]+$/; $dir=$1;

$flavour = shift;
$output = shift;

open STDOUT,">$output";

if ($flavour =~ /64/) {
	$LEVEL		="2.0W";
	$SIZE_T		=8;
	$FRAME_MARKER	=80;
	$SAVED_RP	=16;
	$PUSH		="std";
	$PUSHMA		="std,ma";
	$POP		="ldd";
	$POPMB		="ldd,mb";
	$BN_SZ		=$SIZE_T;
} else {
	$LEVEL		="1.1";	#$LEVEL.="\n\t.ALLOW\t2.0";
	$SIZE_T		=4;
	$FRAME_MARKER	=48;
	$SAVED_RP	=20;
	$PUSH		="stw";
	$PUSHMA		="stwm";
	$POP		="ldw";
	$POPMB		="ldwm";
	$BN_SZ		=$SIZE_T;
	if (open CONF,"<${dir}../../opensslconf.h") {
	    while(<CONF>) {
		if (m/#\s*define\s+SIXTY_FOUR_BIT/) {
		    $BN_SZ=8;
		    $LEVEL="2.0";
		    last;
		}
	    }
	    close CONF;
	}
}

$FRAME=8*$SIZE_T+$FRAME_MARKER;	# 8 saved regs + frame marker
				#                [+ argument transfer]
$LOCALS=$FRAME-$FRAME_MARKER;
$FRAME+=32;			# local variables

$tp="%r31";
$ti1="%r29";
$ti0="%r28";

$rp="%r26";
$ap="%r25";
$bp="%r24";
$np="%r23";
$n0="%r22";	# passed through stack in 32-bit
$num="%r21";	# passed through stack in 32-bit
$idx="%r20";
$arrsz="%r19";

$nm1="%r7";
$nm0="%r6";
$ab1="%r5";
$ab0="%r4";

$fp="%r3";
$hi1="%r2";
$hi0="%r1";

$xfer=$n0;	# accomodates [-16..15] offset in fld[dw]s

$fm0="%fr4";	$fti=$fm0;
$fbi="%fr5L";
$fn0="%fr5R";
$fai="%fr6";	$fab0="%fr7";	$fab1="%fr8";
$fni="%fr9";	$fnm0="%fr10";	$fnm1="%fr11";

$code=<<___;
	.LEVEL	$LEVEL
	.SPACE	\$TEXT\$
	.SUBSPA	\$CODE\$,QUAD=0,ALIGN=8,ACCESS=0x2C,CODE_ONLY

	.EXPORT	bn_mul_mont,ENTRY,ARGW0=GR,ARGW1=GR,ARGW2=GR,ARGW3=GR
	.ALIGN	64
bn_mul_mont
	.PROC
	.CALLINFO	FRAME=`$FRAME-8*$SIZE_T`,NO_CALLS,SAVE_RP,SAVE_SP,ENTRY_GR=6
	.ENTRY
	$PUSH	%r2,-$SAVED_RP(%sp)		; standard prologue
	$PUSHMA	%r3,$FRAME(%sp)
	$PUSH	%r4,`-$FRAME+1*$SIZE_T`(%sp)
	$PUSH	%r5,`-$FRAME+2*$SIZE_T`(%sp)
	$PUSH	%r6,`-$FRAME+3*$SIZE_T`(%sp)
	$PUSH	%r7,`-$FRAME+4*$SIZE_T`(%sp)
	$PUSH	%r8,`-$FRAME+5*$SIZE_T`(%sp)
	$PUSH	%r9,`-$FRAME+6*$SIZE_T`(%sp)
	$PUSH	%r10,`-$FRAME+7*$SIZE_T`(%sp)
	ldo	-$FRAME(%sp),$fp
___
$code.=<<___ if ($SIZE_T==4);
	ldw	`-$FRAME_MARKER-4`($fp),$n0
	ldw	`-$FRAME_MARKER-8`($fp),$num
	nop
	nop					; alignment
___
$code.=<<___ if ($BN_SZ==4);
	comiclr,<=	6,$num,%r0		; are vectors long enough?
	b		L\$abort
	ldi		0,%r28			; signal "unhandled"
	add,ev		%r0,$num,$num		; is $num even?
	b		L\$abort
	nop
	or		$ap,$np,$ti1
	extru,=		$ti1,31,3,%r0		; are ap and np 64-bit aligned?
	b		L\$abort
	nop
	nop					; alignment
	nop

	fldws		0($n0),${fn0}
	fldws,ma	4($bp),${fbi}		; bp[0]
___
$code.=<<___ if ($BN_SZ==8);
	comib,>		3,$num,L\$abort		; are vectors long enough?
	ldi		0,%r28			; signal "unhandled"
	addl		$num,$num,$num		; I operate on 32-bit values

	fldws		4($n0),${fn0}		; only low part of n0
	fldws		4($bp),${fbi}		; bp[0] in flipped word order
___
$code.=<<___;
	fldds		0($ap),${fai}		; ap[0,1]
	fldds		0($np),${fni}		; np[0,1]

	sh2addl		$num,%r0,$arrsz
	ldi		31,$hi0
	ldo		36($arrsz),$hi1		; space for tp[num+1]
	andcm		$hi1,$hi0,$hi1		; align
	addl		$hi1,%sp,%sp
	$PUSH		$fp,-$SIZE_T(%sp)

	ldo		`$LOCALS+16`($fp),$xfer
	ldo		`$LOCALS+32+4`($fp),$tp

	xmpyu		${fai}L,${fbi},${fab0}	; ap[0]*bp[0]
	xmpyu		${fai}R,${fbi},${fab1}	; ap[1]*bp[0]
	xmpyu		${fn0},${fab0}R,${fm0}

	addl		$arrsz,$ap,$ap		; point at the end
	addl		$arrsz,$np,$np
	subi		0,$arrsz,$idx		; j=0
	ldo		8($idx),$idx		; j++++

	xmpyu		${fni}L,${fm0}R,${fnm0}	; np[0]*m
	xmpyu		${fni}R,${fm0}R,${fnm1}	; np[1]*m
	fstds		${fab0},-16($xfer)
	fstds		${fnm0},-8($xfer)
	fstds		${fab1},0($xfer)
	fstds		${fnm1},8($xfer)
	 flddx		$idx($ap),${fai}	; ap[2,3]
	 flddx		$idx($np),${fni}	; np[2,3]
___
$code.=<<___ if ($BN_SZ==4);
	mtctl		$hi0,%cr11		; $hi0 still holds 31
	extrd,u,*=	$hi0,%sar,1,$hi0	; executes on PA-RISC 1.0
	b		L\$parisc11
	nop
___
$code.=<<___;					# PA-RISC 2.0 code-path
	xmpyu		${fai}L,${fbi},${fab0}	; ap[j]*bp[0]
	xmpyu		${fni}L,${fm0}R,${fnm0}	; np[j]*m
	ldd		-16($xfer),$ab0
	fstds		${fab0},-16($xfer)

	extrd,u		$ab0,31,32,$hi0
	extrd,u		$ab0,63,32,$ab0
	ldd		-8($xfer),$nm0
	fstds		${fnm0},-8($xfer)
	 ldo		8($idx),$idx		; j++++
	 addl		$ab0,$nm0,$nm0		; low part is discarded
	 extrd,u	$nm0,31,32,$hi1


L\$1st
	xmpyu		${fai}R,${fbi},${fab1}	; ap[j+1]*bp[0]
	xmpyu		${fni}R,${fm0}R,${fnm1}	; np[j+1]*m
	ldd		0($xfer),$ab1
	fstds		${fab1},0($xfer)
	 addl		$hi0,$ab1,$ab1
	 extrd,u	$ab1,31,32,$hi0
	ldd		8($xfer),$nm1
	fstds		${fnm1},8($xfer)
	 extrd,u	$ab1,63,32,$ab1
	 addl		$hi1,$nm1,$nm1
	flddx		$idx($ap),${fai}	; ap[j,j+1]
	flddx		$idx($np),${fni}	; np[j,j+1]
	 addl		$ab1,$nm1,$nm1
	 extrd,u	$nm1,31,32,$hi1

	xmpyu		${fai}L,${fbi},${fab0}	; ap[j]*bp[0]
	xmpyu		${fni}L,${fm0}R,${fnm0}	; np[j]*m
	ldd		-16($xfer),$ab0
	fstds		${fab0},-16($xfer)
	 addl		$hi0,$ab0,$ab0
	 extrd,u	$ab0,31,32,$hi0
	ldd		-8($xfer),$nm0
	fstds		${fnm0},-8($xfer)
	 extrd,u	$ab0,63,32,$ab0
	 addl		$hi1,$nm0,$nm0
	stw		$nm1,-4($tp)		; tp[j-1]
	 addl		$ab0,$nm0,$nm0
	 stw,ma		$nm0,8($tp)		; tp[j-1]
	addib,<>	8,$idx,L\$1st		; j++++
	 extrd,u	$nm0,31,32,$hi1

	xmpyu		${fai}R,${fbi},${fab1}	; ap[j]*bp[0]
	xmpyu		${fni}R,${fm0}R,${fnm1}	; np[j]*m
	ldd		0($xfer),$ab1
	fstds		${fab1},0($xfer)
	 addl		$hi0,$ab1,$ab1
	 extrd,u	$ab1,31,32,$hi0
	ldd		8($xfer),$nm1
	fstds		${fnm1},8($xfer)
	 extrd,u	$ab1,63,32,$ab1
	 addl		$hi1,$nm1,$nm1
	ldd		-16($xfer),$ab0
	 addl		$ab1,$nm1,$nm1
	ldd		-8($xfer),$nm0
	 extrd,u	$nm1,31,32,$hi1

	 addl		$hi0,$ab0,$ab0
	 extrd,u	$ab0,31,32,$hi0
	stw		$nm1,-4($tp)		; tp[j-1]
	 extrd,u	$ab0,63,32,$ab0
	 addl		$hi1,$nm0,$nm0
	ldd		0($xfer),$ab1
	 addl		$ab0,$nm0,$nm0
	ldd,mb		8($xfer),$nm1
	 extrd,u	$nm0,31,32,$hi1
	stw,ma		$nm0,8($tp)		; tp[j-1]

	ldo		-1($num),$num		; i--
	subi		0,$arrsz,$idx		; j=0
___
$code.=<<___ if ($BN_SZ==4);
	fldws,ma	4($bp),${fbi}		; bp[1]
___
$code.=<<___ if ($BN_SZ==8);
	fldws		0($bp),${fbi}		; bp[1] in flipped word order
___
$code.=<<___;
	 flddx		$idx($ap),${fai}	; ap[0,1]
	 flddx		$idx($np),${fni}	; np[0,1]
	 fldws		8($xfer),${fti}R	; tp[0]
	addl		$hi0,$ab1,$ab1
	 extrd,u	$ab1,31,32,$hi0
	 extrd,u	$ab1,63,32,$ab1
	 ldo		8($idx),$idx		; j++++
	 xmpyu		${fai}L,${fbi},${fab0}	; ap[0]*bp[1]
	 xmpyu		${fai}R,${fbi},${fab1}	; ap[1]*bp[1]
	addl		$hi1,$nm1,$nm1
	addl		$ab1,$nm1,$nm1
	extrd,u		$nm1,31,32,$hi1
	 fstws,mb	${fab0}L,-8($xfer)	; save high part
	stw		$nm1,-4($tp)		; tp[j-1]

	 fcpy,sgl	%fr0,${fti}L		; zero high part
	 fcpy,sgl	%fr0,${fab0}L
	addl		$hi1,$hi0,$hi0
	extrd,u		$hi0,31,32,$hi1
	 fcnvxf,dbl,dbl	${fti},${fti}		; 32-bit unsigned int -> double
	 fcnvxf,dbl,dbl	${fab0},${fab0}
	stw		$hi0,0($tp)
	stw		$hi1,4($tp)

	fadd,dbl	${fti},${fab0},${fab0}	; add tp[0]
	fcnvfx,dbl,dbl	${fab0},${fab0}		; double -> 33-bit unsigned int
	xmpyu		${fn0},${fab0}R,${fm0}
	ldo		`$LOCALS+32+4`($fp),$tp
L\$outer
	xmpyu		${fni}L,${fm0}R,${fnm0}	; np[0]*m
	xmpyu		${fni}R,${fm0}R,${fnm1}	; np[1]*m
	fstds		${fab0},-16($xfer)	; 33-bit value
	fstds		${fnm0},-8($xfer)
	 flddx		$idx($ap),${fai}	; ap[2]
	 flddx		$idx($np),${fni}	; np[2]
	 ldo		8($idx),$idx		; j++++
	ldd		-16($xfer),$ab0		; 33-bit value
	ldd		-8($xfer),$nm0
	ldw		0($xfer),$hi0		; high part

	xmpyu		${fai}L,${fbi},${fab0}	; ap[j]*bp[i]
	xmpyu		${fni}L,${fm0}R,${fnm0}	; np[j]*m
	 extrd,u	$ab0,31,32,$ti0		; carry bit
	 extrd,u	$ab0,63,32,$ab0
	fstds		${fab1},0($xfer)
	 addl		$ti0,$hi0,$hi0		; account carry bit
	fstds		${fnm1},8($xfer)
	 addl		$ab0,$nm0,$nm0		; low part is discarded
	ldw		0($tp),$ti1		; tp[1]
	 extrd,u	$nm0,31,32,$hi1
	fstds		${fab0},-16($xfer)
	fstds		${fnm0},-8($xfer)


L\$inner
	xmpyu		${fai}R,${fbi},${fab1}	; ap[j+1]*bp[i]
	xmpyu		${fni}R,${fm0}R,${fnm1}	; np[j+1]*m
	ldd		0($xfer),$ab1
	fstds		${fab1},0($xfer)
	 addl		$hi0,$ti1,$ti1
	 addl		$ti1,$ab1,$ab1
	ldd		8($xfer),$nm1
	fstds		${fnm1},8($xfer)
	 extrd,u	$ab1,31,32,$hi0
	 extrd,u	$ab1,63,32,$ab1
	flddx		$idx($ap),${fai}	; ap[j,j+1]
	flddx		$idx($np),${fni}	; np[j,j+1]
	 addl		$hi1,$nm1,$nm1
	 addl		$ab1,$nm1,$nm1
	ldw		4($tp),$ti0		; tp[j]
	stw		$nm1,-4($tp)		; tp[j-1]

	xmpyu		${fai}L,${fbi},${fab0}	; ap[j]*bp[i]
	xmpyu		${fni}L,${fm0}R,${fnm0}	; np[j]*m
	ldd		-16($xfer),$ab0
	fstds		${fab0},-16($xfer)
	 addl		$hi0,$ti0,$ti0
	 addl		$ti0,$ab0,$ab0
	ldd		-8($xfer),$nm0
	fstds		${fnm0},-8($xfer)
	 extrd,u	$ab0,31,32,$hi0
	 extrd,u	$nm1,31,32,$hi1
	ldw		8($tp),$ti1		; tp[j]
	 extrd,u	$ab0,63,32,$ab0
	 addl		$hi1,$nm0,$nm0
	 addl		$ab0,$nm0,$nm0
	 stw,ma		$nm0,8($tp)		; tp[j-1]
	addib,<>	8,$idx,L\$inner		; j++++
	 extrd,u	$nm0,31,32,$hi1

	xmpyu		${fai}R,${fbi},${fab1}	; ap[j]*bp[i]
	xmpyu		${fni}R,${fm0}R,${fnm1}	; np[j]*m
	ldd		0($xfer),$ab1
	fstds		${fab1},0($xfer)
	 addl		$hi0,$ti1,$ti1
	 addl		$ti1,$ab1,$ab1
	ldd		8($xfer),$nm1
	fstds		${fnm1},8($xfer)
	 extrd,u	$ab1,31,32,$hi0
	 extrd,u	$ab1,63,32,$ab1
	ldw		4($tp),$ti0		; tp[j]
	 addl		$hi1,$nm1,$nm1
	 addl		$ab1,$nm1,$nm1
	ldd		-16($xfer),$ab0
	ldd		-8($xfer),$nm0
	 extrd,u	$nm1,31,32,$hi1

	addl		$hi0,$ab0,$ab0
	 addl		$ti0,$ab0,$ab0
	 stw		$nm1,-4($tp)		; tp[j-1]
	 extrd,u	$ab0,31,32,$hi0
	ldw		8($tp),$ti1		; tp[j]
	 extrd,u	$ab0,63,32,$ab0
	 addl		$hi1,$nm0,$nm0
	ldd		0($xfer),$ab1
	 addl		$ab0,$nm0,$nm0
	ldd,mb		8($xfer),$nm1
	 extrd,u	$nm0,31,32,$hi1
	 stw,ma		$nm0,8($tp)		; tp[j-1]

	addib,=		-1,$num,L\$outerdone	; i--
	subi		0,$arrsz,$idx		; j=0
___
$code.=<<___ if ($BN_SZ==4);
	fldws,ma	4($bp),${fbi}		; bp[i]
___
$code.=<<___ if ($BN_SZ==8);
	ldi		12,$ti0			; bp[i] in flipped word order
	addl,ev		%r0,$num,$num
	ldi		-4,$ti0
	addl		$ti0,$bp,$bp
	fldws		0($bp),${fbi}
___
$code.=<<___;
	 flddx		$idx($ap),${fai}	; ap[0]
	addl		$hi0,$ab1,$ab1
	 flddx		$idx($np),${fni}	; np[0]
	 fldws		8($xfer),${fti}R	; tp[0]
	addl		$ti1,$ab1,$ab1
	extrd,u		$ab1,31,32,$hi0
	extrd,u		$ab1,63,32,$ab1

	 ldo		8($idx),$idx		; j++++
	 xmpyu		${fai}L,${fbi},${fab0}	; ap[0]*bp[i]
	 xmpyu		${fai}R,${fbi},${fab1}	; ap[1]*bp[i]
	ldw		4($tp),$ti0		; tp[j]

	addl		$hi1,$nm1,$nm1
	 fstws,mb	${fab0}L,-8($xfer)	; save high part
	addl		$ab1,$nm1,$nm1
	extrd,u		$nm1,31,32,$hi1
	 fcpy,sgl	%fr0,${fti}L		; zero high part
	 fcpy,sgl	%fr0,${fab0}L
	stw		$nm1,-4($tp)		; tp[j-1]

	 fcnvxf,dbl,dbl	${fti},${fti}		; 32-bit unsigned int -> double
	 fcnvxf,dbl,dbl	${fab0},${fab0}
	addl		$hi1,$hi0,$hi0
	 fadd,dbl	${fti},${fab0},${fab0}	; add tp[0]
	addl		$ti0,$hi0,$hi0
	extrd,u		$hi0,31,32,$hi1
	 fcnvfx,dbl,dbl	${fab0},${fab0}		; double -> 33-bit unsigned int
	stw		$hi0,0($tp)
	stw		$hi1,4($tp)
	 xmpyu		${fn0},${fab0}R,${fm0}

	b		L\$outer
	ldo		`$LOCALS+32+4`($fp),$tp


L\$outerdone
	addl		$hi0,$ab1,$ab1
	addl		$ti1,$ab1,$ab1
	extrd,u		$ab1,31,32,$hi0
	extrd,u		$ab1,63,32,$ab1

	ldw		4($tp),$ti0		; tp[j]

	addl		$hi1,$nm1,$nm1
	addl		$ab1,$nm1,$nm1
	extrd,u		$nm1,31,32,$hi1
	stw		$nm1,-4($tp)		; tp[j-1]

	addl		$hi1,$hi0,$hi0
	addl		$ti0,$hi0,$hi0
	extrd,u		$hi0,31,32,$hi1
	stw		$hi0,0($tp)
	stw		$hi1,4($tp)

	ldo		`$LOCALS+32`($fp),$tp
	sub		%r0,%r0,%r0		; clear borrow
___
$code.=<<___ if ($BN_SZ==4);
	ldws,ma		4($tp),$ti0
	extru,=		$rp,31,3,%r0		; is rp 64-bit aligned?
	b		L\$sub_pa11
	addl		$tp,$arrsz,$tp
L\$sub
	ldwx		$idx($np),$hi0
	subb		$ti0,$hi0,$hi1
	ldwx		$idx($tp),$ti0
	addib,<>	4,$idx,L\$sub
	stws,ma		$hi1,4($rp)

	subb		$ti0,%r0,$hi1
	ldo		-4($tp),$tp
___
$code.=<<___ if ($BN_SZ==8);
	ldd,ma		8($tp),$ti0
L\$sub
	ldd		$idx($np),$hi0
	shrpd		$ti0,$ti0,32,$ti0	; flip word order
	std		$ti0,-8($tp)		; save flipped value
	sub,db		$ti0,$hi0,$hi1
	ldd,ma		8($tp),$ti0
	addib,<>	8,$idx,L\$sub
	std,ma		$hi1,8($rp)

	extrd,u		$ti0,31,32,$ti0		; carry in flipped word order
	sub,db		$ti0,%r0,$hi1
	ldo		-8($tp),$tp
___
$code.=<<___;
	and		$tp,$hi1,$ap
	andcm		$rp,$hi1,$bp
	or		$ap,$bp,$np

	sub		$rp,$arrsz,$rp		; rewind rp
	subi		0,$arrsz,$idx
	ldo		`$LOCALS+32`($fp),$tp
L\$copy
	ldd		$idx($np),$hi0
	std,ma		%r0,8($tp)
	addib,<>	8,$idx,.-8		; L\$copy
	std,ma		$hi0,8($rp)	
___

if ($BN_SZ==4) {				# PA-RISC 1.1 code-path
$ablo=$ab0;
$abhi=$ab1;
$nmlo0=$nm0;
$nmhi0=$nm1;
$nmlo1="%r9";
$nmhi1="%r8";

$code.=<<___;
	b		L\$done
	nop

	.ALIGN		8
L\$parisc11
	xmpyu		${fai}L,${fbi},${fab0}	; ap[j]*bp[0]
	xmpyu		${fni}L,${fm0}R,${fnm0}	; np[j]*m
	ldw		-12($xfer),$ablo
	ldw		-16($xfer),$hi0
	ldw		-4($xfer),$nmlo0
	ldw		-8($xfer),$nmhi0
	fstds		${fab0},-16($xfer)
	fstds		${fnm0},-8($xfer)

	 ldo		8($idx),$idx		; j++++
	 add		$ablo,$nmlo0,$nmlo0	; discarded
	 addc		%r0,$nmhi0,$hi1
	ldw		4($xfer),$ablo
	ldw		0($xfer),$abhi
	nop


L\$1st_pa11
	xmpyu		${fai}R,${fbi},${fab1}	; ap[j+1]*bp[0]
	flddx		$idx($ap),${fai}	; ap[j,j+1]
	xmpyu		${fni}R,${fm0}R,${fnm1}	; np[j+1]*m
	flddx		$idx($np),${fni}	; np[j,j+1]
	 add		$hi0,$ablo,$ablo
	ldw		12($xfer),$nmlo1
	 addc		%r0,$abhi,$hi0
	ldw		8($xfer),$nmhi1
	 add		$ablo,$nmlo1,$nmlo1
	fstds		${fab1},0($xfer)
	 addc		%r0,$nmhi1,$nmhi1
	fstds		${fnm1},8($xfer)
	 add		$hi1,$nmlo1,$nmlo1
	ldw		-12($xfer),$ablo
	 addc		%r0,$nmhi1,$hi1
	ldw		-16($xfer),$abhi

	xmpyu		${fai}L,${fbi},${fab0}	; ap[j]*bp[0]
	ldw		-4($xfer),$nmlo0
	xmpyu		${fni}L,${fm0}R,${fnm0}	; np[j]*m
	ldw		-8($xfer),$nmhi0
	 add		$hi0,$ablo,$ablo
	stw		$nmlo1,-4($tp)		; tp[j-1]
	 addc		%r0,$abhi,$hi0
	fstds		${fab0},-16($xfer)
	 add		$ablo,$nmlo0,$nmlo0
	fstds		${fnm0},-8($xfer)
	 addc		%r0,$nmhi0,$nmhi0
	ldw		0($xfer),$abhi
	 add		$hi1,$nmlo0,$nmlo0
	ldw		4($xfer),$ablo
	 stws,ma	$nmlo0,8($tp)		; tp[j-1]
	addib,<>	8,$idx,L\$1st_pa11	; j++++
	 addc		%r0,$nmhi0,$hi1

	 ldw		8($xfer),$nmhi1
	 ldw		12($xfer),$nmlo1
	xmpyu		${fai}R,${fbi},${fab1}	; ap[j]*bp[0]
	xmpyu		${fni}R,${fm0}R,${fnm1}	; np[j]*m
	 add		$hi0,$ablo,$ablo
	fstds		${fab1},0($xfer)
	 addc		%r0,$abhi,$hi0
	fstds		${fnm1},8($xfer)
	 add		$ablo,$nmlo1,$nmlo1
	ldw		-16($xfer),$abhi
	 addc		%r0,$nmhi1,$nmhi1
	ldw		-12($xfer),$ablo
	 add		$hi1,$nmlo1,$nmlo1
	ldw		-8($xfer),$nmhi0
	 addc		%r0,$nmhi1,$hi1
	ldw		-4($xfer),$nmlo0

	 add		$hi0,$ablo,$ablo
	stw		$nmlo1,-4($tp)		; tp[j-1]
	 addc		%r0,$abhi,$hi0
	ldw		0($xfer),$abhi
	 add		$ablo,$nmlo0,$nmlo0
	ldw		4($xfer),$ablo
	 addc		%r0,$nmhi0,$nmhi0
	ldws,mb		8($xfer),$nmhi1
	 add		$hi1,$nmlo0,$nmlo0
	ldw		4($xfer),$nmlo1
	 addc		%r0,$nmhi0,$hi1
	stws,ma		$nmlo0,8($tp)		; tp[j-1]

	ldo		-1($num),$num		; i--
	subi		0,$arrsz,$idx		; j=0

	 fldws,ma	4($bp),${fbi}		; bp[1]
	 flddx		$idx($ap),${fai}	; ap[0,1]
	 flddx		$idx($np),${fni}	; np[0,1]
	 fldws		8($xfer),${fti}R	; tp[0]
	add		$hi0,$ablo,$ablo
	addc		%r0,$abhi,$hi0
	 ldo		8($idx),$idx		; j++++
	 xmpyu		${fai}L,${fbi},${fab0}	; ap[0]*bp[1]
	 xmpyu		${fai}R,${fbi},${fab1}	; ap[1]*bp[1]
	add		$hi1,$nmlo1,$nmlo1
	addc		%r0,$nmhi1,$nmhi1
	add		$ablo,$nmlo1,$nmlo1
	addc		%r0,$nmhi1,$hi1
	 fstws,mb	${fab0}L,-8($xfer)	; save high part
	stw		$nmlo1,-4($tp)		; tp[j-1]

	 fcpy,sgl	%fr0,${fti}L		; zero high part
	 fcpy,sgl	%fr0,${fab0}L
	add		$hi1,$hi0,$hi0
	addc		%r0,%r0,$hi1
	 fcnvxf,dbl,dbl	${fti},${fti}		; 32-bit unsigned int -> double
	 fcnvxf,dbl,dbl	${fab0},${fab0}
	stw		$hi0,0($tp)
	stw		$hi1,4($tp)

	fadd,dbl	${fti},${fab0},${fab0}	; add tp[0]
	fcnvfx,dbl,dbl	${fab0},${fab0}		; double -> 33-bit unsigned int
	xmpyu		${fn0},${fab0}R,${fm0}
	ldo		`$LOCALS+32+4`($fp),$tp
L\$outer_pa11
	xmpyu		${fni}L,${fm0}R,${fnm0}	; np[0]*m
	xmpyu		${fni}R,${fm0}R,${fnm1}	; np[1]*m
	fstds		${fab0},-16($xfer)	; 33-bit value
	fstds		${fnm0},-8($xfer)
	 flddx		$idx($ap),${fai}	; ap[2,3]
	 flddx		$idx($np),${fni}	; np[2,3]
	ldw		-16($xfer),$abhi	; carry bit actually
	 ldo		8($idx),$idx		; j++++
	ldw		-12($xfer),$ablo
	ldw		-8($xfer),$nmhi0
	ldw		-4($xfer),$nmlo0
	ldw		0($xfer),$hi0		; high part

	xmpyu		${fai}L,${fbi},${fab0}	; ap[j]*bp[i]
	xmpyu		${fni}L,${fm0}R,${fnm0}	; np[j]*m
	fstds		${fab1},0($xfer)
	 addl		$abhi,$hi0,$hi0		; account carry bit
	fstds		${fnm1},8($xfer)
	 add		$ablo,$nmlo0,$nmlo0	; discarded
	ldw		0($tp),$ti1		; tp[1]
	 addc		%r0,$nmhi0,$hi1
	fstds		${fab0},-16($xfer)
	fstds		${fnm0},-8($xfer)
	ldw		4($xfer),$ablo
	ldw		0($xfer),$abhi


L\$inner_pa11
	xmpyu		${fai}R,${fbi},${fab1}	; ap[j+1]*bp[i]
	flddx		$idx($ap),${fai}	; ap[j,j+1]
	xmpyu		${fni}R,${fm0}R,${fnm1}	; np[j+1]*m
	flddx		$idx($np),${fni}	; np[j,j+1]
	 add		$hi0,$ablo,$ablo
	ldw		4($tp),$ti0		; tp[j]
	 addc		%r0,$abhi,$abhi
	ldw		12($xfer),$nmlo1
	 add		$ti1,$ablo,$ablo
	ldw		8($xfer),$nmhi1
	 addc		%r0,$abhi,$hi0
	fstds		${fab1},0($xfer)
	 add		$ablo,$nmlo1,$nmlo1
	fstds		${fnm1},8($xfer)
	 addc		%r0,$nmhi1,$nmhi1
	ldw		-12($xfer),$ablo
	 add		$hi1,$nmlo1,$nmlo1
	ldw		-16($xfer),$abhi
	 addc		%r0,$nmhi1,$hi1

	xmpyu		${fai}L,${fbi},${fab0}	; ap[j]*bp[i]
	ldw		8($tp),$ti1		; tp[j]
	xmpyu		${fni}L,${fm0}R,${fnm0}	; np[j]*m
	ldw		-4($xfer),$nmlo0
	 add		$hi0,$ablo,$ablo
	ldw		-8($xfer),$nmhi0
	 addc		%r0,$abhi,$abhi
	stw		$nmlo1,-4($tp)		; tp[j-1]
	 add		$ti0,$ablo,$ablo
	fstds		${fab0},-16($xfer)
	 addc		%r0,$abhi,$hi0
	fstds		${fnm0},-8($xfer)
	 add		$ablo,$nmlo0,$nmlo0
	ldw		4($xfer),$ablo
	 addc		%r0,$nmhi0,$nmhi0
	ldw		0($xfer),$abhi
	 add		$hi1,$nmlo0,$nmlo0
	 stws,ma	$nmlo0,8($tp)		; tp[j-1]
	addib,<>	8,$idx,L\$inner_pa11	; j++++
	 addc		%r0,$nmhi0,$hi1

	xmpyu		${fai}R,${fbi},${fab1}	; ap[j]*bp[i]
	ldw		12($xfer),$nmlo1
	xmpyu		${fni}R,${fm0}R,${fnm1}	; np[j]*m
	ldw		8($xfer),$nmhi1
	 add		$hi0,$ablo,$ablo
	ldw		4($tp),$ti0		; tp[j]
	 addc		%r0,$abhi,$abhi
	fstds		${fab1},0($xfer)
	 add		$ti1,$ablo,$ablo
	fstds		${fnm1},8($xfer)
	 addc		%r0,$abhi,$hi0
	ldw		-16($xfer),$abhi
	 add		$ablo,$nmlo1,$nmlo1
	ldw		-12($xfer),$ablo
	 addc		%r0,$nmhi1,$nmhi1
	ldw		-8($xfer),$nmhi0
	 add		$hi1,$nmlo1,$nmlo1
	ldw		-4($xfer),$nmlo0
	 addc		%r0,$nmhi1,$hi1

	add		$hi0,$ablo,$ablo
	 stw		$nmlo1,-4($tp)		; tp[j-1]
	addc		%r0,$abhi,$abhi
	 add		$ti0,$ablo,$ablo
	ldw		8($tp),$ti1		; tp[j]
	 addc		%r0,$abhi,$hi0
	ldw		0($xfer),$abhi
	 add		$ablo,$nmlo0,$nmlo0
	ldw		4($xfer),$ablo
	 addc		%r0,$nmhi0,$nmhi0
	ldws,mb		8($xfer),$nmhi1
	 add		$hi1,$nmlo0,$nmlo0
	ldw		4($xfer),$nmlo1
	 addc		%r0,$nmhi0,$hi1
	 stws,ma	$nmlo0,8($tp)		; tp[j-1]

	addib,=		-1,$num,L\$outerdone_pa11; i--
	subi		0,$arrsz,$idx		; j=0

	 fldws,ma	4($bp),${fbi}		; bp[i]
	 flddx		$idx($ap),${fai}	; ap[0]
	add		$hi0,$ablo,$ablo
	addc		%r0,$abhi,$abhi
	 flddx		$idx($np),${fni}	; np[0]
	 fldws		8($xfer),${fti}R	; tp[0]
	add		$ti1,$ablo,$ablo
	addc		%r0,$abhi,$hi0

	 ldo		8($idx),$idx		; j++++
	 xmpyu		${fai}L,${fbi},${fab0}	; ap[0]*bp[i]
	 xmpyu		${fai}R,${fbi},${fab1}	; ap[1]*bp[i]
	ldw		4($tp),$ti0		; tp[j]

	add		$hi1,$nmlo1,$nmlo1
	addc		%r0,$nmhi1,$nmhi1
	 fstws,mb	${fab0}L,-8($xfer)	; save high part
	add		$ablo,$nmlo1,$nmlo1
	addc		%r0,$nmhi1,$hi1
	 fcpy,sgl	%fr0,${fti}L		; zero high part
	 fcpy,sgl	%fr0,${fab0}L
	stw		$nmlo1,-4($tp)		; tp[j-1]

	 fcnvxf,dbl,dbl	${fti},${fti}		; 32-bit unsigned int -> double
	 fcnvxf,dbl,dbl	${fab0},${fab0}
	add		$hi1,$hi0,$hi0
	addc		%r0,%r0,$hi1
	 fadd,dbl	${fti},${fab0},${fab0}	; add tp[0]
	add		$ti0,$hi0,$hi0
	addc		%r0,$hi1,$hi1
	 fcnvfx,dbl,dbl	${fab0},${fab0}		; double -> 33-bit unsigned int
	stw		$hi0,0($tp)
	stw		$hi1,4($tp)
	 xmpyu		${fn0},${fab0}R,${fm0}

	b		L\$outer_pa11
	ldo		`$LOCALS+32+4`($fp),$tp


L\$outerdone_pa11
	add		$hi0,$ablo,$ablo
	addc		%r0,$abhi,$abhi
	add		$ti1,$ablo,$ablo
	addc		%r0,$abhi,$hi0

	ldw		4($tp),$ti0		; tp[j]

	add		$hi1,$nmlo1,$nmlo1
	addc		%r0,$nmhi1,$nmhi1
	add		$ablo,$nmlo1,$nmlo1
	addc		%r0,$nmhi1,$hi1
	stw		$nmlo1,-4($tp)		; tp[j-1]

	add		$hi1,$hi0,$hi0
	addc		%r0,%r0,$hi1
	add		$ti0,$hi0,$hi0
	addc		%r0,$hi1,$hi1
	stw		$hi0,0($tp)
	stw		$hi1,4($tp)

	ldo		`$LOCALS+32+4`($fp),$tp
	sub		%r0,%r0,%r0		; clear borrow
	ldw		-4($tp),$ti0
	addl		$tp,$arrsz,$tp
L\$sub_pa11
	ldwx		$idx($np),$hi0
	subb		$ti0,$hi0,$hi1
	ldwx		$idx($tp),$ti0
	addib,<>	4,$idx,L\$sub_pa11
	stws,ma		$hi1,4($rp)

	subb		$ti0,%r0,$hi1
	ldo		-4($tp),$tp
	and		$tp,$hi1,$ap
	andcm		$rp,$hi1,$bp
	or		$ap,$bp,$np

	sub		$rp,$arrsz,$rp		; rewind rp
	subi		0,$arrsz,$idx
	ldo		`$LOCALS+32`($fp),$tp
L\$copy_pa11
	ldwx		$idx($np),$hi0
	stws,ma		%r0,4($tp)
	addib,<>	4,$idx,L\$copy_pa11
	stws,ma		$hi0,4($rp)	

	nop					; alignment
L\$done
___
}


$code.=<<___;
	ldi		1,%r28			; signal "handled"
	ldo		$FRAME($fp),%sp		; destroy tp[num+1]

	$POP	`-$FRAME-$SAVED_RP`(%sp),%r2	; standard epilogue
	$POP	`-$FRAME+1*$SIZE_T`(%sp),%r4
	$POP	`-$FRAME+2*$SIZE_T`(%sp),%r5
	$POP	`-$FRAME+3*$SIZE_T`(%sp),%r6
	$POP	`-$FRAME+4*$SIZE_T`(%sp),%r7
	$POP	`-$FRAME+5*$SIZE_T`(%sp),%r8
	$POP	`-$FRAME+6*$SIZE_T`(%sp),%r9
	$POP	`-$FRAME+7*$SIZE_T`(%sp),%r10
L\$abort
	bv	(%r2)
	.EXIT
	$POPMB	-$FRAME(%sp),%r3
	.PROCEND
	.STRINGZ "Montgomery Multiplication for PA-RISC, CRYPTOGAMS by <appro\@openssl.org>"
___


# Explicitly encode PA-RISC 2.0 instructions used in this module, so
# that it can be compiled with .LEVEL 1.0. It should be noted that I
# wouldn't have to do this, if GNU assembler understood .ALLOW 2.0
# directive...

my $ldd = sub {
  my ($mod,$args) = @_;
  my $orig = "ldd$mod\t$args";

    if ($args =~ /%r([0-9]+)\(%r([0-9]+)\),%r([0-9]+)/)		# format 4
    {	my $opcode=(0x03<<26)|($2<<21)|($1<<16)|(3<<6)|$3;
	sprintf "\t.WORD\t0x%08x\t; %s",$opcode,$orig;
    }
    elsif ($args =~ /(\-?[0-9]+)\(%r([0-9]+)\),%r([0-9]+)/)	# format 5
    {	my $opcode=(0x03<<26)|($2<<21)|(1<<12)|(3<<6)|$3;
	$opcode|=(($1&0xF)<<17)|(($1&0x10)<<12);		# encode offset
	$opcode|=(1<<5)  if ($mod =~ /^,m/);
	$opcode|=(1<<13) if ($mod =~ /^,mb/);
	sprintf "\t.WORD\t0x%08x\t; %s",$opcode,$orig;
    }
    else { "\t".$orig; }
};

my $std = sub {
  my ($mod,$args) = @_;
  my $orig = "std$mod\t$args";

    if ($args =~ /%r([0-9]+),(\-?[0-9]+)\(%r([0-9]+)\)/)	# format 6
    {	my $opcode=(0x03<<26)|($3<<21)|($1<<16)|(1<<12)|(0xB<<6);
	$opcode|=(($2&0xF)<<1)|(($2&0x10)>>4);			# encode offset
	$opcode|=(1<<5)  if ($mod =~ /^,m/);
	$opcode|=(1<<13) if ($mod =~ /^,mb/);
	sprintf "\t.WORD\t0x%08x\t; %s",$opcode,$orig;
    }
    else { "\t".$orig; }
};

my $extrd = sub {
  my ($mod,$args) = @_;
  my $orig = "extrd$mod\t$args";

    # I only have ",u" completer, it's implicitly encoded...
    if ($args =~ /%r([0-9]+),([0-9]+),([0-9]+),%r([0-9]+)/)	# format 15
    {	my $opcode=(0x36<<26)|($1<<21)|($4<<16);
	my $len=32-$3;
	$opcode |= (($2&0x20)<<6)|(($2&0x1f)<<5);		# encode pos
	$opcode |= (($len&0x20)<<7)|($len&0x1f);		# encode len
	sprintf "\t.WORD\t0x%08x\t; %s",$opcode,$orig;
    }
    elsif ($args =~ /%r([0-9]+),%sar,([0-9]+),%r([0-9]+)/)	# format 12
    {	my $opcode=(0x34<<26)|($1<<21)|($3<<16)|(2<<11)|(1<<9);
	my $len=32-$2;
	$opcode |= (($len&0x20)<<3)|($len&0x1f);		# encode len
	$opcode |= (1<<13) if ($mod =~ /,\**=/);
	sprintf "\t.WORD\t0x%08x\t; %s",$opcode,$orig;
    }
    else { "\t".$orig; }
};

my $shrpd = sub {
  my ($mod,$args) = @_;
  my $orig = "shrpd$mod\t$args";

    if ($args =~ /%r([0-9]+),%r([0-9]+),([0-9]+),%r([0-9]+)/)	# format 14
    {	my $opcode=(0x34<<26)|($2<<21)|($1<<16)|(1<<10)|$4;
	my $cpos=63-$3;
	$opcode |= (($cpos&0x20)<<6)|(($cpos&0x1f)<<5);		# encode sa
	sprintf "\t.WORD\t0x%08x\t; %s",$opcode,$orig;
    }
    else { "\t".$orig; }
};

my $sub = sub {
  my ($mod,$args) = @_;
  my $orig = "sub$mod\t$args";

    if ($mod eq ",db" && $args =~ /%r([0-9]+),%r([0-9]+),%r([0-9]+)/) {
	my $opcode=(0x02<<26)|($2<<21)|($1<<16)|$3;
	$opcode|=(1<<10);	# e1
	$opcode|=(1<<8);	# e2
	$opcode|=(1<<5);	# d
	sprintf "\t.WORD\t0x%08x\t; %s",$opcode,$orig
    }
    else { "\t".$orig; }
};

sub assemble {
  my ($mnemonic,$mod,$args)=@_;
  my $opcode = eval("\$$mnemonic");

    ref($opcode) eq 'CODE' ? &$opcode($mod,$args) : "\t$mnemonic$mod\t$args";
}

foreach (split("\n",$code)) {
	s/\`([^\`]*)\`/eval $1/ge;
	# flip word order in 64-bit mode...
	s/(xmpyu\s+)($fai|$fni)([LR])/$1.$2.($3 eq "L"?"R":"L")/e if ($BN_SZ==8);
	# assemble 2.0 instructions in 32-bit mode...
	s/^\s+([a-z]+)([\S]*)\s+([\S]*)/&assemble($1,$2,$3)/e if ($BN_SZ==4);

	s/\bbv\b/bve/gm	if ($SIZE_T==8);

	print $_,"\n";
}
close STDOUT;
