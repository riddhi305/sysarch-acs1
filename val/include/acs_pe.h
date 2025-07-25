/** @file
 * Copyright (c) 2016-2020,2022-2025, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#ifndef __ACS_PE_H__
#define __ACS_PE_H__

/* Processor Family Type 4 */
#define PROCESSOR_FAMILY_ARMV9         0x102

#define MPIDR_AFF_MASK           (0xFF00FFFFFF)
#define INVALID_PE_INFO          (0xDEADDEAD)

/* PE Affinity level masks */
#define PE_AFFINITY_LVL_1  0xFF00FFFF00ULL
#define PE_AFFINITY_LVL_2  0xFF00FF0000ULL
#define PE_AFFINITY_LVL_3  0xFF00000000ULL

/* PE PMU Version */

#define PE_PMUv3p5 0x06
#define PE_PMUv3p7 0x07

//
//  AARCH64 processor exception types.
//
#define EXCEPT_AARCH64_SYNCHRONOUS_EXCEPTIONS    0
#define EXCEPT_AARCH64_IRQ                       1
#define EXCEPT_AARCH64_FIQ                       2
#define EXCEPT_AARCH64_SERROR                    3

// AArch64 Exception Level
#define AARCH64_EL2  0x8
#define AARCH64_EL1  0x4
#define AARCH64_EL_MASK (0x3ull << 2)

#define AARCH64_HCR_E2H_MASK (0x1ull << 34)
#define AARCH64_TTBR_ADDR_MASK (((0x1ull << 47) - 1) << 1)
#define BSA_TCR_TG1_SHIFT   30
#define BSA_TCR_SH1_SHIFT   28
#define BSA_TCR_ORGN1_SHIFT 26
#define BSA_TCR_IRGN1_SHIFT 24
#define BSA_TCR_T1SZ_SHIFT  16

#define BSA_TCR_TG1_MASK   (0x3ull << BSA_TCR_TG1_SHIFT)
#define BSA_TCR_SH1_MASK   (0x3ull << BSA_TCR_SH1_SHIFT)
#define BSA_TCR_ORGN1_MASK (0x3ull << BSA_TCR_ORGN1_SHIFT)
#define BSA_TCR_IRGN1_MASK (0x3ull << BSA_TCR_IRGN1_SHIFT)
#define BSA_TCR_T1SZ_MASK  (0x3full << BSA_TCR_T1SZ_SHIFT)

#define BSA_TCR_TG0_SHIFT   14
#define BSA_TCR_SH0_SHIFT   12
#define BSA_TCR_ORGN0_SHIFT 10
#define BSA_TCR_IRGN0_SHIFT 8
#define BSA_TCR_T0SZ_SHIFT  0

#define BSA_TCR_TG0_MASK    (0x3ull << BSA_TCR_TG0_SHIFT)
#define BSA_TCR_SH0_MASK    (0x3ull << BSA_TCR_SH0_SHIFT)
#define BSA_TCR_ORGN0_MASK  (0x3ull << BSA_TCR_ORGN0_SHIFT)
#define BSA_TCR_IRGN0_MASK  (0x3ull << BSA_TCR_IRGN0_SHIFT)
#define BSA_TCR_T0SZ_MASK   (0x3full << BSA_TCR_T0SZ_SHIFT)

#define BSA_TCR_IPS_SHIFT  32
#define BSA_TCR_IPS_MASK   (0x7ull << BSA_TCR_IPS_SHIFT)
#define BSA_TCR_PS_SHIFT   16
#define BSA_TCR_PS_MASK    (0x7ull << BSA_TCR_PS_SHIFT)

/* MPIDR macros */
#define PAL_MPIDR_AFFLVL_MASK    0xffull
#define PAL_MPIDR_AFFINITY_BITS  8
#define PAL_MPIDR_AFF0_SHIFT     0
#define PAL_MPIDR_AFF1_SHIFT     8u
#define PAL_MPIDR_AFF2_SHIFT     16u
#define PAL_MPIDR_AFF3_SHIFT     32u
#define PAL_MPIDR_AFFLVL0        0x0ull
#define PAL_MPIDR_AFFLVL1        0x1ull
#define PAL_MPIDR_AFFLVL2        0x2ull
#define PAL_MPIDR_AFFLVL3        0x3ull

