/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef METAL__DRIVERS__RISCV_CPU_H
#define METAL__DRIVERS__RISCV_CPU_H

#include <stdint.h>
#include <metal/cpu.h>
#include <metal/compiler.h>

#define METAL_MAX_CORES             8
#define METAL_MAX_MI                32  /* Per ISA MCause interrupts 32+ are Reserved */
#define METAL_MAX_ME                12  /* Per ISA Exception codes 12+ are Reserved   */
#define METAL_DEFAULT_RTC_FREQ      32768

#define METAL_DISABLE              0
#define METAL_ENABLE               1

#define METAL_MTVEC_DIRECT         0x00
#define METAL_MTVEC_VECTORED       0x01
#define METAL_MTVEC_CLIC           0x02
#define METAL_MTVEC_CLIC_VECTORED  0x03
#define METAL_MTVEC_CLIC_RESERVED  0x3C
#define METAL_MTVEC_MASK           0x3F
#if __riscv_xlen == 32
#define METAL_MCAUSE_INTR          0x80000000UL
#define METAL_MCAUSE_CAUSE         0x000003FFUL
#else
#define METAL_MCAUSE_INTR          0x8000000000000000UL
#define METAL_MCAUSE_CAUSE         0x00000000000003FFUL
#endif
#define METAL_MCAUSE_MINHV         0x40000000UL
#define METAL_MCAUSE_MPP           0x30000000UL
#define METAL_MCAUSE_MPIE          0x08000000UL
#define METAL_MCAUSE_MPIL          0x00FF0000UL
#define METAL_MSTATUS_MIE          0x00000008UL
#define METAL_MSTATUS_MPIE         0x00000080UL
#define METAL_MSTATUS_MPP          0x00001800UL
#define METAL_MSTATUS_MPRV         0x00020000UL
#define METAL_MSTATUS_MXR          0x00080000UL
#define METAL_MINTSTATUS_MIL       0xFF000000UL
#define METAL_MINTSTATUS_SIL       0x0000FF00UL
#define METAL_MINTSTATUS_UIL       0x000000FFUL

#define METAL_LOCAL_INTR(X)        (16 + X)
#define METAL_MCAUSE_EVAL(cause)   (cause & METAL_MCAUSE_INTR)
#define METAL_INTERRUPT(cause)     (METAL_MCAUSE_EVAL(cause) ? 1 : 0)
#define METAL_EXCEPTION(cause)     (METAL_MCAUSE_EVAL(cause) ? 0 : 1)
#define METAL_SW_INTR_EXCEPTION    (METAL_MCAUSE_INTR + 3)
#define METAL_TMR_INTR_EXCEPTION   (METAL_MCAUSE_INTR + 7)
#define METAL_EXT_INTR_EXCEPTION   (METAL_MCAUSE_INTR + 11)
#define METAL_LOCAL_INTR_EXCEPTION(X) (METAL_MCAUSE_INTR + METAL_LOCAL_INTR(X))
#define METAL_LOCAL_INTR_RESERVE0  1
#define METAL_LOCAL_INTR_RESERVE1  2
#define METAL_LOCAL_INTR_RESERVE2  4
#define METAL_LOCAL_INTERRUPT_SW   8             /* Bit3 0x008 */
#define METAL_LOCAL_INTR_RESERVE4  16
#define METAL_LOCAL_INTR_RESERVE5  32
#define METAL_LOCAL_INTR_RESERVE6  64
#define METAL_LOCAL_INTERRUPT_TMR  128           /* Bit7 0x080 */
#define METAL_LOCAL_INTR_RESERVE8  256
#define METAL_LOCAL_INTR_RESERVE9  512
#define METAL_LOCAL_INTR_RESERVE10 1024   
#define METAL_LOCAL_INTERRUPT_EXT  2048          /* Bit11 0x800 */
/* Bit12 to Bit15 are Reserved */
#define METAL_LOCAL_INTERRUPT(X)  (0x10000 << X) /* Bit16+ Start of Custom Local Interrupt */
#define METAL_MIE_INTERRUPT        METAL_MSTATUS_MIE

typedef enum {
  METAL_MACHINE_PRIVILEGE_MODE,
  METAL_SUPERVISOR_PRIVILEGE_MODE,
  METAL_USER_PRIVILEGE_MODE,
} metal_privilege_mode_e;

