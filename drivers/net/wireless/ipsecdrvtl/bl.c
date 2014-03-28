/*
   'src_ipsec_pgpIPsecESP.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Wed Dec 18 14:11:11 2013
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
bba bb82 bb4;bb10{bb98=0 ,bb361=-12000 ,bb345=-11999 ,bb377=-11998 ,bb674
=-11997 ,bb722=-11996 ,bb738=-11995 ,bb874=-11994 ,bb854=-11992 ,bb779=-
11991 ,bb804=-11990 ,bb750=-11989 ,bb838=-11988 ,bb639=-11987 ,bb683=-
11986 ,bb763=-11985 ,bb848=-11984 ,bb711=-11983 ,bb622=-11982 ,bb773=-
11981 ,bb895=-11980 ,bb684=-11979 ,bb714=-11978 ,bb844=-11977 ,bb586=-
11976 ,bb897=-11975 ,bb637=-11960 ,bb885=-11959 ,bb876=-11500 ,bb723=-
11499 ,bb670=-11498 ,bb810=-11497 ,bb696=-11496 ,bb755=-11495 ,bb720=-
11494 ,bb783=-11493 ,bb668=-11492 ,bb632=-11491 ,bb635=-11490 ,bb676=-
11489 ,bb680=-11488 ,bb700=-11487 ,bb840=-11486 ,bb889=-11485 ,bb649=-
11484 ,bb900=-11483 ,bb716=-11482 ,bb634=-11481 ,bb708=-11480 ,bb768=-
11479 ,bb648=-11478 ,bb730=-11477 ,bb774=-11476 ,bb769=-11475 ,bb866=-
11474 ,bb775=-11473 ,bb841=-11472 ,bb757=-11460 ,bb843=-11450 ,bb751=-
11449 ,bb717=-11448 ,bb726=-11447 ,bb864=-11446 ,bb629=-11445 ,bb826=-
11444 ,bb734=-11443 ,bb713=-11440 ,bb789=-11439 ,bb791=-11438 ,bb697=-
11437 ,bb707=-11436 ,bb673=-11435 ,bb630=-11420 ,bb531=-11419 ,bb567=-
11418 ,bb693=-11417 ,bb631=-11416 ,bb792=-11415 ,bb808=-11414 ,bb748=-
11413 ,bb633=-11412 ,bb830=-11411 ,bb681=-11410 ,bb781=-11409 ,bb715=-
11408 ,bb905=-11407 ,bb873=-11406 ,bb814=-11405 ,bb894=-11404 ,bb654=-
11403 ,bb770=-11402 ,bb846=-11401 ,bb642=-11400 ,bb879=-11399 ,bb855=-
11398 ,bb659=-11397 ,bb687=-11396 ,bb898=-11395 ,bb705=-11394 ,bb860=-
11393 ,bb811=-11392 ,bb878=-11391 ,bb888=-11390 ,bb743=-11389 ,bb790=-
11388 ,bb813=-11387 ,bb903=-11386 ,bb758=-11385 ,bb706=-11384 ,bb899=-
11383 ,bb645=-11382 ,bb908=-11381 ,bb640=-11380 ,bb795=-11379 ,bb623=-
11378 ,bb760=-11377 ,bb815=-11376 ,bb870=-11375 ,bb788=-11374 ,bb816=-
11373 ,bb688=-11372 ,bb865=-11371 ,bb812=-11370 ,bb620=-11369 ,bb712=-
11368 ,bb765=-11367 ,bb858=-11366 ,bb710=-11365 ,bb636=-11364 ,bb653=-
11363 ,bb387=-11350 ,bb884=bb387,bb721=-11349 ,bb662=-11348 ,bb835=-11347
,bb646=-11346 ,bb651=-11345 ,bb877=-11344 ,bb818=-11343 ,bb754=-11342 ,
bb824=-11341 ,bb764=-11340 ,bb902=-11339 ,bb397=-11338 ,bb724=-11337 ,
bb701=bb397,bb819=-11330 ,bb619=-11329 ,bb799=-11328 ,bb801=-11327 ,bb731
=-11326 ,bb762=-11325 ,bb772=-11324 ,bb672=-11320 ,bb638=-11319 ,bb678=-
11318 ,bb752=-11317 ,bb644=-11316 ,bb667=-11315 ,bb831=-11314 ,bb736=-
11313 ,bb776=-11312 ,bb881=-11300 ,bb787=-11299 ,bb749=-11298 ,bb709=-
11297 ,bb704=-11296 ,bb867=-11295 ,bb892=-11294 ,bb656=-11293 ,bb890=-
11292 ,bb906=-11291 ,bb618=-11290 ,bb727=-11289 ,bb809=-11288 ,bb725=-
11287 ,bb785=-11286 ,bb616=-11285 ,bb806=-11284 ,bb850=-11283 ,bb728=-
11282 ,bb679=-11281 ,bb807=-11280 ,bb685=-11279 ,bb753=-11250 ,bb832=-
11249 ,bb663=-11248 ,bb677=-11247 ,bb641=-11246 ,bb650=-11245 ,bb719=-
11244 ,bb761=-11243 ,bb621=-11242 ,bb820=-11240 ,bb655=-11239 ,bb745=-
11238 ,bb825=-11237 ,bb702=-11150 ,bb703=-11100 ,bb836=-11099 ,bb786=-
11098 ,bb871=-11097 ,bb735=-11096 ,bb802=-11095 ,bb643=-11094 ,bb617=-
11093 ,bb834=-11092 ,bb686=-11091 ,bb805=-11090 ,bb628=-11089 ,bb868=-
11088 ,bb661=-11087 ,bb901=-11086 ,bb767=-11085 ,bb793=-11050 ,bb759=-
11049 ,bb695=-10999 ,bb771=-10998 ,bb658=-10997 ,bb669=-10996 ,bb904=-
10995 ,bb893=-10994 ,bb699=-10993 ,bb817=-10992 ,bb780=-10991 ,bb627=-
10990 ,bb626=-10989 ,bb822=-10988 ,bb694=-10979 ,bb660=-10978 ,bb880=-
10977 ,bb875=-10976 ,bb744=-10975 ,bb742=-10974 ,};bb10{bb558=1 ,};bbb*
bb499(bbd bb1201,bbd bb374);bb4 bb461(bbb*bb972);bba bbi bb991*bb1015
;bba bbi bb1028*bb1029;bba bbi bb993*bb1023;bba bbi bb1025*bb1011;bba
bbi bb1017*bb1024;bba bbi bb1014*bb987;bba bb10{bb554=0 ,bb582=1 ,bb580
=2 ,bb827=3 ,bb587=4 ,bb561=5 ,bb575=6 ,bb562=7 ,bb579=9 ,}bb422;bba bb10{
bb601=0 ,bb992,bb615,bb1010,bb931,bb923,bb925,bb918,bb927,bb922,bb916,
}bb519;bba bbi bb449{bb1 bb74;bbd bb125;bbd bb181;bbi bb449*bb93;}bbx
;bb4 bb467(bbx*bb689,bbd bb909,bbx*bb837,bbd bb896,bbd bb536);bb4
bb526(bbx*bbj,bbd bb92,bbh bbb*bb95,bbd bb47);bb4 bb563(bbx*bbj,bbd
bb92,bbb*bb130,bbd bb47);bbu bb833(bbx*bbj,bbd bb92,bbh bbb*bb95,bbd
bb47);bb4 bb2057(bbx*bb299,bbd*bb104);bb4 bb2116(bbx*bb85,bbu bb177,
bbd bb486,bb519 bb154,bbh bbf*bb1309,bbf*bb131,bb422 bb412,bbf*bb553,
bbd bb104,bbd bb497,bbx*bb58);bb4 bb2044(bbx*bb85,bbu bb177,bb519
bb154,bbh bbf*bb1309,bb422 bb412,bbf*bb553,bbd*bb489,bbd*bb459,bbd*
bb539,bbx*bb58);
#define bb960 bb53(0x0800)
#define bb1140 bb53(0x0806)
#define bb948 bb53(0x01f4)
#define bb937 bb53(0x1194)
#define bb1135 bb53(0x4000)
#define bb1100 bb53(0x2000)
#define bb1114 bb53(0x1FFF)
#define bb1069( bb8) (( bb8) & bb53(0x2000 | 0x1FFF))
#define bb1022( bb8) ((( bb194( bb8)) & 0x1FFF) << 3)
#define bb978( bb8) ((( bb8) & bb53(0x1FFF)) == 0)
#define bb495( bb8) (( bb8) & bb53(0x2000))
#define bb988( bb8) (!( bb495( bb8)))
#pragma pack(push, 1)
bba bbi{bbf bb370[6 ];bbf bb1008[6 ];bbk bb375;}bb363, *bb378;bba bbi{
bbf bb442[6 ];bbk bb375;}bb1084, *bb1093;bba bbi{bbf bb936:4 ;bbf bb1090
:4 ;bbf bb1053;bbk bb372;bbk bb886;bbk bb573;bbf bb1003;bbf bb253;bbk
bb611;bbd bb311;bbd bb204;}bb325, *bb316;bba bbi{bbk bb1034;bbk bb1042
;bbf bb1055;bbf bb1043;bbk bb1064;bbf bb1083[6 ];bbd bb1038;bbf bb1037
[6 ];bbd bb1066;}bb1091, *bb1068;
#pragma pack(pop)
bba bbi{bbk bb290;bbk bb423;bbk bb1007;bbk bb319;}bb414, *bb347;bba
bbi{bbk bb290;bbk bb578;bbd bb540;bbd bb919;bbf bb92;bbf bb170;bbk
bb158;bbk bb319;bbk bb1021;}bb485, *bb318;bba bbi{bbf bb1080;bbf
bb1073;bbf bb1089;bbf bb1039;bbd bb1065;bbk bb1085;bbk bb374;bbd
bb1032;bbd bb1051;bbd bb1067;bbd bb1063;bbf bb1081[16 ];bbf bb1050[64 ]
;bbf bb25[128 ];bbf bb1033[64 ];}bb1082, *bb1076;bba bbi{bbd bb311;bbd
bb204;bbf bb917;bbf bb253;bbk bb913;}bb610, *bb571;
#if defined( _WIN32)
#define bb53( bbc) (((( bbc) & 0XFF00) >> 8) | ((( bbc) & 0X00FF) <<  \
8))
#define bb194( bbc) ( bb53( bbc))
#define bb445( bbc) (((( bbc) & 0XFF000000) >> 24) | ((( bbc) &  \
0X00FF0000) >> 8) | ((( bbc) & 0X0000FF00) << 8) | ((( bbc) &  \
0X000000FF) << 24))
#define bb493( bbc) ( bb445( bbc))
#endif
bbk bb928(bbh bbb*bb301);bbk bb849(bbh bbb*bb503,bbe bb21);bb4 bb596(
bbx*bb85,bbf bb103,bbx*bb58);bb4 bb690(bbx*bb85,bbu bb177,bbf*bb403);
bb4 bb951(bbx*bb58,bbf*bb391);bb4 bb952(bbh bbf*bb391,bbx*bb58);bb4
bb537(bbx*bb50,bbf bb103,bbd*bb941);bb4 bb924(bbx*bb85,bbf bb103,bbf
bb403,bbx*bb58);bbd bb518(bbx*bb50);bbk bb547(bbx*bb50);bbb bb522(bbk
bb151,bbx*bb50);bbb bb541(bbx*bb50);bbb bb968(bbx*bb50,bbd*bb26);bbb
bb1000(bbx*bb50,bbd*bb26);bbb bb994(bbx*bb50,bbd bb26);bbb bb921(bbx*
bb50,bbd bb26);bbb bb985(bbx*bb50);bbu bb1012(bbf*bb50);bb10{bb1129=-
5000 ,bb1111=-4000 ,bb999=-4999 ,bb1020=-4998 ,bb1031=-4997 ,bb983=-4996 ,
bb1095=-4995 ,bb1088=-4994 ,bb1102=-4993 ,bb1027=-4992 ,bb1086=-4991 };bb4
bb1134(bb4 bb1137,bbd bb1120,bbl*bb1105);bba bb10{bb402,bb1505,}bb297
;bbk bb1207(bb297 bb859,bbh bbf*bb453);bbd bb542(bb297 bb859,bbh bbf*
bb453);bbb bb1166(bbk bb159,bb297 bb550,bbf bb438[2 ]);bbb bb976(bbd
bb159,bb297 bb550,bbf bb438[4 ]);
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbf bb417[8 *16 ];}bb932;bbb bb1750(bb932*bbj,bbh bbb*bb71);bbb
bb1931(bb932*bbj,bbh bbb*bb71);bbb bb1282(bb932*bbj,bbb*bb14,bbh bbb*
bb5);bba bbi{bb932 bb2033,bb2034,bb2032;}bb829;bbb bb2102(bb829*bbj,
bbh bbb*bb1303);bbb bb2136(bb829*bbj,bbh bbb*bb1303);bbb bb2043(bb829
 *bbj,bbb*bb14,bbh bbb*bb5);bbb bb2178(bb829*bbj,bbb*bb14,bbh bbb*bb5
);
#ifdef __cplusplus
}
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
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbd bb417[2 *16 ];}bb984;bbb bb2000(bb984*bbj,bbh bbb*bb71);bbb
bb2145(bb984*bbj,bbh bbb*bb71);bbb bb1982(bb984*bbj,bbb*bb14,bbh bbb*
bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbq bb447;bbd bb417[4 * (16 +1 )];}bb576;bbb bb1598(bb576*bbj,
bbh bbb*bb71,bbq bb142);bbb bb2018(bb576*bbj,bbh bbb*bb71,bbq bb142);
bbb bb1243(bb576*bbj,bbb*bb14,bbh bbb*bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbb( *bb468)(bbb*bbj,bbb*bb14,bbh bbb*bb5);bba bbi{bbe bb57;bbq
bb38;bbq bb142;bbf bb150[16 ];bbq bb88;bbf bb131[16 ];bb468 bb180;bbf
bb154[512 ];}bb434;bbb bb2068(bb434*bbj,bbe bb57);bbb bb2065(bb434*bbj
,bbe bb2220,bbh bbb*bb71,bbh bbb*bb1271);bbb bb2301(bb434*bbj,bbe bb57
,bbh bbb*bb71,bbh bbb*bb1271);bbb bb1615(bb434*bbj,bbb*bb14,bb94 bb176
,bbh bbb*bb5,bbq bb9);bbu bb2097(bb434*bbj,bbb*bb14,bb94 bb176);bbb
bb2392(bbe bb57,bbh bbb*bb71,bbh bbb*bb1271,bbb*bb2125,bb94 bb176,bbh
bbb*bb5,bbq bb9);bb83 bb2267(bbe bb154);bb83 bb2242(bbe bb1661);bb83
bb2414(bbe bb57);bbu bb2512(bbe bb57);
#ifdef __cplusplus
}
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
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbd bb9;bbd bb23[5 ];bbf bb102[64 ];}bb524;bbb bb1794(bb524*bbj
);bbb bb1290(bb524*bbj,bbh bbb*bb5,bbq bb9);bbb bb1802(bb524*bbj,bbb*
bb14);bba bbi{bbd bb9;bbd bb23[8 ];bbf bb102[64 ];}bb529;bbb bb1818(
bb529*bbj);bbb bb1292(bb529*bbj,bbh bbb*bb5,bbq bb9);bbb bb1814(bb529
 *bbj,bbb*bb14);bba bbi{bbd bb9;bb56 bb23[8 ];bbf bb102[128 ];}bb463;
bbb bb1808(bb463*bbj);bbb bb1228(bb463*bbj,bbh bbb*bb5,bbq bb9);bbb
bb1835(bb463*bbj,bbb*bb14);bba bb463 bb926;bbb bb1797(bb926*bbj);bbb
bb1810(bb926*bbj,bbb*bb14);bbb bb1902(bbb*bb14,bbh bbb*bb5,bbq bb9);
bbb bb1866(bbb*bb14,bbh bbb*bb5,bbq bb9);bbb bb1849(bbb*bb14,bbh bbb*
bb5,bbq bb9);bbb bb1929(bbb*bb14,bbh bbb*bb5,bbq bb9);bbb bb2016(bbb*
bb14,bb83 bb5);bbb bb1965(bbb*bb14,bb83 bb5);bbb bb2025(bbb*bb14,bb83
bb5);bbb bb2022(bbb*bb14,bb83 bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbd bb9;bbd bb23[5 ];bbf bb102[64 ];}bb525;bbb bb1801(bb525*bbj
);bbb bb1295(bb525*bbj,bbh bbb*bb498,bbq bb9);bbb bb1795(bb525*bbj,
bbb*bb14);bbb bb1924(bbb*bb14,bbh bbb*bb5,bbq bb9);bbb bb1973(bbb*
bb14,bb83 bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbd bb9;bbd bb23[5 ];bbf bb102[64 ];}bb532;bbb bb1804(bb532*bbj
);bbb bb1356(bb532*bbj,bbh bbb*bb498,bbq bb9);bbb bb1838(bb532*bbj,
bbb*bb14);bbb bb1875(bbb*bb14,bbh bbb*bb5,bbq bb9);bbb bb2002(bbb*
bb14,bb83 bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbb( *bb1059)(bbb*bbj);bba bbb( *bb839)(bbb*bbj,bbh bbb*bb5,bbq
bb9);bba bbb( *bb784)(bbb*bbj,bbb*bb14);bba bbi{bbe bb129;bbq bb38;
bbq bb393;bb1059 bb887;bb839 bb180;bb784 bb746;}bb448;bbb bb1858(
bb448*bbj,bbe bb129);bba bbi{bb448 bbn;bbf bbt[256 -bb12(bb448)];}
bb454;bbb bb1984(bb454*bbj,bbe bb129);bbb bb1991(bb454*bbj);bbb bb2020
(bb454*bbj,bbe bb129);bbb bb1983(bb454*bbj,bbh bbb*bb5,bbq bb9);bbb
bb1976(bb454*bbj,bbb*bb14);bbb bb1989(bbe bb129,bbb*bb14,bbh bbb*bb5,
bbq bb9);bbb bb2049(bbe bb129,bbb*bb14,bb83 bb5);bb83 bb1970(bbe bb129
);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bb448 bbn;bbf bb543[(512 -bb12(bb448))/2 ];bbf bb1341[(512 -bb12
(bb448))/2 ];}bb494;bbb bb1961(bb494*bbj,bbe bb595);bbb bb2005(bb494*
bbj,bbh bbb*bb71,bbq bb142);bbb bb2108(bb494*bbj,bbe bb595,bbh bbb*
bb71,bbq bb142);bbb bb1988(bb494*bbj,bbh bbb*bb5,bbq bb9);bbb bb2007(
bb494*bbj,bbb*bb14);bbb bb2107(bbe bb595,bbh bbb*bb71,bbq bb142,bbb*
bb14,bbh bbb*bb5,bbq bb9);bbb bb2190(bbe bb595,bb83 bb71,bbb*bb14,
bb83 bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bb364 bb2117;bbq bb9;bbf bb102[16 ];bbf bb1926[16 ];bbf bb1928[
16 ];bbf bb1842[16 ];}bb939;bbb bb2042(bb939*bbj,bbh bbb*bb71,bbq bb142
);bbb bb2092(bb939*bbj,bbh bbb*bb5,bbq bb9);bbb bb2103(bb939*bbj,bbb*
bb14);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbb( *bb1865)(bbb*bbj,bbh bbb*bb71,bbq bb142);bba bbi{bbe bb129;
bbq bb38;bbq bb393;bb1865 bb887;bb839 bb180;bb784 bb746;}bb2029;bba
bbi{bb2029 bbn;bbf bbt[512 ];}bb527;bbb bb2144(bb527*bbj,bbe bb129);
bbb bb2168(bb527*bbj,bbh bbb*bb71,bbq bb142);bbb bb1833(bb527*bbj,bbe
bb129,bbh bbb*bb71,bbq bb142);bbb bb1244(bb527*bbj,bbh bbb*bb5,bbq bb9
);bbb bb1815(bb527*bbj,bbb*bb14);bbb bb2165(bbe bb129,bbh bbb*bb71,
bbq bb142,bbb*bb14,bbh bbb*bb5,bbq bb9);bbb bb2195(bbe bb129,bb83 bb71
,bbb*bb14,bb83 bb5);bb83 bb2205(bbe bb129);
#ifdef __cplusplus
}
#endif
bb41 bbf bb2352[]={0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 };bb4 bb2057(bbx*
bb299,bbd*bb104){bb4 bb18;bbd bb368;bbf bb74[8 ]; *bb104=0 ;bb18=bb537(
bb299,50 ,&bb368);bbm(((bb18)!=bb98))bb2 bb18;bb563(bb299,bb368,bb74,8
); *bb104=bb542(bb402,&(bb74[0 ]));bb2 bb18;}bb4 bb2116(bbx*bb85,bbu
bb177,bbd bb486,bb519 bb154,bbh bbf*bb1309,bbf*bb131,bb422 bb412,bbf*
bb553,bbd bb104,bbd bb497,bbx*bb58){bbd bb368;bbd bb163;bbd bb2365;
bbk bb151;bbd bb1298;bbd bb1745;bbd bb168;bbd bb146;bbd bb475;bbd
bb501;bbd bb1030;bbd bb157;bbd bb570;bbf bb940;bbf bb577;bbd bb142;
bbd bb2038;bbd bb38;bbd bb965;bbf bb191=0 ;bbf bb420=0 ;bbf bb981[32 ];
bbf bb910[32 ];bbf bb2179[32 ];bbf bb2434[64 ];bbf bb560[8 +64 ];bbf*
bb1914=bb91;bbf*bb613=bb91;bbx*bb73=bb91;bbx*bb46=bb91;bb4 bb18=bb98;
bb434*bb535=bb91;bb527*bb528=bb91;bbe bb740=0x100000 ,bb404;bb151=
bb547(bb85);bb300(bb154){bb15 bb615:bb142=24 ;bb38=8 ;bb740|=0x11 ;bb22;
bb15 bb931:bb142=16 ;bb38=16 ;bb740|=0x80 ;bb22;bb15 bb927:bb142=16 ;bb38
=16 ;bb740|=0x91 ;bb22;bb15 bb922:bb142=24 ;bb38=16 ;bb740|=0x92 ;bb22;
bb15 bb916:bb142=32 ;bb38=16 ;bb740|=0x93 ;bb22;bb15 bb923:bb142=16 ;bb38
=16 ;bb740|=0x20 ;bb22;bb15 bb925:bb142=24 ;bb38=16 ;bb740|=0x21 ;bb22;
bb15 bb918:bb142=32 ;bb38=16 ;bb740|=0x22 ;bb22;bb15 bb601:bb142=0 ;bb38=
0 ;bb740|=0x00 ;bb22;bb419:bb2 bb567;}bb535=bb128(bb12( *bb535));bb2068
(bb535,bb740);bb690(bb85,bb177,&bb940);bb368=bb518(bb85);bb18=bb596(
bb85,50 ,bb58);bbm(((bb18)!=bb98))bb100 bb164;bbm(bb177)bb151+=bb368;
bb2038=bb151-bb368;bb1298=bb177?0 :bb368;bb965=bb2038;bb1745=bb368+8 +
bb38;bbm(bb154!=bb601){bb1 bb1261=bb91;bb2065(bb535,0x0100 |0x1000000 ,
bb1309,bb131?bb131:bb2352);bbm(((bb18)!=bb98))bb100 bb164;bb73=bb85;
bb46=bb58;bb475=0 ;bb501=0 ;{bb157=bb73->bb125;bb570=0 ;bb106(bb157<=
bb1298&&bb73->bb93){bb570=bb157;bb73=bb73->bb93;bb157+=bb73->bb125;}
bb168=bb1298-bb570;bbm(bb157<=bb1298){bb18=bb377;bb100 bb164;}}{bb157
=bb46->bb181;bb570=0 ;bb106(bb157<=bb1745&&bb46->bb93){bb570=bb157;
bb46=bb46->bb93;bb157+=bb46->bb181;}bb146=bb1745-bb570;bbm(bb157<=
bb1745){bb18=bb377;bb100 bb164;}}bb106(bb965>=bb38&&bb73){bbm(!bb46){
bb18=bb377;bb100 bb164;}bb475=0 ;bb501=0 ;bbm(bb73->bb125-bb168>=bb38&&
bb46->bb181-bb146>=bb38){bb1030=((bb73->bb125-bb168)<(bb46->bb181-
bb146)?(bb73->bb125-bb168):(bb46->bb181-bb146));bb1030=bb1030-bb1030%
bb38;{bbq bb393;bb1615(bb535,&bb46->bb74[bb146],&bb393,&bb73->bb74[
bb168],bb1030);bb1261=&bb46->bb74[bb146]+bb393;}bb146+=bb1030;bb168+=
bb1030;bb965-=bb1030;bbm(bb168==bb73->bb125){bb73=bb73->bb93;bb168=0 ;
}bbm(bb146==bb46->bb181){bb46->bb125=bb46->bb181;bb46=bb46->bb93;
bb146=0 ;}}bb54{bbm(bb73->bb125-bb168<bb38){bb1914=bb981;bb106(bb965>=
bb38-bb475&&bb475<bb38&&bb73){bb157=bb73->bb125-bb168;bbm(bb157>bb38-
bb475)bb157=bb38-bb475;bb81(&bb981[bb475],&bb73->bb74[bb168],bb157);
bb475+=bb157;bb168+=bb157;bb965-=bb157;bbm(bb168==bb73->bb125){bb73=
bb73->bb93;bb168=0 ;}}}bb54{bb1914=&bb73->bb74[bb168];bb475=bb38;bb168
+=bb38;bb965-=bb38;}bb613=bb46->bb181-bb146<bb38?bb910:&bb46->bb74[
bb146];bbm(bb475==bb38){bb475=0 ;{bbq bb393;bb1615(bb535,bb613,&bb393,
bb1914,bb38);bb1261=bb613+bb393;}bbm(bb613==bb910){bb501=bb46->bb181-
bb146;bb81(&bb46->bb74[bb146],bb910,bb501);bb46->bb125=bb46->bb181;
bb46=bb46->bb93;bbm(!bb46){bb18=bb377;bb100 bb164;}bb81(bb46->bb74,&
bb910[bb501],bb38-bb501);bb146=bb38-bb501;}bb54 bb146+=bb38;}}}{bbm(
bb965){bbm(!bb73){bb18=bb1086;bb100 bb164;}bbm(bb73->bb125-bb168<
bb965){bb81(&bb981[bb475],&bb73->bb74[bb168],bb73->bb125-bb168);bb475
+=bb73->bb125-bb168;bb965-=bb73->bb125-bb168;bb73=bb73->bb93;bb81(&
bb981[bb475],bb73->bb74,bb965);}bb54 bb81(&bb981[bb475],&bb73->bb74[
bb168],bb965);bb475+=bb965;}bbm(!bb46){bb18=bb377;bb100 bb164;}bb613=
bb46->bb181-bb146<bb38?bb910:&bb46->bb74[bb146];bbm(bb475<bb38-1 ){
bb577=bb38-bb475-2 ;bb90(bb163=0 ;bb163<bb577;bb163++)bb981[bb163+bb475
]=bb163+1 ;bb981[bb475+bb577]=bb577;bb981[bb475+bb577+1 ]=bb940;{bbq
bb393;bb1615(bb535,bb613,&bb393,bb981,bb38);bb1261=bb613+bb393;}bbm(
bb613==bb910){bb501=bb46->bb181-bb146;bb81(&bb46->bb74[bb146],bb910,
bb501);bb46->bb125=bb46->bb181;bb46=bb46->bb93;bb81(bb46->bb74,&bb910
[bb501],bb38-bb501);bb146=bb38-bb501;}bb54 bb146+=bb38;}bb54{bb981[
bb38-1 ]=1 ;{bbq bb393;bb1615(bb535,bb613,&bb393,bb981,bb38);bb1261=
bb613+bb393;}bbm(bb613==bb910){bb501=bb46->bb181-bb146;bb81(&bb46->
bb74[bb146],bb910,bb501);bb46->bb125=bb46->bb181;bb46=bb46->bb93;bb81
(bb46->bb74,&bb910[bb501],bb38-bb501);bb146=bb38-bb501;}bb54 bb146+=
bb38;bb613=bb46->bb181-bb146<bb38?bb910:&bb46->bb74[bb146];{bb90(
bb163=0 ;bb163<bb38-2 ;bb163++)bb981[bb163]=bb163+2 ;bb577=bb38-1 ;bb981[
bb38-2 ]=bb577;bb981[bb38-1 ]=bb940;{bbq bb393;bb1615(bb535,bb613,&
bb393,bb981,bb38);bb1261=bb613+bb393;}}bbm(bb613==bb910){bb501=bb46->
bb181-bb146;bb81(&bb46->bb74[bb146],bb910,bb501);bb46->bb125=bb46->
bb181;bb46=bb46->bb93;bb81(bb46->bb74,&bb910[bb501],bb38-bb501);bb146
=bb38-bb501;}bb54 bb146+=bb38;}}bb151+=bb577+2 +8 +bb38;bb46->bb125=
bb146;bb32(bb1261);bb2097(bb535,bb1261,bb91);}bb54{bb18=bb467(bb85,
bb1298,bb58,bb368+8 ,bb2038);bbm(((bb18)!=bb98))bb100 bb164;bb2365=
bb368+8 +bb2038;bb577=3 -(bb2038+2 +3 )%4 ;bb90(bb163=0 ;bb163<bb577;bb163
++)bb2179[bb163]=bb163+1 ;bb2179[bb577]=bb577;bb2179[bb577+1 ]=bb940;
bb18=bb526(bb58,bb2365,bb2179,bb577+2 );bbm(((bb18)!=bb98))bb100 bb164
;bb151+=bb577+2 +8 ;}bb976(bb104,bb402,&bb560[0 ]);bb976(bb497,bb402,&
bb560[4 ]);bb81(&bb560[8 ],bb131?bb131:bb2352,bb38);bbm(bb131&&bb613)bb81
(bb131,bb613,bb38);bb18=bb526(bb58,bb368,bb560,8 +bb38);bbm(((bb18)!=
bb98))bb100 bb164;bb18=bb596(bb85,50 ,bb58);bbm(((bb18)!=bb98))bb100
bb164;bbm(bb177)bb921(bb58,bb486);bb522(bb151,bb58);bbm(bb412!=bb554){
bb300(bb412){bb15 bb582:bb191=16 ;bb420=16 ;bb404=0x1000 |0x10 ;bb22;bb15
bb580:bb191=20 ;bb420=20 ;bb404=0x1000 |0x20 ;bb22;bb15 bb561:bb191=32 ;
bb420=32 ;bb404=0x1000 |0x21 ;bb22;bb15 bb575:bb191=48 ;bb420=48 ;bb404=
0x1000 |0x22 ;bb22;bb15 bb562:bb191=64 ;bb420=64 ;bb404=0x1000 |0x23 ;bb22;
bb15 bb579:bb191=16 ;bb420=16 ;bb404=0x2000 ;bb22;bb15 bb587:bb191=20 ;
bb420=20 ;bb404=0x1000 |0x80 ;bb22;bb15 bb827:bb191=20 ;bb420=20 ;bb404=
0x1000 |0x30 ;bb22;bb419:bb18=bb531;bb100 bb164;}bbm(bb191>12 )bb191=12 ;
bb528=bb128(bb12( *bb528));bb1833(bb528,bb404,bb553,(bbq)bb420);bb146
=bb368;bb46=bb58;bb106(bb146>bb46->bb125){bb146-=bb46->bb125;bb46=
bb46->bb93;}bb157=bb46->bb125-bb146;bb1244(bb528,bb46->bb74+bb146,
bb157);bb106(bb157<bb151-bb368){bb46=bb46->bb93;bb157+=bb46->bb125;
bb1244(bb528,bb46->bb74,bb46->bb125);}bb1815(bb528,bb2434);bb18=bb526
(bb46,bb46->bb125,bb2434,bb191);bbm(((bb18)!=bb98))bb100 bb164;bb151
+=bb191;bb522(bb151,bb58);}bb541(bb58);bb164:bbm(bb535)bb108(bb535);
bbm(bb528)bb108(bb528);bb2 bb18;}bb4 bb2044(bbx*bb85,bbu bb177,bb519
bb154,bbh bbf*bb1309,bb422 bb412,bbf*bb553,bbd*bb489,bbd*bb459,bbd*
bb539,bbx*bb58){bbd bb368;bbd bb497=0 ;bbk bb151;bbd bb168;bbd bb146;
bbd bb475;bbd bb501;bbd bb1030;bbd bb1298;bbd bb1745;bbd bb157;bbd
bb570;bbd bb142;bbd bb38;bbd bb965;bbf bb577;bbf bb940;bbf bb191=0 ;
bbf bb420=0 ;bbf*bb131=bb91;bbf bb1828[64 ];bbf bb981[32 ];bbf bb910[32 ]
;bbf bb560[8 +64 ];bbf*bb1914=bb91;bbf*bb613=bb91;bbx*bb73=bb91;bbx*
bb46=bb91;bbx*bb469=bb91;bb4 bb18=bb98;bb434*bb535=bb91;bb527*bb528=
bb91;bbe bb740=0x100000 ,bb404;bb151=bb547(bb85);bb300(bb154){bb15
bb615:bb142=24 ;bb38=8 ;bb740|=0x11 ;bb22;bb15 bb931:bb142=16 ;bb38=16 ;
bb740|=0x80 ;bb22;bb15 bb927:bb142=16 ;bb38=16 ;bb740|=0x91 ;bb22;bb15
bb922:bb142=24 ;bb38=16 ;bb740|=0x92 ;bb22;bb15 bb916:bb142=32 ;bb38=16 ;
bb740|=0x93 ;bb22;bb15 bb923:bb142=16 ;bb38=16 ;bb740|=0x20 ;bb22;bb15
bb925:bb142=24 ;bb38=16 ;bb740|=0x21 ;bb22;bb15 bb918:bb142=32 ;bb38=16 ;
bb740|=0x22 ;bb22;bb15 bb601:bb142=0 ;bb38=0 ;bb740|=0x00 ;bb22;bb419:bb2
bb567;}bb535=bb128(bb12( *bb535));bb2068(bb535,bb740);bb18=bb537(bb85
,50 ,&bb368);bbm(((bb18)!=bb98))bb100 bb164;bb563(bb85,bb368,bb560,8 +
bb38);bbm( *bb459<(bb12( *bb489)<<3 )-1 ) *bb459=(bb12( *bb489)<<3 )-1 ; *
bb539= *bb459-(bb12( *bb489)<<3 )+1 ;bb497=bb542(bb402,&bb560[4 ]);bbm(
bb497< *bb539){bb18=bb1020;bb100 bb164;}bbm(bb497<= *bb459&& *bb489&1
<<(bb497- *bb539)){bb18=bb999;bb100 bb164;}bbm(bb412!=bb554){bb300(
bb412){bb15 bb582:bb191=16 ;bb420=16 ;bb404=0x1000 |0x10 ;bb22;bb15 bb580
:bb191=20 ;bb420=20 ;bb404=0x1000 |0x20 ;bb22;bb15 bb561:bb191=32 ;bb420=
32 ;bb404=0x1000 |0x21 ;bb22;bb15 bb575:bb191=48 ;bb420=48 ;bb404=0x1000 |
0x22 ;bb22;bb15 bb562:bb191=64 ;bb420=64 ;bb404=0x1000 |0x23 ;bb22;bb15
bb579:bb191=16 ;bb420=16 ;bb404=0x2000 ;bb22;bb15 bb587:bb191=20 ;bb420=
20 ;bb404=0x1000 |0x80 ;bb22;bb15 bb827:bb191=20 ;bb420=20 ;bb404=0x1000 |
0x30 ;bb22;bb419:bb18=bb531;bb100 bb164;}bbm(bb191>12 )bb191=12 ;bb528=
bb128(bb12( *bb528));bb1833(bb528,bb404,bb553,bb420);bb73=bb85;bb168=
bb368;bb157=bb73->bb125>(bbd)(bb151-bb191)?bb151-bb368-bb191:bb73->
bb125-bb168;bb1244(bb528,bb73->bb74+bb168,bb157);bb168+=bb157;bb106(
bb157<bb151-bb368-bb191){bb73=bb73->bb93;bb570=bb157;bb157+=bb73->
bb125;bb168=bb157<=bb151-bb368-bb191?bb73->bb125:bb151-bb368-bb191-
bb570;bb1244(bb528,bb73->bb74,bb168);}bb1815(bb528,bb1828);bbm(!bb833
(bb85,bb151-bb191,bb1828,bb191)){bb18=bb1031;bb100 bb164;}bb151-=
bb191;bbm(bb168+bb191>bb73->bb125)bb73->bb93->bb125=0 ;bb73->bb125=
bb168;}bbm(bb154!=bb601){bb1 bb1261=bb91;bb131=(bbf* )bb499(bb38,
bb558);bbm(!bb131){bb18=bb345;bb100 bb164;}bb81(bb131,&bb560[8 ],bb38);
bb2065(bb535,0x0200 |0x1000000 ,bb1309,bb131);bb73=bb85;bb46=bb58;bb475
=0 ;bb501=0 ;bb1298=bb368+8 +bb38;bb965=bb151-bb1298;bb1745=bb177?0 :
bb368;{bb157=bb73->bb125;bb570=0 ;bb106(bb157<=bb1298&&bb73->bb93){
bb570=bb157;bb73=bb73->bb93;bb157+=bb73->bb125;}bb168=bb1298-bb570;
bbm(bb157<=bb1298){bb18=bb377;bb100 bb164;}}{bb157=bb46->bb181;bb570=
0 ;bb106(bb157<=bb1745&&bb46->bb93){bb570=bb157;bb46=bb46->bb93;bb157
+=bb46->bb181;}bb146=bb1745-bb570;bbm(bb157<=bb1745){bb18=bb377;bb100
bb164;}}bb106(bb965>=bb38&&bb73){bbm(!bb46){bb18=bb377;bb100 bb164;}
bb475=0 ;bb501=0 ;bbm(bb73->bb125-bb168>=bb38&&bb46->bb181-bb146>=bb38){
bb1030=((bb73->bb125-bb168)<(bb46->bb181-bb146)?(bb73->bb125-bb168):(
bb46->bb181-bb146));bb1030=bb1030-bb1030%bb38;{bbq bb393;bb1615(bb535
,&bb46->bb74[bb146],&bb393,&bb73->bb74[bb168],bb1030);bb1261=&bb46->
bb74[bb146]+bb393;}bb146+=bb1030;bb168+=bb1030;bb965-=bb1030;bbm(
bb168==bb73->bb125){bb73=bb73->bb93;bb168=0 ;}bbm(bb146==bb46->bb181){
bb46->bb125=bb46->bb181;bb469=bb46;bb46=bb46->bb93;bb146=0 ;}}bb54{bbm
(bb73->bb125-bb168<bb38){bb1914=bb981;bb106(bb475<bb38&&bb73){bb157=
bb73->bb125-bb168;bbm(bb157>bb38-bb475)bb157=bb38-bb475;bb81(&bb981[
bb475],&bb73->bb74[bb168],bb157);bb475+=bb157;bb168+=bb157;bb965-=
bb157;bbm(bb168==bb73->bb125){bb73=bb73->bb93;bb168=0 ;}}}bb54{bb1914=
&bb73->bb74[bb168];bb475=bb38;bb168+=bb38;bb965-=bb38;}bb613=bb46->
bb181-bb146<bb38?bb910:&bb46->bb74[bb146];bbm(bb475==bb38){bb475=0 ;{
bbq bb393;bb1615(bb535,bb613,&bb393,bb1914,bb38);bb1261=bb613+bb393;}
bbm(bb613==bb910){bb501=bb46->bb181-bb146;bb81(&bb46->bb74[bb146],
bb910,bb501);bb46->bb125=bb46->bb181;bb469=bb46;bb46=bb46->bb93;bbm(!
bb46){bb18=bb377;bb100 bb164;}bb81(bb46->bb74,&bb910[bb501],bb38-
bb501);bb146=bb38-bb501;}bb54 bb146+=bb38;}}}bb32(bb1261);bb2097(
bb535,bb1261,bb91);}bb54{bb18=bb467(bb85,bb368+8 +bb38,bb58,bb177?0 :
bb368,bb151-bb368-8 );bbm(((bb18)!=bb98))bb100 bb164;bb46=bb58;bb469=
bb91;bb106(bb46->bb93){bbm(bb46->bb93->bb125==0 )bb22;bb469=bb46;bb46=
bb46->bb93;}bb146=bb46->bb125;}bbm(bb497> *bb459){ *bb489>>=bb497- *
bb459; *bb489&=0x7fffffff >>(bb497- *bb459-1 ); *bb459=bb497; *bb539= *
bb459-(bb12( *bb489)<<3 )+1 ;} *bb489|=1 <<(bb497- *bb539);bbm(bb146>1 ){
bb577=bb46->bb74[bb146-2 ];bb940=bb46->bb74[bb146-1 ];}bb54 bbm(bb146==
1 ){bb577=bb469->bb74[bb469->bb125-1 ];bb940=bb46->bb74[0 ];}bb54{bb577=
bb469->bb74[bb469->bb125-2 ];bb940=bb469->bb74[bb469->bb125-1 ];}bbm(
bb146>(bbd)(bb577+2 )){bb146-=bb577+2 ;bb46->bb125=bb146;}bb54{bbm(!
bb469){bb18=bb377;bb100 bb164;}bb46->bb125=0 ;bb469->bb125-=bb577+2 -
bb146;bb146=bb469->bb125;bb46=bb469;}bbm(bb940==4 &&!bb177){bb177=1 ;
bb18=bb467(bb58,bb368,bb58,0 ,bb151-(bb368+8 +bb38)-(bb577+2 ));bbm(((
bb18)!=bb98))bb100 bb164;}bb151-=(bb177?bb368:0 )+8 +bb38+bb577+2 ;bbm(!
bb177){bb18=bb924(bb85,50 ,bb940,bb58);bbm(((bb18)!=bb98))bb100 bb164;
}bb522(bb151,bb58);bb541(bb58);bb164:bbm(bb131)bb461(bb131);bbm(bb535
)bb108(bb535);bbm(bb528)bb108(bb528);bb2 bb18;}
