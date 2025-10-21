#ifndef HW_DMA_APPLE_SIO_H
#define HW_DMA_APPLE_SIO_H

#include "qemu/osdep.h"
#include "hw/arm/apple-silicon/dt.h"
#include "hw/misc/apple-silicon/a7iop/base.h"
#include "hw/sysbus.h"

#define TYPE_APPLE_SIO "apple-sio"
OBJECT_DECLARE_TYPE(AppleSIOState, AppleSIOClass, APPLE_SIO)

typedef struct AppleSIODMAEndpoint AppleSIODMAEndpoint;

uint64_t apple_sio_dma_read(AppleSIODMAEndpoint *ep, void *buffer,
                            uint64_t len);
uint64_t apple_sio_dma_write(AppleSIODMAEndpoint *ep, void *buffer,
                             uint64_t len);
uint64_t apple_sio_dma_remaining(AppleSIODMAEndpoint *ep);
AppleSIODMAEndpoint *apple_sio_get_endpoint(AppleSIOState *s, int ep);
AppleSIODMAEndpoint *
apple_sio_get_endpoint_from_node(AppleSIOState *s, AppleDTNode *node, int idx);
SysBusDevice *apple_sio_from_node(AppleDTNode *node, AppleA7IOPVersion version,
                                  uint32_t protocol);

#endif /* HW_DMA_APPLE_SIO_H */
