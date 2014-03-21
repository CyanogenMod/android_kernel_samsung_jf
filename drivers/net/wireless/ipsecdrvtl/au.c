/*
   'src_compress_deflate_deflate.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Wed Dec 18 14:11:10 2013
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
bb1291,bbe bb1115));bb41 bbh bb458 bb1738[(256 +1 +29 )+2 ]={{{12 },{8 }},{
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
163 },{8 }},{{99 },{8 }},{{227 },{8 }}};bb41 bbh bb458 bb2234[30 ]={{{0 },{5 }
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
bb2052[258 -3 +1 ]={0 ,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,8 ,9 ,9 ,10 ,10 ,11 ,11 ,12 ,12 ,12 ,12 ,13 ,
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
27 ,27 ,27 ,27 ,28 };bb41 bbh bbe bb2333[29 ]={0 ,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,10 ,12 ,14 ,
16 ,20 ,24 ,28 ,32 ,40 ,48 ,56 ,64 ,80 ,96 ,112 ,128 ,160 ,192 ,224 ,0 };bb41 bbh bbe
bb2416[30 ]={0 ,1 ,2 ,3 ,4 ,6 ,8 ,12 ,16 ,24 ,32 ,48 ,64 ,96 ,128 ,192 ,256 ,384 ,512 ,
768 ,1024 ,1536 ,2048 ,3072 ,4096 ,6144 ,8192 ,12288 ,16384 ,24576 };bba bb10{
bb1283,bb2159,bb1756,bb2138}bb1823;bba bb1823( *bb2386)bbp((bb192*bbg
,bbe bb174));bb41 bbb bb2189 bbp((bb192*bbg));bb41 bb1823 bb2380 bbp(
(bb192*bbg,bbe bb174));bb41 bb1823 bb2160 bbp((bb192*bbg,bbe bb174));
bb41 bb1823 bb1947 bbp((bb192*bbg,bbe bb174));bb41 bbb bb2427 bbp((
bb192*bbg));bb41 bbb bb2009 bbp((bb192*bbg,bb7 bbn));bb41 bbb bb1265
bbp((bb17 bb16));bb41 bbe bb2432 bbp((bb17 bb16,bb30*bb40,bbs bb47));
bb41 bb7 bb2258 bbp((bb192*bbg,bb1313 bb1993));bba bbi bb2625{bb126
bb2429;bb126 bb2437;bb126 bb2383;bb126 bb2348;bb2386 bb2019;}bb2513;
bb41 bbh bb2513 bb1332[10 ]={{0 ,0 ,0 ,0 ,bb2380},{4 ,4 ,8 ,4 ,bb2160},{4 ,5 ,16
,8 ,bb2160},{4 ,6 ,32 ,32 ,bb2160},{4 ,4 ,16 ,16 ,bb1947},{8 ,16 ,32 ,32 ,bb1947},
{8 ,16 ,128 ,128 ,bb1947},{8 ,32 ,128 ,256 ,bb1947},{32 ,128 ,258 ,1024 ,bb1947},
{32 ,258 ,258 ,4096 ,bb1947}};bbi bb2278{bbe bb442;};bbe bb1116(bb16,
bb123,bb189,bb196)bb17 bb16;bbe bb123;bbh bbl*bb189;bbe bb196;{bb2
bb1047(bb16,bb123,8 ,15 ,bb803,0 ,bb189,bb196);}bbe bb1047(bb16,bb123,
bb569,bb452,bb958,bb295,bb189,bb196)bb17 bb16;bbe bb123;bbe bb569;bbe
bb452;bbe bb958;bbe bb295;bbh bbl*bb189;bbe bb196;{bb192*bbg;bbe
bb1350=0 ;bb41 bbh bbl*bb2444="\x31\x2e\x31\x2e\x33";bb500*bb1884;bbm(
bb189==0 ||bb189[0 ]!=bb2444[0 ]||bb196!=bb12(bb435)){bb2(-6 );}bbm(bb16
==0 )bb2(-2 );bb16->bb320=0 ;bbm(bb16->bb401==0 ){bb16->bb401=bb1175;bb16
->bb110=(bb70)0 ;}bbm(bb16->bb371==0 )bb16->bb371=bb1160;bbm(bb123==(-1
))bb123=6 ;bbm(bb452<0 ){bb1350=1 ;bb452=-bb452;}bbm(bb958<1 ||bb958>
bb206||bb569!=8 ||bb452<8 ||bb452>15 ||bb123<0 ||bb123>9 ||bb295<0 ||bb295>
2 ){bb2(-2 );}bbg=(bb192* )( * ((bb16)->bb401))((bb16)->bb110,(1 ),(bb12
(bb192)));bbm(bbg==0 )bb2(-4 );bb16->bb23=(bbi bb381* )bbg;bbg->bb16=
bb16;bbg->bb1350=bb1350;bbg->bb2162=bb452;bbg->bb934=1 <<bbg->bb2162;
bbg->bb1807=bbg->bb934-1 ;bbg->bb2130=bb958+7 ;bbg->bb1323=1 <<bbg->
bb2130;bbg->bb1380=bbg->bb1323-1 ;bbg->bb1464=((bbg->bb2130+3 -1 )/3 );
bbg->bb158=(bb30* )( * ((bb16)->bb401))((bb16)->bb110,(bbg->bb934),(2
 *bb12(bb152)));bbg->bb966=(bb1354* )( * ((bb16)->bb401))((bb16)->
bb110,(bbg->bb934),(bb12(bb1070)));bbg->bb385=(bb1354* )( * ((bb16)->
bb401))((bb16)->bb110,(bbg->bb1323),(bb12(bb1070)));bbg->bb1126=1 <<(
bb958+6 );bb1884=(bb500* )( * ((bb16)->bb401))((bb16)->bb110,(bbg->
bb1126),(bb12(bb126)+2 ));bbg->bb172=(bb1205* )bb1884;bbg->bb2039=(
bb389)bbg->bb1126* (bb12(bb126)+2L );bbm(bbg->bb158==0 ||bbg->bb966==0
||bbg->bb385==0 ||bbg->bb172==0 ){bb16->bb320=(bbl* )bb1071[2 -((-4 ))];
bb943(bb16);bb2(-4 );}bbg->bb1645=bb1884+bbg->bb1126/bb12(bb126);bbg->
bb1725=bbg->bb172+(1 +bb12(bb126)) *bbg->bb1126;bbg->bb123=bb123;bbg->
bb295=bb295;bbg->bb569=(bb152)bb569;bb2 bb1058(bb16);}bbe bb1177(bb16
,bb424,bb431)bb17 bb16;bbh bb30*bb424;bb7 bb431;{bb192*bbg;bb7 bb464=
bb431;bb7 bb11;bb1313 bb1121=0 ;bbm(bb16==0 ||bb16->bb23==0 ||bb424==0 ||
bb16->bb23->bb359!=42 )bb2(-2 );bbg=bb16->bb23;bb16->bb367=bb986(bb16->
bb367,bb424,bb431);bbm(bb464<3 )bb2 0 ;bbm(bb464>((bbg)->bb934-(258 +3 +1
))){bb464=((bbg)->bb934-(258 +3 +1 ));bb424+=bb431-bb464;}bb81(bbg->
bb158,bb424,bb464);bbg->bb183=bb464;bbg->bb427=(bb6)bb464;bbg->bb492=
bbg->bb158[0 ];(bbg->bb492=(((bbg->bb492)<<bbg->bb1464)^(bbg->bb158[1 ]
))&bbg->bb1380);bb90(bb11=0 ;bb11<=bb464-3 ;bb11++){((bbg->bb492=(((bbg
->bb492)<<bbg->bb1464)^(bbg->bb158[(bb11)+(3 -1 )]))&bbg->bb1380),bbg->
bb966[(bb11)&bbg->bb1807]=bb1121=bbg->bb385[bbg->bb492],bbg->bb385[
bbg->bb492]=(bb1070)(bb11));}bbm(bb1121)bb1121=0 ;bb2 0 ;}bbe bb1058(
bb16)bb17 bb16;{bb192*bbg;bbm(bb16==0 ||bb16->bb23==0 ||bb16->bb401==0
||bb16->bb371==0 )bb2(-2 );bb16->bb187=bb16->bb605=0 ;bb16->bb320=0 ;bb16
->bb969=2 ;bbg=(bb192* )bb16->bb23;bbg->bb185=0 ;bbg->bb1873=bbg->bb172
;bbm(bbg->bb1350<0 ){bbg->bb1350=0 ;}bbg->bb359=bbg->bb1350?113 :42 ;bb16
->bb367=1 ;bbg->bb1894=0 ;bb2209(bbg);bb2427(bbg);bb2 0 ;}bbe bb1178(
bb16,bb123,bb295)bb17 bb16;bbe bb123;bbe bb295;{bb192*bbg;bb2386
bb2019;bbe bb18=0 ;bbm(bb16==0 ||bb16->bb23==0 )bb2(-2 );bbg=bb16->bb23;
bbm(bb123==(-1 )){bb123=6 ;}bbm(bb123<0 ||bb123>9 ||bb295<0 ||bb295>2 ){bb2
(-2 );}bb2019=bb1332[bbg->bb123].bb2019;bbm(bb2019!=bb1332[bb123].
bb2019&&bb16->bb187!=0 ){bb18=bb506(bb16,1 );}bbm(bbg->bb123!=bb123){
bbg->bb123=bb123;bbg->bb2055=bb1332[bb123].bb2437;bbg->bb2180=bb1332[
bb123].bb2429;bbg->bb1837=bb1332[bb123].bb2383;bbg->bb2175=bb1332[
bb123].bb2348;}bbg->bb295=bb295;bb2 bb18;}bb41 bbb bb2009(bbg,bbn)bb192
 *bbg;bb7 bbn;{{bbg->bb172[bbg->bb185++]=((bb152)(bbn>>8 ));};{bbg->
bb172[bbg->bb185++]=((bb152)(bbn&0xff ));};}bb41 bbb bb1265(bb16)bb17
bb16;{bbs bb21=bb16->bb23->bb185;bbm(bb21>bb16->bb384)bb21=bb16->
bb384;bbm(bb21==0 )bb2;bb81(bb16->bb597,bb16->bb23->bb1873,bb21);bb16
->bb597+=bb21;bb16->bb23->bb1873+=bb21;bb16->bb605+=bb21;bb16->bb384
-=bb21;bb16->bb23->bb185-=bb21;bbm(bb16->bb23->bb185==0 ){bb16->bb23->
bb1873=bb16->bb23->bb172;}}bbe bb506(bb16,bb174)bb17 bb16;bbe bb174;{
bbe bb2344;bb192*bbg;bbm(bb16==0 ||bb16->bb23==0 ||bb174>4 ||bb174<0 ){
bb2(-2 );}bbg=bb16->bb23;bbm(bb16->bb597==0 ||(bb16->bb127==0 &&bb16->
bb148!=0 )||(bbg->bb359==666 &&bb174!=4 )){bb2(bb16->bb320=(bbl* )bb1071
[2 -((-2 ))],((-2 )));}bbm(bb16->bb384==0 )bb2(bb16->bb320=(bbl* )bb1071[
2 -((-5 ))],((-5 )));bbg->bb16=bb16;bb2344=bbg->bb1894;bbg->bb1894=bb174
;bbm(bbg->bb359==42 ){bb7 bb980=(8 +((bbg->bb2162-8 )<<4 ))<<8 ;bb7 bb2298
=(bbg->bb123-1 )>>1 ;bbm(bb2298>3 )bb2298=3 ;bb980|=(bb2298<<6 );bbm(bbg->
bb183!=0 )bb980|=0x20 ;bb980+=31 -(bb980%31 );bbg->bb359=113 ;bb2009(bbg,
bb980);bbm(bbg->bb183!=0 ){bb2009(bbg,(bb7)(bb16->bb367>>16 ));bb2009(
bbg,(bb7)(bb16->bb367&0xffff ));}bb16->bb367=1L ;}bbm(bbg->bb185!=0 ){
bb1265(bb16);bbm(bb16->bb384==0 ){bbg->bb1894=-1 ;bb2 0 ;}}bb54 bbm(bb16
->bb148==0 &&bb174<=bb2344&&bb174!=4 ){bb2(bb16->bb320=(bbl* )bb1071[2 -
((-5 ))],((-5 )));}bbm(bbg->bb359==666 &&bb16->bb148!=0 ){bb2(bb16->bb320
=(bbl* )bb1071[2 -((-5 ))],((-5 )));}bbm(bb16->bb148!=0 ||bbg->bb462!=0 ||
(bb174!=0 &&bbg->bb359!=666 )){bb1823 bb1985;bb1985=( * (bb1332[bbg->
bb123].bb2019))(bbg,bb174);bbm(bb1985==bb1756||bb1985==bb2138){bbg->
bb359=666 ;}bbm(bb1985==bb1283||bb1985==bb1756){bbm(bb16->bb384==0 ){
bbg->bb1894=-1 ;}bb2 0 ;}bbm(bb1985==bb2159){bbm(bb174==1 ){bb2256(bbg);
}bb54{bb2141(bbg,(bbl* )0 ,0L ,0 );bbm(bb174==3 ){bbg->bb385[bbg->bb1323-
1 ]=0 ;bb998((bb30* )bbg->bb385,0 ,(bbs)(bbg->bb1323-1 ) *bb12( *bbg->
bb385));;}}bb1265(bb16);bbm(bb16->bb384==0 ){bbg->bb1894=-1 ;bb2 0 ;}}};
bbm(bb174!=4 )bb2 0 ;bbm(bbg->bb1350)bb2 1 ;bb2009(bbg,(bb7)(bb16->bb367
>>16 ));bb2009(bbg,(bb7)(bb16->bb367&0xffff ));bb1265(bb16);bbg->bb1350
=-1 ;bb2 bbg->bb185!=0 ?0 :1 ;}bbe bb943(bb16)bb17 bb16;{bbe bb359;bbm(
bb16==0 ||bb16->bb23==0 )bb2(-2 );bb359=bb16->bb23->bb359;bbm(bb359!=42
&&bb359!=113 &&bb359!=666 ){bb2(-2 );}{bbm(bb16->bb23->bb172)( * ((bb16)->
bb371))((bb16)->bb110,(bb70)(bb16->bb23->bb172));};{bbm(bb16->bb23->
bb385)( * ((bb16)->bb371))((bb16)->bb110,(bb70)(bb16->bb23->bb385));}
;{bbm(bb16->bb23->bb966)( * ((bb16)->bb371))((bb16)->bb110,(bb70)(
bb16->bb23->bb966));};{bbm(bb16->bb23->bb158)( * ((bb16)->bb371))((
bb16)->bb110,(bb70)(bb16->bb23->bb158));};( * ((bb16)->bb371))((bb16)->
bb110,(bb70)(bb16->bb23));bb16->bb23=0 ;bb2 bb359==113 ?(-3 ):0 ;}bbe
bb1150(bb130,bb184)bb17 bb130;bb17 bb184;{
#ifdef bb510
bb2(-2 );
#else
bb192*bb426;bb192*bb1824;bb500*bb1884;bbm(bb184==0 ||bb130==0 ||bb184->
bb23==0 ){bb2(-2 );}bb1824=bb184->bb23; *bb130= *bb184;bb426=(bb192* )(
 * ((bb130)->bb401))((bb130)->bb110,(1 ),(bb12(bb192)));bbm(bb426==0 )bb2
(-4 );bb130->bb23=(bbi bb381* )bb426; *bb426= *bb1824;bb426->bb16=
bb130;bb426->bb158=(bb30* )( * ((bb130)->bb401))((bb130)->bb110,(
bb426->bb934),(2 *bb12(bb152)));bb426->bb966=(bb1354* )( * ((bb130)->
bb401))((bb130)->bb110,(bb426->bb934),(bb12(bb1070)));bb426->bb385=(
bb1354* )( * ((bb130)->bb401))((bb130)->bb110,(bb426->bb1323),(bb12(
bb1070)));bb1884=(bb500* )( * ((bb130)->bb401))((bb130)->bb110,(bb426
->bb1126),(bb12(bb126)+2 ));bb426->bb172=(bb1205* )bb1884;bbm(bb426->
bb158==0 ||bb426->bb966==0 ||bb426->bb385==0 ||bb426->bb172==0 ){bb943(
bb130);bb2(-4 );}bb81(bb426->bb158,bb1824->bb158,bb426->bb934*2 *bb12(
bb152));bb81((bb30* )bb426->bb966,(bb30* )bb1824->bb966,bb426->bb934*
bb12(bb1070));bb81((bb30* )bb426->bb385,(bb30* )bb1824->bb385,bb426->
bb1323*bb12(bb1070));bb81(bb426->bb172,bb1824->bb172,(bb7)bb426->
bb2039);bb426->bb1873=bb426->bb172+(bb1824->bb1873-bb1824->bb172);
bb426->bb1645=bb1884+bb426->bb1126/bb12(bb126);bb426->bb1725=bb426->
bb172+(1 +bb12(bb126)) *bb426->bb1126;bb426->bb1937.bb1748=bb426->
bb970;bb426->bb1862.bb1748=bb426->bb1682;bb426->bb2063.bb1748=bb426->
bb523;bb2 0 ;
#endif
}bb41 bbe bb2432(bb16,bb40,bb47)bb17 bb16;bb30*bb40;bbs bb47;{bbs bb21
=bb16->bb148;bbm(bb21>bb47)bb21=bb47;bbm(bb21==0 )bb2 0 ;bb16->bb148-=
bb21;bbm(!bb16->bb23->bb1350){bb16->bb367=bb986(bb16->bb367,bb16->
bb127,bb21);}bb81(bb40,bb16->bb127,bb21);bb16->bb127+=bb21;bb16->
bb187+=bb21;bb2(bbe)bb21;}bb41 bbb bb2427(bbg)bb192*bbg;{bbg->bb2227=
(bb389)2L *bbg->bb934;bbg->bb385[bbg->bb1323-1 ]=0 ;bb998((bb30* )bbg->
bb385,0 ,(bbs)(bbg->bb1323-1 ) *bb12( *bbg->bb385));;bbg->bb2055=bb1332
[bbg->bb123].bb2437;bbg->bb2180=bb1332[bbg->bb123].bb2429;bbg->bb1837
=bb1332[bbg->bb123].bb2383;bbg->bb2175=bb1332[bbg->bb123].bb2348;bbg
->bb183=0 ;bbg->bb427=0L ;bbg->bb462=0 ;bbg->bb957=bbg->bb1277=3 -1 ;bbg->
bb1939=0 ;bbg->bb492=0 ;}bb41 bb7 bb2258(bbg,bb1993)bb192*bbg;bb1313
bb1993;{bbs bb2318=bbg->bb2175;bb930 bb30*bb491=bbg->bb158+bbg->bb183
;bb930 bb30*bb607;bb930 bbe bb21;bbe bb1189=bbg->bb1277;bbe bb1837=
bbg->bb1837;bb1313 bb2443=bbg->bb183>(bb1313)((bbg)->bb934-(258 +3 +1 ))?
bbg->bb183-(bb1313)((bbg)->bb934-(258 +3 +1 )):0 ;bb1354*bb966=bbg->bb966
;bb7 bb2470=bbg->bb1807;
#ifdef bb1040
bb930 bb30*bb1893=bbg->bb158+bbg->bb183+258 -1 ;bb930 bb126 bb2532= * (
bb500* )bb491;bb930 bb126 bb2121= * (bb500* )(bb491+bb1189-1 );
#else
bb930 bb30*bb1893=bbg->bb158+bbg->bb183+258 ;bb930 bb152 bb2320=bb491[
bb1189-1 ];bb930 bb152 bb2121=bb491[bb1189];
#endif
;bbm(bbg->bb1277>=bbg->bb2180){bb2318>>=2 ;}bbm((bb7)bb1837>bbg->bb462
)bb1837=bbg->bb462;;bb565{;bb607=bbg->bb158+bb1993;
#if ( defined( bb1040) && bb2580 == 258)
bbm( * (bb500* )(bb607+bb1189-1 )!=bb2121|| * (bb500* )bb607!=bb2532)bb1690
;;bb491++,bb607++;bb565{}bb106( * (bb500* )(bb491+=2 )== * (bb500* )(
bb607+=2 )&& * (bb500* )(bb491+=2 )== * (bb500* )(bb607+=2 )&& * (bb500*
)(bb491+=2 )== * (bb500* )(bb607+=2 )&& * (bb500* )(bb491+=2 )== * (
bb500* )(bb607+=2 )&&bb491<bb1893);;bbm( *bb491== *bb607)bb491++;bb21=
(258 -1 )-(bbe)(bb1893-bb491);bb491=bb1893-(258 -1 );
#else
bbm(bb607[bb1189]!=bb2121||bb607[bb1189-1 ]!=bb2320|| *bb607!= *bb491
|| * ++bb607!=bb491[1 ])bb1690;bb491+=2 ,bb607++;;bb565{}bb106( * ++
bb491== * ++bb607&& * ++bb491== * ++bb607&& * ++bb491== * ++bb607&& *
++bb491== * ++bb607&& * ++bb491== * ++bb607&& * ++bb491== * ++bb607&&
 * ++bb491== * ++bb607&& * ++bb491== * ++bb607&&bb491<bb1893);;bb21=
258 -(bbe)(bb1893-bb491);bb491=bb1893-258 ;
#endif
bbm(bb21>bb1189){bbg->bb2023=bb1993;bb1189=bb21;bbm(bb21>=bb1837)bb22
;
#ifdef bb1040
bb2121= * (bb500* )(bb491+bb1189-1 );
#else
bb2320=bb491[bb1189-1 ];bb2121=bb491[bb1189];
#endif
}}bb106((bb1993=bb966[bb1993&bb2470])>bb2443&&--bb2318!=0 );bbm((bb7)bb1189
<=bbg->bb462)bb2(bb7)bb1189;bb2 bbg->bb462;}bb41 bbb bb2189(bbg)bb192
 *bbg;{bb930 bbs bb11,bb97;bb930 bb1354*bb28;bbs bb1927;bb7 bb1227=
bbg->bb934;bb565{bb1927=(bbs)(bbg->bb2227-(bb389)bbg->bb462-(bb389)bbg
->bb183);bbm(bb1927==0 &&bbg->bb183==0 &&bbg->bb462==0 ){bb1927=bb1227;}
bb54 bbm(bb1927==(bbs)(-1 )){bb1927--;}bb54 bbm(bbg->bb183>=bb1227+((
bbg)->bb934-(258 +3 +1 ))){bb81(bbg->bb158,bbg->bb158+bb1227,(bbs)bb1227
);bbg->bb2023-=bb1227;bbg->bb183-=bb1227;bbg->bb427-=(bb6)bb1227;bb11
=bbg->bb1323;bb28=&bbg->bb385[bb11];bb565{bb97= * --bb28; *bb28=(
bb1070)(bb97>=bb1227?bb97-bb1227:0 );}bb106(--bb11);bb11=bb1227;bb28=&
bbg->bb966[bb11];bb565{bb97= * --bb28; *bb28=(bb1070)(bb97>=bb1227?
bb97-bb1227:0 );}bb106(--bb11);bb1927+=bb1227;}bbm(bbg->bb16->bb148==0
)bb2;;bb11=bb2432(bbg->bb16,bbg->bb158+bbg->bb183+bbg->bb462,bb1927);
bbg->bb462+=bb11;bbm(bbg->bb462>=3 ){bbg->bb492=bbg->bb158[bbg->bb183]
;(bbg->bb492=(((bbg->bb492)<<bbg->bb1464)^(bbg->bb158[bbg->bb183+1 ]))&
bbg->bb1380);}}bb106(bbg->bb462<(258 +3 +1 )&&bbg->bb16->bb148!=0 );}bb41
bb1823 bb2380(bbg,bb174)bb192*bbg;bbe bb174;{bb389 bb2273=0xffff ;
bb389 bb2150;bbm(bb2273>bbg->bb2039-5 ){bb2273=bbg->bb2039-5 ;}bb90(;;){
bbm(bbg->bb462<=1 ){;bb2189(bbg);bbm(bbg->bb462==0 &&bb174==0 )bb2 bb1283
;bbm(bbg->bb462==0 )bb22;};bbg->bb183+=bbg->bb462;bbg->bb462=0 ;bb2150=
bbg->bb427+bb2273;bbm(bbg->bb183==0 ||(bb389)bbg->bb183>=bb2150){bbg->
bb462=(bb7)(bbg->bb183-bb2150);bbg->bb183=(bb7)bb2150;{{bb1619(bbg,(
bbg->bb427>=0L ?(bb439* )&bbg->bb158[(bbs)bbg->bb427]:(bb439* )0 ),(
bb389)((bb6)bbg->bb183-bbg->bb427),(0 ));bbg->bb427=bbg->bb183;bb1265(
bbg->bb16);;};bbm(bbg->bb16->bb384==0 )bb2(0 )?bb1756:bb1283;};}bbm(bbg
->bb183-(bb7)bbg->bb427>=((bbg)->bb934-(258 +3 +1 ))){{{bb1619(bbg,(bbg
->bb427>=0L ?(bb439* )&bbg->bb158[(bbs)bbg->bb427]:(bb439* )0 ),(bb389)(
(bb6)bbg->bb183-bbg->bb427),(0 ));bbg->bb427=bbg->bb183;bb1265(bbg->
bb16);;};bbm(bbg->bb16->bb384==0 )bb2(0 )?bb1756:bb1283;};}}{{bb1619(
bbg,(bbg->bb427>=0L ?(bb439* )&bbg->bb158[(bbs)bbg->bb427]:(bb439* )0 ),
(bb389)((bb6)bbg->bb183-bbg->bb427),(bb174==4 ));bbg->bb427=bbg->bb183
;bb1265(bbg->bb16);;};bbm(bbg->bb16->bb384==0 )bb2(bb174==4 )?bb1756:
bb1283;};bb2 bb174==4 ?bb2138:bb2159;}bb41 bb1823 bb2160(bbg,bb174)bb192
 *bbg;bbe bb174;{bb1313 bb1121=0 ;bbe bb1762;bb90(;;){bbm(bbg->bb462<(
258 +3 +1 )){bb2189(bbg);bbm(bbg->bb462<(258 +3 +1 )&&bb174==0 ){bb2 bb1283;
}bbm(bbg->bb462==0 )bb22;}bbm(bbg->bb462>=3 ){((bbg->bb492=(((bbg->
bb492)<<bbg->bb1464)^(bbg->bb158[(bbg->bb183)+(3 -1 )]))&bbg->bb1380),
bbg->bb966[(bbg->bb183)&bbg->bb1807]=bb1121=bbg->bb385[bbg->bb492],
bbg->bb385[bbg->bb492]=(bb1070)(bbg->bb183));}bbm(bb1121!=0 &&bbg->
bb183-bb1121<=((bbg)->bb934-(258 +3 +1 ))){bbm(bbg->bb295!=2 ){bbg->bb957
=bb2258(bbg,bb1121);}}bbm(bbg->bb957>=3 ){;{bb155 bb21=(bbg->bb957-3 );
bb126 bb415=(bbg->bb183-bbg->bb2023);bbg->bb1645[bbg->bb609]=bb415;
bbg->bb1725[bbg->bb609++]=bb21;bb415--;bbg->bb970[bb2052[bb21]+256 +1 ]
.bb282.bb429++;bbg->bb1682[((bb415)<256 ?bb1735[bb415]:bb1735[256 +((
bb415)>>7 )])].bb282.bb429++;bb1762=(bbg->bb609==bbg->bb1126-1 );};bbg
->bb462-=bbg->bb957;bbm(bbg->bb957<=bbg->bb2055&&bbg->bb462>=3 ){bbg->
bb957--;bb565{bbg->bb183++;((bbg->bb492=(((bbg->bb492)<<bbg->bb1464)^
(bbg->bb158[(bbg->bb183)+(3 -1 )]))&bbg->bb1380),bbg->bb966[(bbg->bb183
)&bbg->bb1807]=bb1121=bbg->bb385[bbg->bb492],bbg->bb385[bbg->bb492]=(
bb1070)(bbg->bb183));}bb106(--bbg->bb957!=0 );bbg->bb183++;}bb54{bbg->
bb183+=bbg->bb957;bbg->bb957=0 ;bbg->bb492=bbg->bb158[bbg->bb183];(bbg
->bb492=(((bbg->bb492)<<bbg->bb1464)^(bbg->bb158[bbg->bb183+1 ]))&bbg
->bb1380);}}bb54{;{bb155 bb1841=(bbg->bb158[bbg->bb183]);bbg->bb1645[
bbg->bb609]=0 ;bbg->bb1725[bbg->bb609++]=bb1841;bbg->bb970[bb1841].
bb282.bb429++;bb1762=(bbg->bb609==bbg->bb1126-1 );};bbg->bb462--;bbg->
bb183++;}bbm(bb1762){{bb1619(bbg,(bbg->bb427>=0L ?(bb439* )&bbg->bb158
[(bbs)bbg->bb427]:(bb439* )0 ),(bb389)((bb6)bbg->bb183-bbg->bb427),(0 ));
bbg->bb427=bbg->bb183;bb1265(bbg->bb16);;};bbm(bbg->bb16->bb384==0 )bb2
(0 )?bb1756:bb1283;};}{{bb1619(bbg,(bbg->bb427>=0L ?(bb439* )&bbg->
bb158[(bbs)bbg->bb427]:(bb439* )0 ),(bb389)((bb6)bbg->bb183-bbg->bb427
),(bb174==4 ));bbg->bb427=bbg->bb183;bb1265(bbg->bb16);;};bbm(bbg->
bb16->bb384==0 )bb2(bb174==4 )?bb1756:bb1283;};bb2 bb174==4 ?bb2138:
bb2159;}bb41 bb1823 bb1947(bbg,bb174)bb192*bbg;bbe bb174;{bb1313
bb1121=0 ;bbe bb1762;bb90(;;){bbm(bbg->bb462<(258 +3 +1 )){bb2189(bbg);
bbm(bbg->bb462<(258 +3 +1 )&&bb174==0 ){bb2 bb1283;}bbm(bbg->bb462==0 )bb22
;}bbm(bbg->bb462>=3 ){((bbg->bb492=(((bbg->bb492)<<bbg->bb1464)^(bbg->
bb158[(bbg->bb183)+(3 -1 )]))&bbg->bb1380),bbg->bb966[(bbg->bb183)&bbg
->bb1807]=bb1121=bbg->bb385[bbg->bb492],bbg->bb385[bbg->bb492]=(
bb1070)(bbg->bb183));}bbg->bb1277=bbg->bb957,bbg->bb2299=bbg->bb2023;
bbg->bb957=3 -1 ;bbm(bb1121!=0 &&bbg->bb1277<bbg->bb2055&&bbg->bb183-
bb1121<=((bbg)->bb934-(258 +3 +1 ))){bbm(bbg->bb295!=2 ){bbg->bb957=
bb2258(bbg,bb1121);}bbm(bbg->bb957<=5 &&(bbg->bb295==1 ||(bbg->bb957==3
&&bbg->bb183-bbg->bb2023>4096 ))){bbg->bb957=3 -1 ;}}bbm(bbg->bb1277>=3
&&bbg->bb957<=bbg->bb1277){bb7 bb2492=bbg->bb183+bbg->bb462-3 ;;{bb155
bb21=(bbg->bb1277-3 );bb126 bb415=(bbg->bb183-1 -bbg->bb2299);bbg->
bb1645[bbg->bb609]=bb415;bbg->bb1725[bbg->bb609++]=bb21;bb415--;bbg->
bb970[bb2052[bb21]+256 +1 ].bb282.bb429++;bbg->bb1682[((bb415)<256 ?
bb1735[bb415]:bb1735[256 +((bb415)>>7 )])].bb282.bb429++;bb1762=(bbg->
bb609==bbg->bb1126-1 );};bbg->bb462-=bbg->bb1277-1 ;bbg->bb1277-=2 ;
bb565{bbm(++bbg->bb183<=bb2492){((bbg->bb492=(((bbg->bb492)<<bbg->
bb1464)^(bbg->bb158[(bbg->bb183)+(3 -1 )]))&bbg->bb1380),bbg->bb966[(
bbg->bb183)&bbg->bb1807]=bb1121=bbg->bb385[bbg->bb492],bbg->bb385[bbg
->bb492]=(bb1070)(bbg->bb183));}}bb106(--bbg->bb1277!=0 );bbg->bb1939=
0 ;bbg->bb957=3 -1 ;bbg->bb183++;bbm(bb1762){{bb1619(bbg,(bbg->bb427>=0L
?(bb439* )&bbg->bb158[(bbs)bbg->bb427]:(bb439* )0 ),(bb389)((bb6)bbg->
bb183-bbg->bb427),(0 ));bbg->bb427=bbg->bb183;bb1265(bbg->bb16);;};bbm
(bbg->bb16->bb384==0 )bb2(0 )?bb1756:bb1283;};}bb54 bbm(bbg->bb1939){;{
bb155 bb1841=(bbg->bb158[bbg->bb183-1 ]);bbg->bb1645[bbg->bb609]=0 ;bbg
->bb1725[bbg->bb609++]=bb1841;bbg->bb970[bb1841].bb282.bb429++;bb1762
=(bbg->bb609==bbg->bb1126-1 );};bbm(bb1762){{bb1619(bbg,(bbg->bb427>=
0L ?(bb439* )&bbg->bb158[(bbs)bbg->bb427]:(bb439* )0 ),(bb389)((bb6)bbg
->bb183-bbg->bb427),(0 ));bbg->bb427=bbg->bb183;bb1265(bbg->bb16);;};}
bbg->bb183++;bbg->bb462--;bbm(bbg->bb16->bb384==0 )bb2 bb1283;}bb54{
bbg->bb1939=1 ;bbg->bb183++;bbg->bb462--;}};bbm(bbg->bb1939){;{bb155
bb1841=(bbg->bb158[bbg->bb183-1 ]);bbg->bb1645[bbg->bb609]=0 ;bbg->
bb1725[bbg->bb609++]=bb1841;bbg->bb970[bb1841].bb282.bb429++;bb1762=(
bbg->bb609==bbg->bb1126-1 );};bbg->bb1939=0 ;}{{bb1619(bbg,(bbg->bb427
>=0L ?(bb439* )&bbg->bb158[(bbs)bbg->bb427]:(bb439* )0 ),(bb389)((bb6)bbg
->bb183-bbg->bb427),(bb174==4 ));bbg->bb427=bbg->bb183;bb1265(bbg->
bb16);;};bbm(bbg->bb16->bb384==0 )bb2(bb174==4 )?bb1756:bb1283;};bb2
bb174==4 ?bb2138:bb2159;}
