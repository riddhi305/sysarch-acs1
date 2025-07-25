/** @file
 * Copyright (c) 2020, 2023-2025, Arm Limited or its affiliates. All rights reserved.
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
#ifndef __SMMU_REG_H__
#define __SMMU_REG_H__

#define BITFIELD_DECL(type, name, msb, lsb) \
    const uint32_t name##_SHIFT = lsb; \
    const type name##_MASK = ((((type)0x1) << (msb - lsb + 1)) - 1);

#define BITFIELD_GET(name, val) ((val >> name##_SHIFT) & name##_MASK)
#define BITFIELD_SET(name, val) ((val & name##_MASK) << name##_SHIFT)

#define BYTES_PER_DWORD 8

#define SMMU_QUEUE_OVERFLOW_FLAG        (1U << 31)
#define SMMU_QUEUE_OVF(val)             ((val) & SMMU_QUEUE_OVERFLOW_FLAG)

BITFIELD_DECL(uint32_t, IDR0_ST_LEVEL, 28, 27)
#define SMMU_IDR0_OFFSET 0x0
#define IDR0_ST_LEVEL_2LVL 1
#define IDR0_CD2L   (1 << 19)
#define IDR0_HYP    (1 << 9)
#define IDR0_COHACC (1 << 4)

BITFIELD_DECL(uint32_t, IDR0_TTF, 3, 2)
#define IDR0_TTF_AARCH64 2
#define IDR0_TTF_AARCH32_64 3
#define IDR0_S1P (1 << 1)
#define IDR0_S2P (1 << 0)
#define IDR0_MSI (1 << 13)

#define SMMU_IDR1_OFFSET 0x4
#define IDR1_TABLES_PRESET (1 << 30)
#define IDR1_QUEUES_PRESET (1 << 29)
#define IDR1_REL (1 << 28)
BITFIELD_DECL(uint32_t, IDR1_CMDQS, 25, 21)
BITFIELD_DECL(uint32_t, IDR1_EVNTQS, 20, 16)
BITFIELD_DECL(uint32_t, IDR1_SSIDSIZE, 10, 6)
BITFIELD_DECL(uint32_t, IDR1_SIDSIZE, 5, 0)


#define SMMU_IDR5_OFFSET 0x14
BITFIELD_DECL(uint32_t, IDR5_OAS, 2, 0)
#define SMMU_OAS_MAX_IDX 7
uint32_t smmu_oas[SMMU_OAS_MAX_IDX] = {32, 36, 40, 42, 44, 48, 52};

#define SMMU_CR0_OFFSET 0x20
#define CR0_CMDQEN  (1 << 3)
#define CR0_EVNTQEN (1 << 2)
#define CR0_SMMUEN  (1 << 0)

#define SMMU_CR0ACK_OFFSET 0x24

#define SMMU_CR1_OFFSET 0x28
BITFIELD_DECL(uint32_t, CR1_TABLE_SH, 11, 10)
BITFIELD_DECL(uint32_t, CR1_TABLE_OC, 9, 8)
BITFIELD_DECL(uint32_t, CR1_TABLE_IC, 7, 6)
BITFIELD_DECL(uint32_t, CR1_QUEUE_SH, 5, 4)
BITFIELD_DECL(uint32_t, CR1_QUEUE_OC, 3, 2)
BITFIELD_DECL(uint32_t, CR1_QUEUE_IC, 1, 0)
#define CR1_CACHE_NC 0
#define CR1_CACHE_WB 1
#define CR1_CACHE_WT 2

#define SMMU_CR2_OFFSET     0x2c
#define SMMU_GERROR_OFFSET  0x60
#define SMMU_GERRORN_OFFSET 0x64
#define SMMU_GERROR_EVTQ_ABT_ERR     (1 << 2)
#define SMMU_GERROR_MSI_EVTQ_ABT_ERR (1 << 5)

#define SMMU_STRTAB_BASE_OFFSET 0x80
#define STRTAB_BASE_RA (1UL << 62)
BITFIELD_DECL(uint64_t, STRTAB_BASE_ADDR, 51, 6)

#define SMMU_STRTAB_BASE_CFG_OFFSET 0x88
BITFIELD_DECL(uint32_t, STRTAB_BASE_CFG_FMT, 17, 16)
#define STRTAB_BASE_CFG_FMT_LINEAR 0
#define STRTAB_BASE_CFG_FMT_2LVL 1
BITFIELD_DECL(uint32_t, STRTAB_BASE_CFG_SPLIT, 10, 6)
BITFIELD_DECL(uint32_t, STRTAB_BASE_CFG_LOG2SIZE, 5, 0)

#define SMMU_CMDQ_BASE_OFFSET 0x90
#define SMMU_CMDQ_PROD_OFFSET 0x98
#define SMMU_CMDQ_CONS_OFFSET 0x9c

/* Event Queue */
#define SMMU_EVNTQ_BASE_OFFSET 0xa0
#define SMMU_EVNTQ_PROD_OFFSET 0xa8
#define SMMU_EVNTQ_CONS_OFFSET 0xac

