/*
   'src_nf_nf.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Wed Dec 18 14:11:11 2013
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
#include"uncobf.h"
#include<linux/netdevice.h>
#include<linux/netfilter.h>
#include<linux/netfilter_ipv4.h>
#include<linux/skbuff.h>
#include<linux/ip.h>
#include<net/ip.h>
#include<linux/udp.h>
#include<linux/miscdevice.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
#include"cobf.h"
bba bb10{bb402,bb1505,}bb297;bbk bb1207(bb297 bb859,bbh bbf*bb453);
bbd bb542(bb297 bb859,bbh bbf*bb453);bbb bb1166(bbk bb159,bb297 bb550
,bbf bb438[2 ]);bbb bb976(bbd bb159,bb297 bb550,bbf bb438[4 ]);bba bb82
bb4;bb10{bb98=0 ,bb361=-12000 ,bb345=-11999 ,bb377=-11998 ,bb674=-11997 ,
bb722=-11996 ,bb738=-11995 ,bb874=-11994 ,bb854=-11992 ,bb779=-11991 ,
bb804=-11990 ,bb750=-11989 ,bb838=-11988 ,bb639=-11987 ,bb683=-11986 ,
bb763=-11985 ,bb848=-11984 ,bb711=-11983 ,bb622=-11982 ,bb773=-11981 ,
bb895=-11980 ,bb684=-11979 ,bb714=-11978 ,bb844=-11977 ,bb586=-11976 ,
bb897=-11975 ,bb637=-11960 ,bb885=-11959 ,bb876=-11500 ,bb723=-11499 ,
bb670=-11498 ,bb810=-11497 ,bb696=-11496 ,bb755=-11495 ,bb720=-11494 ,
bb783=-11493 ,bb668=-11492 ,bb632=-11491 ,bb635=-11490 ,bb676=-11489 ,
bb680=-11488 ,bb700=-11487 ,bb840=-11486 ,bb889=-11485 ,bb649=-11484 ,
bb900=-11483 ,bb716=-11482 ,bb634=-11481 ,bb708=-11480 ,bb768=-11479 ,
bb648=-11478 ,bb730=-11477 ,bb774=-11476 ,bb769=-11475 ,bb866=-11474 ,
bb775=-11473 ,bb841=-11472 ,bb757=-11460 ,bb843=-11450 ,bb751=-11449 ,
bb717=-11448 ,bb726=-11447 ,bb864=-11446 ,bb629=-11445 ,bb826=-11444 ,
bb734=-11443 ,bb713=-11440 ,bb789=-11439 ,bb791=-11438 ,bb697=-11437 ,
bb707=-11436 ,bb673=-11435 ,bb630=-11420 ,bb531=-11419 ,bb567=-11418 ,
bb693=-11417 ,bb631=-11416 ,bb792=-11415 ,bb808=-11414 ,bb748=-11413 ,
bb633=-11412 ,bb830=-11411 ,bb681=-11410 ,bb781=-11409 ,bb715=-11408 ,
bb905=-11407 ,bb873=-11406 ,bb814=-11405 ,bb894=-11404 ,bb654=-11403 ,
bb770=-11402 ,bb846=-11401 ,bb642=-11400 ,bb879=-11399 ,bb855=-11398 ,
bb659=-11397 ,bb687=-11396 ,bb898=-11395 ,bb705=-11394 ,bb860=-11393 ,
bb811=-11392 ,bb878=-11391 ,bb888=-11390 ,bb743=-11389 ,bb790=-11388 ,
bb813=-11387 ,bb903=-11386 ,bb758=-11385 ,bb706=-11384 ,bb899=-11383 ,
bb645=-11382 ,bb908=-11381 ,bb640=-11380 ,bb795=-11379 ,bb623=-11378 ,
bb760=-11377 ,bb815=-11376 ,bb870=-11375 ,bb788=-11374 ,bb816=-11373 ,
bb688=-11372 ,bb865=-11371 ,bb812=-11370 ,bb620=-11369 ,bb712=-11368 ,
bb765=-11367 ,bb858=-11366 ,bb710=-11365 ,bb636=-11364 ,bb653=-11363 ,
bb387=-11350 ,bb884=bb387,bb721=-11349 ,bb662=-11348 ,bb835=-11347 ,bb646
=-11346 ,bb651=-11345 ,bb877=-11344 ,bb818=-11343 ,bb754=-11342 ,bb824=-
11341 ,bb764=-11340 ,bb902=-11339 ,bb397=-11338 ,bb724=-11337 ,bb701=bb397
,bb819=-11330 ,bb619=-11329 ,bb799=-11328 ,bb801=-11327 ,bb731=-11326 ,
bb762=-11325 ,bb772=-11324 ,bb672=-11320 ,bb638=-11319 ,bb678=-11318 ,
bb752=-11317 ,bb644=-11316 ,bb667=-11315 ,bb831=-11314 ,bb736=-11313 ,
bb776=-11312 ,bb881=-11300 ,bb787=-11299 ,bb749=-11298 ,bb709=-11297 ,
bb704=-11296 ,bb867=-11295 ,bb892=-11294 ,bb656=-11293 ,bb890=-11292 ,
bb906=-11291 ,bb618=-11290 ,bb727=-11289 ,bb809=-11288 ,bb725=-11287 ,
bb785=-11286 ,bb616=-11285 ,bb806=-11284 ,bb850=-11283 ,bb728=-11282 ,
bb679=-11281 ,bb807=-11280 ,bb685=-11279 ,bb753=-11250 ,bb832=-11249 ,
bb663=-11248 ,bb677=-11247 ,bb641=-11246 ,bb650=-11245 ,bb719=-11244 ,
bb761=-11243 ,bb621=-11242 ,bb820=-11240 ,bb655=-11239 ,bb745=-11238 ,
bb825=-11237 ,bb702=-11150 ,bb703=-11100 ,bb836=-11099 ,bb786=-11098 ,
bb871=-11097 ,bb735=-11096 ,bb802=-11095 ,bb643=-11094 ,bb617=-11093 ,
bb834=-11092 ,bb686=-11091 ,bb805=-11090 ,bb628=-11089 ,bb868=-11088 ,
bb661=-11087 ,bb901=-11086 ,bb767=-11085 ,bb793=-11050 ,bb759=-11049 ,
bb695=-10999 ,bb771=-10998 ,bb658=-10997 ,bb669=-10996 ,bb904=-10995 ,
bb893=-10994 ,bb699=-10993 ,bb817=-10992 ,bb780=-10991 ,bb627=-10990 ,
bb626=-10989 ,bb822=-10988 ,bb694=-10979 ,bb660=-10978 ,bb880=-10977 ,
bb875=-10976 ,bb744=-10975 ,bb742=-10974 ,};bba bbi bb449{bb1 bb74;bbd
bb125;bbd bb181;bbi bb449*bb93;}bbx;bb4 bb467(bbx*bb689,bbd bb909,bbx
 *bb837,bbd bb896,bbd bb536);bb4 bb526(bbx*bbj,bbd bb92,bbh bbb*bb95,
bbd bb47);bb4 bb563(bbx*bbj,bbd bb92,bbb*bb130,bbd bb47);bbu bb833(
bbx*bbj,bbd bb92,bbh bbb*bb95,bbd bb47);
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
bb50,bbd bb26);bbb bb985(bbx*bb50);bbu bb1012(bbf*bb50);bba bbi bb991
 *bb1015;bba bbi bb1028*bb1029;bba bbi bb993*bb1023;bba bbi bb1025*
bb1011;bba bbi bb1017*bb1024;bba bbi bb1014*bb987;bba bb10{bb554=0 ,
bb582=1 ,bb580=2 ,bb827=3 ,bb587=4 ,bb561=5 ,bb575=6 ,bb562=7 ,bb579=9 ,}
bb422;bba bb10{bb601=0 ,bb992,bb615,bb1010,bb931,bb923,bb925,bb918,
bb927,bb922,bb916,}bb519;bb4 bb2057(bbx*bb299,bbd*bb104);bb4 bb2116(
bbx*bb85,bbu bb177,bbd bb486,bb519 bb154,bbh bbf*bb1309,bbf*bb131,
bb422 bb412,bbf*bb553,bbd bb104,bbd bb497,bbx*bb58);bb4 bb2044(bbx*
bb85,bbu bb177,bb519 bb154,bbh bbf*bb1309,bb422 bb412,bbf*bb553,bbd*
bb489,bbd*bb459,bbd*bb539,bbx*bb58);bb4 bb2073(bbx*bb299,bbd*bb104);
bb4 bb2088(bbx*bb85,bbu bb177,bbd bb486,bb422 bb412,bbf*bb553,bbd
bb104,bbd bb497,bbx*bb58);bb4 bb2062(bbx*bb85,bbu bb177,bb422 bb412,
bbf*bb553,bbd*bb489,bbd*bb459,bbd*bb539,bbx*bb58);bb10{bb558=1 ,};bbb*
bb499(bbd bb1201,bbd bb374);bb4 bb461(bbb*bb972);bba bbi bb1987 bb1972
, *bb382;bba bb10{bb1994=0 ,bb1754=1 ,bb1768=2 }bb842;bb4 bb1813(bb842
bb1872,bb382*bb360);bb4 bb1949(bb382 bb360,bbf*bb437,bbd bb418,bbf*
bb309,bbd bb293,bbd*bb440,bbd*bb305);bb4 bb1942(bb382 bb360,bbf*bb309
,bbd bb293,bbd*bb305,bbu*bb975);bb4 bb1950(bb382 bb360,bbf*bb437,bbd
bb418,bbf*bb309,bbd bb293,bbd*bb440,bbd*bb305,bbu*bb962);bb4 bb1822(
bb382 bb360);bb4 bb2186(bbx*bb85,bbu bb177,bbd bb486,bb842 bb1365,bbx
 *bb58,bbu*bb2087);bb4 bb2100(bbx*bb85,bbu bb177,bb842 bb1365,bbx*
bb58);bbu bb2133(bbx*bb299);bbu bb2191(bbx*bb299);bb4 bb2061(bbx*
bb299,bbd*bb104);bb4 bb2010(bbx*bb299,bbd*bb104);bb4 bb1846(bbx*bb85,
bbx*bb58,bbu bb1061,bbk bb2119,bbk bb1812);bb4 bb1861(bbx*bb85,bbx*
bb58,bbu bb1061);
#pragma pack(push, 8)
#ifdef _MSC_VER
#pragma warning (disable:4200)
#endif
bba bbf bb178[4 ];bba bb10{bb1650=0 ,bb1470=1 ,}bb1395;bba bb10{bb1532=0
,bb1721=1 ,bb1669=2 ,bb1441=3 ,bb1660=4 ,bb1495=5 ,bb1637=6 ,bb1520=7 ,
bb1610=8 ,bb1524=9 ,bb1683=10 ,bb1513=11 ,bb1697=12 ,bb1595=13 ,bb1722=14 ,
bb1423=15 ,bb1456=16 ,bb1396=17 ,bb1603=18 ,bb1692=19 ,bb1642=20 ,bb1497=21
,bb1511=22 ,bb1477=23 ,bb1608=24 ,bb1612=25 ,bb1452=26 ,bb1584=27 ,bb1377=
28 ,bb1709=29 ,bb1689=30 ,bb1631=16300 ,bb1618=16301 ,bb1728=16384 ,bb1541=
24576 ,bb1467=24577 ,bb1439=24578 ,bb1482=34793 ,bb1382=40500 ,}bb782;bba
bb10{bb1465=0 ,bb1529=1 ,bb1460=2 ,bb1425=3 ,bb1701=4 ,bb1388=5 ,bb1672=6 ,
bb1478=7 ,bb1533=8 ,bb1400=9 ,bb1446=21 ,bb1492=22 ,bb1498=23 ,bb1448=24 ,
bb1545=25 ,bb1514=26 ,bb1466=27 ,bb1384=28 ,bb1486=29 ,bb1494=80 ,}bb657;
bba bb10{bb1640=0 ,bb1699=1 ,bb1696=2 ,bb1488=3 ,bb1523=4 ,}bb1655;bba bb10
{bb1688=0 ,bb1351=1 ,bb1161=2 ,bb1220=3 ,bb1289=4 ,bb1079=61440 ,bb1362=
61441 ,bb1118=61443 ,bb1300=61444 ,}bb479;bba bb10{bb1702=0 ,bb1501=1 ,
bb1565=2 ,}bb1671;bba bb10{bb1390=0 ,bb1727,bb1440,bb1458,bb1568,bb1503
,bb1638,bb1468,bb1528,bb1504,bb1397,bb1694,}bb733;bba bb10{bb1680=0 ,
bb1368=2 ,bb1324=3 ,bb1378=4 ,bb1319=9 ,bb1297=12 ,bb1366=13 ,bb1307=14 ,
bb1352=249 ,}bb624;bba bb10{bb1363=0 ,bb1299=1 ,bb1286=2 ,bb1430=3 ,bb1636
=4 ,bb1360=5 ,bb1327=12 ,bb1317=13 ,bb1370=14 ,bb1287=61440 ,}bb481;bba bb10
{bb1293=1 ,bb1306=2 ,bb1338=3 ,bb1544=4 ,bb1606=5 ,bb1453=6 ,bb1429=7 ,
bb1474=8 ,bb1457=9 ,bb1543=10 ,bb1302=11 ,bb394=12 ,bb1340=13 ,bb392=240 ,
bb1348=(128 <<16 )|bb392,bb1345=(192 <<16 )|bb392,bb1329=(256 <<16 )|bb392,
bb1301=(128 <<16 )|bb394,bb1294=(192 <<16 )|bb394,bb1339=(256 <<16 )|bb394,
}bb729;bba bb10{bb1296=0 ,bb1563=1 ,bb1369=2 ,bb1328=3 ,bb1462=4 ,}bb692;
bba bb10{bb1438=0 ,bb1576=1 ,bb1225=2 ,bb614=3 ,bb1238=4 ,}bb778;bba bb10{
bb1580=0 ,bb1531=1 ,bb1407=2 ,bb1674=5 ,bb1708=7 ,}bb490;bba bb10{bb1427=0
,bb1519=1 ,bb1605=2 ,bb1714=3 ,bb1484=4 ,bb1686=5 ,bb1648=6 ,bb395=7 ,bb1548
=65001 ,bb400=240 ,bb1489=(128 <<16 )|bb400,bb1509=(192 <<16 )|bb400,bb1518
=(256 <<16 )|bb400,bb1547=(128 <<16 )|bb395,bb1560=(192 <<16 )|bb395,bb1617
=(256 <<16 )|bb395,}bb664;bba bb10{bb1720=0 ,bb1461=1 ,bb1663=2 ,bb1575=3 ,
bb1483=4 ,bb1534=5 ,bb1570=6 ,bb1647=65001 ,}bb852;bba bb10{bb1681=0 ,
bb1530=1 ,bb1662=2 ,bb1554=3 ,bb1658=4 ,bb1616=5 ,bb1558=64221 ,bb1634=
64222 ,bb1659=64223 ,bb1677=64224 ,bb1711=65001 ,bb1684=65002 ,bb1556=
65003 ,bb1442=65004 ,bb1724=65005 ,bb1493=65006 ,bb1516=65007 ,bb1481=
65008 ,bb1712=65009 ,bb1479=65010 ,}bb883;bba bb10{bb1700=0 ,bb1420=1 ,
bb1433=2 ,}bb856;bba bb10{bb1410=0 ,bb1372=1 ,bb1633=2 ,bb1687=3 ,}bb798;
bba bb10{bb1594=0 ,bb1421=1 ,bb1437=2 ,bb1649=3 ,bb1602=4 ,bb1641=5 ,bb1508
=21 ,bb1572=6 ,bb1620=7 ,bb1540=8 ,bb1381=1000 ,}bb480;bba bb10{bb1412=0 ,
bb1668=1 ,bb1673=2 ,}bb747;bba bb10{bb1667=0 ,bb1664=1 ,bb1718=2 ,bb1450=3
,bb1475=4 ,}bb857;bba bb10{bb1535=0 ,bb1676=1 ,bb1394=1001 ,bb1713=1002 ,}
bb800;bba bb10{bb1562=0 ,bb1098=1 ,bb1044=2 ,bb1056=3 ,bb1131=4 ,bb1128=5 ,
bb1096=6 ,bb1698=100 ,bb1586=101 ,}bb483;bba bbi bb386{bb729 bb154;bb481
bb585;bb479 bb57;}bb386;bba bbi bb399{bb624 bb1355;bb481 bb585;bb479
bb57;}bb399;bba bbi bb396{bb692 bb1004;}bb396;bba bbi bb482{bb883
bb1629;bb852 bb412;bb664 bb154;bbu bb1490;bb490 bb666;}bb482;bba bbi
bb477{bbu bb608;bb386 bb310;bbu bb777;bb399 bb556;bbu bb732;bb396
bb603;bb490 bb666;}bb477;bba bbi bb455{bb178 bb954;bb178 bb1210;bb778
bb103;bb559{bbi{bb399 bb45;bbf bb555[64 ];bbf bb551[64 ];}bb556;bbi{
bb386 bb45;bbf bb1213[32 ];bbf bb1229[32 ];bbf bb555[64 ];bbf bb551[64 ];
bbf bb1211[16 ];}bb310;bbi{bb396 bb45;}bb603;}bb315;}bb455;bba bbi{bbd
bb853,bb594;bbf bb1143:1 ;bbf bb1156:1 ;bbf bb103;bbk bb433;}bb186;bba
bbi bb512{bbd bb11;bb186 bbc[64 *2 ];}bb512;
#ifdef UNDER_CE
bba bb43 bb379;
#else
bba bb82 bb379;
#endif
bba bbi bb198{bbi bb198*bb1472, *bb1389;bbd bb26;bbd bb1119;bb186
bb912[64 ];bb483 bb507;bbd bb1358;bbk bb1075;bbd bb557;bbd bb691;bbd
bb823;bbf bb474;bbf bb1364;bbf bb1110;bbd bb1002;bbd bb1383;bb379
bb564;bbk bb1281;bb455 bb408[3 ];bb379 bb1571;bbf bb1510[40 ];bbd bb588
;bbd bb1582;}bb198;bba bbi bb390{bbi bb390*bb1726;bb186 bb476;}bb390;
bba bbi bb756{bbu bb473;bbu bb474;bbd bb26;bbd bb588;bbf bb1517;bbk
bb1644;bbf*bb1552;bbd bb1426;bbf*bb1500;bbd bb1717;bbf*bb1379;bbd
bb1414;bbu bb1451;bbu bb1574;bb390*bb130;bbu bb1480;bb857 bb1525;bbd
bb1596;bb856 bb1710;bb483 bb507;bbk bb1374;bbd bb1537;bb800 bb1406;
bbd bb1416;bbd bb1657;bb733 bb1419;bbf*bb1408;bbd bb1415;bb480 bb671;
bbd bb1653;bbd bb1626;bbd bb1411;bbd bb1706;bbd bb1499;bb482*bb1542;
bbd bb1613;bb477*bb1512;bbd bb1399;bbd bb1536;bbd bb1652;}bb756;bba
bbi bb739{bbu bb473;bbd bb26;bb186 bb476;}bb739;bba bbi bb845{bb198*
bb314;bbu bb1573;bbf*bb1703;bbd bb1670;}bb845;bba bbi bb872{bbd bb26;
bb186 bb476;bbf bb1435;bbf bb1449;}bb872;bba bbi bb861{bbu bb473;bbu
bb1124;bbd bb26;bbf*bb1628;bbd bb1546;}bb861;bba bbi bb665{bbd bb26;
bbk bb1729;bbk bb1731;bbd bb151;bbf*bb49;}bb665;bba bbi bb882{bbu
bb1590;bbd bb26;bbd bb557;bbd bb691;bbd bb823;}bb882;bba bbi bb907{
bb782 bb1496;bbd bb26;bb657 bb1320;bbu bb1559;}bb907;bba bbi bb737{
bbf bb1434;bbf bb1392;bbf bb1597;bbf bb1589;bbf bb1578;bbf bb1604;bbf
bb1443;bbf bb1463;bbf bb1375;bbf bb1526;bbf bb1413;bbf bb1705;bbf
bb1730;bbf bb1409;bbf bb1404;bbf bb1444;bbf bb1623;bbf bb1385;bbf
bb1455;bbf bb506;bbf bb1553;bbf bb1622;bbf bb1538;bbf bb1691;bbf
bb1418;bbf bb1432;bbf bb1417;}bb737;bba bbi bb698{bbu bb1646;bbd bb486
;bbd bb1600;bb798 bb1428;bbk bb1635;bbu bb1522;bbu bb1566;bbu bb1654;
bbu bb1454;bbu bb1632;bbu bb1599;bbu bb1401;bbl bb1624[128 ];bbl bb1476
[128 ];bbl bb1601[128 ];bbl bb1422[256 ];bbl bb1639[128 ];bbl bb1447[128 ]
;bbd bb1592;bbf bb1567[8 ];bbf bb1581[8 ];}bb698;bba bbi bb863{bbd bb26
;bbd bb1387;}bb863;bba bbi bb862{bbd bb26;bbu bb474;}bb862;bba bbi
bb891{bbu bb1716;bbd bb503;bbd bb1167;}bb891;bba bbi bb766{bbd bb26;
bb480 bb671;bb747 bb1607;bbf*bb1583;bbd bb1591;}bb766;bba bb10{bb1403
=0 ,bb1557,bb1675,bb1386,bb1436,bb1539,bb1609,bb1393,bb1527,bb1585,
bb1587,bb1693,bb1707,bb1656,bb1402,bb1588,bb1469,bb1405,bb1625,bb1502
,}bb652;bba bbi bb1651 bb794;bba bb4( *bb1555)(bb794*bb1569,bbb*
bb1593,bb652 bb320,bbb*bb74);
#pragma pack(pop)
#ifdef _WIN32
#ifdef UNDER_CE
#define bb466 bb1704 bb600("1:")
#else
#define bb466 bb600("\\\\.\\IPSecTL")
#endif
#else
#define bb606 "ipsecdrvtl"
#define bb466 "/dev/" bb606
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
#include"uncobf.h"
#include<linux/ioctl.h>
#include"cobf.h"
bba bbi{bb1 bb1349;bbd bb1336;bb1 bb1241;bbd bb1147;bbd bb451;}bb1199
;
#define bb1337 1
#endif
#pragma pack(push, 8)
bb10{bb1325=3 ,bb1357,bb1359,bb1424,};bba bbi{bbf bb104[4 ];}bb1239;bba
bbi{bbf bb104[4 ];}bb1237;bba bbi{bbd bb945;bbd bb26;}bb1258;bba bbi{
bbd bb129;bbf bb1215[8 ];}bb405;bba bb10{bb1224=0 ,bb1234,bb1251,bb1231
,bb1373}bb1235;bba bbi{bbf bb1125;bbd bb1072;bbf bb1367;}bb488;
#pragma pack(pop)
#pragma pack(push, 8)
bb10{bb1129=-5000 ,bb1111=-4000 ,bb999=-4999 ,bb1020=-4998 ,bb1031=-4997 ,
bb983=-4996 ,bb1095=-4995 ,bb1088=-4994 ,bb1102=-4993 ,bb1027=-4992 ,
bb1086=-4991 };bb4 bb1134(bb4 bb1137,bbd bb1120,bbl*bb1105);bba bbi{
bb198 bb179;bbd bb1197;bbd bb1087;bbd bb1371;bbd bb1036;bbd bb1242;
bbd bb1269;bbd bb1278;bbd bb1259;bbd bb1249;bbd bb1280;bbd bb1250;bbu
bb1200;bb43 bb564,bb1170,bb1168;bbf bb370[6 ];}bb160;bba bbi bb478{bbi
bb478*bb93;bbf bb103;bbk bb1276;bbk bb1270;bbk bb1273;bbk bb1275;}
bb425;bba bbi bb797{bbi bb797*bb93;bbi bb478*bb1097;bbd bb26;bbf bb370
[6 ];}bb410;bba bb10{bb1094=0 ,bb1577,bb1041,bb1018,bb1019}bb203;bba bbi
{bbd bb385;bbd bb451;bbd bb502;bb405*bb914;bb96 bb979;}bb307;bba bbi{
bb488*bb456;bb410*bb1132;bbd bb581;bb425*bb544;bb96 bb602;bbq bb1123;
bbq bb548;bb160*bb511;bbu bb1268;bbk bb1157;bbk bb1113;bb307 bb1046;}
bb33, *bb1611;
#pragma pack(pop)
bba bbi bb967 bb1315, *bb79;bba bbi bb828{bbi bb828*bb322;bb1 bb471;
bbq bb566;bbd bb26;bbk bb433;bbq bb92;bb1 bb313;bbq bb457;bb1 bb549;
bbq bb546;bb1 bb1506;bb101 bb1326;bbf bb1305[6 ];bb101 bb964;bb101
bb1142;bb101 bb520;bb101 bb533;}bb175, *bb86;bba bbi bb869{bbi bb869*
bb93;bb175*bb322;bbd bb26;bbk bb543;bbk bb1473;bbq bb1445;bbq bb1491;
bbk bb1431;}bb1459, *bb460;bbu bb1266(bb33* *bb1204);bbb bb1284(bb33*
bbj);bb203 bb1267(bb33*bb109,bb378 bb450,bb316 bb138,bb347 bb413,
bb318 bb200);bb203 bb1247(bb33*bb109,bb378 bb450,bb316 bb138,bb347
bb413,bb318 bb200);bb203 bb1256(bb33*bb109,bb175*bb49,bb79 bb75);
bb203 bb1236(bb33*bb109,bb175*bb49,bb79 bb75);bb4 bb1246(bb33*bb109,
bb175*bb49,bbd*bb104);bb4 bb1154(bb79 bb75,bb33*bb109,bb175*bb49,
bb160*bb314,bbu bb592,bbu bb944);bbu bb1868(bbd bb294);bb160*bb1789(
bb33*bbj,bbd bb294,bbu bb592);bb160*bb1844(bb33*bbj,bbd bb294,bbd
bb104);bb160*bb1918(bb33*bbj,bb178 bb104);bbb bb1938(bb512*bb40);
bb160*bb1936(bb33*bbj,bb198*bb179);bbb bb1881(bb33*bbj,bb178 bb104);
bbb bb1859(bb33*bbj,bb178 bb104);bbb bb1980(bb33*bbj);bbb bb1796(bb33
 *bbj);bbb bb1910(bb33*bbj,bbd bb294,bbh bbf bb1186[6 ]);bbu bb1922(
bb33*bbj,bbd bb294,bb410*bb430);bbb bb2036(bb33*bbj);bbb bb2004(bb33*
bbj,bbd bb294,bbh bbf bb1186[6 ],bbf bb103,bbk bb409,bbk bb411);bbu
bb2013(bb33*bbj,bbd bb294,bbf bb103,bbk bb409,bbk bb411);bbu bb1853(
bb33*bbj,bbf bb103,bbk bb409,bbk bb411);bbb bb1986(bb33*bbj,bb425*
bb544,bbq bb581);bbu bb1870(bb307*bbj,bbq bb502);bbb bb1848(bb307*bbj
);bbb bb1960(bb307*bbj);bbu bb1790(bb307*bbj,bb405*bb625);bbu bb1930(
bb307*bbj,bb405*bb625);bbb bb1933(bb33*bbj,bb178 bb104);bbb bb1869(
bb33*bbj,bb178 bb104);bbb bb1817(bb33*bbj,bbd bb26,bbd bb945);bb4
bb1778(bb33*bbj,bb488*bb456);bbb bb2031(bb33*bbj);
#ifdef UNDER_CE
#define bb1915 64
#endif
bba bbi bb1887{bb121 bb1908;bb121 bb1919;bb33*bb974;}bb1045, *bb1889;
bbr bb1045 bb949;bbi bb967{bb121 bb1882;bbq bb1892;bbd bb1952;bb86
bb1001;bb86 bb1934;bb86 bb1857;bb86 bb1891;bb86 bb1935;bb460 bb1856;
bb460 bb1951;bb460 bb1903;bb96 bb1122;bb101 bb1904;bb101 bb1946;bb101
bb1925;bb121 bb1948;bb121 bb1871;};bbr bb79 bb1956;bbr bb96 bb1913;
bbd bb1863(bbb*bb521,bbb*bb1879,bb161*bb1136);bb161 bb1943(bb121
bb1955,bb121 bb1907,bb77 bb549,bbq bb546,bb77 bb1117,bbq bb1109,bbq
bb1148);
#ifdef UNDER_CE
#define bb591 16
#define bb1144 32
#else
#define bb591 128
#define bb1144 256
#endif
#define bb1112 bb591  *2
#define bb568 ( bb1112  *  2)
#define bb1901 bb568  *  2
#define bb1860 bb568  *  2
bbr bbq bb946;bb161 bb1782(bb60 bb982,bbb*bb40,bbq bb1092,bb122 bb1685
);bb141 bb1921(IN bb79 bb75,IN bb121 bb1916,IN bb1 bb549,IN bbq bb546
,IN bb77 bb1117,IN bbq bb1109,IN bbq bb1148);bb141 bb1905(IN bb79 bb75
);bbd bb1899(bb77 bb521,bb121 bb1917,bb77 bb1909,bbq bb1958,bb77
bb1854,bbq bb1850,bbq bb1911,bb161*bb1136);bbb bb1232(bb79 bb75,bb86*
bb534,bb86 bb49);bb86 bb1264(bb79 bb75,bb86*bb534);bbu bb1779(bb79
bb75);bbb bb1793(bb79 bb75);bb86 bb1471(bb173 bb359,bb79 bb75);bb86
bb1827(bb173 bb359,bb79 bb75);bb86 bb1772(bb173 bb359,bb79 bb75);
bb141 bb1666(bb79 bb75,bb86 bb49);bb141 bb1798(bb79 bb75,bb86 bb49);
bb141 bb1845(bb79 bb75,bb86 bb49);bbb bb1867();bbb bb1878();bbb bb156
(bbh bbl*bb20,...);bbb bb1999(bb188 bbg);bbb bb2035(bbb*bb28,bbq bb11
);bbb bb1962(bbb*bb5,bbq bb9);bbb bb1809(bbb*bb5,bbq bb9);bbb bb1330(
bbb*bb5,bbq bb9);bbb bb1896();bbb bb1747();bbb bb1977(bb363*bb1920);
bbb bb1579(bb325*bb28);bbb bb1263(bb325*bb796,bb485*bb150);bbb bb1487
(bb325*bb796,bb414*bb1614);bbi bb2362{bbd bb1288;bbd bb92;bbd bb464;
bbe bb374;bbe bb543;bbf*bb40;bbi bb2362*bb93;};bbi bb2533{bbd bb412;
bbd bb464;bbd bb2618;bbi bb2533*bb2617;bbi bb2362*bb2616;bbf*bb40;};
#ifdef _WIN32
bbe bb2549(bbi bb1897*bb301);
#else
bbe bb2549(bbi bb2066*bb470);
#endif
bb2572("");bb2621("\x61\x68\x6f\x70\x65");bb1045 bb949={0 };bb41 bb1315
bb521={0 };bb41 bb6 bb2480(bbi bb25*bb20,bbs bbe bbo,bbs bb6 bb2517){
bbd bb982;bb1199 bb97, *bb2387=(bb1199* )bb2517;bbf*bb40=bb91;bbe bb27
;
#ifdef _DEBUG
bb156("\x6e\x66\x2e\x63\x2c\x20\x62\x65\x67\x69\x6e\x20\x64\x65\x76"
"\x5f\x69\x6f\x63\x74\x6c\x5f\n");
#endif
bb27=bb2543(&bb97,bb2387,bb12(bb1199));bbm(bb27!=0 ){bb590("\x64\x65"
"\x76\x5f\x69\x6f\x63\x74\x6c\x5f\x2c\x20\x63\x6f\x70\x79\x5f\x66\x72"
"\x6f\x6d\x5f\x75\x73\x65\x72\x2c\x20\x25\x64\n",bb27);bb100 bb1398;}
bbm(bb97.bb1147>2048 ){bb590("\x6d\x2e\x6f\x75\x74\x53\x69\x7a\x65\x20"
"\x3e\x20\x32\x30\x34\x38\x2c\x20\x6d\x2e\x6f\x75\x74\x53\x69\x7a\x65"
"\x3a\x20\x25\x64\n",bb97.bb1147);bb27=-bb2202;bb100 bb1398;}bbm(bb97
.bb1336!=4 ){bb590("\x6d\x2e\x69\x6e\x53\x69\x7a\x65\x20\x21\x3d\x20"
"\x34\n");bb27=-bb2202;bb100 bb1398;}bbm(bbo!=bb1337){bb590("\x63\x20"
"\x21\x3d\x20\x49\x4f\x43\x54\x4c\x5f\x56\x50\x4e\n");bb27=-bb2202;
bb100 bb1398;}bb40=bb128(bb97.bb1147);bbm(bb40==bb91){bb590("\x62\x75"
"\x66\x20\x3d\x3d\x20\x4e\x55\x4c\x4c\n");bb27=-bb2262;bb100 bb1398;}
bb27=bb2633(bb982,(bb29)bb97.bb1349);bbm(bb27!=0 ){bb590("\x64\x65\x76"
"\x5f\x69\x6f\x63\x74\x6c\x5f\x2c\x20\x67\x65\x74\x5f\x75\x73\x65\x72"
"\x2c\x20\x25\x64\n",bb27);bb100 bb1398;}bb27=bb2543(bb40,bb97.bb1241
,bb97.bb1147);bbm(bb27!=0 ){bb590("\x64\x65\x76\x5f\x69\x6f\x63\x74"
"\x6c\x5f\x2c\x20\x63\x6f\x70\x79\x5f\x66\x72\x6f\x6d\x5f\x75\x73\x65"
"\x72\x2c\x20\x25\x64\n",bb27);bb100 bb1398;}bb134(&bb521.bb1122);
#ifdef _DEBUG
bbm(bb982==0xFF010212 ){bb156("\x62\x65\x66\x6f\x72\x65\x2c\x20\x4f"
"\x49\x44\x5f\x50\x47\x50\x5f\x53\x48\x55\x54\x44\x4f\x57\x4e");
bb1747();}
#endif
bbm(bb1782(bb982,bb40,bb97.bb1147,&bb97.bb451)!=0 ){bb132(&bb521.
bb1122);bb27=-bb2202;bb100 bb1398;}
#ifdef _DEBUG
bbm(bb982==0xFF010205 )bb1896();bbm(bb982==0xFF010207 )bb1747();bbm(
bb982==0xFF010212 ){bb156("\x61\x66\x74\x65\x72\x2c\x20\x4f\x49\x44"
"\x5f\x50\x47\x50\x5f\x53\x48\x55\x54\x44\x4f\x57\x4e");bb1747();}
#endif
bb132(&bb521.bb1122);bb27=bb2627(bb97.bb1241,bb40,bb97.bb451);bbm(
bb27!=0 ){bb590("\x64\x65\x76\x5f\x69\x6f\x63\x74\x6c\x5f\x2c\x20\x63"
"\x6f\x70\x79\x5f\x74\x6f\x5f\x75\x73\x65\x72\x2c\x20\x25\x64\n",bb27
);bb100 bb1398;}bb27=bb2632(bb97.bb451,&bb2387->bb451);bbm(bb27!=0 ){
bb590("\x64\x65\x76\x5f\x69\x6f\x63\x74\x6c\x5f\x2c\x20\x70\x75\x74"
"\x5f\x75\x73\x65\x72\x2c\x20\x25\x64\n",bb27);bb100 bb1398;}bb1398:
bbm(bb40)bb108(bb40);
#ifdef _DEBUG
bb156("\x6e\x66\x2e\x63\x2c\x20\x65\x6e\x64\x20\x64\x65\x76\x5f\x69"
"\x6f\x63\x74\x6c\x5f\n");
#endif
bb2 bb27;}bb41 bbe bb2521(bbi bb2233*bb2233,bbi bb25*bb20){bb2 0 ;}
bb41 bbe bb2525(bbi bb2233*bb2233,bbi bb25*bb20){bb2 0 ;}bb41 bbi
bb2596 bb2438={.bb2594=bb2480,.bb2631=bb2521,.bb2610=bb2525};bb41 bbi
bb2587 bb2337={.bb2619=217 ,.bb35=bb606,.bb2639=&bb2438,};bbs bbl*
bb2400(bbi bb2066*bb470,bbs bbe bb21){bbs bbl*bb2547=bb2575(bb470);
bb470->bb2465+=bb21;bb470->bb21+=bb21;bbm(bb2578(bb470->bb2465>bb470
->bb443))bb156("\x73\x6b\x62\x5f\x70\x75\x74\x2c\x20\x74\x61\x69\x6c"
"\x20\x3e\x20\x65\x6e\x64\n");bb2 bb2547;}bba bbi{bb60 bb2275;bb60
bb1812;bb60 bb2306;}bb2552;bb41 bb2552 bb1829;bb41 bbq bb2454(bbq
bb2260,bbi bb2066*bb470,bbh bbi bb2123*bb2539,bbh bbi bb2123*bb438,
bbe( *bb2477)(bbi bb2066* )){bbq bb515=bb2350;bb203 bb1159;bbi bb1897
 *bb301;bb363 bb593={{0 },{0 },0 };bb325 bb428;bb347 bb1257=bb91;bb318
bb973=bb91;bb414 bb1245;bb485 bb1221;bb175*bb416=bb91;bb593.bb375=
bb960;bb301=(bbi bb1897* )bb2071(bb470);bbm(!bb301){bb515=bb1272;
bb100 bb164;}bb81(&bb428,bb301,bb12(bb428));bbm(bb428.bb253==2 ){bb515
=bb1272;bb100 bb164;}bbm(bb428.bb253==6 ){bbm(!(bb301+bb301->bb1288*4 )){
bb515=bb1272;bb100 bb164;}bb81(&bb1221,(bb1)bb301+bb301->bb1288*4 ,
bb12(bb1221));bb973=&bb1221;
#ifdef _DEBUG
bb156("\x69\x6e\x63\x6f\x6d\x69\x6e\x67\x2c\x20");bb1263(&bb428,bb973
);
#endif
}bb54 bbm(bb428.bb253==17 ){bbm(!(bb301+bb301->bb1288*4 )){bb515=bb1272
;bb100 bb164;}bb81(&bb1245,(bb1)bb301+bb301->bb1288*4 ,bb12(bb1245));
bb1257=&bb1245;
#ifdef _DEBUG
bbm(1 ){bb156("\x69\x6e\x63\x6f\x6d\x69\x6e\x67\x2c\x20");bb1487(&
bb428,bb1257);}bb54{bbm(bb1245.bb290!=4500 &&bb1245.bb290!=bb53(4500 )){
bb156("\x6c\x6f\x63\x61\x6c\x20\x69\x6e\x2c\x20\x75\x64\x70\n");}}
#endif
}bb1159=bb1247(bb949.bb974,&bb593,&bb428,bb1257,bb973);
#ifdef _DEBUG
bb156("\x70\x6d\x73\x3a\x25\x64\x2c\x20\x69\x6e\x63\x6f\x6d\x69\x6e"
"\x67\x20\x69\x70\x2c\x20",bb1159);bb1579(&bb428);
#endif
bbm(bb1159==bb1019){bb515=bb1272;bb100 bb164;}bbm(bb1159!=bb1018)bb100
bb164;{bbq bb372;bb161 bbg;bbe bb1179;bb416=bb1471(&bbg,&bb521);bbm(!
bb416&&bb416->bb471&&bb416->bb313){bb156("\x63\x61\x6e\x27\x74\x20"
"\x61\x6c\x6c\x6f\x63\x20\x70\x61\x63\x6b\x65\x74\n");bb100 bb164;}
bb372=bb194(bb301->bb2516);bb81(bb416->bb471,&bb593,bb12(bb593));bb81
(bb416->bb471+bb12(bb593),bb301,bb372);bb81(bb416->bb313,&bb593,bb12(
bb593));bb416->bb566=bb12(bb593)+bb372;bb416->bb964=1 ;bb416->bb26=
bb493(bb301->bb2581);bb416->bb433=bb1257?bb1257->bb423:0 ;bb1159=
bb1236(bb949.bb974,bb416,&bb521);bbm(bb1159!=bb1094)bb100 bb164;
#ifdef _DEBUG
{bbi bb1897*bb1840;bb325 bb1957;bb347 bb1245=bb91;bb318 bb1221=bb91;
bb414 bb2257;bb485 bb2196;bb1840=(bbi bb1897* )(bb416->bb313+bb12(
bb593));bb81(&bb1957,bb1840,bb12(bb1957));bb156("\x69\x6e\x63\x6f\x6d"
"\x69\x6e\x67\x20\x78\x66\x6f\x72\x6d\x42\x6c\x6f\x63\x6b\x2c\x20");
bb1579(&bb1957);bbm(bb1957.bb253==6 ){bb81(&bb2196,(bb1)bb1840+bb1840
->bb1288*4 ,bb12(bb2196));bb1221=&bb2196;bb156("\x69\x6e\x63\x6f\x6d"
"\x69\x6e\x67\x20\x78\x66\x6f\x72\x6d\x42\x6c\x6f\x63\x6b\x20\x74\x63"
"\x70\x2c\x20");bb1263(&bb1957,bb1221);}bb54 bbm(bb1957.bb253==17 ){
bbm(!(bb301+bb301->bb1288*4 )){bb515=bb1272;bb100 bb164;}bb81(&bb2257,
(bb1)bb1840+bb1840->bb1288*4 ,bb12(bb2257));bb1245=&bb2257;bb156("\x69"
"\x6e\x63\x6f\x6d\x69\x6e\x67\x20\x78\x66\x6f\x72\x6d\x42\x6c\x6f\x63"
"\x6b\x20\x75\x64\x70\x2c\x20");bb1487(&bb1957,bb1245);}}
#endif
bb1179=bb416->bb457-bb416->bb566;bb372=bb416->bb457-bb12(bb593);bbm(
bb1179>0 ){bb32(bb2538(bb470)==0 );bbm(bb2540(bb470,0 ,bb1179,bb140)!=0 ){
bb156("\x63\x61\x6e\x27\x74\x20\x65\x78\x70\x61\x6e\x64\x20\x73\x6b"
"\x62\n");bb100 bb164;}bb2400(bb470,bb1179);}bb54{bbe bb2054=bb470->
bb21+bb1179;bbm(bb1179<0 )bb2447(bb470,bb2054);bbm(!bb2500(bb470,
bb2054)){bb156("\x63\x61\x6e\x27\x74\x20\x6d\x61\x6b\x65\x20\x73\x6b"
"\x62\x20\x77\x72\x69\x74\x61\x62\x6c\x65\n");bb100 bb164;}}bb81(
bb2071(bb470),bb416->bb313+bb12(bb593),bb372);bb301=(bbi bb1897* )bb2071
(bb470);bb81(&bb428,bb301,bb12(bb428));bbm(bb428.bb253==6 ){bb81(&
bb1221,(bb1)bb301+bb301->bb1288*4 ,bb12(bb1221));bb973=&bb1221;bb1829.
bb2275=bb428.bb311;bb1829.bb1812=bb973->bb578;bb1829.bb2306=bb973->
bb919;}bbm(bb1069(bb2071(bb470)->bb2593)){bb156("\x66\x72\x61\x67\x6d"
"\x65\x6e\x74\n");bb2602(bb470);bb2583(bb470);bb2577(bb470);bb515=
bb2579;bb100 bb164;}}bb515=bb1272;bb164:bbm(bb416)bb1666(&bb521,bb416
);bb2 bb515;}bb41 bbq bb2527(bbq bb2260,bbi bb2066*bb470,bbh bbi
bb2123*bb2539,bbh bbi bb2123*bb438,bbe( *bb2477)(bbi bb2066* )){bbq
bb515=bb2350;bb203 bb1159;bbi bb2123*bb2148;bbi bb1897*bb301;bb363
bb593={{0 },{0 },0 };bb325 bb428;bb347 bb1257=bb91;bb318 bb973=bb91;
bb414 bb1245;bb485 bb1221;bb175*bb416=bb91;bb593.bb375=bb960;bb301=(
bbi bb1897* )bb2071(bb470);bbm(!bb301){bb515=bb1272;bb100 bb164;}bb81
(&bb428,bb301,bb12(bb428));bb2148=bb2622(bb470)->bb2148;bbm(!bb2148||
bb2595(bb2148->bb35,"\x77\x6c\x61\x6e\x30")!=0 ){bb515=bb1272;bb100
bb164;}bbm(bb1868(bb301->bb2214)){bb2 bb2350;}bbm(bb428.bb253==2 ){
bb515=bb1272;bb100 bb164;}bbm(bb428.bb253==6 ){bbe bb2355=bb301->
bb1288*4 ;bb318 bb2177=(bb318)((bb1)bb301+bb2355);bb81(&bb1221,bb2177,
bb12(bb1221));bb973=&bb1221;
#ifdef _DEBUG
bb156("\x6f\x75\x74\x67\x6f\x69\x6e\x67\x2c\x20");bb1263(&bb428,bb973
);
#endif
{bb33*bb109=bb949.bb974;bbm(bb109->bb548>0 &&bb301->bb2214==bb109->
bb511[0 ].bb179.bb26&&bb973->bb540==bb1829.bb2306&&bb973->bb290==
bb1829.bb1812){bbq bb2139=bb194(bb428.bb372)-bb2355;bbf*bb40=bb128(
bb12(bb610)+bb2139);bb571 bb552=(bb571)bb40;bb1829.bb2306=0 ;bb552->
bb311=bb428.bb311;bb552->bb204=bb1829.bb2275;bb552->bb917=0 ;bb552->
bb253=6 ;bb552->bb913=bb53(bb2139);bb301->bb2214=bb1829.bb2275;bb973->
bb319=0 ;bb81(&bb2177->bb319,&bb973->bb319,2 );bb81(bb40+bb12(bb610),
bb2177,bb2139);bb973->bb319=bb849(bb40,bb12(bb610)+bb2139);bb81(&
bb2177->bb319,&bb973->bb319,2 );
#ifdef _DEBUG
bb156("\x6e\x65\x77\x20\x63\x68\x65\x63\x6b\x73\x75\x6d\x3a\x25\x30"
"\x34\x78\n",bb53(bb973->bb319));
#endif
bb108(bb40);bb2576(bb301);bb81(&bb428,bb301,bb12(bb428));}}}bb54 bbm(
bb428.bb253==17 ){bbm(!(bb301+bb301->bb1288*4 )){bb515=bb1272;bb100
bb164;}bb81(&bb1245,(bb1)bb301+bb301->bb1288*4 ,bb12(bb1245));bb1257=&
bb1245;
#ifdef _DEBUG
bb156("\x6f\x75\x74\x67\x6f\x69\x6e\x67\x2c\x20\n");bb1487(&bb428,
bb1257);
#endif
}bb1159=bb1267(bb949.bb974,&bb593,&bb428,bb1257,bb973);
#ifdef _DEBUG
bb156("\x70\x6d\x73\x3a\x25\x64\x2c\x20\x6f\x75\x74\x67\x6f\x69\x6e"
"\x67\x20\x69\x70\x2c\x20",bb1159);bb1579(&bb428);
#endif
bbm(bb1159==bb1019){bb515=bb1272;bb100 bb164;}bbm(bb1159!=bb1018)bb100
bb164;{bbq bb372;bb161 bbg;bbe bb1179;bb416=bb1471(&bbg,&bb521);bbm(!
bb416&&bb416->bb471&&bb416->bb313){bb156("\x63\x61\x6e\x27\x74\x20"
"\x61\x6c\x6c\x6f\x63\x20\x70\x61\x63\x6b\x65\x74\n");bb100 bb164;}
bb372=bb194(bb301->bb2516);bb81(bb416->bb471,&bb593,bb12(bb593));bb81
(bb416->bb471+bb12(bb593),bb301,bb372);bb81(bb416->bb313,&bb593,bb12(
bb593));
#ifdef _DEBUG
bbm(bb428.bb253==6 ){bb156("\x6f\x75\x74\x67\x6f\x69\x6e\x67\x20\x73"
"\x72\x63\x42\x6c\x6f\x63\x6b\x20\x74\x63\x70\x2c\x20");bb1263(&bb428
,bb973);}bb54 bbm(bb428.bb253==17 ){bb156("\x6f\x75\x74\x67\x6f\x69"
"\x6e\x67\x20\x73\x72\x63\x42\x6c\x6f\x63\x6b\x20\x75\x64\x70\x2c\x20"
);bb1487(&bb428,bb1257);}
#endif
bb416->bb566=bb12(bb593)+bb372;bb416->bb964=1 ;bb416->bb26=bb493(bb301
->bb2214);bb416->bb433=bb1257?bb1257->bb423:0 ;bb1159=bb1256(bb949.
bb974,bb416,&bb521);bb156("\x50\x4d\x44\x6f\x54\x72\x61\x6e\x73\x66"
"\x6f\x72\x6d\x4f\x75\x74\x67\x6f\x69\x6e\x67\x2c\x20\x70\x6d\x73\x3a"
"\x25\x64\n",bb1159);bbm(bb1159!=bb1094)bb100 bb164;
#ifdef _DEBUG
{bb325 bb428;bb81(&bb428,bb416->bb313+bb12(bb593),bb12(bb428));bb156(""
"\x6f\x75\x74\x67\x6f\x69\x6e\x67\x20\x78\x66\x6f\x72\x6d\x42\x6c\x6f"
"\x63\x6b\x2c\x20");bb1579(&bb428);}
#endif
bb1179=bb416->bb457-bb416->bb566;bb372=bb416->bb457-bb12(bb593);bbm(
bb1179>0 ){bb32(bb2538(bb470)==0 );bbm(bb2540(bb470,0 ,bb1179,bb140)!=0 ){
bb156("\x63\x61\x6e\x27\x74\x20\x65\x78\x70\x61\x6e\x64\x20\x73\x6b"
"\x62\n");bb100 bb164;}bb2400(bb470,bb1179);}bb54{bbe bb2054=bb470->
bb21+bb1179;bbm(bb1179<0 )bb2447(bb470,bb2054);bbm(!bb2500(bb470,
bb2054)){bb156("\x63\x61\x6e\x27\x74\x20\x6d\x61\x6b\x65\x20\x73\x6b"
"\x62\x20\x77\x72\x69\x74\x61\x62\x6c\x65\n");bb100 bb164;}}bb81(
bb2071(bb470),bb416->bb313+bb12(bb593),bb372);
#ifdef _DEBUG
bb156("\x73\x6b\x62\x2d\x3e\x69\x70\x5f\x73\x75\x6d\x6d\x65\x64\x3a"
"\x25\x64\n",bb470->bb2561);
#endif
bb470->bb2561=bb2573;bb2624(bb470,bb2615);}bb515=bb1272;bb164:bbm(
bb416)bb1666(&bb521,bb416);bb2 bb515;}bb41 bbi bb2628 bb2436={.bb2457
=bb2454,.bb2522=bb2455,.bb2260=bb2589,.bb2553=bb2526},bb2411={.bb2457
=bb2527,.bb2522=bb2455,.bb2260=bb2570,.bb2553=bb2526};bb41 bbe bb2453
(bbl*bb1052,bbl* *bb2153,bbl* *bb2208,bbe bb2354){bbi bb2585*bb2213;
bb2626 bb2505=(bb2354==bb2604)?bb140:bb2623;
#ifdef _DEBUG
bbl* *bb28=bb2153;bb590("\x63\x61\x6c\x6c\x5f\x75\x73\x65\x72\x6d\x6f"
"\x64\x65\x68\x65\x6c\x70\x65\x72\x5f\x2c\x20\x25\x73",bb1052);bb106(
 *bb28){bb590("\x20\x25\x73", *bb28);bb28++;}bb590("\n");
#endif
bb2213=bb2638(bb1052,bb2153,bb2208,bb2505);bbm(bb2213==bb91){bb590(""
"\x63\x61\x6c\x6c\x5f\x75\x73\x65\x72\x6d\x6f\x64\x65\x68\x65\x6c\x70"
"\x65\x72\x5f\x2c\x20\x69\x6e\x66\x6f\x20\x3d\x3d\x20\x4e\x55\x4c\x4c"
"\n");bb2-bb2262;}bb2 bb2647(bb2213,bb2354);}bbe bb2641(){bbe bb27;
#ifdef _DEBUG
bb1867();
#endif
bb590("\x49\x2d\x57\x4c\x41\x4e\x2f\x69\x70\x73\x65\x63\x64\x72\x76"
"\x74\x6c\x2c\x20\x6d\x77\x6c\x61\x6e\x20\x6b\x6f\x20\x73\x72\x63\x2c"
"\x20\x32\x30\x31\x33\x2e\x31\x32\x2e\x31\x38\n");bb156("\x69\x70\x73"
"\x65\x63\x64\x72\x76\x74\x6c\x2c\x20\x62\x65\x67\x69\x6e\x20\x69\x6e"
"\x69\x74\x5f\x6d\x6f\x64\x75\x6c\x65\n");bb139(&bb521.bb1122);bbm(!
bb1266(&bb949.bb974)){bb590("\x63\x61\x6e\x27\x74\x20\x69\x6e\x69\x74"
"\x20\x70\x6d\n");bb2-bb2262;}{bb488 bbo={0 };bbo.bb1125=1 ;bb1778(
bb949.bb974,&bbo);}bb946=1480 ;bbm(!bb1779(&bb521)){bb590("\x63\x61"
"\x6e\x27\x74\x20\x61\x6c\x6c\x6f\x63\x20\x70\x6b\x74\x20\x70\x6f\x6f"
"\x6c\n");bb2-bb2262;}bb27=bb2592(&bb2337);bbm(bb27!=0 ){bb590("\x63"
"\x61\x6e\x27\x74\x20\x72\x65\x67\x20\x6d\x69\x73\x63\x20\x64\x65\x76"
"\x2c\x20\x20\x25\x64\n",bb27);bb2 bb27;}bb156("\x73\x75\x63\x63\x65"
"\x65\x64\x20\x69\x6e\x20\x72\x65\x67\x69\x73\x74\x65\x72\x69\x6e\x67"
"\x20\x6d\x69\x73\x63\n");bb27=bb2450(&bb2436);bbm(bb27!=0 ){bb590(""
"\x63\x61\x6e\x27\x74\x20\x72\x65\x67\x20\x68\x6f\x6f\x6b\x20\x69\x6e"
"\x2c\x20\x25\x64\n",bb27);bb2 bb27;}bb156("\x73\x75\x63\x63\x65\x65"
"\x64\x20\x69\x6e\x20\x72\x65\x67\x69\x73\x74\x65\x72\x69\x6e\x67\x20"
"\x74\x68\x65\x20\x68\x6f\x6f\x6b\x5f\x69\x6e\n");bb27=bb2450(&bb2411
);bbm(bb27!=0 ){bb590("\x63\x61\x6e\x27\x74\x20\x72\x65\x67\x20\x68"
"\x6f\x6f\x6b\x20\x6f\x75\x74\x2c\x20\x25\x64\n",bb27);bb2 bb27;}
bb156("\x73\x75\x63\x63\x65\x65\x64\x20\x69\x6e\x20\x72\x65\x67\x69"
"\x73\x74\x65\x72\x69\x6e\x67\x20\x74\x68\x65\x20\x68\x6f\x6f\x6b\x5f"
"\x6f\x75\x74\n");{bbl*bb2153[]={"\x2f\x73\x79\x73\x74\x65\x6d\x2f"
"\x62\x69\x6e\x2f\x63\x68\x6f\x77\x6e","\x73\x79\x73\x74\x65\x6d\x2e"
"\x73\x79\x73\x74\x65\x6d",bb466,bb91};bb41 bbl*bb2208[]={"\x48\x4f"
"\x4d\x45\x3d\x2f","\x54\x45\x52\x4d\x3d\x6c\x69\x6e\x75\x78","\x50"
"\x41\x54\x48\x3d\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e",bb91};
bbe bb27=bb2453("\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e\x2f\x74"
"\x6f\x6f\x6c\x62\x6f\x78",bb2153,bb2208,bb2591);bbm(bb27!=0 )bb590(""
"\x69\x6e\x69\x74\x5f\x6d\x6f\x64\x75\x6c\x65\x2c\x20\x63\x61\x6c\x6c"
"\x5f\x75\x73\x65\x72\x6d\x6f\x64\x65\x68\x65\x6c\x70\x65\x72\x5f\x20"
"\x66\x61\x69\x6c\x65\x64\x3a\x20\x25\x64\n",bb27);}bb156("\x69\x70"
"\x73\x65\x63\x64\x72\x76\x74\x6c\x2c\x20\x65\x6e\x64\x20\x69\x6e\x69"
"\x74\x5f\x6d\x6f\x64\x75\x6c\x65\n");bb2 0 ;}bbb bb2642(){bb156("\x69"
"\x70\x73\x65\x63\x64\x72\x76\x74\x6c\x2c\x20\x62\x65\x67\x69\x6e\x20"
"\x63\x6c\x65\x61\x6e\x75\x70\x5f\x6d\x6f\x64\x75\x6c\x65\n");bb2484(
&bb2436);bb2484(&bb2411);bb2613(&bb2337);bb1793(&bb521);bb1284(bb949.
bb974);bb156("\x69\x70\x73\x65\x63\x64\x72\x76\x74\x6c\x2c\x20\x65"
"\x6e\x64\x20\x63\x6c\x65\x61\x6e\x75\x70\x5f\x6d\x6f\x64\x75\x6c\x65"
"\n");
#ifdef _DEBUG
bb1878();
#endif
}
