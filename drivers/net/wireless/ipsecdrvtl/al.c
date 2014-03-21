/*
   'md5.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Wed Dec 18 14:11:10 2013
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
bb41 bbb bb1254(bbd bb23[4 ],bbh bbf bb95[64 ]){bb41 bbd bb3[64 ]={
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
bb90(bbz=0 ;bbz<16 ;bbz++,bb95+=4 )bbv[bbz]=(bb95[0 ]|bb95[1 ]<<8 |bb95[2 ]
<<16 |bb95[3 ]<<24 );bb8=bbc+(bbn&bbo|~bbn&bbt)+bb3[0 ]+bbv[(0 )];bbc=bbn+
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
bb23[3 ]+=bbt;}}bbb bb1816(bb530*bbj){bb41 bbd bb23[4 ]={0x67452301 ,
0xefcdab89 ,0x98badcfe ,0x10325476 };bbj->bb9=0 ;bb81(bbj->bb23,bb23,bb12
(bb23));}bbb bb1361(bb530*bbj,bbh bbb*bb498,bbq bb9){bbh bbf*bb5=(bbh
bbf* )bb498;bbq bb383=bbj->bb9%bb12(bbj->bb102);bbj->bb9+=bb9;bbm(
bb383){bbq bb11=bb12(bbj->bb102)-bb383;bb81(bbj->bb102+bb383,bb5,((
bb9)<(bb11)?(bb9):(bb11)));bbm(bb9<bb11)bb2;bb5+=bb11;bb9-=bb11;
bb1254(bbj->bb23,bbj->bb102);}bb90(;bb9>=bb12(bbj->bb102);bb9-=bb12(
bbj->bb102),bb5+=bb12(bbj->bb102))bb1254(bbj->bb23,bb5);bb81(bbj->
bb102,bb5,bb9);}bbb bb1819(bb530*bbj,bbb*bb14){bbd bb1006[2 ]={(bbd)(
bbj->bb9<<3 ),(bbd)(bbj->bb9>>29 )};bbf bb436[bb12(bb1006)];bbq bbz;
bb90(bbz=0 ;bbz<bb12(bb436);bbz++)bb436[bbz]=bb1006[bbz/4 ]>>((bbz%4 ) *
8 )&0xff ;{bbf bb1312[]={0x80 },bb1311[bb12(bbj->bb102)]={0 };bbq bb383=
bbj->bb9%bb12(bbj->bb102);bb1361(bbj,bb1312,1 );bb1361(bbj,bb1311,(
bb12(bbj->bb102) *2 -1 -bb12(bb436)-bb383)%bb12(bbj->bb102));}bb1361(
bbj,bb436,bb12(bb436));bb90(bbz=0 ;bbz<bb12(bbj->bb23);bbz++)((bbf* )bb14
)[bbz]=bbj->bb23[bbz/4 ]>>((bbz%4 ) *8 )&0xff ;}bbb bb1852(bbb*bb14,bbh
bbb*bb5,bbq bb9){bb530 bb97;bb1816(&bb97);bb1361(&bb97,bb5,bb9);
bb1819(&bb97,bb14);}bbb bb1964(bbb*bb14,bb83 bb5){bb1852(bb14,bb5,(
bbq)bb1304(bb5));}
