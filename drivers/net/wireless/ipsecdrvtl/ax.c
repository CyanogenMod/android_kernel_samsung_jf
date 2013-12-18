/*
   'src_compress_deflate_inffast.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Mon Oct 29 11:54:27 2012
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
#if ( defined( _WIN32) || defined( __WIN32__)) && ! defined( WIN32)
#define WIN32
#endif
#if defined( __GNUC__) || defined( WIN32) || defined( bb1224) ||  \
defined( bb1198)
#ifndef bb399
#define bb399
#endif
#endif
#if defined( __MSDOS__) && ! defined( bb168)
#define bb168
#endif
#if defined( bb168) && ! defined( bb399)
#define bb508
#endif
#ifdef bb168
#define bb1044
#endif
#if ( defined( bb168) || defined( bb1192) || defined( WIN32)) && !  \
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
#define bb1145
#endif
#ifndef bb210
#ifdef bb508
#define bb210 8
#else
#define bb210 9
#endif
#endif
#ifndef bbp
#ifdef bb136
#define bbp( bb403) bb403
#else
#define bbp( bb403) ()
#endif
#endif
bba bbf bb152;bba bbs bbe bb7;bba bbs bb6 bb24;bba bb152 bb30;bba bbl
bb439;bba bbe bb1098;bba bb7 bb165;bba bb24 bb167;
#ifdef bb136
bba bbb*bb70;bba bbb*bb182;
#else
bba bb152*bb70;bba bb152*bb182;
#endif
#ifdef __cplusplus
bbr"\x43"{
#endif
bba bb70( *bb507)bbp((bb70 bb110,bb7 bb495,bb7 bb47));bba bbb( *bb506
)bbp((bb70 bb110,bb70 bb1140));bbi bb381;bba bbi bb1188{bb30*bb126;
bb7 bb148;bb24 bb192;bb30*bb596;bb7 bb384;bb24 bb615;bbl*bb322;bbi
bb381*bb23;bb507 bb401;bb506 bb367;bb70 bb110;bbe bb980;bb24 bb368;
bb24 bb1152;}bb433;bba bb433*bb17;bbr bbh bbl*bb1159 bbp((bbb));bbr
bbe bb513 bbp((bb17 bb16,bbe bb174));bbr bbe bb945 bbp((bb17 bb16));
bbr bbe bb1055 bbp((bb17 bb16,bbe bb174));bbr bbe bb958 bbp((bb17 bb16
));bbr bbe bb1178 bbp((bb17 bb16,bbh bb30*bb424,bb7 bb436));bbr bbe
bb1151 bbp((bb17 bb129,bb17 bb183));bbr bbe bb1059 bbp((bb17 bb16));
bbr bbe bb1177 bbp((bb17 bb16,bbe bb123,bbe bb299));bbr bbe bb1149 bbp
((bb17 bb16,bbh bb30*bb424,bb7 bb436));bbr bbe bb1176 bbp((bb17 bb16));
bbr bbe bb1011 bbp((bb17 bb16));bbr bbe bb1150 bbp((bb30*bb129,bb167*
bb314,bbh bb30*bb183,bb24 bb326));bbr bbe bb1148 bbp((bb30*bb129,
bb167*bb314,bbh bb30*bb183,bb24 bb326,bbe bb123));bbr bbe bb1161 bbp(
(bb30*bb129,bb167*bb314,bbh bb30*bb183,bb24 bb326));bba bb182 bb34;
bbr bb34 bb1206 bbp((bbh bbl*bb1072,bbh bbl*bb57));bbr bb34 bb1207 bbp
((bbe bb471,bbh bbl*bb57));bbr bbe bb1227 bbp((bb34 bb25,bbe bb123,
bbe bb299));bbr bbe bb1193 bbp((bb34 bb25,bb182 bb40,bbs bb22));bbr
bbe bb1189 bbp((bb34 bb25,bbh bb182 bb40,bbs bb22));bbr bbe bb1214 bbp
((bb34 bb25,bbh bbl*bb1228,...));bbr bbe bb1191 bbp((bb34 bb25,bbh bbl
 *bbg));bbr bbl*bb1182 bbp((bb34 bb25,bbl*bb40,bbe bb22));bbr bbe
bb1218 bbp((bb34 bb25,bbe bbo));bbr bbe bb1183 bbp((bb34 bb25));bbr
bbe bb1187 bbp((bb34 bb25,bbe bb174));bbr bb6 bb1195 bbp((bb34 bb25,
bb6 bb92,bbe bb1202));bbr bbe bb1181 bbp((bb34 bb25));bbr bb6 bb1230
bbp((bb34 bb25));bbr bbe bb1197 bbp((bb34 bb25));bbr bbe bb1203 bbp((
bb34 bb25));bbr bbh bbl*bb1186 bbp((bb34 bb25,bbe*bb1180));bbr bb24
bb986 bbp((bb24 bb368,bbh bb30*bb40,bb7 bb22));bbr bb24 bb1167 bbp((
bb24 bb378,bbh bb30*bb40,bb7 bb22));bbr bbe bb1115 bbp((bb17 bb16,bbe
bb123,bbh bbl*bb187,bbe bb197));bbr bbe bb1127 bbp((bb17 bb16,bbh bbl
 *bb187,bbe bb197));bbr bbe bb1050 bbp((bb17 bb16,bbe bb123,bbe bb568
,bbe bb451,bbe bb933,bbe bb299,bbh bbl*bb187,bbe bb197));bbr bbe
bb1082 bbp((bb17 bb16,bbe bb451,bbh bbl*bb187,bbe bb197));bbr bbh bbl
 *bb1170 bbp((bbe bb18));bbr bbe bb1175 bbp((bb17 bby));bbr bbh bb167
 *bb1162 bbp((bbb));
#ifdef __cplusplus
}
#endif
#define bb963 1
#ifdef bb136
#if defined( bb1741)
#else
#endif
#endif
bba bbs bbl bb155;bba bb155 bb1205;bba bbs bb135 bb125;bba bb125 bb501
;bba bbs bb6 bb393;bbr bbh bbl*bb1070[10 ];
#if bb210 >= 8
#define bb803 8
#else
#define bb803 bb210
#endif
#ifdef bb168
#define bb419 0x00
#if defined( __TURBOC__) || defined( __BORLANDC__)
#if( __STDC__ == 1) && ( defined( bb1792) || defined( bb1772))
bbb bb941 bb1318(bbb*bb101);bbb*bb941 bb1344(bbs bb6 bb1740);
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
#define bb419 0x0b
#endif
#if ( defined( _MSC_VER) && ( _MSC_VER > 600))
#define bb1754( bb471, bb131) bb1788( bb471, bb131)
#endif
#ifndef bb419
#define bb419 0x03
#endif
#if defined( bb1549) && ! defined( _MSC_VER) && ! defined( bb1775)
#define bb963
#endif
bba bb24( *bb954)bbp((bb24 bb473,bbh bb30*bb40,bb7 bb22));bb70 bb1168
bbp((bb70 bb110,bbs bb495,bbs bb47));bbb bb1163 bbp((bb70 bb110,bb70
bb914));bba bbi bb1374 bb153;bbi bb1374{bb550{bbi{bb152 bb1171;bb152
bb960;}bb512;bb7 bb1547;}bb511;bb7 bb610;};bbr bbe bb2014 bbp((bb165*
,bb165* ,bb153* * ,bb153* ,bb17));bbr bbe bb1997 bbp((bb7,bb7,bb165* ,
bb165* ,bb165* ,bb153* * ,bb153* * ,bb153* ,bb17));bbr bbe bb1967 bbp
((bb165* ,bb165* ,bb153* * ,bb153* * ,bb17));bbi bb1103;bba bbi bb1103
bb302;bbr bb302*bb2011 bbp((bb17 bby,bb954 bbo,bb7 bbv));bbr bbe
bb1981 bbp((bb302* ,bb17,bbe));bbr bbb bb1791 bbp((bb302* ,bb17,bb167
 * ));bbr bbe bb1996 bbp((bb302* ,bb17));bbr bbb bb2024 bbp((bb302*
bbg,bbh bb30*bbt,bb7 bb11));bbr bbe bb1979 bbp((bb302*bbg));bbi bb1232
;bba bbi bb1232 bb702;bbr bb702*bb1968 bbp((bb7,bb7,bb153* ,bb153* ,
bb17));bbr bbe bb2071 bbp((bb302* ,bb17,bbe));bbr bbb bb1990 bbp((
bb702* ,bb17));bba bb9{bb1770,bb2026,bb2037,bb2074,bb2017,bb1978,
bb1958,bb1884,bb1785,bb922}bb1900;bbi bb1103{bb1900 bb57;bb550{bb7
bb189;bbi{bb7 bb1023;bb7 bb163;bb165*bb1129;bb7 bb1721;bb153*bb1771;}
bb443;bbi{bb702*bb1762;}bb1752;}bb149;bb7 bb1885;bb7 bb366;bb24 bb362
;bb153*bb1807;bb30*bb158;bb30*bb442;bb30*bb371;bb30*bb199;bb954 bb1559
;bb24 bb473;};bb41 bbh bb7 bb1114[17 ]={0x0000 ,0x0001 ,0x0003 ,0x0007 ,
0x000f ,0x001f ,0x003f ,0x007f ,0x00ff ,0x01ff ,0x03ff ,0x07ff ,0x0fff ,0x1fff
,0x3fff ,0x7fff ,0xffff };bbr bbe bb394 bbp((bb302* ,bb17,bbe));bbi bb381
{bbe bb445;};bbr bbe bb2200 bbp((bb7,bb7,bb153* ,bb153* ,bb302* ,bb17
));bbi bb1232{bbe bb445;};bbe bb2200(bb55,bb962,bb1018,bb1005,bbg,bby
)bb7 bb55,bb962;bb153*bb1018;bb153*bb1005;bb302*bbg;bb17 bby;{bb153*
bb45;bb7 bbw;bb24 bbn;bb7 bb3;bb30*bb28;bb7 bb11;bb30*bb84;bb7 bb95;
bb7 bb2395;bb7 bb2397;bb7 bbo;bb7 bbt;bb30*bb27;{{bb28=bby->bb126;
bb11=bby->bb148;bbn=bbg->bb362;bb3=bbg->bb366;}{bb84=bbg->bb199;bb95=
(bb7)(bb7)(bb84<bbg->bb371?bbg->bb371-bb84-1 :bbg->bb442-bb84);}}
bb2395=bb1114[bb55];bb2397=bb1114[bb962];bb573{{bb106(bb3<(20 )){bbn|=
((bb24)(bb11--, *bb28++))<<bb3;bb3+=8 ;}}bbm((bbw=(bb45=bb1018+((bb7)bbn
&bb2395))->bb511.bb512.bb1171)==0 ){{bbn>>=(bb45->bb511.bb512.bb960);
bb3-=(bb45->bb511.bb512.bb960);}; *bb84++=(bb152)bb45->bb610;bb95--;
bb1684;}bb573{{bbn>>=(bb45->bb511.bb512.bb960);bb3-=(bb45->bb511.
bb512.bb960);}bbm(bbw&16 ){bbw&=15 ;bbo=bb45->bb610+((bb7)bbn&bb1114[
bbw]);{bbn>>=(bbw);bb3-=(bbw);};{bb106(bb3<(15 )){bbn|=((bb24)(bb11--,
 *bb28++))<<bb3;bb3+=8 ;}};bbw=(bb45=bb1005+((bb7)bbn&bb2397))->bb511.
bb512.bb1171;bb573{{bbn>>=(bb45->bb511.bb512.bb960);bb3-=(bb45->bb511
.bb512.bb960);}bbm(bbw&16 ){bbw&=15 ;{bb106(bb3<(bbw)){bbn|=((bb24)(
bb11--, *bb28++))<<bb3;bb3+=8 ;}}bbt=bb45->bb610+((bb7)bbn&bb1114[bbw]
);{bbn>>=(bbw);bb3-=(bbw);};bb95-=bbo;bbm((bb7)(bb84-bbg->bb158)>=bbt
){bb27=bb84-bbt; *bb84++= *bb27++;bbo--; *bb84++= *bb27++;bbo--;}bb54
{bbw=bbt-(bb7)(bb84-bbg->bb158);bb27=bbg->bb442-bbw;bbm(bbo>bbw){bbo
-=bbw;bb573{ *bb84++= *bb27++;}bb106(--bbw);bb27=bbg->bb158;}}bb573{ *
bb84++= *bb27++;}bb106(--bbo);bb21;}bb54 bbm((bbw&64 )==0 ){bb45+=bb45
->bb610;bbw=(bb45+=((bb7)bbn&bb1114[bbw]))->bb511.bb512.bb1171;}bb54{
bby->bb322=(bbl* )"";{bbo=bby->bb148-bb11;bbo=(bb3>>3 )<bbo?bb3>>3 :bbo
;bb11+=bbo;bb28-=bbo;bb3-=bbo<<3 ;}{{bbg->bb362=bbn;bbg->bb366=bb3;}{
bby->bb148=bb11;bby->bb192+=(bb24)(bb28-bby->bb126);bby->bb126=bb28;}
{bbg->bb199=bb84;}}bb2(-3 );}}bb106(1 );bb21;}bbm((bbw&64 )==0 ){bb45+=
bb45->bb610;bbm((bbw=(bb45+=((bb7)bbn&bb1114[bbw]))->bb511.bb512.
bb1171)==0 ){{bbn>>=(bb45->bb511.bb512.bb960);bb3-=(bb45->bb511.bb512.
bb960);}; *bb84++=(bb152)bb45->bb610;bb95--;bb21;}}bb54 bbm(bbw&32 ){;
{bbo=bby->bb148-bb11;bbo=(bb3>>3 )<bbo?bb3>>3 :bbo;bb11+=bbo;bb28-=bbo;
bb3-=bbo<<3 ;}{{bbg->bb362=bbn;bbg->bb366=bb3;}{bby->bb148=bb11;bby->
bb192+=(bb24)(bb28-bby->bb126);bby->bb126=bb28;}{bbg->bb199=bb84;}}
bb2 1 ;}bb54{bby->bb322=(bbl* )"";{bbo=bby->bb148-bb11;bbo=(bb3>>3 )<
bbo?bb3>>3 :bbo;bb11+=bbo;bb28-=bbo;bb3-=bbo<<3 ;}{{bbg->bb362=bbn;bbg
->bb366=bb3;}{bby->bb148=bb11;bby->bb192+=(bb24)(bb28-bby->bb126);bby
->bb126=bb28;}{bbg->bb199=bb84;}}bb2(-3 );}}bb106(1 );}bb106(bb95>=258
&&bb11>=10 );{bbo=bby->bb148-bb11;bbo=(bb3>>3 )<bbo?bb3>>3 :bbo;bb11+=
bbo;bb28-=bbo;bb3-=bbo<<3 ;}{{bbg->bb362=bbn;bbg->bb366=bb3;}{bby->
bb148=bb11;bby->bb192+=(bb24)(bb28-bby->bb126);bby->bb126=bb28;}{bbg
->bb199=bb84;}}bb2 0 ;}
