#include "qemu/osdep.h"
#include "hw/arm/apple-silicon/dtb.h"
#include "hw/irq.h"
#include "hw/misc/apple-silicon/spmi-baseband.h"
#include "migration/vmstate.h"
#include "qemu/module.h"

#define DEBUG_SPMI_BASEBAND

#ifdef DEBUG_SPMI_BASEBAND
#define DPRINTF(v, ...) fprintf(stderr, v, ##__VA_ARGS__)
#else
#define DPRINTF(v, ...) \
    do {                \
    } while (0)
#endif

#define RREG32(off) ldl_le_p(&s->reg[off])
#define WREG32(off, val) stl_le_p(&s->reg[off], val)
#define WREG32_OR(off, val) WREG32(off, RREG32(off) | val)

void apple_spmi_baseband_set_irq(AppleSPMIBasebandState *s, int value)
{
    if (value) {
        qemu_irq_raise(s->irq);
    } else {
        qemu_irq_lower(s->irq);
    }
}

static int apple_spmi_baseband_send(SPMISlave *s, uint8_t *data, uint8_t len)
{
    AppleSPMIBasebandState *p = APPLE_SPMI_BASEBAND(s);
    // bool aflg = false;
    uint16_t addr;
    DPRINTF("%s: addr 0x%x len 0x%x\n", __func__, p->addr, len);

    for (addr = p->addr; addr < p->addr + len; addr++) {
        p->reg[addr] = data[addr - p->addr];
    }
    p->addr = addr;
    return len;
}

static int apple_spmi_baseband_recv(SPMISlave *s, uint8_t *data, uint8_t len)
{
    AppleSPMIBasebandState *p = APPLE_SPMI_BASEBAND(s);
    uint16_t addr;
    DPRINTF("%s: addr 0x%x len 0x%x\n", __func__, p->addr, len);

    for (addr = p->addr; addr < p->addr + len; addr++) {
        data[addr - p->addr] = p->reg[addr];
    }
    p->addr = addr;
    return len;
}

static int apple_spmi_baseband_command(SPMISlave *s, uint8_t opcode,
                                       uint16_t addr)
{
    AppleSPMIBasebandState *p = APPLE_SPMI_BASEBAND(s);
    p->addr = addr;
    DPRINTF("%s: opcode 0x%x addr 0x%x\n", __func__, opcode, addr);

    switch (opcode) {
    case SPMI_CMD_EXT_READ:
    case SPMI_CMD_EXT_READL:
    case SPMI_CMD_EXT_WRITE:
    case SPMI_CMD_EXT_WRITEL:
        return 0;
    default:
        return 1;
    }
}

DeviceState *apple_spmi_baseband_create(DTBNode *node)
{
    DeviceState *dev = qdev_new(TYPE_APPLE_SPMI_BASEBAND);
    AppleSPMIBasebandState *p = APPLE_SPMI_BASEBAND(dev);
    DTBProp *prop;

    prop = dtb_find_prop(node, "reg");
    g_assert_nonnull(prop);
    spmi_set_slave_sid(SPMI_SLAVE(dev), *(uint32_t *)prop->data);

    qdev_init_gpio_out(dev, &p->irq, 1);
    return dev;
}

static const VMStateDescription vmstate_apple_spmi_baseband = {
    .name = "apple_spmi_baseband",
    .version_id = 0,
    .minimum_version_id = 0,
    .fields =
        (const VMStateField[]){
            VMSTATE_UINT16(addr, AppleSPMIBasebandState),
            VMSTATE_UINT8_ARRAY(reg, AppleSPMIBasebandState, 0xFFFF),
            VMSTATE_END_OF_LIST(),
        }
};

static void apple_spmi_baseband_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    SPMISlaveClass *sc = SPMI_SLAVE_CLASS(klass);

    dc->desc = "Apple SPMI Baseband";
    dc->vmsd = &vmstate_apple_spmi_baseband;

    sc->send = apple_spmi_baseband_send;
    sc->recv = apple_spmi_baseband_recv;
    sc->command = apple_spmi_baseband_command;
}

static const TypeInfo apple_spmi_baseband_type_info = {
    .name = TYPE_APPLE_SPMI_BASEBAND,
    .parent = TYPE_SPMI_SLAVE,
    .instance_size = sizeof(AppleSPMIBasebandState),
    .class_init = apple_spmi_baseband_class_init,
};

static void apple_spmi_baseband_register_types(void)
{
    type_register_static(&apple_spmi_baseband_type_info);
}

type_init(apple_spmi_baseband_register_types)
