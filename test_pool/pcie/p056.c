/** @file
 * Copyright (c) 2019-2025, Arm Limited or its affiliates. All rights reserved.
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

#include "val/include/acs_val.h"
#include "val/include/val_interface.h"

#include "p056_data.h"

#define TEST_NUM   (ACS_PCIE_TEST_NUM_BASE + 56)
#define TEST_DESC  "Check Power management cap rules      "
#define TEST_RULE  "RE_REG_2, IE_REG_5"

static
void
payload(void)
{

  uint32_t pe_index;
  uint32_t ret;
  uint32_t table_entries;

  pe_index = val_pe_get_index_mpid(val_pe_get_mpid());

  table_entries = sizeof(bf_info_table56)/sizeof(bf_info_table56[0]);
  ret = val_pcie_register_bitfields_check((void *)&bf_info_table56, table_entries);

  if (ret == ACS_STATUS_SKIP)
      val_set_status(pe_index, RESULT_SKIP(TEST_NUM, 01));
  else if (ret)
      val_set_status(pe_index, RESULT_FAIL(TEST_NUM, ret));
  else
      val_set_status(pe_index, RESULT_PASS(TEST_NUM, 01));

}

uint32_t
p056_entry(uint32_t num_pe)
{

  uint32_t status = ACS_STATUS_FAIL;

  num_pe = 1;  //This test is run on single processor

  status = val_initialize_test(TEST_NUM, TEST_DESC, num_pe);
  if (status != ACS_STATUS_SKIP)
      val_run_test_payload(TEST_NUM, num_pe, payload, 0);

  /* get the result from all PE and check for failure */
  status = val_check_for_error(TEST_NUM, num_pe, TEST_RULE);

  val_report_status(0, ACS_END(TEST_NUM), TEST_RULE);

  return status;
}
