/** @file
 * Copyright (c) 2016-2025, Arm Limited or its affiliates. All rights reserved.
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

#include "include/acs_val.h"
#include "include/acs_timer_support.h"
#include "include/acs_wd.h"
#include "include/acs_common.h"


WD_INFO_TABLE  *g_wd_info_table;

/**
  @brief   This API is a single point of entry to retrieve
           information stored in the WD Info table
           1. Caller       -  Test Suite
           2. Prerequisite -  val_wd_create_info_table
  @param   index  watchdog index in the table
  @param   type   the type of information being requested
  @return  64-bit data
**/
uint64_t
val_wd_get_info(uint32_t index, WD_INFO_TYPE_e info_type)
{

  if (g_wd_info_table == NULL)
      return 0;

  switch (info_type) {
  case WD_INFO_COUNT:
      return g_wd_info_table->header.num_wd;
  case WD_INFO_CTRL_BASE:
    return g_wd_info_table->wd_info[index].wd_ctrl_base;
  case WD_INFO_REFRESH_BASE:
    return g_wd_info_table->wd_info[index].wd_refresh_base;
  case WD_INFO_GSIV:
    return g_wd_info_table->wd_info[index].wd_gsiv;
  case WD_INFO_ISSECURE:
    return ((g_wd_info_table->wd_info[index].wd_flags >> 2) & 1);
  case WD_INFO_IS_EDGE:
    return ((g_wd_info_table->wd_info[index].wd_flags) & 1);
  default:
    return 0;
  }
}

/**
  @brief   This API will call PAL layer to fill in the Watchdog information
           into the address pointed by g_wd_info_table pointer.
           1. Caller       -  Application layer.
           2. Prerequisite -  Memory allocated and passed as argument.
  @param   wd_info_table  pre-allocated memory pointer for smmu_info
  @return  Error if Input param is NULL
**/
void
val_wd_create_info_table(uint64_t *wd_info_table)
{

  if (wd_info_table == NULL) {
      val_print(ACS_PRINT_ERR, "Input for Create Info table cannot be NULL\n", 0);
      return;
  }
  val_print(ACS_PRINT_INFO, " Creating WATCHDOG INFO table\n", 0);

  g_wd_info_table = (WD_INFO_TABLE *)wd_info_table;

  pal_wd_create_info_table(g_wd_info_table);

  val_print(ACS_PRINT_TEST, " WATCHDOG_INFO: Number of Watchdogs   : %4d\n",
                                                            val_wd_get_info(0, WD_INFO_COUNT));
}

/**
  @brief  Free the memory allocated for the Watchdog information table

  @param  None

  @return None
**/
void
val_wd_free_info_table(void)
{
    if (g_wd_info_table != NULL) {
        pal_mem_free_aligned((void *)g_wd_info_table);
        g_wd_info_table = NULL;
    }
    else {
      val_print(ACS_PRINT_ERR,
                  "\n WARNING: g_wd_info_table pointer is already NULL",
        0);
    }
}

/**
  @brief   This API Enables watchdog by writing to Control Base register
  @param   index   - identifies the watchdog instance to enable
  @return  None
 **/
static void
val_wd_enable(uint32_t index)
{

  val_mmio_write((g_wd_info_table->wd_info[index].wd_ctrl_base + WD_CS_OFFSET), 1);

}

/**
  @brief   This API disables watchdog by writing to Control Base register
  @param   index   - identifies the watchdog instance to enable
  @return  None
 **/
static void
val_wd_disable(uint32_t index)
{

  val_mmio_write((g_wd_info_table->wd_info[index].wd_ctrl_base + WD_CS_OFFSET), 0);

}

/**
  @brief   This API arms the watchdog by writing to Control Base register
  @param   index   - identifies the watchdog instance to program
  @param   timeout - ticks to generation of ws0 interrupt
  @return  Success/Failure
 **/
uint32_t
val_wd_set_ws0(uint32_t index, uint32_t timeout)
{
  uint64_t counter_freq;
  uint32_t wor_l;
  uint32_t wor_h = 0;
  uint64_t ctrl_base;
  uint32_t data;

  if (timeout == 0) {
      val_wd_disable(index);
      return 0;
  }

  ctrl_base    = val_wd_get_info(index, WD_INFO_CTRL_BASE);

  /* W_IIDR.Architecture Revision [19:16] = 0x1 for Watchdog Rev 1 */
  data = VAL_EXTRACT_BITS(val_mmio_read(ctrl_base + WD_IIDR_OFFSET), 16, 19);

  counter_freq = val_get_counter_frequency();

  /* Check if the timeout value exceeds */
  if (data == 0)
  {
      if ((counter_freq * timeout) >> 32)
      {
          val_print(ACS_PRINT_ERR, "\nCounter frequency value exceeded", 0);
          return 1;
      }
  }

  wor_l = (uint32_t)(counter_freq * timeout);
  wor_h = (uint32_t)((counter_freq * timeout) >> 32);

  val_mmio_write((g_wd_info_table->wd_info[index].wd_ctrl_base + 8), wor_l);

  /* Upper bits are applicable only for WDog Version 1 */
  if (data == 1)
      val_mmio_write((g_wd_info_table->wd_info[index].wd_ctrl_base + 12), wor_h);

  val_wd_enable(index);

  return 0;

}

/**
  @brief   This API is to get counter frequency
  @param   None
  @return  counter frequency
 **/
uint64_t
val_get_counter_frequency(void)
{
  uint64_t counter_freq;

  /* Option to override system counter frequency value */
  counter_freq = pal_timer_get_counter_frequency();
  if (counter_freq == 0)
      counter_freq = val_timer_get_info(TIMER_INFO_CNTFREQ, 0);

  return counter_freq;
}