BITFIELD_DECL(uint32_t, EVTQ_0_ID, 7, 0)
BITFIELD_DECL(uint64_t, MSI_MASK, 51, 2)

#define EVT_ID_UUT               0x01
#define EVT_ID_TRANSID_FAULT     0x02
#define EVT_ID_STE_FETCH_FAULT   0x03
#define EVT_ID_BAD_STE           0x04
#define EVT_ID_BAD_ATS_TREQ      0x05
#define EVT_ID_STREAM_DISABLED   0x06
#define EVT_ID_TRANSL_FORBIDDEN  0x07
#define EVT_ID_BAD_SSID          0x08
#define EVT_ID_CD_FETCH_FAULT    0x09
#define EVT_ID_BAD_CD            0x0A
#define EVT_ID_WALK_EABT         0x0B
#define EVT_ID_TRANSLATION_FAULT 0x10
#define EVT_ID_ADDR_SIZE_FAULT   0x11
#define EVT_ID_ACCESS_FAULT      0x12
#define EVT_ID_PERMISSION_FAULT  0x13
#define EVT_ID_TLB_CONFLICT      0x20
#define EVT_ID_CFG_CONFLICT      0x21
#define EVT_ID_PAGE_REQUEST      0x24
#define EVT_ID_VMS_FETCH         0x25

#define SMMU_PAGE1_BASE_OFFSET   0x10000
#define SMMU_SH_ISH              3

#define SMMU_SH_NSH 0
#define SMMU_SH_OSH 2
#define SMMU_SH_ISH 3
#define SMMU_MEMATTR_DEVICE_nGnRE 0x1
#define SMMU_MEMATTR_OIWB 0xf

#define QUEUE_BASE_RWA (1UL << 62)
BITFIELD_DECL(uint64_t, QUEUE_BASE_ADDR, 51, 5)
BITFIELD_DECL(uint64_t, QUEUE_BASE_LOG2SIZE, 4, 0)

#define STRTAB_L1_SZ_SHIFT 20
#define STRTAB_SPLIT       8

#define STRTAB_L1_DESC_DWORDS 1
#define STRTAB_L1_DESC_SIZE   8
BITFIELD_DECL(uint64_t, STRTAB_L1_DESC_SPAN, 4, 0)
BITFIELD_DECL(uint64_t, STRTAB_L1_DESC_L2PTR, 51, 6)

#define STRTAB_STE_DWORDS 8
#define STRTAB_STE_0_V (1UL << 0)
BITFIELD_DECL(uint64_t, STRTAB_STE_0_CONFIG, 3, 1)
#define STRTAB_STE_0_CONFIG_ABORT    0
#define STRTAB_STE_0_CONFIG_BYPASS   4
#define STRTAB_STE_0_CONFIG_S1_TRANS 5
#define STRTAB_STE_0_CONFIG_S2_TRANS 6

BITFIELD_DECL(uint64_t, STRTAB_STE_0_S1FMT, 5, 4)
#define STRTAB_STE_0_S1FMT_LINEAR 0
#define STRTAB_STE_0_S1FMT_64K_L2 2
BITFIELD_DECL(uint64_t, STRTAB_STE_0_S1CONTEXTPTR, 51, 6)
BITFIELD_DECL(uint64_t, STRTAB_STE_0_S1CDMAX, 63, 59)

