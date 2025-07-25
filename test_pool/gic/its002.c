/** @file
 * Copyright (c) 2021, 2023-2025, Arm Limited or its affiliates. All rights reserved.
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
#include "val/include/acs_iovirt.h"

#define TEST_NUM   (ACS_GIC_ITS_TEST_NUM_BASE + 2)
#define TEST_RULE  "ITS_02"
#define TEST_DESC  "Check ITS block association with group"

static
void
payload()
{
  uint32_t status;
  uint32_t index = val_pe_get_index_mpid(val_pe_get_mpid());
  uint32_t num_group, num_blocks, its_id, blk_index;
  uint32_t i, j, k;

  status = val_iovirt_get_its_info(ITS_NUM_GROUPS, 0, 0, &num_group);
  if (status) {
      val_print(ACS_PRINT_ERR, "\n       ITS get group number failed          ", 0);
      val_set_status(index, RESULT_FAIL(TEST_NUM, 1));
      return;
  }
  if (!num_group) {
      val_print(ACS_PRINT_DEBUG, "\n       No ITS group found            ", 0);
      val_set_status(index, RESULT_SKIP(TEST_NUM, 1));
      return;
  }

  val_print(ACS_PRINT_DEBUG, "\n       Number of ITS groups = %d", num_group);

  for (i = 0; i < (num_group - 1); i++) {
      status = val_iovirt_get_its_info(ITS_GROUP_NUM_BLOCKS, i, 0, &num_blocks);
      if (status) {
          val_print(ACS_PRINT_ERR, "\n       ITS get number of blocks failed        ", 0);
          val_set_status(index, RESULT_FAIL(TEST_NUM, 2));
          return;
      }

      if (!num_blocks) {
          val_print(ACS_PRINT_ERR, "\n       No valid ITS Blocks found in group %d  ", i);
          val_set_status(index, RESULT_FAIL(TEST_NUM, 3));
          return;
      }
      val_print(ACS_PRINT_DEBUG, "\n       ITS group index      = %d", i);
      val_print(ACS_PRINT_DEBUG, "\n       Number of ITS Blocks = %d", num_blocks);

      for (j = 0; j < num_blocks; j++) {
          status = val_iovirt_get_its_info(ITS_GET_ID_FOR_BLK_INDEX, i, j, &its_id);
          if (status) {
              val_print(ACS_PRINT_ERR, "\n       ITS get id failed        ", 0);
              val_set_status(index, RESULT_FAIL(TEST_NUM, 4));
              return;
          }

          val_print(ACS_PRINT_DEBUG, "\n       ITS block index  = %d         "
                                            "         ", j);
          for (k = (i + 1); k < num_group; k++) {
              status =
                  val_iovirt_get_its_info(ITS_GET_BLK_INDEX_FOR_ID, k, its_id, &blk_index);
              if (status != ACS_INVALID_INDEX) {
                  val_print(ACS_PRINT_ERR, "\n       ITS ID (%d) repeated in multiple groups",
                            its_id);
                  val_set_status(index, RESULT_FAIL(TEST_NUM, 5));
                  return;
              }
          }
      }
  }
  val_set_status(index, RESULT_PASS(TEST_NUM, 1));
}

uint32_t
its002_entry(uint32_t num_pe)
{
  uint32_t status = ACS_STATUS_FAIL;
  num_pe = 1;  //This ITS test is run on single processor
  status = val_initialize_test(TEST_NUM, TEST_DESC, num_pe);
  if (status != ACS_STATUS_SKIP)
      val_run_test_payload(TEST_NUM, num_pe, payload, 0);
  /* get the result from all PE and check for failure */
  status = val_check_for_error(TEST_NUM, num_pe, TEST_RULE);
  val_report_status(0, ACS_END(TEST_NUM), NULL);

  return status;
}
