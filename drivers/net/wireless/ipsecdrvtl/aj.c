/*
   'kmd.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Wed Dec 18 14:11:10 2013
*/
#include"cobf.h"
#ifdef _WIN32
#if defined( UNDER_CE) && defined( bb344) || ! defined( bb338)
#define bb340 1
#define bb336 1
#else
#define bb351 bb357
#define bb330 1
#define bb352 1
#endif
#define bb353 1
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
bba bbs bbl bbf, *bb1;bba bbs bbe bbq, *bb94;bba bb135 bb124, *bb337;
bba bbs bbl bb39, *bb72;bba bbs bb135 bbk, *bb59;bba bbe bbu, *bb133;
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
#ifdef bb113
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
bba bb111 bb255;
#else
bba bbe bbu, *bb133, *bb279;
#define bb201 1
#define bb202 0
bba bb271 bb228, *bb217, *bb230;bba bbe bb237, *bb250, *bb286;bba bbs
bbq, *bb94, *bb288;bba bb6 bb223, *bb284;bba bbs bb6 bb227, *bb258;
bba bb6 bb117, *bb240;bba bbs bb6 bb63, *bb241;bba bb63 bb257, *bb229
;bba bb63 bb276, *bb291;bba bb117 bb111, *bb249;bba bb289 bb262;bba
bb209 bb124;bba bb270 bb82;bba bb115 bb114;bba bb115 bb274;
#ifdef bb226
bba bb236 bb39, *bb72;bba bb254 bbk, *bb59;bba bb252 bbd, *bb29;bba
bb269 bb56, *bb119;
#else
bba bb264 bb39, *bb72;bba bb256 bbk, *bb59;bba bb278 bbd, *bb29;bba
bb207 bb56, *bb119;
#endif
bba bb39 bbf, *bb1, *bb224;bba bbk bb244, *bb214, *bb221;bba bbk bb275
, *bb210, *bb247;bba bbd bb60, *bb122, *bb205;bba bb82 bb37, *bb266, *
bb242;bba bbd bb235, *bb211, *bb222;bba bb114 bb251, *bb268, *bb232;
bba bb56 bb225, *bb280, *bb273;
#define bb141 bbb
bba bbb*bb212, *bb77;bba bbh bbb*bb231;bba bbl bb208;bba bbl*bb233;
bba bbh bbl*bb83;
#if defined( bb113)
bba bbe bb112;
#endif
bba bb112 bb19;bba bb19*bb234;bba bbh bb19*bb188;
#if defined( bb283) || defined( bb238)
bba bb19 bb36;bba bb19 bb120;
#else
bba bbl bb36;bba bbs bbl bb120;
#endif
bba bbh bb36*bb261;bba bb36*bb267;bba bb60 bb265, *bb216;bba bbb*
bb107;bba bb107*bb239;
#define bb215( bb35) bbi bb35##__ { bbe bb219; }; bba bbi bb35##__  * \
 bb35
bba bbi{bb37 bb190,bb246,bb243,bb245;}bb272, *bb281, *bb260;bba bbi{
bb37 bb8,bb193;}bb292, *bb263, *bb277;bba bbi{bb37 bb218,bb248;}bb220
, *bb213, *bb259;
#endif
bba bbh bbf*bb89;
#endif
bba bbf bb101;
#define IN
#define OUT
#ifdef _DEBUG
#define bb145( bbc) bb32( bbc)
#else
#define bb145( bbc) ( bbb)( bbc)
#endif
bba bbe bb161, *bb173;
#define bb287 0
#define bb312 1
#define bb296 2
#define bb323 3
#define bb343 4
bba bbe bb349;bba bbb*bb121;
#endif
#ifdef _WIN32
#ifndef UNDER_CE
#define bb31 bb341
#define bb43 bb346
bba bbs bb6 bb31;bba bb6 bb43;
#endif
#else
#endif
#ifdef _WIN32
bbb*bb128(bb31 bb47);bbb bb108(bbb* );bbb*bb137(bb31 bb159,bb31 bb47);
#else
#define bb128( bbc) bb147(1, bbc, bb140)
#define bb108( bbc) bb331( bbc)
#define bb137( bbc, bbn) bb147( bbc, bbn, bb140)
#endif
#ifdef _WIN32
#define bb32( bbc) bb339( bbc)
#else
#ifdef _DEBUG
bbe bb144(bbh bbl*bb95,bbh bbl*bb25,bbs bb285);
#define bb32( bbc) ( bbb)(( bbc) || ( bb144(# bbc, __FILE__, __LINE__ \
)))
#else
#define bb32( bbc) (( bbb)0)
#endif
#endif
bb43 bb302(bb43*bb324);
#ifndef _WIN32
bbe bb328(bbh bbl*bbg);bbe bb321(bbh bbl*bb20,...);
#endif
#ifdef _WIN32
bba bb342 bb96;
#define bb139( bbc) bb354( bbc)
#define bb143( bbc) bb329( bbc)
#define bb134( bbc) bb348( bbc)
#define bb132( bbc) bb332( bbc)
#else
bba bb334 bb96;
#define bb139( bbc) ( bbb)(  *  bbc = bb356( bbc))
#define bb143( bbc) (( bbb)0)
#define bb134( bbc) bb333( bbc)
#define bb132( bbc) bb358( bbc)
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbd bb9;bbd bb23[4 ];bbf bb102[64 ];}bb530;bbb bb1816(bb530*bbj
);bbb bb1361(bb530*bbj,bbh bbb*bb498,bbq bb9);bbb bb1819(bb530*bbj,
bbb*bb14);bbb bb1852(bbb*bb14,bbh bbb*bb5,bbq bb9);bbb bb1964(bbb*
bb14,bb83 bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbd bb9;bbd bb23[5 ];bbf bb102[64 ];}bb524;bbb bb1794(bb524*bbj
);bbb bb1290(bb524*bbj,bbh bbb*bb5,bbq bb9);bbb bb1802(bb524*bbj,bbb*
bb14);bba bbi{bbd bb9;bbd bb23[8 ];bbf bb102[64 ];}bb529;bbb bb1818(
bb529*bbj);bbb bb1292(bb529*bbj,bbh bbb*bb5,bbq bb9);bbb bb1814(bb529
 *bbj,bbb*bb14);bba bbi{bbd bb9;bb56 bb23[8 ];bbf bb102[128 ];}bb463;