typedef enum {
  METAL_INTERRUPT_ID_BASE,
  METAL_INTERRUPT_ID_SW   = (METAL_INTERRUPT_ID_BASE + 3),
  METAL_INTERRUPT_ID_TMR  = (METAL_INTERRUPT_ID_BASE + 7),
  METAL_INTERRUPT_ID_EXT  = (METAL_INTERRUPT_ID_BASE + 11),
  METAL_INTERRUPT_ID_LC0  = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(0)),
  METAL_INTERRUPT_ID_LC1  = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(1)),
  METAL_INTERRUPT_ID_LC2  = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(2)),
  METAL_INTERRUPT_ID_LC3  = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(3)),
  METAL_INTERRUPT_ID_LC4  = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(4)),
  METAL_INTERRUPT_ID_LC5  = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(5)),
  METAL_INTERRUPT_ID_LC6  = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(6)),
  METAL_INTERRUPT_ID_LC7  = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(7)),
  METAL_INTERRUPT_ID_LC8  = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(8)),
  METAL_INTERRUPT_ID_LC9  = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(9)),
  METAL_INTERRUPT_ID_LC10 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(10)),
  METAL_INTERRUPT_ID_LC11 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(11)),
  METAL_INTERRUPT_ID_LC12 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(12)),
  METAL_INTERRUPT_ID_LC13 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(13)),
  METAL_INTERRUPT_ID_LC14 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(14)),
  METAL_INTERRUPT_ID_LC15 = (METAL_INTERRUPT_ID_BASE + METAL_LOCAL_INTR(15)),
  METAL_INTERRUPT_ID_LCMX,
  METAL_INTERRUPT_ID_GL0 = METAL_INTERRUPT_ID_LCMX,
  METAL_INTERRUPT_ID_GLMX = (METAL_MCAUSE_CAUSE + 1),
} metal_interrupt_id_e;

typedef enum {
  METAL_IAM_EXCEPTION_CODE,     /* Instruction address misaligned */
  METAL_IAF_EXCEPTION_CODE,     /* Instruction access faultd */
  METAL_II_EXCEPTION_CODE,      /* Illegal instruction */
  METAL_BREAK_EXCEPTION_CODE,   /* Breakpoint */
  METAL_LAM_EXCEPTION_CODE,     /* Load address misaligned */
  METAL_LAF_EXCEPTION_CODE,     /* Load access fault */
  METAL_SAMOAM_EXCEPTION_CODE,  /* Store/AMO address misaligned */
  METAL_SAMOAF_EXCEPTION_CODE,  /* Store/AMO access fault */
  METAL_ECALL_U_EXCEPTION_CODE, /* Environment call from U-mode */
  METAL_R9_EXCEPTION_CODE,      /* Reserved */
  METAL_R10_EXCEPTION_CODE,     /* Reserved */
  METAL_ECALL_M_EXCEPTION_CODE, /* Environment call from M-mode */
  METAL_MAX_EXCEPTION_CODE,
} metal_exception_code_e;

typedef enum {
  METAL_TIMER_MTIME_GET = 1,
  METAL_TIMER_MTIME_SET,
  METAL_SOFTWARE_IPI_CLEAR,
  METAL_SOFTWARE_IPI_SET,
  METAL_SOFTWARE_MSIP_GET,
  METAL_MAX_INTERRUPT_GET,
  METAL_INDEX_INTERRUPT_GET,
} metal_interrup_cmd_e;

typedef struct __metal_interrupt_data {
    long long pad : 64;
    metal_interrupt_handler_t  handler;
    void *sub_int;
    void *exint_data;
} __metal_interrupt_data;

/* CPU interrupt controller */

uintptr_t __metal_myhart_id(void);

struct __metal_driver_interrupt_controller_vtable {
    void (*interrupt_init)(const struct metal_interrupt *controller);
    int (*interrupt_register)(const struct metal_interrupt *controller,
			      int id, metal_interrupt_handler_t isr, void *priv_data);
    int (*interrupt_enable)(const struct metal_interrupt *controller, int id);
    int (*interrupt_disable)(const struct metal_interrupt *controller, int id);
    int (*command_request)(const struct metal_interrupt *intr, int cmd, void *data);
};

struct __metal_driver_vtable_riscv_cpu_intc {
  const struct metal_interrupt_vtable controller_vtable;
};

void __metal_driver_riscv_cpu_controller_interrupt_init(const struct metal_interrupt *controller);
int __metal_driver_riscv_cpu_controller_interrupt_register(const struct metal_interrupt *controller,
							 int id, metal_interrupt_handler_t isr,
							 void *priv_data);
int __metal_driver_riscv_cpu_controller_interrupt_enable(const struct metal_interrupt *controller, int id);
int __metal_driver_riscv_cpu_controller_interrupt_disable(const struct metal_interrupt *controller, int id);
int __metal_driver_riscv_cpu_controller_interrupt_enable_vector(const struct metal_interrupt *controller,
                                                              int id, metal_vector_mode mode);
int __metal_driver_riscv_cpu_controller_interrupt_disable_vector(const struct metal_interrupt *controller, int id);
int __metal_driver_riscv_cpu_controller_command_request(const struct metal_interrupt *controller,
						      int cmd, void *data);

void __metal_interrupt_global_enable(void);
void __metal_interrupt_global_disable(void);
void __metal_controller_interrupt_vector(metal_vector_mode mode, void *vec_table);
inline int __metal_controller_interrupt_is_selective_vectored (void)
{
    uintptr_t val;

    asm volatile ("csrr %0, mtvec" : "=r"(val));
    return ((val & METAL_MTVEC_CLIC_VECTORED) == METAL_MTVEC_CLIC);
}