#define PAL_MPIDR_AFFINITY_MASK ((PAL_MPIDR_AFFLVL_MASK << PAL_MPIDR_AFF3_SHIFT) | \
                 (PAL_MPIDR_AFFLVL_MASK << PAL_MPIDR_AFF2_SHIFT) | \
                 (PAL_MPIDR_AFFLVL_MASK << PAL_MPIDR_AFF1_SHIFT) | \
                 (PAL_MPIDR_AFFLVL_MASK << PAL_MPIDR_AFF0_SHIFT))

#define PAL_INVALID_MPID        0xFFFFFFFFu

typedef enum {
  MPIDR_EL1 = 1,
  ID_AA64PFR0_EL1,
  ID_AA64PFR1_EL1,
  ID_AA64MMFR0_EL1,
  ID_AA64MMFR1_EL1,
  ID_AA64MMFR2_EL1,
  ID_AA64DFR0_EL1,
  ID_AA64DFR1_EL1,
  CTR_EL0,
  ID_AA64ISAR0_EL1,
  ID_AA64ISAR1_EL1,
  SCTLR_EL3,
  SCTLR_EL2,
  SCTLR_EL1,
  PMCR_EL0,
  PMCCNTR_EL0,
  PMCCFILTR_EL0,
  PMCNTENSET_EL0,
  PMOVSSET_EL0,
  PMOVSCLR_EL0,
  PMINTENSET_EL1,
  PMINTENCLR_EL1,
  CurrentEL,
  MDCR_EL2,
  VBAR_EL2,
  CCSIDR_EL1,
  CSSELR_EL1,
  CLIDR_EL1,
  ID_DFR0_EL1,
  ID_ISAR0_EL1,
  ID_ISAR1_EL1,
  ID_ISAR2_EL1,
  ID_ISAR3_EL1,
  ID_ISAR4_EL1,
  ID_ISAR5_EL1,
  ID_MMFR0_EL1,
  ID_MMFR1_EL1,
  ID_MMFR2_EL1,
  ID_MMFR3_EL1,
  ID_MMFR4_EL1,
  ID_PFR0_EL1,
  ID_PFR1_EL1,
  MIDR_EL1,
  MVFR0_EL1,
  MVFR1_EL1,
  MVFR2_EL1,
  PMCEID0_EL0,
  PMCEID1_EL0,
  VMPIDR_EL2,
  VPIDR_EL2,
  PMBIDR_EL1,
  PMSIDR_EL1,
  LORID_EL1,
  ERRIDR_EL1,
  ERR0FR_EL1,
  ERR1FR_EL1,
  ERR2FR_EL1,
  ERR3FR_EL1,
  PMSIRR_EL1,
  PMSCR_EL2,
  PMSFCR_EL1,
  PMBPTR_EL1,
  PMBLIMITR_EL1,
  ESR_EL2,
  FAR_EL2,
  RDVL,
  MAIR_ELx,
  TCR_ELx,
  TTBR_ELx,
  DBGBCR0_EL1,
  DBGBCR1_EL1,
  DBGBCR2_EL1,
  DBGBCR3_EL1,
  DBGBCR4_EL1,
  DBGBCR5_EL1,
  DBGBCR6_EL1,
  DBGBCR7_EL1,
  DBGBCR8_EL1,
  DBGBCR9_EL1,
  DBGBCR10_EL1,
  DBGBCR11_EL1,
  DBGBCR12_EL1,
  DBGBCR13_EL1,
  DBGBCR14_EL1,
  DBGBCR15_EL1,
  ID_AA64ZFR0_EL1,
  ID_AA64ISAR2_EL1,
  BRBIDR0_EL,
  BRBIDR0_EL1,
  TRBIDR_EL1,
  TRCIDR0,
  TRCIDR4,
  TRCIDR5,
  HCR_EL2,
  VTCR_EL2
} BSA_ACS_PE_REGS;

