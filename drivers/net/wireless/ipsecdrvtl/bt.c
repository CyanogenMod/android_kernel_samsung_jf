/*
   'src_pm_ftp_nat.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Mon Oct 29 11:54:27 2012
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
bba bb9{bb402,bb1501,}bb297;bbk bb1208(bb297 bb706,bbh bbf*bb454);bbd
bb546(bb297 bb706,bbh bbf*bb454);bbb bb1174(bbk bb159,bb297 bb553,bbf
bb435[2 ]);bbb bb975(bbd bb159,bb297 bb553,bbf bb435[4 ]);
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
bbk bb923(bbh bbb*bb295);bbk bb868(bbh bbb*bb504,bbe bb22);bba bb83
bb4;bb9{bb98=0 ,bb363=-12000 ,bb351=-11999 ,bb377=-11998 ,bb664=-11997 ,
bb804=-11996 ,bb714=-11995 ,bb869=-11994 ,bb894=-11992 ,bb771=-11991 ,
bb873=-11990 ,bb723=-11989 ,bb623=-11988 ,bb646=-11987 ,bb681=-11986 ,
bb769=-11985 ,bb692=-11984 ,bb628=-11983 ,bb648=-11982 ,bb795=-11981 ,
bb903=-11980 ,bb673=-11979 ,bb836=-11978 ,bb829=-11977 ,bb587=-11976 ,
bb690=-11975 ,bb748=-11960 ,bb629=-11959 ,bb683=-11500 ,bb721=-11499 ,
bb844=-11498 ,bb785=-11497 ,bb684=-11496 ,bb750=-11495 ,bb851=-11494 ,
bb764=-11493 ,bb712=-11492 ,bb883=-11491 ,bb635=-11490 ,bb663=-11489 ,
bb694=-11488 ,bb881=-11487 ,bb835=-11486 ,bb888=-11485 ,bb641=-11484 ,
bb904=-11483 ,bb822=-11482 ,bb841=-11481 ,bb831=-11480 ,bb746=-11479 ,
bb640=-11478 ,bb707=-11477 ,bb747=-11476 ,bb751=-11475 ,bb798=-11474 ,
bb801=-11473 ,bb697=-11472 ,bb772=-11460 ,bb636=-11450 ,bb727=-11449 ,
bb699=-11448 ,bb722=-11447 ,bb850=-11446 ,bb760=-11445 ,bb861=-11444 ,
bb814=-11443 ,bb830=-11440 ,bb856=-11439 ,bb781=-11438 ,bb718=-11437 ,
bb689=-11436 ,bb667=-11435 ,bb617=-11420 ,bb532=-11419 ,bb565=-11418 ,
bb679=-11417 ,bb857=-11416 ,bb661=-11415 ,bb768=-11414 ,bb720=-11413 ,
bb840=-11412 ,bb738=-11411 ,bb676=-11410 ,bb627=-11409 ,bb762=-11408 ,
bb810=-11407 ,bb906=-11406 ,bb756=-11405 ,bb886=-11404 ,bb651=-11403 ,
bb749=-11402 ,bb630=-11401 ,bb672=-11400 ,bb879=-11399 ,bb625=-11398 ,
bb650=-11397 ,bb675=-11396 ,bb870=-11395 ,bb794=-11394 ,bb833=-11393 ,
bb800=-11392 ,bb685=-11391 ,bb892=-11390 ,bb716=-11389 ,bb789=-11388 ,
bb864=-11387 ,bb726=-11386 ,bb742=-11385 ,bb693=-11384 ,bb899=-11383 ,
bb644=-11382 ,bb638=-11381 ,bb724=-11380 ,bb776=-11379 ,bb622=-11378 ,
bb740=-11377 ,bb793=-11376 ,bb865=-11375 ,bb784=-11374 ,bb797=-11373 ,
bb677=-11372 ,bb758=-11371 ,bb660=-11370 ,bb766=-11369 ,bb696=-11368 ,
bb744=-11367 ,bb878=-11366 ,bb656=-11365 ,bb843=-11364 ,bb907=-11363 ,
bb386=-11350 ,bb691=bb386,bb703=-11349 ,bb860=-11348 ,bb890=-11347 ,bb717
=-11346 ,bb645=-11345 ,bb682=-11344 ,bb813=-11343 ,bb658=-11342 ,bb807=-
11341 ,bb752=-11340 ,bb732=-11339 ,bb388=-11338 ,bb852=-11337 ,bb816=bb388
,bb786=-11330 ,bb618=-11329 ,bb838=-11328 ,bb773=-11327 ,bb709=-11326 ,
bb639=-11325 ,bb796=-11324 ,bb862=-11320 ,bb688=-11319 ,bb671=-11318 ,
bb711=-11317 ,bb763=-11316 ,bb668=-11315 ,bb759=-11314 ,bb713=-11313 ,
bb782=-11312 ,bb757=-11300 ,bb637=-11299 ,bb783=-11298 ,bb695=-11297 ,
bb863=-11296 ,bb854=-11295 ,bb818=-11294 ,bb649=-11293 ,bb837=-11292 ,
bb891=-11291 ,bb901=-11290 ,bb791=-11289 ,bb876=-11288 ,bb848=-11287 ,
bb761=-11286 ,bb647=-11285 ,bb787=-11284 ,bb853=-11283 ,bb725=-11282 ,
bb670=-11281 ,bb788=-11280 ,bb846=-11279 ,bb730=-11250 ,bb827=-11249 ,
bb659=-11248 ,bb736=-11247 ,bb654=-11246 ,bb834=-11245 ,bb686=-11244 ,
bb743=-11243 ,bb819=-11242 ,bb799=-11240 ,bb634=-11239 ,bb719=-11238 ,
bb779=-11237 ,bb839=-11150 ,bb832=-11100 ,bb808=-11099 ,bb729=-11098 ,
bb858=-11097 ,bb624=-11096 ,bb780=-11095 ,bb755=-11094 ,bb871=-11093 ,
bb823=-11092 ,bb898=-11091 ,bb616=-11090 ,bb874=-11089 ,bb884=-11088 ,
bb842=-11087 ,bb815=-11086 ,bb770=-11085 ,bb775=-11050 ,bb739=-11049 ,
bb875=-10999 ,bb632=-10998 ,bb700=-10997 ,bb741=-10996 ,bb905=-10995 ,
bb674=-10994 ,bb872=-10993 ,bb824=-10992 ,bb805=-10991 ,bb895=-10990 ,
bb619=-10989 ,bb698=-10988 ,bb880=-10979 ,bb653=-10978 ,bb845=-10977 ,
bb877=-10976 ,bb733=-10975 ,bb705=-10974 ,};bba bbi bb450{bb1 bb74;bbd
bb127;bbd bb181;bbi bb450*bb93;}bbx;bb4 bb472(bbx*bb900,bbd bb911,bbx
 *bb825,bbd bb859,bbd bb543);bb4 bb526(bbx*bbj,bbd bb92,bbh bbb*bb97,
bbd bb47);bb4 bb571(bbx*bbj,bbd bb92,bbb*bb129,bbd bb47);bbu bb678(
bbx*bbj,bbd bb92,bbh bbb*bb97,bbd bb47);bb4 bb590(bbx*bb85,bbf bb102,
bbx*bb58);bb4 bb893(bbx*bb85,bbu bb177,bbf*bb405);bb4 bb937(bbx*bb58,
bbf*bb396);bb4 bb942(bbh bbf*bb396,bbx*bb58);bb4 bb541(bbx*bb50,bbf
bb102,bbd*bb943);bb4 bb924(bbx*bb85,bbf bb102,bbf bb405,bbx*bb58);bbd
bb520(bbx*bb50);bbk bb548(bbx*bb50);bbb bb528(bbk bb151,bbx*bb50);bbb
bb540(bbx*bb50);bbb bb979(bbx*bb50,bbd*bb26);bbb bb1030(bbx*bb50,bbd*
bb26);bbb bb994(bbx*bb50,bbd bb26);bbb bb932(bbx*bb50,bbd bb26);bbb
bb984(bbx*bb50);bbu bb1017(bbf*bb50);
#if defined( bb308) && defined( _WIN32)
#include"uncobf.h"
#include<stdio.h>
#include"cobf.h"
#endif
bba bbi{bbu bb1122;bbd bb203;bbk bb226;bbk bb427;bbd bb2150;bbd bb1825
;bbd bb1953;}bb2467, *bb2600;bb41 bbd bb2003=0 ;bb41 bb2467 bb464[5 ];
bb41 bbe bb2226(bbd bb203,bbk bb226,bbk bb427){bbe bbz;bb90(bbz=0 ;bbz
<(bbe)(bb12(bb464)/bb12((bb464)[0 ]));bbz++){bbm(bb464[bbz].bb1122&&
bb464[bbz].bb203==bb203&&bb464[bbz].bb226==bb226&&bb464[bbz].bb427==
bb427)bb2 bbz;}bb2-1 ;}bb41 bbe bb2369(){bbd bb2009=0xFFFFFFFF ;bbe bbz
,bb2422=0 ;bb90(bbz=0 ;bbz<(bbe)(bb12(bb464)/bb12((bb464)[0 ]));bbz++){
bbm(!bb464[bbz].bb1122)bb2 bbz;bbm(bb464[bbz].bb1953>=bb2009)bb2422=
bbz;}bb2 bb2422;}bb41 bbe bb2552(bbd bb203,bbk bb226,bbk bb427,bbd
bb2150,bbd bb1825){bbe bbz=bb2369();bb464[bbz].bb1122=1 ;bb464[bbz].
bb203=bb203;bb464[bbz].bb226=bb226;bb464[bbz].bb427=bb427;bb464[bbz].
bb2150=bb2150;bb464[bbz].bb1825=bb1825;bb464[bbz].bb1953=bb2003;bb2
bbz;}bb41 bbe bb2364(bbe bbz,bbd bb2460,bbd bb2462){bb464[bbz].bb2150
=bb2460;bb464[bbz].bb1825+=bb2462;bbm(bb464[bbz].bb1953==bb2003)bb2
bbz;bb2003++;bb464[bbz].bb1953=bb2003;bbm(bb2003==0xFFFFFFFF ){bbe bb76
;bbd bb2009=0xFFFFFFFF ;bb90(bb76=0 ;bb76<(bbe)(bb12(bb464)/bb12((bb464
)[0 ]));bb76++){bbm(bb464[bb76].bb1953<bb2009)bb2009=bb464[bb76].
bb1953;}bb90(bb76=0 ;bb76<(bbe)(bb12(bb464)/bb12((bb464)[0 ]));bb76++)bb464
[bbz].bb1953-=bb2009;bb2003-=bb2009;}bb2 bbz;}bb41 bb1 bb2484(bb1
bb1300,bbd*bb2509){bbe bbz,bb370=0 ;bb90(bbz=0 ;bbz<4 ;){bbm( *bb1300++
==',')bbz++;bb370++;} *bb2509=bb370;bb2 bb1300;}bb41 bbb bb2562(bbl*
bb2134,bbd bb1088,bbd*bb2445){bbl bbg[32 ];bbd bb11=bb1317(bbg,"\x25"
"\x64\x2c\x25\x64\x2c\x25\x64\x2c\x25\x64\x2c",bb1088&0xff ,bb1088>>8 &
0xff ,bb1088>>16 &0xff ,bb1088>>24 &0xff );bb81(bb2134,bbg,bb11); *bb2445=
bb11;}bbe bb2419(bb317 bb138,bbd bb1088){bb313 bb200;bb1 bb1300;bb1
bb437;bbl bb2134[20 ];bbd bb2277;bbd bb370;bbd bb1831;bbe bb163;bbd
bb203;bbk bb226;bbk bb427;bbd bb547;bbd bb2160;bbd bb2182;bb200=(
bb313)(bb138+1 );bbm(bb194(bb200->bb583)==21 ){bb2182=(bb200->bb92>>4 )<<
2 ;bb1300=(bb1)bb200+bb2182;bb203=bb546(bb402,(bb1)&bb138->bb203);
bb226=bb194(bb200->bb226);bb427=bb194(bb200->bb583);bb547=bb546(bb402
,(bb1)&bb200->bb547);bb2160=bb547+bb194(bb138->bb370)-bb12(bb325)-
bb2182;bbm(bb1898(bb1300,"\x50\x4f\x52\x54\x20",5 )||bb1898(bb1300,""
"\x70\x6f\x72\x74\x20",5 )){bb1300+=5 ;bb437=bb2484(bb1300,&bb370);
bb2277=bb194(bb138->bb370)-bb12(bb325)-bb2182-5 -bb370;bb2562(bb2134,
bb493(bb1088),&bb1831);bb2368(bb1300+bb1831,bb437,bb2277);bb81(bb1300
,bb2134,bb1831);{bbd bb11=bb138->bb370;bb11=bb194(bb11)+bb1831-bb370;
bb138->bb370=bb53(bb11);} * (bb1300+bb1831+bb2277)=0 ;bb163=bb2226(
bb203,bb226,bb427);bbm(bb163==-1 ){bb163=bb2369();bb2552(bb203,bb226,
bb427,bb2160,bb1831-bb370);}bb54{bb975(bb547+bb464[bb163].bb1825,
bb402,(bb1)&bb200->bb547);bb2364(bb163,bb2160,bb1831-bb370);}bb2
bb1831-bb370;}bb163=bb2226(bb203,bb226,bb427);bbm(bb163!=-1 ){bb975(
bb547+bb464[bb163].bb1825,bb402,(bb1)&bb200->bb547);bb2364(bb163,
bb2160,0 );bbm(bb200->bb172&0x01 )bb464[bb163].bb1122=0 ;}}bb2 0 ;}bbe
bb2315(bb317 bb138){bb313 bb200;bbd bb309;bbk bb226;bbk bb427;bbd
bb2244;bb200=(bb313)(bb138+1 );bbm(bb194(bb200->bb226)==21 ){bbe bbz;
bb309=bb546(bb402,(bb1)&bb138->bb309);bb226=bb194(bb200->bb226);bb427
=bb194(bb200->bb583);bb2244=bb546(bb402,(bb1)&bb200->bb920);bbz=
bb2226(bb309,bb427,bb226);bbm(bbz!=-1 ){bb2244-=bb464[bbz].bb1825;
bb975(bb2244,bb402,(bb1)&bb200->bb920);bb2 bb464[bbz].bb1825;}}bb2 0 ;
}