__METAL_DECLARE_VTABLE(__metal_driver_vtable_riscv_cpu_intc) = {
    .controller_vtable.interrupt_init = __metal_driver_riscv_cpu_controller_interrupt_init,
    .controller_vtable.interrupt_register = __metal_driver_riscv_cpu_controller_interrupt_register,
    .controller_vtable.interrupt_enable   = __metal_driver_riscv_cpu_controller_interrupt_enable,
    .controller_vtable.interrupt_disable  = __metal_driver_riscv_cpu_controller_interrupt_disable,
    .controller_vtable.interrupt_vector_enable   = __metal_driver_riscv_cpu_controller_interrupt_enable_vector,
    .controller_vtable.interrupt_vector_disable  = __metal_driver_riscv_cpu_controller_interrupt_disable_vector,
    .controller_vtable.command_request    = __metal_driver_riscv_cpu_controller_command_request,
};


struct __metal_driver_riscv_cpu_intc_data {
    int init_done;
    uintptr_t metal_mtvec_table[METAL_MAX_MI];
    __metal_interrupt_data metal_int_table[METAL_MAX_MI];
    metal_exception_handler_t metal_exception_table[METAL_MAX_ME];
};

struct __metal_driver_riscv_cpu_intc {
    const struct metal_interrupt controller;
    const struct __metal_driver_vtable_riscv_cpu_intc *vtable;
    const int interrupt_controller;
    struct __metal_driver_riscv_cpu_intc_data *data;
};

/* CPU driver*/
struct __metal_driver_vtable_cpu {
  const struct metal_cpu_vtable cpu_vtable;
};

unsigned long long  __metal_driver_cpu_timer_get(const struct metal_cpu *cpu);
unsigned long long  __metal_driver_cpu_timebase_get(const struct metal_cpu *cpu);
unsigned long long
      __metal_driver_cpu_mtime_get(const struct metal_cpu *cpu);
int  __metal_driver_cpu_mtimecmp_set(const struct metal_cpu *cpu, unsigned long long time);
const struct metal_interrupt*
     __metal_driver_cpu_timer_controller_interrupt(const struct metal_cpu *cpu);
int  __metal_driver_cpu_get_timer_interrupt_id(const struct metal_cpu *cpu);
const struct metal_interrupt*
     __metal_driver_cpu_sw_controller_interrupt(const struct metal_cpu *cpu);
int  __metal_driver_cpu_get_sw_interrupt_id(const struct metal_cpu *cpu);
int  __metal_driver_cpu_set_sw_ipi(const struct metal_cpu *cpu, int hartid);
int  __metal_driver_cpu_clear_sw_ipi(const struct metal_cpu *cpu, int hartid);
int  __metal_driver_cpu_get_msip(const struct metal_cpu *cpu, int hartid);
const struct metal_interrupt*
     __metal_driver_cpu_controller_interrupt(const struct metal_cpu *cpu);
int  __metal_driver_cpu_exception_register(const struct metal_cpu *cpu, int ecode,
					 metal_exception_handler_t isr);
int  __metal_driver_cpu_get_instruction_length(const struct metal_cpu *cpu, uintptr_t epc);
uintptr_t  __metal_driver_cpu_get_exception_pc(const struct metal_cpu *cpu);
int  __metal_driver_cpu_set_exception_pc(const struct metal_cpu *cpu, uintptr_t epc);

__METAL_DECLARE_VTABLE(__metal_driver_vtable_cpu) = {
    .cpu_vtable.timer_get     = __metal_driver_cpu_timer_get,
    .cpu_vtable.timebase_get  = __metal_driver_cpu_timebase_get,
    .cpu_vtable.mtime_get = __metal_driver_cpu_mtime_get,
    .cpu_vtable.mtimecmp_set = __metal_driver_cpu_mtimecmp_set,
    .cpu_vtable.tmr_controller_interrupt = __metal_driver_cpu_timer_controller_interrupt,
    .cpu_vtable.get_tmr_interrupt_id = __metal_driver_cpu_get_timer_interrupt_id,
    .cpu_vtable.sw_controller_interrupt = __metal_driver_cpu_sw_controller_interrupt,
    .cpu_vtable.get_sw_interrupt_id = __metal_driver_cpu_get_sw_interrupt_id,
    .cpu_vtable.set_sw_ipi = __metal_driver_cpu_set_sw_ipi,
    .cpu_vtable.clear_sw_ipi = __metal_driver_cpu_clear_sw_ipi,
    .cpu_vtable.get_msip = __metal_driver_cpu_get_msip,
    .cpu_vtable.controller_interrupt = __metal_driver_cpu_controller_interrupt,
    .cpu_vtable.exception_register = __metal_driver_cpu_exception_register,
    .cpu_vtable.get_ilen = __metal_driver_cpu_get_instruction_length,
    .cpu_vtable.get_epc = __metal_driver_cpu_get_exception_pc,
    .cpu_vtable.set_epc = __metal_driver_cpu_set_exception_pc,
};

struct __metal_driver_cpu {
    const struct metal_cpu cpu;
    const struct __metal_driver_vtable_cpu *vtable;
    const int timebase;    
    const struct metal_interrupt *interrupt_controller;
};

#endif
