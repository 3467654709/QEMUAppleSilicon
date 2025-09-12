/*
 * Apple T8030 SoC (iPhone 11).
 *
 * Copyright (c) 2023-2025 Visual Ehrmanntraut (VisualEhrmanntraut).
 * Copyright (c) 2023-2025 Christian Inci (chris-pcguy).
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HW_ARM_APPLE_SILICON_T8030_H
#define HW_ARM_APPLE_SILICON_T8030_H

#include "qemu/osdep.h"
#include "exec/hwaddr.h"
#include "hw/arm/apple-silicon/a13.h"
#include "hw/arm/apple-silicon/boot.h"
#include "hw/boards.h"
#include "hw/sysbus.h"
#include "hw/usb/tcp-usb.h"
#include "system/kvm.h"

#define TYPE_APPLE_T8030 MACHINE_TYPE_NAME("t8030")

#define APPLE_T8030(obj) \
    OBJECT_CHECK(AppleT8030MachineState, (obj), TYPE_APPLE_T8030)

typedef struct {
    MachineClass parent;
} AppleT8030MachineClass;

typedef enum {
    kBootModeAuto = 0,
    kBootModeManual,
    kBootModeEnterRecovery,
    kBootModeExitRecovery,
} AppleBootMode;

typedef struct {
    MachineState parent;

    hwaddr armio_base;
    hwaddr armio_size;
    unsigned long dram_size;
    AppleA13State *cpus[A13_MAX_CPU];
    AppleA13Cluster clusters[A13_MAX_CLUSTER];
    SysBusDevice *aic;
    MachoHeader64 *kernel;
    AppleDTNode *device_tree;
    uint8_t *trustcache;
    char *securerom;
    gsize securerom_size;
    AppleBootInfo boot_info;
    AppleVideoArgs video_args;
    char *trustcache_filename;
    char *ticket_filename;
    char *sep_rom_filename;
    char *sep_fw_filename;
    char *securerom_filename;
    AppleBootMode boot_mode;
    uint32_t rtkit_protocol_ver;
    uint32_t sio_protocol;
    uint32_t build_version;
    uint64_t ecid;
    Notifier init_done_notifier;
    hwaddr panic_base;
    hwaddr panic_size;
    uint8_t pmgr_reg[0x100000];
    MemoryRegion amcc;
    uint8_t amcc_reg[0x100000];
    bool kaslr_off;
    bool force_dfu;
    uint32_t board_id;
    uint32_t chip_revision;
    USBTCPRemoteConnType usb_conn_type;
    char *usb_conn_addr;
    uint16_t usb_conn_port;
    char *model_number;
    char *region_info;
    char *config_number;
    char *serial_number;
    char *mlb_serial_number;
    char *regulatory_model;
} AppleT8030MachineState;

#endif /* HW_ARM_APPLE_SILICON_T8030_H */
