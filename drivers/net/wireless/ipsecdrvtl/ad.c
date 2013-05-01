/*
   'aria.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Mon Oct 29 11:54:27 2012
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
bba bbi{bbq bb452;bbd bb418[4 * (16 +1 )];}bb562;bbb bb1592(bb562*bbj,
bbh bbb*bb71,bbq bb143);bbb bb2019(bb562*bbj,bbh bbb*bb71,bbq bb143);
bbb bb1282(bb562*bbj,bbb*bb14,bbh bbb*bb5);
#ifdef __cplusplus
}
#endif
bb41 bbh bbf bb1876[256 ]={0x63 ,0x7c ,0x77 ,0x7b ,0xf2 ,0x6b ,0x6f ,0xc5 ,
0x30 ,0x01 ,0x67 ,0x2b ,0xfe ,0xd7 ,0xab ,0x76 ,0xca ,0x82 ,0xc9 ,0x7d ,0xfa ,0x59
,0x47 ,0xf0 ,0xad ,0xd4 ,0xa2 ,0xaf ,0x9c ,0xa4 ,0x72 ,0xc0 ,0xb7 ,0xfd ,0x93 ,
0x26 ,0x36 ,0x3f ,0xf7 ,0xcc ,0x34 ,0xa5 ,0xe5 ,0xf1 ,0x71 ,0xd8 ,0x31 ,0x15 ,0x04
,0xc7 ,0x23 ,0xc3 ,0x18 ,0x96 ,0x05 ,0x9a ,0x07 ,0x12 ,0x80 ,0xe2 ,0xeb ,0x27 ,
0xb2 ,0x75 ,0x09 ,0x83 ,0x2c ,0x1a ,0x1b ,0x6e ,0x5a ,0xa0 ,0x52 ,0x3b ,0xd6 ,0xb3
,0x29 ,0xe3 ,0x2f ,0x84 ,0x53 ,0xd1 ,0x00 ,0xed ,0x20 ,0xfc ,0xb1 ,0x5b ,0x6a ,
0xcb ,0xbe ,0x39 ,0x4a ,0x4c ,0x58 ,0xcf ,0xd0 ,0xef ,0xaa ,0xfb ,0x43 ,0x4d ,0x33
,0x85 ,0x45 ,0xf9 ,0x02 ,0x7f ,0x50 ,0x3c ,0x9f ,0xa8 ,0x51 ,0xa3 ,0x40 ,0x8f ,
0x92 ,0x9d ,0x38 ,0xf5 ,0xbc ,0xb6 ,0xda ,0x21 ,0x10 ,0xff ,0xf3 ,0xd2 ,0xcd ,0x0c
,0x13 ,0xec ,0x5f ,0x97 ,0x44 ,0x17 ,0xc4 ,0xa7 ,0x7e ,0x3d ,0x64 ,0x5d ,0x19 ,
0x73 ,0x60 ,0x81 ,0x4f ,0xdc ,0x22 ,0x2a ,0x90 ,0x88 ,0x46 ,0xee ,0xb8 ,0x14 ,0xde
,0x5e ,0x0b ,0xdb ,0xe0 ,0x32 ,0x3a ,0x0a ,0x49 ,0x06 ,0x24 ,0x5c ,0xc2 ,0xd3 ,
0xac ,0x62 ,0x91 ,0x95 ,0xe4 ,0x79 ,0xe7 ,0xc8 ,0x37 ,0x6d ,0x8d ,0xd5 ,0x4e ,0xa9
,0x6c ,0x56 ,0xf4 ,0xea ,0x65 ,0x7a ,0xae ,0x08 ,0xba ,0x78 ,0x25 ,0x2e ,0x1c ,
0xa6 ,0xb4 ,0xc6 ,0xe8 ,0xdd ,0x74 ,0x1f ,0x4b ,0xbd ,0x8b ,0x8a ,0x70 ,0x3e ,0xb5
,0x66 ,0x48 ,0x03 ,0xf6 ,0x0e ,0x61 ,0x35 ,0x57 ,0xb9 ,0x86 ,0xc1 ,0x1d ,0x9e ,
0xe1 ,0xf8 ,0x98 ,0x11 ,0x69 ,0xd9 ,0x8e ,0x94 ,0x9b ,0x1e ,0x87 ,0xe9 ,0xce ,0x55
,0x28 ,0xdf ,0x8c ,0xa1 ,0x89 ,0x0d ,0xbf ,0xe6 ,0x42 ,0x68 ,0x41 ,0x99 ,0x2d ,
0x0f ,0xb0 ,0x54 ,0xbb ,0x16 };bb41 bbh bbf bb1875[256 ]={0xe2 ,0x4e ,0x54 ,
0xfc ,0x94 ,0xc2 ,0x4a ,0xcc ,0x62 ,0x0d ,0x6a ,0x46 ,0x3c ,0x4d ,0x8b ,0xd1 ,0x5e
,0xfa ,0x64 ,0xcb ,0xb4 ,0x97 ,0xbe ,0x2b ,0xbc ,0x77 ,0x2e ,0x03 ,0xd3 ,0x19 ,
0x59 ,0xc1 ,0x1d ,0x06 ,0x41 ,0x6b ,0x55 ,0xf0 ,0x99 ,0x69 ,0xea ,0x9c ,0x18 ,0xae
,0x63 ,0xdf ,0xe7 ,0xbb ,0x00 ,0x73 ,0x66 ,0xfb ,0x96 ,0x4c ,0x85 ,0xe4 ,0x3a ,
0x09 ,0x45 ,0xaa ,0x0f ,0xee ,0x10 ,0xeb ,0x2d ,0x7f ,0xf4 ,0x29 ,0xac ,0xcf ,0xad
,0x91 ,0x8d ,0x78 ,0xc8 ,0x95 ,0xf9 ,0x2f ,0xce ,0xcd ,0x08 ,0x7a ,0x88 ,0x38 ,
0x5c ,0x83 ,0x2a ,0x28 ,0x47 ,0xdb ,0xb8 ,0xc7 ,0x93 ,0xa4 ,0x12 ,0x53 ,0xff ,0x87
,0x0e ,0x31 ,0x36 ,0x21 ,0x58 ,0x48 ,0x01 ,0x8e ,0x37 ,0x74 ,0x32 ,0xca ,0xe9 ,
0xb1 ,0xb7 ,0xab ,0x0c ,0xd7 ,0xc4 ,0x56 ,0x42 ,0x26 ,0x07 ,0x98 ,0x60 ,0xd9 ,0xb6
,0xb9 ,0x11 ,0x40 ,0xec ,0x20 ,0x8c ,0xbd ,0xa0 ,0xc9 ,0x84 ,0x04 ,0x49 ,0x23 ,
0xf1 ,0x4f ,0x50 ,0x1f ,0x13 ,0xdc ,0xd8 ,0xc0 ,0x9e ,0x57 ,0xe3 ,0xc3 ,0x7b ,0x65
,0x3b ,0x02 ,0x8f ,0x3e ,0xe8 ,0x25 ,0x92 ,0xe5 ,0x15 ,0xdd ,0xfd ,0x17 ,0xa9 ,
0xbf ,0xd4 ,0x9a ,0x7e ,0xc5 ,0x39 ,0x67 ,0xfe ,0x76 ,0x9d ,0x43 ,0xa7 ,0xe1 ,0xd0
,0xf5 ,0x68 ,0xf2 ,0x1b ,0x34 ,0x70 ,0x05 ,0xa3 ,0x8a ,0xd5 ,0x79 ,0x86 ,0xa8 ,
0x30 ,0xc6 ,0x51 ,0x4b ,0x1e ,0xa6 ,0x27 ,0xf6 ,0x35 ,0xd2 ,0x6e ,0x24 ,0x16 ,0x82
,0x5f ,0xda ,0xe6 ,0x75 ,0xa2 ,0xef ,0x2c ,0xb2 ,0x1c ,0x9f ,0x5d ,0x6f ,0x80 ,
0x0a ,0x72 ,0x44 ,0x9b ,0x6c ,0x90 ,0x0b ,0x5b ,0x33 ,0x7d ,0x5a ,0x52 ,0xf3 ,0x61
,0xa1 ,0xf7 ,0xb0 ,0xd6 ,0x3f ,0x7c ,0x6d ,0xed ,0x14 ,0xe0 ,0xa5 ,0x3d ,0x22 ,
0xb3 ,0xf8 ,0x89 ,0xde ,0x71 ,0x1a ,0xaf ,0xba ,0xb5 ,0x81 };bb41 bbf bb2095[
256 ],bb2094[256 ];bb41 bbd bb666[256 ],bb665[256 ];bb41 bbd bb778[256 ],
bb753[256 ];bb41 bbb bb2058(){bbq bbz;bb90(bbz=0 ;bbz<256 ;bbz++){bbq
bb2109=bb1876[bbz],bb1835=bb1875[bbz];bb2095[bb2109]=bbz;bb2094[
bb1835]=bbz;bb666[bbz]=bb2109<<16 |bb2109<<8 |bb2109;bb665[bbz]=bb1835
<<24 |bb1835<<8 |bb1835;bb778[bb2109]=bbz<<24 |bbz<<16 |bbz;bb753[bb1835]
=bbz<<24 |bbz<<16 |bbz<<8 ;}}bbb bb1592(bb562*bbj,bbh bbb*bb71,bbq bb143
){bbq bb1256,bb452,bbz,bb76;bb29 bb3=bbj->bb418;bbd bbv[4 ][4 ];bb41 bbu
bb1822=1 ;bbm(bb1822){bb2058();bb1822=0 ;}bb32(bb143==16 ||bb143==24 ||
bb143==32 );bb1256=bb143/4 ;bbj->bb452=bb452=bb1256+8 ;bb90(bbz=0 ;bbz<4 ;
bbz++){bbm(bbz==0 )bb999(bbv[0 ],0 ,16 );bb54{bb41 bbd bb2559[3 ][4 ]={{
0x517cc1b7 ,0x27220a94 ,0xfe13abe8 ,0xfa9a6ee0 },{0x6db14acc ,0x9e21c820 ,
0xff28b1d5 ,0xef5de2b0 },{0xdb92371d ,0x2126e970 ,0x03249775 ,0x04e8c90e }}
;bb29 bb105=bbv[bbz],bb1340=bbv[bbz-1 ],bbo=bb2559[(bbz-1 +(bb1256-4 )/2
)%3 ];bbm(bbz%2 ==1 ){bb105[0 ]=bb1340[0 ]^bbo[0 ];bb105[1 ]=bb1340[1 ]^bbo[1
];bb105[2 ]=bb1340[2 ]^bbo[2 ];bb105[3 ]=bb1340[3 ]^bbo[3 ];bb105[0 ]=bb666[
bb105[0 ]>>24 ]^bb665[bb105[0 ]>>16 &0xff ]^bb778[bb105[0 ]>>8 &0xff ]^bb753[
bb105[0 ]&0xff ];bb105[1 ]=bb666[bb105[1 ]>>24 ]^bb665[bb105[1 ]>>16 &0xff ]^
bb778[bb105[1 ]>>8 &0xff ]^bb753[bb105[1 ]&0xff ];bb105[2 ]=bb666[bb105[2 ]
>>24 ]^bb665[bb105[2 ]>>16 &0xff ]^bb778[bb105[2 ]>>8 &0xff ]^bb753[bb105[2 ]
&0xff ];bb105[3 ]=bb666[bb105[3 ]>>24 ]^bb665[bb105[3 ]>>16 &0xff ]^bb778[
bb105[3 ]>>8 &0xff ]^bb753[bb105[3 ]&0xff ];bb105[1 ]^=bb105[2 ];bb105[2 ]^=
bb105[3 ];bb105[0 ]^=bb105[1 ];bb105[3 ]^=bb105[1 ];bb105[2 ]^=bb105[0 ];
bb105[1 ]^=bb105[2 ];bb105[1 ]=(bb105[1 ]&0xff00ff )<<8 |bb105[1 ]>>8 &
0xff00ff ;bb105[2 ]=bb105[2 ]<<16 |bb105[2 ]>>16 ;bb105[3 ]=bb105[3 ]<<24 |(
bb105[3 ]&0xff00 )<<8 |bb105[3 ]>>8 &0xff00 |bb105[3 ]>>24 ;bb105[1 ]^=bb105[2
];bb105[2 ]^=bb105[3 ];bb105[0 ]^=bb105[1 ];bb105[3 ]^=bb105[1 ];bb105[2 ]^=
bb105[0 ];bb105[1 ]^=bb105[2 ];}bb54{bb105[0 ]=bb1340[0 ]^bbo[0 ];bb105[1 ]=
bb1340[1 ]^bbo[1 ];bb105[2 ]=bb1340[2 ]^bbo[2 ];bb105[3 ]=bb1340[3 ]^bbo[3 ];
bb105[0 ]=bb778[bb105[0 ]>>24 ]^bb753[bb105[0 ]>>16 &0xff ]^bb666[bb105[0 ]
>>8 &0xff ]^bb665[bb105[0 ]&0xff ];bb105[1 ]=bb778[bb105[1 ]>>24 ]^bb753[
bb105[1 ]>>16 &0xff ]^bb666[bb105[1 ]>>8 &0xff ]^bb665[bb105[1 ]&0xff ];bb105
[2 ]=bb778[bb105[2 ]>>24 ]^bb753[bb105[2 ]>>16 &0xff ]^bb666[bb105[2 ]>>8 &
0xff ]^bb665[bb105[2 ]&0xff ];bb105[3 ]=bb778[bb105[3 ]>>24 ]^bb753[bb105[3
]>>16 &0xff ]^bb666[bb105[3 ]>>8 &0xff ]^bb665[bb105[3 ]&0xff ];bb105[1 ]^=
bb105[2 ];bb105[2 ]^=bb105[3 ];bb105[0 ]^=bb105[1 ];bb105[3 ]^=bb105[1 ];
bb105[2 ]^=bb105[0 ];bb105[1 ]^=bb105[2 ];bb105[3 ]=(bb105[3 ]&0xff00ff )<<8
|bb105[3 ]>>8 &0xff00ff ;bb105[0 ]=bb105[0 ]<<16 |bb105[0 ]>>16 ;bb105[1 ]=
bb105[1 ]<<24 |(bb105[1 ]&0xff00 )<<8 |bb105[1 ]>>8 &0xff00 |bb105[1 ]>>24 ;
bb105[1 ]^=bb105[2 ];bb105[2 ]^=bb105[3 ];bb105[0 ]^=bb105[1 ];bb105[3 ]^=
bb105[1 ];bb105[2 ]^=bb105[0 ];bb105[1 ]^=bb105[2 ];}}bb90(bb76=0 ;bb76<4 ;
bb76++){bbq bb195=4 *bbz+bb76;bbv[bbz][bb76]^=(bbz<2 ?(bb195<bb1256?(
bbd)(((bb1)((bb29)bb71+bb195))[3 ]|((bb1)((bb29)bb71+bb195))[2 ]<<8 |((
bb1)((bb29)bb71+bb195))[1 ]<<16 |((bb1)((bb29)bb71+bb195))[0 ]<<24 ):0U ):
bbv[bbz-2 ][bb76]);}}bb90(bbz=0 ;bbz<=bb452;bbz++){bb90(bb76=0 ;bb76<4 ;
bb76++){bb41 bbq bbg[]={19 ,31 ,67 ,97 ,109 };bbq bb195=128 -bbg[bbz/4 ];
bb29 bb105=bbv[bbz%4 ],bb1340=bbv[(bbz+1 )%4 ]; *bb3++=bb105[bb76]^
bb1340[(bb76+bb195/32 )%4 ]<<(bb195%32 )^bb1340[(bb76+bb195/32 +1 )%4 ]>>(
32 -bb195%32 );}}}bbb bb2019(bb562*bbj,bbh bbb*bb71,bbq bb143){bb562 bbw
;bb29 bb3=bbj->bb418;bbq bbz;bb1592(&bbw,bb71,bb143);bbj->bb452=bbw.
bb452;bb90(bbz=0 ;bbz<=bbw.bb452;bbz++){bb81(bb3+4 *bbz,bbw.bb418+4 * (
bbw.bb452-bbz),16 );}bb90(bbz=1 ;bbz<bbw.bb452;bbz++){bb3+=4 ;bb3[0 ]=bb3
[0 ]<<8 ^bb3[0 ]>>8 ^bb3[0 ]<<16 ^bb3[0 ]>>16 ^bb3[0 ]<<24 ^bb3[0 ]>>24 ;bb3[1 ]=
bb3[1 ]<<8 ^bb3[1 ]>>8 ^bb3[1 ]<<16 ^bb3[1 ]>>16 ^bb3[1 ]<<24 ^bb3[1 ]>>24 ;bb3[2
]=bb3[2 ]<<8 ^bb3[2 ]>>8 ^bb3[2 ]<<16 ^bb3[2 ]>>16 ^bb3[2 ]<<24 ^bb3[2 ]>>24 ;bb3
[3 ]=bb3[3 ]<<8 ^bb3[3 ]>>8 ^bb3[3 ]<<16 ^bb3[3 ]>>16 ^bb3[3 ]<<24 ^bb3[3 ]>>24 ;
bb3[1 ]^=bb3[2 ];bb3[2 ]^=bb3[3 ];bb3[0 ]^=bb3[1 ];bb3[3 ]^=bb3[1 ];bb3[2 ]^=
bb3[0 ];bb3[1 ]^=bb3[2 ];bb3[1 ]=(bb3[1 ]&0xff00ff )<<8 |bb3[1 ]>>8 &0xff00ff ;
bb3[2 ]=bb3[2 ]<<16 |bb3[2 ]>>16 ;bb3[3 ]=bb3[3 ]<<24 |(bb3[3 ]&0xff00 )<<8 |bb3
[3 ]>>8 &0xff00 |bb3[3 ]>>24 ;bb3[1 ]^=bb3[2 ];bb3[2 ]^=bb3[3 ];bb3[0 ]^=bb3[1 ]
;bb3[3 ]^=bb3[1 ];bb3[2 ]^=bb3[0 ];bb3[1 ]^=bb3[2 ];}}bbb bb1282(bb562*bbj,
bbb*bb14,bbh bbb*bb5){bbd bbg[4 ];bb29 bb3=bbj->bb418;bbq bb452=bbj->
bb452,bbz;bbg[0 ]=(bbd)(((bb1)((bb29)bb5))[3 ]|((bb1)((bb29)bb5))[2 ]<<8
|((bb1)((bb29)bb5))[1 ]<<16 |((bb1)((bb29)bb5))[0 ]<<24 );bbg[1 ]=(bbd)(((
bb1)((bb29)bb5+1 ))[3 ]|((bb1)((bb29)bb5+1 ))[2 ]<<8 |((bb1)((bb29)bb5+1 ))[
1 ]<<16 |((bb1)((bb29)bb5+1 ))[0 ]<<24 );bbg[2 ]=(bbd)(((bb1)((bb29)bb5+2 ))[
3 ]|((bb1)((bb29)bb5+2 ))[2 ]<<8 |((bb1)((bb29)bb5+2 ))[1 ]<<16 |((bb1)((
bb29)bb5+2 ))[0 ]<<24 );bbg[3 ]=(bbd)(((bb1)((bb29)bb5+3 ))[3 ]|((bb1)((
bb29)bb5+3 ))[2 ]<<8 |((bb1)((bb29)bb5+3 ))[1 ]<<16 |((bb1)((bb29)bb5+3 ))[0
]<<24 );bb90(bbz=2 ;bbz<bb452;bbz+=2 ){bbg[0 ]=bbg[0 ]^bb3[0 ];bbg[1 ]=bbg[1
]^bb3[1 ];bbg[2 ]=bbg[2 ]^bb3[2 ];bbg[3 ]=bbg[3 ]^bb3[3 ];bbg[0 ]=bb666[bbg[0
]>>24 ]^bb665[bbg[0 ]>>16 &0xff ]^bb778[bbg[0 ]>>8 &0xff ]^bb753[bbg[0 ]&0xff
];bbg[1 ]=bb666[bbg[1 ]>>24 ]^bb665[bbg[1 ]>>16 &0xff ]^bb778[bbg[1 ]>>8 &
0xff ]^bb753[bbg[1 ]&0xff ];bbg[2 ]=bb666[bbg[2 ]>>24 ]^bb665[bbg[2 ]>>16 &
0xff ]^bb778[bbg[2 ]>>8 &0xff ]^bb753[bbg[2 ]&0xff ];bbg[3 ]=bb666[bbg[3 ]>>
24 ]^bb665[bbg[3 ]>>16 &0xff ]^bb778[bbg[3 ]>>8 &0xff ]^bb753[bbg[3 ]&0xff ];
bbg[1 ]^=bbg[2 ];bbg[2 ]^=bbg[3 ];bbg[0 ]^=bbg[1 ];bbg[3 ]^=bbg[1 ];bbg[2 ]^=
bbg[0 ];bbg[1 ]^=bbg[2 ];bbg[1 ]=(bbg[1 ]&0xff00ff )<<8 |bbg[1 ]>>8 &0xff00ff ;
bbg[2 ]=bbg[2 ]<<16 |bbg[2 ]>>16 ;bbg[3 ]=bbg[3 ]<<24 |(bbg[3 ]&0xff00 )<<8 |bbg
[3 ]>>8 &0xff00 |bbg[3 ]>>24 ;bbg[1 ]^=bbg[2 ];bbg[2 ]^=bbg[3 ];bbg[0 ]^=bbg[1 ]
;bbg[3 ]^=bbg[1 ];bbg[2 ]^=bbg[0 ];bbg[1 ]^=bbg[2 ];bb3+=4 ;bbg[0 ]=bbg[0 ]^
bb3[0 ];bbg[1 ]=bbg[1 ]^bb3[1 ];bbg[2 ]=bbg[2 ]^bb3[2 ];bbg[3 ]=bbg[3 ]^bb3[3 ]
;bbg[0 ]=bb778[bbg[0 ]>>24 ]^bb753[bbg[0 ]>>16 &0xff ]^bb666[bbg[0 ]>>8 &0xff
]^bb665[bbg[0 ]&0xff ];bbg[1 ]=bb778[bbg[1 ]>>24 ]^bb753[bbg[1 ]>>16 &0xff ]^
bb666[bbg[1 ]>>8 &0xff ]^bb665[bbg[1 ]&0xff ];bbg[2 ]=bb778[bbg[2 ]>>24 ]^
bb753[bbg[2 ]>>16 &0xff ]^bb666[bbg[2 ]>>8 &0xff ]^bb665[bbg[2 ]&0xff ];bbg[3
]=bb778[bbg[3 ]>>24 ]^bb753[bbg[3 ]>>16 &0xff ]^bb666[bbg[3 ]>>8 &0xff ]^
bb665[bbg[3 ]&0xff ];bbg[1 ]^=bbg[2 ];bbg[2 ]^=bbg[3 ];bbg[0 ]^=bbg[1 ];bbg[3
]^=bbg[1 ];bbg[2 ]^=bbg[0 ];bbg[1 ]^=bbg[2 ];bbg[3 ]=(bbg[3 ]&0xff00ff )<<8 |
bbg[3 ]>>8 &0xff00ff ;bbg[0 ]=bbg[0 ]<<16 |bbg[0 ]>>16 ;bbg[1 ]=bbg[1 ]<<24 |(
bbg[1 ]&0xff00 )<<8 |bbg[1 ]>>8 &0xff00 |bbg[1 ]>>24 ;bbg[1 ]^=bbg[2 ];bbg[2 ]^=
bbg[3 ];bbg[0 ]^=bbg[1 ];bbg[3 ]^=bbg[1 ];bbg[2 ]^=bbg[0 ];bbg[1 ]^=bbg[2 ];
bb3+=4 ;}bbg[0 ]=bbg[0 ]^bb3[0 ];bbg[1 ]=bbg[1 ]^bb3[1 ];bbg[2 ]=bbg[2 ]^bb3[2
];bbg[3 ]=bbg[3 ]^bb3[3 ];bbg[0 ]=bb666[bbg[0 ]>>24 ]^bb665[bbg[0 ]>>16 &0xff
]^bb778[bbg[0 ]>>8 &0xff ]^bb753[bbg[0 ]&0xff ];bbg[1 ]=bb666[bbg[1 ]>>24 ]^
bb665[bbg[1 ]>>16 &0xff ]^bb778[bbg[1 ]>>8 &0xff ]^bb753[bbg[1 ]&0xff ];bbg[2
]=bb666[bbg[2 ]>>24 ]^bb665[bbg[2 ]>>16 &0xff ]^bb778[bbg[2 ]>>8 &0xff ]^
bb753[bbg[2 ]&0xff ];bbg[3 ]=bb666[bbg[3 ]>>24 ]^bb665[bbg[3 ]>>16 &0xff ]^
bb778[bbg[3 ]>>8 &0xff ]^bb753[bbg[3 ]&0xff ];bbg[1 ]^=bbg[2 ];bbg[2 ]^=bbg[3
];bbg[0 ]^=bbg[1 ];bbg[3 ]^=bbg[1 ];bbg[2 ]^=bbg[0 ];bbg[1 ]^=bbg[2 ];bbg[1 ]=
(bbg[1 ]&0xff00ff )<<8 |bbg[1 ]>>8 &0xff00ff ;bbg[2 ]=bbg[2 ]<<16 |bbg[2 ]>>16 ;
bbg[3 ]=bbg[3 ]<<24 |(bbg[3 ]&0xff00 )<<8 |bbg[3 ]>>8 &0xff00 |bbg[3 ]>>24 ;bbg[
1 ]^=bbg[2 ];bbg[2 ]^=bbg[3 ];bbg[0 ]^=bbg[1 ];bbg[3 ]^=bbg[1 ];bbg[2 ]^=bbg[0
];bbg[1 ]^=bbg[2 ];bb3+=4 ;bbg[0 ]^=bb3[0 ];bbg[1 ]^=bb3[1 ];bbg[2 ]^=bb3[2 ];
bbg[3 ]^=bb3[3 ];bb3+=4 ;bbg[0 ]=bb2095[bbg[0 ]>>24 ]<<24 ^bb2094[bbg[0 ]>>16
&0xff ]<<16 ^bb1876[bbg[0 ]>>8 &0xff ]<<8 ^bb1875[bbg[0 ]&0xff ]^bb3[0 ];bbg[1
]=bb2095[bbg[1 ]>>24 ]<<24 ^bb2094[bbg[1 ]>>16 &0xff ]<<16 ^bb1876[bbg[1 ]>>8
&0xff ]<<8 ^bb1875[bbg[1 ]&0xff ]^bb3[1 ];bbg[2 ]=bb2095[bbg[2 ]>>24 ]<<24 ^
bb2094[bbg[2 ]>>16 &0xff ]<<16 ^bb1876[bbg[2 ]>>8 &0xff ]<<8 ^bb1875[bbg[2 ]&
0xff ]^bb3[2 ];bbg[3 ]=bb2095[bbg[3 ]>>24 ]<<24 ^bb2094[bbg[3 ]>>16 &0xff ]<<
16 ^bb1876[bbg[3 ]>>8 &0xff ]<<8 ^bb1875[bbg[3 ]&0xff ]^bb3[3 ];((bb29)bb14)[
0 ]=(bbd)(((bb1)(bbg))[3 ]|((bb1)(bbg))[2 ]<<8 |((bb1)(bbg))[1 ]<<16 |((bb1
)(bbg))[0 ]<<24 );((bb29)bb14)[1 ]=(bbd)(((bb1)(bbg+1 ))[3 ]|((bb1)(bbg+1 ))[
2 ]<<8 |((bb1)(bbg+1 ))[1 ]<<16 |((bb1)(bbg+1 ))[0 ]<<24 );((bb29)bb14)[2 ]=(
bbd)(((bb1)(bbg+2 ))[3 ]|((bb1)(bbg+2 ))[2 ]<<8 |((bb1)(bbg+2 ))[1 ]<<16 |((
bb1)(bbg+2 ))[0 ]<<24 );((bb29)bb14)[3 ]=(bbd)(((bb1)(bbg+3 ))[3 ]|((bb1)(
bbg+3 ))[2 ]<<8 |((bb1)(bbg+3 ))[1 ]<<16 |((bb1)(bbg+3 ))[0 ]<<24 );}