bbb bb1808(bb463*bbj);bbb bb1228(bb463*bbj,bbh bbb*bb5,bbq bb9);bbb
bb1835(bb463*bbj,bbb*bb14);bba bb463 bb926;bbb bb1797(bb926*bbj);bbb
bb1810(bb926*bbj,bbb*bb14);bbb bb1902(bbb*bb14,bbh bbb*bb5,bbq bb9);
bbb bb1866(bbb*bb14,bbh bbb*bb5,bbq bb9);bbb bb1849(bbb*bb14,bbh bbb*
bb5,bbq bb9);bbb bb1929(bbb*bb14,bbh bbb*bb5,bbq bb9);bbb bb2016(bbb*
bb14,bb83 bb5);bbb bb1965(bbb*bb14,bb83 bb5);bbb bb2025(bbb*bb14,bb83
bb5);bbb bb2022(bbb*bb14,bb83 bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbd bb9;bbd bb23[5 ];bbf bb102[64 ];}bb525;bbb bb1801(bb525*bbj
);bbb bb1295(bb525*bbj,bbh bbb*bb498,bbq bb9);bbb bb1795(bb525*bbj,
bbb*bb14);bbb bb1924(bbb*bb14,bbh bbb*bb5,bbq bb9);bbb bb1973(bbb*
bb14,bb83 bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbd bb9;bbd bb23[5 ];bbf bb102[64 ];}bb532;bbb bb1804(bb532*bbj
);bbb bb1356(bb532*bbj,bbh bbb*bb498,bbq bb9);bbb bb1838(bb532*bbj,
bbb*bb14);bbb bb1875(bbb*bb14,bbh bbb*bb5,bbq bb9);bbb bb2002(bbb*
bb14,bb83 bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbb( *bb1059)(bbb*bbj);bba bbb( *bb839)(bbb*bbj,bbh bbb*bb5,bbq
bb9);bba bbb( *bb784)(bbb*bbj,bbb*bb14);bba bbi{bbe bb129;bbq bb38;
bbq bb393;bb1059 bb887;bb839 bb180;bb784 bb746;}bb448;bbb bb1858(
bb448*bbj,bbe bb129);bba bbi{bb448 bbn;bbf bbt[256 -bb12(bb448)];}
bb454;bbb bb1984(bb454*bbj,bbe bb129);bbb bb1991(bb454*bbj);bbb bb2020
(bb454*bbj,bbe bb129);bbb bb1983(bb454*bbj,bbh bbb*bb5,bbq bb9);bbb
bb1976(bb454*bbj,bbb*bb14);bbb bb1989(bbe bb129,bbb*bb14,bbh bbb*bb5,
bbq bb9);bbb bb2049(bbe bb129,bbb*bb14,bb83 bb5);bb83 bb1970(bbe bb129
);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bb448 bbn;bbf bb543[(512 -bb12(bb448))/2 ];bbf bb1341[(512 -bb12
(bb448))/2 ];}bb494;bbb bb1961(bb494*bbj,bbe bb595);bbb bb2005(bb494*
bbj,bbh bbb*bb71,bbq bb142);bbb bb2108(bb494*bbj,bbe bb595,bbh bbb*
bb71,bbq bb142);bbb bb1988(bb494*bbj,bbh bbb*bb5,bbq bb9);bbb bb2007(
bb494*bbj,bbb*bb14);bbb bb2107(bbe bb595,bbh bbb*bb71,bbq bb142,bbb*
bb14,bbh bbb*bb5,bbq bb9);bbb bb2190(bbe bb595,bb83 bb71,bbb*bb14,
bb83 bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbq bb447;bbd bb417[4 * (14 +1 )];}bb364;bbb bb1099(bb364*bbj,
bbh bbb*bb71,bbq bb142);bbb bb1736(bb364*bbj,bbh bbb*bb71,bbq bb142);
bbb bb1035(bb364*bbj,bbb*bb14,bbh bbb*bb5);bbb bb1775(bb364*bbj,bbb*
bb14,bbh bbb*bb5);
#ifdef __cplusplus
}
#endif
bba bbi{bb364 bb2117;bbq bb9;bbf bb102[16 ];bbf bb1926[16 ];bbf bb1928[
16 ];bbf bb1842[16 ];}bb939;bbb bb2042(bb939*bbj,bbh bbb*bb71,bbq bb142
);bbb bb2092(bb939*bbj,bbh bbb*bb5,bbq bb9);bbb bb2103(bb939*bbj,bbb*
bb14);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbb( *bb1865)(bbb*bbj,bbh bbb*bb71,bbq bb142);bba bbi{bbe bb129;
bbq bb38;bbq bb393;bb1865 bb887;bb839 bb180;bb784 bb746;}bb2029;bba
bbi{bb2029 bbn;bbf bbt[512 ];}bb527;bbb bb2144(bb527*bbj,bbe bb129);
bbb bb2168(bb527*bbj,bbh bbb*bb71,bbq bb142);bbb bb1833(bb527*bbj,bbe
bb129,bbh bbb*bb71,bbq bb142);bbb bb1244(bb527*bbj,bbh bbb*bb5,bbq bb9
);bbb bb1815(bb527*bbj,bbb*bb14);bbb bb2165(bbe bb129,bbh bbb*bb71,
bbq bb142,bbb*bb14,bbh bbb*bb5,bbq bb9);bbb bb2195(bbe bb129,bb83 bb71
,bbb*bb14,bb83 bb5);bb83 bb2205(bbe bb129);
#ifdef __cplusplus
}
#endif
bbb bb2144(bb527*bbj,bbe bb129){bb2029 bbn={0 };bbn.bb129=bb129;bb300(
bb129&0xff00 ){bb15 0x1000 :{bb494*bb2238=(bb494* )bbj->bbt;bb1961(
bb2238,bb129&0xff );bbn.bb38=bb2238->bbn.bb38;bbn.bb393=bb2238->bbn.
bb393;}bbn.bb887=(bb1865)bb2005;bbn.bb180=(bb839)bb1988;bbn.bb746=(
bb784)bb2007;bb22;bb15 0x2000 :bbn.bb38=16 ;bbn.bb393=16 ;bbn.bb887=(
bb1865)bb2042;bbn.bb180=(bb839)bb2092;bbn.bb746=(bb784)bb2103;bb22;
bb419:bb32(0 );}bbj->bbn=bbn;}bbb bb2168(bb527*bbj,bbh bbb*bb71,bbq
bb142){bbj->bbn.bb887(bbj->bbt,bb71,bb142);}bbb bb1833(bb527*bbj,bbe
bb129,bbh bbb*bb71,bbq bb142){bb2144(bbj,bb129);bb2168(bbj,bb71,bb142
);}bbb bb1244(bb527*bbj,bbh bbb*bb5,bbq bb9){bbj->bbn.bb180(bbj->bbt,
bb5,bb9);}bbb bb1815(bb527*bbj,bbb*bb14){bbj->bbn.bb746(bbj->bbt,bb14
);}bbb bb2165(bbe bb129,bbh bbb*bb71,bbq bb142,bbb*bb14,bbh bbb*bb5,
bbq bb9){bb527 bb97;bb1833(&bb97,bb129,bb71,bb142);bb1244(&bb97,bb5,
bb9);bb1815(&bb97,bb14);}bbb bb2195(bbe bb129,bb83 bb71,bbb*bb14,bb83
bb5){bb2165(bb129,bb71,(bbq)bb1304(bb71),bb14,bb5,(bbq)bb1304(bb5));}
bb83 bb2205(bbe bb129){bb41 bbl bbg[32 ];bb300(bb129&0xff00 ){bb15
0x1000 :bb1316(bbg,"\x48\x4d\x41\x43\x5f\x25\x73",bb1970(bb129&0xff ));
bb2 bbg;bb15 0x2000 :bb2"\x41\x45\x53\x5f\x58\x43\x42\x43";}bb2 bb91;}
