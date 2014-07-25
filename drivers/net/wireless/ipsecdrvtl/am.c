/*
   'ripemd.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Mon Oct 29 11:54:27 2012
*/
#include"cobf.h"
#ifdef _WIN32
#if defined( UNDER_CE) && defined( bb353) || ! defined( bb344)
#define bb335 1
#define bb337 1
#else
#define bb329 bb343
#define bb346 1
#define bb355 1
#endif
#define bb332 1
#include"uncobf.h"
#include<ndis.h>
#include"cobf.h"
#ifdef UNDER_CE
#include"uncobf.h"
#include<ndiswan.h>
#include"cobf.h"
#endif
#include"uncobf.h"
#include<stdio.h>
#include<basetsd.h>
#include"cobf.h"
bba bbs bbl bbf, *bb1;bba bbs bbe bbq, *bb94;bba bb135 bb124, *bb356;
bba bbs bbl bb39, *bb72;bba bbs bb135 bbk, *bb59;bba bbe bbu, *bb134;
bba bbh bbf*bb89;
#ifdef bb308
bba bbd bb60, *bb122;
#endif
#else
#include"uncobf.h"
#include<linux/module.h>
#include<linux/ctype.h>
#include<linux/time.h>
#include<linux/slab.h>
#include"cobf.h"
#ifndef bb116
#define bb116
#ifdef _WIN32
#include"uncobf.h"
#include<wtypes.h>
#include"cobf.h"
#else
#ifdef bb118
#include"uncobf.h"
#include<linux/types.h>
#include"cobf.h"
#else
#include"uncobf.h"
#include<stddef.h>
#include<sys/types.h>
#include"cobf.h"
#endif
#endif
#ifdef _WIN32
bba bb114 bb239;
#else
bba bbe bbu, *bb134, *bb205;
#define bb201 1
#define bb202 0
bba bb260 bb209, *bb247, *bb262;bba bbe bb221, *bb267, *bb282;bba bbs
bbq, *bb94, *bb207;bba bb6 bb283, *bb279;bba bbs bb6 bb245, *bb286;
bba bb6 bb119, *bb233;bba bbs bb6 bb64, *bb217;bba bb64 bb265, *bb258
;bba bb64 bb238, *bb252;bba bb119 bb114, *bb211;bba bb242 bb257;bba
bb280 bb124;bba bb229 bb83;bba bb112 bb120;bba bb112 bb249;
#ifdef bb276
bba bb269 bb39, *bb72;bba bb225 bbk, *bb59;bba bb236 bbd, *bb29;bba
bb281 bb56, *bb111;
#else
bba bb273 bb39, *bb72;bba bb227 bbk, *bb59;bba bb224 bbd, *bb29;bba
bb244 bb56, *bb111;
#endif
bba bb39 bbf, *bb1, *bb214;bba bbk bb292, *bb291, *bb285;bba bbk bb223
, *bb246, *bb284;bba bbd bb60, *bb122, *bb220;bba bb83 bb37, *bb259, *
bb234;bba bbd bb231, *bb288, *bb208;bba bb120 bb219, *bb263, *bb277;
bba bb56 bb222, *bb240, *bb256;
#define bb140 bbb
bba bbb*bb230, *bb77;bba bbh bbb*bb274;bba bbl bb213;bba bbl*bb218;
bba bbh bbl*bb82;
#if defined( bb118)
bba bbe bb113;
#endif
bba bb113 bb20;bba bb20*bb237;bba bbh bb20*bb191;
#if defined( bb275) || defined( bb266)
bba bb20 bb36;bba bb20 bb117;
#else
bba bbl bb36;bba bbs bbl bb117;
#endif
bba bbh bb36*bb253;bba bb36*bb255;bba bb60 bb212, *bb264;bba bbb*
bb107;bba bb107*bb250;
#define bb290( bb35) bbi bb35##__ { bbe bb268; }; bba bbi bb35##__  * \
 bb35
