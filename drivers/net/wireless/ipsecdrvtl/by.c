/*
   'src_pm_pgpNetPolicyManager.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Mon Oct 29 11:54:27 2012
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
bb161*bb316,bbu bb595,bbu bb946);bb4 bb1778(bb33*bbj,bb489*bb462);bbb
bb2030(bb33*bbj);bbb bb1911(bb33*bbj,bbd bb294,bbh bbf bb1185[6 ]);bbu
bb1922(bb33*bbj,bbd bb294,bb408*bb425);bbb bb2036(bb33*bbj);bbb bb2004
(bb33*bbj,bbd bb294,bbh bbf bb1185[6 ],bbf bb102,bbk bb409,bbk bb410);
bbu bb2012(bb33*bbj,bbd bb294,bbf bb102,bbk bb409,bbk bb410);bbu
bb1853(bb33*bbj,bbf bb102,bbk bb409,bbk bb410);bbb bb1987(bb33*bbj,
bb423*bb544,bbq bb580);bbu bb1867(bbd bb294);bb161*bb1789(bb33*bbj,
bbd bb294,bbu bb595);bb161*bb1844(bb33*bbj,bbd bb294,bbd bb104);bb161
 *bb1918(bb33*bbj,bb178 bb104);bbb bb1937(bb510*bb40);bb161*bb1934(
bb33*bbj,bb198*bb179);bbb bb1881(bb33*bbj,bb178 bb104);bbb bb1859(
bb33*bbj,bb178 bb104);bbb bb1980(bb33*bbj);bbb bb1796(bb33*bbj);bbu
bb1870(bb307*bbj,bbq bb505);bbb bb1848(bb307*bbj);bbb bb1960(bb307*
bbj);bbu bb1790(bb307*bbj,bb411*bb820);bbu bb1930(bb307*bbj,bb411*
bb820);bbb bb1933(bb33*bbj,bb178 bb104);bbb bb1868(bb33*bbj,bb178
bb104);bbb bb1818(bb33*bbj,bbd bb26,bbd bb947);bb4 bb2057(bbx*bb306,
bbd*bb104);bb4 bb2116(bbx*bb85,bbu bb177,bbd bb483,bb515 bb154,bbh bbf
 *bb1311,bbf*bb130,bb422 bb413,bbf*bb558,bbd bb104,bbd bb498,bbx*bb58
);bb4 bb2044(bbx*bb85,bbu bb177,bb515 bb154,bbh bbf*bb1311,bb422 bb413
,bbf*bb558,bbd*bb482,bbd*bb459,bbd*bb534,bbx*bb58);bb4 bb2073(bbx*
bb306,bbd*bb104);bb4 bb2088(bbx*bb85,bbu bb177,bbd bb483,bb422 bb413,
bbf*bb558,bbd bb104,bbd bb498,bbx*bb58);bb4 bb2063(bbx*bb85,bbu bb177
,bb422 bb413,bbf*bb558,bbd*bb482,bbd*bb459,bbd*bb534,bbx*bb58);bbu
bb2133(bbx*bb306);bbu bb2191(bbx*bb306);bb4 bb2061(bbx*bb306,bbd*
bb104);bb4 bb2010(bbx*bb306,bbd*bb104);bb4 bb1846(bbx*bb85,bbx*bb58,
bbu bb1091,bbk bb2119,bbk bb1813);bb4 bb1861(bbx*bb85,bbx*bb58,bbu
bb1091);bbb bb1878();bbb bb1877();bbb bb156(bbh bbl*bb19,...);bbb
bb2000(bb191 bbg);bbb bb2035(bbb*bb28,bbq bb11);bbb bb1964(bbb*bb5,
bbq bb10);bbb bb1810(bbb*bb5,bbq bb10);bbb bb1334(bbb*bb5,bbq bb10);
bbb bb1897();bbb bb1748();bbb bb1977(bb365*bb1920);bbb bb1574(bb325*
bb28);bbb bb1263(bb325*bb657,bb490*bb150);bbb bb1483(bb325*bb657,
bb412*bb1611);
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
bb140 bb1845(bb78 bb75,bb86 bb49);bbe bb2419(bb317 bb138,bbd bb1088);
bbe bb2315(bb317 bb138);bb41 bbf bb2236[8192 ];bb204 bb1268(bb33*bb109
,bb376 bb446,bb317 bb138,bb333 bb414,bb313 bb200){bb1235 bb123;bbk
bb1783;bbf bb1732;bbk bb409,bb410;bbm(!bb109->bb462)bb2 bb1010;bb123=
bb109->bb462->bb1078;bbm(bb109->bb462->bb1123){bb161*bb166=bb1789(
bb109,bb138->bb203,0 );bbm(bb166)bb2 bb1002;}bb1783=bb138->bb867>>3 ;
bb1732=1 <<(bb138->bb867&0x07 );bbm(!bb970(bb138->bb577)){bbm(bb2236[
bb1783]&bb1732)bb99 bb1330;bb54 bb99 bb2056;}bbm(bb414){bb409=bb194(
bb414->bb226);bb410=bb194(bb414->bb427);bbm(bb410==0x01f4 ||bb410==
0x1194 ){bb1911(bb109,bb138->bb203,bb446->bb369);bb99 bb1330;}}bb54 bbm
(bb200){bb409=bb194(bb200->bb226);bb410=bb194(bb200->bb583);}bb54{
bb409=bb410=0 ;}bbm(bb123==bb1221)bb99 bb1330;bbm(bb123==bb1233)bb99
bb2056;bbm(bb123==bb1251){bb1911(bb109,bb138->bb203,bb446->bb369);}
bb54 bbm(bb123==bb1264){bbm(bb1853(bb109,bb138->bb254,bb409,bb410))bb99
bb1330;bb2004(bb109,bb138->bb203,bb446->bb369,bb138->bb254,bb409,
bb410);}bb1330:bb2236[bb1783]|=bb1732;bb2 bb1010;bb2056:bb2236[bb1783
]&=~bb1732;bb2 bb1045;}bb41 bbf bb2297[8192 ];bb204 bb1247(bb33*bb109,
bb376 bb446,bb317 bb138,bb333 bb414,bb313 bb200){bb1235 bb123;bbk
bb1783;bbf bb1732;bbk bb409,bb410;(bbb)bb446;bbm(!bb109->bb462)bb2
bb1010;bb123=bb109->bb462->bb1078;bbm(bb109->bb462->bb1123){bb161*
bb316=bb1789(bb109,bb138->bb309,1 );bbm(bb316){bbm(bb138->bb254==50 ||
bb138->bb254==51 ||bb138->bb254==108 )bb2 bb1002;bbm(bb414){bbu bb950;
bb455*bb373;bb373=&bb316->bb179.bb404[0 ];bb300(bb316->bb179.bb503){
bb15 bb1139:bb950=1 ;bb21;bb15 bb1095:bb950=bb373->bb102==bb604&&bb373
->bb315.bb311.bb45.bb57==bb1117;bb21;bb15 bb1130:bb950=bb373->bb102==
bb604&&bb373->bb315.bb311.bb45.bb57==bb1216;bb21;bb15 bb1132:bb950=
bb373->bb102==bb604&&(bb373->bb315.bb311.bb45.bb57==bb1117||bb373->
bb315.bb311.bb45.bb57==bb1033);bb21;bb15 bb1079:bb15 bb1056:bb950=
bb373->bb102==bb604&&bb373->bb315.bb311.bb45.bb57==bb1033;bb21;bb416:
bb950=0 ;bb21;}bbm(bb950){bbk bb2124=0 ;bb300(bb316->bb179.bb503){bb15
bb1095:bb15 bb1130:bb2124=bb109->bb1111;bb21;bb15 bb1139:bb15 bb1132:
bb2124=bb109->bb1157;bb21;bb15 bb1079:bb15 bb1056:bb2124=bb53(bb316->
bb179.bb1067);}bbm(bb414->bb427==bb2124)bb2 bb1002;}}}}bb1783=bb138->
bb867>>3 ;bb1732=1 <<(bb138->bb867&0x07 );bbm(!bb970(bb138->bb577)){bbm(
bb2297[bb1783]&bb1732)bb99 bb1330;bb54 bb99 bb2056;}bbm(bb414){bb409=
bb194(bb414->bb226);bb410=bb194(bb414->bb427);bbm(bb409==0x01f4 ||
bb409==0x1194 )bb99 bb1330;}bb54 bbm(bb200){bb409=bb194(bb200->bb226);
bb410=bb194(bb200->bb583);}bb54{bb409=bb410=0 ;}bbm(bb123==bb1221)bb99
bb1330;bbm(bb123==bb1233)bb99 bb2056;bbm(bb123==bb1251){bbm(bb1922(
bb109,bb138->bb309,bb91))bb99 bb1330;}bb54 bbm(bb123==bb1264){bbm(
bb1853(bb109,bb138->bb254,bb410,bb409))bb99 bb1330;bbm(bb2012(bb109,
bb138->bb309,bb138->bb254,bb410,bb409))bb99 bb1330;}bb2056:bb2297[
bb1783]&=!bb1732;bb2 bb1045;bb1330:bb2297[bb1783]|=bb1732;bb2 bb1010;
}bb41 bbu bb2324(bb33*bb2265,bb161*bb166){bbm(bb166->bb179.bb1031&&
bb166->bb1037>=bb166->bb179.bb1031*1024 ||bb166->bb1084>=0xfffffffe ){
bb1933(bb2265,bb166->bb179.bb404[0 ].bb955);bb2 1 ;}bb2 0 ;}bb41 bbb
bb2436(bb33*bb2265,bb161*bb166){bbu bb2106=0 ;bbd bb2104=1 ;bbm(!bb166)bb2
;bbm(bb166->bb1223)bb2;bbm(bb166->bb1084>=0xFFFF7FFF ){bb2106=1 ;bb99
bb2168;}bbm(bb166->bb179.bb1031==0 ||bb166->bb1037<1024 )bb2;bb2104=
bb166->bb1037/1024 ;{bbd bb2426=(bbd)(bb301(bb91)-bb166->bb566);bbd
bb2405=bb2426?bb2104/bb2426:0 ;bbm(bb2405&&(bb166->bb179.bb1031-bb2104
)/bb2405<=30 ){bb2106=1 ;bb99 bb2168;}}bbm(bb166->bb179.bb1031>2048 ){
bbm(bb2104>=bb166->bb179.bb1031-1024 ){bb2106=1 ;bb99 bb2168;}}bb54 bbm
(bb2104>=bb166->bb179.bb1031){bb2106=1 ;bb99 bb2168;}bb2168:bbm(bb2106
){bb1868(bb2265,bb166->bb179.bb404[0 ].bb955);bb166->bb1223=1 ;}}bb41
bb204 bb2494(bb33*bb109,bbd bb26,bbk bb437,bb161* *bb2351,bb161* *
bb2330){bb161*bb166;bb455*bb373;(bbb)bb437; *bb2351=bb91; *bb2330=
bb91;bb166=bb1789(bb109,bb26,0 );bbm(!bb166)bb2 bb1045;bb373=&bb166->
bb179.bb404[0 ];bbm((bb373->bb102==bb1220&&bb373->bb315.bb559.bb45.
bb57==bb1164)||(bb373->bb102==bb604&&bb373->bb315.bb311.bb45.bb57==
bb1164)) *bb2351=bb166;bb54*bb2330=bb166;bb2 bb1002;}bb41 bbb bb2279(
bb175*bb1804){bb175*bb600;bbm(!bb1804->bb965)bb2;bb90(bb600=bb1804;
bb600;bb600=bb600->bb319){bb81(bb600->bb312,bb600->bb468,bb600->bb561
);bb600->bb456=bb600->bb561;bb600->bb1143=bb600->bb965;bb600->bb527=
bb600->bb521;bbm(bb600->bb521)bb21;}}bb41 bbb bb2475(bb175*bb1804){
bb175*bb600;bb317 bb138;bb333 bb414;bbq bb1156=0 ;bbm(!bb1804->bb965)bb2
;bb2279(bb1804);bb138=(bb317)(bb1804->bb312+bb12(bb365));bbm(bb138->
bb254!=17 )bb2;bb414=(bb333)(bb138+1 );bbm(bb414->bb320==0 ){bb1 bb101=
bb128(bb1156+12 );bbm(!bb101)bb2;bb90(bb600=bb1804;bb600;bb600=bb600->
bb319){bb138=(bb317)(bb600->bb312+bb12(bb365));bbm(bb600==bb1804){
bb567 bb551=(bb567)bb101;bb81(&bb551->bb309,&bb138->bb309,4 );bb81(&
bb551->bb203,&bb138->bb203,4 );bb551->bb912=0 ;bb551->bb254=17 ;bb551->
bb921=bb53(bb1156);bb414->bb320=0 ;}{bbq bb2310=bb194(bb138->bb370)-
bb12(bb325);bb81(bb101+bb12(bb612)+bb991(bb138->bb577),bb138+1 ,bb2310
);bb1156+=bb2310;}bbm(bb600->bb527)bb21;}bb414->bb320=bb868(bb101,
bb1156+12 );bb108(bb101);}}bb204 bb1257(bb33*bb109,bb175*bb49,bb78 bb75
){bbd bb26=bb444(bb49->bb26);bb161*bb1662=0 , *bb939=0 ;bb4 bb18=bb98;
bb204 bb519;bbu bb2432=0 ;bb49->bb456=0 ;bb2424:bb519=bb2494(bb109,bb26
,bb49->bb437,&bb1662,&bb939);bbm(bb519!=bb1002&&bb519!=bb1010)bb99
bb442;bbm(bb1662&&bb2324(bb109,bb1662)){bb1662->bb179.bb1107=0 ;bb99
bb2424;}bbm(bb939&&bb2324(bb109,bb939)){bb939->bb179.bb1107=0 ;bb99
bb2424;}bbm(bb1662){bb18=bb1154(bb75,bb109,bb49,bb1662,0 ,0 );bbm(((
bb18)!=bb98))bb99 bb1962;bb1662->bb1084++;{bb86 bb296;bb90(bb296=bb49
;bb296;bb296=bb296->bb319){bb1662->bb1037+=bb49->bb561;bbm(bb296->
bb521)bb21;}}}bb54 bbm((bb49->bb437==bb952||bb49->bb437==bb956)&&!
bb939){bb2475(bb49);}bbm(bb939){bbm(bb49->bb437==bb53(bb939->bb179.
bb1067)&&bb49->bb521&&(bb939->bb179.bb503==bb1079||bb939->bb179.bb503
==bb1056)&&bb49->bb26==bb444(bb939->bb179.bb26)){bbd bb2264;bbd bb92;
bb2264=(bb49->bb468[14 ]&0x0f )<<2 ;bb92=14 +bb2264+8 ;bbm(bb49->bb561==
bb92+1 &&bb49->bb468[bb92]==0xff ){bb2279(bb49);bb92=14 +bb2264;bb49->
bb312[bb92]=bb49->bb468[bb92+2 ];bb49->bb312[bb92+1 ]=bb49->bb468[bb92+
3 ];bb49->bb312[bb92+6 ]=0 ;bb49->bb312[bb92+7 ]=0 ;bb2432=1 ;}}}bbm(bb939
&&!bb2432){bb376 bb446;bbk bb372;bbk bb1895;bb317 bb138;bbd bb2220=0 ;
bbd bb1088=0 ;bb103 bb2075=0 ;bb103 bb2047=0 ;bbq bb1156=0 ;bb86 bb328=
bb49;bb106(bb328){bb446=(bb376)bb328->bb468;bb372=bb446->bb372;bb1895
=bb12(bb365);bbm(bb372==bb959){bb138=(bb317)(bb328->bb468+bb1895);
bb81(&bb2220,&bb138->bb309,4 );bb1088=bb939->bb179.bb557;bbm(bb1088){
bb81(&bb138->bb309,&bb1088,4 );bb939->bb1196=bb2220;bbm(bb328==bb49){
bbm(bb138->bb254==6 ){bb2075=1 ;bb328->bb561+=bb2419(bb138,bb1088);}
bb54 bbm(bb138->bb254==17 )bb2047=1 ;}bb138->bb614=0 ;bb138->bb614=bb923
(bb138);}bb1156+=bb194(bb138->bb370)-bb12(bb325);}bbm(bb328->bb521)bb21
;bb328=bb328->bb319;}bbm(bb2075){bb1 bb1320;bb567 bb551;bb313 bb200=
bb91;bb1320=bb128(bb1156+12 );bb328=bb49;bb106(bb328){bb138=(bb317)(
bb328->bb468+bb12(bb365));bbm(bb328==bb49){bb551=(bb567)bb1320;bb81(&
bb551->bb309,&bb138->bb309,4 );bb81(&bb551->bb203,&bb138->bb203,4 );
bb551->bb912=0 ;bb551->bb254=6 ;bb551->bb921=bb53((bbk)bb1156);bb200=(
bb313)(bb138+1 );
#ifdef _DEBUG
bb156("\x69\x70\x73\x65\x63\x20\x73\x65\x6e\x64\x20\x78\x66\x6f\x72"
"\x6d\x65\x64\x20");bb1263(bb138,bb91);
#endif
bb200->bb320=0 ;bbm(bb200->bb172&0x02 ){bbm(((bb200->bb92&0xF0 )>>2 )>(
bbe)bb12(bb490)){bbl*bb961=((bbl* )bb200)+bb12(bb490);bbl*bb2111=
bb961+(((bb200->bb92&0xF0 )>>2 )-bb12(bb490));bb106(bb961<bb2111){bb300
( *bb961){bb15 0 :bb961=bb2111;bb21;bb15 1 :bb961++;bb21;bb15 2 :{bbk
bb1209;bb59 bb2027=(bb59)(bb961+2 );bb1209=bb194( *bb2027);bbm(bb1209>
1400 )bb1209=1280 ;bb54 bbm(bb1209>300 )bb1209-=120 ; *bb2027=bb53(bb1209
);bb961+=4 ;}bb21;bb416:bb961+=bb961[1 ];bb21;}}}}}bb81(bb1320+bb12(
bb612)+bb991(bb138->bb577),bb138+1 ,bb194(bb138->bb370)-bb12(bb325));
bbm(bb328->bb521||bb987(bb138->bb577))bb21;bb328=bb328->bb319;}bb200
->bb320=bb868(bb1320,bb1156+12 );bb108(bb1320);}bbm(bb2047){bb333 bb414
=(bb333)(bb49->bb468+bb12(bb365)+bb12(bb325));bb414->bb320=0 ;}
#ifdef _DEBUG
bb156("\x6f\x75\x74\x67\x6f\x69\x6e\x67\x2c\x20\x62\x65\x66\x6f\x72"
"\x65\x20\x41\x70\x70\x6c\x79\x53\x41\n");bb1810(bb49->bb468+bb12(
bb365),bb49->bb561-bb12(bb365));
#endif
bb18=bb1154(bb75,bb109,bb49,bb939,0 ,1 );bbm(((bb18)==bb98)){bb328=bb49
;bb106(bb328){bb446=(bb376)bb328->bb312;bb372=bb446->bb372;bb1895=
bb12(bb365);bb138=(bb317)(bb328->bb312+bb1895);bbm(bb1088){bb81(&
bb138->bb309,&bb2220,4 );bb138->bb614=0 ;bb138->bb614=bb923(bb138);}bbm
(bb328->bb527||bb987(bb138->bb577))bb21;bb328=bb328->bb319;}}bbm(
bb109->bb1234&&bb49->bb312[bb12(bb365)+9 ]==17 ){bbk bb2335;bbd bb157;
bbq bbz,bb22,bb92,bb2234;bb59 bb312=(bb59)bb49->bb312;bb157=0 ;bb92=
bb12(bb365)+12 ;bb22=bb92+8 ;bb90(bbz=bb92;bbz<bb22;bbz+=2 )bb157+=bb312
[bbz/2 ];bb157+=17 <<8 ;bb92=bb12(bb365)+(( * (bb49->bb312+bb12(bb365))&
0xf )<<2 );bb157+=bb312[(bb92+4 )/2 ];bb2234=bb92+6 ;bb312[bb2234/2 ]=0 ;
bb90(bb328=bb49;bb328;bb328=bb328->bb319){bb59 bb312=(bb59)bb328->
bb312;bb92=bb12(bb365)+(( * (bb328->bb312+bb12(bb365))&0xf )<<2 );bb22=
bb328->bb456&0xfffffffe ;bb90(bbz=bb92;bbz<bb22;bbz+=2 )bb157+=bb312[
bbz/2 ];bbm(bb22!=bb328->bb456)bb157+= * (bb328->bb312+bb328->bb456-1 );
bb106(bb157>>16 )bb157=(bb157&0xffff )+(bb157>>16 );bbm(bb328->bb527)bb21
;}bb2335=~bb157;bb312[bb2234/2 ]=bb2335;}bbm(((bb18)!=bb98))bb99 bb1962
;bb49->bb1335=1 ;bb81(bb49->bb1306,bb939->bb369,6 );bb939->bb1084++;{
bb86 bb296;bb90(bb296=bb49;bb296;bb296=bb296->bb319){bb939->bb1037+=
bb49->bb561;bbm(bb296->bb521)bb21;}}}bbm(!bb939&&!bb1662&&bb49->bb437
!=bb952&&bb49->bb437!=bb956){bb18=bb1085;bb99 bb1962;}bb2436(bb109,
bb1662);bb2436(bb109,bb939);bb2 bb1147;bb1962:bb1818(bb109,bb26,bb18);
bb2 bb1045;bb442:bb2 bb519;}bb204 bb1236(bb33*bb109,bb175*bb49,bb78
bb75){bbd bb26=bb444(bb49->bb26);bbd bb104=0 ;bb161*bb166=0 ;bb4 bb18=
bb98;bbu bb177=0 ;bb49->bb456=0 ;bb18=bb1245(bb109,bb49,&bb104);bbm(((
bb18)!=bb98))bb99 bb1962;bbm(!bb104){bb2279(bb49);bb2 bb1147;}{bb166=
bb1844(bb109,bb26,bb104);bbm(!bb166)bb2 bb1045;bb300(bb166->bb179.
bb404[0 ].bb315.bb311.bb45.bb57){bb15 bb1353:bb15 bb1216:bb15 bb1033:
bb15 bb1117:bb177=1 ;bb21;bb15 bb1164:bb15 bb1289:bb15 bb1364:bb15
bb1299:bb416:bb177=0 ;bb21;}bb18=bb1154(bb75,bb109,bb49,bb166,1 ,bb177);
bbm(((bb18)==bb98)){bb166->bb1371++;{bb86 bb296=bb49;bb106(bb296){
bb166->bb1241+=bb49->bb456;bbm(bb296->bb527)bb21;bb296=bb296->bb319;}
}
#ifdef _DEBUG
bb156("\x69\x6e\x63\x6f\x6d\x69\x6e\x67\x2c\x20\x61\x66\x74\x65\x72"
"\x20\x41\x70\x70\x6c\x79\x53\x41\n");bb1810(bb49->bb312+bb12(bb365),
bb49->bb456-bb12(bb365));
#endif
}bbm(((bb18)==bb98)){bb376 bb446;bbk bb372;bbk bb1895;bb317 bb138;bbq
bb2411;bbq bb1088;bb103 bb2075=0 ;bb103 bb2047=0 ;bbq bb1156=0 ;bb86
bb328=bb49;bb103 bb2435=1 ;bbm(bb166->bb179.bb557){bb106(bb328){bb446=
(bb376)bb328->bb312;bb372=bb446->bb372;bb1895=bb12(bb365);bbm(bb372==
bb959){bb138=(bb317)(bb328->bb312+bb1895);bb81(&bb2411,&bb138->bb203,
4 );bb1088=bb166->bb179.bb557;bb1156+=(bb328->bb456-bb12(bb365)-bb12(
bb325));bbm(bb1088==bb2411){bb81(&bb138->bb203,&bb166->bb1196,4 );bbm(
bb328==bb49){bbm(bb138->bb254==6 ){bb2075=1 ;bb2315(bb138);}bb54 bbm(
bb138->bb254==17 ){bb2435=bb970(bb138->bb577);bb2047=1 ;}}bb138->bb614=
0 ;bb138->bb614=bb923(bb138);}bbm((bb328->bb527)){bb21;}}bb328=bb328->
bb319;}}bbm(bb2075){bb1 bb1320;bb567 bb551;bb313 bb200=bb91;bbq bb2316
=0 ;bb1320=bb128(bb1156+12 );bb328=bb49;bb106(bb328){bb138=(bb317)(
bb328->bb312+bb12(bb365));bbm(bb328==bb49){bb551=(bb567)bb1320;bb81(&
bb551->bb309,&bb138->bb309,4 );bb81(&bb551->bb203,&bb138->bb203,4 );
bb551->bb912=0 ;bb551->bb254=6 ;bb551->bb921=bb53((bbk)bb1156);bb200=(
bb313)(bb138+1 );
#ifdef _DEBUG
bb156("\x69\x70\x73\x65\x63\x20\x72\x65\x63\x76\x20");bb1263(bb138,
bb200);
#endif
bb200->bb320=0 ;bbm(bb200->bb172&0x02 ){bbm(((bb200->bb92&0xF0 )>>2 )>(
bbe)bb12(bb490)){bbl*bb961=((bbl* )bb200)+bb12(bb490);bbl*bb2111=
bb961+(((bb200->bb92&0xF0 )>>2 )-bb12(bb490));bb106(bb961<bb2111){bb300
( *bb961){bb15 0 :bb961=bb2111;bb21;bb15 1 :bb961++;bb21;bb15 2 :{bbk
bb1209;bb59 bb2027;bb2027=(bb59)(bb961+2 );bb1209=bb194( *bb2027);bbm(
bb1209>1400 )bb1209=1280 ;bb54 bbm(bb1209>300 )bb1209-=120 ; *bb2027=bb53
(bb1209);bb961+=4 ;}bb21;bb416:bb961+=bb961[1 ];bb21;}}}}}bb81(bb1320+
bb12(bb612)+bb2316,bb138+1 ,bb328->bb456-bb12(bb365)-bb12(bb325));
bb2316+=bb328->bb456-bb12(bb365)-bb12(bb325);bbm((bb328->bb527)){bb21
;}bb328=bb328->bb319;}bb200->bb320=bb868(bb1320,bb1156+12 );bb108(
bb1320);}bbm(bb2047){bb333 bb414=(bb333)(bb49->bb312+bb12(bb365)+bb12
(bb325));bbm(bb2435){bb414->bb320=0 ;}}}bbm(((bb18)!=bb98)){bb99 bb1962
;}}bb2 bb1147;bb1962:bb1818(bb109,bb26,bb18);bb2 bb1045;}
