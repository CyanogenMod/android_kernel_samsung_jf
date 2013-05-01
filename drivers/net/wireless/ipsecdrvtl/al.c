/*
   'md5.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Mon Oct 29 11:54:27 2012
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
bb41 bbb bb1255(bbd bb23[4 ],bbh bbf bb97[64 ]){bb41 bbd bb3[64 ]={
0xd76aa478 ,0xe8c7b756 ,0x242070db ,0xc1bdceee ,0xf57c0faf ,0x4787c62a ,
0xa8304613 ,0xfd469501 ,0x698098d8 ,0x8b44f7af ,0xffff5bb1 ,0x895cd7be ,
0x6b901122 ,0xfd987193 ,0xa679438e ,0x49b40821 ,0xf61e2562 ,0xc040b340 ,
0x265e5a51 ,0xe9b6c7aa ,0xd62f105d ,0x02441453 ,0xd8a1e681 ,0xe7d3fbc8 ,
0x21e1cde6 ,0xc33707d6 ,0xf4d50d87 ,0x455a14ed ,0xa9e3e905 ,0xfcefa3f8 ,
0x676f02d9 ,0x8d2a4c8a ,0xfffa3942 ,0x8771f681 ,0x6d9d6122 ,0xfde5380c ,
0xa4beea44 ,0x4bdecfa9 ,0xf6bb4b60 ,0xbebfbc70 ,0x289b7ec6 ,0xeaa127fa ,
0xd4ef3085 ,0x04881d05 ,0xd9d4d039 ,0xe6db99e5 ,0x1fa27cf8 ,0xc4ac5665 ,
0xf4292244 ,0x432aff97 ,0xab9423a7 ,0xfc93a039 ,0x655b59c3 ,0x8f0ccc92 ,
0xffeff47d ,0x85845dd1 ,0x6fa87e4f ,0xfe2ce6e0 ,0xa3014314 ,0x4e0811a1 ,
0xf7537e82 ,0xbd3af235 ,0x2ad7d2bb ,0xeb86d391 ,};bb32(bb12(bbe)>=4 );{bbd
bbc=bb23[0 ],bbn=bb23[1 ],bbo=bb23[2 ],bbt=bb23[3 ],bb8;bbd bbv[16 ],bbz;
bb90(bbz=0 ;bbz<16 ;bbz++,bb97+=4 )bbv[bbz]=(bb97[0 ]|bb97[1 ]<<8 |bb97[2 ]
<<16 |bb97[3 ]<<24 );bb8=bbc+(bbn&bbo|~bbn&bbt)+bb3[0 ]+bbv[(0 )];bbc=bbn+
((bb8)<<(7 )|(bb8)>>(32 -7 ));bb8=bbt+(bbc&bbn|~bbc&bbo)+bb3[0 +1 ]+bbv[(0
+1 )];bbt=bbc+((bb8)<<(12 )|(bb8)>>(32 -12 ));bb8=bbo+(bbt&bbc|~bbt&bbn)+
bb3[0 +2 ]+bbv[(0 +2 )];bbo=bbt+((bb8)<<(17 )|(bb8)>>(32 -17 ));bb8=bbn+(bbo
&bbt|~bbo&bbc)+bb3[0 +3 ]+bbv[(0 +3 )];bbn=bbo+((bb8)<<(22 )|(bb8)>>(32 -22
));bb8=bbc+(bbn&bbo|~bbn&bbt)+bb3[0 +4 ]+bbv[(0 +4 )];bbc=bbn+((bb8)<<(7 )|
(bb8)>>(32 -7 ));bb8=bbt+(bbc&bbn|~bbc&bbo)+bb3[0 +4 +1 ]+bbv[(0 +4 +1 )];bbt
=bbc+((bb8)<<(12 )|(bb8)>>(32 -12 ));bb8=bbo+(bbt&bbc|~bbt&bbn)+bb3[0 +4 +
2 ]+bbv[(0 +4 +2 )];bbo=bbt+((bb8)<<(17 )|(bb8)>>(32 -17 ));bb8=bbn+(bbo&bbt
|~bbo&bbc)+bb3[0 +4 +3 ]+bbv[(0 +4 +3 )];bbn=bbo+((bb8)<<(22 )|(bb8)>>(32 -22
));bb8=bbc+(bbn&bbo|~bbn&bbt)+bb3[0 +8 ]+bbv[(0 +8 )];bbc=bbn+((bb8)<<(7 )|
(bb8)>>(32 -7 ));bb8=bbt+(bbc&bbn|~bbc&bbo)+bb3[0 +8 +1 ]+bbv[(0 +8 +1 )];bbt
=bbc+((bb8)<<(12 )|(bb8)>>(32 -12 ));bb8=bbo+(bbt&bbc|~bbt&bbn)+bb3[0 +8 +
2 ]+bbv[(0 +8 +2 )];bbo=bbt+((bb8)<<(17 )|(bb8)>>(32 -17 ));bb8=bbn+(bbo&bbt
|~bbo&bbc)+bb3[0 +8 +3 ]+bbv[(0 +8 +3 )];bbn=bbo+((bb8)<<(22 )|(bb8)>>(32 -22
));bb8=bbc+(bbn&bbo|~bbn&bbt)+bb3[0 +12 ]+bbv[(0 +12 )];bbc=bbn+((bb8)<<(
7 )|(bb8)>>(32 -7 ));bb8=bbt+(bbc&bbn|~bbc&bbo)+bb3[0 +12 +1 ]+bbv[(0 +12 +1 )]
;bbt=bbc+((bb8)<<(12 )|(bb8)>>(32 -12 ));bb8=bbo+(bbt&bbc|~bbt&bbn)+bb3[
0 +12 +2 ]+bbv[(0 +12 +2 )];bbo=bbt+((bb8)<<(17 )|(bb8)>>(32 -17 ));bb8=bbn+(
bbo&bbt|~bbo&bbc)+bb3[0 +12 +3 ]+bbv[(0 +12 +3 )];bbn=bbo+((bb8)<<(22 )|(bb8
)>>(32 -22 ));bb8=bbc+(bbt&bbn|~bbt&bbo)+bb3[16 ]+bbv[(5 * (16 )+1 )%16 ];
bbc=bbn+((bb8)<<(5 )|(bb8)>>(32 -5 ));bb8=bbt+(bbo&bbc|~bbo&bbn)+bb3[16 +
1 ]+bbv[(5 * (16 +1 )+1 )%16 ];bbt=bbc+((bb8)<<(9 )|(bb8)>>(32 -9 ));bb8=bbo+(
bbn&bbt|~bbn&bbc)+bb3[16 +2 ]+bbv[(5 * (16 +2 )+1 )%16 ];bbo=bbt+((bb8)<<(14
)|(bb8)>>(32 -14 ));bb8=bbn+(bbc&bbo|~bbc&bbt)+bb3[16 +3 ]+bbv[(5 * (16 +3 )+
1 )%16 ];bbn=bbo+((bb8)<<(20 )|(bb8)>>(32 -20 ));bb8=bbc+(bbt&bbn|~bbt&bbo
)+bb3[16 +4 ]+bbv[(5 * (16 +4 )+1 )%16 ];bbc=bbn+((bb8)<<(5 )|(bb8)>>(32 -5 ));
bb8=bbt+(bbo&bbc|~bbo&bbn)+bb3[16 +4 +1 ]+bbv[(5 * (16 +4 +1 )+1 )%16 ];bbt=
bbc+((bb8)<<(9 )|(bb8)>>(32 -9 ));bb8=bbo+(bbn&bbt|~bbn&bbc)+bb3[16 +4 +2 ]
+bbv[(5 * (16 +4 +2 )+1 )%16 ];bbo=bbt+((bb8)<<(14 )|(bb8)>>(32 -14 ));bb8=bbn
+(bbc&bbo|~bbc&bbt)+bb3[16 +4 +3 ]+bbv[(5 * (16 +4 +3 )+1 )%16 ];bbn=bbo+((bb8
)<<(20 )|(bb8)>>(32 -20 ));bb8=bbc+(bbt&bbn|~bbt&bbo)+bb3[16 +8 ]+bbv[(5 * (
16 +8 )+1 )%16 ];bbc=bbn+((bb8)<<(5 )|(bb8)>>(32 -5 ));bb8=bbt+(bbo&bbc|~bbo
&bbn)+bb3[16 +8 +1 ]+bbv[(5 * (16 +8 +1 )+1 )%16 ];bbt=bbc+((bb8)<<(9 )|(bb8)>>
(32 -9 ));bb8=bbo+(bbn&bbt|~bbn&bbc)+bb3[16 +8 +2 ]+bbv[(5 * (16 +8 +2 )+1 )%16
];bbo=bbt+((bb8)<<(14 )|(bb8)>>(32 -14 ));bb8=bbn+(bbc&bbo|~bbc&bbt)+bb3
[16 +8 +3 ]+bbv[(5 * (16 +8 +3 )+1 )%16 ];bbn=bbo+((bb8)<<(20 )|(bb8)>>(32 -20 ));
bb8=bbc+(bbt&bbn|~bbt&bbo)+bb3[16 +12 ]+bbv[(5 * (16 +12 )+1 )%16 ];bbc=bbn+
((bb8)<<(5 )|(bb8)>>(32 -5 ));bb8=bbt+(bbo&bbc|~bbo&bbn)+bb3[16 +12 +1 ]+
bbv[(5 * (16 +12 +1 )+1 )%16 ];bbt=bbc+((bb8)<<(9 )|(bb8)>>(32 -9 ));bb8=bbo+(
bbn&bbt|~bbn&bbc)+bb3[16 +12 +2 ]+bbv[(5 * (16 +12 +2 )+1 )%16 ];bbo=bbt+((bb8
)<<(14 )|(bb8)>>(32 -14 ));bb8=bbn+(bbc&bbo|~bbc&bbt)+bb3[16 +12 +3 ]+bbv[(
5 * (16 +12 +3 )+1 )%16 ];bbn=bbo+((bb8)<<(20 )|(bb8)>>(32 -20 ));bb8=bbc+(bbn
^bbo^bbt)+bb3[32 ]+bbv[(3 * (32 )+5 )%16 ];bbc=bbn+((bb8)<<(4 )|(bb8)>>(32 -
4 ));bb8=bbt+(bbc^bbn^bbo)+bb3[32 +1 ]+bbv[(3 * (32 +1 )+5 )%16 ];bbt=bbc+((
bb8)<<(11 )|(bb8)>>(32 -11 ));bb8=bbo+(bbt^bbc^bbn)+bb3[32 +2 ]+bbv[(3 * (
32 +2 )+5 )%16 ];bbo=bbt+((bb8)<<(16 )|(bb8)>>(32 -16 ));bb8=bbn+(bbo^bbt^
bbc)+bb3[32 +3 ]+bbv[(3 * (32 +3 )+5 )%16 ];bbn=bbo+((bb8)<<(23 )|(bb8)>>(32 -
23 ));bb8=bbc+(bbn^bbo^bbt)+bb3[32 +4 ]+bbv[(3 * (32 +4 )+5 )%16 ];bbc=bbn+((
bb8)<<(4 )|(bb8)>>(32 -4 ));bb8=bbt+(bbc^bbn^bbo)+bb3[32 +4 +1 ]+bbv[(3 * (
32 +4 +1 )+5 )%16 ];bbt=bbc+((bb8)<<(11 )|(bb8)>>(32 -11 ));bb8=bbo+(bbt^bbc^
bbn)+bb3[32 +4 +2 ]+bbv[(3 * (32 +4 +2 )+5 )%16 ];bbo=bbt+((bb8)<<(16 )|(bb8)>>
(32 -16 ));bb8=bbn+(bbo^bbt^bbc)+bb3[32 +4 +3 ]+bbv[(3 * (32 +4 +3 )+5 )%16 ];
bbn=bbo+((bb8)<<(23 )|(bb8)>>(32 -23 ));bb8=bbc+(bbn^bbo^bbt)+bb3[32 +8 ]+
bbv[(3 * (32 +8 )+5 )%16 ];bbc=bbn+((bb8)<<(4 )|(bb8)>>(32 -4 ));bb8=bbt+(bbc
^bbn^bbo)+bb3[32 +8 +1 ]+bbv[(3 * (32 +8 +1 )+5 )%16 ];bbt=bbc+((bb8)<<(11 )|(
bb8)>>(32 -11 ));bb8=bbo+(bbt^bbc^bbn)+bb3[32 +8 +2 ]+bbv[(3 * (32 +8 +2 )+5 )%
16 ];bbo=bbt+((bb8)<<(16 )|(bb8)>>(32 -16 ));bb8=bbn+(bbo^bbt^bbc)+bb3[32
+8 +3 ]+bbv[(3 * (32 +8 +3 )+5 )%16 ];bbn=bbo+((bb8)<<(23 )|(bb8)>>(32 -23 ));
bb8=bbc+(bbn^bbo^bbt)+bb3[32 +12 ]+bbv[(3 * (32 +12 )+5 )%16 ];bbc=bbn+((bb8
)<<(4 )|(bb8)>>(32 -4 ));bb8=bbt+(bbc^bbn^bbo)+bb3[32 +12 +1 ]+bbv[(3 * (32 +
12 +1 )+5 )%16 ];bbt=bbc+((bb8)<<(11 )|(bb8)>>(32 -11 ));bb8=bbo+(bbt^bbc^
bbn)+bb3[32 +12 +2 ]+bbv[(3 * (32 +12 +2 )+5 )%16 ];bbo=bbt+((bb8)<<(16 )|(bb8)>>
(32 -16 ));bb8=bbn+(bbo^bbt^bbc)+bb3[32 +12 +3 ]+bbv[(3 * (32 +12 +3 )+5 )%16 ];
bbn=bbo+((bb8)<<(23 )|(bb8)>>(32 -23 ));bb8=bbc+(bbo^(bbn|~bbt))+bb3[48 ]
+bbv[(7 * (48 ))%16 ];bbc=bbn+((bb8)<<(6 )|(bb8)>>(32 -6 ));bb8=bbt+(bbn^(
bbc|~bbo))+bb3[48 +1 ]+bbv[(7 * (48 +1 ))%16 ];bbt=bbc+((bb8)<<(10 )|(bb8)>>
(32 -10 ));bb8=bbo+(bbc^(bbt|~bbn))+bb3[48 +2 ]+bbv[(7 * (48 +2 ))%16 ];bbo=
bbt+((bb8)<<(15 )|(bb8)>>(32 -15 ));bb8=bbn+(bbt^(bbo|~bbc))+bb3[48 +3 ]+
bbv[(7 * (48 +3 ))%16 ];bbn=bbo+((bb8)<<(21 )|(bb8)>>(32 -21 ));bb8=bbc+(bbo
^(bbn|~bbt))+bb3[48 +4 ]+bbv[(7 * (48 +4 ))%16 ];bbc=bbn+((bb8)<<(6 )|(bb8)>>
(32 -6 ));bb8=bbt+(bbn^(bbc|~bbo))+bb3[48 +4 +1 ]+bbv[(7 * (48 +4 +1 ))%16 ];
bbt=bbc+((bb8)<<(10 )|(bb8)>>(32 -10 ));bb8=bbo+(bbc^(bbt|~bbn))+bb3[48 +
4 +2 ]+bbv[(7 * (48 +4 +2 ))%16 ];bbo=bbt+((bb8)<<(15 )|(bb8)>>(32 -15 ));bb8=
bbn+(bbt^(bbo|~bbc))+bb3[48 +4 +3 ]+bbv[(7 * (48 +4 +3 ))%16 ];bbn=bbo+((bb8)<<
(21 )|(bb8)>>(32 -21 ));bb8=bbc+(bbo^(bbn|~bbt))+bb3[48 +8 ]+bbv[(7 * (48 +8
))%16 ];bbc=bbn+((bb8)<<(6 )|(bb8)>>(32 -6 ));bb8=bbt+(bbn^(bbc|~bbo))+
bb3[48 +8 +1 ]+bbv[(7 * (48 +8 +1 ))%16 ];bbt=bbc+((bb8)<<(10 )|(bb8)>>(32 -10 ));
bb8=bbo+(bbc^(bbt|~bbn))+bb3[48 +8 +2 ]+bbv[(7 * (48 +8 +2 ))%16 ];bbo=bbt+((
bb8)<<(15 )|(bb8)>>(32 -15 ));bb8=bbn+(bbt^(bbo|~bbc))+bb3[48 +8 +3 ]+bbv[(
7 * (48 +8 +3 ))%16 ];bbn=bbo+((bb8)<<(21 )|(bb8)>>(32 -21 ));bb8=bbc+(bbo^(
bbn|~bbt))+bb3[48 +12 ]+bbv[(7 * (48 +12 ))%16 ];bbc=bbn+((bb8)<<(6 )|(bb8)>>
(32 -6 ));bb8=bbt+(bbn^(bbc|~bbo))+bb3[48 +12 +1 ]+bbv[(7 * (48 +12 +1 ))%16 ];
bbt=bbc+((bb8)<<(10 )|(bb8)>>(32 -10 ));bb8=bbo+(bbc^(bbt|~bbn))+bb3[48 +
12 +2 ]+bbv[(7 * (48 +12 +2 ))%16 ];bbo=bbt+((bb8)<<(15 )|(bb8)>>(32 -15 ));bb8
=bbn+(bbt^(bbo|~bbc))+bb3[48 +12 +3 ]+bbv[(7 * (48 +12 +3 ))%16 ];bbn=bbo+((
bb8)<<(21 )|(bb8)>>(32 -21 ));bb23[0 ]+=bbc;bb23[1 ]+=bbn;bb23[2 ]+=bbo;
bb23[3 ]+=bbt;}}bbb bb1817(bb525*bbj){bb41 bbd bb23[4 ]={0x67452301 ,
0xefcdab89 ,0x98badcfe ,0x10325476 };bbj->bb10=0 ;bb81(bbj->bb23,bb23,
bb12(bb23));}bbb bb1309(bb525*bbj,bbh bbb*bb496,bbq bb10){bbh bbf*bb5
=(bbh bbf* )bb496;bbq bb385=bbj->bb10%bb12(bbj->bb101);bbj->bb10+=
bb10;bbm(bb385){bbq bb11=bb12(bbj->bb101)-bb385;bb81(bbj->bb101+bb385
,bb5,((bb10)<(bb11)?(bb10):(bb11)));bbm(bb10<bb11)bb2;bb5+=bb11;bb10
-=bb11;bb1255(bbj->bb23,bbj->bb101);}bb90(;bb10>=bb12(bbj->bb101);
bb10-=bb12(bbj->bb101),bb5+=bb12(bbj->bb101))bb1255(bbj->bb23,bb5);
bb81(bbj->bb101,bb5,bb10);}bbb bb1820(bb525*bbj,bbb*bb14){bbd bb1027[
2 ]={(bbd)(bbj->bb10<<3 ),(bbd)(bbj->bb10>>29 )};bbf bb440[bb12(bb1027)]
;bbq bbz;bb90(bbz=0 ;bbz<bb12(bb440);bbz++)bb440[bbz]=bb1027[bbz/4 ]>>(
(bbz%4 ) *8 )&0xff ;{bbf bb1315[]={0x80 },bb1313[bb12(bbj->bb101)]={0 };
bbq bb385=bbj->bb10%bb12(bbj->bb101);bb1309(bbj,bb1315,1 );bb1309(bbj,
bb1313,(bb12(bbj->bb101) *2 -1 -bb12(bb440)-bb385)%bb12(bbj->bb101));}
bb1309(bbj,bb440,bb12(bb440));bb90(bbz=0 ;bbz<bb12(bbj->bb23);bbz++)((
bbf* )bb14)[bbz]=bbj->bb23[bbz/4 ]>>((bbz%4 ) *8 )&0xff ;}bbb bb1852(bbb*
bb14,bbh bbb*bb5,bbq bb10){bb525 bb95;bb1817(&bb95);bb1309(&bb95,bb5,
bb10);bb1820(&bb95,bb14);}bbb bb1963(bbb*bb14,bb82 bb5){bb1852(bb14,
bb5,(bbq)bb1305(bb5));}
