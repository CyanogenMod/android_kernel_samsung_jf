/*
   'src_compress_pgpCompress.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Mon Oct 29 11:54:27 2012
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
10974 ,};bb9{bb552=1 ,};bbb*bb491(bbd bb1201,bbd bb374);bb4 bb457(bbb*
bb973);bba bbi bb1986 bb1972, *bb382;bba bb9{bb1995=0 ,bb1755=1 ,bb1769
=2 }bb774;bb4 bb1814(bb774 bb1871,bb382*bb359);bb4 bb1949(bb382 bb359,
bbf*bb431,bbd bb420,bbf*bb310,bbd bb293,bbd*bb434,bbd*bb304);bb4
bb1942(bb382 bb359,bbf*bb310,bbd bb293,bbd*bb304,bbu*bb976);bb4 bb1950
(bb382 bb359,bbf*bb431,bbd bb420,bbf*bb310,bbd bb293,bbd*bb434,bbd*
bb304,bbu*bb964);bb4 bb1799(bb382 bb359);bb4 bb2211(bb77*bb162);bb4
bb2240(bb77 bb162,bbf*bb431,bbd bb420,bbf*bb310,bbd bb293,bbd*bb434,
bbd*bb304);bb4 bb2291(bb77 bb162,bbf*bb310,bbd bb293,bbd*bb304,bbu*
bb976);bb4 bb2251(bb77*bb162);bb4 bb2254(bb77*bb162);bb4 bb2287(bb77
bb162,bbf*bb431,bbd bb420,bbf*bb310,bbd bb293,bbd*bb434,bbd*bb304,bbu
 *bb964);bb4 bb2247(bb77*bb162);bb4 bb2204(bb77*bb162);bb4 bb2294(
bb77 bb162,bbf*bb431,bbd bb420,bbf*bb310,bbd bb293,bbd*bb434,bbd*
bb304);bb4 bb2289(bb77 bb162,bbf*bb310,bbd bb293,bbd*bb304,bbu*bb976);
bb4 bb2243(bb77*bb162);bb4 bb2300(bb77*bb162);bb4 bb2203(bb77 bb162,
bbf*bb431,bbd bb420,bbf*bb310,bbd bb293,bbd*bb434,bbd*bb304,bbu*bb964
);bb4 bb2284(bb77*bb162);bba bb4( *bb2554)(bb77*bb162);bba bb4( *
bb2587)(bb77 bb162,bbf*bb431,bbd bb420,bbf* *bb310,bbd*bb293,bbd*
bb2495);bba bb4( *bb2485)(bb77 bb162,bbf*bb431,bbd bb420,bbf*bb310,
bbd bb293,bbd*bb434,bbd*bb304);bba bb4( *bb2486)(bb77 bb162,bbf*bb310
,bbd bb293,bbd*bb304,bbu*bb976);bba bb4( *bb2464)(bb77*bb162);bba bb4
( *bb2489)(bb77*bb162);bba bb4( *bb2609)(bb77 bb162,bbf*bb431,bbd
bb420,bbf* *bb310,bbd*bb293,bbd*bb2495);bba bb4( *bb2508)(bb77 bb162,
bbf*bb431,bbd bb420,bbf*bb310,bbd bb293,bbd*bb434,bbd*bb304,bbu*bb964
);bba bb4( *bb2459)(bb77*bb162);bbi bb1986{bb2554 bb2285;bb2485 bb2253
;bb2486 bb2276;bb2464 bb2201;bb2489 bb2217;bb2508 bb2282;bb2459 bb2239
;bb77 bb2141;bb77 bb2212;};bb4 bb1814(bb774 bb1871,bb382*bb359){bb382
bb591;bb4 bb18=bb98;bbm(!bb359)bb2 bb363; *bb359=bb91;bb591=(bb382)bb491
(bb12(bb1972),bb552);bbm(!bb591)bb2 bb351;bb300(bb1871){bb15 bb1755:
bb591->bb2285=bb2211;bb591->bb2253=bb2240;bb591->bb2276=bb2291;bb591
->bb2201=bb2251;bb591->bb2217=bb2254;bb591->bb2282=bb2287;bb591->
bb2239=bb2247;bb21;bb15 bb1769:bb591->bb2285=bb2204;bb591->bb2253=
bb2294;bb591->bb2276=bb2289;bb591->bb2201=bb2243;bb591->bb2217=bb2300
;bb591->bb2282=bb2203;bb591->bb2239=bb2284;bb21;bb416:bb457(bb591);
bb591=bb91;bb18=bb363;bb21;}bbm(((bb18)==bb98))bb18=bb591->bb2285(&
bb591->bb2141);bbm(((bb18)==bb98))bb18=bb591->bb2217(&bb591->bb2212);
bbm(((bb18)==bb98)) *bb359=bb591;bb2 bb18;}bb4 bb1949(bb382 bb359,bbf
 *bb431,bbd bb420,bbf*bb310,bbd bb293,bbd*bb434,bbd*bb304){bb4 bb18=
bb98;bbm(!bb434||!bb304)bb2 bb363; *bb304=0 ; *bb434=0 ;bbm(!bb431||!
bb310)bb2 bb363;bb18=bb359->bb2253(bb359->bb2141,bb431,bb420,bb310,
bb293,bb434,bb304);bb2 bb18;}bb4 bb1942(bb382 bb359,bbf*bb310,bbd
bb293,bbd*bb304,bbu*bb976){bb4 bb18=bb98;bbm(!bb976||!bb304)bb2 bb363
; *bb304=0 ; *bb976=0 ;bbm(!bb310)bb2 bb363;bb18=bb359->bb2276(bb359->
bb2141,bb310,bb293,bb304,bb976);bb2 bb18;}bb4 bb1950(bb382 bb359,bbf*
bb431,bbd bb420,bbf*bb310,bbd bb293,bbd*bb434,bbd*bb304,bbu*bb964){
bb4 bb18=bb98;bbm(!bb434||!bb304||!bb964)bb2 bb363; *bb304=0 ; *bb434=
0 ; *bb964=0 ;bbm(!bb431||!bb310)bb2 bb363;bb18=bb359->bb2282(bb359->
bb2212,bb431,bb420,bb310,bb293,bb434,bb304,bb964);bb2 bb18;}bb4 bb1799
(bb382 bb359){bb4 bb18=bb98;bbm(!bb359)bb2 bb363;bb359->bb2201(&(
bb359->bb2141));bb359->bb2239(&(bb359->bb2212));bb457(bb359);bb2 bb18
;}
