/*
   'src_compress_LZS_lzsc.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Mon Oct 29 11:54:27 2012
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
bba bbi bb1864 bb1864;bba bbi bb1864*bb478;bbd bb2193(bbb);bbb bb2135
(bb478 bb0,bbb*bb1314);bbk bb2173(bb478 bb0,bbf* *bb1739,bbf* *bb1733
,bbd*bb916,bbd*bb597,bbb*bb1314,bbk bb374,bbk bb2158);bbk bb2293(
bb478 bb0,bbf* *bb1739,bbf* *bb1733,bbd*bb916,bbd*bb597,bbb*bb1314,
bbk bb374);
#define bb201 1
#define bb202 0
#define bb1249 bb201
#define bb2376 bb202
bba bbi{bbf*bb1753;bbd bb988;bbf*bb1743;bbd bb607;bbd bb1279;bbd
bb1746;bbk bb2048;bbf bb1912;bbf bb2611;bbk bb1690;bbd bb536;bbd
bb1840;bbk bb1254;bbd*bb2272;bbd bb2290;bbk bb1337;bbk bb2248;bbk
bb2050;bbk bb2098;bbk bb2059;bbk bb2114;bbk bb1021;bb124 bb1062;bbf
bb1711;bbf bb2610;bb124 bb1944;bbk bb1999;bbk bb1821;}bb2519;bba bbi{
bbf*bb1753;bbd bb988;bbf*bb1743;bbd bb607;bbk bb2090;bbk bb1758;bbk
bb1165;bbk bb2440;bb124 bb1153;bbk bb1173;bbk bb998;bb124 bb1051;
bb124 bb92;bbd bb470;bbk bb1971;}bb2536;bba bbi{bbf bb1843[2048 ];bbk
bb1872[2048 ];bbd bb2083[4096 ];bb2519 bb44;bbf bb2164[2048 ];bb2536 bb80
;bbf bb1152[64 ];}bb2046;bbi bb1864{bb2046*bb453;bb2046 bb13;bbk bb2112
;bbk bb1750;};bba bbi bb2583{bbk bb2317;bb124 bb47;}bb2451;bb41 bb2451
bb2156[24 ]={{0x0 ,0 },{0x0 ,0 },{0x0 ,2 },{0x1 ,2 },{0x2 ,2 },{0xC ,4 },{0xD ,4 },{
0xE ,4 },{0xF0 ,8 },{0xF1 ,8 },{0xF2 ,8 },{0xF3 ,8 },{0xF4 ,8 },{0xF5 ,8 },{0xF6 ,8 }
,{0xF7 ,8 },{0xF8 ,8 },{0xF9 ,8 },{0xFA ,8 },{0xFB ,8 },{0xFC ,8 },{0xFD ,8 },{0xFE
,8 }};bb41 bbk bb2481[5 ][3 ]={{1 ,1 ,1 },{1 ,1 ,1 },{0 ,0 ,1 },{0 ,0 ,1 },{0 ,1 ,1 }};
bb9{bb2230,bb2563,bb2488,bb2473,bb2387};bb9{bb2082,bb2347,bb2382,
bb2350,bb2308,bb2309,bb2423,bb2237,bb2366};bb41 bbb bb2224(bb478 bb0);
bb41 bbb bb2374(bb478 bb0);bb41 bbb bb1101(bb478 bb0);bb41 bbb bb2381
(bb478 bb0);bb41 bbb bb2128(bbd*bb2410,bbd bb2325);bb41 bbb bb2077(
bb478 bb0,bbk bb2157,bbd bb2099);bb41 bbb bb2122(bb478 bb0);bb41 bbk
bb2283(bb478 bb0);bb41 bbk bb1847(bb478 bb0);bb41 bbb bb2072(bb478 bb0
,bbf bbo);bbd bb2193(bbb){bb2 bb12(bb1864);}bb41 bbb bb2224(bb478 bb0
){bb0->bb13.bb44.bb1944=8 ;bb0->bb13.bb44.bb1711=0 ;bb0->bb13.bb44.
bb1337=bb0->bb13.bb44.bb1690=0 ;bb0->bb13.bb44.bb536=bb0->bb13.bb44.
bb1840=0 ;bb0->bb13.bb44.bb2114=bb0->bb13.bb44.bb1821=0 ;bb2;}bbb bb2135
(bb478 bb0,bbb*bb1314){bb0->bb453=(bb2046* )bb1314;bb0->bb13.bb44=bb0
->bb453->bb44;bb0->bb13.bb44.bb1279=0xFFFFFFFFL ;bb0->bb13.bb44.bb1746
=bb0->bb13.bb44.bb1279-1 ;bb0->bb13.bb44.bb1999=0 ;bb2224(bb0);bb2128(
bb0->bb453->bb2083,0xC0000000L );bb0->bb453->bb44=bb0->bb13.bb44;bb0->
bb13.bb80=bb0->bb453->bb80;bb0->bb13.bb80.bb2090=0 ;bb2122(bb0);bb0->
bb453->bb80=bb0->bb13.bb80;bb2;}bb41 bbb bb2128(bbd*bb2410,bbd bb2325
){bbk bbz;bb90(bbz=0 ;bbz<4096 ;bbz++) *bb2410++=bb2325;bb2;}bb41 bbb
bb2374(bb478 bb0){bb925 bbk bbz;bbd*bb2132;bbd bb2223;bb2223=
0xC0000000L ;bbm(bb0->bb13.bb44.bb1279!=0 )bb2223=0x40000000L ;bb2132=
bb0->bb453->bb2083;bb90(bbz=0 ;bbz<4096 ;bbz++,bb2132++)bbm(bb0->bb13.
bb44.bb1279- *bb2132>2048 -2 ) *bb2132=bb2223;bb2;}bb41 bbb bb1101(
bb478 bb0){bb925 bb124 bb1969;bbm(bb0->bb13.bb44.bb607==0 )bb0->bb13.
bb44.bb1999=1 ;bbm((bb1969=bb0->bb13.bb44.bb1062-bb0->bb13.bb44.bb1944
)>0 ){bb0->bb13.bb44.bb1711|=(bb0->bb13.bb44.bb1021>>bb1969);bb0->bb13
.bb44.bb1944=8 ; *bb0->bb13.bb44.bb1743++=bb0->bb13.bb44.bb1711;--bb0
->bb13.bb44.bb607;bb0->bb13.bb44.bb1711=0 ;bb0->bb13.bb44.bb1021&=((1
<<bb1969)-1 );bb0->bb13.bb44.bb1062=bb1969;bb1101(bb0);}bb54 bbm(
bb1969<0 ){bb0->bb13.bb44.bb1711|=(bb0->bb13.bb44.bb1021<<-bb1969);bb0
->bb13.bb44.bb1944-=bb0->bb13.bb44.bb1062;}bb54{bb0->bb13.bb44.bb1711
|=bb0->bb13.bb44.bb1021;bb0->bb13.bb44.bb1944=8 ; *bb0->bb13.bb44.
bb1743++=bb0->bb13.bb44.bb1711;--bb0->bb13.bb44.bb607;bb0->bb13.bb44.
bb1711=0 ;}bb2;}bb41 bbb bb2381(bb478 bb0){bb0->bb13.bb44.bb1021=(
0x180 );bb0->bb13.bb44.bb1062=(9 );bb1101(bb0);bbm(bb0->bb13.bb44.
bb1944!=8 ){ *bb0->bb13.bb44.bb1743++=bb0->bb13.bb44.bb1711;--bb0->
bb13.bb44.bb607;bbm(bb0->bb13.bb44.bb607==0 ){bb0->bb13.bb44.bb1999=1 ;
}}bb0->bb13.bb44.bb1944=8 ;bb0->bb13.bb44.bb1711=0 ;bb2;}bb41 bbb bb2077
(bb478 bb0,bbk bb2157,bbd bb2099){bbm(bb0->bb13.bb44.bb1821==0 ){bbm(
bb2157<128 ){bb0->bb13.bb44.bb1021=(0x180 |bb2157);bb0->bb13.bb44.
bb1062=(9 );bb1101(bb0);}bb54{bb0->bb13.bb44.bb1021=(0x1000 |bb2157);
bb0->bb13.bb44.bb1062=(13 );bb1101(bb0);}}bbm(bb2099>=23 ){bb0->bb13.
bb44.bb1021=(((1 <<4 )-1 ));bb0->bb13.bb44.bb1062=(4 );bb1101(bb0);bb2099
-=((1 <<4 )-1 );bb0->bb13.bb44.bb1840+=((1 <<4 )-1 );bb0->bb13.bb44.bb1821=
1 ;}bb54{bb0->bb13.bb44.bb1021=(bb2156[(bbk)bb2099].bb2317);bb0->bb13.
bb44.bb1062=(bb2156[(bbk)bb2099].bb47);bb1101(bb0);bb0->bb13.bb44.
bb1821=0 ;bb0->bb13.bb44.bb536=0 ;bb0->bb13.bb44.bb1840=0 ;bb0->bb1750=
bb2387;}bb2;}bb41 bbb bb2194(bb478 bb0,bbk bb374){bbm(bb0->bb13.bb44.
bb2114==1 ){bbm(bb0->bb13.bb44.bb536==0 ){bb0->bb13.bb44.bb1021=((bb0->
bb13.bb44.bb1912));bb0->bb13.bb44.bb1062=(9 );bb1101(bb0);bb0->bb1750=
bb2230;}bb54 bb2077(bb0,bb0->bb13.bb44.bb1337,bb0->bb13.bb44.bb536);}
bb2381(bb0);bbm((bb374&0x04 )==0 ){bbm(0 -bb0->bb13.bb44.bb1279>=2048 )bb2128
(bb0->bb453->bb2083,0xC0000000L );bb54 bbm(0x80000000L -bb0->bb13.bb44.
bb1279>=2048 )bb2128(bb0->bb453->bb2083,0x40000000L );bb0->bb13.bb44.
bb1279+=2048 ;bb0->bb13.bb44.bb1746+=2048 ;bb2224(bb0);}bb0->bb13.bb44.
bb2114=0 ;bb2;}bbk bb2173(bb478 bb0,bbf* *bb1739,bbf* *bb1733,bbd*
bb916,bbd*bb597,bbb*bb1314,bbk bb374,bbk bb2158){bb925 bbk bb2183;
bb925 bbk bb2040;bbk bb2169;bbk bb1080=0 ;bb0->bb453=(bb2046* )bb1314;
bb0->bb13.bb44=bb0->bb453->bb44;bb0->bb13.bb44.bb1753= *bb1739;bb0->
bb13.bb44.bb988= *bb916;bb0->bb13.bb44.bb1743= *bb1733;bb0->bb13.bb44
.bb607= *bb597;bb0->bb13.bb44.bb1999=0 ;bb0->bb2112=0X0018 &bb374;bbm(
bb0->bb2112>0x0010 ){bb0->bb2112=0x0010 ;}bb0->bb2112>>=3 ;bbm( *bb597<=
15 )bb1080=0 ;bb54 bbm(bb0->bb13.bb44.bb988!=0 ){bb0->bb13.bb44.bb607-=
15 ;bbm(bb0->bb13.bb44.bb2114==0 ){bb0->bb13.bb44.bb1912= *bb0->bb13.
bb44.bb1753++;--bb0->bb13.bb44.bb988;++bb0->bb13.bb44.bb1279;++bb0->
bb13.bb44.bb1746;bb0->bb13.bb44.bb2048=(bbk)bb0->bb13.bb44.bb1746&(
2048 -1 );bb0->bb453->bb1843[(bbk)bb0->bb13.bb44.bb1279&(2048 -1 )]=bb0->
bb13.bb44.bb1912;bb0->bb13.bb44.bb1690=(bb0->bb13.bb44.bb1690<<8 )+bb0
->bb13.bb44.bb1912;bb0->bb13.bb44.bb2114=1 ;}bb106((bb0->bb13.bb44.
bb988!=0 )&&(bb0->bb13.bb44.bb1999==0 )){++bb0->bb13.bb44.bb1279;++bb0
->bb13.bb44.bb1746;bb0->bb13.bb44.bb2048=(bbk)bb0->bb13.bb44.bb1746&(
2048 -1 );bbm(((bb0->bb13.bb44.bb1279&0x7FFFFFFFL )==0 ))bb2374(bb0);bb0
->bb453->bb1843[(bbk)bb0->bb13.bb44.bb1279&(2048 -1 )]=bb0->bb13.bb44.
bb1912= *bb0->bb13.bb44.bb1753++;bb0->bb13.bb44.bb1690=(bb0->bb13.
bb44.bb1690<<8 )+bb0->bb13.bb44.bb1912;--bb0->bb13.bb44.bb988;bb0->
bb13.bb44.bb2272=bb0->bb453->bb2083+((((bb0->bb13.bb44.bb1690)&0xFF00
)>>4 )^((bb0->bb13.bb44.bb1690)&0x00FF ));bbm((bb0->bb13.bb44.bb2290=
bb0->bb13.bb44.bb1746- *bb0->bb13.bb44.bb2272)>2048 -2 ){bb0->bb453->
bb1872[bb0->bb13.bb44.bb2048]=0 ;bbm(bb0->bb13.bb44.bb536!=0 ){bb2077(
bb0,bb0->bb13.bb44.bb1337,bb0->bb13.bb44.bb536);}bb54{bb0->bb13.bb44.
bb1021=((bb0->bb13.bb44.bb1690>>8 ));bb0->bb13.bb44.bb1062=(9 );bb1101(
bb0);bb0->bb1750=bb2230;}}bb54{bb0->bb453->bb1872[bb0->bb13.bb44.
bb2048]=(bbk)bb0->bb13.bb44.bb2290;bbm(bb0->bb13.bb44.bb536!=0 ){bbm((
bb0->bb453->bb1843[(bbk)(((bbd)bb0->bb13.bb44.bb1254+bb0->bb13.bb44.
bb536+bb0->bb13.bb44.bb1840)&(bbd)(2048 -1 ))]==bb0->bb13.bb44.bb1912)&&
((bb0->bb13.bb44.bb536+bb0->bb13.bb44.bb1840)<(bbd)0xFFFFFFFFL )){++
bb0->bb13.bb44.bb536;bb0->bb1750=bb2488;bbm(bb0->bb13.bb44.bb1821){
bbm(bb0->bb13.bb44.bb536>=23 ){bb0->bb13.bb44.bb1021=(((1 <<4 )-1 ));bb0
->bb13.bb44.bb1062=(4 );bb1101(bb0);bb0->bb13.bb44.bb536-=((1 <<4 )-1 );
bb0->bb13.bb44.bb1840+=((1 <<4 )-1 );}}bb54 bbm(bb0->bb13.bb44.bb536>=23
){bbm(bb0->bb13.bb44.bb1337<128 ){bb0->bb13.bb44.bb1021=(0x180 |bb0->
bb13.bb44.bb1337);bb0->bb13.bb44.bb1062=(9 );bb1101(bb0);}bb54{bb0->
bb13.bb44.bb1021=(0x1000 |bb0->bb13.bb44.bb1337);bb0->bb13.bb44.bb1062
=(13 );bb1101(bb0);}bb0->bb13.bb44.bb1021=(((1 <<4 )-1 ));bb0->bb13.bb44.
bb1062=(4 );bb1101(bb0);bb0->bb13.bb44.bb536-=((1 <<4 )-1 );bb0->bb13.
bb44.bb1840+=((1 <<4 )-1 );bb0->bb13.bb44.bb1821=1 ;}}bb54 bbm(bb0->bb13.
bb44.bb1821){bb0->bb13.bb44.bb1021=(bb2156[(bbk)bb0->bb13.bb44.bb536]
.bb2317);bb0->bb13.bb44.bb1062=(bb2156[(bbk)bb0->bb13.bb44.bb536].
bb47);bb1101(bb0);bb0->bb13.bb44.bb536=0 ;bb0->bb13.bb44.bb1840=0 ;bb0
->bb13.bb44.bb1821=0 ;bb0->bb1750=bb2387;}bb54 bbm(bb0->bb13.bb44.
bb536>=8 ){bb2077(bb0,bb0->bb13.bb44.bb1337,bb0->bb13.bb44.bb536);}
bb54{bb2040=0 ;bb0->bb13.bb44.bb2098=bb0->bb13.bb44.bb1337;bb106((bb0
->bb453->bb1872[bb0->bb13.bb44.bb1254]!=0 )&&(bb2040==0 )&&(bb0->bb13.
bb44.bb2059<bb2158)&&(bb0->bb13.bb44.bb2098<(bbk)(2048 -bb0->bb13.bb44
.bb536))){bb0->bb13.bb44.bb2098+=bb0->bb453->bb1872[bb0->bb13.bb44.
bb1254];++bb0->bb13.bb44.bb2059;bbm(bb0->bb13.bb44.bb2098<(bbk)(2048 -
bb0->bb13.bb44.bb536)){bb0->bb13.bb44.bb1254=bb0->bb13.bb44.bb1254-
bb0->bb453->bb1872[bb0->bb13.bb44.bb1254]&(2048 -1 );bbm(bb0->bb453->
bb1843[bb0->bb13.bb44.bb1254]==bb0->bb453->bb1843[bb0->bb13.bb44.
bb2248]){bb2040=1 ;bb90(bb2183=2 ,bb2169=(bb0->bb13.bb44.bb1254+2 )&(
2048 -1 );bb2183<=(bbk)bb0->bb13.bb44.bb536;bb2183++,bb2169=(bb2169+1 )&
(2048 -1 )){bbm(bb0->bb453->bb1843[bb2169]!=bb0->bb453->bb1843[(bb0->
bb13.bb44.bb2248+bb2183)&(2048 -1 )]){bb2040=0 ;bb21;}}}}}bbm(bb2040){
bb0->bb13.bb44.bb1337=bb0->bb13.bb44.bb2098;++bb0->bb13.bb44.bb536;
bb0->bb1750=bb2473;}bb54{bb2077(bb0,bb0->bb13.bb44.bb1337,bb0->bb13.
bb44.bb536);}}}bb54{bb0->bb13.bb44.bb2050=(bbk)bb0->bb13.bb44.bb2290;
bb0->bb13.bb44.bb2059=0 ;bb573{bb0->bb13.bb44.bb1254=(bbk)(bb0->bb13.
bb44.bb1746-bb0->bb13.bb44.bb2050&(2048 -1 ));bbm(bb0->bb453->bb1843[
bb0->bb13.bb44.bb1254]==(bbf)(bb0->bb13.bb44.bb1690>>8 )){bb0->bb13.
bb44.bb536=2 ;bb0->bb13.bb44.bb2248=bb0->bb13.bb44.bb2048;bb0->bb13.
bb44.bb1337=bb0->bb13.bb44.bb2050;bb0->bb1750=bb2563;bb21;}bb54{bb0->
bb13.bb44.bb2050+=bb0->bb453->bb1872[bb0->bb13.bb44.bb1254];++bb0->
bb13.bb44.bb2059;}}bb106((bb0->bb453->bb1872[bb0->bb13.bb44.bb1254]!=
0 )&&(bb0->bb13.bb44.bb2059<bb2158)&&(bb0->bb13.bb44.bb2050<2048 -2 ));
bbm(bb0->bb13.bb44.bb536==0 ){bb0->bb13.bb44.bb1021=((bb0->bb13.bb44.
bb1690>>8 ));bb0->bb13.bb44.bb1062=(9 );bb1101(bb0);bb0->bb1750=bb2230;
}}}bbm(bb2481[bb0->bb1750][bb0->bb2112]){ *bb0->bb13.bb44.bb2272=bb0
->bb13.bb44.bb1746;}}bbm(bb0->bb13.bb44.bb988==0 ){bb1080=1 ;bbm(bb374&
0x01 ){bb2194(bb0,bb374);bb1080|=4 ;}}bbm((bb0->bb13.bb44.bb607==0 )||(
bb0->bb13.bb44.bb1999)){bbm(!(bb1080&1 )){bb1080=2 ;bbm(bb374&0x02 ){
bb2194(bb0,bb374);bb1080|=4 ;}}bb54{bb1080|=3 ;bbm((!(bb374&0x01 ))&&(
bb374&0x02 )){bb2194(bb0,bb374);bb1080|=4 ;}}}bb0->bb13.bb44.bb607+=15 ;
}bb54{bb1080=1 ;bbm(bb374&0x01 ){bb2194(bb0,bb374);bb1080|=4 ;}}bb0->
bb453->bb44=bb0->bb13.bb44; *bb1739=bb0->bb13.bb44.bb1753; *bb916=bb0
->bb13.bb44.bb988; *bb1733=bb0->bb13.bb44.bb1743; *bb597=bb0->bb13.
bb44.bb607;bb2(bb1080);}bb41 bbb bb2122(bb478 bb0){bb0->bb13.bb80.
bb2090&=(2048 -1 );bb0->bb13.bb80.bb1051=bb0->bb13.bb80.bb1153=bb0->
bb13.bb80.bb92=0 ;bb0->bb13.bb80.bb998=bb0->bb13.bb80.bb1758=bb0->bb13
.bb80.bb1173=0 ;bb0->bb13.bb80.bb470=0 ;bb0->bb13.bb80.bb1971=0 ;bb0->
bb13.bb80.bb1165=bb2082;bb0->bb13.bb80.bb2440=1 ;bb2;}bb41 bbk bb2283(
bb478 bb0){bbm((bb0->bb13.bb80.bb988==0 )&&(bb0->bb13.bb80.bb1051==0 ))bb0
->bb13.bb80.bb998=bb2376;bb54{bb0->bb13.bb80.bb998=bb1249;bbm(bb0->
bb13.bb80.bb1051==0 ){bb0->bb13.bb80.bb1758= *bb0->bb13.bb80.bb1753++;
--bb0->bb13.bb80.bb988;bb0->bb13.bb80.bb1051=7 ;bb0->bb13.bb80.bb1173=
(bb0->bb13.bb80.bb1758>127 )?1 :0 ;bb0->bb13.bb80.bb1758&=((bbf)0x7F );}
bb54{bb0->bb13.bb80.bb1173=(bb0->bb13.bb80.bb1758>>(bb0->bb13.bb80.
bb1051-1 ));--bb0->bb13.bb80.bb1051;bb0->bb13.bb80.bb1758&=((bbf)0xFF
>>(8 -bb0->bb13.bb80.bb1051));}}bb2(bb0->bb13.bb80.bb998);}bb41 bbk
bb1847(bb478 bb0){bbk bb2155;bb124 bb8;bbm(bb0->bb13.bb80.bb998==
bb1249)bb0->bb13.bb80.bb1173=0 ;bb54 bb0->bb13.bb80.bb998=bb1249;bb106
((bb0->bb13.bb80.bb1153>0 )&&(bb0->bb13.bb80.bb998==bb1249)){bbm((bb0
->bb13.bb80.bb988==0 )&&(bb0->bb13.bb80.bb1051==0 ))bb0->bb13.bb80.
bb998=bb2376;bb54{bbm(bb0->bb13.bb80.bb1051==0 ){bb0->bb13.bb80.bb1758
= *bb0->bb13.bb80.bb1753++;--bb0->bb13.bb80.bb988;bb0->bb13.bb80.
bb1051=8 ;}bb2155=bb0->bb13.bb80.bb1758;bbm((bb8=bb0->bb13.bb80.bb1153
-bb0->bb13.bb80.bb1051)>0 )bb2155<<=bb8;bb54 bb2155>>=-bb8;bb0->bb13.
bb80.bb1173|=bb2155;bb8=((((8 )<(bb0->bb13.bb80.bb1153)?(8 ):(bb0->bb13
.bb80.bb1153)))<(bb0->bb13.bb80.bb1051)?(((8 )<(bb0->bb13.bb80.bb1153)?
(8 ):(bb0->bb13.bb80.bb1153))):(bb0->bb13.bb80.bb1051));bb0->bb13.bb80
.bb1153-=bb8;bb0->bb13.bb80.bb1051-=bb8;bb0->bb13.bb80.bb1758&=((bbf)0xFF
>>(8 -bb0->bb13.bb80.bb1051));}}bb2(bb0->bb13.bb80.bb998);}bb41 bbb
bb2072(bb478 bb0,bbf bbo){bbm(bb0->bb13.bb80.bb607!=0 ){ *bb0->bb13.
bb80.bb1743++=bbo;--bb0->bb13.bb80.bb607;bb0->bb453->bb2164[bb0->bb13
.bb80.bb2090++]=(bbf)bbo;bb0->bb13.bb80.bb2090&=(2048 -1 );}}bbk bb2293
(bb478 bb0,bbf* *bb1739,bbf* *bb1733,bbd*bb916,bbd*bb597,bbb*bb1314,
bbk bb374){bbk bb2198=0 ;bbk bb1080=0 ;bb0->bb453=(bb2046* )bb1314;bb0
->bb13.bb80=bb0->bb453->bb80;bb0->bb13.bb80.bb1753= *bb1739;bb0->bb13
.bb80.bb988= *bb916;bb0->bb13.bb80.bb1743= *bb1733;bb0->bb13.bb80.
bb607= *bb597;bbm(bb374&0x01 ){bb2122(bb0);}bbm((bb0->bb13.bb80.bb988
!=0 )&&(bb0->bb13.bb80.bb607!=0 )){bb106((bb0->bb13.bb80.bb607!=0 )&&((
bb0->bb13.bb80.bb988!=0 )||(bb0->bb13.bb80.bb1051!=0 ))&&(bb2198==0 )){
bbm(bb0->bb13.bb80.bb1971){bb106((bb0->bb13.bb80.bb607!=0 )&&(bb0->
bb13.bb80.bb470>0 )){bb0->bb13.bb80.bb92&=(2048 -1 );bb2072(bb0,bb0->
bb453->bb2164[bb0->bb13.bb80.bb92++]);--bb0->bb13.bb80.bb470;}bbm(bb0
->bb13.bb80.bb470==0 )bb0->bb13.bb80.bb1971=0 ;bb0->bb13.bb80.bb1165=
bb2082;}bb54{bb300(bb0->bb13.bb80.bb1165){bb15 bb2082:bb2283(bb0);bbm
(bb0->bb13.bb80.bb1173==0 ){bb0->bb13.bb80.bb1153=8 ;bb0->bb13.bb80.
bb1165=bb2347;bb15 bb2347:bb1847(bb0);bbm(bb0->bb13.bb80.bb998==
bb1249){bb2072(bb0,(bbf)bb0->bb13.bb80.bb1173);bb0->bb13.bb80.bb1165=
bb2082;}}bb54{bb0->bb13.bb80.bb1165=bb2382;bb15 bb2382:bb2283(bb0);
bbm(bb0->bb13.bb80.bb998==bb1249){bb0->bb13.bb80.bb1153=bb0->bb13.
bb80.bb1173?7 :11 ;bb0->bb13.bb80.bb1165=bb2350;bb15 bb2350:bb1847(bb0);
bbm(bb0->bb13.bb80.bb998==bb1249){bb0->bb13.bb80.bb92=bb0->bb13.bb80.
bb1173;bbm(bb0->bb13.bb80.bb92==0 )bb2198=1 ;bb54{bb0->bb13.bb80.bb92=
bb0->bb13.bb80.bb2090-bb0->bb13.bb80.bb92;bb0->bb13.bb80.bb1153=2 ;bb0
->bb13.bb80.bb1165=bb2308;bb15 bb2308:bb1847(bb0);bbm(bb0->bb13.bb80.
bb998==bb1249){bb0->bb13.bb80.bb470=2 +bb0->bb13.bb80.bb1173;bbm(bb0->
bb13.bb80.bb470==5 ){bb0->bb13.bb80.bb1153=2 ;bb0->bb13.bb80.bb1165=
bb2309;bb15 bb2309:bb1847(bb0);bbm(bb0->bb13.bb80.bb998==bb1249){bb0
->bb13.bb80.bb470+=bb0->bb13.bb80.bb1173;bbm(bb0->bb13.bb80.bb470==8 ){
bb0->bb13.bb80.bb1153=4 ;bb0->bb13.bb80.bb1165=bb2423;bb15 bb2423:
bb1847(bb0);bbm(bb0->bb13.bb80.bb998==bb1249){bb0->bb13.bb80.bb470+=
bb0->bb13.bb80.bb1173;bbm(bb0->bb13.bb80.bb470==23 ){bb573{bb15 bb2237
:bb106((bb0->bb13.bb80.bb607!=0 )&&(bb0->bb13.bb80.bb470>0 )){bb0->bb13
.bb80.bb92&=(2048 -1 );bb2072(bb0,bb0->bb453->bb2164[bb0->bb13.bb80.
bb92++]);--bb0->bb13.bb80.bb470;}bbm(bb0->bb13.bb80.bb607==0 ){bb0->
bb13.bb80.bb1165=bb2237;bb21;}bb54{bb0->bb13.bb80.bb1153=4 ;bb0->bb13.
bb80.bb1165=bb2366;bb15 bb2366:bb1847(bb0);bbm(bb0->bb13.bb80.bb998==
bb1249)bb0->bb13.bb80.bb470+=bb0->bb13.bb80.bb1173;bb54 bb21;}}bb106(
bb0->bb13.bb80.bb1173==((1 <<4 )-1 ));}}}}}}bbm((bb0->bb13.bb80.bb998==
bb1249)&&(bb0->bb13.bb80.bb1165!=bb2237)){bb0->bb13.bb80.bb1971=1 ;}}}
}}}}}bbm(bb0->bb13.bb80.bb1971){bb106((bb0->bb13.bb80.bb607!=0 )&&(bb0
->bb13.bb80.bb470>0 )){bb0->bb13.bb80.bb92&=(2048 -1 );bb2072(bb0,bb0->
bb453->bb2164[bb0->bb13.bb80.bb92++]);--bb0->bb13.bb80.bb470;}bbm(bb0
->bb13.bb80.bb470==0 )bb0->bb13.bb80.bb1971=0 ;bb0->bb13.bb80.bb1165=
bb2082;}}bbm(bb2198){bb2122(bb0);bb1080|=4 ;}bbm(bb0->bb13.bb80.bb988
==0 ){bb1080|=1 ;}bbm(bb0->bb13.bb80.bb607==0 ){bb1080|=2 ;}bb0->bb453->
bb80=bb0->bb13.bb80; *bb1739=bb0->bb13.bb80.bb1753; *bb916=bb0->bb13.
bb80.bb988; *bb1733=bb0->bb13.bb80.bb1743; *bb597=bb0->bb13.bb80.
bb607;bb2(bb1080);}
