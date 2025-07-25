/** @file
 * Copyright (c) 2016-2019,2023-2025, Arm Limited or its affiliates. All rights reserved.
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
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ShellLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>


#include "Include/IndustryStandard/Pci.h"
#include "Include/IndustryStandard/Pci22.h"
#include <Protocol/PciIo.h>

#include "include/bsa_pcie_enum.h"
#include "include/pal_uefi.h"


/**
  @brief  Increment the Device number (and Bus number if Dev num reaches 32) to the next valid value.

  @param  StartBdf  Segment/Bus/Dev/Func in the format created by PCIE_CREATE_BDF

  @return the new incremented BDF
**/
UINT32
incrementBusDev(UINT32 StartBdf)
{

  UINT32 Seg = PCIE_EXTRACT_BDF_SEG(StartBdf);
  UINT32 Bus = PCIE_EXTRACT_BDF_BUS(StartBdf);
  UINT32 Dev = PCIE_EXTRACT_BDF_DEV(StartBdf);

  if (Dev != PCI_MAX_DEVICE) {
      Dev++;
  } else {
      Bus++;
      Dev = 0;
  }

  return PCIE_CREATE_BDF(Seg, Bus, Dev, 0);
}

/**
    @brief   Returns the Bus, Dev, Function (in the form seg<<24 | bus<<16 | Dev <<8 | func)
             for a matching class code.

    @param   ClassCode  - is a 32bit value of format ClassCode << 16 | sub_class_code
    @param   StartBdf   - is 0     : start enumeration from Host bridge
                          is not 0 : start enumeration from the input segment, bus, dev
                          this is needed as multiple controllers with same class code are
                          potentially present in a system.
    @return  the BDF of the device matching the class code
**/
UINT32
palPcieGetBdf(UINT32 ClassCode, UINT32 StartBdf)
{

  EFI_STATUS                    Status;
  EFI_PCI_IO_PROTOCOL           *Pci;
  UINTN                         HandleCount;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         Seg, Bus, Dev, Func;
  UINT32                        Index;
  PCI_TYPE_GENERIC              PciHeader;
  PCI_DEVICE_INDEPENDENT_REGION *Hdr;
  UINT32                        ThisBus, InputBus;
  UINT32                        ThisDev, InputDev;


  Status = gBS->LocateHandleBuffer (ByProtocol, &gEfiPciIoProtocolGuid, NULL, &HandleCount, &HandleBuffer);
  if (EFI_ERROR (Status)) {
    acs_print(ACS_PRINT_INFO,L" No PCI devices found in the system\n");
    return EFI_SUCCESS;
  }

  InputBus = PCIE_EXTRACT_BDF_BUS(StartBdf);
  InputDev = PCIE_EXTRACT_BDF_DEV(StartBdf);
  /* Don't care about multi-function devices for now */

  /* These loops are to search for devices in an incremental order of bus numbers, device numbers */
  for (ThisBus = InputBus; ThisBus <= PCI_MAX_BUS; ThisBus++) {
    for (ThisDev = InputDev; ThisDev <= PCI_MAX_DEVICE; ThisDev++) {
      for (Index = 0; Index < HandleCount; Index++) {
        Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiPciIoProtocolGuid, (VOID **)&Pci);
        if (!EFI_ERROR (Status)) {
          Pci->GetLocation (Pci, &Seg, &Bus, &Dev, &Func);
          if (ThisBus != Bus) {
            continue;
          }
          if (ThisDev != Dev) {
            continue;
          }
          Status = Pci->Pci.Read (Pci, EfiPciIoWidthUint32, 0, sizeof (PciHeader)/sizeof (UINT32), &PciHeader);
          if (!EFI_ERROR (Status)) {
            Hdr = &PciHeader.Bridge.Hdr;
            acs_print(ACS_PRINT_INFO,L"\n%03d.%02d.%02d class_code = %d %d", Bus, Dev, Index, Hdr->ClassCode[1], Hdr->ClassCode[2]);
            if (Hdr->ClassCode[2] == ((ClassCode >> 16) & 0xFF)) {
              if (Hdr->ClassCode[1] == ((ClassCode >> 8) & 0xFF)) {
                 /* Found our device */
                 /* Return the BDF   */
                 pal_mem_free(HandleBuffer);
                 return (UINT32)(PCIE_CREATE_BDF(Seg, Bus, Dev, Func));
              }
            }
          }
        }
      }
    }
  }

  pal_mem_free(HandleBuffer);
  return 0;
}

