/*
   'src_compress_deflate_deflate.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Mon Oct 29 11:54:27 2012
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
bb1290,bbe bb1113));bb41 bbh bb458 bb1738[(256 +1 +29 )+2 ]={{{12 },{8 }},{
{140 },{8 }},{{76 },{8 }},{{204 },{8 }},{{44 },{8 }},{{172 },{8 }},{{108 },{8 }},
{{236 },{8 }},{{28 },{8 }},{{156 },{8 }},{{92 },{8 }},{{220 },{8 }},{{60 },{8 }},
{{188 },{8 }},{{124 },{8 }},{{252 },{8 }},{{2 },{8 }},{{130 },{8 }},{{66 },{8 }},
{{194 },{8 }},{{34 },{8 }},{{162 },{8 }},{{98 },{8 }},{{226 },{8 }},{{18 },{8 }},
{{146 },{8 }},{{82 },{8 }},{{210 },{8 }},{{50 },{8 }},{{178 },{8 }},{{114 },{8 }}
,{{242 },{8 }},{{10 },{8 }},{{138 },{8 }},{{74 },{8 }},{{202 },{8 }},{{42 },{8 }}
,{{170 },{8 }},{{106 },{8 }},{{234 },{8 }},{{26 },{8 }},{{154 },{8 }},{{90 },{8 }
},{{218 },{8 }},{{58 },{8 }},{{186 },{8 }},{{122 },{8 }},{{250 },{8 }},{{6 },{8 }
},{{134 },{8 }},{{70 },{8 }},{{198 },{8 }},{{38 },{8 }},{{166 },{8 }},{{102 },{8
}},{{230 },{8 }},{{22 },{8 }},{{150 },{8 }},{{86 },{8 }},{{214 },{8 }},{{54 },{8
}},{{182 },{8 }},{{118 },{8 }},{{246 },{8 }},{{14 },{8 }},{{142 },{8 }},{{78 },{
8 }},{{206 },{8 }},{{46 },{8 }},{{174 },{8 }},{{110 },{8 }},{{238 },{8 }},{{30 },
{8 }},{{158 },{8 }},{{94 },{8 }},{{222 },{8 }},{{62 },{8 }},{{190 },{8 }},{{126 }
,{8 }},{{254 },{8 }},{{1 },{8 }},{{129 },{8 }},{{65 },{8 }},{{193 },{8 }},{{33 },
{8 }},{{161 },{8 }},{{97 },{8 }},{{225 },{8 }},{{17 },{8 }},{{145 },{8 }},{{81 },
{8 }},{{209 },{8 }},{{49 },{8 }},{{177 },{8 }},{{113 },{8 }},{{241 },{8 }},{{9 },
{8 }},{{137 },{8 }},{{73 },{8 }},{{201 },{8 }},{{41 },{8 }},{{169 },{8 }},{{105 }
,{8 }},{{233 },{8 }},{{25 },{8 }},{{153 },{8 }},{{89 },{8 }},{{217 },{8 }},{{57 }
,{8 }},{{185 },{8 }},{{121 },{8 }},{{249 },{8 }},{{5 },{8 }},{{133 },{8 }},{{69 }
,{8 }},{{197 },{8 }},{{37 },{8 }},{{165 },{8 }},{{101 },{8 }},{{229 },{8 }},{{21
},{8 }},{{149 },{8 }},{{85 },{8 }},{{213 },{8 }},{{53 },{8 }},{{181 },{8 }},{{
117 },{8 }},{{245 },{8 }},{{13 },{8 }},{{141 },{8 }},{{77 },{8 }},{{205 },{8 }},{
{45 },{8 }},{{173 },{8 }},{{109 },{8 }},{{237 },{8 }},{{29 },{8 }},{{157 },{8 }},
{{93 },{8 }},{{221 },{8 }},{{61 },{8 }},{{189 },{8 }},{{125 },{8 }},{{253 },{8 }}
,{{19 },{9 }},{{275 },{9 }},{{147 },{9 }},{{403 },{9 }},{{83 },{9 }},{{339 },{9 }
},{{211 },{9 }},{{467 },{9 }},{{51 },{9 }},{{307 },{9 }},{{179 },{9 }},{{435 },{
9 }},{{115 },{9 }},{{371 },{9 }},{{243 },{9 }},{{499 },{9 }},{{11 },{9 }},{{267 }
,{9 }},{{139 },{9 }},{{395 },{9 }},{{75 },{9 }},{{331 },{9 }},{{203 },{9 }},{{
459 },{9 }},{{43 },{9 }},{{299 },{9 }},{{171 },{9 }},{{427 },{9 }},{{107 },{9 }},
{{363 },{9 }},{{235 },{9 }},{{491 },{9 }},{{27 },{9 }},{{283 },{9 }},{{155 },{9 }
},{{411 },{9 }},{{91 },{9 }},{{347 },{9 }},{{219 },{9 }},{{475 },{9 }},{{59 },{9
}},{{315 },{9 }},{{187 },{9 }},{{443 },{9 }},{{123 },{9 }},{{379 },{9 }},{{251 }
,{9 }},{{507 },{9 }},{{7 },{9 }},{{263 },{9 }},{{135 },{9 }},{{391 },{9 }},{{71 }
,{9 }},{{327 },{9 }},{{199 },{9 }},{{455 },{9 }},{{39 },{9 }},{{295 },{9 }},{{
167 },{9 }},{{423 },{9 }},{{103 },{9 }},{{359 },{9 }},{{231 },{9 }},{{487 },{9 }}
,{{23 },{9 }},{{279 },{9 }},{{151 },{9 }},{{407 },{9 }},{{87 },{9 }},{{343 },{9 }
},{{215 },{9 }},{{471 },{9 }},{{55 },{9 }},{{311 },{9 }},{{183 },{9 }},{{439 },{
9 }},{{119 },{9 }},{{375 },{9 }},{{247 },{9 }},{{503 },{9 }},{{15 },{9 }},{{271 }
,{9 }},{{143 },{9 }},{{399 },{9 }},{{79 },{9 }},{{335 },{9 }},{{207 },{9 }},{{
463 },{9 }},{{47 },{9 }},{{303 },{9 }},{{175 },{9 }},{{431 },{9 }},{{111 },{9 }},
{{367 },{9 }},{{239 },{9 }},{{495 },{9 }},{{31 },{9 }},{{287 },{9 }},{{159 },{9 }
},{{415 },{9 }},{{95 },{9 }},{{351 },{9 }},{{223 },{9 }},{{479 },{9 }},{{63 },{9
}},{{319 },{9 }},{{191 },{9 }},{{447 },{9 }},{{127 },{9 }},{{383 },{9 }},{{255 }
,{9 }},{{511 },{9 }},{{0 },{7 }},{{64 },{7 }},{{32 },{7 }},{{96 },{7 }},{{16 },{7
}},{{80 },{7 }},{{48 },{7 }},{{112 },{7 }},{{8 },{7 }},{{72 },{7 }},{{40 },{7 }},
{{104 },{7 }},{{24 },{7 }},{{88 },{7 }},{{56 },{7 }},{{120 },{7 }},{{4 },{7 }},{{
68 },{7 }},{{36 },{7 }},{{100 },{7 }},{{20 },{7 }},{{84 },{7 }},{{52 },{7 }},{{
116 },{7 }},{{3 },{8 }},{{131 },{8 }},{{67 },{8 }},{{195 },{8 }},{{35 },{8 }},{{
163 },{8 }},{{99 },{8 }},{{227 },{8 }}};bb41 bbh bb458 bb2235[30 ]={{{0 },{5 }
},{{16 },{5 }},{{8 },{5 }},{{24 },{5 }},{{4 },{5 }},{{20 },{5 }},{{12 },{5 }},{{
28 },{5 }},{{2 },{5 }},{{18 },{5 }},{{10 },{5 }},{{26 },{5 }},{{6 },{5 }},{{22 },{
5 }},{{14 },{5 }},{{30 },{5 }},{{1 },{5 }},{{17 },{5 }},{{9 },{5 }},{{25 },{5 }},{
{5 },{5 }},{{21 },{5 }},{{13 },{5 }},{{29 },{5 }},{{3 },{5 }},{{19 },{5 }},{{11 },
{5 }},{{27 },{5 }},{{7 },{5 }},{{23 },{5 }}};bb41 bbh bb155 bb1735[512 ]={0 ,1
,2 ,3 ,4 ,4 ,5 ,5 ,6 ,6 ,6 ,6 ,7 ,7 ,7 ,7 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8 ,9 ,9 ,9 ,9 ,9 ,9 ,9 ,9 ,10 ,10 ,10
,10 ,10 ,10 ,10 ,10 ,10 ,10 ,10 ,10 ,10 ,10 ,10 ,10 ,11 ,11 ,11 ,11 ,11 ,11 ,11 ,11 ,11 ,11
,11 ,11 ,11 ,11 ,11 ,11 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12
,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,12 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13
,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13 ,13
,13 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14
,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14
,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,14 ,15 ,15 ,15 ,15
,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15
,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15
,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,15 ,0 ,0 ,16 ,17 ,18 ,18 ,19 ,19 ,20 ,
20 ,20 ,20 ,21 ,21 ,21 ,21 ,22 ,22 ,22 ,22 ,22 ,22 ,22 ,22 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,
24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,
25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,
26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,27 ,27 ,27 ,27 ,27 ,
27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,
27 ,27 ,27 ,27 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,
28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,
28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,28 ,29 ,
29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,
29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,
29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 ,29 };bb41 bbh bb155
bb2051[258 -3 +1 ]={0 ,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,8 ,9 ,9 ,10 ,10 ,11 ,11 ,12 ,12 ,12 ,12 ,13 ,
13 ,13 ,13 ,14 ,14 ,14 ,14 ,15 ,15 ,15 ,15 ,16 ,16 ,16 ,16 ,16 ,16 ,16 ,16 ,17 ,17 ,17 ,17 ,
17 ,17 ,17 ,17 ,18 ,18 ,18 ,18 ,18 ,18 ,18 ,18 ,19 ,19 ,19 ,19 ,19 ,19 ,19 ,19 ,20 ,20 ,20 ,
20 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,20 ,21 ,21 ,21 ,21 ,21 ,21 ,21 ,21 ,21 ,21 ,
21 ,21 ,21 ,21 ,21 ,21 ,22 ,22 ,22 ,22 ,22 ,22 ,22 ,22 ,22 ,22 ,22 ,22 ,22 ,22 ,22 ,22 ,23 ,
23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,23 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,
24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,24 ,
24 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,
25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,25 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,
26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,26 ,27 ,27 ,27 ,27 ,
27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,
27 ,27 ,27 ,27 ,28 };bb41 bbh bbe bb2334[29 ]={0 ,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,10 ,12 ,14 ,
16 ,20 ,24 ,28 ,32 ,40 ,48 ,56 ,64 ,80 ,96 ,112 ,128 ,160 ,192 ,224 ,0 };bb41 bbh bbe
bb2417[30 ]={0 ,1 ,2 ,3 ,4 ,6 ,8 ,12 ,16 ,24 ,32 ,48 ,64 ,96 ,128 ,192 ,256 ,384 ,512 ,
768 ,1024 ,1536 ,2048 ,3072 ,4096 ,6144 ,8192 ,12288 ,16384 ,24576 };bba bb9{
bb1248,bb2159,bb1757,bb2138}bb1823;bba bb1823( *bb2386)bbp((bb190*bbg
,bbe bb174));bb41 bbb bb2189 bbp((bb190*bbg));bb41 bb1823 bb2380 bbp(
(bb190*bbg,bbe bb174));bb41 bb1823 bb2161 bbp((bb190*bbg,bbe bb174));
bb41 bb1823 bb1947 bbp((bb190*bbg,bbe bb174));bb41 bbb bb2428 bbp((
bb190*bbg));bb41 bbb bb2008 bbp((bb190*bbg,bb7 bbn));bb41 bbb bb1266
bbp((bb17 bb16));bb41 bbe bb2434 bbp((bb17 bb16,bb30*bb40,bbs bb47));
bb41 bb7 bb2258 bbp((bb190*bbg,bb1321 bb1994));bba bbi bb2627{bb125
bb2431;bb125 bb2438;bb125 bb2383;bb125 bb2348;bb2386 bb2021;}bb2513;
bb41 bbh bb2513 bb1336[10 ]={{0 ,0 ,0 ,0 ,bb2380},{4 ,4 ,8 ,4 ,bb2161},{4 ,5 ,16
,8 ,bb2161},{4 ,6 ,32 ,32 ,bb2161},{4 ,4 ,16 ,16 ,bb1947},{8 ,16 ,32 ,32 ,bb1947},
{8 ,16 ,128 ,128 ,bb1947},{8 ,32 ,128 ,256 ,bb1947},{32 ,128 ,258 ,1024 ,bb1947},
{32 ,258 ,258 ,4096 ,bb1947}};bbi bb2278{bbe bb445;};bbe bb1115(bb16,
bb123,bb187,bb197)bb17 bb16;bbe bb123;bbh bbl*bb187;bbe bb197;{bb2
bb1050(bb16,bb123,8 ,15 ,bb803,0 ,bb187,bb197);}bbe bb1050(bb16,bb123,
bb568,bb451,bb933,bb299,bb187,bb197)bb17 bb16;bbe bb123;bbe bb568;bbe
bb451;bbe bb933;bbe bb299;bbh bbl*bb187;bbe bb197;{bb190*bbg;bbe
bb1352=0 ;bb41 bbh bbl*bb2442="\x31\x2e\x31\x2e\x33";bb501*bb1883;bbm(
bb187==0 ||bb187[0 ]!=bb2442[0 ]||bb197!=bb12(bb433)){bb2(-6 );}bbm(bb16
==0 )bb2(-2 );bb16->bb322=0 ;bbm(bb16->bb401==0 ){bb16->bb401=bb1168;bb16
->bb110=(bb70)0 ;}bbm(bb16->bb367==0 )bb16->bb367=bb1163;bbm(bb123==(-1
))bb123=6 ;bbm(bb451<0 ){bb1352=1 ;bb451=-bb451;}bbm(bb933<1 ||bb933>
bb210||bb568!=8 ||bb451<8 ||bb451>15 ||bb123<0 ||bb123>9 ||bb299<0 ||bb299>
2 ){bb2(-2 );}bbg=(bb190* )( * ((bb16)->bb401))((bb16)->bb110,(1 ),(bb12
(bb190)));bbm(bbg==0 )bb2(-4 );bb16->bb23=(bbi bb381* )bbg;bbg->bb16=
bb16;bbg->bb1352=bb1352;bbg->bb2162=bb451;bbg->bb935=1 <<bbg->bb2162;
bbg->bb1808=bbg->bb935-1 ;bbg->bb2129=bb933+7 ;bbg->bb1328=1 <<bbg->
bb2129;bbg->bb1381=bbg->bb1328-1 ;bbg->bb1610=((bbg->bb2129+3 -1 )/3 );
bbg->bb158=(bb30* )( * ((bb16)->bb401))((bb16)->bb110,(bbg->bb935),(2
 *bb12(bb152)));bbg->bb968=(bb1357* )( * ((bb16)->bb401))((bb16)->
bb110,(bbg->bb935),(bb12(bb1039)));bbg->bb383=(bb1357* )( * ((bb16)->
bb401))((bb16)->bb110,(bbg->bb1328),(bb12(bb1039)));bbg->bb1126=1 <<(
bb933+6 );bb1883=(bb501* )( * ((bb16)->bb401))((bb16)->bb110,(bbg->
bb1126),(bb12(bb125)+2 ));bbg->bb171=(bb1205* )bb1883;bbg->bb2039=(
bb393)bbg->bb1126* (bb12(bb125)+2L );bbm(bbg->bb158==0 ||bbg->bb968==0
||bbg->bb383==0 ||bbg->bb171==0 ){bb16->bb322=(bbl* )bb1070[2 -((-4 ))];
bb945(bb16);bb2(-4 );}bbg->bb1641=bb1883+bbg->bb1126/bb12(bb125);bbg->
bb1722=bbg->bb171+(1 +bb12(bb125)) *bbg->bb1126;bbg->bb123=bb123;bbg->
bb299=bb299;bbg->bb568=(bb152)bb568;bb2 bb1059(bb16);}bbe bb1178(bb16
,bb424,bb436)bb17 bb16;bbh bb30*bb424;bb7 bb436;{bb190*bbg;bb7 bb470=
bb436;bb7 bb11;bb1321 bb1136=0 ;bbm(bb16==0 ||bb16->bb23==0 ||bb424==0 ||
bb16->bb23->bb360!=42 )bb2(-2 );bbg=bb16->bb23;bb16->bb368=bb986(bb16->
bb368,bb424,bb436);bbm(bb470<3 )bb2 0 ;bbm(bb470>((bbg)->bb935-(258 +3 +1
))){bb470=((bbg)->bb935-(258 +3 +1 ));bb424+=bb436-bb470;}bb81(bbg->
bb158,bb424,bb470);bbg->bb185=bb470;bbg->bb429=(bb6)bb470;bbg->bb499=
bbg->bb158[0 ];(bbg->bb499=(((bbg->bb499)<<bbg->bb1610)^(bbg->bb158[1 ]
))&bbg->bb1381);bb90(bb11=0 ;bb11<=bb470-3 ;bb11++){((bbg->bb499=(((bbg
->bb499)<<bbg->bb1610)^(bbg->bb158[(bb11)+(3 -1 )]))&bbg->bb1381),bbg->
bb968[(bb11)&bbg->bb1808]=bb1136=bbg->bb383[bbg->bb499],bbg->bb383[
bbg->bb499]=(bb1039)(bb11));}bbm(bb1136)bb1136=0 ;bb2 0 ;}bbe bb1059(
bb16)bb17 bb16;{bb190*bbg;bbm(bb16==0 ||bb16->bb23==0 ||bb16->bb401==0
||bb16->bb367==0 )bb2(-2 );bb16->bb192=bb16->bb615=0 ;bb16->bb322=0 ;bb16
->bb980=2 ;bbg=(bb190* )bb16->bb23;bbg->bb186=0 ;bbg->bb1874=bbg->bb171
;bbm(bbg->bb1352<0 ){bbg->bb1352=0 ;}bbg->bb360=bbg->bb1352?113 :42 ;bb16
->bb368=1 ;bbg->bb1894=0 ;bb2209(bbg);bb2428(bbg);bb2 0 ;}bbe bb1177(
bb16,bb123,bb299)bb17 bb16;bbe bb123;bbe bb299;{bb190*bbg;bb2386
bb2021;bbe bb18=0 ;bbm(bb16==0 ||bb16->bb23==0 )bb2(-2 );bbg=bb16->bb23;
bbm(bb123==(-1 )){bb123=6 ;}bbm(bb123<0 ||bb123>9 ||bb299<0 ||bb299>2 ){bb2
(-2 );}bb2021=bb1336[bbg->bb123].bb2021;bbm(bb2021!=bb1336[bb123].
bb2021&&bb16->bb192!=0 ){bb18=bb513(bb16,1 );}bbm(bbg->bb123!=bb123){
bbg->bb123=bb123;bbg->bb2055=bb1336[bb123].bb2438;bbg->bb2180=bb1336[
bb123].bb2431;bbg->bb1838=bb1336[bb123].bb2383;bbg->bb2175=bb1336[
bb123].bb2348;}bbg->bb299=bb299;bb2 bb18;}bb41 bbb bb2008(bbg,bbn)bb190
 *bbg;bb7 bbn;{{bbg->bb171[bbg->bb186++]=((bb152)(bbn>>8 ));};{bbg->
bb171[bbg->bb186++]=((bb152)(bbn&0xff ));};}bb41 bbb bb1266(bb16)bb17
bb16;{bbs bb22=bb16->bb23->bb186;bbm(bb22>bb16->bb384)bb22=bb16->
bb384;bbm(bb22==0 )bb2;bb81(bb16->bb596,bb16->bb23->bb1874,bb22);bb16
->bb596+=bb22;bb16->bb23->bb1874+=bb22;bb16->bb615+=bb22;bb16->bb384
-=bb22;bb16->bb23->bb186-=bb22;bbm(bb16->bb23->bb186==0 ){bb16->bb23->
bb1874=bb16->bb23->bb171;}}bbe bb513(bb16,bb174)bb17 bb16;bbe bb174;{
bbe bb2344;bb190*bbg;bbm(bb16==0 ||bb16->bb23==0 ||bb174>4 ||bb174<0 ){
bb2(-2 );}bbg=bb16->bb23;bbm(bb16->bb596==0 ||(bb16->bb126==0 &&bb16->
bb148!=0 )||(bbg->bb360==666 &&bb174!=4 )){bb2(bb16->bb322=(bbl* )bb1070
[2 -((-2 ))],((-2 )));}bbm(bb16->bb384==0 )bb2(bb16->bb322=(bbl* )bb1070[
2 -((-5 ))],((-5 )));bbg->bb16=bb16;bb2344=bbg->bb1894;bbg->bb1894=bb174
;bbm(bbg->bb360==42 ){bb7 bb985=(8 +((bbg->bb2162-8 )<<4 ))<<8 ;bb7 bb2298
=(bbg->bb123-1 )>>1 ;bbm(bb2298>3 )bb2298=3 ;bb985|=(bb2298<<6 );bbm(bbg->
bb185!=0 )bb985|=0x20 ;bb985+=31 -(bb985%31 );bbg->bb360=113 ;bb2008(bbg,
bb985);bbm(bbg->bb185!=0 ){bb2008(bbg,(bb7)(bb16->bb368>>16 ));bb2008(
bbg,(bb7)(bb16->bb368&0xffff ));}bb16->bb368=1L ;}bbm(bbg->bb186!=0 ){
bb1266(bb16);bbm(bb16->bb384==0 ){bbg->bb1894=-1 ;bb2 0 ;}}bb54 bbm(bb16
->bb148==0 &&bb174<=bb2344&&bb174!=4 ){bb2(bb16->bb322=(bbl* )bb1070[2 -
((-5 ))],((-5 )));}bbm(bbg->bb360==666 &&bb16->bb148!=0 ){bb2(bb16->bb322
=(bbl* )bb1070[2 -((-5 ))],((-5 )));}bbm(bb16->bb148!=0 ||bbg->bb463!=0 ||
(bb174!=0 &&bbg->bb360!=666 )){bb1823 bb1985;bb1985=( * (bb1336[bbg->
bb123].bb2021))(bbg,bb174);bbm(bb1985==bb1757||bb1985==bb2138){bbg->
bb360=666 ;}bbm(bb1985==bb1248||bb1985==bb1757){bbm(bb16->bb384==0 ){
bbg->bb1894=-1 ;}bb2 0 ;}bbm(bb1985==bb2159){bbm(bb174==1 ){bb2255(bbg);
}bb54{bb2142(bbg,(bbl* )0 ,0L ,0 );bbm(bb174==3 ){bbg->bb383[bbg->bb1328-
1 ]=0 ;bb999((bb30* )bbg->bb383,0 ,(bbs)(bbg->bb1328-1 ) *bb12( *bbg->
bb383));;}}bb1266(bb16);bbm(bb16->bb384==0 ){bbg->bb1894=-1 ;bb2 0 ;}}};
bbm(bb174!=4 )bb2 0 ;bbm(bbg->bb1352)bb2 1 ;bb2008(bbg,(bb7)(bb16->bb368
>>16 ));bb2008(bbg,(bb7)(bb16->bb368&0xffff ));bb1266(bb16);bbg->bb1352
=-1 ;bb2 bbg->bb186!=0 ?0 :1 ;}bbe bb945(bb16)bb17 bb16;{bbe bb360;bbm(
bb16==0 ||bb16->bb23==0 )bb2(-2 );bb360=bb16->bb23->bb360;bbm(bb360!=42
&&bb360!=113 &&bb360!=666 ){bb2(-2 );}{bbm(bb16->bb23->bb171)( * ((bb16)->
bb367))((bb16)->bb110,(bb70)(bb16->bb23->bb171));};{bbm(bb16->bb23->
bb383)( * ((bb16)->bb367))((bb16)->bb110,(bb70)(bb16->bb23->bb383));}
;{bbm(bb16->bb23->bb968)( * ((bb16)->bb367))((bb16)->bb110,(bb70)(
bb16->bb23->bb968));};{bbm(bb16->bb23->bb158)( * ((bb16)->bb367))((
bb16)->bb110,(bb70)(bb16->bb23->bb158));};( * ((bb16)->bb367))((bb16)->
bb110,(bb70)(bb16->bb23));bb16->bb23=0 ;bb2 bb360==113 ?(-3 ):0 ;}bbe
bb1151(bb129,bb183)bb17 bb129;bb17 bb183;{
#ifdef bb508
bb2(-2 );
#else
bb190*bb430;bb190*bb1824;bb501*bb1883;bbm(bb183==0 ||bb129==0 ||bb183->
bb23==0 ){bb2(-2 );}bb1824=bb183->bb23; *bb129= *bb183;bb430=(bb190* )(
 * ((bb129)->bb401))((bb129)->bb110,(1 ),(bb12(bb190)));bbm(bb430==0 )bb2
(-4 );bb129->bb23=(bbi bb381* )bb430; *bb430= *bb1824;bb430->bb16=
bb129;bb430->bb158=(bb30* )( * ((bb129)->bb401))((bb129)->bb110,(
bb430->bb935),(2 *bb12(bb152)));bb430->bb968=(bb1357* )( * ((bb129)->
bb401))((bb129)->bb110,(bb430->bb935),(bb12(bb1039)));bb430->bb383=(
bb1357* )( * ((bb129)->bb401))((bb129)->bb110,(bb430->bb1328),(bb12(
bb1039)));bb1883=(bb501* )( * ((bb129)->bb401))((bb129)->bb110,(bb430
->bb1126),(bb12(bb125)+2 ));bb430->bb171=(bb1205* )bb1883;bbm(bb430->
bb158==0 ||bb430->bb968==0 ||bb430->bb383==0 ||bb430->bb171==0 ){bb945(
bb129);bb2(-4 );}bb81(bb430->bb158,bb1824->bb158,bb430->bb935*2 *bb12(
bb152));bb81((bb30* )bb430->bb968,(bb30* )bb1824->bb968,bb430->bb935*
bb12(bb1039));bb81((bb30* )bb430->bb383,(bb30* )bb1824->bb383,bb430->
bb1328*bb12(bb1039));bb81(bb430->bb171,bb1824->bb171,(bb7)bb430->
bb2039);bb430->bb1874=bb430->bb171+(bb1824->bb1874-bb1824->bb171);
bb430->bb1641=bb1883+bb430->bb1126/bb12(bb125);bb430->bb1722=bb430->
bb171+(1 +bb12(bb125)) *bb430->bb1126;bb430->bb1936.bb1749=bb430->
bb971;bb430->bb1862.bb1749=bb430->bb1680;bb430->bb2062.bb1749=bb430->
bb530;bb2 0 ;
#endif
}bb41 bbe bb2434(bb16,bb40,bb47)bb17 bb16;bb30*bb40;bbs bb47;{bbs bb22
=bb16->bb148;bbm(bb22>bb47)bb22=bb47;bbm(bb22==0 )bb2 0 ;bb16->bb148-=
bb22;bbm(!bb16->bb23->bb1352){bb16->bb368=bb986(bb16->bb368,bb16->
bb126,bb22);}bb81(bb40,bb16->bb126,bb22);bb16->bb126+=bb22;bb16->
bb192+=bb22;bb2(bbe)bb22;}bb41 bbb bb2428(bbg)bb190*bbg;{bbg->bb2227=
(bb393)2L *bbg->bb935;bbg->bb383[bbg->bb1328-1 ]=0 ;bb999((bb30* )bbg->
bb383,0 ,(bbs)(bbg->bb1328-1 ) *bb12( *bbg->bb383));;bbg->bb2055=bb1336
[bbg->bb123].bb2438;bbg->bb2180=bb1336[bbg->bb123].bb2431;bbg->bb1838
=bb1336[bbg->bb123].bb2383;bbg->bb2175=bb1336[bbg->bb123].bb2348;bbg
->bb185=0 ;bbg->bb429=0L ;bbg->bb463=0 ;bbg->bb951=bbg->bb1274=3 -1 ;bbg->
bb1940=0 ;bbg->bb499=0 ;}bb41 bb7 bb2258(bbg,bb1994)bb190*bbg;bb1321
bb1994;{bbs bb2318=bbg->bb2175;bb925 bb30*bb497=bbg->bb158+bbg->bb185
;bb925 bb30*bb606;bb925 bbe bb22;bbe bb1190=bbg->bb1274;bbe bb1838=
bbg->bb1838;bb1321 bb2444=bbg->bb185>(bb1321)((bbg)->bb935-(258 +3 +1 ))?
bbg->bb185-(bb1321)((bbg)->bb935-(258 +3 +1 )):0 ;bb1357*bb968=bbg->bb968
;bb7 bb2470=bbg->bb1808;
#ifdef bb1044
bb925 bb30*bb1893=bbg->bb158+bbg->bb185+258 -1 ;bb925 bb125 bb2533= * (
bb501* )bb497;bb925 bb125 bb2120= * (bb501* )(bb497+bb1190-1 );
#else
bb925 bb30*bb1893=bbg->bb158+bbg->bb185+258 ;bb925 bb152 bb2320=bb497[
bb1190-1 ];bb925 bb152 bb2120=bb497[bb1190];
#endif
;bbm(bbg->bb1274>=bbg->bb2180){bb2318>>=2 ;}bbm((bb7)bb1838>bbg->bb463
)bb1838=bbg->bb463;;bb573{;bb606=bbg->bb158+bb1994;
#if ( defined( bb1044) && bb2581 == 258)
bbm( * (bb501* )(bb606+bb1190-1 )!=bb2120|| * (bb501* )bb606!=bb2533)bb1684
;;bb497++,bb606++;bb573{}bb106( * (bb501* )(bb497+=2 )== * (bb501* )(
bb606+=2 )&& * (bb501* )(bb497+=2 )== * (bb501* )(bb606+=2 )&& * (bb501*
)(bb497+=2 )== * (bb501* )(bb606+=2 )&& * (bb501* )(bb497+=2 )== * (
bb501* )(bb606+=2 )&&bb497<bb1893);;bbm( *bb497== *bb606)bb497++;bb22=
(258 -1 )-(bbe)(bb1893-bb497);bb497=bb1893-(258 -1 );
#else
bbm(bb606[bb1190]!=bb2120||bb606[bb1190-1 ]!=bb2320|| *bb606!= *bb497
|| * ++bb606!=bb497[1 ])bb1684;bb497+=2 ,bb606++;;bb573{}bb106( * ++
bb497== * ++bb606&& * ++bb497== * ++bb606&& * ++bb497== * ++bb606&& *
++bb497== * ++bb606&& * ++bb497== * ++bb606&& * ++bb497== * ++bb606&&
 * ++bb497== * ++bb606&& * ++bb497== * ++bb606&&bb497<bb1893);;bb22=
258 -(bbe)(bb1893-bb497);bb497=bb1893-258 ;
#endif
bbm(bb22>bb1190){bbg->bb2023=bb1994;bb1190=bb22;bbm(bb22>=bb1838)bb21
;
#ifdef bb1044
bb2120= * (bb501* )(bb497+bb1190-1 );
#else
bb2320=bb497[bb1190-1 ];bb2120=bb497[bb1190];
#endif
}}bb106((bb1994=bb968[bb1994&bb2470])>bb2444&&--bb2318!=0 );bbm((bb7)bb1190
<=bbg->bb463)bb2(bb7)bb1190;bb2 bbg->bb463;}bb41 bbb bb2189(bbg)bb190
 *bbg;{bb925 bbs bb11,bb95;bb925 bb1357*bb28;bbs bb1927;bb7 bb1222=
bbg->bb935;bb573{bb1927=(bbs)(bbg->bb2227-(bb393)bbg->bb463-(bb393)bbg
->bb185);bbm(bb1927==0 &&bbg->bb185==0 &&bbg->bb463==0 ){bb1927=bb1222;}
bb54 bbm(bb1927==(bbs)(-1 )){bb1927--;}bb54 bbm(bbg->bb185>=bb1222+((
bbg)->bb935-(258 +3 +1 ))){bb81(bbg->bb158,bbg->bb158+bb1222,(bbs)bb1222
);bbg->bb2023-=bb1222;bbg->bb185-=bb1222;bbg->bb429-=(bb6)bb1222;bb11
=bbg->bb1328;bb28=&bbg->bb383[bb11];bb573{bb95= * --bb28; *bb28=(
bb1039)(bb95>=bb1222?bb95-bb1222:0 );}bb106(--bb11);bb11=bb1222;bb28=&
bbg->bb968[bb11];bb573{bb95= * --bb28; *bb28=(bb1039)(bb95>=bb1222?
bb95-bb1222:0 );}bb106(--bb11);bb1927+=bb1222;}bbm(bbg->bb16->bb148==0
)bb2;;bb11=bb2434(bbg->bb16,bbg->bb158+bbg->bb185+bbg->bb463,bb1927);
bbg->bb463+=bb11;bbm(bbg->bb463>=3 ){bbg->bb499=bbg->bb158[bbg->bb185]
;(bbg->bb499=(((bbg->bb499)<<bbg->bb1610)^(bbg->bb158[bbg->bb185+1 ]))&
bbg->bb1381);}}bb106(bbg->bb463<(258 +3 +1 )&&bbg->bb16->bb148!=0 );}bb41
bb1823 bb2380(bbg,bb174)bb190*bbg;bbe bb174;{bb393 bb2273=0xffff ;
bb393 bb2152;bbm(bb2273>bbg->bb2039-5 ){bb2273=bbg->bb2039-5 ;}bb90(;;){
bbm(bbg->bb463<=1 ){;bb2189(bbg);bbm(bbg->bb463==0 &&bb174==0 )bb2 bb1248
;bbm(bbg->bb463==0 )bb21;};bbg->bb185+=bbg->bb463;bbg->bb463=0 ;bb2152=
bbg->bb429+bb2273;bbm(bbg->bb185==0 ||(bb393)bbg->bb185>=bb2152){bbg->
bb463=(bb7)(bbg->bb185-bb2152);bbg->bb185=(bb7)bb2152;{{bb1616(bbg,(
bbg->bb429>=0L ?(bb439* )&bbg->bb158[(bbs)bbg->bb429]:(bb439* )0 ),(
bb393)((bb6)bbg->bb185-bbg->bb429),(0 ));bbg->bb429=bbg->bb185;bb1266(
bbg->bb16);;};bbm(bbg->bb16->bb384==0 )bb2(0 )?bb1757:bb1248;};}bbm(bbg
->bb185-(bb7)bbg->bb429>=((bbg)->bb935-(258 +3 +1 ))){{{bb1616(bbg,(bbg
->bb429>=0L ?(bb439* )&bbg->bb158[(bbs)bbg->bb429]:(bb439* )0 ),(bb393)(
(bb6)bbg->bb185-bbg->bb429),(0 ));bbg->bb429=bbg->bb185;bb1266(bbg->
bb16);;};bbm(bbg->bb16->bb384==0 )bb2(0 )?bb1757:bb1248;};}}{{bb1616(
bbg,(bbg->bb429>=0L ?(bb439* )&bbg->bb158[(bbs)bbg->bb429]:(bb439* )0 ),
(bb393)((bb6)bbg->bb185-bbg->bb429),(bb174==4 ));bbg->bb429=bbg->bb185
;bb1266(bbg->bb16);;};bbm(bbg->bb16->bb384==0 )bb2(bb174==4 )?bb1757:
bb1248;};bb2 bb174==4 ?bb2138:bb2159;}bb41 bb1823 bb2161(bbg,bb174)bb190
 *bbg;bbe bb174;{bb1321 bb1136=0 ;bbe bb1763;bb90(;;){bbm(bbg->bb463<(
258 +3 +1 )){bb2189(bbg);bbm(bbg->bb463<(258 +3 +1 )&&bb174==0 ){bb2 bb1248;
}bbm(bbg->bb463==0 )bb21;}bbm(bbg->bb463>=3 ){((bbg->bb499=(((bbg->
bb499)<<bbg->bb1610)^(bbg->bb158[(bbg->bb185)+(3 -1 )]))&bbg->bb1381),
bbg->bb968[(bbg->bb185)&bbg->bb1808]=bb1136=bbg->bb383[bbg->bb499],
bbg->bb383[bbg->bb499]=(bb1039)(bbg->bb185));}bbm(bb1136!=0 &&bbg->
bb185-bb1136<=((bbg)->bb935-(258 +3 +1 ))){bbm(bbg->bb299!=2 ){bbg->bb951
=bb2258(bbg,bb1136);}}bbm(bbg->bb951>=3 ){;{bb155 bb22=(bbg->bb951-3 );
bb125 bb415=(bbg->bb185-bbg->bb2023);bbg->bb1641[bbg->bb605]=bb415;
bbg->bb1722[bbg->bb605++]=bb22;bb415--;bbg->bb971[bb2051[bb22]+256 +1 ]
.bb248.bb428++;bbg->bb1680[((bb415)<256 ?bb1735[bb415]:bb1735[256 +((
bb415)>>7 )])].bb248.bb428++;bb1763=(bbg->bb605==bbg->bb1126-1 );};bbg
->bb463-=bbg->bb951;bbm(bbg->bb951<=bbg->bb2055&&bbg->bb463>=3 ){bbg->
bb951--;bb573{bbg->bb185++;((bbg->bb499=(((bbg->bb499)<<bbg->bb1610)^
(bbg->bb158[(bbg->bb185)+(3 -1 )]))&bbg->bb1381),bbg->bb968[(bbg->bb185
)&bbg->bb1808]=bb1136=bbg->bb383[bbg->bb499],bbg->bb383[bbg->bb499]=(
bb1039)(bbg->bb185));}bb106(--bbg->bb951!=0 );bbg->bb185++;}bb54{bbg->
bb185+=bbg->bb951;bbg->bb951=0 ;bbg->bb499=bbg->bb158[bbg->bb185];(bbg
->bb499=(((bbg->bb499)<<bbg->bb1610)^(bbg->bb158[bbg->bb185+1 ]))&bbg
->bb1381);}}bb54{;{bb155 bb1841=(bbg->bb158[bbg->bb185]);bbg->bb1641[
bbg->bb605]=0 ;bbg->bb1722[bbg->bb605++]=bb1841;bbg->bb971[bb1841].
bb248.bb428++;bb1763=(bbg->bb605==bbg->bb1126-1 );};bbg->bb463--;bbg->
bb185++;}bbm(bb1763){{bb1616(bbg,(bbg->bb429>=0L ?(bb439* )&bbg->bb158
[(bbs)bbg->bb429]:(bb439* )0 ),(bb393)((bb6)bbg->bb185-bbg->bb429),(0 ));
bbg->bb429=bbg->bb185;bb1266(bbg->bb16);;};bbm(bbg->bb16->bb384==0 )bb2
(0 )?bb1757:bb1248;};}{{bb1616(bbg,(bbg->bb429>=0L ?(bb439* )&bbg->
bb158[(bbs)bbg->bb429]:(bb439* )0 ),(bb393)((bb6)bbg->bb185-bbg->bb429
),(bb174==4 ));bbg->bb429=bbg->bb185;bb1266(bbg->bb16);;};bbm(bbg->
bb16->bb384==0 )bb2(bb174==4 )?bb1757:bb1248;};bb2 bb174==4 ?bb2138:
bb2159;}bb41 bb1823 bb1947(bbg,bb174)bb190*bbg;bbe bb174;{bb1321
bb1136=0 ;bbe bb1763;bb90(;;){bbm(bbg->bb463<(258 +3 +1 )){bb2189(bbg);
bbm(bbg->bb463<(258 +3 +1 )&&bb174==0 ){bb2 bb1248;}bbm(bbg->bb463==0 )bb21
;}bbm(bbg->bb463>=3 ){((bbg->bb499=(((bbg->bb499)<<bbg->bb1610)^(bbg->
bb158[(bbg->bb185)+(3 -1 )]))&bbg->bb1381),bbg->bb968[(bbg->bb185)&bbg
->bb1808]=bb1136=bbg->bb383[bbg->bb499],bbg->bb383[bbg->bb499]=(
bb1039)(bbg->bb185));}bbg->bb1274=bbg->bb951,bbg->bb2299=bbg->bb2023;
bbg->bb951=3 -1 ;bbm(bb1136!=0 &&bbg->bb1274<bbg->bb2055&&bbg->bb185-
bb1136<=((bbg)->bb935-(258 +3 +1 ))){bbm(bbg->bb299!=2 ){bbg->bb951=
bb2258(bbg,bb1136);}bbm(bbg->bb951<=5 &&(bbg->bb299==1 ||(bbg->bb951==3
&&bbg->bb185-bbg->bb2023>4096 ))){bbg->bb951=3 -1 ;}}bbm(bbg->bb1274>=3
&&bbg->bb951<=bbg->bb1274){bb7 bb2492=bbg->bb185+bbg->bb463-3 ;;{bb155
bb22=(bbg->bb1274-3 );bb125 bb415=(bbg->bb185-1 -bbg->bb2299);bbg->
bb1641[bbg->bb605]=bb415;bbg->bb1722[bbg->bb605++]=bb22;bb415--;bbg->
bb971[bb2051[bb22]+256 +1 ].bb248.bb428++;bbg->bb1680[((bb415)<256 ?
bb1735[bb415]:bb1735[256 +((bb415)>>7 )])].bb248.bb428++;bb1763=(bbg->
bb605==bbg->bb1126-1 );};bbg->bb463-=bbg->bb1274-1 ;bbg->bb1274-=2 ;
bb573{bbm(++bbg->bb185<=bb2492){((bbg->bb499=(((bbg->bb499)<<bbg->
bb1610)^(bbg->bb158[(bbg->bb185)+(3 -1 )]))&bbg->bb1381),bbg->bb968[(
bbg->bb185)&bbg->bb1808]=bb1136=bbg->bb383[bbg->bb499],bbg->bb383[bbg
->bb499]=(bb1039)(bbg->bb185));}}bb106(--bbg->bb1274!=0 );bbg->bb1940=
0 ;bbg->bb951=3 -1 ;bbg->bb185++;bbm(bb1763){{bb1616(bbg,(bbg->bb429>=0L
?(bb439* )&bbg->bb158[(bbs)bbg->bb429]:(bb439* )0 ),(bb393)((bb6)bbg->
bb185-bbg->bb429),(0 ));bbg->bb429=bbg->bb185;bb1266(bbg->bb16);;};bbm
(bbg->bb16->bb384==0 )bb2(0 )?bb1757:bb1248;};}bb54 bbm(bbg->bb1940){;{
bb155 bb1841=(bbg->bb158[bbg->bb185-1 ]);bbg->bb1641[bbg->bb605]=0 ;bbg
->bb1722[bbg->bb605++]=bb1841;bbg->bb971[bb1841].bb248.bb428++;bb1763
=(bbg->bb605==bbg->bb1126-1 );};bbm(bb1763){{bb1616(bbg,(bbg->bb429>=
0L ?(bb439* )&bbg->bb158[(bbs)bbg->bb429]:(bb439* )0 ),(bb393)((bb6)bbg
->bb185-bbg->bb429),(0 ));bbg->bb429=bbg->bb185;bb1266(bbg->bb16);;};}
bbg->bb185++;bbg->bb463--;bbm(bbg->bb16->bb384==0 )bb2 bb1248;}bb54{
bbg->bb1940=1 ;bbg->bb185++;bbg->bb463--;}};bbm(bbg->bb1940){;{bb155
bb1841=(bbg->bb158[bbg->bb185-1 ]);bbg->bb1641[bbg->bb605]=0 ;bbg->
bb1722[bbg->bb605++]=bb1841;bbg->bb971[bb1841].bb248.bb428++;bb1763=(
bbg->bb605==bbg->bb1126-1 );};bbg->bb1940=0 ;}{{bb1616(bbg,(bbg->bb429
>=0L ?(bb439* )&bbg->bb158[(bbs)bbg->bb429]:(bb439* )0 ),(bb393)((bb6)bbg
->bb185-bbg->bb429),(bb174==4 ));bbg->bb429=bbg->bb185;bb1266(bbg->
bb16);;};bbm(bbg->bb16->bb384==0 )bb2(bb174==4 )?bb1757:bb1248;};bb2
bb174==4 ?bb2138:bb2159;}
