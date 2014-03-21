/*
   'src_compress_LZS_lzsc.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Wed Dec 18 14:11:10 2013
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
bba bbi bb1864 bb1864;bba bbi bb1864*bb484;bbd bb2193(bbb);bbb bb2135
(bb484 bb0,bbb*bb1342);bbk bb2173(bb484 bb0,bbf* *bb1739,bbf* *bb1733
,bbd*bb915,bbd*bb599,bbb*bb1342,bbk bb374,bbk bb2158);bbk bb2293(
bb484 bb0,bbf* *bb1739,bbf* *bb1733,bbd*bb915,bbd*bb599,bbb*bb1342,
bbk bb374);
#define bb201 1
#define bb202 0
#define bb1248 bb201
#define bb2376 bb202
bba bbi{bbf*bb1753;bbd bb990;bbf*bb1743;bbd bb598;bbd bb1285;bbd
bb1744;bbk bb2047;bbf bb1912;bbf bb2609;bbk bb1695;bbd bb538;bbd
bb1839;bbk bb1253;bbd*bb2271;bbd bb2290;bbk bb1333;bbk bb2247;bbk
bb2050;bbk bb2098;bbk bb2060;bbk bb2114;bbk bb1016;bb124 bb1062;bbf
bb1715;bbf bb2608;bb124 bb1945;bbk bb2001;bbk bb1820;}bb2519;bba bbi{
bbf*bb1753;bbd bb990;bbf*bb1743;bbd bb598;bbk bb2091;bbk bb1757;bbk
bb1164;bbk bb2440;bb124 bb1153;bbk bb1169;bbk bb997;bb124 bb1048;
bb124 bb92;bbd bb464;bbk bb1971;}bb2535;bba bbi{bbf bb1843[2048 ];bbk
bb1874[2048 ];bbd bb2083[4096 ];bb2519 bb44;bbf bb2163[2048 ];bb2535 bb80
;bbf bb1152[64 ];}bb2046;bbi bb1864{bb2046*bb446;bb2046 bb13;bbk bb2111
;bbk bb1749;};bba bbi bb2582{bbk bb2317;bb124 bb47;}bb2451;bb41 bb2451
bb2155[24 ]={{0x0 ,0 },{0x0 ,0 },{0x0 ,2 },{0x1 ,2 },{0x2 ,2 },{0xC ,4 },{0xD ,4 },{
0xE ,4 },{0xF0 ,8 },{0xF1 ,8 },{0xF2 ,8 },{0xF3 ,8 },{0xF4 ,8 },{0xF5 ,8 },{0xF6 ,8 }
,{0xF7 ,8 },{0xF8 ,8 },{0xF9 ,8 },{0xFA ,8 },{0xFB ,8 },{0xFC ,8 },{0xFD ,8 },{0xFE
,8 }};bb41 bbk bb2482[5 ][3 ]={{1 ,1 ,1 },{1 ,1 ,1 },{0 ,0 ,1 },{0 ,0 ,1 },{0 ,1 ,1 }};
bb10{bb2230,bb2562,bb2488,bb2473,bb2388};bb10{bb2082,bb2347,bb2382,
bb2349,bb2309,bb2308,bb2421,bb2237,bb2366};bb41 bbb bb2223(bb484 bb0);
bb41 bbb bb2374(bb484 bb0);bb41 bbb bb1104(bb484 bb0);bb41 bbb bb2381
(bb484 bb0);bb41 bbb bb2127(bbd*bb2409,bbd bb2328);bb41 bbb bb2076(
bb484 bb0,bbk bb2156,bbd bb2099);bb41 bbb bb2122(bb484 bb0);bb41 bbk
bb2283(bb484 bb0);bb41 bbk bb1847(bb484 bb0);bb41 bbb bb2072(bb484 bb0
,bbf bbo);bbd bb2193(bbb){bb2 bb12(bb1864);}bb41 bbb bb2223(bb484 bb0
){bb0->bb13.bb44.bb1945=8 ;bb0->bb13.bb44.bb1715=0 ;bb0->bb13.bb44.
bb1333=bb0->bb13.bb44.bb1695=0 ;bb0->bb13.bb44.bb538=bb0->bb13.bb44.
bb1839=0 ;bb0->bb13.bb44.bb2114=bb0->bb13.bb44.bb1820=0 ;bb2;}bbb bb2135
(bb484 bb0,bbb*bb1342){bb0->bb446=(bb2046* )bb1342;bb0->bb13.bb44=bb0
->bb446->bb44;bb0->bb13.bb44.bb1285=0xFFFFFFFFL ;bb0->bb13.bb44.bb1744
=bb0->bb13.bb44.bb1285-1 ;bb0->bb13.bb44.bb2001=0 ;bb2223(bb0);bb2127(
bb0->bb446->bb2083,0xC0000000L );bb0->bb446->bb44=bb0->bb13.bb44;bb0->
bb13.bb80=bb0->bb446->bb80;bb0->bb13.bb80.bb2091=0 ;bb2122(bb0);bb0->
bb446->bb80=bb0->bb13.bb80;bb2;}bb41 bbb bb2127(bbd*bb2409,bbd bb2328
){bbk bbz;bb90(bbz=0 ;bbz<4096 ;bbz++) *bb2409++=bb2328;bb2;}bb41 bbb
bb2374(bb484 bb0){bb930 bbk bbz;bbd*bb2132;bbd bb2225;bb2225=
0xC0000000L ;bbm(bb0->bb13.bb44.bb1285!=0 )bb2225=0x40000000L ;bb2132=
bb0->bb446->bb2083;bb90(bbz=0 ;bbz<4096 ;bbz++,bb2132++)bbm(bb0->bb13.
bb44.bb1285- *bb2132>2048 -2 ) *bb2132=bb2225;bb2;}bb41 bbb bb1104(
bb484 bb0){bb930 bb124 bb1969;bbm(bb0->bb13.bb44.bb598==0 )bb0->bb13.
bb44.bb2001=1 ;bbm((bb1969=bb0->bb13.bb44.bb1062-bb0->bb13.bb44.bb1945
)>0 ){bb0->bb13.bb44.bb1715|=(bb0->bb13.bb44.bb1016>>bb1969);bb0->bb13
.bb44.bb1945=8 ; *bb0->bb13.bb44.bb1743++=bb0->bb13.bb44.bb1715;--bb0
->bb13.bb44.bb598;bb0->bb13.bb44.bb1715=0 ;bb0->bb13.bb44.bb1016&=((1
<<bb1969)-1 );bb0->bb13.bb44.bb1062=bb1969;bb1104(bb0);}bb54 bbm(
bb1969<0 ){bb0->bb13.bb44.bb1715|=(bb0->bb13.bb44.bb1016<<-bb1969);bb0
->bb13.bb44.bb1945-=bb0->bb13.bb44.bb1062;}bb54{bb0->bb13.bb44.bb1715
|=bb0->bb13.bb44.bb1016;bb0->bb13.bb44.bb1945=8 ; *bb0->bb13.bb44.
bb1743++=bb0->bb13.bb44.bb1715;--bb0->bb13.bb44.bb598;bb0->bb13.bb44.
bb1715=0 ;}bb2;}bb41 bbb bb2381(bb484 bb0){bb0->bb13.bb44.bb1016=(
0x180 );bb0->bb13.bb44.bb1062=(9 );bb1104(bb0);bbm(bb0->bb13.bb44.
bb1945!=8 ){ *bb0->bb13.bb44.bb1743++=bb0->bb13.bb44.bb1715;--bb0->
bb13.bb44.bb598;bbm(bb0->bb13.bb44.bb598==0 ){bb0->bb13.bb44.bb2001=1 ;
}}bb0->bb13.bb44.bb1945=8 ;bb0->bb13.bb44.bb1715=0 ;bb2;}bb41 bbb bb2076
(bb484 bb0,bbk bb2156,bbd bb2099){bbm(bb0->bb13.bb44.bb1820==0 ){bbm(
bb2156<128 ){bb0->bb13.bb44.bb1016=(0x180 |bb2156);bb0->bb13.bb44.
bb1062=(9 );bb1104(bb0);}bb54{bb0->bb13.bb44.bb1016=(0x1000 |bb2156);
bb0->bb13.bb44.bb1062=(13 );bb1104(bb0);}}bbm(bb2099>=23 ){bb0->bb13.
bb44.bb1016=(((1 <<4 )-1 ));bb0->bb13.bb44.bb1062=(4 );bb1104(bb0);bb2099
-=((1 <<4 )-1 );bb0->bb13.bb44.bb1839+=((1 <<4 )-1 );bb0->bb13.bb44.bb1820=
1 ;}bb54{bb0->bb13.bb44.bb1016=(bb2155[(bbk)bb2099].bb2317);bb0->bb13.
bb44.bb1062=(bb2155[(bbk)bb2099].bb47);bb1104(bb0);bb0->bb13.bb44.
bb1820=0 ;bb0->bb13.bb44.bb538=0 ;bb0->bb13.bb44.bb1839=0 ;bb0->bb1749=
bb2388;}bb2;}bb41 bbb bb2194(bb484 bb0,bbk bb374){bbm(bb0->bb13.bb44.
bb2114==1 ){bbm(bb0->bb13.bb44.bb538==0 ){bb0->bb13.bb44.bb1016=((bb0->
bb13.bb44.bb1912));bb0->bb13.bb44.bb1062=(9 );bb1104(bb0);bb0->bb1749=
bb2230;}bb54 bb2076(bb0,bb0->bb13.bb44.bb1333,bb0->bb13.bb44.bb538);}
bb2381(bb0);bbm((bb374&0x04 )==0 ){bbm(0 -bb0->bb13.bb44.bb1285>=2048 )bb2127
(bb0->bb446->bb2083,0xC0000000L );bb54 bbm(0x80000000L -bb0->bb13.bb44.
bb1285>=2048 )bb2127(bb0->bb446->bb2083,0x40000000L );bb0->bb13.bb44.
bb1285+=2048 ;bb0->bb13.bb44.bb1744+=2048 ;bb2223(bb0);}bb0->bb13.bb44.
bb2114=0 ;bb2;}bbk bb2173(bb484 bb0,bbf* *bb1739,bbf* *bb1733,bbd*
bb915,bbd*bb599,bbb*bb1342,bbk bb374,bbk bb2158){bb930 bbk bb2182;
bb930 bbk bb2040;bbk bb2166;bbk bb1077=0 ;bb0->bb446=(bb2046* )bb1342;
bb0->bb13.bb44=bb0->bb446->bb44;bb0->bb13.bb44.bb1753= *bb1739;bb0->
bb13.bb44.bb990= *bb915;bb0->bb13.bb44.bb1743= *bb1733;bb0->bb13.bb44
.bb598= *bb599;bb0->bb13.bb44.bb2001=0 ;bb0->bb2111=0X0018 &bb374;bbm(
bb0->bb2111>0x0010 ){bb0->bb2111=0x0010 ;}bb0->bb2111>>=3 ;bbm( *bb599<=
15 )bb1077=0 ;bb54 bbm(bb0->bb13.bb44.bb990!=0 ){bb0->bb13.bb44.bb598-=
15 ;bbm(bb0->bb13.bb44.bb2114==0 ){bb0->bb13.bb44.bb1912= *bb0->bb13.
bb44.bb1753++;--bb0->bb13.bb44.bb990;++bb0->bb13.bb44.bb1285;++bb0->
bb13.bb44.bb1744;bb0->bb13.bb44.bb2047=(bbk)bb0->bb13.bb44.bb1744&(
2048 -1 );bb0->bb446->bb1843[(bbk)bb0->bb13.bb44.bb1285&(2048 -1 )]=bb0->
bb13.bb44.bb1912;bb0->bb13.bb44.bb1695=(bb0->bb13.bb44.bb1695<<8 )+bb0
->bb13.bb44.bb1912;bb0->bb13.bb44.bb2114=1 ;}bb106((bb0->bb13.bb44.
bb990!=0 )&&(bb0->bb13.bb44.bb2001==0 )){++bb0->bb13.bb44.bb1285;++bb0
->bb13.bb44.bb1744;bb0->bb13.bb44.bb2047=(bbk)bb0->bb13.bb44.bb1744&(
2048 -1 );bbm(((bb0->bb13.bb44.bb1285&0x7FFFFFFFL )==0 ))bb2374(bb0);bb0
->bb446->bb1843[(bbk)bb0->bb13.bb44.bb1285&(2048 -1 )]=bb0->bb13.bb44.
bb1912= *bb0->bb13.bb44.bb1753++;bb0->bb13.bb44.bb1695=(bb0->bb13.
bb44.bb1695<<8 )+bb0->bb13.bb44.bb1912;--bb0->bb13.bb44.bb990;bb0->
bb13.bb44.bb2271=bb0->bb446->bb2083+((((bb0->bb13.bb44.bb1695)&0xFF00
)>>4 )^((bb0->bb13.bb44.bb1695)&0x00FF ));bbm((bb0->bb13.bb44.bb2290=
bb0->bb13.bb44.bb1744- *bb0->bb13.bb44.bb2271)>2048 -2 ){bb0->bb446->
bb1874[bb0->bb13.bb44.bb2047]=0 ;bbm(bb0->bb13.bb44.bb538!=0 ){bb2076(
bb0,bb0->bb13.bb44.bb1333,bb0->bb13.bb44.bb538);}bb54{bb0->bb13.bb44.
bb1016=((bb0->bb13.bb44.bb1695>>8 ));bb0->bb13.bb44.bb1062=(9 );bb1104(
bb0);bb0->bb1749=bb2230;}}bb54{bb0->bb446->bb1874[bb0->bb13.bb44.
bb2047]=(bbk)bb0->bb13.bb44.bb2290;bbm(bb0->bb13.bb44.bb538!=0 ){bbm((
bb0->bb446->bb1843[(bbk)(((bbd)bb0->bb13.bb44.bb1253+bb0->bb13.bb44.
bb538+bb0->bb13.bb44.bb1839)&(bbd)(2048 -1 ))]==bb0->bb13.bb44.bb1912)&&
((bb0->bb13.bb44.bb538+bb0->bb13.bb44.bb1839)<(bbd)0xFFFFFFFFL )){++
bb0->bb13.bb44.bb538;bb0->bb1749=bb2488;bbm(bb0->bb13.bb44.bb1820){
bbm(bb0->bb13.bb44.bb538>=23 ){bb0->bb13.bb44.bb1016=(((1 <<4 )-1 ));bb0
->bb13.bb44.bb1062=(4 );bb1104(bb0);bb0->bb13.bb44.bb538-=((1 <<4 )-1 );
bb0->bb13.bb44.bb1839+=((1 <<4 )-1 );}}bb54 bbm(bb0->bb13.bb44.bb538>=23
){bbm(bb0->bb13.bb44.bb1333<128 ){bb0->bb13.bb44.bb1016=(0x180 |bb0->
bb13.bb44.bb1333);bb0->bb13.bb44.bb1062=(9 );bb1104(bb0);}bb54{bb0->
bb13.bb44.bb1016=(0x1000 |bb0->bb13.bb44.bb1333);bb0->bb13.bb44.bb1062
=(13 );bb1104(bb0);}bb0->bb13.bb44.bb1016=(((1 <<4 )-1 ));bb0->bb13.bb44.
bb1062=(4 );bb1104(bb0);bb0->bb13.bb44.bb538-=((1 <<4 )-1 );bb0->bb13.
bb44.bb1839+=((1 <<4 )-1 );bb0->bb13.bb44.bb1820=1 ;}}bb54 bbm(bb0->bb13.
bb44.bb1820){bb0->bb13.bb44.bb1016=(bb2155[(bbk)bb0->bb13.bb44.bb538]
.bb2317);bb0->bb13.bb44.bb1062=(bb2155[(bbk)bb0->bb13.bb44.bb538].
bb47);bb1104(bb0);bb0->bb13.bb44.bb538=0 ;bb0->bb13.bb44.bb1839=0 ;bb0
->bb13.bb44.bb1820=0 ;bb0->bb1749=bb2388;}bb54 bbm(bb0->bb13.bb44.
bb538>=8 ){bb2076(bb0,bb0->bb13.bb44.bb1333,bb0->bb13.bb44.bb538);}
bb54{bb2040=0 ;bb0->bb13.bb44.bb2098=bb0->bb13.bb44.bb1333;bb106((bb0
->bb446->bb1874[bb0->bb13.bb44.bb1253]!=0 )&&(bb2040==0 )&&(bb0->bb13.
bb44.bb2060<bb2158)&&(bb0->bb13.bb44.bb2098<(bbk)(2048 -bb0->bb13.bb44
.bb538))){bb0->bb13.bb44.bb2098+=bb0->bb446->bb1874[bb0->bb13.bb44.
bb1253];++bb0->bb13.bb44.bb2060;bbm(bb0->bb13.bb44.bb2098<(bbk)(2048 -
bb0->bb13.bb44.bb538)){bb0->bb13.bb44.bb1253=bb0->bb13.bb44.bb1253-
bb0->bb446->bb1874[bb0->bb13.bb44.bb1253]&(2048 -1 );bbm(bb0->bb446->
bb1843[bb0->bb13.bb44.bb1253]==bb0->bb446->bb1843[bb0->bb13.bb44.
bb2247]){bb2040=1 ;bb90(bb2182=2 ,bb2166=(bb0->bb13.bb44.bb1253+2 )&(
2048 -1 );bb2182<=(bbk)bb0->bb13.bb44.bb538;bb2182++,bb2166=(bb2166+1 )&
(2048 -1 )){bbm(bb0->bb446->bb1843[bb2166]!=bb0->bb446->bb1843[(bb0->
bb13.bb44.bb2247+bb2182)&(2048 -1 )]){bb2040=0 ;bb22;}}}}}bbm(bb2040){
bb0->bb13.bb44.bb1333=bb0->bb13.bb44.bb2098;++bb0->bb13.bb44.bb538;
bb0->bb1749=bb2473;}bb54{bb2076(bb0,bb0->bb13.bb44.bb1333,bb0->bb13.
bb44.bb538);}}}bb54{bb0->bb13.bb44.bb2050=(bbk)bb0->bb13.bb44.bb2290;
bb0->bb13.bb44.bb2060=0 ;bb565{bb0->bb13.bb44.bb1253=(bbk)(bb0->bb13.
bb44.bb1744-bb0->bb13.bb44.bb2050&(2048 -1 ));bbm(bb0->bb446->bb1843[
bb0->bb13.bb44.bb1253]==(bbf)(bb0->bb13.bb44.bb1695>>8 )){bb0->bb13.
bb44.bb538=2 ;bb0->bb13.bb44.bb2247=bb0->bb13.bb44.bb2047;bb0->bb13.
bb44.bb1333=bb0->bb13.bb44.bb2050;bb0->bb1749=bb2562;bb22;}bb54{bb0->
bb13.bb44.bb2050+=bb0->bb446->bb1874[bb0->bb13.bb44.bb1253];++bb0->
bb13.bb44.bb2060;}}bb106((bb0->bb446->bb1874[bb0->bb13.bb44.bb1253]!=
0 )&&(bb0->bb13.bb44.bb2060<bb2158)&&(bb0->bb13.bb44.bb2050<2048 -2 ));
bbm(bb0->bb13.bb44.bb538==0 ){bb0->bb13.bb44.bb1016=((bb0->bb13.bb44.
bb1695>>8 ));bb0->bb13.bb44.bb1062=(9 );bb1104(bb0);bb0->bb1749=bb2230;
}}}bbm(bb2482[bb0->bb1749][bb0->bb2111]){ *bb0->bb13.bb44.bb2271=bb0
->bb13.bb44.bb1744;}}bbm(bb0->bb13.bb44.bb990==0 ){bb1077=1 ;bbm(bb374&
0x01 ){bb2194(bb0,bb374);bb1077|=4 ;}}bbm((bb0->bb13.bb44.bb598==0 )||(
bb0->bb13.bb44.bb2001)){bbm(!(bb1077&1 )){bb1077=2 ;bbm(bb374&0x02 ){
bb2194(bb0,bb374);bb1077|=4 ;}}bb54{bb1077|=3 ;bbm((!(bb374&0x01 ))&&(
bb374&0x02 )){bb2194(bb0,bb374);bb1077|=4 ;}}}bb0->bb13.bb44.bb598+=15 ;
}bb54{bb1077=1 ;bbm(bb374&0x01 ){bb2194(bb0,bb374);bb1077|=4 ;}}bb0->
bb446->bb44=bb0->bb13.bb44; *bb1739=bb0->bb13.bb44.bb1753; *bb915=bb0
->bb13.bb44.bb990; *bb1733=bb0->bb13.bb44.bb1743; *bb599=bb0->bb13.
bb44.bb598;bb2(bb1077);}bb41 bbb bb2122(bb484 bb0){bb0->bb13.bb80.
bb2091&=(2048 -1 );bb0->bb13.bb80.bb1048=bb0->bb13.bb80.bb1153=bb0->
bb13.bb80.bb92=0 ;bb0->bb13.bb80.bb997=bb0->bb13.bb80.bb1757=bb0->bb13
.bb80.bb1169=0 ;bb0->bb13.bb80.bb464=0 ;bb0->bb13.bb80.bb1971=0 ;bb0->
bb13.bb80.bb1164=bb2082;bb0->bb13.bb80.bb2440=1 ;bb2;}bb41 bbk bb2283(
bb484 bb0){bbm((bb0->bb13.bb80.bb990==0 )&&(bb0->bb13.bb80.bb1048==0 ))bb0
->bb13.bb80.bb997=bb2376;bb54{bb0->bb13.bb80.bb997=bb1248;bbm(bb0->
bb13.bb80.bb1048==0 ){bb0->bb13.bb80.bb1757= *bb0->bb13.bb80.bb1753++;
--bb0->bb13.bb80.bb990;bb0->bb13.bb80.bb1048=7 ;bb0->bb13.bb80.bb1169=
(bb0->bb13.bb80.bb1757>127 )?1 :0 ;bb0->bb13.bb80.bb1757&=((bbf)0x7F );}
bb54{bb0->bb13.bb80.bb1169=(bb0->bb13.bb80.bb1757>>(bb0->bb13.bb80.
bb1048-1 ));--bb0->bb13.bb80.bb1048;bb0->bb13.bb80.bb1757&=((bbf)0xFF
>>(8 -bb0->bb13.bb80.bb1048));}}bb2(bb0->bb13.bb80.bb997);}bb41 bbk
bb1847(bb484 bb0){bbk bb2154;bb124 bb8;bbm(bb0->bb13.bb80.bb997==
bb1248)bb0->bb13.bb80.bb1169=0 ;bb54 bb0->bb13.bb80.bb997=bb1248;bb106
((bb0->bb13.bb80.bb1153>0 )&&(bb0->bb13.bb80.bb997==bb1248)){bbm((bb0
->bb13.bb80.bb990==0 )&&(bb0->bb13.bb80.bb1048==0 ))bb0->bb13.bb80.
bb997=bb2376;bb54{bbm(bb0->bb13.bb80.bb1048==0 ){bb0->bb13.bb80.bb1757
= *bb0->bb13.bb80.bb1753++;--bb0->bb13.bb80.bb990;bb0->bb13.bb80.
bb1048=8 ;}bb2154=bb0->bb13.bb80.bb1757;bbm((bb8=bb0->bb13.bb80.bb1153
-bb0->bb13.bb80.bb1048)>0 )bb2154<<=bb8;bb54 bb2154>>=-bb8;bb0->bb13.
bb80.bb1169|=bb2154;bb8=((((8 )<(bb0->bb13.bb80.bb1153)?(8 ):(bb0->bb13
.bb80.bb1153)))<(bb0->bb13.bb80.bb1048)?(((8 )<(bb0->bb13.bb80.bb1153)?
(8 ):(bb0->bb13.bb80.bb1153))):(bb0->bb13.bb80.bb1048));bb0->bb13.bb80
.bb1153-=bb8;bb0->bb13.bb80.bb1048-=bb8;bb0->bb13.bb80.bb1757&=((bbf)0xFF
>>(8 -bb0->bb13.bb80.bb1048));}}bb2(bb0->bb13.bb80.bb997);}bb41 bbb
bb2072(bb484 bb0,bbf bbo){bbm(bb0->bb13.bb80.bb598!=0 ){ *bb0->bb13.
bb80.bb1743++=bbo;--bb0->bb13.bb80.bb598;bb0->bb446->bb2163[bb0->bb13
.bb80.bb2091++]=(bbf)bbo;bb0->bb13.bb80.bb2091&=(2048 -1 );}}bbk bb2293
(bb484 bb0,bbf* *bb1739,bbf* *bb1733,bbd*bb915,bbd*bb599,bbb*bb1342,
bbk bb374){bbk bb2198=0 ;bbk bb1077=0 ;bb0->bb446=(bb2046* )bb1342;bb0
->bb13.bb80=bb0->bb446->bb80;bb0->bb13.bb80.bb1753= *bb1739;bb0->bb13
.bb80.bb990= *bb915;bb0->bb13.bb80.bb1743= *bb1733;bb0->bb13.bb80.
bb598= *bb599;bbm(bb374&0x01 ){bb2122(bb0);}bbm((bb0->bb13.bb80.bb990
!=0 )&&(bb0->bb13.bb80.bb598!=0 )){bb106((bb0->bb13.bb80.bb598!=0 )&&((
bb0->bb13.bb80.bb990!=0 )||(bb0->bb13.bb80.bb1048!=0 ))&&(bb2198==0 )){
bbm(bb0->bb13.bb80.bb1971){bb106((bb0->bb13.bb80.bb598!=0 )&&(bb0->
bb13.bb80.bb464>0 )){bb0->bb13.bb80.bb92&=(2048 -1 );bb2072(bb0,bb0->
bb446->bb2163[bb0->bb13.bb80.bb92++]);--bb0->bb13.bb80.bb464;}bbm(bb0
->bb13.bb80.bb464==0 )bb0->bb13.bb80.bb1971=0 ;bb0->bb13.bb80.bb1164=
bb2082;}bb54{bb300(bb0->bb13.bb80.bb1164){bb15 bb2082:bb2283(bb0);bbm
(bb0->bb13.bb80.bb1169==0 ){bb0->bb13.bb80.bb1153=8 ;bb0->bb13.bb80.
bb1164=bb2347;bb15 bb2347:bb1847(bb0);bbm(bb0->bb13.bb80.bb997==
bb1248){bb2072(bb0,(bbf)bb0->bb13.bb80.bb1169);bb0->bb13.bb80.bb1164=
bb2082;}}bb54{bb0->bb13.bb80.bb1164=bb2382;bb15 bb2382:bb2283(bb0);
bbm(bb0->bb13.bb80.bb997==bb1248){bb0->bb13.bb80.bb1153=bb0->bb13.
bb80.bb1169?7 :11 ;bb0->bb13.bb80.bb1164=bb2349;bb15 bb2349:bb1847(bb0);
bbm(bb0->bb13.bb80.bb997==bb1248){bb0->bb13.bb80.bb92=bb0->bb13.bb80.
bb1169;bbm(bb0->bb13.bb80.bb92==0 )bb2198=1 ;bb54{bb0->bb13.bb80.bb92=
bb0->bb13.bb80.bb2091-bb0->bb13.bb80.bb92;bb0->bb13.bb80.bb1153=2 ;bb0
->bb13.bb80.bb1164=bb2309;bb15 bb2309:bb1847(bb0);bbm(bb0->bb13.bb80.
bb997==bb1248){bb0->bb13.bb80.bb464=2 +bb0->bb13.bb80.bb1169;bbm(bb0->
bb13.bb80.bb464==5 ){bb0->bb13.bb80.bb1153=2 ;bb0->bb13.bb80.bb1164=
bb2308;bb15 bb2308:bb1847(bb0);bbm(bb0->bb13.bb80.bb997==bb1248){bb0
->bb13.bb80.bb464+=bb0->bb13.bb80.bb1169;bbm(bb0->bb13.bb80.bb464==8 ){
bb0->bb13.bb80.bb1153=4 ;bb0->bb13.bb80.bb1164=bb2421;bb15 bb2421:
bb1847(bb0);bbm(bb0->bb13.bb80.bb997==bb1248){bb0->bb13.bb80.bb464+=
bb0->bb13.bb80.bb1169;bbm(bb0->bb13.bb80.bb464==23 ){bb565{bb15 bb2237
:bb106((bb0->bb13.bb80.bb598!=0 )&&(bb0->bb13.bb80.bb464>0 )){bb0->bb13
.bb80.bb92&=(2048 -1 );bb2072(bb0,bb0->bb446->bb2163[bb0->bb13.bb80.
bb92++]);--bb0->bb13.bb80.bb464;}bbm(bb0->bb13.bb80.bb598==0 ){bb0->
bb13.bb80.bb1164=bb2237;bb22;}bb54{bb0->bb13.bb80.bb1153=4 ;bb0->bb13.
bb80.bb1164=bb2366;bb15 bb2366:bb1847(bb0);bbm(bb0->bb13.bb80.bb997==
bb1248)bb0->bb13.bb80.bb464+=bb0->bb13.bb80.bb1169;bb54 bb22;}}bb106(
bb0->bb13.bb80.bb1169==((1 <<4 )-1 ));}}}}}}bbm((bb0->bb13.bb80.bb997==
bb1248)&&(bb0->bb13.bb80.bb1164!=bb2237)){bb0->bb13.bb80.bb1971=1 ;}}}
}}}}}bbm(bb0->bb13.bb80.bb1971){bb106((bb0->bb13.bb80.bb598!=0 )&&(bb0
->bb13.bb80.bb464>0 )){bb0->bb13.bb80.bb92&=(2048 -1 );bb2072(bb0,bb0->
bb446->bb2163[bb0->bb13.bb80.bb92++]);--bb0->bb13.bb80.bb464;}bbm(bb0
->bb13.bb80.bb464==0 )bb0->bb13.bb80.bb1971=0 ;bb0->bb13.bb80.bb1164=
bb2082;}}bbm(bb2198){bb2122(bb0);bb1077|=4 ;}bbm(bb0->bb13.bb80.bb990
==0 ){bb1077|=1 ;}bbm(bb0->bb13.bb80.bb598==0 ){bb1077|=2 ;}bb0->bb446->
bb80=bb0->bb13.bb80; *bb1739=bb0->bb13.bb80.bb1753; *bb915=bb0->bb13.
bb80.bb990; *bb1733=bb0->bb13.bb80.bb1743; *bb599=bb0->bb13.bb80.
bb598;bb2(bb1077);}