/**
    @brief   Returns the Bus, Dev, Function (in the form seg<<24 | bus<<16 | Dev <<8 | func)
             for a matching class code.

    @param   ClassCode  - is a 32bit value of format ClassCode << 16 | sub_class_code
    @param   StartBdf   - is 0     : start enumeration from Host bridge
                          is not 0 : start enumeration from the input segment, bus, dev
                          this is needed as multiple controllers with same class code are
                          potentially present in a system.
    @return  the BDF of the device matching the class code
**/
UINT32
pal_pcie_get_bdf_wrapper (
  UINT32 ClassCode,
  UINT32 StartBdf
  )
{

  return palPcieGetBdf(ClassCode, StartBdf);
}

/**
  @brief  This API returns the Base Address Register value for a given BDF and index
  @param  bdf       - the device whose PCI Config space BAR needs to be returned.
  @param  bar_index - the '0' based BAR index identifying the correct 64-bit BAR

  @return the 64-bit BAR value
*/
UINT64
palPcieGetBase(UINT32 bdf, UINT32 bar_index)
{

  EFI_STATUS                    Status;
  EFI_PCI_IO_PROTOCOL           *Pci;
  UINTN                         HandleCount;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         Seg, Bus, Dev, Func;
  UINT32                        Index;
  PCI_TYPE_GENERIC              PciHeader;
  PCI_DEVICE_HEADER_TYPE_REGION *Device;
  UINT32                        InputSeg, InputBus, InputDev, InputFunc;
  UINT64                        bar_value;

  Status = gBS->LocateHandleBuffer (ByProtocol, &gEfiPciIoProtocolGuid, NULL, &HandleCount, &HandleBuffer);
  if (EFI_ERROR (Status)) {
    acs_print(ACS_PRINT_INFO,L" No PCI devices found in the system\n");
    return EFI_SUCCESS;
  }

  InputSeg = (bdf >> 24) & 0xFF;
  InputBus = (bdf >> 16) & 0xFF;
  InputDev = (bdf >> 8)  & 0xFF;
  InputFunc = bdf & 0xFF;

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiPciIoProtocolGuid, (VOID **)&Pci);
    if (!EFI_ERROR (Status)) {
      Pci->GetLocation (Pci, &Seg, &Bus, &Dev, &Func);

      if ((Seg == InputSeg) && (Bus == InputBus) && (Dev == InputDev) && (Func == InputFunc)) {
          Status = Pci->Pci.Read (Pci, EfiPciIoWidthUint32, 0, sizeof (PciHeader)/sizeof (UINT32), &PciHeader);
          if (!EFI_ERROR (Status)) {
            Device = &PciHeader.Device.Device;
            pal_mem_free(HandleBuffer);
            if ((((Device->Bar[bar_index]) >> BAR_MDT_SHIFT) & BAR_MDT_MASK) == BITS_64)
            {
                bar_value = Device->Bar[bar_index + 1];
                bar_value = (bar_value << 32) | (Device->Bar[bar_index]);
                return bar_value;
            }

            else
                return (Device->Bar[bar_index]);
        }
      }

    }
  }

  pal_mem_free(HandleBuffer);
  return 0;
}

/**
  @brief Returns the PCI device structure for the given bdf

  @param Bdf PCI Bus, Device, and Function

  @return Pointer to device structure for the input bdf
**/
VOID *
pal_pci_bdf_to_dev (
 UINT32 Bdf
  )
{
  return NULL;
}

/**
  @brief Reads 1 byte from the PCI configuration space for the current BDF at given offset.

  @param Bdf PCI Bus, Device, and Function
  @param Offset offset in the PCI configuration space for that BDF
  @param Val return value

  @return None
**/
VOID
pal_pci_read_config_byte (
  UINT32 Bdf,
  UINT8 Offset,
  UINT8 *Val
  )
{

}

/**
  @brief This API performs the PCI enumeration

  @param None

  @return None
**/
VOID pal_pcie_enumerate(VOID)
{
  /**Implemented functionality only for Baremetal support
   * For uefi, enumeration is done during bootup
   * Hence, not implemented for uefi.*/
}

/**
  @brief  This API is used as placeholder to check if the bdf
          obtained is valid or not

  @param  bdf
  @return 0 if bdf is valid else 1
**/
UINT32
pal_pcie_check_device_valid(UINT32 bdf)
{

  /*Add BDFs to this function if PCIe tests
    need to be ignored for a BDF for any reason
  */

  return 0;
}

/**
  @brief  This API is used as placeholder to check if the bus
          is to be skipped for PCIe table creation

  @param  bus_index
  @return 1 if bus to be skipped else 0
**/
UINT32
pal_pcie_check_bus_valid(UINT32 bus_index)
{
  /* Add Bus index to this function if PCIe table
     creation need to be ignored for a bus
  */

  return 0;
}
