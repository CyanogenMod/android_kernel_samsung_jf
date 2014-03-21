/*
   'src_compress_deflate_crc32.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Wed Dec 18 14:11:10 2013
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
#if ( defined( _WIN32) || defined( __WIN32__)) && ! defined( WIN32)
#define WIN32
#endif
#if defined( __GNUC__) || defined( WIN32) || defined( bb1226) ||  \
defined( bb1217)
#ifndef bb398
#define bb398
#endif
#endif
#if defined( __MSDOS__) && ! defined( bb169)
#define bb169
#endif
#if defined( bb169) && ! defined( bb398)
#define bb510
#endif
#ifdef bb169
#define bb1040
#endif
#if ( defined( bb169) || defined( bb1190) || defined( WIN32)) && !  \
defined( bb136)
#define bb136
#endif
#if defined( __STDC__) || defined( __cplusplus) || defined( bb1212)
#ifndef bb136
#define bb136
#endif
#endif
#ifndef bb136
#ifndef bbh
#define bbh
#endif
#endif
#if defined( __BORLANDC__) && ( __BORLANDC__ < 0x500)
#define bb1146
#endif
#ifndef bb206
#ifdef bb510
#define bb206 8
#else
#define bb206 9
#endif
#endif
#ifndef bbp
#ifdef bb136
#define bbp( bb406) bb406
#else
#define bbp( bb406) ()
#endif
#endif
bba bbf bb152;bba bbs bbe bb7;bba bbs bb6 bb24;bba bb152 bb30;bba bbl
bb439;bba bbe bb1141;bba bb7 bb165;bba bb24 bb167;
#ifdef bb136
bba bbb*bb70;bba bbb*bb182;
#else
bba bb152*bb70;bba bb152*bb182;
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bb70( *bb505)bbp((bb70 bb110,bb7 bb496,bb7 bb47));bba bbb( *bb504
)bbp((bb70 bb110,bb70 bb1103));bbi bb381;bba bbi bb1187{bb30*bb127;
bb7 bb148;bb24 bb187;bb30*bb597;bb7 bb384;bb24 bb605;bbl*bb320;bbi
bb381*bb23;bb505 bb401;bb504 bb371;bb70 bb110;bbe bb969;bb24 bb367;
bb24 bb1152;}bb435;bba bb435*bb17;bbr bbh bbl*bb1158 bbp((bbb));bbr
bbe bb506 bbp((bb17 bb16,bbe bb174));bbr bbe bb943 bbp((bb17 bb16));
bbr bbe bb1054 bbp((bb17 bb16,bbe bb174));bbr bbe bb956 bbp((bb17 bb16
));bbr bbe bb1177 bbp((bb17 bb16,bbh bb30*bb424,bb7 bb431));bbr bbe
bb1150 bbp((bb17 bb130,bb17 bb184));bbr bbe bb1058 bbp((bb17 bb16));
bbr bbe bb1178 bbp((bb17 bb16,bbe bb123,bbe bb295));bbr bbe bb1149 bbp
((bb17 bb16,bbh bb30*bb424,bb7 bb431));bbr bbe bb1172 bbp((bb17 bb16));
bbr bbe bb1009 bbp((bb17 bb16));bbr bbe bb1151 bbp((bb30*bb130,bb167*
bb317,bbh bb30*bb184,bb24 bb326));bbr bbe bb1108 bbp((bb30*bb130,
bb167*bb317,bbh bb30*bb184,bb24 bb326,bbe bb123));bbr bbe bb1162 bbp(
(bb30*bb130,bb167*bb317,bbh bb30*bb184,bb24 bb326));bba bb182 bb34;
bbr bb34 bb1206 bbp((bbh bbl*bb1052,bbh bbl*bb57));bbr bb34 bb1208 bbp
((bbe bb472,bbh bbl*bb57));bbr bbe bb1214 bbp((bb34 bb25,bbe bb123,
bbe bb295));bbr bbe bb1191 bbp((bb34 bb25,bb182 bb40,bbs bb21));bbr
bbe bb1188 bbp((bb34 bb25,bbh bb182 bb40,bbs bb21));bbr bbe bb1218 bbp
((bb34 bb25,bbh bbl*bb1230,...));bbr bbe bb1194 bbp((bb34 bb25,bbh bbl
 *bbg));bbr bbl*bb1182 bbp((bb34 bb25,bbl*bb40,bbe bb21));bbr bbe
bb1192 bbp((bb34 bb25,bbe bbo));bbr bbe bb1183 bbp((bb34 bb25));bbr
bbe bb1185 bbp((bb34 bb25,bbe bb174));bbr bb6 bb1195 bbp((bb34 bb25,
bb6 bb92,bbe bb1202));bbr bbe bb1181 bbp((bb34 bb25));bbr bb6 bb1193
bbp((bb34 bb25));bbr bbe bb1198 bbp((bb34 bb25));bbr bbe bb1203 bbp((
bb34 bb25));bbr bbh bbl*bb1223 bbp((bb34 bb25,bbe*bb1180));bbr bb24
bb986 bbp((bb24 bb367,bbh bb30*bb40,bb7 bb21));bbr bb24 bb1174 bbp((
bb24 bb380,bbh bb30*bb40,bb7 bb21));bbr bbe bb1116 bbp((bb17 bb16,bbe
bb123,bbh bbl*bb189,bbe bb196));bbr bbe bb1127 bbp((bb17 bb16,bbh bbl
 *bb189,bbe bb196));bbr bbe bb1047 bbp((bb17 bb16,bbe bb123,bbe bb569
,bbe bb452,bbe bb958,bbe bb295,bbh bbl*bb189,bbe bb196));bbr bbe
bb1049 bbp((bb17 bb16,bbe bb452,bbh bbl*bb189,bbe bb196));bbi bb381{
bbe bb442;};bbr bbh bbl*bb1165 bbp((bbe bb18));bbr bbe bb1173 bbp((
bb17 bby));bbr bbh bb167*bb1163 bbp((bbb));
#ifdef __cplusplus
}
#endif
bb41 bbh bb167 bb1521[256 ]={0x00000000L ,0x77073096L ,0xee0e612cL ,
0x990951baL ,0x076dc419L ,0x706af48fL ,0xe963a535L ,0x9e6495a3L ,
0x0edb8832L ,0x79dcb8a4L ,0xe0d5e91eL ,0x97d2d988L ,0x09b64c2bL ,
0x7eb17cbdL ,0xe7b82d07L ,0x90bf1d91L ,0x1db71064L ,0x6ab020f2L ,
0xf3b97148L ,0x84be41deL ,0x1adad47dL ,0x6ddde4ebL ,0xf4d4b551L ,
0x83d385c7L ,0x136c9856L ,0x646ba8c0L ,0xfd62f97aL ,0x8a65c9ecL ,
0x14015c4fL ,0x63066cd9L ,0xfa0f3d63L ,0x8d080df5L ,0x3b6e20c8L ,
0x4c69105eL ,0xd56041e4L ,0xa2677172L ,0x3c03e4d1L ,0x4b04d447L ,
0xd20d85fdL ,0xa50ab56bL ,0x35b5a8faL ,0x42b2986cL ,0xdbbbc9d6L ,
0xacbcf940L ,0x32d86ce3L ,0x45df5c75L ,0xdcd60dcfL ,0xabd13d59L ,
0x26d930acL ,0x51de003aL ,0xc8d75180L ,0xbfd06116L ,0x21b4f4b5L ,
0x56b3c423L ,0xcfba9599L ,0xb8bda50fL ,0x2802b89eL ,0x5f058808L ,
0xc60cd9b2L ,0xb10be924L ,0x2f6f7c87L ,0x58684c11L ,0xc1611dabL ,
0xb6662d3dL ,0x76dc4190L ,0x01db7106L ,0x98d220bcL ,0xefd5102aL ,
0x71b18589L ,0x06b6b51fL ,0x9fbfe4a5L ,0xe8b8d433L ,0x7807c9a2L ,
0x0f00f934L ,0x9609a88eL ,0xe10e9818L ,0x7f6a0dbbL ,0x086d3d2dL ,
0x91646c97L ,0xe6635c01L ,0x6b6b51f4L ,0x1c6c6162L ,0x856530d8L ,
0xf262004eL ,0x6c0695edL ,0x1b01a57bL ,0x8208f4c1L ,0xf50fc457L ,
0x65b0d9c6L ,0x12b7e950L ,0x8bbeb8eaL ,0xfcb9887cL ,0x62dd1ddfL ,
0x15da2d49L ,0x8cd37cf3L ,0xfbd44c65L ,0x4db26158L ,0x3ab551ceL ,
0xa3bc0074L ,0xd4bb30e2L ,0x4adfa541L ,0x3dd895d7L ,0xa4d1c46dL ,
0xd3d6f4fbL ,0x4369e96aL ,0x346ed9fcL ,0xad678846L ,0xda60b8d0L ,
0x44042d73L ,0x33031de5L ,0xaa0a4c5fL ,0xdd0d7cc9L ,0x5005713cL ,
0x270241aaL ,0xbe0b1010L ,0xc90c2086L ,0x5768b525L ,0x206f85b3L ,
0xb966d409L ,0xce61e49fL ,0x5edef90eL ,0x29d9c998L ,0xb0d09822L ,
0xc7d7a8b4L ,0x59b33d17L ,0x2eb40d81L ,0xb7bd5c3bL ,0xc0ba6cadL ,
0xedb88320L ,0x9abfb3b6L ,0x03b6e20cL ,0x74b1d29aL ,0xead54739L ,
0x9dd277afL ,0x04db2615L ,0x73dc1683L ,0xe3630b12L ,0x94643b84L ,
0x0d6d6a3eL ,0x7a6a5aa8L ,0xe40ecf0bL ,0x9309ff9dL ,0x0a00ae27L ,
0x7d079eb1L ,0xf00f9344L ,0x8708a3d2L ,0x1e01f268L ,0x6906c2feL ,
0xf762575dL ,0x806567cbL ,0x196c3671L ,0x6e6b06e7L ,0xfed41b76L ,
0x89d32be0L ,0x10da7a5aL ,0x67dd4accL ,0xf9b9df6fL ,0x8ebeeff9L ,
0x17b7be43L ,0x60b08ed5L ,0xd6d6a3e8L ,0xa1d1937eL ,0x38d8c2c4L ,
0x4fdff252L ,0xd1bb67f1L ,0xa6bc5767L ,0x3fb506ddL ,0x48b2364bL ,
0xd80d2bdaL ,0xaf0a1b4cL ,0x36034af6L ,0x41047a60L ,0xdf60efc3L ,
0xa867df55L ,0x316e8eefL ,0x4669be79L ,0xcb61b38cL ,0xbc66831aL ,
0x256fd2a0L ,0x5268e236L ,0xcc0c7795L ,0xbb0b4703L ,0x220216b9L ,
0x5505262fL ,0xc5ba3bbeL ,0xb2bd0b28L ,0x2bb45a92L ,0x5cb36a04L ,
0xc2d7ffa7L ,0xb5d0cf31L ,0x2cd99e8bL ,0x5bdeae1dL ,0x9b64c2b0L ,
0xec63f226L ,0x756aa39cL ,0x026d930aL ,0x9c0906a9L ,0xeb0e363fL ,
0x72076785L ,0x05005713L ,0x95bf4a82L ,0xe2b87a14L ,0x7bb12baeL ,
0x0cb61b38L ,0x92d28e9bL ,0xe5d5be0dL ,0x7cdcefb7L ,0x0bdbdf21L ,
0x86d3d2d4L ,0xf1d4e242L ,0x68ddb3f8L ,0x1fda836eL ,0x81be16cdL ,
0xf6b9265bL ,0x6fb077e1L ,0x18b74777L ,0x88085ae6L ,0xff0f6a70L ,
0x66063bcaL ,0x11010b5cL ,0x8f659effL ,0xf862ae69L ,0x616bffd3L ,
0x166ccf45L ,0xa00ae278L ,0xd70dd2eeL ,0x4e048354L ,0x3903b3c2L ,
0xa7672661L ,0xd06016f7L ,0x4969474dL ,0x3e6e77dbL ,0xaed16a4aL ,
0xd9d65adcL ,0x40df0b66L ,0x37d83bf0L ,0xa9bcae53L ,0xdebb9ec5L ,
0x47b2cf7fL ,0x30b5ffe9L ,0xbdbdf21cL ,0xcabac28aL ,0x53b39330L ,
0x24b4a3a6L ,0xbad03605L ,0xcdd70693L ,0x54de5729L ,0x23d967bfL ,
0xb3667a2eL ,0xc4614ab8L ,0x5d681b02L ,0x2a6f2b94L ,0xb40bbe37L ,
0xc30c8ea1L ,0x5a05df1bL ,0x2d02ef8dL };bbh bb167*bb1163(){bb2(bbh bb167
 * )bb1521;}bb24 bb1174(bb380,bb40,bb21)bb24 bb380;bbh bb30*bb40;bb7
bb21;{bbm(bb40==0 )bb2 0L ;bb380=bb380^0xffffffffL ;bb106(bb21>=8 ){bb380
=bb1521[((bbe)bb380^( *bb40++))&0xff ]^(bb380>>8 );;bb380=bb1521[((bbe)bb380
^( *bb40++))&0xff ]^(bb380>>8 );;;bb380=bb1521[((bbe)bb380^( *bb40++))&
0xff ]^(bb380>>8 );;bb380=bb1521[((bbe)bb380^( *bb40++))&0xff ]^(bb380>>
8 );;;;bb380=bb1521[((bbe)bb380^( *bb40++))&0xff ]^(bb380>>8 );;bb380=
bb1521[((bbe)bb380^( *bb40++))&0xff ]^(bb380>>8 );;;bb380=bb1521[((bbe)bb380
^( *bb40++))&0xff ]^(bb380>>8 );;bb380=bb1521[((bbe)bb380^( *bb40++))&
0xff ]^(bb380>>8 );;;;;bb21-=8 ;}bbm(bb21)bb565{bb380=bb1521[((bbe)bb380
^( *bb40++))&0xff ]^(bb380>>8 );;}bb106(--bb21);bb2 bb380^0xffffffffL ;}
