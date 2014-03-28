/*
   'aes.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Wed Dec 18 14:11:10 2013
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
bba bbi{bbq bb447;bbd bb417[4 * (14 +1 )];}bb364;bbb bb1099(bb364*bbj,
bbh bbb*bb71,bbq bb142);bbb bb1736(bb364*bbj,bbh bbb*bb71,bbq bb142);
bbb bb1035(bb364*bbj,bbb*bb14,bbh bbb*bb5);bbb bb1775(bb364*bbj,bbb*
bb14,bbh bbb*bb5);
#ifdef __cplusplus
}
#endif
bb41 bbf bb407[256 ]={0x63 ,0x7c ,0x77 ,0x7b ,0xf2 ,0x6b ,0x6f ,0xc5 ,0x30 ,
0x01 ,0x67 ,0x2b ,0xfe ,0xd7 ,0xab ,0x76 ,0xca ,0x82 ,0xc9 ,0x7d ,0xfa ,0x59 ,0x47
,0xf0 ,0xad ,0xd4 ,0xa2 ,0xaf ,0x9c ,0xa4 ,0x72 ,0xc0 ,0xb7 ,0xfd ,0x93 ,0x26 ,
0x36 ,0x3f ,0xf7 ,0xcc ,0x34 ,0xa5 ,0xe5 ,0xf1 ,0x71 ,0xd8 ,0x31 ,0x15 ,0x04 ,0xc7
,0x23 ,0xc3 ,0x18 ,0x96 ,0x05 ,0x9a ,0x07 ,0x12 ,0x80 ,0xe2 ,0xeb ,0x27 ,0xb2 ,
0x75 ,0x09 ,0x83 ,0x2c ,0x1a ,0x1b ,0x6e ,0x5a ,0xa0 ,0x52 ,0x3b ,0xd6 ,0xb3 ,0x29
,0xe3 ,0x2f ,0x84 ,0x53 ,0xd1 ,0x00 ,0xed ,0x20 ,0xfc ,0xb1 ,0x5b ,0x6a ,0xcb ,
0xbe ,0x39 ,0x4a ,0x4c ,0x58 ,0xcf ,0xd0 ,0xef ,0xaa ,0xfb ,0x43 ,0x4d ,0x33 ,0x85
,0x45 ,0xf9 ,0x02 ,0x7f ,0x50 ,0x3c ,0x9f ,0xa8 ,0x51 ,0xa3 ,0x40 ,0x8f ,0x92 ,
0x9d ,0x38 ,0xf5 ,0xbc ,0xb6 ,0xda ,0x21 ,0x10 ,0xff ,0xf3 ,0xd2 ,0xcd ,0x0c ,0x13
,0xec ,0x5f ,0x97 ,0x44 ,0x17 ,0xc4 ,0xa7 ,0x7e ,0x3d ,0x64 ,0x5d ,0x19 ,0x73 ,
0x60 ,0x81 ,0x4f ,0xdc ,0x22 ,0x2a ,0x90 ,0x88 ,0x46 ,0xee ,0xb8 ,0x14 ,0xde ,0x5e
,0x0b ,0xdb ,0xe0 ,0x32 ,0x3a ,0x0a ,0x49 ,0x06 ,0x24 ,0x5c ,0xc2 ,0xd3 ,0xac ,
0x62 ,0x91 ,0x95 ,0xe4 ,0x79 ,0xe7 ,0xc8 ,0x37 ,0x6d ,0x8d ,0xd5 ,0x4e ,0xa9 ,0x6c
,0x56 ,0xf4 ,0xea ,0x65 ,0x7a ,0xae ,0x08 ,0xba ,0x78 ,0x25 ,0x2e ,0x1c ,0xa6 ,
0xb4 ,0xc6 ,0xe8 ,0xdd ,0x74 ,0x1f ,0x4b ,0xbd ,0x8b ,0x8a ,0x70 ,0x3e ,0xb5 ,0x66
,0x48 ,0x03 ,0xf6 ,0x0e ,0x61 ,0x35 ,0x57 ,0xb9 ,0x86 ,0xc1 ,0x1d ,0x9e ,0xe1 ,
0xf8 ,0x98 ,0x11 ,0x69 ,0xd9 ,0x8e ,0x94 ,0x9b ,0x1e ,0x87 ,0xe9 ,0xce ,0x55 ,0x28
,0xdf ,0x8c ,0xa1 ,0x89 ,0x0d ,0xbf ,0xe6 ,0x42 ,0x68 ,0x41 ,0x99 ,0x2d ,0x0f ,
0xb0 ,0x54 ,0xbb ,0x16 };bb41 bbq bb2086(bbq bb441){bb441<<=1 ;bbm(bb441&
0x0100 )bb441^=0x011b ;bb2 bb441;}bb41 bbd bb2081[256 ],bb2080[256 ],
bb2079[256 ],bb2078[256 ];bb41 bbf bb996[256 ];bb41 bbd bb1766[256 ],
bb1765[256 ],bb1764[256 ],bb1763[256 ];bb41 bbb bb2056(){bbq bbz;bb90(
bbz=0 ;bbz<256 ;bbz++){bbq bb76=bb407[bbz];{bbq bb1834=bb2086(bb76),
bb2545=bb1834^bb76;bbq bb45=bb1834<<24 |bb76<<16 |bb76<<8 |bb2545;bb2081
[bbz]=bb45;bb2080[bbz]=((bb45)>>(8 )|(bb45)<<(32 -8 ));bb2079[bbz]=((
bb45)>>(16 )|(bb45)<<(32 -16 ));bb2078[bbz]=((bb45)>>(24 )|(bb45)<<(32 -24
));}bb996[bb76]=bbz;{bbq bb2296=bb2086(bbz),bb2295=bb2086(bb2296),
bb2185=bb2086(bb2295),bb2546=bb2185^bbz,bb2499=bb2185^bb2296^bbz,
bb2502=bb2185^bb2295^bbz,bb2501=bb2185^bb2295^bb2296;bbq bb45=bb2501
<<24 |bb2546<<16 |bb2502<<8 |bb2499;bb1766[bb76]=bb45;bb1765[bb76]=((
bb45)>>(8 )|(bb45)<<(32 -8 ));bb1764[bb76]=((bb45)>>(16 )|(bb45)<<(32 -16 ));
bb1763[bb76]=((bb45)>>(24 )|(bb45)<<(32 -24 ));}}}bbb bb1099(bb364*bbj,
bbh bbb*bb71,bbq bb142){bbq bb1255,bb447,bbz;bb29 bb3=bbj->bb417;bb41
bbu bb1821=1 ;bbm(bb1821){bb2056();bb1821=0 ;}bb32(bb142==16 ||bb142==24
||bb142==32 );bb1255=bb142/4 ;bbj->bb447=bb447=bb1255+6 ;bb90(bbz=0 ;bbz<
bb1255;bbz++) *bb3++=(((bb1)((bb29)bb71+bbz))[3 ]|((bb1)((bb29)bb71+
bbz))[2 ]<<8 |((bb1)((bb29)bb71+bbz))[1 ]<<16 |((bb1)((bb29)bb71+bbz))[0 ]
<<24 );{bbq bbo=1 ;bb90(;bbz<4 * (bb447+1 );bbz++){bbd bb45= * (bb3-1 );
bbm(bbz%bb1255==0 ){bb45=(bb407[bb45>>24 ]^bb407[bb45>>16 &0xff ]<<24 ^
bb407[bb45>>8 &0xff ]<<16 ^bb407[bb45&0xff ]<<8 )^(bbo<<24 );bbo=bb2086(bbo
);}bb54 bbm(bb1255>6 &&bbz%bb1255==4 ){bb45=((bb45)>>(8 )|(bb45)<<(32 -8 ));
bb45=(bb407[bb45>>24 ]^bb407[bb45>>16 &0xff ]<<24 ^bb407[bb45>>8 &0xff ]<<
16 ^bb407[bb45&0xff ]<<8 );} *bb3= * (bb3-bb1255)^bb45;bb3++;}}}bbb
bb1736(bb364*bbj,bbh bbb*bb71,bbq bb142){bb364 bbw;bb29 bb3=bbj->
bb417;bbq bbz;bb1099(&bbw,bb71,bb142);bbj->bb447=bbw.bb447;bb90(bbz=0
;bbz<=bbw.bb447;bbz++){bb81(bb3+4 *bbz,bbw.bb417+4 * (bbw.bb447-bbz),16
);}bb90(bbz=1 ;bbz<bbw.bb447;bbz++){bb3+=4 ;bb3[0 ]=bb1766[bb407[bb3[0 ]
>>24 ]]^bb1765[bb407[bb3[0 ]>>16 &0xff ]]^bb1764[bb407[bb3[0 ]>>8 &0xff ]]^
bb1763[bb407[bb3[0 ]&0xff ]];;bb3[1 ]=bb1766[bb407[bb3[1 ]>>24 ]]^bb1765[
bb407[bb3[1 ]>>16 &0xff ]]^bb1764[bb407[bb3[1 ]>>8 &0xff ]]^bb1763[bb407[
bb3[1 ]&0xff ]];;bb3[2 ]=bb1766[bb407[bb3[2 ]>>24 ]]^bb1765[bb407[bb3[2 ]>>
16 &0xff ]]^bb1764[bb407[bb3[2 ]>>8 &0xff ]]^bb1763[bb407[bb3[2 ]&0xff ]];;
bb3[3 ]=bb1766[bb407[bb3[3 ]>>24 ]]^bb1765[bb407[bb3[3 ]>>16 &0xff ]]^
bb1764[bb407[bb3[3 ]>>8 &0xff ]]^bb1763[bb407[bb3[3 ]&0xff ]];;}}bbb bb1035
(bb364*bbj,bbb*bb14,bbh bbb*bb5){bbd bb718,bb195,bb355,bb821,bb1133,
bb51,bb87,bb1138;bbq bb447=bbj->bb447,bbz;bb29 bb3=(bb29)bbj->bb417;
bb718=(((bb1)((bb29)bb5))[3 ]|((bb1)((bb29)bb5))[2 ]<<8 |((bb1)((bb29)bb5
))[1 ]<<16 |((bb1)((bb29)bb5))[0 ]<<24 )^bb3[0 ];bb195=(((bb1)((bb29)bb5+1
))[3 ]|((bb1)((bb29)bb5+1 ))[2 ]<<8 |((bb1)((bb29)bb5+1 ))[1 ]<<16 |((bb1)((
bb29)bb5+1 ))[0 ]<<24 )^bb3[1 ];bb355=(((bb1)((bb29)bb5+2 ))[3 ]|((bb1)((
bb29)bb5+2 ))[2 ]<<8 |((bb1)((bb29)bb5+2 ))[1 ]<<16 |((bb1)((bb29)bb5+2 ))[0
]<<24 )^bb3[2 ];bb821=(((bb1)((bb29)bb5+3 ))[3 ]|((bb1)((bb29)bb5+3 ))[2 ]
<<8 |((bb1)((bb29)bb5+3 ))[1 ]<<16 |((bb1)((bb29)bb5+3 ))[0 ]<<24 )^bb3[3 ];
bb90(bbz=1 ;bbz<bb447;bbz++){bb3+=4 ;bb1133=bb2081[bb718>>24 ]^bb2080[(
bb195>>16 )&0xff ]^bb2079[(bb355>>8 )&0xff ]^bb2078[(bb821&0xff )]^bb3[0 ];
bb51=bb2081[bb195>>24 ]^bb2080[(bb355>>16 )&0xff ]^bb2079[(bb821>>8 )&
0xff ]^bb2078[(bb718&0xff )]^bb3[1 ];bb87=bb2081[bb355>>24 ]^bb2080[(
bb821>>16 )&0xff ]^bb2079[(bb718>>8 )&0xff ]^bb2078[(bb195&0xff )]^bb3[2 ];
bb1138=bb2081[bb821>>24 ]^bb2080[(bb718>>16 )&0xff ]^bb2079[(bb195>>8 )&
0xff ]^bb2078[(bb355&0xff )]^bb3[3 ];bb718=bb1133;bb195=bb51;bb355=bb87;
bb821=bb1138;}bb3+=4 ;bb1133=bb407[bb718>>24 ]<<24 ^bb407[bb195>>16 &0xff
]<<16 ^bb407[bb355>>8 &0xff ]<<8 ^bb407[bb821&0xff ]^bb3[0 ];bb51=bb407[
bb195>>24 ]<<24 ^bb407[bb355>>16 &0xff ]<<16 ^bb407[bb821>>8 &0xff ]<<8 ^
bb407[bb718&0xff ]^bb3[1 ];bb87=bb407[bb355>>24 ]<<24 ^bb407[bb821>>16 &
0xff ]<<16 ^bb407[bb718>>8 &0xff ]<<8 ^bb407[bb195&0xff ]^bb3[2 ];bb1138=
bb407[bb821>>24 ]<<24 ^bb407[bb718>>16 &0xff ]<<16 ^bb407[bb195>>8 &0xff ]<<
8 ^bb407[bb355&0xff ]^bb3[3 ];((bb29)bb14)[0 ]=(((bb1)(&bb1133))[3 ]|((bb1
)(&bb1133))[2 ]<<8 |((bb1)(&bb1133))[1 ]<<16 |((bb1)(&bb1133))[0 ]<<24 );((
bb29)bb14)[1 ]=(((bb1)(&bb51))[3 ]|((bb1)(&bb51))[2 ]<<8 |((bb1)(&bb51))[
1 ]<<16 |((bb1)(&bb51))[0 ]<<24 );((bb29)bb14)[2 ]=(((bb1)(&bb87))[3 ]|((
bb1)(&bb87))[2 ]<<8 |((bb1)(&bb87))[1 ]<<16 |((bb1)(&bb87))[0 ]<<24 );((
bb29)bb14)[3 ]=(((bb1)(&bb1138))[3 ]|((bb1)(&bb1138))[2 ]<<8 |((bb1)(&
bb1138))[1 ]<<16 |((bb1)(&bb1138))[0 ]<<24 );}bbb bb1775(bb364*bbj,bbb*
bb14,bbh bbb*bb5){bbd bb718,bb195,bb355,bb821,bb1133,bb51,bb87,bb1138
;bbq bb447=bbj->bb447,bbz;bb29 bb3=(bb29)bbj->bb417;bb718=(((bb1)((
bb29)bb5))[3 ]|((bb1)((bb29)bb5))[2 ]<<8 |((bb1)((bb29)bb5))[1 ]<<16 |((
bb1)((bb29)bb5))[0 ]<<24 )^bb3[0 ];bb195=(((bb1)((bb29)bb5+1 ))[3 ]|((bb1)(
(bb29)bb5+1 ))[2 ]<<8 |((bb1)((bb29)bb5+1 ))[1 ]<<16 |((bb1)((bb29)bb5+1 ))[
0 ]<<24 )^bb3[1 ];bb355=(((bb1)((bb29)bb5+2 ))[3 ]|((bb1)((bb29)bb5+2 ))[2 ]
<<8 |((bb1)((bb29)bb5+2 ))[1 ]<<16 |((bb1)((bb29)bb5+2 ))[0 ]<<24 )^bb3[2 ];
bb821=(((bb1)((bb29)bb5+3 ))[3 ]|((bb1)((bb29)bb5+3 ))[2 ]<<8 |((bb1)((
bb29)bb5+3 ))[1 ]<<16 |((bb1)((bb29)bb5+3 ))[0 ]<<24 )^bb3[3 ];bb90(bbz=1 ;
bbz<bb447;bbz++){bb3+=4 ;bb1133=bb1766[bb718>>24 ]^bb1765[bb821>>16 &
0xff ]^bb1764[bb355>>8 &0xff ]^bb1763[bb195&0xff ]^bb3[0 ];bb51=bb1766[
bb195>>24 ]^bb1765[bb718>>16 &0xff ]^bb1764[bb821>>8 &0xff ]^bb1763[bb355&
0xff ]^bb3[1 ];bb87=bb1766[bb355>>24 ]^bb1765[bb195>>16 &0xff ]^bb1764[
bb718>>8 &0xff ]^bb1763[bb821&0xff ]^bb3[2 ];bb1138=bb1766[bb821>>24 ]^
bb1765[bb355>>16 &0xff ]^bb1764[bb195>>8 &0xff ]^bb1763[bb718&0xff ]^bb3[3
];bb718=bb1133;bb195=bb51;bb355=bb87;bb821=bb1138;}bb3+=4 ;bb1133=
bb996[bb718>>24 ]<<24 ^bb996[bb821>>16 &0xff ]<<16 ^bb996[bb355>>8 &0xff ]<<
8 ^bb996[bb195&0xff ]^bb3[0 ];bb51=bb996[bb195>>24 ]<<24 ^bb996[bb718>>16 &
0xff ]<<16 ^bb996[bb821>>8 &0xff ]<<8 ^bb996[bb355&0xff ]^bb3[1 ];bb87=bb996
[bb355>>24 ]<<24 ^bb996[bb195>>16 &0xff ]<<16 ^bb996[bb718>>8 &0xff ]<<8 ^
bb996[bb821&0xff ]^bb3[2 ];bb1138=bb996[bb821>>24 ]<<24 ^bb996[bb355>>16 &
0xff ]<<16 ^bb996[bb195>>8 &0xff ]<<8 ^bb996[bb718&0xff ]^bb3[3 ];((bb29)bb14
)[0 ]=(((bb1)(&bb1133))[3 ]|((bb1)(&bb1133))[2 ]<<8 |((bb1)(&bb1133))[1 ]
<<16 |((bb1)(&bb1133))[0 ]<<24 );((bb29)bb14)[1 ]=(((bb1)(&bb51))[3 ]|((
bb1)(&bb51))[2 ]<<8 |((bb1)(&bb51))[1 ]<<16 |((bb1)(&bb51))[0 ]<<24 );((
bb29)bb14)[2 ]=(((bb1)(&bb87))[3 ]|((bb1)(&bb87))[2 ]<<8 |((bb1)(&bb87))[
1 ]<<16 |((bb1)(&bb87))[0 ]<<24 );((bb29)bb14)[3 ]=(((bb1)(&bb1138))[3 ]|((
bb1)(&bb1138))[2 ]<<8 |((bb1)(&bb1138))[1 ]<<16 |((bb1)(&bb1138))[0 ]<<24 );
}
