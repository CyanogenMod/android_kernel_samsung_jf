/*
   'src_compress_deflate_trees.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Wed Dec 18 14:11:11 2013
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
bb1049 bbp((bb17 bb16,bbe bb452,bbh bbl*bb189,bbe bb196));bbr bbh bbl
 *bb1165 bbp((bbe bb18));bbr bbe bb1173 bbp((bb17 bby));bbr bbh bb167
 *bb1163 bbp((bbb));
#ifdef __cplusplus
}
#endif
#define bb963 1
#ifdef bb136
#if defined( bb1742)
#else
#endif
#endif
bba bbs bbl bb155;bba bb155 bb1205;bba bbs bb135 bb126;bba bb126 bb500
;bba bbs bb6 bb389;bbr bbh bbl*bb1071[10 ];
#if bb206 >= 8
#define bb803 8
#else
#define bb803 bb206
#endif
#ifdef bb169
#define bb421 0x00
#if defined( __TURBOC__) || defined( __BORLANDC__)
#if( __STDC__ == 1) && ( defined( bb1792) || defined( bb1771))
bbb bb938 bb1310(bbb*bb102);bbb*bb938 bb1344(bbs bb6 bb1740);
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
#define bb421 0x0b
#endif
#if ( defined( _MSC_VER) && ( _MSC_VER > 600))
#define bb1752( bb472, bb129) bb1788( bb472, bb129)
#endif
#ifndef bb421
#define bb421 0x03
#endif
#if defined( bb1549) && ! defined( _MSC_VER) && ! defined( bb1773)
#define bb963
#endif
bba bb24( *bb955)bbp((bb24 bb487,bbh bb30*bb40,bb7 bb21));bb70 bb1175
bbp((bb70 bb110,bbs bb496,bbs bb47));bbb bb1160 bbp((bb70 bb110,bb70
bb911));bba bbi bb1953{bb559{bb126 bb429;bb126 bb170;}bb282;bb559{
bb126 bb2161;bb126 bb21;}bb48;}bb458;bba bbi bb2278 bb2006;bba bbi
bb1940{bb458*bb1748;bbe bb513;bb2006*bb1719;}bb1734;bba bb126 bb1070;
bba bb1070 bb1354;bba bbs bb1313;bba bbi bb381{bb17 bb16;bbe bb359;
bb30*bb172;bb389 bb2039;bb30*bb1873;bbe bb185;bbe bb1350;bb152 bb969;
bb152 bb569;bbe bb1894;bb7 bb934;bb7 bb2162;bb7 bb1807;bb30*bb158;
bb389 bb2227;bb1354*bb966;bb1354*bb385;bb7 bb492;bb7 bb1323;bb7 bb2130
;bb7 bb1380;bb7 bb1464;bb6 bb427;bb7 bb957;bb1313 bb2299;bbe bb1939;
bb7 bb183;bb7 bb2023;bb7 bb462;bb7 bb1277;bb7 bb2175;bb7 bb2055;bbe
bb123;bbe bb295;bb7 bb2180;bbe bb1837;bbi bb1953 bb970[(2 * (256 +1 +29 )+
1 )];bbi bb1953 bb1682[2 *30 +1 ];bbi bb1953 bb523[2 *19 +1 ];bbi bb1940
bb1937;bbi bb1940 bb1862;bbi bb1940 bb2063;bb126 bb1196[15 +1 ];bbe
bb514[2 * (256 +1 +29 )+1 ];bbe bb1507;bbe bb1944;bb155 bb1240[2 * (256 +1 +
29 )+1 ];bb1205*bb1725;bb7 bb1126;bb7 bb609;bb500*bb1645;bb389 bb1906;
bb389 bb2118;bb7 bb2231;bbe bb1992;bb126 bb99;bbe bb78;}bb192;bbb
bb2209 bbp((bb192*bbg));bbe bb2389 bbp((bb192*bbg,bbs bb415,bbs bb1139
));bbb bb1619 bbp((bb192*bbg,bb439*bb40,bb389 bb1291,bbe bb1115));bbb
bb2256 bbp((bb192*bbg));bbb bb2141 bbp((bb192*bbg,bb439*bb40,bb389
bb1291,bbe bb1115));bb41 bbh bbe bb2413[29 ]={0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,1 ,1 ,1 ,
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
bb41 bbh bb458 bb2234[30 ]={{{0 },{5 }},{{16 },{5 }},{{8 },{5 }},{{24 },{5 }},
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
,29 ,29 ,29 ,29 ,29 };bb41 bbh bb155 bb2052[258 -3 +1 ]={0 ,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,8 ,
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
,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,27 ,28 };bb41 bbh bbe bb2333[29
]={0 ,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,10 ,12 ,14 ,16 ,20 ,24 ,28 ,32 ,40 ,48 ,56 ,64 ,80 ,96 ,112 ,
128 ,160 ,192 ,224 ,0 };bb41 bbh bbe bb2416[30 ]={0 ,1 ,2 ,3 ,4 ,6 ,8 ,12 ,16 ,24 ,32
,48 ,64 ,96 ,128 ,192 ,256 ,384 ,512 ,768 ,1024 ,1536 ,2048 ,3072 ,4096 ,6144 ,8192 ,
12288 ,16384 ,24576 };bbi bb2278{bbh bb458*bb2345;bbh bb1141*bb2472;bbe
bb2452;bbe bb2174;bbe bb1941;};bb41 bb2006 bb2490={bb1738,bb2413,256 +
1 ,(256 +1 +29 ),15 };bb41 bb2006 bb2558={bb2234,bb2379,0 ,30 ,15 };bb41
bb2006 bb2498={(bbh bb458* )0 ,bb2479,0 ,19 ,7 };bb41 bbb bb2250 bbp((
bb192*bbg));bb41 bbb bb2169 bbp((bb192*bbg,bb458*bb303,bbe bb3));bb41
bbb bb2367 bbp((bb192*bbg,bb1734*bb1060));bb41 bbb bb2394 bbp((bb458*
bb303,bbe bb513,bb500*bb1196));bb41 bbb bb2184 bbp((bb192*bbg,bb1734*
bb1060));bb41 bbb bb2307 bbp((bb192*bbg,bb458*bb303,bbe bb513));bb41
bbb bb2232 bbp((bb192*bbg,bb458*bb303,bbe bb513));bb41 bbe bb2385 bbp
((bb192*bbg));bb41 bbb bb2405 bbp((bb192*bbg,bbe bb2051,bbe bb2093,
bbe bb2045));bb41 bbb bb2274 bbp((bb192*bbg,bb458*bb1078,bb458*bb1746
));bb41 bbb bb2314 bbp((bb192*bbg));bb41 bbs bb2373 bbp((bbs bb1320,
bbe bb464));bb41 bbb bb2259 bbp((bb192*bbg));bb41 bbb bb2304 bbp((
bb192*bbg));bb41 bbb bb2319 bbp((bb192*bbg,bb439*bb40,bbs bb21,bbe
bb980));bbb bb2209(bbg)bb192*bbg;{bbg->bb1937.bb1748=bbg->bb970;bbg->
bb1937.bb1719=&bb2490;bbg->bb1862.bb1748=bbg->bb1682;bbg->bb1862.
bb1719=&bb2558;bbg->bb2063.bb1748=bbg->bb523;bbg->bb2063.bb1719=&
bb2498;bbg->bb99=0 ;bbg->bb78=0 ;bbg->bb1992=8 ;bb2250(bbg);}bb41 bbb
bb2250(bbg)bb192*bbg;{bbe bb11;bb90(bb11=0 ;bb11<(256 +1 +29 );bb11++)bbg
->bb970[bb11].bb282.bb429=0 ;bb90(bb11=0 ;bb11<30 ;bb11++)bbg->bb1682[
bb11].bb282.bb429=0 ;bb90(bb11=0 ;bb11<19 ;bb11++)bbg->bb523[bb11].bb282
.bb429=0 ;bbg->bb970[256 ].bb282.bb429=1 ;bbg->bb1906=bbg->bb2118=0L ;bbg
->bb609=bbg->bb2231=0 ;}bb41 bbb bb2169(bbg,bb303,bb3)bb192*bbg;bb458*
bb303;bbe bb3;{bbe bb441=bbg->bb514[bb3];bbe bb76=bb3<<1 ;bb106(bb76<=
bbg->bb1507){bbm(bb76<bbg->bb1507&&(bb303[bbg->bb514[bb76+1 ]].bb282.
bb429<bb303[bbg->bb514[bb76]].bb282.bb429||(bb303[bbg->bb514[bb76+1 ]]
.bb282.bb429==bb303[bbg->bb514[bb76]].bb282.bb429&&bbg->bb1240[bbg->
bb514[bb76+1 ]]<=bbg->bb1240[bbg->bb514[bb76]]))){bb76++;}bbm((bb303[
bb441].bb282.bb429<bb303[bbg->bb514[bb76]].bb282.bb429||(bb303[bb441]
.bb282.bb429==bb303[bbg->bb514[bb76]].bb282.bb429&&bbg->bb1240[bb441]
<=bbg->bb1240[bbg->bb514[bb76]])))bb22;bbg->bb514[bb3]=bbg->bb514[
bb76];bb3=bb76;bb76<<=1 ;}bbg->bb514[bb3]=bb441;}bb41 bbb bb2367(bbg,
bb1060)bb192*bbg;bb1734*bb1060;{bb458*bb303=bb1060->bb1748;bbe bb513=
bb1060->bb513;bbh bb458*bb2115=bb1060->bb1719->bb2345;bbh bb1141*
bb1799=bb1060->bb1719->bb2472;bbe bb604=bb1060->bb1719->bb2452;bbe
bb1941=bb1060->bb1719->bb1941;bbe bb42;bbe bb11,bb97;bbe bb516;bbe
bb2142;bb126 bb20;bbe bb2137=0 ;bb90(bb516=0 ;bb516<=15 ;bb516++)bbg->
bb1196[bb516]=0 ;bb303[bbg->bb514[bbg->bb1944]].bb48.bb21=0 ;bb90(bb42=
bbg->bb1944+1 ;bb42<(2 * (256 +1 +29 )+1 );bb42++){bb11=bbg->bb514[bb42];
bb516=bb303[bb303[bb11].bb48.bb2161].bb48.bb21+1 ;bbm(bb516>bb1941)bb516
=bb1941,bb2137++;bb303[bb11].bb48.bb21=(bb126)bb516;bbm(bb11>bb513)bb1690
;bbg->bb1196[bb516]++;bb2142=0 ;bbm(bb11>=bb604)bb2142=bb1799[bb11-
bb604];bb20=bb303[bb11].bb282.bb429;bbg->bb1906+=(bb389)bb20* (bb516+
bb2142);bbm(bb2115)bbg->bb2118+=(bb389)bb20* (bb2115[bb11].bb48.bb21+
bb2142);}bbm(bb2137==0 )bb2;;bb565{bb516=bb1941-1 ;bb106(bbg->bb1196[
bb516]==0 )bb516--;bbg->bb1196[bb516]--;bbg->bb1196[bb516+1 ]+=2 ;bbg->
bb1196[bb1941]--;bb2137-=2 ;}bb106(bb2137>0 );bb90(bb516=bb1941;bb516!=
0 ;bb516--){bb11=bbg->bb1196[bb516];bb106(bb11!=0 ){bb97=bbg->bb514[--
bb42];bbm(bb97>bb513)bb1690;bbm(bb303[bb97].bb48.bb21!=(bbs)bb516){;
bbg->bb1906+=((bb6)bb516-(bb6)bb303[bb97].bb48.bb21) * (bb6)bb303[
bb97].bb282.bb429;bb303[bb97].bb48.bb21=(bb126)bb516;}bb11--;}}}bb41
bbb bb2394(bb303,bb513,bb1196)bb458*bb303;bbe bb513;bb500*bb1196;{
bb126 bb2403[15 +1 ];bb126 bb170=0 ;bbe bb516;bbe bb11;bb90(bb516=1 ;
bb516<=15 ;bb516++){bb2403[bb516]=bb170=(bb170+bb1196[bb516-1 ])<<1 ;};;
bb90(bb11=0 ;bb11<=bb513;bb11++){bbe bb21=bb303[bb11].bb48.bb21;bbm(
bb21==0 )bb1690;bb303[bb11].bb282.bb170=bb2373(bb2403[bb21]++,bb21);;}
}bb41 bbb bb2184(bbg,bb1060)bb192*bbg;bb1734*bb1060;{bb458*bb303=
bb1060->bb1748;bbh bb458*bb2115=bb1060->bb1719->bb2345;bbe bb2174=
bb1060->bb1719->bb2174;bbe bb11,bb97;bbe bb513=-1 ;bbe bb1776;bbg->
bb1507=0 ,bbg->bb1944=(2 * (256 +1 +29 )+1 );bb90(bb11=0 ;bb11<bb2174;bb11++
){bbm(bb303[bb11].bb282.bb429!=0 ){bbg->bb514[++(bbg->bb1507)]=bb513=
bb11;bbg->bb1240[bb11]=0 ;}bb54{bb303[bb11].bb48.bb21=0 ;}}bb106(bbg->
bb1507<2 ){bb1776=bbg->bb514[++(bbg->bb1507)]=(bb513<2 ?++bb513:0 );
bb303[bb1776].bb282.bb429=1 ;bbg->bb1240[bb1776]=0 ;bbg->bb1906--;bbm(
bb2115)bbg->bb2118-=bb2115[bb1776].bb48.bb21;}bb1060->bb513=bb513;
bb90(bb11=bbg->bb1507/2 ;bb11>=1 ;bb11--)bb2169(bbg,bb303,bb11);bb1776=
bb2174;bb565{{bb11=bbg->bb514[1 ];bbg->bb514[1 ]=bbg->bb514[bbg->bb1507
--];bb2169(bbg,bb303,1 );};bb97=bbg->bb514[1 ];bbg->bb514[--(bbg->
bb1944)]=bb11;bbg->bb514[--(bbg->bb1944)]=bb97;bb303[bb1776].bb282.
bb429=bb303[bb11].bb282.bb429+bb303[bb97].bb282.bb429;bbg->bb1240[
bb1776]=(bb155)((bbg->bb1240[bb11]>=bbg->bb1240[bb97]?bbg->bb1240[
bb11]:bbg->bb1240[bb97])+1 );bb303[bb11].bb48.bb2161=bb303[bb97].bb48.
bb2161=(bb126)bb1776;bbg->bb514[1 ]=bb1776++;bb2169(bbg,bb303,1 );}
bb106(bbg->bb1507>=2 );bbg->bb514[--(bbg->bb1944)]=bbg->bb514[1 ];
bb2367(bbg,(bb1734* )bb1060);bb2394((bb458* )bb303,bb513,bbg->bb1196);
}bb41 bbb bb2307(bbg,bb303,bb513)bb192*bbg;bb458*bb303;bbe bb513;{bbe
bb11;bbe bb2084=-1 ;bbe bb851;bbe bb1216=bb303[0 ].bb48.bb21;bbe bb971=
0 ;bbe bb1334=7 ;bbe bb1343=4 ;bbm(bb1216==0 )bb1334=138 ,bb1343=3 ;bb303[
bb513+1 ].bb48.bb21=(bb126)0xffff ;bb90(bb11=0 ;bb11<=bb513;bb11++){
bb851=bb1216;bb1216=bb303[bb11+1 ].bb48.bb21;bbm(++bb971<bb1334&&bb851
==bb1216){bb1690;}bb54 bbm(bb971<bb1343){bbg->bb523[bb851].bb282.
bb429+=bb971;}bb54 bbm(bb851!=0 ){bbm(bb851!=bb2084)bbg->bb523[bb851].
bb282.bb429++;bbg->bb523[16 ].bb282.bb429++;}bb54 bbm(bb971<=10 ){bbg->
bb523[17 ].bb282.bb429++;}bb54{bbg->bb523[18 ].bb282.bb429++;}bb971=0 ;
bb2084=bb851;bbm(bb1216==0 ){bb1334=138 ,bb1343=3 ;}bb54 bbm(bb851==
bb1216){bb1334=6 ,bb1343=3 ;}bb54{bb1334=7 ,bb1343=4 ;}}}bb41 bbb bb2232(
bbg,bb303,bb513)bb192*bbg;bb458*bb303;bbe bb513;{bbe bb11;bbe bb2084=
-1 ;bbe bb851;bbe bb1216=bb303[0 ].bb48.bb21;bbe bb971=0 ;bbe bb1334=7 ;
bbe bb1343=4 ;bbm(bb1216==0 )bb1334=138 ,bb1343=3 ;bb90(bb11=0 ;bb11<=
bb513;bb11++){bb851=bb1216;bb1216=bb303[bb11+1 ].bb48.bb21;bbm(++bb971
<bb1334&&bb851==bb1216){bb1690;}bb54 bbm(bb971<bb1343){bb565{{bbe bb21
=bbg->bb523[bb851].bb48.bb21;bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl))-bb21){
bbe bb171=bbg->bb523[bb851].bb282.bb170;bbg->bb99|=(bb171<<bbg->bb78);
{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&0xff ));};{bbg->bb172[
bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));};};bbg->bb99=(bb126)bb171
>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78+=bb21-(8 *2 *bb12(bbl));}
bb54{bbg->bb99|=(bbg->bb523[bb851].bb282.bb170)<<bbg->bb78;bbg->bb78
+=bb21;}};}bb106(--bb971!=0 );}bb54 bbm(bb851!=0 ){bbm(bb851!=bb2084){{
bbe bb21=bbg->bb523[bb851].bb48.bb21;bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl
))-bb21){bbe bb171=bbg->bb523[bb851].bb282.bb170;bbg->bb99|=(bb171<<
bbg->bb78);{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&0xff ));};{
bbg->bb172[bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));};};bbg->
bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78+=bb21-(
8 *2 *bb12(bbl));}bb54{bbg->bb99|=(bbg->bb523[bb851].bb282.bb170)<<bbg
->bb78;bbg->bb78+=bb21;}};bb971--;};{bbe bb21=bbg->bb523[16 ].bb48.
bb21;bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl))-bb21){bbe bb171=bbg->bb523[16
].bb282.bb170;bbg->bb99|=(bb171<<bbg->bb78);{{bbg->bb172[bbg->bb185++
]=((bb155)((bbg->bb99)&0xff ));};{bbg->bb172[bbg->bb185++]=((bb155)((
bb126)(bbg->bb99)>>8 ));};};bbg->bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(
bbl))-bbg->bb78);bbg->bb78+=bb21-(8 *2 *bb12(bbl));}bb54{bbg->bb99|=(
bbg->bb523[16 ].bb282.bb170)<<bbg->bb78;bbg->bb78+=bb21;}};{bbe bb21=2
;bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl))-bb21){bbe bb171=bb971-3 ;bbg->bb99
|=(bb171<<bbg->bb78);{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&
0xff ));};{bbg->bb172[bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));}
;};bbg->bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78
+=bb21-(8 *2 *bb12(bbl));}bb54{bbg->bb99|=(bb971-3 )<<bbg->bb78;bbg->
bb78+=bb21;}};}bb54 bbm(bb971<=10 ){{bbe bb21=bbg->bb523[17 ].bb48.bb21
;bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl))-bb21){bbe bb171=bbg->bb523[17 ].
bb282.bb170;bbg->bb99|=(bb171<<bbg->bb78);{{bbg->bb172[bbg->bb185++]=
((bb155)((bbg->bb99)&0xff ));};{bbg->bb172[bbg->bb185++]=((bb155)((
bb126)(bbg->bb99)>>8 ));};};bbg->bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(
bbl))-bbg->bb78);bbg->bb78+=bb21-(8 *2 *bb12(bbl));}bb54{bbg->bb99|=(
bbg->bb523[17 ].bb282.bb170)<<bbg->bb78;bbg->bb78+=bb21;}};{bbe bb21=3
;bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl))-bb21){bbe bb171=bb971-3 ;bbg->bb99
|=(bb171<<bbg->bb78);{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&
0xff ));};{bbg->bb172[bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));}
;};bbg->bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78
+=bb21-(8 *2 *bb12(bbl));}bb54{bbg->bb99|=(bb971-3 )<<bbg->bb78;bbg->
bb78+=bb21;}};}bb54{{bbe bb21=bbg->bb523[18 ].bb48.bb21;bbm(bbg->bb78>
(bbe)(8 *2 *bb12(bbl))-bb21){bbe bb171=bbg->bb523[18 ].bb282.bb170;bbg->
bb99|=(bb171<<bbg->bb78);{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->
bb99)&0xff ));};{bbg->bb172[bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>
8 ));};};bbg->bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg
->bb78+=bb21-(8 *2 *bb12(bbl));}bb54{bbg->bb99|=(bbg->bb523[18 ].bb282.
bb170)<<bbg->bb78;bbg->bb78+=bb21;}};{bbe bb21=7 ;bbm(bbg->bb78>(bbe)(
8 *2 *bb12(bbl))-bb21){bbe bb171=bb971-11 ;bbg->bb99|=(bb171<<bbg->bb78);
{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&0xff ));};{bbg->bb172[
bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));};};bbg->bb99=(bb126)bb171
>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78+=bb21-(8 *2 *bb12(bbl));}
bb54{bbg->bb99|=(bb971-11 )<<bbg->bb78;bbg->bb78+=bb21;}};}bb971=0 ;
bb2084=bb851;bbm(bb1216==0 ){bb1334=138 ,bb1343=3 ;}bb54 bbm(bb851==
bb1216){bb1334=6 ,bb1343=3 ;}bb54{bb1334=7 ,bb1343=4 ;}}}bb41 bbe bb2385(
bbg)bb192*bbg;{bbe bb1784;bb2307(bbg,(bb458* )bbg->bb970,bbg->bb1937.
bb513);bb2307(bbg,(bb458* )bbg->bb1682,bbg->bb1862.bb513);bb2184(bbg,
(bb1734* )(&(bbg->bb2063)));bb90(bb1784=19 -1 ;bb1784>=3 ;bb1784--){bbm(
bbg->bb523[bb2197[bb1784]].bb48.bb21!=0 )bb22;}bbg->bb1906+=3 * (bb1784
+1 )+5 +5 +4 ;;bb2 bb1784;}bb41 bbb bb2405(bbg,bb2051,bb2093,bb2045)bb192
 *bbg;bbe bb2051,bb2093,bb2045;{bbe bb2120;;;;{bbe bb21=5 ;bbm(bbg->
bb78>(bbe)(8 *2 *bb12(bbl))-bb21){bbe bb171=bb2051-257 ;bbg->bb99|=(
bb171<<bbg->bb78);{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&
0xff ));};{bbg->bb172[bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));}
;};bbg->bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78
+=bb21-(8 *2 *bb12(bbl));}bb54{bbg->bb99|=(bb2051-257 )<<bbg->bb78;bbg->
bb78+=bb21;}};{bbe bb21=5 ;bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl))-bb21){
bbe bb171=bb2093-1 ;bbg->bb99|=(bb171<<bbg->bb78);{{bbg->bb172[bbg->
bb185++]=((bb155)((bbg->bb99)&0xff ));};{bbg->bb172[bbg->bb185++]=((
bb155)((bb126)(bbg->bb99)>>8 ));};};bbg->bb99=(bb126)bb171>>((bbe)(8 *2
 *bb12(bbl))-bbg->bb78);bbg->bb78+=bb21-(8 *2 *bb12(bbl));}bb54{bbg->
bb99|=(bb2093-1 )<<bbg->bb78;bbg->bb78+=bb21;}};{bbe bb21=4 ;bbm(bbg->
bb78>(bbe)(8 *2 *bb12(bbl))-bb21){bbe bb171=bb2045-4 ;bbg->bb99|=(bb171
<<bbg->bb78);{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&0xff ));}
;{bbg->bb172[bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));};};bbg->
bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78+=bb21-(
8 *2 *bb12(bbl));}bb54{bbg->bb99|=(bb2045-4 )<<bbg->bb78;bbg->bb78+=bb21
;}};bb90(bb2120=0 ;bb2120<bb2045;bb2120++){;{bbe bb21=3 ;bbm(bbg->bb78>
(bbe)(8 *2 *bb12(bbl))-bb21){bbe bb171=bbg->bb523[bb2197[bb2120]].bb48.
bb21;bbg->bb99|=(bb171<<bbg->bb78);{{bbg->bb172[bbg->bb185++]=((bb155
)((bbg->bb99)&0xff ));};{bbg->bb172[bbg->bb185++]=((bb155)((bb126)(bbg
->bb99)>>8 ));};};bbg->bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(bbl))-bbg->
bb78);bbg->bb78+=bb21-(8 *2 *bb12(bbl));}bb54{bbg->bb99|=(bbg->bb523[
bb2197[bb2120]].bb48.bb21)<<bbg->bb78;bbg->bb78+=bb21;}};};bb2232(bbg
,(bb458* )bbg->bb970,bb2051-1 );;bb2232(bbg,(bb458* )bbg->bb1682,
bb2093-1 );;}bbb bb2141(bbg,bb40,bb1291,bb1115)bb192*bbg;bb439*bb40;
bb389 bb1291;bbe bb1115;{{bbe bb21=3 ;bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl
))-bb21){bbe bb171=(0 <<1 )+bb1115;bbg->bb99|=(bb171<<bbg->bb78);{{bbg
->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&0xff ));};{bbg->bb172[bbg->
bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));};};bbg->bb99=(bb126)bb171
>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78+=bb21-(8 *2 *bb12(bbl));}
bb54{bbg->bb99|=((0 <<1 )+bb1115)<<bbg->bb78;bbg->bb78+=bb21;}};bb2319(
bbg,bb40,(bbs)bb1291,1 );}bbb bb2256(bbg)bb192*bbg;{{bbe bb21=3 ;bbm(
bbg->bb78>(bbe)(8 *2 *bb12(bbl))-bb21){bbe bb171=1 <<1 ;bbg->bb99|=(bb171
<<bbg->bb78);{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&0xff ));}
;{bbg->bb172[bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));};};bbg->
bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78+=bb21-(
8 *2 *bb12(bbl));}bb54{bbg->bb99|=(1 <<1 )<<bbg->bb78;bbg->bb78+=bb21;}};
{bbe bb21=bb1738[256 ].bb48.bb21;bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl))-
bb21){bbe bb171=bb1738[256 ].bb282.bb170;bbg->bb99|=(bb171<<bbg->bb78);
{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&0xff ));};{bbg->bb172[
bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));};};bbg->bb99=(bb126)bb171
>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78+=bb21-(8 *2 *bb12(bbl));}
bb54{bbg->bb99|=(bb1738[256 ].bb282.bb170)<<bbg->bb78;bbg->bb78+=bb21;
}};bb2304(bbg);bbm(1 +bbg->bb1992+10 -bbg->bb78<9 ){{bbe bb21=3 ;bbm(bbg
->bb78>(bbe)(8 *2 *bb12(bbl))-bb21){bbe bb171=1 <<1 ;bbg->bb99|=(bb171<<
bbg->bb78);{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&0xff ));};{
bbg->bb172[bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));};};bbg->
bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78+=bb21-(
8 *2 *bb12(bbl));}bb54{bbg->bb99|=(1 <<1 )<<bbg->bb78;bbg->bb78+=bb21;}};
{bbe bb21=bb1738[256 ].bb48.bb21;bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl))-
bb21){bbe bb171=bb1738[256 ].bb282.bb170;bbg->bb99|=(bb171<<bbg->bb78);
{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&0xff ));};{bbg->bb172[
bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));};};bbg->bb99=(bb126)bb171
>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78+=bb21-(8 *2 *bb12(bbl));}
bb54{bbg->bb99|=(bb1738[256 ].bb282.bb170)<<bbg->bb78;bbg->bb78+=bb21;
}};bb2304(bbg);}bbg->bb1992=7 ;}bbb bb1619(bbg,bb40,bb1291,bb1115)bb192
 *bbg;bb439*bb40;bb389 bb1291;bbe bb1115;{bb389 bb2012,bb2077;bbe
bb1784=0 ;bbm(bbg->bb123>0 ){bbm(bbg->bb969==2 )bb2314(bbg);bb2184(bbg,(
bb1734* )(&(bbg->bb1937)));;bb2184(bbg,(bb1734* )(&(bbg->bb1862)));;
bb1784=bb2385(bbg);bb2012=(bbg->bb1906+3 +7 )>>3 ;bb2077=(bbg->bb2118+3 +
7 )>>3 ;;bbm(bb2077<=bb2012)bb2012=bb2077;}bb54{;bb2012=bb2077=bb1291+5
;}bbm(bb1291+4 <=bb2012&&bb40!=(bbl* )0 ){bb2141(bbg,bb40,bb1291,bb1115
);}bb54 bbm(bb2077==bb2012){{bbe bb21=3 ;bbm(bbg->bb78>(bbe)(8 *2 *bb12(
bbl))-bb21){bbe bb171=(1 <<1 )+bb1115;bbg->bb99|=(bb171<<bbg->bb78);{{
bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&0xff ));};{bbg->bb172[
bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));};};bbg->bb99=(bb126)bb171
>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78+=bb21-(8 *2 *bb12(bbl));}
bb54{bbg->bb99|=((1 <<1 )+bb1115)<<bbg->bb78;bbg->bb78+=bb21;}};bb2274(
bbg,(bb458* )bb1738,(bb458* )bb2234);}bb54{{bbe bb21=3 ;bbm(bbg->bb78>
(bbe)(8 *2 *bb12(bbl))-bb21){bbe bb171=(2 <<1 )+bb1115;bbg->bb99|=(bb171
<<bbg->bb78);{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&0xff ));}
;{bbg->bb172[bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));};};bbg->
bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78+=bb21-(
8 *2 *bb12(bbl));}bb54{bbg->bb99|=((2 <<1 )+bb1115)<<bbg->bb78;bbg->bb78
+=bb21;}};bb2405(bbg,bbg->bb1937.bb513+1 ,bbg->bb1862.bb513+1 ,bb1784+1
);bb2274(bbg,(bb458* )bbg->bb970,(bb458* )bbg->bb1682);};bb2250(bbg);
bbm(bb1115){bb2259(bbg);};}bbe bb2389(bbg,bb415,bb1139)bb192*bbg;bbs
bb415;bbs bb1139;{bbg->bb1645[bbg->bb609]=(bb126)bb415;bbg->bb1725[
bbg->bb609++]=(bb155)bb1139;bbm(bb415==0 ){bbg->bb970[bb1139].bb282.
bb429++;}bb54{bbg->bb2231++;bb415--;;bbg->bb970[bb2052[bb1139]+256 +1 ]
.bb282.bb429++;bbg->bb1682[((bb415)<256 ?bb1735[bb415]:bb1735[256 +((
bb415)>>7 )])].bb282.bb429++;}bb2(bbg->bb609==bbg->bb1126-1 );}bb41 bbb
bb2274(bbg,bb1078,bb1746)bb192*bbg;bb458*bb1078;bb458*bb1746;{bbs
bb415;bbe bb1139;bbs bb2288=0 ;bbs bb170;bbe bb1799;bbm(bbg->bb609!=0 )bb565
{bb415=bbg->bb1645[bb2288];bb1139=bbg->bb1725[bb2288++];bbm(bb415==0 ){
{bbe bb21=bb1078[bb1139].bb48.bb21;bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl))-
bb21){bbe bb171=bb1078[bb1139].bb282.bb170;bbg->bb99|=(bb171<<bbg->
bb78);{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&0xff ));};{bbg->
bb172[bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));};};bbg->bb99=(
bb126)bb171>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78+=bb21-(8 *2 *
bb12(bbl));}bb54{bbg->bb99|=(bb1078[bb1139].bb282.bb170)<<bbg->bb78;
bbg->bb78+=bb21;}};;}bb54{bb170=bb2052[bb1139];{bbe bb21=bb1078[bb170
+256 +1 ].bb48.bb21;bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl))-bb21){bbe bb171=
bb1078[bb170+256 +1 ].bb282.bb170;bbg->bb99|=(bb171<<bbg->bb78);{{bbg->
bb172[bbg->bb185++]=((bb155)((bbg->bb99)&0xff ));};{bbg->bb172[bbg->
bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));};};bbg->bb99=(bb126)bb171
>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78+=bb21-(8 *2 *bb12(bbl));}
bb54{bbg->bb99|=(bb1078[bb170+256 +1 ].bb282.bb170)<<bbg->bb78;bbg->
bb78+=bb21;}};bb1799=bb2413[bb170];bbm(bb1799!=0 ){bb1139-=bb2333[
bb170];{bbe bb21=bb1799;bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl))-bb21){bbe
bb171=bb1139;bbg->bb99|=(bb171<<bbg->bb78);{{bbg->bb172[bbg->bb185++]
=((bb155)((bbg->bb99)&0xff ));};{bbg->bb172[bbg->bb185++]=((bb155)((
bb126)(bbg->bb99)>>8 ));};};bbg->bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(
bbl))-bbg->bb78);bbg->bb78+=bb21-(8 *2 *bb12(bbl));}bb54{bbg->bb99|=(
bb1139)<<bbg->bb78;bbg->bb78+=bb21;}};}bb415--;bb170=((bb415)<256 ?
bb1735[bb415]:bb1735[256 +((bb415)>>7 )]);;{bbe bb21=bb1746[bb170].bb48
.bb21;bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl))-bb21){bbe bb171=bb1746[bb170
].bb282.bb170;bbg->bb99|=(bb171<<bbg->bb78);{{bbg->bb172[bbg->bb185++
]=((bb155)((bbg->bb99)&0xff ));};{bbg->bb172[bbg->bb185++]=((bb155)((
bb126)(bbg->bb99)>>8 ));};};bbg->bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(
bbl))-bbg->bb78);bbg->bb78+=bb21-(8 *2 *bb12(bbl));}bb54{bbg->bb99|=(
bb1746[bb170].bb282.bb170)<<bbg->bb78;bbg->bb78+=bb21;}};bb1799=
bb2379[bb170];bbm(bb1799!=0 ){bb415-=bb2416[bb170];{bbe bb21=bb1799;
bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl))-bb21){bbe bb171=bb415;bbg->bb99|=(
bb171<<bbg->bb78);{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&
0xff ));};{bbg->bb172[bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));}
;};bbg->bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(bbl))-bbg->bb78);bbg->bb78
+=bb21-(8 *2 *bb12(bbl));}bb54{bbg->bb99|=(bb415)<<bbg->bb78;bbg->bb78
+=bb21;}};}};}bb106(bb2288<bbg->bb609);{bbe bb21=bb1078[256 ].bb48.
bb21;bbm(bbg->bb78>(bbe)(8 *2 *bb12(bbl))-bb21){bbe bb171=bb1078[256 ].
bb282.bb170;bbg->bb99|=(bb171<<bbg->bb78);{{bbg->bb172[bbg->bb185++]=
((bb155)((bbg->bb99)&0xff ));};{bbg->bb172[bbg->bb185++]=((bb155)((
bb126)(bbg->bb99)>>8 ));};};bbg->bb99=(bb126)bb171>>((bbe)(8 *2 *bb12(
bbl))-bbg->bb78);bbg->bb78+=bb21-(8 *2 *bb12(bbl));}bb54{bbg->bb99|=(
bb1078[256 ].bb282.bb170)<<bbg->bb78;bbg->bb78+=bb21;}};bbg->bb1992=
bb1078[256 ].bb48.bb21;}bb41 bbb bb2314(bbg)bb192*bbg;{bbe bb11=0 ;bbs
bb2326=0 ;bbs bb2270=0 ;bb106(bb11<7 )bb2270+=bbg->bb970[bb11++].bb282.
bb429;bb106(bb11<128 )bb2326+=bbg->bb970[bb11++].bb282.bb429;bb106(
bb11<256 )bb2270+=bbg->bb970[bb11++].bb282.bb429;bbg->bb969=(bb152)(
bb2270>(bb2326>>2 )?0 :1 );}bb41 bbs bb2373(bb170,bb21)bbs bb170;bbe bb21
;{bb930 bbs bb2305=0 ;bb565{bb2305|=bb170&1 ;bb170>>=1 ,bb2305<<=1 ;}
bb106(--bb21>0 );bb2 bb2305>>1 ;}bb41 bbb bb2304(bbg)bb192*bbg;{bbm(bbg
->bb78==16 ){{{bbg->bb172[bbg->bb185++]=((bb155)((bbg->bb99)&0xff ));};
{bbg->bb172[bbg->bb185++]=((bb155)((bb126)(bbg->bb99)>>8 ));};};bbg->
bb99=0 ;bbg->bb78=0 ;}bb54 bbm(bbg->bb78>=8 ){{bbg->bb172[bbg->bb185++]=
((bb152)bbg->bb99);};bbg->bb99>>=8 ;bbg->bb78-=8 ;}}bb41 bbb bb2259(bbg
)bb192*bbg;{bbm(bbg->bb78>8 ){{{bbg->bb172[bbg->bb185++]=((bb155)((bbg
->bb99)&0xff ));};{bbg->bb172[bbg->bb185++]=((bb155)((bb126)(bbg->bb99
)>>8 ));};};}bb54 bbm(bbg->bb78>0 ){{bbg->bb172[bbg->bb185++]=((bb152)bbg
->bb99);};}bbg->bb99=0 ;bbg->bb78=0 ;}bb41 bbb bb2319(bbg,bb40,bb21,
bb980)bb192*bbg;bb439*bb40;bbs bb21;bbe bb980;{bb2259(bbg);bbg->
bb1992=8 ;bbm(bb980){{{bbg->bb172[bbg->bb185++]=((bb155)(((bb126)bb21)&
0xff ));};{bbg->bb172[bbg->bb185++]=((bb155)((bb126)((bb126)bb21)>>8 ));
};};{{bbg->bb172[bbg->bb185++]=((bb155)(((bb126)~bb21)&0xff ));};{bbg
->bb172[bbg->bb185++]=((bb155)((bb126)((bb126)~bb21)>>8 ));};};}bb106(
bb21--){{bbg->bb172[bbg->bb185++]=( *bb40++);};}}
