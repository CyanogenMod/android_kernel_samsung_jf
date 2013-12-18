/*
   'aes_xcbc.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Mon Oct 29 11:54:27 2012
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
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbq bb452;bbd bb418[4 * (14 +1 )];}bb364;bbb bb1100(bb364*bbj,
bbh bbb*bb71,bbq bb143);bbb bb1736(bb364*bbj,bbh bbb*bb71,bbq bb143);
bbb bb1036(bb364*bbj,bbb*bb14,bbh bbb*bb5);bbb bb1776(bb364*bbj,bbb*
bb14,bbh bbb*bb5);
#ifdef __cplusplus
}
#endif
bba bbi{bb364 bb2117;bbq bb10;bbf bb101[16 ];bbf bb1928[16 ];bbf bb1926
[16 ];bbf bb1842[16 ];}bb940;bbb bb2042(bb940*bbj,bbh bbb*bb71,bbq bb143
);bbb bb2093(bb940*bbj,bbh bbb*bb5,bbq bb10);bbb bb2103(bb940*bbj,bbb
 *bb14);
#ifdef __cplusplus
}
#endif
bbb bb2042(bb940*bbj,bbh bbb*bb71,bbq bb143){bb364 bb2149;bbf bb2181[
16 ];bbj->bb10=0 ;bb32(bb143==16 );bb1100(&bb2149,bb71,bb143);bb999(bbj
->bb1842,0 ,16 );bb999(bb2181,1 ,16 );bb1036(&bb2149,bb2181,bb2181);bb999
(bbj->bb1928,2 ,16 );bb1036(&bb2149,bbj->bb1928,bbj->bb1928);bb999(bbj
->bb1926,3 ,16 );bb1036(&bb2149,bbj->bb1926,bbj->bb1926);bb1100(&bbj->
bb2117,bb2181,bb143);}bb41 bbb bb1255(bb940*bbj,bbh bbf*bb5){bbq bbz;
bb90(bbz=0 ;bbz<16 ;bbz++)bbj->bb1842[bbz]^=bb5[bbz];bb1036(&bbj->
bb2117,bbj->bb1842,bbj->bb1842);}bbb bb2093(bb940*bbj,bbh bbb*bb496,
bbq bb10){bbh bbf*bb5=(bbh bbf* )bb496;bbq bb385=bbj->bb10?(bbj->bb10
-1 )%16 +1 :0 ;bbj->bb10+=bb10;bbm(bb385){bbq bb11=16 -bb385;bb81(bbj->
bb101+bb385,bb5,((bb10)<(bb11)?(bb10):(bb11)));bbm(bb10<=bb11)bb2;bb5
+=bb11;bb10-=bb11;bb1255(bbj,bbj->bb101);}bb90(;bb10>16 ;bb10-=16 ,bb5
+=16 )bb1255(bbj,bb5);bb81(bbj->bb101,bb5,bb10);}bbb bb2103(bb940*bbj,
bbb*bb14){bb1 bb3;bbq bbz,bb385=bbj->bb10?(bbj->bb10-1 )%16 +1 :0 ;bbm(
bb385<16 ){bbj->bb101[bb385++]=0x80 ;bb999(bbj->bb101+bb385,0 ,16 -bb385);
bb3=bbj->bb1926;}bb54 bb3=bbj->bb1928;bb90(bbz=0 ;bbz<16 ;bbz++)bbj->
bb101[bbz]^=bb3[bbz];bb1255(bbj,bbj->bb101);bb81(bb14,bbj->bb1842,16 );
}
