/*
   'hmac.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Mon Oct 29 11:54:27 2012
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
bba bbi{bbd bb10;bbd bb23[4 ];bbf bb101[64 ];}bb525;bbb bb1817(bb525*
bbj);bbb bb1309(bb525*bbj,bbh bbb*bb496,bbq bb10);bbb bb1820(bb525*
bbj,bbb*bb14);bbb bb1852(bbb*bb14,bbh bbb*bb5,bbq bb10);bbb bb1963(
bbb*bb14,bb82 bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbd bb10;bbd bb23[5 ];bbf bb101[64 ];}bb529;bbb bb1794(bb529*
bbj);bbb bb1288(bb529*bbj,bbh bbb*bb5,bbq bb10);bbb bb1803(bb529*bbj,
bbb*bb14);bba bbi{bbd bb10;bbd bb23[8 ];bbf bb101[64 ];}bb533;bbb bb1819
(bb533*bbj);bbb bb1291(bb533*bbj,bbh bbb*bb5,bbq bb10);bbb bb1815(
bb533*bbj,bbb*bb14);bba bbi{bbd bb10;bb56 bb23[8 ];bbf bb101[128 ];}
bb460;bbb bb1809(bb460*bbj);bbb bb1204(bb460*bbj,bbh bbb*bb5,bbq bb10
);bbb bb1836(bb460*bbj,bbb*bb14);bba bb460 bb913;bbb bb1797(bb913*bbj
);bbb bb1811(bb913*bbj,bbb*bb14);bbb bb1906(bbb*bb14,bbh bbb*bb5,bbq
bb10);bbb bb1866(bbb*bb14,bbh bbb*bb5,bbq bb10);bbb bb1849(bbb*bb14,
bbh bbb*bb5,bbq bb10);bbb bb1929(bbb*bb14,bbh bbb*bb5,bbq bb10);bbb
bb2016(bbb*bb14,bb82 bb5);bbb bb1965(bbb*bb14,bb82 bb5);bbb bb2025(
bbb*bb14,bb82 bb5);bbb bb2018(bbb*bb14,bb82 bb5);
#ifdef __cplusplus
}
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
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbd bb10;bbd bb23[5 ];bbf bb101[64 ];}bb531;bbb bb1805(bb531*
bbj);bbb bb1319(bb531*bbj,bbh bbb*bb496,bbq bb10);bbb bb1839(bb531*
bbj,bbb*bb14);bbb bb1873(bbb*bb14,bbh bbb*bb5,bbq bb10);bbb bb2002(
bbb*bb14,bb82 bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbb( *bb1060)(bbb*bbj);bba bbb( *bb828)(bbb*bbj,bbh bbb*bb5,bbq
bb10);bba bbb( *bb765)(bbb*bbj,bbb*bb14);bba bbi{bbe bb131;bbq bb38;
bbq bb389;bb1060 bb887;bb828 bb180;bb765 bb704;}bb448;bbb bb1855(
bb448*bbj,bbe bb131);bba bbi{bb448 bbn;bbf bbt[256 -bb12(bb448)];}
bb449;bbb bb1984(bb449*bbj,bbe bb131);bbb bb1991(bb449*bbj);bbb bb2020
(bb449*bbj,bbe bb131);bbb bb1983(bb449*bbj,bbh bbb*bb5,bbq bb10);bbb
bb1976(bb449*bbj,bbb*bb14);bbb bb1989(bbe bb131,bbb*bb14,bbh bbb*bb5,
bbq bb10);bbb bb2049(bbe bb131,bbb*bb14,bb82 bb5);bb82 bb1970(bbe
bb131);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bb448 bbn;bbf bb542[(512 -bb12(bb448))/2 ];bbf bb1359[(512 -bb12
(bb448))/2 ];}bb492;bbb bb1961(bb492*bbj,bbe bb586);bbb bb2006(bb492*
bbj,bbh bbb*bb71,bbq bb143);bbb bb2108(bb492*bbj,bbe bb586,bbh bbb*
bb71,bbq bb143);bbb bb1988(bb492*bbj,bbh bbb*bb5,bbq bb10);bbb bb2007
(bb492*bbj,bbb*bb14);bbb bb2107(bbe bb586,bbh bbb*bb71,bbq bb143,bbb*
bb14,bbh bbb*bb5,bbq bb10);bbb bb2190(bbe bb586,bb82 bb71,bbb*bb14,
bb82 bb5);
#ifdef __cplusplus
}
#endif
bbb bb1961(bb492*bbj,bbe bb586){bb1855(&bbj->bbn,bb586);}bbb bb2006(
bb492*bbj,bbh bbb*bb1304,bbq bb143){bb448 bbn=bbj->bbn;bb1 bb542=bbj
->bb542,bb1359=bbj->bb1359;bbh bbf*bb71=(bbh bbf* )bb1304;bbf bb2274[
256 ],bb1547[256 ];bbn.bb887(bb542);bbm(bb143>bbn.bb38){bbn.bb180(bb542
,bb71,bb143);bb32(bbn.bb389<=bb12(bb2274));bbn.bb704(bb542,bb2274);
bb71=bb2274;bb143=bbn.bb389;bb32(bb143<=bbn.bb38);}{bbq bbz;bb32(bbn.
bb38<=bb12(bb1547));bb90(bbz=0 ;bbz<bbn.bb38;bbz++)bb1547[bbz]=0x36 ^(
bbz<bb143?bb71[bbz]:0 );bbn.bb887(bb542);bbn.bb180(bb542,bb1547,bbn.
bb38);}{bbq bbz;bb90(bbz=0 ;bbz<bbn.bb38;bbz++)bb1547[bbz]=0x5c ^(bbz<
bb143?bb71[bbz]:0 );bbn.bb887(bb1359);bbn.bb180(bb1359,bb1547,bbn.bb38
);}}bbb bb2108(bb492*bbj,bbe bb586,bbh bbb*bb71,bbq bb143){bb1961(bbj
,bb586);bb2006(bbj,bb71,bb143);}bbb bb1988(bb492*bbj,bbh bbb*bb5,bbq
bb10){bbj->bbn.bb180(bbj->bb542,bb5,bb10);}bbb bb2007(bb492*bbj,bbb*
bb14){bb448 bbn=bbj->bbn;bb1 bb542=bbj->bb542,bb1359=bbj->bb1359;bbn.
bb704(bb542,bb14);bbn.bb180(bb1359,bb14,bbn.bb389);bbn.bb704(bb1359,
bb14);}bbb bb2107(bbe bb586,bbh bbb*bb71,bbq bb143,bbb*bb14,bbh bbb*
bb5,bbq bb10){bb492 bb95;bb2108(&bb95,bb586,bb71,bb143);bb1988(&bb95,
bb5,bb10);bb2007(&bb95,bb14);}bbb bb2190(bbe bb586,bb82 bb71,bbb*bb14
,bb82 bb5){bb2107(bb586,bb71,(bbq)bb1305(bb71),bb14,bb5,(bbq)bb1305(
bb5));}