#define STRTAB_STE_1_S1DSS_SSID0    0x2
#define STRTAB_STE_1_S1C_CACHE_NC   0UL
#define STRTAB_STE_1_S1C_CACHE_WBRA 1UL
BITFIELD_DECL(uint64_t, STRTAB_STE_1_S1DSS, 1, 0)
BITFIELD_DECL(uint64_t, STRTAB_STE_1_S1CIR, 3, 2)
BITFIELD_DECL(uint64_t, STRTAB_STE_1_S1COR, 5, 4)
BITFIELD_DECL(uint64_t, STRTAB_STE_1_S1CSH, 7, 6)
BITFIELD_DECL(uint64_t, STRTAB_STE_1_DCP, 17, 17)
BITFIELD_DECL(uint64_t, STRTAB_STE_1_EATS, 29, 28)
BITFIELD_DECL(uint64_t, STRTAB_STE_1_STRW, 31, 30)
BITFIELD_DECL(uint64_t, STRTAB_STE_1_SHCFG, 45, 44)
#define STRTAB_STE_1_DCP_SHIFT      17
#define STRTAB_STE_1_STRW_NSEL1     0UL
#define STRTAB_STE_1_STRW_EL2       2UL
#define STRTAB_STE_1_SHCFG_INCOMING 1UL

BITFIELD_DECL(uint64_t, STRTAB_STE_2_S2VMID, 15, 0)
BITFIELD_DECL(uint64_t, STRTAB_STE_2_VTCR, 50, 32)
BITFIELD_DECL(uint64_t, STRTAB_STE_2_VTCR_S2T0SZ, 5, 0)
BITFIELD_DECL(uint64_t, STRTAB_STE_2_VTCR_S2SL0, 7, 6)
BITFIELD_DECL(uint64_t, STRTAB_STE_2_VTCR_S2IR0, 9, 8)
BITFIELD_DECL(uint64_t, STRTAB_STE_2_VTCR_S2OR0, 11, 10)
BITFIELD_DECL(uint64_t, STRTAB_STE_2_VTCR_S2SH0, 13, 12)
BITFIELD_DECL(uint64_t, STRTAB_STE_2_VTCR_S2TG, 15, 14)
BITFIELD_DECL(uint64_t, STRTAB_STE_2_VTCR_S2PS, 18, 16)

#define STRTAB_STE_2_S2AA64 (1UL << 51)
#define STRTAB_STE_2_S2PTW  (1UL << 54)
#define STRTAB_STE_2_S2R    (1UL << 58)

BITFIELD_DECL(uint64_t, STRTAB_STE_3_S2TTB, 51, 4)

#define CMDQ_DWORDS_PER_ENT  2
#define EVNTQ_DWORDS_PER_ENT 4
BITFIELD_DECL(uint64_t, CMDQ_0_OP, 7, 0)
BITFIELD_DECL(uint64_t, CMDQ_CFGI_1_RANGE, 4, 0)
#define CMDQ_CFGI_1_ALL_STES 31

#define SMMU_CMDQ_POLL_TIMEOUT 0x100000

#define CDTAB_SPLIT             10
#define CDTAB_L2_ENTRY_COUNT    (1 << CDTAB_SPLIT)

#define CDTAB_L1_DESC_DWORDS    1
#define CDTAB_L1_DESC_V         (1UL << 0)
BITFIELD_DECL(uint64_t, CDTAB_L1_DESC_L2PTR, 51, 12)

#define CDTAB_CD_DWORDS         8
BITFIELD_DECL(uint64_t, CDTAB_CD_0_TCR_T0SZ, 5, 0)
BITFIELD_DECL(uint64_t, CDTAB_CD_0_TCR_TG0, 7, 6)
BITFIELD_DECL(uint64_t, CDTAB_CD_0_TCR_IRGN0, 9, 8)
BITFIELD_DECL(uint64_t, CDTAB_CD_0_TCR_ORGN0, 11, 10)
BITFIELD_DECL(uint64_t, CDTAB_CD_0_TCR_SH0, 13, 12)
#define CDTAB_CD_0_TCR_EPD0    (1ULL << 14)
#define CDTAB_CD_0_TCR_EPD1    (1ULL << 30)

#define CDTAB_CD_0_ENDI        (1UL << 15)
#define CDTAB_CD_0_V           (1UL << 31)

BITFIELD_DECL(uint64_t, CDTAB_CD_0_TCR_IPS, 34, 32)
#define CDTAB_CD_0_AA64        (1UL << 41)
#define CDTAB_CD_0_R           (1UL << 45)
#define CDTAB_CD_0_A           (1UL << 46)
#define CDTAB_CD_0_ASET        (1UL << 47)
BITFIELD_DECL(uint64_t, CDTAB_CD_0_ASID, 63, 48)

BITFIELD_DECL(uint64_t, CDTAB_CD_1_TTB0, 51, 4)

#endif /*__SMMU_REG_H__ */
