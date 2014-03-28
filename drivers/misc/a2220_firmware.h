#if defined CONFIG_MACH_STUNNER
#include "a2220_stunner.h"
#elif defined CONFIG_MACH_GODIVA
#include "a2220_godiva.h"
#elif defined CONFIG_MACH_MELIUS_CHN_CTC
#include "a2220_melius_chn_ctc.h"
#elif defined CONFIG_MACH_MELIUS_TMO
#include "a2220_melius_tmo.h"
#elif defined CONFIG_MACH_MELIUS_USC
#include "a2220_melius_usc.h"
#elif defined CONFIG_MACH_MELIUS_VZW
#include "a2220_melius_vzw.h"
#elif defined CONFIG_MACH_MELIUS_SPR
#include "a2220_melius_spr.h"
#elif defined CONFIG_MACH_MELIUS_MTR
#include "a2220_melius_mtr.h"
#elif defined CONFIG_MACH_MELIUS_ATT
#ifdef CONFIG_MACH_MELIUS_CAN 
#include "a2220_melius.h"
#else
#include "a2220_melius_att.h"
#endif
#elif defined (CONFIG_MACH_MELIUS_SKT) || defined(CONFIG_MACH_MELIUS_KTT) || \
      defined(CONFIG_MACH_MELIUS_LGT)
#include "a2220_melius_kor.h"
#elif defined CONFIG_MACH_MELIUS_SWA_OPEN
#include "a2220_melius_swa.h"
#elif defined CONFIG_MACH_MELIUS
#include "a2220_melius.h"
#elif defined CONFIG_MACH_EXPRESS2
#include "a2220_express2.h"
#elif defined CONFIG_MACH_SERRANO_ATT
#include "a2220_serrano_att.h"
#elif defined CONFIG_MACH_SERRANO_TMO
#include "a2220_serrano_tmo.h"
#elif defined CONFIG_MACH_SERRANO_SPR
#include "a2220_serrano_spr.h"
#elif defined CONFIG_MACH_SERRANO_USC
#include "a2220_serrano_att.h"
#elif defined CONFIG_MACH_SERRANO_VZW
#include "a2220_serrano_vzw.h"
#elif defined CONFIG_MACH_SERRANO_LRA
#include "a2220_serrano_vzw.h"
#elif defined CONFIG_MACH_CRATER
#include "a2220_serrano_craterTD.h"
#elif defined CONFIG_MACH_BAFFIN
#include "a2220_serrano_baffinVETD.h"
#elif defined CONFIG_MACH_SERRANO
#include "a2220_serrano.h"
#elif defined CONFIG_MACH_GOLDEN_BMC
#include "a2220_golden_bmc.h"
#elif defined CONFIG_MACH_GOLDEN_ATT
#include "a2220_golden_att.h"
#elif defined CONFIG_MACH_GOLDEN_VZW
#include "a2220_golden_vzw.h"
#elif defined CONFIG_MACH_GOLDEN
#include "a2220_golden.h"
#elif defined CONFIG_MACH_CANE
#include "a2220_cane.h"
#elif defined CONFIG_MACH_LOGANRE
#include "a2220_loganre.h"
#else
#include "a2220_default.h"
#endif
