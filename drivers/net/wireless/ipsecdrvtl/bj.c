/*
   'src_ipsec_pgpIPsecBuffer.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Wed Dec 18 14:11:11 2013
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
10977 ,bb875=-10976 ,bb744=-10975 ,bb742=-10974 ,};bba bbi bb449{bb1 bb74
;bbd bb125;bbd bb181;bbi bb449*bb93;}bbx;bb4 bb467(bbx*bb689,bbd bb909
,bbx*bb837,bbd bb896,bbd bb536);bb4 bb526(bbx*bbj,bbd bb92,bbh bbb*
bb95,bbd bb47);bb4 bb563(bbx*bbj,bbd bb92,bbb*bb130,bbd bb47);bbu
bb833(bbx*bbj,bbd bb92,bbh bbb*bb95,bbd bb47);bb4 bb467(bbx*bb689,bbd
bb909,bbx*bb837,bbd bb896,bbd bb536){bbd bb168=0 ;bbd bb146=0 ;bbd bb157
=0 ;bbd bb570=0 ;bbd bb1855;bbd bb1923;bbx*bb73=bb91;bbx*bb46=bb91;bb4
bb18=bb98;bbm(!bb689||!bb837)bb2 bb361;bbm(!bb689->bb74||!bb837->bb74
)bb2 bb361;bb73=bb689;bb46=bb837;bb146=bb896;bb157=bb73->bb125;bb106(
bb157<bb909&&bb73){bb73=bb73->bb93;bb570=bb157;bb157+=bb73->bb125;}
bb168=bb909-bb570;bb570=0 ;bb157=bb46->bb181;bb106(bb157<bb896&&((bb18
)==bb98)){bbm(!bb46->bb93)bb18=bb377;bb54{bb46=bb46->bb93;bb570=bb157
;bb157+=bb46->bb181;}}bb146=bb896-bb570;bbm(&(bb73->bb74[bb168])==&(
bb46->bb74[bb146]))bb2 bb18;bb106(bb536>0 &&bb73&&((bb18)==bb98)){
bb1855=bb73->bb125-bb168;bb1923=bb46->bb181-bb146;bbm(bb1855>bb536)bb1855
=bb536;bbm(bb1923>bb536)bb1923=bb536;bbm(bb1855<=bb1923){bb81(&bb46->
bb74[bb146],&bb73->bb74[bb168],bb1855);bb73=bb73->bb93;bb168=0 ;bb146
+=bb1855;bb536-=bb1855;bbm(bb46->bb125<bb146)bb46->bb125=bb146;}bb54{
bbm(!bb46->bb93)bb18=bb377;bb54{bb81(&bb46->bb74[bb146],&bb73->bb74[
bb168],bb1923);bb46->bb125=bb46->bb181;bb46=bb46->bb93;bb146=0 ;bb168
+=bb1923;bb536-=bb1923;}}}bbm(((bb18)==bb98)&&(bb536>0 ))bb18=bb377;
bb2 bb18;}bb4 bb526(bbx*bbj,bbd bb92,bbh bbb*bb2511,bbd bb47){bbx bb95
;bb95.bb74=(bb1)bb2511;bb95.bb125=bb47;bb95.bb181=bb47;bb95.bb93=bb91
;bb2 bb467(&bb95,0 ,bbj,bb92,bb47);}bb4 bb563(bbx*bbj,bbd bb92,bbb*
bb2449,bbd bb47){bbx bb130;bb130.bb74=bb2449;bb130.bb125=0 ;bb130.
bb181=bb47;bb130.bb93=bb91;bb2 bb467(bbj,bb92,&bb130,0 ,bb47);}bbu
bb833(bbx*bbj,bbd bb92,bbh bbb*bb95,bbd bb47){bbd bb168=0 ;bbd bb146=0
;bbd bb157=0 ;bbd bb570=0 ;bbd bb1890;bbd bb1888;bbd bb2548=bb47;bbx*
bb73=bb91;bbm(!bbj)bb2 0 ;bbm(!bbj->bb74||!bb95)bb2 0 ;bb73=bbj;bb157=
bb73->bb125;bb106(bb157<bb92&&bb73){bb73=bb73->bb93;bb570=bb157;bb157
+=bb73->bb125;}bb168=bb92-bb570;bbm(&bb73->bb74[bb168]==bb95)bb2 1 ;
bb106(bb47&&bb73){bb1890=bb73->bb125-bb168;bb1888=bb2548-bb146;bbm(
bb1890>bb47)bb1890=bb47;bbm(bb1888>bb47)bb1888=bb47;bbm(bb1890<=
bb1888){bbm(bb1898(&bb73->bb74[bb168],(bb1)bb95+bb146,bb1890)!=0 )bb2 0
;bb73=bb73->bb93;bb168=0 ;bb146+=bb1890;bb47-=bb1890;}bb54{bbm(bb1898(
&bb73->bb74[bb168],(bb1)bb95+bb146,bb1888)!=0 )bb2 0 ;bb168+=bb1888;
bb47-=bb1888;bb22;}}bb2!bb47;}
