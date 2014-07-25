/*
   'src_compress_deflate_trees.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Mon Oct 29 11:54:27 2012
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
#if ( defined( _WIN32) || defined( __WIN32__)) && ! defined( WIN32)
#define WIN32
#endif
#if defined( __GNUC__) || defined( WIN32) || defined( bb1224) ||  \
defined( bb1198)
#ifndef bb399
#define bb399
#endif
#endif
#if defined( __MSDOS__) && ! defined( bb168)
#define bb168
#endif
#if defined( bb168) && ! defined( bb399)
#define bb508
#endif
#ifdef bb168
#define bb1044
#endif
#if ( defined( bb168) || defined( bb1192) || defined( WIN32)) && !  \
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
#define bb1145
#endif
#ifndef bb210
#ifdef bb508
#define bb210 8
#else
#define bb210 9
#endif
#endif
#ifndef bbp
#ifdef bb136
#define bbp( bb403) bb403
#else
#define bbp( bb403) ()
#endif
#endif
bba bbf bb152;bba bbs bbe bb7;bba bbs bb6 bb24;bba bb152 bb30;bba bbl
bb439;bba bbe bb1098;bba bb7 bb165;bba bb24 bb167;
#ifdef bb136
bba bbb*bb70;bba bbb*bb182;
#else
bba bb152*bb70;bba bb152*bb182;
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bb70( *bb507)bbp((bb70 bb110,bb7 bb495,bb7 bb47));bba bbb( *bb506
)bbp((bb70 bb110,bb70 bb1140));bbi bb381;bba bbi bb1188{bb30*bb126;
bb7 bb148;bb24 bb192;bb30*bb596;bb7 bb384;bb24 bb615;bbl*bb322;bbi
bb381*bb23;bb507 bb401;bb506 bb367;bb70 bb110;bbe bb980;bb24 bb368;
bb24 bb1152;}bb433;bba bb433*bb17;bbr bbh bbl*bb1159 bbp((bbb));bbr
bbe bb513 bbp((bb17 bb16,bbe bb174));bbr bbe bb945 bbp((bb17 bb16));
bbr bbe bb1055 bbp((bb17 bb16,bbe bb174));bbr bbe bb958 bbp((bb17 bb16
));bbr bbe bb1178 bbp((bb17 bb16,bbh bb30*bb424,bb7 bb436));bbr bbe
bb1151 bbp((bb17 bb129,bb17 bb183));bbr bbe bb1059 bbp((bb17 bb16));
bbr bbe bb1177 bbp((bb17 bb16,bbe bb123,bbe bb299));bbr bbe bb1149 bbp
((bb17 bb16,bbh bb30*bb424,bb7 bb436));bbr bbe bb1176 bbp((bb17 bb16));
bbr bbe bb1011 bbp((bb17 bb16));bbr bbe bb1150 bbp((bb30*bb129,bb167*
bb314,bbh bb30*bb183,bb24 bb326));bbr bbe bb1148 bbp((bb30*bb129,
bb167*bb314,bbh bb30*bb183,bb24 bb326,bbe bb123));bbr bbe bb1161 bbp(
(bb30*bb129,bb167*bb314,bbh bb30*bb183,bb24 bb326));bba bb182 bb34;
bbr bb34 bb1206 bbp((bbh bbl*bb1072,bbh bbl*bb57));bbr bb34 bb1207 bbp
((bbe bb471,bbh bbl*bb57));bbr bbe bb1227 bbp((bb34 bb25,bbe bb123,
bbe bb299));bbr bbe bb1193 bbp((bb34 bb25,bb182 bb40,bbs bb22));bbr
bbe bb1189 bbp((bb34 bb25,bbh bb182 bb40,bbs bb22));bbr bbe bb1214 bbp
((bb34 bb25,bbh bbl*bb1228,...));bbr bbe bb1191 bbp((bb34 bb25,bbh bbl
 *bbg));bbr bbl*bb1182 bbp((bb34 bb25,bbl*bb40,bbe bb22));bbr bbe
bb1218 bbp((bb34 bb25,bbe bbo));bbr bbe bb1183 bbp((bb34 bb25));bbr
bbe bb1187 bbp((bb34 bb25,bbe bb174));bbr bb6 bb1195 bbp((bb34 bb25,
bb6 bb92,bbe bb1202));bbr bbe bb1181 bbp((bb34 bb25));bbr bb6 bb1230
bbp((bb34 bb25));bbr bbe bb1197 bbp((bb34 bb25));bbr bbe bb1203 bbp((
bb34 bb25));bbr bbh bbl*bb1186 bbp((bb34 bb25,bbe*bb1180));bbr bb24
bb986 bbp((bb24 bb368,bbh bb30*bb40,bb7 bb22));bbr bb24 bb1167 bbp((
bb24 bb378,bbh bb30*bb40,bb7 bb22));bbr bbe bb1115 bbp((bb17 bb16,bbe
bb123,bbh bbl*bb187,bbe bb197));bbr bbe bb1127 bbp((bb17 bb16,bbh bbl
 *bb187,bbe bb197));bbr bbe bb1050 bbp((bb17 bb16,bbe bb123,bbe bb568
,bbe bb451,bbe bb933,bbe bb299,bbh bbl*bb187,bbe bb197));bbr bbe
bb1082 bbp((bb17 bb16,bbe bb451,bbh bbl*bb187,bbe bb197));bbr bbh bbl
 *bb1170 bbp((bbe bb18));bbr bbe bb1175 bbp((bb17 bby));bbr bbh bb167
 *bb1162 bbp((bbb));
#ifdef __cplusplus
}
#endif
#define bb963 1
#ifdef bb136
#if defined( bb1741)
#else
#endif
#endif
bba bbs bbl bb155;bba bb155 bb1205;bba bbs bb135 bb125;bba bb125 bb501
;bba bbs bb6 bb393;bbr bbh bbl*bb1070[10 ];
#if bb210 >= 8
#define bb803 8
#else
#define bb803 bb210
#endif
#ifdef bb168
#define bb419 0x00
#if defined( __TURBOC__) || defined( __BORLANDC__)
#if( __STDC__ == 1) && ( defined( bb1792) || defined( bb1772))
bbb bb941 bb1318(bbb*bb101);bbb*bb941 bb1344(bbs bb6 bb1740);
#else
#include"uncobf.h"
#include<alloc.h>
#include"cobf.h"
#endif
#else
#include"uncobf.h"
#include<malloc.h>
#include"cobf.h"
#endif
#endif
#ifdef WIN32
#define bb419 0x0b
#endif
#if ( defined( _MSC_VER) && ( _MSC_VER > 600))
#define bb1754( bb471, bb131) bb1788( bb471, bb131)
#endif
#ifndef bb419
#define bb419 0x03
#endif
#if defined( bb1549) && ! defined( _MSC_VER) && ! defined( bb1775)
#define bb963
#endif
bba bb24( *bb954)bbp((bb24 bb473,bbh bb30*bb40,bb7 bb22));bb70 bb1168
bbp((bb70 bb110,bbs bb495,bbs bb47));bbb bb1163 bbp((bb70 bb110,bb70
bb914));bba bbi bb1954{bb550{bb125 bb428;bb125 bb172;}bb248;bb550{
bb125 bb2163;bb125 bb22;}bb48;}bb458;bba bbi bb2278 bb2005;bba bbi
bb1939{bb458*bb1749;bbe bb502;bb2005*bb1719;}bb1734;bba bb125 bb1039;
bba bb1039 bb1357;bba bbs bb1321;bba bbi bb381{bb17 bb16;bbe bb360;
bb30*bb171;bb393 bb2039;bb30*bb1874;bbe bb186;bbe bb1352;bb152 bb980;
bb152 bb568;bbe bb1894;bb7 bb935;bb7 bb2162;bb7 bb1808;bb30*bb158;
bb393 bb2227;bb1357*bb968;bb1357*bb383;bb7 bb499;bb7 bb1328;bb7 bb2129
;bb7 bb1381;bb7 bb1610;bb6 bb429;bb7 bb951;bb1321 bb2299;bbe bb1940;
bb7 bb185;bb7 bb2023;bb7 bb463;bb7 bb1274;bb7 bb2175;bb7 bb2055;bbe
bb123;bbe bb299;bb7 bb2180;bbe bb1838;bbi bb1954 bb971[(2 * (256 +1 +29 )+
1 )];bbi bb1954 bb1680[2 *30 +1 ];bbi bb1954 bb530[2 *19 +1 ];bbi bb1939
bb1936;bbi bb1939 bb1862;bbi bb1939 bb2062;bb125 bb1194[15 +1 ];bbe
bb514[2 * (256 +1 +29 )+1 ];bbe bb1503;bbe bb1945;bb155 bb1239[2 * (256 +1 +
29 )+1 ];bb1205*bb1722;bb7 bb1126;bb7 bb605;bb501*bb1641;bb393 bb1905;
bb393 bb2118;bb7 bb2231;bbe bb1993;bb125 bb100;bbe bb79;}bb190;bbb
bb2209 bbp((bb190*bbg));bbe bb2389 bbp((bb190*bbg,bbs bb415,bbs bb1110
));bbb bb1616 bbp((bb190*bbg,bb439*bb40,bb393 bb1290,bbe bb1113));bbb
bb2255 bbp((bb190*bbg));bbb bb2142 bbp((bb190*bbg,bb439*bb40,bb393
bb1290,bbe bb1113));bb41 bbh bbe bb2415[29 ]={0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,1 ,1 ,1 ,
2 ,2 ,2 ,2 ,3 ,3 ,3 ,3 ,4 ,4 ,4 ,4 ,5 ,5 ,5 ,5 ,0 };bb41 bbh bbe bb2379[30 ]={0 ,0 ,0 ,0 ,1
,1 ,2 ,2 ,3 ,3 ,4 ,4 ,5 ,5 ,6 ,6 ,7 ,7 ,8 ,8 ,9 ,9 ,10 ,10 ,11 ,11 ,12 ,12 ,13 ,13 };bb41 bbh
bbe bb2479[19 ]={0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,2 ,3 ,7 };bb41 bbh bb155
bb2197[19 ]={16 ,17 ,18 ,0 ,8 ,7 ,9 ,6 ,10 ,5 ,11 ,4 ,12 ,3 ,13 ,2 ,14 ,1 ,15 };bb41 bbh
bb458 bb1738[(256 +1 +29 )+2 ]={{{12 },{8 }},{{140 },{8 }},{{76 },{8 }},{{204 },
{8 }},{{44 },{8 }},{{172 },{8 }},{{108 },{8 }},{{236 },{8 }},{{28 },{8 }},{{156 }
,{8 }},{{92 },{8 }},{{220 },{8 }},{{60 },{8 }},{{188 },{8 }},{{124 },{8 }},{{252
},{8 }},{{2 },{8 }},{{130 },{8 }},{{66 },{8 }},{{194 },{8 }},{{34 },{8 }},{{162 }
,{8 }},{{98 },{8 }},{{226 },{8 }},{{18 },{8 }},{{146 },{8 }},{{82 },{8 }},{{210 }
,{8 }},{{50 },{8 }},{{178 },{8 }},{{114 },{8 }},{{242 },{8 }},{{10 },{8 }},{{138
},{8 }},{{74 },{8 }},{{202 },{8 }},{{42 },{8 }},{{170 },{8 }},{{106 },{8 }},{{
234 },{8 }},{{26 },{8 }},{{154 },{8 }},{{90 },{8 }},{{218 },{8 }},{{58 },{8 }},{{
186 },{8 }},{{122 },{8 }},{{250 },{8 }},{{6 },{8 }},{{134 },{8 }},{{70 },{8 }},{{
198 },{8 }},{{38 },{8 }},{{166 },{8 }},{{102 },{8 }},{{230 },{8 }},{{22 },{8 }},{
{150 },{8 }},{{86 },{8 }},{{214 },{8 }},{{54 },{8 }},{{182 },{8 }},{{118 },{8 }},
{{246 },{8 }},{{14 },{8 }},{{142 },{8 }},{{78 },{8 }},{{206 },{8 }},{{46 },{8 }},
{{174 },{8 }},{{110 },{8 }},{{238 },{8 }},{{30 },{8 }},{{158 },{8 }},{{94 },{8 }}
,{{222 },{8 }},{{62 },{8 }},{{190 },{8 }},{{126 },{8 }},{{254 },{8 }},{{1 },{8 }}
,{{129 },{8 }},{{65 },{8 }},{{193 },{8 }},{{33 },{8 }},{{161 },{8 }},{{97 },{8 }}
,{{225 },{8 }},{{17 },{8 }},{{145 },{8 }},{{81 },{8 }},{{209 },{8 }},{{49 },{8 }}
,{{177 },{8 }},{{113 },{8 }},{{241 },{8 }},{{9 },{8 }},{{137 },{8 }},{{73 },{8 }}
,{{201 },{8 }},{{41 },{8 }},{{169 },{8 }},{{105 },{8 }},{{233 },{8 }},{{25 },{8 }
},{{153 },{8 }},{{89 },{8 }},{{217 },{8 }},{{57 },{8 }},{{185 },{8 }},{{121 },{8
}},{{249 },{8 }},{{5 },{8 }},{{133 },{8 }},{{69 },{8 }},{{197 },{8 }},{{37 },{8 }
},{{165 },{8 }},{{101 },{8 }},{{229 },{8 }},{{21 },{8 }},{{149 },{8 }},{{85 },{8
}},{{213 },{8 }},{{53 },{8 }},{{181 },{8 }},{{117 },{8 }},{{245 },{8 }},{{13 },{
8 }},{{141 },{8 }},{{77 },{8 }},{{205 },{8 }},{{45 },{8 }},{{173 },{8 }},{{109 },
{8 }},{{237 },{8 }},{{29 },{8 }},{{157 },{8 }},{{93 },{8 }},{{221 },{8 }},{{61 },
{8 }},{{189 },{8 }},{{125 },{8 }},{{253 },{8 }},{{19 },{9 }},{{275 },{9 }},{{147
},{9 }},{{403 },{9 }},{{83 },{9 }},{{339 },{9 }},{{211 },{9 }},{{467 },{9 }},{{
51 },{9 }},{{307 },{9 }},{{179 },{9 }},{{435 },{9 }},{{115 },{9 }},{{371 },{9 }},
{{243 },{9 }},{{499 },{9 }},{{11 },{9 }},{{267 },{9 }},{{139 },{9 }},{{395 },{9 }
},{{75 },{9 }},{{331 },{9 }},{{203 },{9 }},{{459 },{9 }},{{43 },{9 }},{{299 },{9
}},{{171 },{9 }},{{427 },{9 }},{{107 },{9 }},{{363 },{9 }},{{235 },{9 }},{{491 }
,{9 }},{{27 },{9 }},{{283 },{9 }},{{155 },{9 }},{{411 },{9 }},{{91 },{9 }},{{347
},{9 }},{{219 },{9 }},{{475 },{9 }},{{59 },{9 }},{{315 },{9 }},{{187 },{9 }},{{
443 },{9 }},{{123 },{9 }},{{379 },{9 }},{{251 },{9 }},{{507 },{9 }},{{7 },{9 }},{
{263 },{9 }},{{135 },{9 }},{{391 },{9 }},{{71 },{9 }},{{327 },{9 }},{{199 },{9 }}
,{{455 },{9 }},{{39 },{9 }},{{295 },{9 }},{{167 },{9 }},{{423 },{9 }},{{103 },{9
}},{{359 },{9 }},{{231 },{9 }},{{487 },{9 }},{{23 },{9 }},{{279 },{9 }},{{151 },
{9 }},{{407 },{9 }},{{87 },{9 }},{{343 },{9 }},{{215 },{9 }},{{471 },{9 }},{{55 }
,{9 }},{{311 },{9 }},{{183 },{9 }},{{439 },{9 }},{{119 },{9 }},{{375 },{9 }},{{
247 },{9 }},{{503 },{9 }},{{15 },{9 }},{{271 },{9 }},{{143 },{9 }},{{399 },{9 }},
{{79 },{9 }},{{335 },{9 }},{{207 },{9 }},{{463 },{9 }},{{47 },{9 }},{{303 },{9 }}
,{{175 },{9 }},{{431 },{9 }},{{111 },{9 }},{{367 },{9 }},{{239 },{9 }},{{495 },{
9 }},{{31 },{9 }},{{287 },{9 }},{{159 },{9 }},{{415 },{9 }},{{95 },{9 }},{{351 },
{9 }},{{223 },{9 }},{{479 },{9 }},{{63 },{9 }},{{319 },{9 }},{{191 },{9 }},{{447
},{9 }},{{127 },{9 }},{{383 },{9 }},{{255 },{9 }},{{511 },{9 }},{{0 },{7 }},{{64
},{7 }},{{32 },{7 }},{{96 },{7 }},{{16 },{7 }},{{80 },{7 }},{{48 },{7 }},{{112 },
{7 }},{{8 },{7 }},{{72 },{7 }},{{40 },{7 }},{{104 },{7 }},{{24 },{7 }},{{88 },{7 }
},{{56 },{7 }},{{120 },{7 }},{{4 },{7 }},{{68 },{7 }},{{36 },{7 }},{{100 },{7 }},
{{20 },{7 }},{{84 },{7 }},{{52 },{7 }},{{116 },{7 }},{{3 },{8 }},{{131 },{8 }},{{
67 },{8 }},{{195 },{8 }},{{35 },{8 }},{{163 },{8 }},{{99 },{8 }},{{227 },{8 }}};
bb41 bbh bb458 bb2235[30 ]={{{0 },{5 }},{{16 },{5 }},{{8 },{5 }},{{24 },{5 }},
{{4 },{5 }},{{20 },{5 }},{{12 },{5 }},{{28 },{5 }},{{2 },{5 }},{{18 },{5 }},{{10 }
,{5 }},{{26 },{5 }},{{6 },{5 }},{{22 },{5 }},{{14 },{5 }},{{30 },{5 }},{{1 },{5 }}
,{{17 },{5 }},{{9 },{5 }},{{25 },{5 }},{{5 },{5 }},{{21 },{5 }},{{13 },{5 }},{{29
},{5 }},{{3 },{5 }},{{19 },{5 }},{{11 },{5 }},{{27 },{5 }},{{7 },{5 }},{{23 },{5 }
}};bb41 bbh bb155 bb1735[512 ]={0 ,1 ,2 ,3 ,4 ,4 ,5 ,5 ,6 ,6 ,6 ,6 ,7 ,7 ,7 ,7 ,8 ,8 ,8 ,
8 ,8 ,8 ,8 ,8 ,9 ,9 ,9 ,9 ,9 ,9 ,9 ,9 ,10 ,10 ,10 ,10 ,10 ,10 ,10 ,10 ,10 ,10 ,10 ,10 ,10 ,10 ,
10 ,10 ,11 ,11 ,11 ,11 ,11 ,11 ,11 ,11 ,11 ,11 ,11 ,11 ,11 ,11 ,11 ,11 ,12 ,12 ,12 ,12 ,12 ,
12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,
12 ,12 ,12 ,12 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,
13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,
14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,
14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,
14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,
15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,
15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,
15 ,15 ,15 ,0 ,0 ,16 ,17 ,18 ,18 ,19 ,19 ,20 ,20 ,20 ,20 ,21 ,21 ,21 ,21 ,22 ,22 ,22 ,22 ,22
,22 ,22 ,22 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24
,24 ,24 ,24 ,24 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,26 ,26 ,26
,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26
,26 ,26 ,26 ,26 ,26 ,26 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27
,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28
,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28
,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28
,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29
,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29
,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29
,29 ,29 ,29 ,29 ,29 };bb41 bbh bb155 bb2051[258 -3 +1 ]={0 ,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,8 ,
9 ,9 ,10 ,10 ,11 ,11 ,12 ,12 ,12 ,12 ,13 ,13 ,13 ,13 ,14 ,14 ,14 ,14 ,15 ,15 ,15 ,15 ,16 ,16
,16 ,16 ,16 ,16 ,16 ,16 ,17 ,17 ,17 ,17 ,17 ,17 ,17 ,17 ,18 ,18 ,18 ,18 ,18 ,18 ,18 ,18 ,19
,19 ,19 ,19 ,19 ,19 ,19 ,19 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,20
,21 ,21 ,21 ,21 ,21 ,21 ,21 ,21 ,21 ,21 ,21 ,21 ,21 ,21 ,21 ,21 ,22 ,22 ,22 ,22 ,22 ,22 ,22
,22 ,22 ,22 ,22 ,22 ,22 ,22 ,22 ,22 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23
,23 ,23 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24
,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25
,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,26 ,26 ,26
,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26
,26 ,26 ,26 ,26 ,26 ,26 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27
,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,28 };bb41 bbh bbe bb2334[29
]={0 ,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,10 ,12 ,14 ,16 ,20 ,24 ,28 ,32 ,40 ,48 ,56 ,64 ,80 ,96 ,112 ,
128 ,160 ,192 ,224 ,0 };bb41 bbh bbe bb2417[30 ]={0 ,1 ,2 ,3 ,4 ,6 ,8 ,12 ,16 ,24 ,32
,48 ,64 ,96 ,128 ,192 ,256 ,384 ,512 ,768 ,1024 ,1536 ,2048 ,3072 ,4096 ,6144 ,8192 ,
12288 ,16384 ,24576 };bbi bb2278{bbh bb458*bb2345;bbh bb1098*bb2472;bbe
bb2453;bbe bb2174;bbe bb1941;};bb41 bb2005 bb2491={bb1738,bb2415,256 +
1 ,(256 +1 +29 ),15 };bb41 bb2005 bb2557={bb2235,bb2379,0 ,30 ,15 };bb41
bb2005 bb2497={(bbh bb458* )0 ,bb2479,0 ,19 ,7 };bb41 bbb bb2245 bbp((
bb190*bbg));bb41 bbb bb2167 bbp((bb190*bbg,bb458*bb303,bbe bb3));bb41
bbb bb2367 bbp((bb190*bbg,bb1734*bb1057));bb41 bbb bb2394 bbp((bb458*
bb303,bbe bb502,bb501*bb1194));bb41 bbb bb2184 bbp((bb190*bbg,bb1734*
bb1057));bb41 bbb bb2307 bbp((bb190*bbg,bb458*bb303,bbe bb502));bb41
bbb bb2232 bbp((bb190*bbg,bb458*bb303,bbe bb502));bb41 bbe bb2385 bbp
((bb190*bbg));bb41 bbb bb2409 bbp((bb190*bbg,bbe bb2052,bbe bb2092,
bbe bb2045));bb41 bbb bb2271 bbp((bb190*bbg,bb458*bb1058,bb458*bb1747
));bb41 bbb bb2314 bbp((bb190*bbg));bb41 bbs bb2370 bbp((bbs bb1326,
bbe bb470));bb41 bbb bb2259 bbp((bb190*bbg));bb41 bbb bb2304 bbp((
bb190*bbg));bb41 bbb bb2319 bbp((bb190*bbg,bb439*bb40,bbs bb22,bbe
bb985));bbb bb2209(bbg)bb190*bbg;{bbg->bb1936.bb1749=bbg->bb971;bbg->
bb1936.bb1719=&bb2491;bbg->bb1862.bb1749=bbg->bb1680;bbg->bb1862.
bb1719=&bb2557;bbg->bb2062.bb1749=bbg->bb530;bbg->bb2062.bb1719=&
bb2497;bbg->bb100=0 ;bbg->bb79=0 ;bbg->bb1993=8 ;bb2245(bbg);}bb41 bbb
bb2245(bbg)bb190*bbg;{bbe bb11;bb90(bb11=0 ;bb11<(256 +1 +29 );bb11++)bbg
->bb971[bb11].bb248.bb428=0 ;bb90(bb11=0 ;bb11<30 ;bb11++)bbg->bb1680[
bb11].bb248.bb428=0 ;bb90(bb11=0 ;bb11<19 ;bb11++)bbg->bb530[bb11].bb248
.bb428=0 ;bbg->bb971[256 ].bb248.bb428=1 ;bbg->bb1905=bbg->bb2118=0L ;bbg
->bb605=bbg->bb2231=0 ;}bb41 bbb bb2167(bbg,bb303,bb3)bb190*bbg;bb458*
bb303;bbe bb3;{bbe bb441=bbg->bb514[bb3];bbe bb76=bb3<<1 ;bb106(bb76<=
bbg->bb1503){bbm(bb76<bbg->bb1503&&(bb303[bbg->bb514[bb76+1 ]].bb248.
bb428<bb303[bbg->bb514[bb76]].bb248.bb428||(bb303[bbg->bb514[bb76+1 ]]
.bb248.bb428==bb303[bbg->bb514[bb76]].bb248.bb428&&bbg->bb1239[bbg->
bb514[bb76+1 ]]<=bbg->bb1239[bbg->bb514[bb76]]))){bb76++;}bbm((bb303[
bb441].bb248.bb428<bb303[bbg->bb514[bb76]].bb248.bb428||(bb303[bb441]
.bb248.bb428==bb303[bbg->bb514[bb76]].bb248.bb428&&bbg->bb1239[bb441]
<=bbg->bb1239[bbg->bb514[bb76]])))bb21;bbg->bb514[bb3]=bbg->bb514[
bb76];bb3=bb76;bb76<<=1 ;}bbg->bb514[bb3]=bb441;}bb41 bbb bb2367(bbg,
bb1057)bb190*bbg;bb1734*bb1057;{bb458*bb303=bb1057->bb1749;bbe bb502=
bb1057->bb502;bbh bb458*bb2115=bb1057->bb1719->bb2345;bbh bb1098*
bb1800=bb1057->bb1719->bb2472;bbe bb610=bb1057->bb1719->bb2453;bbe
bb1941=bb1057->bb1719->bb1941;bbe bb42;bbe bb11,bb95;bbe bb516;bbe
bb2143;bb125 bb19;bbe bb2137=0 ;bb90(bb516=0 ;bb516<=15 ;bb516++)bbg->
bb1194[bb516]=0 ;bb303[bbg->bb514[bbg->bb1945]].bb48.bb22=0 ;bb90(bb42=
bbg->bb1945+1 ;bb42<(2 * (256 +1 +29 )+1 );bb42++){bb11=bbg->bb514[bb42];
bb516=bb303[bb303[bb11].bb48.bb2163].bb48.bb22+1 ;bbm(bb516>bb1941)bb516
=bb1941,bb2137++;bb303[bb11].bb48.bb22=(bb125)bb516;bbm(bb11>bb502)bb1684
;bbg->bb1194[bb516]++;bb2143=0 ;bbm(bb11>=bb610)bb2143=bb1800[bb11-
bb610];bb19=bb303[bb11].bb248.bb428;bbg->bb1905+=(bb393)bb19* (bb516+
bb2143);bbm(bb2115)bbg->bb2118+=(bb393)bb19* (bb2115[bb11].bb48.bb22+
bb2143);}bbm(bb2137==0 )bb2;;bb573{bb516=bb1941-1 ;bb106(bbg->bb1194[
bb516]==0 )bb516--;bbg->bb1194[bb516]--;bbg->bb1194[bb516+1 ]+=2 ;bbg->
bb1194[bb1941]--;bb2137-=2 ;}bb106(bb2137>0 );bb90(bb516=bb1941;bb516!=
0 ;bb516--){bb11=bbg->bb1194[bb516];bb106(bb11!=0 ){bb95=bbg->bb514[--
bb42];bbm(bb95>bb502)bb1684;bbm(bb303[bb95].bb48.bb22!=(bbs)bb516){;
bbg->bb1905+=((bb6)bb516-(bb6)bb303[bb95].bb48.bb22) * (bb6)bb303[
bb95].bb248.bb428;bb303[bb95].bb48.bb22=(bb125)bb516;}bb11--;}}}bb41
bbb bb2394(bb303,bb502,bb1194)bb458*bb303;bbe bb502;bb501*bb1194;{
bb125 bb2404[15 +1 ];bb125 bb172=0 ;bbe bb516;bbe bb11;bb90(bb516=1 ;
bb516<=15 ;bb516++){bb2404[bb516]=bb172=(bb172+bb1194[bb516-1 ])<<1 ;};;
bb90(bb11=0 ;bb11<=bb502;bb11++){bbe bb22=bb303[bb11].bb48.bb22;bbm(
bb22==0 )bb1684;bb303[bb11].bb248.bb172=bb2370(bb2404[bb22]++,bb22);;}
}bb41 bbb bb2184(bbg,bb1057)bb190*bbg;bb1734*bb1057;{bb458*bb303=
bb1057->bb1749;bbh bb458*bb2115=bb1057->bb1719->bb2345;bbe bb2174=
bb1057->bb1719->bb2174;bbe bb11,bb95;bbe bb502=-1 ;bbe bb1777;bbg->
bb1503=0 ,bbg->bb1945=(2 * (256 +1 +29 )+1 );bb90(bb11=0 ;bb11<bb2174;bb11++
){bbm(bb303[bb11].bb248.bb428!=0 ){bbg->bb514[++(bbg->bb1503)]=bb502=
bb11;bbg->bb1239[bb11]=0 ;}bb54{bb303[bb11].bb48.bb22=0 ;}}bb106(bbg->
bb1503<2 ){bb1777=bbg->bb514[++(bbg->bb1503)]=(bb502<2 ?++bb502:0 );
bb303[bb1777].bb248.bb428=1 ;bbg->bb1239[bb1777]=0 ;bbg->bb1905--;bbm(
bb2115)bbg->bb2118-=bb2115[bb1777].bb48.bb22;}bb1057->bb502=bb502;
bb90(bb11=bbg->bb1503/2 ;bb11>=1 ;bb11--)bb2167(bbg,bb303,bb11);bb1777=
bb2174;bb573{{bb11=bbg->bb514[1 ];bbg->bb514[1 ]=bbg->bb514[bbg->bb1503
--];bb2167(bbg,bb303,1 );};bb95=bbg->bb514[1 ];bbg->bb514[--(bbg->
bb1945)]=bb11;bbg->bb514[--(bbg->bb1945)]=bb95;bb303[bb1777].bb248.
bb428=bb303[bb11].bb248.bb428+bb303[bb95].bb248.bb428;bbg->bb1239[
bb1777]=(bb155)((bbg->bb1239[bb11]>=bbg->bb1239[bb95]?bbg->bb1239[
bb11]:bbg->bb1239[bb95])+1 );bb303[bb11].bb48.bb2163=bb303[bb95].bb48.
bb2163=(bb125)bb1777;bbg->bb514[1 ]=bb1777++;bb2167(bbg,bb303,1 );}
bb106(bbg->bb1503>=2 );bbg->bb514[--(bbg->bb1945)]=bbg->bb514[1 ];
bb2367(bbg,(bb1734* )bb1057);bb2394((bb458* )bb303,bb502,bbg->bb1194);
}bb41 bbb bb2307(bbg,bb303,bb502)bb190*bbg;bb458*bb303;bbe bb502;{bbe
bb11;bbe bb2084=-1 ;bbe bb866;bbe bb1225=bb303[0 ].bb48.bb22;bbe bb972=
0 ;bbe bb1341=7 ;bbe bb1301=4 ;bbm(bb1225==0 )bb1341=138 ,bb1301=3 ;bb303[
bb502+1 ].bb48.bb22=(bb125)0xffff ;bb90(bb11=0 ;bb11<=bb502;bb11++){
bb866=bb1225;bb1225=bb303[bb11+1 ].bb48.bb22;bbm(++bb972<bb1341&&bb866
==bb1225){bb1684;}bb54 bbm(bb972<bb1301){bbg->bb530[bb866].bb248.
bb428+=bb972;}bb54 bbm(bb866!=0 ){bbm(bb866!=bb2084)bbg->bb530[bb866].
bb248.bb428++;bbg->bb530[16 ].bb248.bb428++;}bb54 bbm(bb972<=10 ){bbg->
bb530[17 ].bb248.bb428++;}bb54{bbg->bb530[18 ].bb248.bb428++;}bb972=0 ;
bb2084=bb866;bbm(bb1225==0 ){bb1341=138 ,bb1301=3 ;}bb54 bbm(bb866==
bb1225){bb1341=6 ,bb1301=3 ;}bb54{bb1341=7 ,bb1301=4 ;}}}bb41 bbb bb2232(
bbg,bb303,bb502)bb190*bbg;bb458*bb303;bbe bb502;{bbe bb11;bbe bb2084=
-1 ;bbe bb866;bbe bb1225=bb303[0 ].bb48.bb22;bbe bb972=0 ;bbe bb1341=7 ;
bbe bb1301=4 ;bbm(bb1225==0 )bb1341=138 ,bb1301=3 ;bb90(bb11=0 ;bb11<=
bb502;bb11++){bb866=bb1225;bb1225=bb303[bb11+1 ].bb48.bb22;bbm(++bb972
<bb1341&&bb866==bb1225){bb1684;}bb54 bbm(bb972<bb1301){bb573{{bbe bb22
=bbg->bb530[bb866].bb48.bb22;bbm(bbg->bb79>(bbe)(8 *2 *bb12(bbl))-bb22){
bbe bb170=bbg->bb530[bb866].bb248.bb172;bbg->bb100|=(bb170<<bbg->bb79
);{{bbg->bb171[bbg->bb186++]=((bb155)((bbg->bb100)&0xff ));};{bbg->
bb171[bbg->bb186++]=((bb155)((bb125)(bbg->bb100)>>8 ));};};bbg->bb100=
(bb125)bb170>>((bbe)(8 *2 *bb12(bbl))-bbg->bb79);bbg->bb79+=bb22-(8 *2 *
bb12(bbl));}bb54{bbg->bb100|=(bbg->bb530[bb866].bb248.bb172)<<bbg->
bb79;bbg->bb79+=bb22;}};}bb106(--bb972!=0 );}bb54 bbm(bb866!=0 ){bbm(
bb866!=bb2084){{bbe bb22=bbg->bb530[bb866].bb48.bb22;bbm(bbg->bb79>(
bbe)(8 *2 *bb12(bbl))-bb22){bbe bb170=bbg->bb530[bb866].bb248.bb172;bbg
->bb100|=(bb170<<bbg->bb79);{{bbg->bb171[bbg->bb186++]=((bb155)((bbg
->bb100)&0xff ));};{bbg->bb171[bbg->bb186++]=((bb155)((bb125)(bbg->
bb100)>>8 ));};};bbg->bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(bbl))-bbg->
bb79);bbg->bb79+=bb22-(8 *2 *bb12(bbl));}bb54{bbg->bb100|=(bbg->bb530[
bb866].bb248.bb172)<<bbg->bb79;bbg->bb79+=bb22;}};bb972--;};{bbe bb22
=bbg->bb530[16 ].bb48.bb22;bbm(bbg->bb79>(bbe)(8 *2 *bb12(bbl))-bb22){
bbe bb170=bbg->bb530[16 ].bb248.bb172;bbg->bb100|=(bb170<<bbg->bb79);{
{bbg->bb171[bbg->bb186++]=((bb155)((bbg->bb100)&0xff ));};{bbg->bb171[
bbg->bb186++]=((bb155)((bb125)(bbg->bb100)>>8 ));};};bbg->bb100=(bb125
)bb170>>((bbe)(8 *2 *bb12(bbl))-bbg->bb79);bbg->bb79+=bb22-(8 *2 *bb12(
bbl));}bb54{bbg->bb100|=(bbg->bb530[16 ].bb248.bb172)<<bbg->bb79;bbg->
bb79+=bb22;}};{bbe bb22=2 ;bbm(bbg->bb79>(bbe)(8 *2 *bb12(bbl))-bb22){
bbe bb170=bb972-3 ;bbg->bb100|=(bb170<<bbg->bb79);{{bbg->bb171[bbg->
bb186++]=((bb155)((bbg->bb100)&0xff ));};{bbg->bb171[bbg->bb186++]=((
bb155)((bb125)(bbg->bb100)>>8 ));};};bbg->bb100=(bb125)bb170>>((bbe)(8
 *2 *bb12(bbl))-bbg->bb79);bbg->bb79+=bb22-(8 *2 *bb12(bbl));}bb54{bbg->
bb100|=(bb972-3 )<<bbg->bb79;bbg->bb79+=bb22;}};}bb54 bbm(bb972<=10 ){{
bbe bb22=bbg->bb530[17 ].bb48.bb22;bbm(bbg->bb79>(bbe)(8 *2 *bb12(bbl))-
bb22){bbe bb170=bbg->bb530[17 ].bb248.bb172;bbg->bb100|=(bb170<<bbg->
bb79);{{bbg->bb171[bbg->bb186++]=((bb155)((bbg->bb100)&0xff ));};{bbg
->bb171[bbg->bb186++]=((bb155)((bb125)(bbg->bb100)>>8 ));};};bbg->
bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(bbl))-bbg->bb79);bbg->bb79+=bb22-
(8 *2 *bb12(bbl));}bb54{bbg->bb100|=(bbg->bb530[17 ].bb248.bb172)<<bbg->
bb79;bbg->bb79+=bb22;}};{bbe bb22=3 ;bbm(bbg->bb79>(bbe)(8 *2 *bb12(bbl))-
bb22){bbe bb170=bb972-3 ;bbg->bb100|=(bb170<<bbg->bb79);{{bbg->bb171[
bbg->bb186++]=((bb155)((bbg->bb100)&0xff ));};{bbg->bb171[bbg->bb186++
]=((bb155)((bb125)(bbg->bb100)>>8 ));};};bbg->bb100=(bb125)bb170>>((
bbe)(8 *2 *bb12(bbl))-bbg->bb79);bbg->bb79+=bb22-(8 *2 *bb12(bbl));}bb54{
bbg->bb100|=(bb972-3 )<<bbg->bb79;bbg->bb79+=bb22;}};}bb54{{bbe bb22=
bbg->bb530[18 ].bb48.bb22;bbm(bbg->bb79>(bbe)(8 *2 *bb12(bbl))-bb22){bbe
bb170=bbg->bb530[18 ].bb248.bb172;bbg->bb100|=(bb170<<bbg->bb79);{{bbg
->bb171[bbg->bb186++]=((bb155)((bbg->bb100)&0xff ));};{bbg->bb171[bbg
->bb186++]=((bb155)((bb125)(bbg->bb100)>>8 ));};};bbg->bb100=(bb125)bb170
>>((bbe)(8 *2 *bb12(bbl))-bbg->bb79);bbg->bb79+=bb22-(8 *2 *bb12(bbl));}
bb54{bbg->bb100|=(bbg->bb530[18 ].bb248.bb172)<<bbg->bb79;bbg->bb79+=
bb22;}};{bbe bb22=7 ;bbm(bbg->bb79>(bbe)(8 *2 *bb12(bbl))-bb22){bbe bb170
=bb972-11 ;bbg->bb100|=(bb170<<bbg->bb79);{{bbg->bb171[bbg->bb186++]=(
(bb155)((bbg->bb100)&0xff ));};{bbg->bb171[bbg->bb186++]=((bb155)((
bb125)(bbg->bb100)>>8 ));};};bbg->bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(
bbl))-bbg->bb79);bbg->bb79+=bb22-(8 *2 *bb12(bbl));}bb54{bbg->bb100|=(
bb972-11 )<<bbg->bb79;bbg->bb79+=bb22;}};}bb972=0 ;bb2084=bb866;bbm(
bb1225==0 ){bb1341=138 ,bb1301=3 ;}bb54 bbm(bb866==bb1225){bb1341=6 ,
bb1301=3 ;}bb54{bb1341=7 ,bb1301=4 ;}}}bb41 bbe bb2385(bbg)bb190*bbg;{
bbe bb1784;bb2307(bbg,(bb458* )bbg->bb971,bbg->bb1936.bb502);bb2307(
bbg,(bb458* )bbg->bb1680,bbg->bb1862.bb502);bb2184(bbg,(bb1734* )(&(
bbg->bb2062)));bb90(bb1784=19 -1 ;bb1784>=3 ;bb1784--){bbm(bbg->bb530[
bb2197[bb1784]].bb48.bb22!=0 )bb21;}bbg->bb1905+=3 * (bb1784+1 )+5 +5 +4 ;;
bb2 bb1784;}bb41 bbb bb2409(bbg,bb2052,bb2092,bb2045)bb190*bbg;bbe
bb2052,bb2092,bb2045;{bbe bb2121;;;;{bbe bb22=5 ;bbm(bbg->bb79>(bbe)(8
 *2 *bb12(bbl))-bb22){bbe bb170=bb2052-257 ;bbg->bb100|=(bb170<<bbg->
bb79);{{bbg->bb171[bbg->bb186++]=((bb155)((bbg->bb100)&0xff ));};{bbg
->bb171[bbg->bb186++]=((bb155)((bb125)(bbg->bb100)>>8 ));};};bbg->
bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(bbl))-bbg->bb79);bbg->bb79+=bb22-
(8 *2 *bb12(bbl));}bb54{bbg->bb100|=(bb2052-257 )<<bbg->bb79;bbg->bb79+=
bb22;}};{bbe bb22=5 ;bbm(bbg->bb79>(bbe)(8 *2 *bb12(bbl))-bb22){bbe bb170
=bb2092-1 ;bbg->bb100|=(bb170<<bbg->bb79);{{bbg->bb171[bbg->bb186++]=(
(bb155)((bbg->bb100)&0xff ));};{bbg->bb171[bbg->bb186++]=((bb155)((
bb125)(bbg->bb100)>>8 ));};};bbg->bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(
bbl))-bbg->bb79);bbg->bb79+=bb22-(8 *2 *bb12(bbl));}bb54{bbg->bb100|=(
bb2092-1 )<<bbg->bb79;bbg->bb79+=bb22;}};{bbe bb22=4 ;bbm(bbg->bb79>(
bbe)(8 *2 *bb12(bbl))-bb22){bbe bb170=bb2045-4 ;bbg->bb100|=(bb170<<bbg
->bb79);{{bbg->bb171[bbg->bb186++]=((bb155)((bbg->bb100)&0xff ));};{
bbg->bb171[bbg->bb186++]=((bb155)((bb125)(bbg->bb100)>>8 ));};};bbg->
bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(bbl))-bbg->bb79);bbg->bb79+=bb22-
(8 *2 *bb12(bbl));}bb54{bbg->bb100|=(bb2045-4 )<<bbg->bb79;bbg->bb79+=
bb22;}};bb90(bb2121=0 ;bb2121<bb2045;bb2121++){;{bbe bb22=3 ;bbm(bbg->
bb79>(bbe)(8 *2 *bb12(bbl))-bb22){bbe bb170=bbg->bb530[bb2197[bb2121]].
bb48.bb22;bbg->bb100|=(bb170<<bbg->bb79);{{bbg->bb171[bbg->bb186++]=(
(bb155)((bbg->bb100)&0xff ));};{bbg->bb171[bbg->bb186++]=((bb155)((
bb125)(bbg->bb100)>>8 ));};};bbg->bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(
bbl))-bbg->bb79);bbg->bb79+=bb22-(8 *2 *bb12(bbl));}bb54{bbg->bb100|=(
bbg->bb530[bb2197[bb2121]].bb48.bb22)<<bbg->bb79;bbg->bb79+=bb22;}};}
;bb2232(bbg,(bb458* )bbg->bb971,bb2052-1 );;bb2232(bbg,(bb458* )bbg->
bb1680,bb2092-1 );;}bbb bb2142(bbg,bb40,bb1290,bb1113)bb190*bbg;bb439*
bb40;bb393 bb1290;bbe bb1113;{{bbe bb22=3 ;bbm(bbg->bb79>(bbe)(8 *2 *
bb12(bbl))-bb22){bbe bb170=(0 <<1 )+bb1113;bbg->bb100|=(bb170<<bbg->
bb79);{{bbg->bb171[bbg->bb186++]=((bb155)((bbg->bb100)&0xff ));};{bbg
->bb171[bbg->bb186++]=((bb155)((bb125)(bbg->bb100)>>8 ));};};bbg->
bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(bbl))-bbg->bb79);bbg->bb79+=bb22-
(8 *2 *bb12(bbl));}bb54{bbg->bb100|=((0 <<1 )+bb1113)<<bbg->bb79;bbg->
bb79+=bb22;}};bb2319(bbg,bb40,(bbs)bb1290,1 );}bbb bb2255(bbg)bb190*
bbg;{{bbe bb22=3 ;bbm(bbg->bb79>(bbe)(8 *2 *bb12(bbl))-bb22){bbe bb170=1
<<1 ;bbg->bb100|=(bb170<<bbg->bb79);{{bbg->bb171[bbg->bb186++]=((bb155
)((bbg->bb100)&0xff ));};{bbg->bb171[bbg->bb186++]=((bb155)((bb125)(
bbg->bb100)>>8 ));};};bbg->bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(bbl))-
bbg->bb79);bbg->bb79+=bb22-(8 *2 *bb12(bbl));}bb54{bbg->bb100|=(1 <<1 )<<
bbg->bb79;bbg->bb79+=bb22;}};{bbe bb22=bb1738[256 ].bb48.bb22;bbm(bbg
->bb79>(bbe)(8 *2 *bb12(bbl))-bb22){bbe bb170=bb1738[256 ].bb248.bb172;
bbg->bb100|=(bb170<<bbg->bb79);{{bbg->bb171[bbg->bb186++]=((bb155)((
bbg->bb100)&0xff ));};{bbg->bb171[bbg->bb186++]=((bb155)((bb125)(bbg->
bb100)>>8 ));};};bbg->bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(bbl))-bbg->
bb79);bbg->bb79+=bb22-(8 *2 *bb12(bbl));}bb54{bbg->bb100|=(bb1738[256 ].
bb248.bb172)<<bbg->bb79;bbg->bb79+=bb22;}};bb2304(bbg);bbm(1 +bbg->
bb1993+10 -bbg->bb79<9 ){{bbe bb22=3 ;bbm(bbg->bb79>(bbe)(8 *2 *bb12(bbl))-
bb22){bbe bb170=1 <<1 ;bbg->bb100|=(bb170<<bbg->bb79);{{bbg->bb171[bbg
->bb186++]=((bb155)((bbg->bb100)&0xff ));};{bbg->bb171[bbg->bb186++]=(
(bb155)((bb125)(bbg->bb100)>>8 ));};};bbg->bb100=(bb125)bb170>>((bbe)(
8 *2 *bb12(bbl))-bbg->bb79);bbg->bb79+=bb22-(8 *2 *bb12(bbl));}bb54{bbg->
bb100|=(1 <<1 )<<bbg->bb79;bbg->bb79+=bb22;}};{bbe bb22=bb1738[256 ].
bb48.bb22;bbm(bbg->bb79>(bbe)(8 *2 *bb12(bbl))-bb22){bbe bb170=bb1738[
256 ].bb248.bb172;bbg->bb100|=(bb170<<bbg->bb79);{{bbg->bb171[bbg->
bb186++]=((bb155)((bbg->bb100)&0xff ));};{bbg->bb171[bbg->bb186++]=((
bb155)((bb125)(bbg->bb100)>>8 ));};};bbg->bb100=(bb125)bb170>>((bbe)(8
 *2 *bb12(bbl))-bbg->bb79);bbg->bb79+=bb22-(8 *2 *bb12(bbl));}bb54{bbg->
bb100|=(bb1738[256 ].bb248.bb172)<<bbg->bb79;bbg->bb79+=bb22;}};bb2304
(bbg);}bbg->bb1993=7 ;}bbb bb1616(bbg,bb40,bb1290,bb1113)bb190*bbg;
bb439*bb40;bb393 bb1290;bbe bb1113;{bb393 bb2013,bb2076;bbe bb1784=0 ;
bbm(bbg->bb123>0 ){bbm(bbg->bb980==2 )bb2314(bbg);bb2184(bbg,(bb1734* )(
&(bbg->bb1936)));;bb2184(bbg,(bb1734* )(&(bbg->bb1862)));;bb1784=
bb2385(bbg);bb2013=(bbg->bb1905+3 +7 )>>3 ;bb2076=(bbg->bb2118+3 +7 )>>3 ;;
bbm(bb2076<=bb2013)bb2013=bb2076;}bb54{;bb2013=bb2076=bb1290+5 ;}bbm(
bb1290+4 <=bb2013&&bb40!=(bbl* )0 ){bb2142(bbg,bb40,bb1290,bb1113);}
bb54 bbm(bb2076==bb2013){{bbe bb22=3 ;bbm(bbg->bb79>(bbe)(8 *2 *bb12(bbl
))-bb22){bbe bb170=(1 <<1 )+bb1113;bbg->bb100|=(bb170<<bbg->bb79);{{bbg
->bb171[bbg->bb186++]=((bb155)((bbg->bb100)&0xff ));};{bbg->bb171[bbg
->bb186++]=((bb155)((bb125)(bbg->bb100)>>8 ));};};bbg->bb100=(bb125)bb170
>>((bbe)(8 *2 *bb12(bbl))-bbg->bb79);bbg->bb79+=bb22-(8 *2 *bb12(bbl));}
bb54{bbg->bb100|=((1 <<1 )+bb1113)<<bbg->bb79;bbg->bb79+=bb22;}};bb2271
(bbg,(bb458* )bb1738,(bb458* )bb2235);}bb54{{bbe bb22=3 ;bbm(bbg->bb79
>(bbe)(8 *2 *bb12(bbl))-bb22){bbe bb170=(2 <<1 )+bb1113;bbg->bb100|=(
bb170<<bbg->bb79);{{bbg->bb171[bbg->bb186++]=((bb155)((bbg->bb100)&
0xff ));};{bbg->bb171[bbg->bb186++]=((bb155)((bb125)(bbg->bb100)>>8 ));
};};bbg->bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(bbl))-bbg->bb79);bbg->
bb79+=bb22-(8 *2 *bb12(bbl));}bb54{bbg->bb100|=((2 <<1 )+bb1113)<<bbg->
bb79;bbg->bb79+=bb22;}};bb2409(bbg,bbg->bb1936.bb502+1 ,bbg->bb1862.
bb502+1 ,bb1784+1 );bb2271(bbg,(bb458* )bbg->bb971,(bb458* )bbg->bb1680
);};bb2245(bbg);bbm(bb1113){bb2259(bbg);};}bbe bb2389(bbg,bb415,
bb1110)bb190*bbg;bbs bb415;bbs bb1110;{bbg->bb1641[bbg->bb605]=(bb125
)bb415;bbg->bb1722[bbg->bb605++]=(bb155)bb1110;bbm(bb415==0 ){bbg->
bb971[bb1110].bb248.bb428++;}bb54{bbg->bb2231++;bb415--;;bbg->bb971[
bb2051[bb1110]+256 +1 ].bb248.bb428++;bbg->bb1680[((bb415)<256 ?bb1735[
bb415]:bb1735[256 +((bb415)>>7 )])].bb248.bb428++;}bb2(bbg->bb605==bbg
->bb1126-1 );}bb41 bbb bb2271(bbg,bb1058,bb1747)bb190*bbg;bb458*bb1058
;bb458*bb1747;{bbs bb415;bbe bb1110;bbs bb2288=0 ;bbs bb172;bbe bb1800
;bbm(bbg->bb605!=0 )bb573{bb415=bbg->bb1641[bb2288];bb1110=bbg->bb1722
[bb2288++];bbm(bb415==0 ){{bbe bb22=bb1058[bb1110].bb48.bb22;bbm(bbg->
bb79>(bbe)(8 *2 *bb12(bbl))-bb22){bbe bb170=bb1058[bb1110].bb248.bb172;
bbg->bb100|=(bb170<<bbg->bb79);{{bbg->bb171[bbg->bb186++]=((bb155)((
bbg->bb100)&0xff ));};{bbg->bb171[bbg->bb186++]=((bb155)((bb125)(bbg->
bb100)>>8 ));};};bbg->bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(bbl))-bbg->
bb79);bbg->bb79+=bb22-(8 *2 *bb12(bbl));}bb54{bbg->bb100|=(bb1058[
bb1110].bb248.bb172)<<bbg->bb79;bbg->bb79+=bb22;}};;}bb54{bb172=
bb2051[bb1110];{bbe bb22=bb1058[bb172+256 +1 ].bb48.bb22;bbm(bbg->bb79>
(bbe)(8 *2 *bb12(bbl))-bb22){bbe bb170=bb1058[bb172+256 +1 ].bb248.bb172;
bbg->bb100|=(bb170<<bbg->bb79);{{bbg->bb171[bbg->bb186++]=((bb155)((
bbg->bb100)&0xff ));};{bbg->bb171[bbg->bb186++]=((bb155)((bb125)(bbg->
bb100)>>8 ));};};bbg->bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(bbl))-bbg->
bb79);bbg->bb79+=bb22-(8 *2 *bb12(bbl));}bb54{bbg->bb100|=(bb1058[bb172
+256 +1 ].bb248.bb172)<<bbg->bb79;bbg->bb79+=bb22;}};bb1800=bb2415[
bb172];bbm(bb1800!=0 ){bb1110-=bb2334[bb172];{bbe bb22=bb1800;bbm(bbg
->bb79>(bbe)(8 *2 *bb12(bbl))-bb22){bbe bb170=bb1110;bbg->bb100|=(bb170
<<bbg->bb79);{{bbg->bb171[bbg->bb186++]=((bb155)((bbg->bb100)&0xff ));
};{bbg->bb171[bbg->bb186++]=((bb155)((bb125)(bbg->bb100)>>8 ));};};bbg
->bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(bbl))-bbg->bb79);bbg->bb79+=
bb22-(8 *2 *bb12(bbl));}bb54{bbg->bb100|=(bb1110)<<bbg->bb79;bbg->bb79
+=bb22;}};}bb415--;bb172=((bb415)<256 ?bb1735[bb415]:bb1735[256 +((
bb415)>>7 )]);;{bbe bb22=bb1747[bb172].bb48.bb22;bbm(bbg->bb79>(bbe)(8
 *2 *bb12(bbl))-bb22){bbe bb170=bb1747[bb172].bb248.bb172;bbg->bb100|=
(bb170<<bbg->bb79);{{bbg->bb171[bbg->bb186++]=((bb155)((bbg->bb100)&
0xff ));};{bbg->bb171[bbg->bb186++]=((bb155)((bb125)(bbg->bb100)>>8 ));
};};bbg->bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(bbl))-bbg->bb79);bbg->
bb79+=bb22-(8 *2 *bb12(bbl));}bb54{bbg->bb100|=(bb1747[bb172].bb248.
bb172)<<bbg->bb79;bbg->bb79+=bb22;}};bb1800=bb2379[bb172];bbm(bb1800
!=0 ){bb415-=bb2417[bb172];{bbe bb22=bb1800;bbm(bbg->bb79>(bbe)(8 *2 *
bb12(bbl))-bb22){bbe bb170=bb415;bbg->bb100|=(bb170<<bbg->bb79);{{bbg
->bb171[bbg->bb186++]=((bb155)((bbg->bb100)&0xff ));};{bbg->bb171[bbg
->bb186++]=((bb155)((bb125)(bbg->bb100)>>8 ));};};bbg->bb100=(bb125)bb170
>>((bbe)(8 *2 *bb12(bbl))-bbg->bb79);bbg->bb79+=bb22-(8 *2 *bb12(bbl));}
bb54{bbg->bb100|=(bb415)<<bbg->bb79;bbg->bb79+=bb22;}};}};}bb106(
bb2288<bbg->bb605);{bbe bb22=bb1058[256 ].bb48.bb22;bbm(bbg->bb79>(bbe
)(8 *2 *bb12(bbl))-bb22){bbe bb170=bb1058[256 ].bb248.bb172;bbg->bb100|=
(bb170<<bbg->bb79);{{bbg->bb171[bbg->bb186++]=((bb155)((bbg->bb100)&
0xff ));};{bbg->bb171[bbg->bb186++]=((bb155)((bb125)(bbg->bb100)>>8 ));
};};bbg->bb100=(bb125)bb170>>((bbe)(8 *2 *bb12(bbl))-bbg->bb79);bbg->
bb79+=bb22-(8 *2 *bb12(bbl));}bb54{bbg->bb100|=(bb1058[256 ].bb248.bb172
)<<bbg->bb79;bbg->bb79+=bb22;}};bbg->bb1993=bb1058[256 ].bb48.bb22;}
bb41 bbb bb2314(bbg)bb190*bbg;{bbe bb11=0 ;bbs bb2327=0 ;bbs bb2270=0 ;
bb106(bb11<7 )bb2270+=bbg->bb971[bb11++].bb248.bb428;bb106(bb11<128 )bb2327
+=bbg->bb971[bb11++].bb248.bb428;bb106(bb11<256 )bb2270+=bbg->bb971[
bb11++].bb248.bb428;bbg->bb980=(bb152)(bb2270>(bb2327>>2 )?0 :1 );}bb41
bbs bb2370(bb172,bb22)bbs bb172;bbe bb22;{bb925 bbs bb2305=0 ;bb573{
bb2305|=bb172&1 ;bb172>>=1 ,bb2305<<=1 ;}bb106(--bb22>0 );bb2 bb2305>>1 ;}
bb41 bbb bb2304(bbg)bb190*bbg;{bbm(bbg->bb79==16 ){{{bbg->bb171[bbg->
bb186++]=((bb155)((bbg->bb100)&0xff ));};{bbg->bb171[bbg->bb186++]=((
bb155)((bb125)(bbg->bb100)>>8 ));};};bbg->bb100=0 ;bbg->bb79=0 ;}bb54 bbm
(bbg->bb79>=8 ){{bbg->bb171[bbg->bb186++]=((bb152)bbg->bb100);};bbg->
bb100>>=8 ;bbg->bb79-=8 ;}}bb41 bbb bb2259(bbg)bb190*bbg;{bbm(bbg->bb79
>8 ){{{bbg->bb171[bbg->bb186++]=((bb155)((bbg->bb100)&0xff ));};{bbg->
bb171[bbg->bb186++]=((bb155)((bb125)(bbg->bb100)>>8 ));};};}bb54 bbm(
bbg->bb79>0 ){{bbg->bb171[bbg->bb186++]=((bb152)bbg->bb100);};}bbg->
bb100=0 ;bbg->bb79=0 ;}bb41 bbb bb2319(bbg,bb40,bb22,bb985)bb190*bbg;
bb439*bb40;bbs bb22;bbe bb985;{bb2259(bbg);bbg->bb1993=8 ;bbm(bb985){{
{bbg->bb171[bbg->bb186++]=((bb155)(((bb125)bb22)&0xff ));};{bbg->bb171
[bbg->bb186++]=((bb155)((bb125)((bb125)bb22)>>8 ));};};{{bbg->bb171[
bbg->bb186++]=((bb155)(((bb125)~bb22)&0xff ));};{bbg->bb171[bbg->bb186
++]=((bb155)((bb125)((bb125)~bb22)>>8 ));};};}bb106(bb22--){{bbg->
bb171[bbg->bb186++]=( *bb40++);};}}