uint64_t ArmReadMpidr(void);
uint64_t ArmReadIdPfr0(void);
uint64_t ArmReadIdPfr1(void);
uint64_t ArmReadHcr(void);
uint64_t AA64ReadMmfr0(void);
uint64_t AA64ReadMmfr1(void);
uint64_t AA64ReadMmfr2(void);
uint64_t AA64ReadCtr(void);
uint64_t AA64ReadIsar0(void);
uint64_t AA64ReadIsar1(void);
uint64_t AA64ReadIsar2(void);
uint64_t AA64ReadSctlr3(void);
uint64_t AA64ReadSctlr2(void);
uint64_t AA64ReadSctlr1(void);
uint64_t AA64ReadPmcr(void);
uint64_t AA64ReadIdDfr0(void);
uint64_t AA64ReadIdDfr1(void);
uint64_t AA64ReadCurrentEL(void);
uint64_t AA64ReadMdcr2(void);
uint64_t AA64ReadVbar2(void);
uint64_t AA64ReadCcsidr(void);
uint64_t AA64ReadCsselr(void);
uint64_t AA64ReadClidr(void);
uint64_t ArmReadDfr0(void);
uint64_t ArmReadIsar0(void);
uint64_t ArmReadIsar1(void);
uint64_t ArmReadIsar2(void);
uint64_t ArmReadIsar3(void);
uint64_t ArmReadIsar4(void);
uint64_t ArmReadIsar5(void);
uint64_t ArmReadMmfr0(void);
uint64_t ArmReadMmfr1(void);
uint64_t ArmReadMmfr2(void);
uint64_t ArmReadMmfr3(void);
uint64_t ArmReadMmfr4(void);
uint64_t ArmReadPfr0(void);
uint64_t ArmReadPfr1(void);
uint64_t ArmReadMidr(void);
uint64_t ArmReadMvfr0(void);
uint64_t ArmReadMvfr1(void);
uint64_t ArmReadMvfr2(void);
uint64_t AA64ReadPmceid0(void);
uint64_t AA64ReadPmceid1(void);
uint64_t AA64ReadVmpidr(void);
uint64_t AA64ReadVpidr(void);
uint64_t AA64ReadPmbidr(void);
uint64_t AA64ReadPmsidr(void);
uint64_t AA64ReadLorid(void);
uint64_t AA64ReadErridr(void);
uint64_t AA64ReadErr0fr(void);
uint64_t AA64ReadErr1fr(void);
uint64_t AA64ReadErr2fr(void);
uint64_t AA64ReadErr3fr(void);
uint64_t AA64ReadMair1(void);
uint64_t AA64ReadMair2(void);
uint64_t AA64ReadTcr1(void);
uint64_t AA64ReadTcr2(void);
uint64_t AA64ReadTtbr0El1(void);
uint64_t AA64ReadTtbr0El2(void);
uint64_t AA64ReadTtbr1El1(void);
uint64_t AA64ReadTtbr1El2(void);
uint64_t AA64WriteSp(uint64_t write_data);
uint64_t AA64ReadEsr2(void);
uint64_t AA64ReadSp(void);
uint64_t AA64ReadFar2(void);
uint64_t ArmRdvl(void);
uint64_t AA64ReadDbgbcr0El1(void);
uint64_t AA64ReadDbgbcr1El1(void);
uint64_t AA64ReadDbgbcr2El1(void);
uint64_t AA64ReadDbgbcr3El1(void);
uint64_t AA64ReadDbgbcr4El1(void);
uint64_t AA64ReadDbgbcr5El1(void);
uint64_t AA64ReadDbgbcr6El1(void);
uint64_t AA64ReadDbgbcr7El1(void);
uint64_t AA64ReadDbgbcr8El1(void);
uint64_t AA64ReadDbgbcr9El1(void);
uint64_t AA64ReadDbgbcr10El1(void);
uint64_t AA64ReadDbgbcr11El1(void);
uint64_t AA64ReadDbgbcr12El1(void);
uint64_t AA64ReadDbgbcr13El1(void);
uint64_t AA64ReadDbgbcr14El1(void);
uint64_t AA64ReadDbgbcr15El1(void);
uint64_t AA64ReadZfr0(void);
uint64_t AA64ReadBrbidr0(void);
uint64_t AA64ReadTrbidr(void);
uint64_t AA64ReadVtcr(void);
uint64_t AA64ReadTrcidr0(void);
uint64_t AA64ReadTrcidr4(void);
uint64_t AA64ReadTrcidr5(void);

