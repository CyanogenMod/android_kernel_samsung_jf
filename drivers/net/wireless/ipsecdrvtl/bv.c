/*
   'src_pm_pgpNetPMEvents.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Mon Oct 29 11:54:27 2012
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
bb984(bbx*bb50);bbu bb1017(bbf*bb50);bba bbi bb1015*bb1022;bba bbi
bb1029*bb1013;bba bbi bb995*bb1025;bba bbi bb1001*bb1014;bba bbi
bb1016*bb1026;bba bbi bb992*bb1024;bba bb9{bb556=0 ,bb582=1 ,bb581=2 ,
bb701=3 ,bb588=4 ,bb560=5 ,bb575=6 ,bb564=7 ,bb579=9 ,}bb422;bba bb9{bb601=
0 ,bb993,bb599,bb1012,bb929,bb931,bb926,bb919,bb927,bb930,bb918,}bb515
;
#pragma pack(push, 8)
#ifdef _MSC_VER
#pragma warning (disable:4200)
#endif
bba bbf bb178[4 ];bba bb9{bb1651=0 ,bb1466=1 ,}bb1394;bba bb9{bb1528=0 ,
bb1717=1 ,bb1667=2 ,bb1437=3 ,bb1658=4 ,bb1499=5 ,bb1633=6 ,bb1517=7 ,bb1607
=8 ,bb1520=9 ,bb1678=10 ,bb1510=11 ,bb1692=12 ,bb1709=13 ,bb1718=14 ,bb1423=
15 ,bb1452=16 ,bb1396=17 ,bb1600=18 ,bb1689=19 ,bb1640=20 ,bb1495=21 ,bb1508
=22 ,bb1473=23 ,bb1603=24 ,bb1606=25 ,bb1449=26 ,bb1539=27 ,bb1375=28 ,
bb1703=29 ,bb1687=30 ,bb1630=16300 ,bb1614=16301 ,bb1726=16384 ,bb1536=
24576 ,bb1464=24577 ,bb1436=24578 ,bb1478=34793 ,bb1383=40500 ,}bb626;bba
bb9{bb1461=0 ,bb1524=1 ,bb1456=2 ,bb1426=3 ,bb1696=4 ,bb1389=5 ,bb1487=6 ,
bb1474=7 ,bb1529=8 ,bb1402=9 ,bb1443=21 ,bb1491=22 ,bb1492=23 ,bb1445=24 ,
bb1542=25 ,bb1511=26 ,bb1462=27 ,bb1385=28 ,bb1480=29 ,bb1493=80 ,}bb767;
bba bb9{bb1563=0 ,bb1694=1 ,bb1554=2 ,bb1486=3 ,bb1522=4 ,}bb1655;bba bb9{
bb1686=0 ,bb1353=1 ,bb1164=2 ,bb1216=3 ,bb1289=4 ,bb1033=61440 ,bb1364=
61441 ,bb1117=61443 ,bb1299=61444 ,}bb474;bba bb9{bb1697=0 ,bb1498=1 ,
bb1561=2 ,}bb1731;bba bb9{bb1395=0 ,bb1724,bb1438,bb1453,bb1565,bb1500,
bb1634,bb1465,bb1604,bb1496,bb1399,bb1588,}bb728;bba bb9{bb1676=0 ,
bb1367=2 ,bb1331=3 ,bb1378=4 ,bb1324=9 ,bb1296=12 ,bb1322=13 ,bb1310=14 ,
bb1354=249 ,}bb620;bba bb9{bb1329=0 ,bb1298=1 ,bb1345=2 ,bb1424=3 ,bb1635=
4 ,bb1363=5 ,bb1346=12 ,bb1323=13 ,bb1316=14 ,bb1286=61440 ,}bb485;bba bb9{
bb1292=1 ,bb1307=2 ,bb1312=3 ,bb1541=4 ,bb1602=5 ,bb1448=6 ,bb1429=7 ,bb1470
=8 ,bb1454=9 ,bb1540=10 ,bb1303=11 ,bb391=12 ,bb1343=13 ,bb398=240 ,bb1350=(
128 <<16 )|bb398,bb1347=(192 <<16 )|bb398,bb1333=(256 <<16 )|bb398,bb1302=(
128 <<16 )|bb391,bb1294=(192 <<16 )|bb391,bb1368=(256 <<16 )|bb391,}bb821;
bba bb9{bb1295=0 ,bb1505=1 ,bb1370=2 ,bb1332=3 ,bb1458=4 ,}bb882;bba bb9{
bb1434=0 ,bb1572=1 ,bb1220=2 ,bb604=3 ,bb1238=4 ,}bb790;bba bb9{bb1576=0 ,
bb1527=1 ,bb1408=2 ,bb1671=5 ,bb1705=7 ,}bb476;bba bb9{bb1427=0 ,bb1516=1 ,
bb1599=2 ,bb1710=3 ,bb1484=4 ,bb1666=5 ,bb1646=6 ,bb397=7 ,bb1545=65001 ,
bb387=240 ,bb1488=(128 <<16 )|bb387,bb1506=(192 <<16 )|bb387,bb1514=(256 <<
16 )|bb387,bb1544=(128 <<16 )|bb397,bb1558=(192 <<16 )|bb397,bb1615=(256 <<
16 )|bb397,}bb792;bba bb9{bb1716=0 ,bb1457=1 ,bb1618=2 ,bb1571=3 ,bb1475=4
,bb1530=5 ,bb1566=6 ,bb1644=65001 ,}bb885;bba bb9{bb1685=0 ,bb1525=1 ,
bb1659=2 ,bb1553=3 ,bb1654=4 ,bb1613=5 ,bb1556=64221 ,bb1620=64222 ,bb1657=
64223 ,bb1673=64224 ,bb1706=65001 ,bb1681=65002 ,bb1551=65003 ,bb1440=
65004 ,bb1720=65005 ,bb1490=65006 ,bb1513=65007 ,bb1477=65008 ,bb1707=
65009 ,bb1476=65010 ,}bb889;bba bb9{bb1695=0 ,bb1418=1 ,bb1432=2 ,}bb896;
bba bb9{bb1411=0 ,bb1372=1 ,bb1479=2 ,bb1683=3 ,}bb855;bba bb9{bb1589=0 ,
bb1420=1 ,bb1433=2 ,bb1647=3 ,bb1642=4 ,bb1639=5 ,bb1504=21 ,bb1568=6 ,
bb1617=7 ,bb1537=8 ,bb1382=1000 ,}bb487;bba bb9{bb1412=0 ,bb1665=1 ,bb1668
=2 ,}bb737;bba bb9{bb1664=0 ,bb1400=1 ,bb1715=2 ,bb1435=3 ,bb1471=4 ,}bb669
;bba bb9{bb1531=0 ,bb1674=1 ,bb1679=1001 ,bb1714=1002 ,}bb817;bba bb9{
bb1560=0 ,bb1139=1 ,bb1079=2 ,bb1056=3 ,bb1132=4 ,bb1130=5 ,bb1095=6 ,bb1693
=100 ,bb1579=101 ,}bb486;bba bbi bb400{bb821 bb154;bb485 bb589;bb474
bb57;}bb400;bba bbi bb395{bb620 bb1356;bb485 bb589;bb474 bb57;}bb395;
bba bbi bb390{bb882 bb1004;}bb390;bba bbi bb479{bb889 bb1628;bb885
bb413;bb792 bb154;bbu bb1489;bb476 bb642;}bb479;bba bbi bb480{bbu
bb613;bb400 bb311;bbu bb631;bb395 bb559;bbu bb710;bb390 bb608;bb476
bb642;}bb480;bba bbi bb455{bb178 bb955;bb178 bb1210;bb790 bb102;bb550
{bbi{bb395 bb45;bbf bb554[64 ];bbf bb555[64 ];}bb559;bbi{bb400 bb45;bbf
bb1217[32 ];bbf bb1229[32 ];bbf bb554[64 ];bbf bb555[64 ];bbf bb1200[16 ];
}bb311;bbi{bb390 bb45;}bb608;}bb315;}bb455;bba bbi{bbd bb847,bb594;
bbf bb1144:1 ;bbf bb1160:1 ;bbf bb102;bbk bb437;}bb184;bba bbi bb510{
bbd bb11;bb184 bbc[64 *2 ];}bb510;
#ifdef UNDER_CE
bba bb43 bb380;
#else
bba bb83 bb380;
#endif
bba bbi bb198{bbi bb198*bb1468, *bb1390;bbd bb26;bbd bb1118;bb184
bb917[64 ];bb486 bb503;bbd bb1293;bbk bb1067;bbd bb557;bbd bb708;bbd
bb802;bbf bb488;bbf bb1339;bbf bb1107;bbd bb1031;bbd bb1384;bb380
bb566;bbk bb1280;bb455 bb404[3 ];bb380 bb1567;bbf bb1507[40 ];bbd bb593
;bbd bb1608;}bb198;bba bbi bb392{bbi bb392*bb1723;bb184 bb481;}bb392;
bba bbi bb734{bbu bb475;bbu bb488;bbd bb26;bbd bb593;bbf bb1515;bbk
bb1594;bbf*bb1546;bbd bb1425;bbf*bb1583;bbd bb1713;bbf*bb1380;bbd
bb1414;bbu bb1645;bbu bb1569;bb392*bb129;bbu bb1472;bb669 bb1521;bbd
bb1593;bb896 bb1704;bb486 bb503;bbk bb1373;bbd bb1534;bb817 bb1406;
bbd bb1649;bbd bb1623;bb728 bb1419;bbf*bb1407;bbd bb1415;bb487 bb662;
bbd bb1624;bbd bb1625;bbd bb1410;bbd bb1699;bbd bb1497;bb479*bb1538;
bbd bb1612;bb480*bb1509;bbd bb1398;bbd bb1532;bbd bb1650;}bb734;bba
bbi bb680{bbu bb475;bbd bb26;bb184 bb481;}bb680;bba bbi bb633{bb198*
bb316;bbu bb1570;bbf*bb1698;bbd bb1669;}bb633;bba bbi bb687{bbd bb26;
bb184 bb481;bbf bb1460;bbf bb1447;}bb687;bba bbi bb849{bbu bb475;bbu
bb1122;bbd bb26;bbf*bb1626;bbd bb1543;}bb849;bba bbi bb655{bbd bb26;
bbk bb1728;bbk bb1729;bbd bb151;bbf*bb49;}bb655;bba bbi bb806{bbu
bb1586;bbd bb26;bbd bb557;bbd bb708;bbd bb802;}bb806;bba bbi bb908{
bb626 bb1494;bbd bb26;bb767 bb1326;bbu bb1557;}bb908;bba bbi bb897{
bbf bb1481;bbf bb1391;bbf bb1591;bbf bb1585;bbf bb1575;bbf bb1605;bbf
bb1582;bbf bb1459;bbf bb1377;bbf bb1523;bbf bb1413;bbf bb1701;bbf
bb1725;bbf bb1409;bbf bb1677;bbf bb1439;bbf bb1621;bbf bb1386;bbf
bb1450;bbf bb513;bbf bb1550;bbf bb1661;bbf bb1533;bbf bb1688;bbf
bb1417;bbf bb1431;bbf bb1416;}bb897;bba bbi bb731{bbu bb1637;bbd bb483
;bbd bb1577;bb855 bb1428;bbk bb1632;bbu bb1519;bbu bb1562;bbu bb1653;
bbu bb1451;bbu bb1631;bbu bb1595;bbu bb1401;bbl bb1652[128 ];bbl bb1670
[128 ];bbl bb1596[128 ];bbl bb1421[256 ];bbl bb1636[128 ];bbl bb1444[128 ]
;bbd bb1590;bbf bb1564[8 ];bbf bb1405[8 ];}bb731;bba bbi bb652{bbd bb26
;bbd bb1388;}bb652;bba bbi bb809{bbd bb26;bbu bb488;}bb809;bba bbi
bb735{bbu bb1712;bbd bb504;bbd bb1172;}bb735;bba bbi bb745{bbd bb26;
bb487 bb662;bb737 bb1601;bbf*bb1578;bbd bb1587;}bb745;bba bb9{bb1403=
0 ,bb1555,bb1672,bb1387,bb1446,bb1535,bb1643,bb1393,bb1526,bb1580,
bb1581,bb1691,bb1702,bb1376,bb1660,bb1584,bb1463,bb1404,bb1622,bb1708
,}bb643;bba bbi bb1648 bb621;bba bb4( *bb1552)(bb621*bb1379,bbb*
bb1727,bb643 bb322,bbb*bb74);
#pragma pack(pop)
#ifdef _WIN32
#ifdef UNDER_CE
#define bb466 bb1700 bb598("1:")
#else
#define bb466 bb598("\\\\.\\IPSecTL")
#endif
#else
#define bb602 "ipsecdrvtl"
#define bb466 "/dev/" bb602
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
#include"uncobf.h"
#include<linux/ioctl.h>
#include"cobf.h"
bba bbi{bb1 bb1351;bbd bb1342;bb1 bb1242;bbd bb1146;bbd bb447;}bb1199
;
#define bb1348 1
#endif
#pragma pack(push, 8)
bb9{bb1362=3 ,bb1360,bb1361,bb1422,};bba bbi{bbf bb104[4 ];}bb1283;bba
bbi{bbf bb104[4 ];}bb1237;bba bbi{bbd bb947;bbd bb26;}bb1270;bba bbi{
bbd bb131;bbf bb1226[8 ];}bb411;bba bb9{bb1221=0 ,bb1233,bb1251,bb1264,
bb1730}bb1235;bba bbi{bbf bb1123;bbd bb1078;bbf bb1366;}bb489;
#pragma pack(pop)
#pragma pack(push, 8)
bb9{bb1131=-5000 ,bb1108=-4000 ,bb1000=-4999 ,bb989=-4998 ,bb1028=-4997 ,
bb982=-4996 ,bb1094=-4995 ,bb1085=-4994 ,bb1099=-4993 ,bb1009=-4992 ,
bb1032=-4991 };bb4 bb1134(bb4 bb1138,bbd bb1119,bbl*bb1102);bba bbi{
bb198 bb179;bbd bb1196;bbd bb1084;bbd bb1371;bbd bb1037;bbd bb1241;
bbd bb1269;bbd bb1275;bbd bb1240;bbd bb1250;bbd bb1281;bbd bb1277;bbu
bb1223;bb43 bb566,bb1158,bb1169;bbf bb369[6 ];}bb161;bba bbi bb484{bbi
bb484*bb93;bbf bb102;bbk bb1259;bbk bb1276;bbk bb1271;bbk bb1273;}
bb423;bba bbi bb777{bbi bb777*bb93;bbi bb484*bb1096;bbd bb26;bbf bb369
[6 ];}bb408;bba bb9{bb1147=0 ,bb1573,bb1045,bb1002,bb1010}bb204;bba bbi
{bbd bb383;bbd bb447;bbd bb505;bb411*bb915;bb96 bb967;}bb307;bba bbi{
bb489*bb462;bb408*bb1125;bbd bb580;bb423*bb544;bb96 bb609;bbq bb1121;
bbq bb539;bb161*bb509;bbu bb1234;bbk bb1157;bbk bb1111;bb307 bb1049;}
bb33, *bb1609;
#pragma pack(pop)
bba bbi bb969 bb1338, *bb78;bba bbi bb826{bbi bb826*bb319;bb1 bb468;
bbq bb561;bbd bb26;bbk bb437;bbq bb92;bb1 bb312;bbq bb456;bb1 bb535;
bbq bb545;bb1 bb1502;bb103 bb1335;bbf bb1306[6 ];bb103 bb965;bb103
bb1143;bb103 bb521;bb103 bb527;}bb175, *bb86;bba bbi bb754{bbi bb754*
bb93;bb175*bb319;bbd bb26;bbk bb542;bbk bb1469;bbq bb1442;bbq bb1485;
bbk bb1430;}bb1455, *bb461;bbu bb1267(bb33* *bb1211);bbb bb1285(bb33*
bbj);bb204 bb1268(bb33*bb109,bb376 bb446,bb317 bb138,bb333 bb414,
bb313 bb200);bb204 bb1247(bb33*bb109,bb376 bb446,bb317 bb138,bb333
bb414,bb313 bb200);bb204 bb1257(bb33*bb109,bb175*bb49,bb78 bb75);
bb204 bb1236(bb33*bb109,bb175*bb49,bb78 bb75);bb4 bb1245(bb33*bb109,
bb175*bb49,bbd*bb104);bb4 bb1154(bb78 bb75,bb33*bb109,bb175*bb49,
bb161*bb316,bbu bb595,bbu bb946);bbu bb1870(bb307*bbj,bbq bb505);bbb
bb1848(bb307*bbj);bbb bb1960(bb307*bbj);bbu bb1790(bb307*bbj,bb411*
bb820);bbu bb1930(bb307*bbj,bb411*bb820);bbb bb1933(bb33*bbj,bb178
bb104);bbb bb1868(bb33*bbj,bb178 bb104);bbb bb1818(bb33*bbj,bbd bb26,
bbd bb947);
#ifdef UNDER_CE
#define bb1915 64
#endif
bba bbi bb1887{bb121 bb1909;bb121 bb1919;bb33*bb974;}bb1047, *bb1890;
bbr bb1047 bb934;bbi bb969{bb121 bb1882;bbq bb1892;bbd bb1952;bb86
bb996;bb86 bb1935;bb86 bb1858;bb86 bb1891;bb86 bb1938;bb461 bb1857;
bb461 bb1951;bb461 bb1902;bb96 bb1124;bb103 bb1903;bb103 bb1946;bb103
bb1925;bb121 bb1948;bb121 bb1869;};bbr bb78 bb1956;bbr bb96 bb1913;
bbd bb1863(bbb*bb518,bbb*bb1879,bb160*bb1120);bb160 bb1943(bb121
bb1955,bb121 bb1907,bb77 bb535,bbq bb545,bb77 bb1116,bbq bb1106,bbq
bb1128);
#ifdef UNDER_CE
#define bb584 16
#define bb1104 32
#else
#define bb584 128
#define bb1104 256
#endif
#define bb1109 bb584  *2
#define bb572 ( bb1109  *  2)
#define bb1901 bb572  *  2
#define bb1860 bb572  *  2
bbr bbq bb953;bb160 bb1782(bb60 bb957,bbb*bb40,bbq bb1090,bb122 bb1682
);bb140 bb1921(IN bb78 bb75,IN bb121 bb1916,IN bb1 bb535,IN bbq bb545
,IN bb77 bb1116,IN bbq bb1106,IN bbq bb1128);bb140 bb1904(IN bb78 bb75
);bbd bb1899(bb77 bb518,bb121 bb1917,bb77 bb1908,bbq bb1959,bb77
bb1854,bbq bb1851,bbq bb1910,bb160*bb1120);bbb bb1231(bb78 bb75,bb86*
bb537,bb86 bb49);bb86 bb1265(bb78 bb75,bb86*bb537);bbu bb1781(bb78
bb75);bbb bb1793(bb78 bb75);bb86 bb1467(bb173 bb360,bb78 bb75);bb86
bb1827(bb173 bb360,bb78 bb75);bb86 bb1774(bb173 bb360,bb78 bb75);
bb140 bb1663(bb78 bb75,bb86 bb49);bb140 bb1798(bb78 bb75,bb86 bb49);
bb140 bb1845(bb78 bb75,bb86 bb49);bbb bb1878();bbb bb1877();bbb bb156
(bbh bbl*bb19,...);bbb bb2000(bb191 bbg);bbb bb2035(bbb*bb28,bbq bb11
);bbb bb1964(bbb*bb5,bbq bb10);bbb bb1810(bbb*bb5,bbq bb10);bbb bb1334
(bbb*bb5,bbq bb10);bbb bb1897();bbb bb1748();bbb bb1977(bb365*bb1920);
bbb bb1574(bb325*bb28);bbb bb1263(bb325*bb657,bb490*bb150);bbb bb1483
(bb325*bb657,bb412*bb1611);bbu bb1870(bb307*bbj,bbq bb505){bb32(bbj);
bbj->bb915=bb137(bb505,bb12(bbj->bb915[0 ]));bbm(!bbj->bb915)bb2 0 ;bbj
->bb505=bb505;bb139(&bbj->bb967);bb2 1 ;}bbb bb1848(bb307*bbj){bb32(
bbj);bbm(bbj->bb915){bb108(bbj->bb915);bbj->bb915=bb91;}bbj->bb505=0 ;
bb142(&bbj->bb967);}bbb bb1960(bb307*bbj){bb32(bbj);bb133(&bbj->bb967
);bbj->bb383=0 ;bbj->bb447=0 ;bb132(&bbj->bb967);}bbu bb1790(bb307*bbj,
bb411*bb820){bbu bb27=0 ;bb32(bb820&&bbj);bbm(bbj->bb915&&bb820){bb133
(&bbj->bb967);{bbd bb505=bbj->bb505;bbm(bbj->bb447+1 >bb505){bbj->
bb383=(bbj->bb383+1 )%bb505;bbj->bb447--;}bb81(bbj->bb915+(bbj->bb383+
bbj->bb447++)%bb505,bb820,bb12( *bb820));bb27=1 ;}bb132(&bbj->bb967);}
bb2 bb27;}bbu bb1930(bb307*bbj,bb411*bb820){bbu bb27=0 ;bb32(bb820);
bbm(bbj&&bbj->bb915&&bb820){bb133(&bbj->bb967);bbm(bbj->bb447){bbd
bb505=bbj->bb505;bb81(bb820,bbj->bb915+bbj->bb383,bb12( *bb820));bbj
->bb383=(bbj->bb383+1 )%bb505;bbj->bb447--;bb27=1 ;}bb132(&bbj->bb967);
}bb2 bb27;}bbb bb1933(bb33*bbj,bb178 bb104){bb411 bb95={0 };bb1283*
bb28=(bb1283* )bb95.bb1226;bb95.bb131=bb1360;bb81(bb28->bb104,bb104,
bb12(bb178));bb1790(&bbj->bb1049,&bb95);}bbb bb1868(bb33*bbj,bb178
bb104){bb411 bb95={0 };bb1237*bb28=(bb1237* )bb95.bb1226;bb95.bb131=
bb1362;bb81(bb28->bb104,bb104,bb12(bb178));bb1790(&bbj->bb1049,&bb95);
}bbb bb1818(bb33*bbj,bbd bb26,bbd bb947){bb411 bb95={0 };bb1270*bb28=(
bb1270* )bb95.bb1226;bb95.bb131=bb1361;bb28->bb26=bb26;bb28->bb947=
bb947;bb1790(&bbj->bb1049,&bb95);}
