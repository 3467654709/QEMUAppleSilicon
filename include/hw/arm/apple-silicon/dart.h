#ifndef HW_ARM_APPLE_SILICON_DART_H
#define HW_ARM_APPLE_SILICON_DART_H

#include "qemu/osdep.h"
#include "hw/arm/apple-silicon/dt.h"
#include "qom/object.h"

typedef struct AppleDARTState AppleDARTState;

#define TYPE_APPLE_DART "apple.dart"
OBJECT_DECLARE_SIMPLE_TYPE(AppleDARTState, APPLE_DART)

#define TYPE_APPLE_DART_IOMMU_MEMORY_REGION "apple.dart.iommu"
OBJECT_DECLARE_SIMPLE_TYPE(AppleDARTIOMMUMemoryRegion,
                           APPLE_DART_IOMMU_MEMORY_REGION)

#define DART_DART_FORCE_ACTIVE "dart-dart_force_active"
#define DART_DART_REQUEST_SID "dart-dart_request_sid"
#define DART_DART_RELEASE_SID "dart-dart_release_sid"
#define DART_DART_SELF "dart-dart_self"

IOMMUMemoryRegion *apple_dart_iommu_mr(AppleDARTState *dart, uint32_t sid);
IOMMUMemoryRegion *apple_dart_instance_iommu_mr(AppleDARTState *s,
                                                uint32_t instance,
                                                uint32_t sid);
AppleDARTState *apple_dart_from_node(AppleDTNode *node);

#endif /* HW_ARM_APPLE_SILICON_DART_H */