void DisableSpe(void);
void ArmCallWFI(void);
void ArmExecuteMemoryBarrier(void);
void AA64WritePmsirr(uint64_t write_data);
void AA64WritePmscr2(uint64_t write_data);
void AA64WritePmsfcr(uint64_t write_data);
void AA64WritePmblimitr(uint64_t write_data);
void AA64WritePmbptr(uint64_t write_data);
void AA64WriteMdcr2(uint64_t write_data);
void AA64WritePmcr(uint64_t write_data);
void AA64WritePmovsset(uint64_t write_data);
void AA64WritePmovsclr(uint64_t write_data);
void AA64WritePmintenset(uint64_t write_data);
void AA64WritePmintenclr(uint64_t write_data);
void AA64WriteCsselr(uint64_t write_data);
void AA64WriteVbar2(uint64_t write_data);

void val_pe_update_elr(void *context, uint64_t offset);
void val_pe_context_save(uint64_t sp, uint64_t elr);
void val_pe_initialize_default_exception_handler(void (*esr)(uint64_t, void *));
void val_pe_context_restore(uint64_t sp);
void val_pe_default_esr(uint64_t interrupt_type, void *context);
void val_pe_cache_clean_range(uint64_t start_addr, uint64_t length);

uint64_t val_pe_get_esr(void *context);
uint64_t val_pe_get_far(void *context);

uint32_t pe001_entry(uint32_t num_pe);
uint32_t pe002_entry(uint32_t num_pe);
uint32_t pe003_entry(uint32_t num_pe);
uint32_t pe004_entry(uint32_t num_pe);
uint32_t pe006_entry(uint32_t num_pe);
uint32_t pe007_entry(uint32_t num_pe);
uint32_t pe008_entry(uint32_t num_pe);
uint32_t pe009_entry(uint32_t num_pe);
uint32_t pe010_entry(uint32_t num_pe);
uint32_t pe011_entry(uint32_t num_pe);
uint32_t pe012_entry(uint32_t num_pe);
uint32_t pe013_entry(uint32_t num_pe);
uint32_t pe014_entry(uint32_t num_pe);
uint32_t pe015_entry(uint32_t num_pe);
uint32_t pe016_entry(uint32_t num_pe);
uint32_t pe017_entry(uint32_t num_pe);
uint32_t pe018_entry(uint32_t num_pe);
uint32_t pe019_entry(uint32_t num_pe);
uint32_t pe020_entry(uint32_t num_pe);
uint32_t pe021_entry(uint32_t num_pe);
uint32_t pe022_entry(uint32_t num_pe);

uint32_t pe023_entry(uint32_t num_pe);
uint32_t pe024_entry(uint32_t num_pe);
uint32_t pe025_entry(uint32_t num_pe);
uint32_t pe026_entry(uint32_t num_pe);
uint32_t pe027_entry(uint32_t num_pe);
uint32_t pe028_entry(uint32_t num_pe);
uint32_t pe029_entry(uint32_t num_pe);
uint32_t pe030_entry(uint32_t num_pe);
uint32_t pe031_entry(uint32_t num_pe);
uint32_t pe032_entry(uint32_t num_pe);
uint32_t pe033_entry(uint32_t num_pe);
uint32_t pe034_entry(uint32_t num_pe);
uint32_t pe035_entry(uint32_t num_pe);
uint32_t pe036_entry(uint32_t num_pe);
uint32_t pe037_entry(uint32_t num_pe);
uint32_t pe038_entry(uint32_t num_pe);
uint32_t pe039_entry(uint32_t num_pe);
uint32_t pe040_entry(uint32_t num_pe);
uint32_t pe041_entry(uint32_t num_pe);
uint32_t pe042_entry(uint32_t num_pe);
uint32_t pe043_entry(uint32_t num_pe);
uint32_t pe044_entry(uint32_t num_pe);
uint32_t pe045_entry(uint32_t num_pe);
uint32_t pe046_entry(uint32_t num_pe);
uint32_t pe047_entry(uint32_t num_pe);
uint32_t pe048_entry(uint32_t num_pe);
uint32_t pe049_entry(uint32_t num_pe);
uint32_t pe051_entry(uint32_t num_pe);
uint32_t pe052_entry(uint32_t num_pe);
uint32_t pe053_entry(uint32_t num_pe);
uint32_t pe054_entry(uint32_t num_pe);
uint32_t pe057_entry(uint32_t num_pe);
uint32_t pe058_entry(uint32_t num_pe);
uint32_t pe059_entry(uint32_t num_pe);
uint32_t pe060_entry(uint32_t num_pe);
uint32_t pe061_entry(uint32_t num_pe);
uint32_t pe062_entry(uint32_t num_pe);

#endif

