/*
   'src_ipsec_pgpIPsecAH.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Mon Oct 29 11:54:27 2012
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
bba bbi bb1015*bb1022;bba bbi bb1029*bb1013;bba bbi bb995*bb1025;bba
bbi bb1001*bb1014;bba bbi bb1016*bb1026;bba bbi bb992*bb1024;bba bb9{
bb556=0 ,bb582=1 ,bb581=2 ,bb701=3 ,bb588=4 ,bb560=5 ,bb575=6 ,bb564=7 ,bb579
=9 ,}bb422;bba bb9{bb601=0 ,bb993,bb599,bb1012,bb929,bb931,bb926,bb919,
bb927,bb930,bb918,}bb515;bba bb83 bb4;bb9{bb98=0 ,bb363=-12000 ,bb351=-
11999 ,bb377=-11998 ,bb664=-11997 ,bb804=-11996 ,bb714=-11995 ,bb869=-
11994 ,bb894=-11992 ,bb771=-11991 ,bb873=-11990 ,bb723=-11989 ,bb623=-
11988 ,bb646=-11987 ,bb681=-11986 ,bb769=-11985 ,bb692=-11984 ,bb628=-
11983 ,bb648=-11982 ,bb795=-11981 ,bb903=-11980 ,bb673=-11979 ,bb836=-
11978 ,bb829=-11977 ,bb587=-11976 ,bb690=-11975 ,bb748=-11960 ,bb629=-
11959 ,bb683=-11500 ,bb721=-11499 ,bb844=-11498 ,bb785=-11497 ,bb684=-
11496 ,bb750=-11495 ,bb851=-11494 ,bb764=-11493 ,bb712=-11492 ,bb883=-
11491 ,bb635=-11490 ,bb663=-11489 ,bb694=-11488 ,bb881=-11487 ,bb835=-
11486 ,bb888=-11485 ,bb641=-11484 ,bb904=-11483 ,bb822=-11482 ,bb841=-
11481 ,bb831=-11480 ,bb746=-11479 ,bb640=-11478 ,bb707=-11477 ,bb747=-
11476 ,bb751=-11475 ,bb798=-11474 ,bb801=-11473 ,bb697=-11472 ,bb772=-
11460 ,bb636=-11450 ,bb727=-11449 ,bb699=-11448 ,bb722=-11447 ,bb850=-
11446 ,bb760=-11445 ,bb861=-11444 ,bb814=-11443 ,bb830=-11440 ,bb856=-
11439 ,bb781=-11438 ,bb718=-11437 ,bb689=-11436 ,bb667=-11435 ,bb617=-
11420 ,bb532=-11419 ,bb565=-11418 ,bb679=-11417 ,bb857=-11416 ,bb661=-
11415 ,bb768=-11414 ,bb720=-11413 ,bb840=-11412 ,bb738=-11411 ,bb676=-
11410 ,bb627=-11409 ,bb762=-11408 ,bb810=-11407 ,bb906=-11406 ,bb756=-
11405 ,bb886=-11404 ,bb651=-11403 ,bb749=-11402 ,bb630=-11401 ,bb672=-
11400 ,bb879=-11399 ,bb625=-11398 ,bb650=-11397 ,bb675=-11396 ,bb870=-
11395 ,bb794=-11394 ,bb833=-11393 ,bb800=-11392 ,bb685=-11391 ,bb892=-
11390 ,bb716=-11389 ,bb789=-11388 ,bb864=-11387 ,bb726=-11386 ,bb742=-
11385 ,bb693=-11384 ,bb899=-11383 ,bb644=-11382 ,bb638=-11381 ,bb724=-
11380 ,bb776=-11379 ,bb622=-11378 ,bb740=-11377 ,bb793=-11376 ,bb865=-
11375 ,bb784=-11374 ,bb797=-11373 ,bb677=-11372 ,bb758=-11371 ,bb660=-
11370 ,bb766=-11369 ,bb696=-11368 ,bb744=-11367 ,bb878=-11366 ,bb656=-
11365 ,bb843=-11364 ,bb907=-11363 ,bb386=-11350 ,bb691=bb386,bb703=-11349
,bb860=-11348 ,bb890=-11347 ,bb717=-11346 ,bb645=-11345 ,bb682=-11344 ,
bb813=-11343 ,bb658=-11342 ,bb807=-11341 ,bb752=-11340 ,bb732=-11339 ,
bb388=-11338 ,bb852=-11337 ,bb816=bb388,bb786=-11330 ,bb618=-11329 ,bb838
=-11328 ,bb773=-11327 ,bb709=-11326 ,bb639=-11325 ,bb796=-11324 ,bb862=-
11320 ,bb688=-11319 ,bb671=-11318 ,bb711=-11317 ,bb763=-11316 ,bb668=-
11315 ,bb759=-11314 ,bb713=-11313 ,bb782=-11312 ,bb757=-11300 ,bb637=-
11299 ,bb783=-11298 ,bb695=-11297 ,bb863=-11296 ,bb854=-11295 ,bb818=-
11294 ,bb649=-11293 ,bb837=-11292 ,bb891=-11291 ,bb901=-11290 ,bb791=-
11289 ,bb876=-11288 ,bb848=-11287 ,bb761=-11286 ,bb647=-11285 ,bb787=-
11284 ,bb853=-11283 ,bb725=-11282 ,bb670=-11281 ,bb788=-11280 ,bb846=-
11279 ,bb730=-11250 ,bb827=-11249 ,bb659=-11248 ,bb736=-11247 ,bb654=-
11246 ,bb834=-11245 ,bb686=-11244 ,bb743=-11243 ,bb819=-11242 ,bb799=-
11240 ,bb634=-11239 ,bb719=-11238 ,bb779=-11237 ,bb839=-11150 ,bb832=-
11100 ,bb808=-11099 ,bb729=-11098 ,bb858=-11097 ,bb624=-11096 ,bb780=-
11095 ,bb755=-11094 ,bb871=-11093 ,bb823=-11092 ,bb898=-11091 ,bb616=-
11090 ,bb874=-11089 ,bb884=-11088 ,bb842=-11087 ,bb815=-11086 ,bb770=-
11085 ,bb775=-11050 ,bb739=-11049 ,bb875=-10999 ,bb632=-10998 ,bb700=-
10997 ,bb741=-10996 ,bb905=-10995 ,bb674=-10994 ,bb872=-10993 ,bb824=-
10992 ,bb805=-10991 ,bb895=-10990 ,bb619=-10989 ,bb698=-10988 ,bb880=-
10979 ,bb653=-10978 ,bb845=-10977 ,bb877=-10976 ,bb733=-10975 ,bb705=-
10974 ,};bba bbi bb450{bb1 bb74;bbd bb127;bbd bb181;bbi bb450*bb93;}
bbx;bb4 bb472(bbx*bb900,bbd bb911,bbx*bb825,bbd bb859,bbd bb543);bb4
bb526(bbx*bbj,bbd bb92,bbh bbb*bb97,bbd bb47);bb4 bb571(bbx*bbj,bbd
bb92,bbb*bb129,bbd bb47);bbu bb678(bbx*bbj,bbd bb92,bbh bbb*bb97,bbd
bb47);bb4 bb2073(bbx*bb306,bbd*bb104);bb4 bb2088(bbx*bb85,bbu bb177,
bbd bb483,bb422 bb413,bbf*bb558,bbd bb104,bbd bb498,bbx*bb58);bb4
bb2063(bbx*bb85,bbu bb177,bb422 bb413,bbf*bb558,bbd*bb482,bbd*bb459,
bbd*bb534,bbx*bb58);
#define bb959 bb53(0x0800)
#define bb1142 bb53(0x0806)
#define bb952 bb53(0x01f4)
#define bb956 bb53(0x1194)
#define bb1137 bb53(0x4000)
#define bb1141 bb53(0x2000)
#define bb1112 bb53(0x1FFF)
#define bb1069( bb8) (( bb8) & bb53(0x2000 | 0x1FFF))
#define bb991( bb8) ((( bb194( bb8)) & 0x1FFF) << 3)
#define bb970( bb8) ((( bb8) & bb53(0x1FFF)) == 0)
#define bb494( bb8) (( bb8) & bb53(0x2000))
#define bb987( bb8) (!( bb494( bb8)))
#pragma pack(push, 1)
bba bbi{bbf bb369[6 ];bbf bb1008[6 ];bbk bb372;}bb365, *bb376;bba bbi{
bbf bb445[6 ];bbk bb372;}bb1083, *bb1093;bba bbi{bbf bb936:4 ;bbf bb1087
:4 ;bbf bb1054;bbk bb370;bbk bb867;bbk bb577;bbf bb1003;bbf bb254;bbk
bb614;bbd bb309;bbd bb203;}bb325, *bb317;bba bbi{bbk bb1035;bbk bb1046
;bbf bb1040;bbf bb1048;bbk bb1064;bbf bb1053[6 ];bbd bb1042;bbf bb1038
[6 ];bbd bb1066;}bb1076, *bb1081;
#pragma pack(pop)
bba bbi{bbk bb226;bbk bb427;bbk bb1007;bbk bb320;}bb412, *bb333;bba
bbi{bbk bb226;bbk bb583;bbd bb547;bbd bb920;bbf bb92;bbf bb172;bbk
bb158;bbk bb320;bbk bb990;}bb490, *bb313;bba bbi{bbf bb1092;bbf bb1071
;bbf bb1086;bbf bb1043;bbd bb1065;bbk bb1077;bbk bb374;bbd bb1061;bbd
bb1075;bbd bb1068;bbd bb1063;bbf bb1089[16 ];bbf bb1052[64 ];bbf bb25[
128 ];bbf bb1034[64 ];}bb1041, *bb1074;bba bbi{bbd bb309;bbd bb203;bbf
bb912;bbf bb254;bbk bb921;}bb612, *bb567;
#if defined( _WIN32)
#define bb53( bbc) (((( bbc) & 0XFF00) >> 8) | ((( bbc) & 0X00FF) <<  \
8))
#define bb194( bbc) ( bb53( bbc))
#define bb444( bbc) (((( bbc) & 0XFF000000) >> 24) | ((( bbc) &  \
0X00FF0000) >> 8) | ((( bbc) & 0X0000FF00) << 8) | ((( bbc) &  \
0X000000FF) << 24))
#define bb493( bbc) ( bb444( bbc))
#endif
bbk bb923(bbh bbb*bb295);bbk bb868(bbh bbb*bb504,bbe bb22);bb4 bb590(
bbx*bb85,bbf bb102,bbx*bb58);bb4 bb893(bbx*bb85,bbu bb177,bbf*bb405);
bb4 bb937(bbx*bb58,bbf*bb396);bb4 bb942(bbh bbf*bb396,bbx*bb58);bb4
bb541(bbx*bb50,bbf bb102,bbd*bb943);bb4 bb924(bbx*bb85,bbf bb102,bbf
bb405,bbx*bb58);bbd bb520(bbx*bb50);bbk bb548(bbx*bb50);bbb bb528(bbk
bb151,bbx*bb50);bbb bb540(bbx*bb50);bbb bb979(bbx*bb50,bbd*bb26);bbb
bb1030(bbx*bb50,bbd*bb26);bbb bb994(bbx*bb50,bbd bb26);bbb bb932(bbx*
bb50,bbd bb26);bbb bb984(bbx*bb50);bbu bb1017(bbf*bb50);bb9{bb1131=-
5000 ,bb1108=-4000 ,bb1000=-4999 ,bb989=-4998 ,bb1028=-4997 ,bb982=-4996 ,
bb1094=-4995 ,bb1085=-4994 ,bb1099=-4993 ,bb1009=-4992 ,bb1032=-4991 };bb4
bb1134(bb4 bb1138,bbd bb1119,bbl*bb1102);bba bb9{bb402,bb1501,}bb297;
bbk bb1208(bb297 bb706,bbh bbf*bb454);bbd bb546(bb297 bb706,bbh bbf*
bb454);bbb bb1174(bbk bb159,bb297 bb553,bbf bb435[2 ]);bbb bb975(bbd
bb159,bb297 bb553,bbf bb435[4 ]);
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
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbd bb10;bbd bb23[5 ];bbf bb101[64 ];}bb529;bbb bb1794(bb529*
bbj);bbb bb1288(bb529*bbj,bbh bbb*bb5,bbq bb10);bbb bb1803(bb529*bbj,
bbb*bb14);bba bbi{bbd bb10;bbd bb23[8 ];bbf bb101[64 ];}bb533;bbb bb1819
(bb533*bbj);bbb bb1291(bb533*bbj,bbh bbb*bb5,bbq bb10);bbb bb1815(
bb533*bbj,bbb*bb14);bba bbi{bbd bb10;bb56 bb23[8 ];bbf bb101[128 ];}
bb460;bbb bb1809(bb460*bbj);bbb bb1204(bb460*bbj,bbh bbb*bb5,bbq bb10
);bbb bb1836(bb460*bbj,bbb*bb14);bba bb460 bb913;bbb bb1797(bb913*bbj
);bbb bb1811(bb913*bbj,bbb*bb14);bbb bb1906(bbb*bb14,bbh bbb*bb5,bbq
bb10);bbb bb1866(bbb*bb14,bbh bbb*bb5,bbq bb10);bbb bb1849(bbb*bb14,
bbh bbb*bb5,bbq bb10);bbb bb1929(bbb*bb14,bbh bbb*bb5,bbq bb10);bbb
bb2016(bbb*bb14,bb82 bb5);bbb bb1965(bbb*bb14,bb82 bb5);bbb bb2025(
bbb*bb14,bb82 bb5);bbb bb2018(bbb*bb14,bb82 bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbd bb10;bbd bb23[5 ];bbf bb101[64 ];}bb523;bbb bb1802(bb523*
bbj);bbb bb1358(bb523*bbj,bbh bbb*bb496,bbq bb10);bbb bb1795(bb523*
bbj,bbb*bb14);bbb bb1923(bbb*bb14,bbh bbb*bb5,bbq bb10);bbb bb1973(
bbb*bb14,bb82 bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbd bb10;bbd bb23[5 ];bbf bb101[64 ];}bb531;bbb bb1805(bb531*
bbj);bbb bb1319(bb531*bbj,bbh bbb*bb496,bbq bb10);bbb bb1839(bb531*
bbj,bbb*bb14);bbb bb1873(bbb*bb14,bbh bbb*bb5,bbq bb10);bbb bb2002(
bbb*bb14,bb82 bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbb( *bb1060)(bbb*bbj);bba bbb( *bb828)(bbb*bbj,bbh bbb*bb5,bbq
bb10);bba bbb( *bb765)(bbb*bbj,bbb*bb14);bba bbi{bbe bb131;bbq bb38;
bbq bb389;bb1060 bb887;bb828 bb180;bb765 bb704;}bb448;bbb bb1855(
bb448*bbj,bbe bb131);bba bbi{bb448 bbn;bbf bbt[256 -bb12(bb448)];}
bb449;bbb bb1984(bb449*bbj,bbe bb131);bbb bb1991(bb449*bbj);bbb bb2020
(bb449*bbj,bbe bb131);bbb bb1983(bb449*bbj,bbh bbb*bb5,bbq bb10);bbb
bb1976(bb449*bbj,bbb*bb14);bbb bb1989(bbe bb131,bbb*bb14,bbh bbb*bb5,
bbq bb10);bbb bb2049(bbe bb131,bbb*bb14,bb82 bb5);bb82 bb1970(bbe
bb131);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bb448 bbn;bbf bb542[(512 -bb12(bb448))/2 ];bbf bb1359[(512 -bb12
(bb448))/2 ];}bb492;bbb bb1961(bb492*bbj,bbe bb586);bbb bb2006(bb492*
bbj,bbh bbb*bb71,bbq bb143);bbb bb2108(bb492*bbj,bbe bb586,bbh bbb*
bb71,bbq bb143);bbb bb1988(bb492*bbj,bbh bbb*bb5,bbq bb10);bbb bb2007
(bb492*bbj,bbb*bb14);bbb bb2107(bbe bb586,bbh bbb*bb71,bbq bb143,bbb*
bb14,bbh bbb*bb5,bbq bb10);bbb bb2190(bbe bb586,bb82 bb71,bbb*bb14,
bb82 bb5);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbi{bbq bb452;bbd bb418[4 * (14 +1 )];}bb364;bbb bb1100(bb364*bbj,
bbh bbb*bb71,bbq bb143);bbb bb1736(bb364*bbj,bbh bbb*bb71,bbq bb143);
bbb bb1036(bb364*bbj,bbb*bb14,bbh bbb*bb5);bbb bb1776(bb364*bbj,bbb*
bb14,bbh bbb*bb5);
#ifdef __cplusplus
}
#endif
bba bbi{bb364 bb2117;bbq bb10;bbf bb101[16 ];bbf bb1928[16 ];bbf bb1926
[16 ];bbf bb1842[16 ];}bb940;bbb bb2042(bb940*bbj,bbh bbb*bb71,bbq bb143
);bbb bb2093(bb940*bbj,bbh bbb*bb5,bbq bb10);bbb bb2103(bb940*bbj,bbb
 *bb14);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bbb( *bb1865)(bbb*bbj,bbh bbb*bb71,bbq bb143);bba bbi{bbe bb131;
bbq bb38;bbq bb389;bb1865 bb887;bb828 bb180;bb765 bb704;}bb2029;bba
bbi{bb2029 bbn;bbf bbt[512 ];}bb522;bbb bb2144(bb522*bbj,bbe bb131);
bbb bb2170(bb522*bbj,bbh bbb*bb71,bbq bb143);bbb bb1834(bb522*bbj,bbe
bb131,bbh bbb*bb71,bbq bb143);bbb bb1243(bb522*bbj,bbh bbb*bb5,bbq
bb10);bbb bb1816(bb522*bbj,bbb*bb14);bbb bb2166(bbe bb131,bbh bbb*
bb71,bbq bb143,bbb*bb14,bbh bbb*bb5,bbq bb10);bbb bb2195(bbe bb131,
bb82 bb71,bbb*bb14,bb82 bb5);bb82 bb2205(bbe bb131);
#ifdef __cplusplus
}
#endif
bb4 bb2073(bbx*bb306,bbd*bb104){bb4 bb18;bbd bb928;bbf bb74[12 ]; *
bb104=0 ;bb18=bb541(bb306,51 ,&bb928);bbm(((bb18)!=bb98))bb2 bb18;bb571
(bb306,bb928,bb74,12 ); *bb104=bb546(bb402,&bb74[4 ]);bb2 bb18;}bb4
bb2088(bbx*bb85,bbu bb177,bbd bb483,bb422 bb413,bbf*bb558,bbd bb104,
bbd bb498,bbx*bb58){bb4 bb18=bb98;bbf bb396[64 ];bbf bb1828[64 ];bbf
bb576[12 +64 ];bbf bb1392=12 ;bbd bb928;bbd bb157;bbk bb151;bbf bb938;
bbf bb188;bbf bb421=0 ;bbx*bb46;bb522 bb524;bbe bb407;bb151=bb548(bb85
);bb300(bb413){bb15 bb556:bb188=0 ;bb558=0 ;bb407=0 ;bb21;bb15 bb582:
bb188=16 ;bb421=16 ;bb407=0x1000 |0x10 ;bb21;bb15 bb581:bb188=20 ;bb421=20
;bb407=0x1000 |0x20 ;bb21;bb15 bb560:bb188=32 ;bb421=32 ;bb407=0x1000 |
0x21 ;bb21;bb15 bb575:bb188=48 ;bb421=48 ;bb407=0x1000 |0x22 ;bb21;bb15
bb564:bb188=64 ;bb421=64 ;bb407=0x1000 |0x23 ;bb21;bb15 bb579:bb188=16 ;
bb421=16 ;bb407=0x2000 ;bb21;bb15 bb588:bb188=20 ;bb421=20 ;bb407=0x1000 |
0x80 ;bb21;bb15 bb701:bb188=20 ;bb421=20 ;bb407=0x1000 |0x30 ;bb21;bb416:
bb2 bb532;}bbm(bb188>12 )bb188=12 ;bb1392+=bb188;bb151+=bb1392;bb893(
bb85,bb177,&bb938);bb928=bb520(bb85);bb18=bb590(bb85,51 ,bb58);bbm(((
bb18)!=bb98))bb99 bb164;bbm(bb177)bb151+=bb928;bb18=bb472(bb85,bb177?
0 :bb928,bb58,bb928+bb1392,bb151-bb928-bb1392);bbm(((bb18)!=bb98))bb99
bb164;bbm(bb177)bb932(bb58,bb483);bb528(bb151,bb58);bb18=bb937(bb58,
bb396);bbm(((bb18)!=bb98))bb99 bb164;{bb999(bb576,0 ,12 +bb188);bb576[0
]=bb938;bb576[1 ]=(bb1392/4 )-2 ;bb975(bb104,bb402,&bb576[4 ]);bb975(
bb498,bb402,&bb576[8 ]);bb18=bb526(bb58,bb928,bb576,12 +bb188);bbm(((
bb18)!=bb98))bb99 bb164;}bbm(bb413!=bb556){bb1834(&bb524,bb407,bb558,
(bbq)bb421);bb46=bb58;bb157=bb46->bb127;bb1243(&bb524,bb46->bb74,
bb157);bb106(bb157<bb151){bb46=bb46->bb93;bb157+=bb46->bb127;bb1243(&
bb524,bb46->bb74,bb46->bb127);}bb1816(&bb524,bb1828);bb18=bb526(bb58,
bb928+12 ,bb1828,bb188);bbm(((bb18)!=bb98))bb99 bb164;bb18=bb942(bb396
,bb58);bbm(((bb18)!=bb98))bb99 bb164;}bb528(bb151,bb58);bb540(bb58);
bb164:bb2 bb18;}bb4 bb2063(bbx*bb85,bbu bb177,bb422 bb413,bbf*bb558,
bbd*bb482,bbd*bb459,bbd*bb534,bbx*bb58){bb4 bb18=bb98;bbf bb396[64 ];
bbf bb1828[64 ];bbf bb2207[64 ];bbf bb576[12 +64 ];bbf bb1392=12 ;bbd bb928
;bbd bb163;bbd bb157;bbd bb498;bbk bb151;bbf bb188;bbf bb421=0 ;bbf
bb938;bbx*bb73;bb522 bb524;bbe bb407;bb151=bb548(bb85);bb300(bb413){
bb15 bb556:bb188=0 ;bb558=0 ;bb407=0 ;bb21;bb15 bb582:bb188=16 ;bb421=16 ;
bb407=0x1000 |0x10 ;bb21;bb15 bb581:bb188=20 ;bb421=20 ;bb407=0x1000 |0x20
;bb21;bb15 bb560:bb188=32 ;bb421=32 ;bb407=0x1000 |0x21 ;bb21;bb15 bb575:
bb188=48 ;bb421=48 ;bb407=0x1000 |0x22 ;bb21;bb15 bb564:bb188=64 ;bb421=64
;bb407=0x1000 |0x23 ;bb21;bb15 bb579:bb188=16 ;bb421=16 ;bb407=0x2000 ;
bb21;bb15 bb588:bb188=20 ;bb421=20 ;bb407=0x1000 |0x80 ;bb21;bb15 bb701:
bb188=20 ;bb421=20 ;bb407=0x1000 |0x30 ;bb21;bb416:bb2 bb532;}bb300(bb413
){bb15 bb560:bb15 bb575:bb15 bb564:bb188/=2 ;bb21;bb416:bb21;}bbm(
bb188>12 )bb188=12 ;bb1392+=bb188;bb18=bb541(bb85,51 ,&bb928);bbm(((bb18
)!=bb98))bb99 bb164;{bb571(bb85,bb928,bb576,12 +bb188);bb938=bb576[0 ];
bbm( *bb459<(bb12( *bb482)<<3 )-1 ) *bb459=(bb12( *bb482)<<3 )-1 ; *bb534
= *bb459-(bb12( *bb482)<<3 )+1 ;bb498=bb546(bb402,&bb576[8 ]);bbm(bb498<
 *bb534){bb18=bb989;bb99 bb164;}bbm(bb498<= *bb459&& *bb482&1 <<(bb498
- *bb534)){bb18=bb1000;bb99 bb164;}}bb18=bb937(bb85,bb396);bbm(((bb18
)!=bb98))bb99 bb164;bb81(bb2207,&bb576[12 ],bb188);bb90(bb163=12 ;bb163
<(bbd)(12 +bb188);bb163++)bb576[bb163]=0 ;bb526(bb85,bb928,bb576,12 +
bb188);bbm(bb413!=bb556){bb1834(&bb524,bb407,bb558,(bbq)bb421);{bb73=
bb85;bb157=bb73->bb127;bb1243(&bb524,bb73->bb74,bb157);bb81(&bb576[12
],bb2207,bb188);bb526(bb85,bb928,bb576,12 +bb188);bb106(bb157<bb151){
bb73=bb73->bb93;bb157+=bb73->bb127;bb1243(&bb524,bb73->bb74,bb73->
bb127);}}bb1816(&bb524,bb1828);bbm(bb1898(bb2207,bb1828,bb188)!=0 ){
bb18=bb1028;bb99 bb164;}}{bbm(bb498> *bb459){ *bb482>>=bb498- *bb459;
 *bb482&=0x7fffffff >>(bb498- *bb459-1 ); *bb459=bb498; *bb534= *bb459-
(bb12( *bb482)<<3 )+1 ;} *bb482|=1 <<(bb498- *bb534);}bb18=bb942(bb396,
bb85);bbm(((bb18)!=bb98))bb99 bb164;bbm(bb938==4 &&!bb177)bb177=1 ;bb18
=bb472(bb85,bb928+bb1392,bb58,bb177?0 :bb928,bb151-bb928-bb1392);bbm((
(bb18)!=bb98))bb99 bb164;bb151-=(bb177?bb928:0 )+bb1392;bbm(!bb177){
bb18=bb924(bb85,51 ,bb85->bb74[bb928],bb58);bbm(((bb18)!=bb98))bb99
bb164;}bb528(bb151,bb58);bb540(bb58);bb164:bb2 bb18;}
