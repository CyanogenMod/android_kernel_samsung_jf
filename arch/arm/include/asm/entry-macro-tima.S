#ifndef ENTRY_MACRO_TIMA_S
#define ENTRY_MACRO_TIMA_S
#ifdef        TIMA_ENABLED
#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 6
        .arch_extension sec
#endif
#endif
#ifdef TIMA_EMUL_ENABLED
    .macro tima_macro_emul_crit_instn, r2val, cmdid
    stmfd   sp!, {r0-r3, r11}
    @ TZ side expects r0 to be in r11
    mov     r11, r0
    @ r2 : mcr_val
    mov     r2, \r2val
    @ r0: command
    ldr     r0, =\cmdid
    smc     #1
    @ Restore r0 and r2
    ldmfd   sp!, {r0-r3, r11}
    .endm
#endif
#endif
