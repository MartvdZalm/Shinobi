#include "gdt.h"

void SegmentDescriptorInit(SegmentDescriptor *descriptor, uint32_t base, uint32_t limit, uint8_t flags) {
    uint8_t* target = (uint8_t*)descriptor;

    if (limit <= 65536) {
        target[6] = 0x40;
    } else {
        if ((limit & 0xFFF) != 0xFFF) {
            limit = (limit >> 12) - 1;
        } else {
            limit = limit >> 12;
        }

        target[6] = 0xC0;
    }

    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;

    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    target[5] = flags;
}

uint32_t SegmentDescriptorBase(SegmentDescriptor *descriptor) {
    uint8_t* target = (uint8_t*)descriptor;
    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];
    return result;
}

uint32_t SegmentDescriptorLimit(SegmentDescriptor *descriptor) {
    uint8_t* target = (uint8_t*)descriptor;
    uint32_t result = target[6] & 0xF;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    if ((target[6] & 0xC0) == 0xC0) {
        result = (result << 12) | 0xFFF;
    }

    return result;
}

void GlobalDescriptorTableInit(GlobalDescriptorTable *gdt) {
    SegmentDescriptorInit(&(gdt->nullSegmentSelector), 0, 0, 0);
    SegmentDescriptorInit(&(gdt->unusedSegmentSelector), 0, 0, 0);
    SegmentDescriptorInit(&(gdt->codeSegmentSelector), 0, 64 * 1024 * 1024, 0x9A);
    SegmentDescriptorInit(&(gdt->dataSegmentSelector), 0, 64 * 1024 * 1024, 0x92);

    uint32_t i[2];
    i[1] = (uint32_t)gdt;
    i[0] = sizeof(GlobalDescriptorTable) << 16;

    asm volatile("lgdt (%0)": :"p" (((uint8_t *) i) + 2));
}

uint16_t DataSegmentSelector(GlobalDescriptorTable *gdt) {
    return (uint8_t*)&(gdt->dataSegmentSelector) - (uint8_t*)gdt;
}

uint16_t CodeSegmentSelector(GlobalDescriptorTable *gdt) {
    return (uint8_t*)&(gdt->codeSegmentSelector) - (uint8_t*)gdt;
}