bba bbi{bb37 bb189,bb261,bb270,bb228;}bb272, *bb289, *bb243;bba bbi{
bb37 bb8,bb193;}bb232, *bb278, *bb287;bba bbi{bb37 bb235,bb215;}bb251
, *bb216, *bb271;
#endif
bba bbh bbf*bb89;
#endif
bba bbf bb103;
#define IN
#define OUT
#ifdef _DEBUG
#define bb145( bbc) bb32( bbc)
#else
#define bb145( bbc) ( bbb)( bbc)
#endif
bba bbe bb160, *bb173;
#define bb241 0
#define bb318 1
#define bb298 2
#define bb324 3
#define bb331 4
bba bbe bb357;bba bbb*bb121;
#endif
#ifdef _WIN32
#ifndef UNDER_CE
#define bb31 bb334
#define bb43 bb336
bba bbs bb6 bb31;bba bb6 bb43;
#endif
#else
#endif
#ifdef _WIN32
bbb*bb128(bb31 bb47);bbb bb108(bbb* );bbb*bb137(bb31 bb159,bb31 bb47);
#else
#define bb128( bbc) bb147(1, bbc, bb141)
#define bb108( bbc) bb342( bbc)
#define bb137( bbc, bbn) bb147( bbc, bbn, bb141)
#endif
#ifdef _WIN32
#define bb32( bbc) bb340( bbc)
#else
#ifdef _DEBUG
bbe bb144(bbh bbl*bb97,bbh bbl*bb25,bbs bb206);
#define bb32( bbc) ( bbb)(( bbc) || ( bb144(# bbc, __FILE__, __LINE__ \
)))
#else
#define bb32( bbc) (( bbb)0)
#endif
#endif
bb43 bb301(bb43*bb323);
#ifndef _WIN32
bbe bb327(bbh bbl*bbg);bbe bb321(bbh bbl*bb19,...);
#endif
#ifdef _WIN32
bba bb347 bb96;
#define bb139( bbc) bb352( bbc)
#define bb142( bbc) bb345( bbc)
#define bb133( bbc) bb339( bbc)
#define bb132( bbc) bb330( bbc)
#else
bba bb349 bb96;
#define bb139( bbc) ( bbb)(  *  bbc = bb358( bbc))
#define bb142( bbc) (( bbb)0)
#define bb133( bbc) bb348( bbc)
#define bb132( bbc) bb354( bbc)
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbd bb10;bbd bb23[5 ];bbf bb101[64 ];}bb523;bbb bb1802(bb523*
bbj);bbb bb1358(bb523*bbj,bbh bbb*bb496,bbq bb10);bbb bb1795(bb523*
bbj,bbb*bb14);bbb bb1923(bbb*bb14,bbh bbb*bb5,bbq bb10);bbb bb1973(
bbb*bb14,bb82 bb5);
#ifdef __cplusplus
}
#endif
bb41 bbb bb1255(bbd bb23[5 ],bbh bbf bb97[64 ]){bb32(bb12(bbe)>=4 );{bbd
bb67,bb61,bb66,bb55,bb63,bb69,bb62,bb48,bb65,bb68;bbd bbv[16 ],bbz;
bb67=bb61=bb23[0 ];bb66=bb55=bb23[1 ];bb63=bb69=bb23[2 ];bb62=bb48=bb23[
3 ];bb65=bb68=bb23[4 ];bb90(bbz=0 ;bbz<16 ;bbz++,bb97+=4 )bbv[bbz]=(bb97[0
]|bb97[1 ]<<8 |bb97[2 ]<<16 |bb97[3 ]<<24 );bb61+=(bb55^bb69^bb48)+bbv[0 ];
bb61=((bb61)<<(11 )|(bb61)>>(32 -11 ))+bb68;bb69=((bb69)<<(10 )|(bb69)>>(
32 -10 ));bb68+=(bb61^bb55^bb69)+bbv[1 ];bb68=((bb68)<<(14 )|(bb68)>>(32 -
14 ))+bb48;bb55=((bb55)<<(10 )|(bb55)>>(32 -10 ));bb48+=(bb68^bb61^bb55)+
bbv[2 ];bb48=((bb48)<<(15 )|(bb48)>>(32 -15 ))+bb69;bb61=((bb61)<<(10 )|(
bb61)>>(32 -10 ));bb69+=(bb48^bb68^bb61)+bbv[3 ];bb69=((bb69)<<(12 )|(
bb69)>>(32 -12 ))+bb55;bb68=((bb68)<<(10 )|(bb68)>>(32 -10 ));bb55+=(bb69^
bb48^bb68)+bbv[4 ];bb55=((bb55)<<(5 )|(bb55)>>(32 -5 ))+bb61;bb48=((bb48)<<
(10 )|(bb48)>>(32 -10 ));bb61+=(bb55^bb69^bb48)+bbv[5 ];bb61=((bb61)<<(8 )|
(bb61)>>(32 -8 ))+bb68;bb69=((bb69)<<(10 )|(bb69)>>(32 -10 ));bb68+=(bb61^
bb55^bb69)+bbv[6 ];bb68=((bb68)<<(7 )|(bb68)>>(32 -7 ))+bb48;bb55=((bb55)<<
(10 )|(bb55)>>(32 -10 ));bb48+=(bb68^bb61^bb55)+bbv[7 ];bb48=((bb48)<<(9 )|
(bb48)>>(32 -9 ))+bb69;bb61=((bb61)<<(10 )|(bb61)>>(32 -10 ));bb69+=(bb48^
bb68^bb61)+bbv[8 ];bb69=((bb69)<<(11 )|(bb69)>>(32 -11 ))+bb55;bb68=((
bb68)<<(10 )|(bb68)>>(32 -10 ));bb55+=(bb69^bb48^bb68)+bbv[9 ];bb55=((
bb55)<<(13 )|(bb55)>>(32 -13 ))+bb61;bb48=((bb48)<<(10 )|(bb48)>>(32 -10 ));
bb61+=(bb55^bb69^bb48)+bbv[10 ];bb61=((bb61)<<(14 )|(bb61)>>(32 -14 ))+
bb68;bb69=((bb69)<<(10 )|(bb69)>>(32 -10 ));bb68+=(bb61^bb55^bb69)+bbv[
11 ];bb68=((bb68)<<(15 )|(bb68)>>(32 -15 ))+bb48;bb55=((bb55)<<(10 )|(bb55
)>>(32 -10 ));bb48+=(bb68^bb61^bb55)+bbv[12 ];bb48=((bb48)<<(6 )|(bb48)>>
(32 -6 ))+bb69;bb61=((bb61)<<(10 )|(bb61)>>(32 -10 ));bb69+=(bb48^bb68^
bb61)+bbv[13 ];bb69=((bb69)<<(7 )|(bb69)>>(32 -7 ))+bb55;bb68=((bb68)<<(
10 )|(bb68)>>(32 -10 ));bb55+=(bb69^bb48^bb68)+bbv[14 ];bb55=((bb55)<<(9 )|
(bb55)>>(32 -9 ))+bb61;bb48=((bb48)<<(10 )|(bb48)>>(32 -10 ));bb61+=(bb55^
bb69^bb48)+bbv[15 ];bb61=((bb61)<<(8 )|(bb61)>>(32 -8 ))+bb68;bb69=((bb69
)<<(10 )|(bb69)>>(32 -10 ));bb68+=(bb61&bb55|~bb61&bb69)+0x5a827999 +bbv[
7 ];bb68=((bb68)<<(7 )|(bb68)>>(32 -7 ))+bb48;bb55=((bb55)<<(10 )|(bb55)>>
(32 -10 ));bb48+=(bb68&bb61|~bb68&bb55)+0x5a827999 +bbv[4 ];bb48=((bb48)<<
(6 )|(bb48)>>(32 -6 ))+bb69;bb61=((bb61)<<(10 )|(bb61)>>(32 -10 ));bb69+=(
bb48&bb68|~bb48&bb61)+0x5a827999 +bbv[13 ];bb69=((bb69)<<(8 )|(bb69)>>(
32 -8 ))+bb55;bb68=((bb68)<<(10 )|(bb68)>>(32 -10 ));bb55+=(bb69&bb48|~
bb69&bb68)+0x5a827999 +bbv[1 ];bb55=((bb55)<<(13 )|(bb55)>>(32 -13 ))+bb61
;bb48=((bb48)<<(10 )|(bb48)>>(32 -10 ));bb61+=(bb55&bb69|~bb55&bb48)+
0x5a827999 +bbv[10 ];bb61=((bb61)<<(11 )|(bb61)>>(32 -11 ))+bb68;bb69=((
bb69)<<(10 )|(bb69)>>(32 -10 ));bb68+=(bb61&bb55|~bb61&bb69)+0x5a827999 +
bbv[6 ];bb68=((bb68)<<(9 )|(bb68)>>(32 -9 ))+bb48;bb55=((bb55)<<(10 )|(
bb55)>>(32 -10 ));bb48+=(bb68&bb61|~bb68&bb55)+0x5a827999 +bbv[15 ];bb48=
((bb48)<<(7 )|(bb48)>>(32 -7 ))+bb69;bb61=((bb61)<<(10 )|(bb61)>>(32 -10 ));
bb69+=(bb48&bb68|~bb48&bb61)+0x5a827999 +bbv[3 ];bb69=((bb69)<<(15 )|(
bb69)>>(32 -15 ))+bb55;bb68=((bb68)<<(10 )|(bb68)>>(32 -10 ));bb55+=(bb69&
bb48|~bb69&bb68)+0x5a827999 +bbv[12 ];bb55=((bb55)<<(7 )|(bb55)>>(32 -7 ))+
bb61;bb48=((bb48)<<(10 )|(bb48)>>(32 -10 ));bb61+=(bb55&bb69|~bb55&bb48)+
0x5a827999 +bbv[0 ];bb61=((bb61)<<(12 )|(bb61)>>(32 -12 ))+bb68;bb69=((
bb69)<<(10 )|(bb69)>>(32 -10 ));bb68+=(bb61&bb55|~bb61&bb69)+0x5a827999 +
bbv[9 ];bb68=((bb68)<<(15 )|(bb68)>>(32 -15 ))+bb48;bb55=((bb55)<<(10 )|(
bb55)>>(32 -10 ));bb48+=(bb68&bb61|~bb68&bb55)+0x5a827999 +bbv[5 ];bb48=(
(bb48)<<(9 )|(bb48)>>(32 -9 ))+bb69;bb61=((bb61)<<(10 )|(bb61)>>(32 -10 ));
bb69+=(bb48&bb68|~bb48&bb61)+0x5a827999 +bbv[2 ];bb69=((bb69)<<(11 )|(
bb69)>>(32 -11 ))+bb55;bb68=((bb68)<<(10 )|(bb68)>>(32 -10 ));bb55+=(bb69&
bb48|~bb69&bb68)+0x5a827999 +bbv[14 ];bb55=((bb55)<<(7 )|(bb55)>>(32 -7 ))+
bb61;bb48=((bb48)<<(10 )|(bb48)>>(32 -10 ));bb61+=(bb55&bb69|~bb55&bb48)+
0x5a827999 +bbv[11 ];bb61=((bb61)<<(13 )|(bb61)>>(32 -13 ))+bb68;bb69=((
bb69)<<(10 )|(bb69)>>(32 -10 ));bb68+=(bb61&bb55|~bb61&bb69)+0x5a827999 +
bbv[8 ];bb68=((bb68)<<(12 )|(bb68)>>(32 -12 ))+bb48;bb55=((bb55)<<(10 )|(
bb55)>>(32 -10 ));bb48+=((bb68|~bb61)^bb55)+0x6ed9eba1 +bbv[3 ];bb48=((
bb48)<<(11 )|(bb48)>>(32 -11 ))+bb69;bb61=((bb61)<<(10 )|(bb61)>>(32 -10 ));
bb69+=((bb48|~bb68)^bb61)+0x6ed9eba1 +bbv[10 ];bb69=((bb69)<<(13 )|(bb69
)>>(32 -13 ))+bb55;bb68=((bb68)<<(10 )|(bb68)>>(32 -10 ));bb55+=((bb69|~
bb48)^bb68)+0x6ed9eba1 +bbv[14 ];bb55=((bb55)<<(6 )|(bb55)>>(32 -6 ))+bb61
;bb48=((bb48)<<(10 )|(bb48)>>(32 -10 ));bb61+=((bb55|~bb69)^bb48)+
0x6ed9eba1 +bbv[4 ];bb61=((bb61)<<(7 )|(bb61)>>(32 -7 ))+bb68;bb69=((bb69)<<
(10 )|(bb69)>>(32 -10 ));bb68+=((bb61|~bb55)^bb69)+0x6ed9eba1 +bbv[9 ];
bb68=((bb68)<<(14 )|(bb68)>>(32 -14 ))+bb48;bb55=((bb55)<<(10 )|(bb55)>>(
32 -10 ));bb48+=((bb68|~bb61)^bb55)+0x6ed9eba1 +bbv[15 ];bb48=((bb48)<<(9
)|(bb48)>>(32 -9 ))+bb69;bb61=((bb61)<<(10 )|(bb61)>>(32 -10 ));bb69+=((
bb48|~bb68)^bb61)+0x6ed9eba1 +bbv[8 ];bb69=((bb69)<<(13 )|(bb69)>>(32 -13
))+bb55;bb68=((bb68)<<(10 )|(bb68)>>(32 -10 ));bb55+=((bb69|~bb48)^bb68)+
0x6ed9eba1 +bbv[1 ];bb55=((bb55)<<(15 )|(bb55)>>(32 -15 ))+bb61;bb48=((
bb48)<<(10 )|(bb48)>>(32 -10 ));bb61+=((bb55|~bb69)^bb48)+0x6ed9eba1 +bbv
[2 ];bb61=((bb61)<<(14 )|(bb61)>>(32 -14 ))+bb68;bb69=((bb69)<<(10 )|(bb69
)>>(32 -10 ));bb68+=((bb61|~bb55)^bb69)+0x6ed9eba1 +bbv[7 ];bb68=((bb68)<<
(8 )|(bb68)>>(32 -8 ))+bb48;bb55=((bb55)<<(10 )|(bb55)>>(32 -10 ));bb48+=((
bb68|~bb61)^bb55)+0x6ed9eba1 +bbv[0 ];bb48=((bb48)<<(13 )|(bb48)>>(32 -13
))+bb69;bb61=((bb61)<<(10 )|(bb61)>>(32 -10 ));bb69+=((bb48|~bb68)^bb61)+
0x6ed9eba1 +bbv[6 ];bb69=((bb69)<<(6 )|(bb69)>>(32 -6 ))+bb55;bb68=((bb68)<<
(10 )|(bb68)>>(32 -10 ));bb55+=((bb69|~bb48)^bb68)+0x6ed9eba1 +bbv[13 ];
bb55=((bb55)<<(5 )|(bb55)>>(32 -5 ))+bb61;bb48=((bb48)<<(10 )|(bb48)>>(32
-10 ));bb61+=((bb55|~bb69)^bb48)+0x6ed9eba1 +bbv[11 ];bb61=((bb61)<<(12 )|
(bb61)>>(32 -12 ))+bb68;bb69=((bb69)<<(10 )|(bb69)>>(32 -10 ));bb68+=((
bb61|~bb55)^bb69)+0x6ed9eba1 +bbv[5 ];bb68=((bb68)<<(7 )|(bb68)>>(32 -7 ))+
bb48;bb55=((bb55)<<(10 )|(bb55)>>(32 -10 ));bb48+=((bb68|~bb61)^bb55)+
0x6ed9eba1 +bbv[12 ];bb48=((bb48)<<(5 )|(bb48)>>(32 -5 ))+bb69;bb61=((bb61
)<<(10 )|(bb61)>>(32 -10 ));bb69+=(bb48&bb61|bb68&~bb61)+0x8f1bbcdc +bbv[
1 ];bb69=((bb69)<<(11 )|(bb69)>>(32 -11 ))+bb55;bb68=((bb68)<<(10 )|(bb68)>>
(32 -10 ));bb55+=(bb69&bb68|bb48&~bb68)+0x8f1bbcdc +bbv[9 ];bb55=((bb55)<<
(12 )|(bb55)>>(32 -12 ))+bb61;bb48=((bb48)<<(10 )|(bb48)>>(32 -10 ));bb61+=
(bb55&bb48|bb69&~bb48)+0x8f1bbcdc +bbv[11 ];bb61=((bb61)<<(14 )|(bb61)>>
(32 -14 ))+bb68;bb69=((bb69)<<(10 )|(bb69)>>(32 -10 ));bb68+=(bb61&bb69|
bb55&~bb69)+0x8f1bbcdc +bbv[10 ];bb68=((bb68)<<(15 )|(bb68)>>(32 -15 ))+
bb48;bb55=((bb55)<<(10 )|(bb55)>>(32 -10 ));bb48+=(bb68&bb55|bb61&~bb55)+
0x8f1bbcdc +bbv[0 ];bb48=((bb48)<<(14 )|(bb48)>>(32 -14 ))+bb69;bb61=((
bb61)<<(10 )|(bb61)>>(32 -10 ));bb69+=(bb48&bb61|bb68&~bb61)+0x8f1bbcdc +
bbv[8 ];bb69=((bb69)<<(15 )|(bb69)>>(32 -15 ))+bb55;bb68=((bb68)<<(10 )|(
bb68)>>(32 -10 ));bb55+=(bb69&bb68|bb48&~bb68)+0x8f1bbcdc +bbv[12 ];bb55=
((bb55)<<(9 )|(bb55)>>(32 -9 ))+bb61;bb48=((bb48)<<(10 )|(bb48)>>(32 -10 ));
bb61+=(bb55&bb48|bb69&~bb48)+0x8f1bbcdc +bbv[4 ];bb61=((bb61)<<(8 )|(
bb61)>>(32 -8 ))+bb68;bb69=((bb69)<<(10 )|(bb69)>>(32 -10 ));bb68+=(bb61&
bb69|bb55&~bb69)+0x8f1bbcdc +bbv[13 ];bb68=((bb68)<<(9 )|(bb68)>>(32 -9 ))+
bb48;bb55=((bb55)<<(10 )|(bb55)>>(32 -10 ));bb48+=(bb68&bb55|bb61&~bb55)+
0x8f1bbcdc +bbv[3 ];bb48=((bb48)<<(14 )|(bb48)>>(32 -14 ))+bb69;bb61=((
bb61)<<(10 )|(bb61)>>(32 -10 ));bb69+=(bb48&bb61|bb68&~bb61)+0x8f1bbcdc +
bbv[7 ];bb69=((bb69)<<(5 )|(bb69)>>(32 -5 ))+bb55;bb68=((bb68)<<(10 )|(
bb68)>>(32 -10 ));bb55+=(bb69&bb68|bb48&~bb68)+0x8f1bbcdc +bbv[15 ];bb55=
((bb55)<<(6 )|(bb55)>>(32 -6 ))+bb61;bb48=((bb48)<<(10 )|(bb48)>>(32 -10 ));
bb61+=(bb55&bb48|bb69&~bb48)+0x8f1bbcdc +bbv[14 ];bb61=((bb61)<<(8 )|(
bb61)>>(32 -8 ))+bb68;bb69=((bb69)<<(10 )|(bb69)>>(32 -10 ));bb68+=(bb61&
bb69|bb55&~bb69)+0x8f1bbcdc +bbv[5 ];bb68=((bb68)<<(6 )|(bb68)>>(32 -6 ))+
bb48;bb55=((bb55)<<(10 )|(bb55)>>(32 -10 ));bb48+=(bb68&bb55|bb61&~bb55)+
0x8f1bbcdc +bbv[6 ];bb48=((bb48)<<(5 )|(bb48)>>(32 -5 ))+bb69;bb61=((bb61)<<
(10 )|(bb61)>>(32 -10 ));bb69+=(bb48&bb61|bb68&~bb61)+0x8f1bbcdc +bbv[2 ];
bb69=((bb69)<<(12 )|(bb69)>>(32 -12 ))+bb55;bb68=((bb68)<<(10 )|(bb68)>>(
32 -10 ));bb55+=(bb69^(bb48|~bb68))+0xa953fd4e +bbv[4 ];bb55=((bb55)<<(9 )|
(bb55)>>(32 -9 ))+bb61;bb48=((bb48)<<(10 )|(bb48)>>(32 -10 ));bb61+=(bb55^
(bb69|~bb48))+0xa953fd4e +bbv[0 ];bb61=((bb61)<<(15 )|(bb61)>>(32 -15 ))+
bb68;bb69=((bb69)<<(10 )|(bb69)>>(32 -10 ));bb68+=(bb61^(bb55|~bb69))+
0xa953fd4e +bbv[5 ];bb68=((bb68)<<(5 )|(bb68)>>(32 -5 ))+bb48;bb55=((bb55)<<
(10 )|(bb55)>>(32 -10 ));bb48+=(bb68^(bb61|~bb55))+0xa953fd4e +bbv[9 ];
bb48=((bb48)<<(11 )|(bb48)>>(32 -11 ))+bb69;bb61=((bb61)<<(10 )|(bb61)>>(
32 -10 ));bb69+=(bb48^(bb68|~bb61))+0xa953fd4e +bbv[7 ];bb69=((bb69)<<(6 )|
(bb69)>>(32 -6 ))+bb55;bb68=((bb68)<<(10 )|(bb68)>>(32 -10 ));bb55+=(bb69^
(bb48|~bb68))+0xa953fd4e +bbv[12 ];bb55=((bb55)<<(8 )|(bb55)>>(32 -8 ))+
bb61;bb48=((bb48)<<(10 )|(bb48)>>(32 -10 ));bb61+=(bb55^(bb69|~bb48))+
0xa953fd4e +bbv[2 ];bb61=((bb61)<<(13 )|(bb61)>>(32 -13 ))+bb68;bb69=((
bb69)<<(10 )|(bb69)>>(32 -10 ));bb68+=(bb61^(bb55|~bb69))+0xa953fd4e +bbv
[10 ];bb68=((bb68)<<(12 )|(bb68)>>(32 -12 ))+bb48;bb55=((bb55)<<(10 )|(
bb55)>>(32 -10 ));bb48+=(bb68^(bb61|~bb55))+0xa953fd4e +bbv[14 ];bb48=((
bb48)<<(5 )|(bb48)>>(32 -5 ))+bb69;bb61=((bb61)<<(10 )|(bb61)>>(32 -10 ));
bb69+=(bb48^(bb68|~bb61))+0xa953fd4e +bbv[1 ];bb69=((bb69)<<(12 )|(bb69)>>
(32 -12 ))+bb55;bb68=((bb68)<<(10 )|(bb68)>>(32 -10 ));bb55+=(bb69^(bb48|~
bb68))+0xa953fd4e +bbv[3 ];bb55=((bb55)<<(13 )|(bb55)>>(32 -13 ))+bb61;
bb48=((bb48)<<(10 )|(bb48)>>(32 -10 ));bb61+=(bb55^(bb69|~bb48))+
0xa953fd4e +bbv[8 ];bb61=((bb61)<<(14 )|(bb61)>>(32 -14 ))+bb68;bb69=((
bb69)<<(10 )|(bb69)>>(32 -10 ));bb68+=(bb61^(bb55|~bb69))+0xa953fd4e +bbv
[11 ];bb68=((bb68)<<(11 )|(bb68)>>(32 -11 ))+bb48;bb55=((bb55)<<(10 )|(
bb55)>>(32 -10 ));bb48+=(bb68^(bb61|~bb55))+0xa953fd4e +bbv[6 ];bb48=((
bb48)<<(8 )|(bb48)>>(32 -8 ))+bb69;bb61=((bb61)<<(10 )|(bb61)>>(32 -10 ));
bb69+=(bb48^(bb68|~bb61))+0xa953fd4e +bbv[15 ];bb69=((bb69)<<(5 )|(bb69)>>
(32 -5 ))+bb55;bb68=((bb68)<<(10 )|(bb68)>>(32 -10 ));bb55+=(bb69^(bb48|~
bb68))+0xa953fd4e +bbv[13 ];bb55=((bb55)<<(6 )|(bb55)>>(32 -6 ))+bb61;bb48
=((bb48)<<(10 )|(bb48)>>(32 -10 ));bb67+=(bb66^(bb63|~bb62))+0x50a28be6 +
bbv[5 ];bb67=((bb67)<<(8 )|(bb67)>>(32 -8 ))+bb65;bb63=((bb63)<<(10 )|(
bb63)>>(32 -10 ));bb65+=(bb67^(bb66|~bb63))+0x50a28be6 +bbv[14 ];bb65=((
bb65)<<(9 )|(bb65)>>(32 -9 ))+bb62;bb66=((bb66)<<(10 )|(bb66)>>(32 -10 ));
bb62+=(bb65^(bb67|~bb66))+0x50a28be6 +bbv[7 ];bb62=((bb62)<<(9 )|(bb62)>>
(32 -9 ))+bb63;bb67=((bb67)<<(10 )|(bb67)>>(32 -10 ));bb63+=(bb62^(bb65|~
bb67))+0x50a28be6 +bbv[0 ];bb63=((bb63)<<(11 )|(bb63)>>(32 -11 ))+bb66;
bb65=((bb65)<<(10 )|(bb65)>>(32 -10 ));bb66+=(bb63^(bb62|~bb65))+
0x50a28be6 +bbv[9 ];bb66=((bb66)<<(13 )|(bb66)>>(32 -13 ))+bb67;bb62=((
bb62)<<(10 )|(bb62)>>(32 -10 ));bb67+=(bb66^(bb63|~bb62))+0x50a28be6 +bbv
[2 ];bb67=((bb67)<<(15 )|(bb67)>>(32 -15 ))+bb65;bb63=((bb63)<<(10 )|(bb63
)>>(32 -10 ));bb65+=(bb67^(bb66|~bb63))+0x50a28be6 +bbv[11 ];bb65=((bb65)<<
(15 )|(bb65)>>(32 -15 ))+bb62;bb66=((bb66)<<(10 )|(bb66)>>(32 -10 ));bb62+=
(bb65^(bb67|~bb66))+0x50a28be6 +bbv[4 ];bb62=((bb62)<<(5 )|(bb62)>>(32 -5
))+bb63;bb67=((bb67)<<(10 )|(bb67)>>(32 -10 ));bb63+=(bb62^(bb65|~bb67))+
0x50a28be6 +bbv[13 ];bb63=((bb63)<<(7 )|(bb63)>>(32 -7 ))+bb66;bb65=((bb65
)<<(10 )|(bb65)>>(32 -10 ));bb66+=(bb63^(bb62|~bb65))+0x50a28be6 +bbv[6 ];
bb66=((bb66)<<(7 )|(bb66)>>(32 -7 ))+bb67;bb62=((bb62)<<(10 )|(bb62)>>(32
-10 ));bb67+=(bb66^(bb63|~bb62))+0x50a28be6 +bbv[15 ];bb67=((bb67)<<(8 )|
(bb67)>>(32 -8 ))+bb65;bb63=((bb63)<<(10 )|(bb63)>>(32 -10 ));bb65+=(bb67^
(bb66|~bb63))+0x50a28be6 +bbv[8 ];bb65=((bb65)<<(11 )|(bb65)>>(32 -11 ))+
bb62;bb66=((bb66)<<(10 )|(bb66)>>(32 -10 ));bb62+=(bb65^(bb67|~bb66))+
0x50a28be6 +bbv[1 ];bb62=((bb62)<<(14 )|(bb62)>>(32 -14 ))+bb63;bb67=((
bb67)<<(10 )|(bb67)>>(32 -10 ));bb63+=(bb62^(bb65|~bb67))+0x50a28be6 +bbv
[10 ];bb63=((bb63)<<(14 )|(bb63)>>(32 -14 ))+bb66;bb65=((bb65)<<(10 )|(
bb65)>>(32 -10 ));bb66+=(bb63^(bb62|~bb65))+0x50a28be6 +bbv[3 ];bb66=((
bb66)<<(12 )|(bb66)>>(32 -12 ))+bb67;bb62=((bb62)<<(10 )|(bb62)>>(32 -10 ));
bb67+=(bb66^(bb63|~bb62))+0x50a28be6 +bbv[12 ];bb67=((bb67)<<(6 )|(bb67)>>
(32 -6 ))+bb65;bb63=((bb63)<<(10 )|(bb63)>>(32 -10 ));bb65+=(bb67&bb63|
bb66&~bb63)+0x5c4dd124 +bbv[6 ];bb65=((bb65)<<(9 )|(bb65)>>(32 -9 ))+bb62;
bb66=((bb66)<<(10 )|(bb66)>>(32 -10 ));bb62+=(bb65&bb66|bb67&~bb66)+
0x5c4dd124 +bbv[11 ];bb62=((bb62)<<(13 )|(bb62)>>(32 -13 ))+bb63;bb67=((
bb67)<<(10 )|(bb67)>>(32 -10 ));bb63+=(bb62&bb67|bb65&~bb67)+0x5c4dd124 +
bbv[3 ];bb63=((bb63)<<(15 )|(bb63)>>(32 -15 ))+bb66;bb65=((bb65)<<(10 )|(
bb65)>>(32 -10 ));bb66+=(bb63&bb65|bb62&~bb65)+0x5c4dd124 +bbv[7 ];bb66=(
(bb66)<<(7 )|(bb66)>>(32 -7 ))+bb67;bb62=((bb62)<<(10 )|(bb62)>>(32 -10 ));
bb67+=(bb66&bb62|bb63&~bb62)+0x5c4dd124 +bbv[0 ];bb67=((bb67)<<(12 )|(
bb67)>>(32 -12 ))+bb65;bb63=((bb63)<<(10 )|(bb63)>>(32 -10 ));bb65+=(bb67&
bb63|bb66&~bb63)+0x5c4dd124 +bbv[13 ];bb65=((bb65)<<(8 )|(bb65)>>(32 -8 ))+
bb62;bb66=((bb66)<<(10 )|(bb66)>>(32 -10 ));bb62+=(bb65&bb66|bb67&~bb66)+
0x5c4dd124 +bbv[5 ];bb62=((bb62)<<(9 )|(bb62)>>(32 -9 ))+bb63;bb67=((bb67)<<
(10 )|(bb67)>>(32 -10 ));bb63+=(bb62&bb67|bb65&~bb67)+0x5c4dd124 +bbv[10 ]
;bb63=((bb63)<<(11 )|(bb63)>>(32 -11 ))+bb66;bb65=((bb65)<<(10 )|(bb65)>>
(32 -10 ));bb66+=(bb63&bb65|bb62&~bb65)+0x5c4dd124 +bbv[14 ];bb66=((bb66)<<
(7 )|(bb66)>>(32 -7 ))+bb67;bb62=((bb62)<<(10 )|(bb62)>>(32 -10 ));bb67+=(
bb66&bb62|bb63&~bb62)+0x5c4dd124 +bbv[15 ];bb67=((bb67)<<(7 )|(bb67)>>(
32 -7 ))+bb65;bb63=((bb63)<<(10 )|(bb63)>>(32 -10 ));bb65+=(bb67&bb63|bb66
&~bb63)+0x5c4dd124 +bbv[8 ];bb65=((bb65)<<(12 )|(bb65)>>(32 -12 ))+bb62;
bb66=((bb66)<<(10 )|(bb66)>>(32 -10 ));bb62+=(bb65&bb66|bb67&~bb66)+
0x5c4dd124 +bbv[12 ];bb62=((bb62)<<(7 )|(bb62)>>(32 -7 ))+bb63;bb67=((bb67
)<<(10 )|(bb67)>>(32 -10 ));bb63+=(bb62&bb67|bb65&~bb67)+0x5c4dd124 +bbv[
4 ];bb63=((bb63)<<(6 )|(bb63)>>(32 -6 ))+bb66;bb65=((bb65)<<(10 )|(bb65)>>
(32 -10 ));bb66+=(bb63&bb65|bb62&~bb65)+0x5c4dd124 +bbv[9 ];bb66=((bb66)<<
(15 )|(bb66)>>(32 -15 ))+bb67;bb62=((bb62)<<(10 )|(bb62)>>(32 -10 ));bb67+=
(bb66&bb62|bb63&~bb62)+0x5c4dd124 +bbv[1 ];bb67=((bb67)<<(13 )|(bb67)>>(
32 -13 ))+bb65;bb63=((bb63)<<(10 )|(bb63)>>(32 -10 ));bb65+=(bb67&bb63|
bb66&~bb63)+0x5c4dd124 +bbv[2 ];bb65=((bb65)<<(11 )|(bb65)>>(32 -11 ))+
bb62;bb66=((bb66)<<(10 )|(bb66)>>(32 -10 ));bb62+=((bb65|~bb67)^bb66)+
0x6d703ef3 +bbv[15 ];bb62=((bb62)<<(9 )|(bb62)>>(32 -9 ))+bb63;bb67=((bb67
)<<(10 )|(bb67)>>(32 -10 ));bb63+=((bb62|~bb65)^bb67)+0x6d703ef3 +bbv[5 ];
bb63=((bb63)<<(7 )|(bb63)>>(32 -7 ))+bb66;bb65=((bb65)<<(10 )|(bb65)>>(32
-10 ));bb66+=((bb63|~bb62)^bb65)+0x6d703ef3 +bbv[1 ];bb66=((bb66)<<(15 )|
(bb66)>>(32 -15 ))+bb67;bb62=((bb62)<<(10 )|(bb62)>>(32 -10 ));bb67+=((
bb66|~bb63)^bb62)+0x6d703ef3 +bbv[3 ];bb67=((bb67)<<(11 )|(bb67)>>(32 -11
))+bb65;bb63=((bb63)<<(10 )|(bb63)>>(32 -10 ));bb65+=((bb67|~bb66)^bb63)+
0x6d703ef3 +bbv[7 ];bb65=((bb65)<<(8 )|(bb65)>>(32 -8 ))+bb62;bb66=((bb66)<<
(10 )|(bb66)>>(32 -10 ));bb62+=((bb65|~bb67)^bb66)+0x6d703ef3 +bbv[14 ];
bb62=((bb62)<<(6 )|(bb62)>>(32 -6 ))+bb63;bb67=((bb67)<<(10 )|(bb67)>>(32
-10 ));bb63+=((bb62|~bb65)^bb67)+0x6d703ef3 +bbv[6 ];bb63=((bb63)<<(6 )|(
bb63)>>(32 -6 ))+bb66;bb65=((bb65)<<(10 )|(bb65)>>(32 -10 ));bb66+=((bb63|
~bb62)^bb65)+0x6d703ef3 +bbv[9 ];bb66=((bb66)<<(14 )|(bb66)>>(32 -14 ))+
bb67;bb62=((bb62)<<(10 )|(bb62)>>(32 -10 ));bb67+=((bb66|~bb63)^bb62)+
0x6d703ef3 +bbv[11 ];bb67=((bb67)<<(12 )|(bb67)>>(32 -12 ))+bb65;bb63=((
bb63)<<(10 )|(bb63)>>(32 -10 ));bb65+=((bb67|~bb66)^bb63)+0x6d703ef3 +bbv
[8 ];bb65=((bb65)<<(13 )|(bb65)>>(32 -13 ))+bb62;bb66=((bb66)<<(10 )|(bb66
)>>(32 -10 ));bb62+=((bb65|~bb67)^bb66)+0x6d703ef3 +bbv[12 ];bb62=((bb62)<<
(5 )|(bb62)>>(32 -5 ))+bb63;bb67=((bb67)<<(10 )|(bb67)>>(32 -10 ));bb63+=((
bb62|~bb65)^bb67)+0x6d703ef3 +bbv[2 ];bb63=((bb63)<<(14 )|(bb63)>>(32 -14
))+bb66;bb65=((bb65)<<(10 )|(bb65)>>(32 -10 ));bb66+=((bb63|~bb62)^bb65)+
0x6d703ef3 +bbv[10 ];bb66=((bb66)<<(13 )|(bb66)>>(32 -13 ))+bb67;bb62=((
bb62)<<(10 )|(bb62)>>(32 -10 ));bb67+=((bb66|~bb63)^bb62)+0x6d703ef3 +bbv
[0 ];bb67=((bb67)<<(13 )|(bb67)>>(32 -13 ))+bb65;bb63=((bb63)<<(10 )|(bb63
)>>(32 -10 ));bb65+=((bb67|~bb66)^bb63)+0x6d703ef3 +bbv[4 ];bb65=((bb65)<<
(7 )|(bb65)>>(32 -7 ))+bb62;bb66=((bb66)<<(10 )|(bb66)>>(32 -10 ));bb62+=((
bb65|~bb67)^bb66)+0x6d703ef3 +bbv[13 ];bb62=((bb62)<<(5 )|(bb62)>>(32 -5 ))+
bb63;bb67=((bb67)<<(10 )|(bb67)>>(32 -10 ));bb63+=(bb62&bb65|~bb62&bb67)+
0x7a6d76e9 +bbv[8 ];bb63=((bb63)<<(15 )|(bb63)>>(32 -15 ))+bb66;bb65=((
bb65)<<(10 )|(bb65)>>(32 -10 ));bb66+=(bb63&bb62|~bb63&bb65)+0x7a6d76e9 +
bbv[6 ];bb66=((bb66)<<(5 )|(bb66)>>(32 -5 ))+bb67;bb62=((bb62)<<(10 )|(
bb62)>>(32 -10 ));bb67+=(bb66&bb63|~bb66&bb62)+0x7a6d76e9 +bbv[4 ];bb67=(
(bb67)<<(8 )|(bb67)>>(32 -8 ))+bb65;bb63=((bb63)<<(10 )|(bb63)>>(32 -10 ));
bb65+=(bb67&bb66|~bb67&bb63)+0x7a6d76e9 +bbv[1 ];bb65=((bb65)<<(11 )|(
bb65)>>(32 -11 ))+bb62;bb66=((bb66)<<(10 )|(bb66)>>(32 -10 ));bb62+=(bb65&
bb67|~bb65&bb66)+0x7a6d76e9 +bbv[3 ];bb62=((bb62)<<(14 )|(bb62)>>(32 -14 ))+
bb63;bb67=((bb67)<<(10 )|(bb67)>>(32 -10 ));bb63+=(bb62&bb65|~bb62&bb67)+
0x7a6d76e9 +bbv[11 ];bb63=((bb63)<<(14 )|(bb63)>>(32 -14 ))+bb66;bb65=((
bb65)<<(10 )|(bb65)>>(32 -10 ));bb66+=(bb63&bb62|~bb63&bb65)+0x7a6d76e9 +
bbv[15 ];bb66=((bb66)<<(6 )|(bb66)>>(32 -6 ))+bb67;bb62=((bb62)<<(10 )|(
bb62)>>(32 -10 ));bb67+=(bb66&bb63|~bb66&bb62)+0x7a6d76e9 +bbv[0 ];bb67=(
(bb67)<<(14 )|(bb67)>>(32 -14 ))+bb65;bb63=((bb63)<<(10 )|(bb63)>>(32 -10 ));
bb65+=(bb67&bb66|~bb67&bb63)+0x7a6d76e9 +bbv[5 ];bb65=((bb65)<<(6 )|(
bb65)>>(32 -6 ))+bb62;bb66=((bb66)<<(10 )|(bb66)>>(32 -10 ));bb62+=(bb65&
bb67|~bb65&bb66)+0x7a6d76e9 +bbv[12 ];bb62=((bb62)<<(9 )|(bb62)>>(32 -9 ))+
bb63;bb67=((bb67)<<(10 )|(bb67)>>(32 -10 ));bb63+=(bb62&bb65|~bb62&bb67)+
0x7a6d76e9 +bbv[2 ];bb63=((bb63)<<(12 )|(bb63)>>(32 -12 ))+bb66;bb65=((
bb65)<<(10 )|(bb65)>>(32 -10 ));bb66+=(bb63&bb62|~bb63&bb65)+0x7a6d76e9 +
bbv[13 ];bb66=((bb66)<<(9 )|(bb66)>>(32 -9 ))+bb67;bb62=((bb62)<<(10 )|(
bb62)>>(32 -10 ));bb67+=(bb66&bb63|~bb66&bb62)+0x7a6d76e9 +bbv[9 ];bb67=(
(bb67)<<(12 )|(bb67)>>(32 -12 ))+bb65;bb63=((bb63)<<(10 )|(bb63)>>(32 -10 ));
bb65+=(bb67&bb66|~bb67&bb63)+0x7a6d76e9 +bbv[7 ];bb65=((bb65)<<(5 )|(
bb65)>>(32 -5 ))+bb62;bb66=((bb66)<<(10 )|(bb66)>>(32 -10 ));bb62+=(bb65&
bb67|~bb65&bb66)+0x7a6d76e9 +bbv[10 ];bb62=((bb62)<<(15 )|(bb62)>>(32 -15
))+bb63;bb67=((bb67)<<(10 )|(bb67)>>(32 -10 ));bb63+=(bb62&bb65|~bb62&
bb67)+0x7a6d76e9 +bbv[14 ];bb63=((bb63)<<(8 )|(bb63)>>(32 -8 ))+bb66;bb65=
((bb65)<<(10 )|(bb65)>>(32 -10 ));bb66+=(bb63^bb62^bb65)+bbv[12 ];bb66=((
bb66)<<(8 )|(bb66)>>(32 -8 ))+bb67;bb62=((bb62)<<(10 )|(bb62)>>(32 -10 ));
bb67+=(bb66^bb63^bb62)+bbv[15 ];bb67=((bb67)<<(5 )|(bb67)>>(32 -5 ))+bb65
;bb63=((bb63)<<(10 )|(bb63)>>(32 -10 ));bb65+=(bb67^bb66^bb63)+bbv[10 ];
bb65=((bb65)<<(12 )|(bb65)>>(32 -12 ))+bb62;bb66=((bb66)<<(10 )|(bb66)>>(
32 -10 ));bb62+=(bb65^bb67^bb66)+bbv[4 ];bb62=((bb62)<<(9 )|(bb62)>>(32 -9
))+bb63;bb67=((bb67)<<(10 )|(bb67)>>(32 -10 ));bb63+=(bb62^bb65^bb67)+
bbv[1 ];bb63=((bb63)<<(12 )|(bb63)>>(32 -12 ))+bb66;bb65=((bb65)<<(10 )|(
bb65)>>(32 -10 ));bb66+=(bb63^bb62^bb65)+bbv[5 ];bb66=((bb66)<<(5 )|(bb66
)>>(32 -5 ))+bb67;bb62=((bb62)<<(10 )|(bb62)>>(32 -10 ));bb67+=(bb66^bb63^
bb62)+bbv[8 ];bb67=((bb67)<<(14 )|(bb67)>>(32 -14 ))+bb65;bb63=((bb63)<<(
10 )|(bb63)>>(32 -10 ));bb65+=(bb67^bb66^bb63)+bbv[7 ];bb65=((bb65)<<(6 )|
(bb65)>>(32 -6 ))+bb62;bb66=((bb66)<<(10 )|(bb66)>>(32 -10 ));bb62+=(bb65^
bb67^bb66)+bbv[6 ];bb62=((bb62)<<(8 )|(bb62)>>(32 -8 ))+bb63;bb67=((bb67)<<
(10 )|(bb67)>>(32 -10 ));bb63+=(bb62^bb65^bb67)+bbv[2 ];bb63=((bb63)<<(13
)|(bb63)>>(32 -13 ))+bb66;bb65=((bb65)<<(10 )|(bb65)>>(32 -10 ));bb66+=(
bb63^bb62^bb65)+bbv[13 ];bb66=((bb66)<<(6 )|(bb66)>>(32 -6 ))+bb67;bb62=(
(bb62)<<(10 )|(bb62)>>(32 -10 ));bb67+=(bb66^bb63^bb62)+bbv[14 ];bb67=((
bb67)<<(5 )|(bb67)>>(32 -5 ))+bb65;bb63=((bb63)<<(10 )|(bb63)>>(32 -10 ));
bb65+=(bb67^bb66^bb63)+bbv[0 ];bb65=((bb65)<<(15 )|(bb65)>>(32 -15 ))+
bb62;bb66=((bb66)<<(10 )|(bb66)>>(32 -10 ));bb62+=(bb65^bb67^bb66)+bbv[3
];bb62=((bb62)<<(13 )|(bb62)>>(32 -13 ))+bb63;bb67=((bb67)<<(10 )|(bb67)>>
(32 -10 ));bb63+=(bb62^bb65^bb67)+bbv[9 ];bb63=((bb63)<<(11 )|(bb63)>>(32
-11 ))+bb66;bb65=((bb65)<<(10 )|(bb65)>>(32 -10 ));bb66+=(bb63^bb62^bb65)+
bbv[11 ];bb66=((bb66)<<(11 )|(bb66)>>(32 -11 ))+bb67;bb62=((bb62)<<(10 )|(
bb62)>>(32 -10 ));bb62+=bb23[1 ]+bb69;bb23[1 ]=bb23[2 ]+bb48+bb65;bb23[2 ]=
bb23[3 ]+bb68+bb67;bb23[3 ]=bb23[4 ]+bb61+bb66;bb23[4 ]=bb23[0 ]+bb55+bb63
;bb23[0 ]=bb62;}}bbb bb1802(bb523*bbj){bb41 bbd bb23[5 ]={0x67452301 ,
0xefcdab89 ,0x98badcfe ,0x10325476 ,0xc3d2e1f0 };bbj->bb10=0 ;bb81(bbj->
bb23,bb23,bb12(bb23));}bbb bb1358(bb523*bbj,bbh bbb*bb496,bbq bb10){
bbh bbf*bb5=(bbh bbf* )bb496;bbq bb385=bbj->bb10%bb12(bbj->bb101);bbj
->bb10+=bb10;bbm(bb385){bbq bb11=bb12(bbj->bb101)-bb385;bb81(bbj->
bb101+bb385,bb5,((bb10)<(bb11)?(bb10):(bb11)));bbm(bb10<bb11)bb2;bb5
+=bb11;bb10-=bb11;bb1255(bbj->bb23,bbj->bb101);}bb90(;bb10>=bb12(bbj
->bb101);bb10-=bb12(bbj->bb101),bb5+=bb12(bbj->bb101))bb1255(bbj->
bb23,bb5);bb81(bbj->bb101,bb5,bb10);}bbb bb1795(bb523*bbj,bbb*bb14){
bbd bb1027[2 ]={(bbd)(bbj->bb10<<3 ),(bbd)(bbj->bb10>>29 )};bbf bb440[
bb12(bb1027)];bbq bbz;bb90(bbz=0 ;bbz<bb12(bb440);bbz++)bb440[bbz]=
bb1027[bbz/4 ]>>((bbz%4 ) *8 )&0xff ;{bbf bb1315[]={0x80 },bb1313[bb12(bbj
->bb101)]={0 };bbq bb385=bbj->bb10%bb12(bbj->bb101);bb1358(bbj,bb1315,
1 );bb1358(bbj,bb1313,(bb12(bbj->bb101) *2 -1 -bb12(bb440)-bb385)%bb12(
bbj->bb101));}bb1358(bbj,bb440,bb12(bb440));bb90(bbz=0 ;bbz<bb12(bbj->
bb23);bbz++)((bbf* )bb14)[bbz]=bbj->bb23[bbz/4 ]>>((bbz%4 ) *8 )&0xff ;}
bbb bb1923(bbb*bb14,bbh bbb*bb5,bbq bb10){bb523 bb95;bb1802(&bb95);
bb1358(&bb95,bb5,bb10);bb1795(&bb95,bb14);}bbb bb1973(bbb*bb14,bb82
bb5){bb1923(bb14,bb5,(bbq)bb1305(bb5));}
