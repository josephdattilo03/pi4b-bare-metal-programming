// AArch64 exception vector table for the Pi 4B (Cortex-A72).
// 16 entries: 4 groups (SP0, SPx, lower-EL AArch64, lower-EL AArch32),
// each group is sync/irq/fiq/serror. Entries are 0x80 apart, table is 0x800 aligned.

.section ".text.vectors"

// save the general purpose registers onto the stack
.macro save_context
    sub     sp, sp, #256
    stp     x0, x1,   [sp, #16 * 0]
    stp     x2, x3,   [sp, #16 * 1]
    stp     x4, x5,   [sp, #16 * 2]
    stp     x6, x7,   [sp, #16 * 3]
    stp     x8, x9,   [sp, #16 * 4]
    stp     x10, x11, [sp, #16 * 5]
    stp     x12, x13, [sp, #16 * 6]
    stp     x14, x15, [sp, #16 * 7]
    stp     x16, x17, [sp, #16 * 8]
    stp     x18, x19, [sp, #16 * 9]
    stp     x20, x21, [sp, #16 * 10]
    stp     x22, x23, [sp, #16 * 11]
    stp     x24, x25, [sp, #16 * 12]
    stp     x26, x27, [sp, #16 * 13]
    stp     x28, x29, [sp, #16 * 14]
    str     x30,      [sp, #16 * 15]
.endm

// restore the general purpose registers and return
.macro restore_context
    ldp     x0, x1,   [sp, #16 * 0]
    ldp     x2, x3,   [sp, #16 * 1]
    ldp     x4, x5,   [sp, #16 * 2]
    ldp     x6, x7,   [sp, #16 * 3]
    ldp     x8, x9,   [sp, #16 * 4]
    ldp     x10, x11, [sp, #16 * 5]
    ldp     x12, x13, [sp, #16 * 6]
    ldp     x14, x15, [sp, #16 * 7]
    ldp     x16, x17, [sp, #16 * 8]
    ldp     x18, x19, [sp, #16 * 9]
    ldp     x20, x21, [sp, #16 * 10]
    ldp     x22, x23, [sp, #16 * 11]
    ldp     x24, x25, [sp, #16 * 12]
    ldp     x26, x27, [sp, #16 * 13]
    ldp     x28, x29, [sp, #16 * 14]
    ldr     x30,      [sp, #16 * 15]
    add     sp, sp, #256
    eret
.endm

// one table slot: pass \kind in x0, jump to the real handler
.macro ventry kind, target
    .align  7                       // 128 bytes per entry
    mov     x0, #\kind
    b       \target
.endm

.global vectors
.align  11                          // 2KB aligned for VBAR
vectors:
    // Current EL with SP_EL0
    ventry  0,  sync_handler
    ventry  1,  irq_handler
    ventry  2,  fiq_handler
    ventry  3,  serror_handler

    // Current EL with SP_ELx
    ventry  4,  sync_handler
    ventry  5,  irq_handler
    ventry  6,  fiq_handler
    ventry  7,  serror_handler

    // Lower EL using AArch64
    ventry  8,  sync_handler
    ventry  9,  irq_handler
    ventry  10, fiq_handler
    ventry  11, serror_handler

    // Lower EL using AArch32
    ventry  12, sync_handler
    ventry  13, irq_handler
    ventry  14, fiq_handler
    ventry  15, serror_handler

// trampolines: save state, pass esr/elr, call into C, restore
sync_handler:
    save_context
    mrs     x1, esr_el2             // syndrome
    mrs     x2, elr_el2             // faulting address
    bl      handle_sync
    restore_context

irq_handler:
    save_context
    bl      handle_irq
    restore_context

fiq_handler:
    save_context
    bl      handle_fiq
    restore_context

serror_handler:
    save_context
    mrs     x1, esr_el2
    mrs     x2, elr_el2
    bl      handle_serror
    restore_context

// install the table into VBAR_EL2 (we run at EL2; use vbar_el1 if you drop to EL1)
.global vector_table_init
vector_table_init:
    ldr     x0, =vectors
    msr     vbar_el2, x0
    isb
    ret

.global enable_irq
enable_irq:
    msr     daifclr, #2             // clear I (irq) mask
    ret

.global disable_irq
disable_irq:
    msr     daifset, #2             // set I (irq) mask
    ret

// weak defaults so this links standalone; override in C. they just spin.
.weak   handle_sync
.weak   handle_irq
.weak   handle_fiq
.weak   handle_serror
handle_sync:
handle_irq:
handle_fiq:
handle_serror:
    b       .
